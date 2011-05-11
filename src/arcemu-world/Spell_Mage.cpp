/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2011 <http://www.ArcEmu.org/>
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

class MirrorImageSpell : public Spell
{
	SPELL_FACTORY_FUNCTION(MirrorImageSpell);

	void SpellEffectSummon(uint32 i)
	{
		CreatureInfo * ci = CreatureNameStorage.LookupEntry(GetProto()->EffectMiscValue[i]);
		CreatureProto * cp = CreatureProtoStorage.LookupEntry(GetProto()->EffectMiscValue[i]);

		if( ci == NULL || cp == NULL )
			return;

		AI_Spell* sp1;
		AI_Spell* sp2;
		SpellRange *range;

		sp1 = new AI_Spell;
		sp1->entryId = 59638;
		sp1->spell = dbcSpell.LookupEntryForced(sp1->entryId);
		sp1->spellType = STYPE_DAMAGE;
		sp1->agent = AGENT_SPELL;
		sp1->spelltargetType = TTYPE_SINGLETARGET;
		sp1->cooldown = 0;
		sp1->cooldowntime = 0;
		sp1->Misc2 = 0;
		sp1->procCount = 0;
		sp1->procChance = 100;
		range = dbcSpellRange.LookupEntry(sp1->spell->rangeIndex);
		sp1->minrange = GetMinRange(range);
		sp1->maxrange = GetMaxRange(range);		

		sp2 = new AI_Spell;
		sp2->entryId = 59637;
		sp2->spell = dbcSpell.LookupEntryForced(sp2->entryId);
		sp2->spellType = STYPE_DAMAGE;
		sp2->agent = AGENT_SPELL;
		sp2->spelltargetType = TTYPE_SINGLETARGET;
		sp2->cooldown = 0;
		sp2->cooldowntime = 0;
		sp2->Misc2 = 0;
		sp2->procCount = 0;
		sp2->procChance = 100;
		range = dbcSpellRange.LookupEntry(sp2->spell->rangeIndex);
		sp2->minrange = GetMinRange(range);
		sp2->maxrange = GetMaxRange(range);

		uint8 count = 1;

		// Glyph of Mirror Image - add an extra copy
		if( GetProto()->Id == 58833 && p_caster->HasAura( 63093 ) )
			count = 2;

		for( uint8 j = 0; j < count; ++j )
		{
			float m_followAngle = -M_PI_FLOAT * 2 * RandomFloat(10);
			float x = p_caster->GetPositionX() + 3 * ( cosf( m_followAngle + p_caster->GetOrientation() ) );
			float y = p_caster->GetPositionY() + 3 * ( sinf( m_followAngle + p_caster->GetOrientation() ) );
			float z = p_caster->GetPositionZ();

			Creature * pCreature = p_caster->GetMapMgr()->CreateCreature( ci->Id );

			pCreature->Load( cp, x, y, z );

			pCreature->SetInstanceID( p_caster->GetMapMgr()->GetInstanceID() );
			pCreature->SetZoneId( p_caster->GetZoneId() );
			pCreature->Phase( PHASE_SET, p_caster->GetPhase() );
			pCreature->SetFaction( p_caster->GetFaction() );
			pCreature->setLevel( p_caster->getLevel() );

			pCreature->SetMaxHealth( pCreature->GetMaxHealth() + 30 * p_caster->getLevel() +28 );
			pCreature->SetHealth( pCreature->GetMaxHealth() );

			pCreature->SetPowerType( p_caster->GetPowerType() );
			pCreature->SetMaxPower( pCreature->GetPowerType(), p_caster->GetMaxPower( p_caster->GetPowerType() ) );
			pCreature->SetPower( pCreature->GetPowerType(), pCreature->GetMaxPower( pCreature->GetPowerType() ) );  
			pCreature->SetAttackPower( p_caster->GetAttackPower() / 3 );

			p_caster->SetUInt32Value(PLAYER_PET_SPELL_POWER, uint32(p_caster->GetUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS + SCHOOL_FROST) * 0.33f));

			p_caster->AddGuardianRef( pCreature );
			pCreature->SetOwner( p_caster );

			pCreature->SetSummonedByGUID( p_caster->GetGUID() );
			pCreature->SetCreatedByGUID( p_caster->GetGUID() );
			pCreature->SetCreatedBySpell( GetProto()->Id );

			pCreature->setRace( p_caster->getRace() );
			pCreature->setGender( p_caster->getGender() );
			pCreature->setClass( p_caster->getClass() );

			pCreature->SetUInt32Value( UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED );
			pCreature->SetUInt32Value( UNIT_FIELD_FLAGS_2, UNIT_FLAG_SELF_RES );
			pCreature->SetUInt32Value( UNIT_FIELD_BYTES_2, 1 );

			pCreature->GetAIInterface()->Init( pCreature, AITYPE_PET, MOVEMENTTYPE_NONE, p_caster );
			pCreature->GetAIInterface()->SetUnitToFollow( p_caster );
			pCreature->GetAIInterface()->SetUnitToFollowAngle( float( (M_PI/4)) );
			pCreature->GetAIInterface()->SetFollowDistance( GetRadius( i ) );

			pCreature->GetAIInterface()->addSpellToList( sp1 );
			pCreature->GetAIInterface()->addSpellToList( sp2 );

			if( p_caster->IsPvPFlagged() )
				pCreature->SetPvPFlag();
			else
				pCreature->RemovePvPFlag();

			pCreature->PushToWorld(p_caster->GetMapMgr());
       
			p_caster->CastSpell( pCreature->GetGUID(), 45204, false );
			p_caster->CastSpell( pCreature->GetGUID(), 58838, false );
       
			sEventMgr.AddEvent( pCreature, &Creature::RemoveFromWorld, false, true, EVENT_CREATURE_REMOVE_CORPSE, GetDuration(), 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
		}

		delete sp1;
		delete sp2;
	}
};

void SpellFactoryMgr::SetupMage()
{
	AddSpellById( 58831, &MirrorImageSpell::Create );
	AddSpellById( 58833, &MirrorImageSpell::Create );
	AddSpellById( 58834, &MirrorImageSpell::Create );
}
