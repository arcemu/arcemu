/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2010 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "StdAfx.h"

DynamicObject::DynamicObject(uint32 high, uint32 low)
{
	m_objectTypeId = TYPEID_DYNAMICOBJECT;
	m_valuesCount = DYNAMICOBJECT_END;
	m_uint32Values = _fields;
	memset(m_uint32Values, 0,(DYNAMICOBJECT_END)*sizeof(uint32));
	m_updateMask.SetCount(DYNAMICOBJECT_END);
	m_uint32Values[OBJECT_FIELD_TYPE] = TYPE_DYNAMICOBJECT|TYPE_OBJECT;
	m_uint32Values[LOWGUID] = low;
	m_uint32Values[HIGHGUID] = high;
	m_wowGuid.Init(GetGUID());
	SetScale( 1 );


	m_parentSpell= NULL;
	m_aliveDuration = 0;
	u_caster = NULL;
	m_spellProto = NULL;
	p_caster = NULL;
}

DynamicObject::~DynamicObject()
{
	if(u_caster != NULL && u_caster->dynObj == this)
		u_caster->dynObj = NULL;
}

void DynamicObject::Create(Unit * caster, Spell * pSpell, float x, float y, float z, uint32 duration, float radius)
{
	Object::_Create(caster->GetMapId(),x, y, z, 0);
	if(pSpell->g_caster)
	{
		m_parentSpell = pSpell;
	}
	if( pSpell->p_caster == NULL )
	{
		// try to find player caster here
		if( caster->IsPlayer() )
			p_caster = static_cast<Player*>( caster );
	}
	else
		p_caster = pSpell->p_caster;

	m_spellProto = pSpell->GetProto();
	SetUInt64Value(DYNAMICOBJECT_CASTER, caster->GetGUID());

	SetEntry( m_spellProto->Id );
	m_uint32Values[DYNAMICOBJECT_BYTES] = 0x01eeeeee;
	m_uint32Values[DYNAMICOBJECT_SPELLID] = m_spellProto->Id;

	m_floatValues[DYNAMICOBJECT_RADIUS] = radius;
	m_position.x = x; //m_floatValues[DYNAMICOBJECT_POS_X]  = x;
	m_position.y = y; //m_floatValues[DYNAMICOBJECT_POS_Y]  = y;
	m_position.z = z; //m_floatValues[DYNAMICOBJECT_POS_Z]  = z;
	

	m_aliveDuration = duration;
	u_caster = caster;
	m_faction = caster->m_faction;
	m_factionDBC = caster->m_factionDBC;
	m_phase = caster->GetPhase();

	if(pSpell->g_caster)
		PushToWorld(pSpell->g_caster->GetMapMgr());
	else 
		PushToWorld(caster->GetMapMgr());

	if(caster->dynObj != NULL)
	{
		//expires
		caster->dynObj->Remove();
	}
	caster->dynObj = this;
  
	//sEventMgr.AddEvent(this, &DynamicObject::UpdateTargets, EVENT_DYNAMICOBJECT_UPDATE, 100, 0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
    UpdateTargets();
}

void DynamicObject::AddInRangeObject( Object* pObj )
{
	Object::AddInRangeObject( pObj );
}

void DynamicObject::OnRemoveInRangeObject( Object* pObj )
{
	if( pObj->IsUnit() )
	{
		targets.erase( pObj->GetGUID() );
	}
	Object::OnRemoveInRangeObject( pObj );
}

void DynamicObject::UpdateTargets()
{
	if(m_aliveDuration == 0)
		return;

	if(m_aliveDuration >= 100)
	{
		Unit * target;
		Aura * pAura;

		float radius = m_floatValues[ DYNAMICOBJECT_RADIUS ] * m_floatValues[ DYNAMICOBJECT_RADIUS ];

        // Looking for targets in the Object set
        for( std::set< Object* >::iterator itr = m_objectsInRange.begin(); itr != m_objectsInRange.end(); ++itr )
		{
            Object *o = *itr;

			if( !o->IsUnit() || !static_cast< Unit* >( o )->isAlive() )
				continue;

			target = static_cast< Unit* >( o );

			if( !isAttackable( u_caster, target, !(m_spellProto->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED) ) )
				continue;

			// skip units already hit, their range will be tested later
			if(targets.find(target->GetGUID()) != targets.end())
				continue;

			if(GetDistanceSq(target) <= radius)
			{
				pAura = new Aura(m_spellProto, m_aliveDuration, u_caster, target, true);
				for(uint32 i = 0; i < 3; ++i)
				{
					if(m_spellProto->Effect[i] == SPELL_EFFECT_PERSISTENT_AREA_AURA)
					{
						pAura->AddMod(m_spellProto->EffectApplyAuraName[i],
							m_spellProto->EffectBasePoints[i]+1, m_spellProto->EffectMiscValue[i], i);
					}
				}
				target->AddAura(pAura);
				if(p_caster)
				{
					p_caster->HandleProc(PROC_ON_CAST_SPECIFIC_SPELL | PROC_ON_CAST_SPELL,target, m_spellProto);
					p_caster->m_procCounter = 0;
				}

				// add to target list
				targets.insert(target->GetGUID());
			}
		}

       
		// loop the targets, check the range of all of them
		DynamicObjectList::iterator jtr  = targets.begin();
		DynamicObjectList::iterator jtr2;
		DynamicObjectList::iterator jend = targets.end();
		
		while(jtr != jend)
		{
			target = GetMapMgr() ? GetMapMgr()->GetUnit(*jtr) : NULL;
			jtr2 = jtr;
			++jtr;

			if((target != NULL) && (GetDistanceSq(target) > radius))
			{
				target->RemoveAura(m_spellProto->Id);
				targets.erase(jtr2);
			}
		}

		m_aliveDuration -= 100;
	}
	else
	{
		m_aliveDuration = 0;
	}

	if(m_aliveDuration == 0)
	{
		Remove();
	}
}

void DynamicObject::Remove()
{
	// remove aura from all targets
	Unit * target;

    if( !IsInWorld() ){
        delete this;
        return;
    }

    for( std::set< uint64 >::iterator itr = targets.begin(); itr != targets.end(); ++itr )
	{

        uint64 TargetGUID = *itr;

        target = m_mapMgr->GetUnit( TargetGUID );

        if (target != NULL)
			target->RemoveAura(m_spellProto->Id);
	}

	WorldPacket data( SMSG_GAMEOBJECT_DESPAWN_ANIM, 8 );
	
	data << GetGUID();
	SendMessageToSet( &data, false );

	if( IsInWorld() )
		RemoveFromWorld(true);

	if (u_caster != NULL && m_spellProto->ChannelInterruptFlags != 0)
	{
		u_caster->SetChannelSpellTargetGUID(0);
		u_caster->SetChannelSpellId(0);
	}

	delete this;
}



