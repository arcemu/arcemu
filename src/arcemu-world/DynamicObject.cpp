/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
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
	m_uint32Values[OBJECT_FIELD_GUID] = low;
	m_uint32Values[OBJECT_FIELD_GUID+1] = high;
	m_wowGuid.Init(GetGUID());
	m_floatValues[OBJECT_FIELD_SCALE_X] = 1;


	m_parentSpell=NULL;
	m_aliveDuration = 0;
	u_caster = 0;
	m_spellProto = 0;
	p_caster = 0;
}

DynamicObject::~DynamicObject()
{
	if(u_caster->dynObj == this)
		u_caster->dynObj = 0;
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

	m_uint32Values[OBJECT_FIELD_ENTRY] = m_spellProto->Id;
	m_uint32Values[DYNAMICOBJECT_BYTES] = 0x01eeeeee;
	m_uint32Values[DYNAMICOBJECT_SPELLID] = m_spellProto->Id;

	m_floatValues[DYNAMICOBJECT_RADIUS] = radius;
	m_floatValues[DYNAMICOBJECT_POS_X]  = x;
	m_floatValues[DYNAMICOBJECT_POS_Y]  = y;
	m_floatValues[DYNAMICOBJECT_POS_Z]  = z;

	m_aliveDuration = duration;
	u_caster = caster;
	m_faction = caster->m_faction;
	m_factionDBC = caster->m_factionDBC;

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
  
	sEventMgr.AddEvent(this, &DynamicObject::UpdateTargets, EVENT_DYNAMICOBJECT_UPDATE, 100, 0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
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
		std::set<Object*>::iterator itr = GetInRangeSetBegin(),itr2;
		std::set<Object*>::iterator iend = GetInRangeSetEnd();
		Unit * target;
		Aura * pAura;
		float radius = m_floatValues[DYNAMICOBJECT_RADIUS]*m_floatValues[DYNAMICOBJECT_RADIUS];

		this->AquireInrangeLock(); //make sure to release lock before exit function !
		while(itr != iend)
		{
//			target = *itr;
//			++itr;

			itr2 = itr;
			++itr;

			if( !( (*itr2)->IsUnit() ) || ! static_cast< Unit* >( *itr2 )->isAlive() || ( static_cast< Creature* >( *itr2 )->IsTotem() && !static_cast< Unit* >( *itr2 )->IsPlayer() ) )
				continue;

			target = static_cast< Unit* >( *itr2 );

			if( !isAttackable( p_caster, target, !(m_spellProto->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED) ) )
				continue;

			// skip units already hit, their range will be tested later
			if(targets.find(target->GetGUID()) != targets.end())
				continue;

			if(GetDistanceSq(target) <= radius)
			{
				pAura = AuraPool.PooledNew();
				pAura->Init(m_spellProto, m_aliveDuration, u_caster, target);
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

		this->ReleaseInrangeLock();
		// loop the targets, check the range of all of them
		DynamicObjectList::iterator jtr  = targets.begin();
		DynamicObjectList::iterator jtr2;
		DynamicObjectList::iterator jend = targets.end();
		
		while(jtr != jend)
		{
			target = GetMapMgr() ? GetMapMgr()->GetUnit(*jtr) : NULL;
			jtr2 = jtr;
			++jtr;

			if(GetDistanceSq(target) > radius)
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
	DynamicObjectList::iterator jtr  = targets.begin();
	DynamicObjectList::iterator jend = targets.end();
	Unit * target;

	while(jtr != jend)
	{
		target = GetMapMgr() ? GetMapMgr()->GetUnit(*jtr) : NULL;
		++jtr;
		if (target != NULL)
			target->RemoveAura(m_spellProto->Id);
	}

	if(IsInWorld())
		RemoveFromWorld(true);
	delete this;
}



