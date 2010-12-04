/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008-2009 Arcemu Team
 * Copyright (C) 2007 Moon++ <http://www.moonplusplus.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Setup.h"

#define BLOOD_PLAGUE 55078
#define FROST_FEVER 55095

bool Pestilence(uint32 i, Spell* pSpell)
{
	if(i == 1) // Script Effect that has been identified to handle the spread of diseases.
	{
		if(!pSpell->u_caster || !pSpell->u_caster->GetTargetGUID() || !pSpell->u_caster->IsInWorld())
			return true;

		Unit* u_caster = pSpell->u_caster;
		Unit* Main = u_caster->GetMapMgr()->GetUnit(u_caster->GetTargetGUID());
		if(Main == NULL)
			return true;
		bool blood = Main->HasAura(BLOOD_PLAGUE);
		bool frost = Main->HasAura(FROST_FEVER);
		int inc = (u_caster->HasAura(59309)?10:5);
		for (Object::InRangeSet::iterator itr = u_caster->GetInRangeSetBegin(); itr != u_caster->GetInRangeSetEnd(); ++itr)
		{
			if(!(*itr)->IsUnit())
				continue;
			Unit* Target = static_cast<Unit*>((*itr));
			if(Main->GetGUID() == Target->GetGUID() && !u_caster->HasAura(63334))
				continue;
			if(isAttackable(Target, u_caster) && u_caster->CalcDistance((*itr)) <= (pSpell->GetRadius(i) + inc))
			{
				if(blood)
					u_caster->CastSpell(Target, BLOOD_PLAGUE, true);
				if(frost)
					u_caster->CastSpell(Target, FROST_FEVER, true);
			}
		}
		return true;
	}
	return true;
}

bool DeathStrike(uint32 i, Spell* pSpell)
{
	if(!pSpell->u_caster || !pSpell->GetUnitTarget())
		return true;

	Unit* Target = pSpell->GetUnitTarget();
	int count = 0;
	if(Target->HasAura(BLOOD_PLAGUE))
		count++;
	if(Target->HasAura(FROST_FEVER))
		count++;
	float pct = (pSpell->u_caster->GetMaxHealth() * 0.05f);
	if(count > 0)
		pSpell->u_caster->Heal(pSpell->u_caster, pSpell->GetProto()->Id, float2int32(pct*count));
	return true;
}

class ArmyofDeadGhoul : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ArmyofDeadGhoul);
	ArmyofDeadGhoul(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->GetAIInterface()->m_canMove = false;
	}

	void OnLoad()
	{
		RegisterAIUpdateEvent(200);
	}

	void AIUpdate()
	{
		_unit->CastSpell(_unit->GetGUID(), 20480, false);		
		RemoveAIUpdateEvent();
		_unit->GetAIInterface()->m_canMove = true;
	}

};

bool Strangulate( uint32 i, Aura * pAura, bool apply ){
	if( !apply )
		return true;

	if( !pAura->GetTarget()->IsPlayer() )
		return true;

	Unit *unitTarget = pAura->GetTarget();

	if( unitTarget->IsCasting() )
		unitTarget->InterruptSpell();

	return true;
}

bool RuneTap( uint32 i, Spell *s ){
	if( s->p_caster == NULL )
		return false;
	
	s->p_caster->SetHealth(( uint32 )(( s->p_caster->GetHealth())+( s->p_caster->GetMaxHealth()) / 10 ));

	return true;
}

bool RaiseDead( uint32 i, Spell *s ){
	if( s->p_caster == NULL )
		return false;
	
	float x = s->p_caster->GetPositionX()+rand()%25;
	float y = s->p_caster->GetPositionY()+rand()%25;
	float z = s->p_caster->GetPositionZ();

	Corpse *corpseTarget = s->GetCorpseTarget();
	
	if( corpseTarget != NULL ) // We are targeting a corpse.
	{
		x = corpseTarget->GetPositionX();
		y = corpseTarget->GetPositionY();
		z = corpseTarget->GetPositionZ();
	}
		
		if( s->p_caster->HasSpellwithNameHash( SPELL_HASH_MASTER_OF_GHOULS ) ){
			SpellEntry *ghoulSpell = dbcSpell.LookupEntryForced(52150);
			if( ghoulSpell != NULL )
				s->p_caster->CastSpellAoF(x,y,z,ghoulSpell,true );
		
		}else{
			LocationVector vec( x, y, z );
			Creature *c = s->p_caster->create_guardian( 26125, 3*60*1000, float( M_PI ), s->p_caster->getLevel(), NULL, &vec );
			c->CastSpell( c->GetGUID(), 50142, true ); // emerge
		}

	return true;
}

bool DeathGrip( uint32 i, Spell *s ){
	Unit *unitTarget = s->GetUnitTarget();

	if(!s->u_caster || !s->u_caster->isAlive() || !unitTarget || !unitTarget->isAlive())
		return false;
	
	// rooted units can't be death gripped
	if( unitTarget->isRooted() )
		return false;
	
	if(unitTarget->IsPlayer())
	{
		Player *playerTarget = static_cast<Player*>(unitTarget);
		
		if(playerTarget->m_CurrentTransporter) // Blizzard screwed this up, so we won't.
			return false;
		
		s->SpellEffectPlayerPull( i );
		
		return false;

	}else{
		float posX, posY, posZ;
		float deltaX,deltaY;
		
		if( s->u_caster->GetPositionX() == 0.0f || s->u_caster->GetPositionY() == 0.0f)
			return false;
		
		deltaX = s->u_caster->GetPositionX()-unitTarget->GetPositionX();
		deltaY = s->u_caster->GetPositionY()-unitTarget->GetPositionY();
		
		if(deltaX == 0.0f || deltaY == 0.0f)
			return false;
		
		float d = sqrt(deltaX*deltaX+deltaY*deltaY) - s->u_caster->GetBoundingRadius()-unitTarget->GetBoundingRadius();
		
		float alpha = atanf(deltaY/deltaX);
		
		if(deltaX<0)
			alpha += float(M_PI);
		
		posX = d*cosf(alpha)+unitTarget->GetPositionX();
		posY = d*sinf(alpha)+unitTarget->GetPositionY();
		posZ = s->u_caster->GetPositionZ();
		
		uint32 time = uint32( (unitTarget->CalcDistance( s->m_caster) / ((unitTarget->m_runSpeed * 3.5) * 0.001f)) + 0.5);
		
		WorldPacket data(SMSG_MONSTER_MOVE, 60);
		data << unitTarget->GetNewGUID();
		data << uint8(0); //VLack: the usual change in SMSG_MONSTER_MOVE packets, initial idea from Mangos
		data << unitTarget->GetPositionX();
		data << unitTarget->GetPositionY();
		data << unitTarget->GetPositionZ();
		data << getMSTime();
		data << uint8(0x00);
		data << uint32(0x00001000);
		data << time;
		data << uint32(1);
		data << posX << posY << posZ;
		
		if(unitTarget->IsCreature())
			unitTarget->GetAIInterface()->StopMovement(2000);
		
		unitTarget->SendMessageToSet(&data, true);
		unitTarget->SetPosition(posX,posY,posZ,alpha,true);
		unitTarget->addStateFlag(UF_ATTACKING);
		unitTarget->smsg_AttackStart( unitTarget );
		unitTarget->setAttackTimer(time, false);
		unitTarget->setAttackTimer(time, true);
		unitTarget->GetAIInterface()->taunt( s->u_caster,true);
	}

	return true;
}

bool BloodWorms( uint32 i, Spell *s ){
	
	Unit *unitTarget = s->GetUnitTarget();

	if( !s->u_caster || !s->u_caster->isAlive() || !unitTarget )
		return false;
	
	uint32 cnt = (2 + RandomUInt(2)); // 2 - 4 worms
	for(uint8 j = 0; j < cnt; j++)
	{
		s->SpellEffectSummon(i);
	}
	
	return true;
}

bool DeathCoil( uint32 i, Spell *s ){
	Unit *unitTarget = s->GetUnitTarget();

	if( s->u_caster == NULL || unitTarget == NULL )
		return false;
	
	if( s->GetProto()->Id == 52375 )
		s->damage = s->damage * 2 / 5;
	
	if( isAttackable( s->u_caster, unitTarget, false ) )
		s->u_caster->SpellNonMeleeDamageLog( unitTarget, s->GetProto()->Id, s->damage, true );
	else if( unitTarget->IsCreature() )
	{
		CreatureInfo * ci = static_cast< Creature* >( unitTarget )->GetCreatureInfo();
		if( ci->Type == UNIT_TYPE_UNDEAD )
			s->u_caster->Heal( unitTarget, s->GetProto()->Id, float2int32( s->damage * 1.5f ) );
	}

	return true;
}

bool BladedArmor( uint32 i, Aura *a, bool apply ){
	Unit *m_target = a->GetTarget();

	int32 realamount = 0;

	uint32 mod1 = m_target->GetResistance(SCHOOL_NORMAL);
	uint32 mod2 = a->m_spellProto->EffectBasePoints[0] + 1; //Thanks Andy for pointing out that BasePoints
	uint32 mod3 = a->m_spellProto->EffectBasePoints[1] + 1; //Should always be used instead of static modifiers.
	realamount = ( a->GetModAmount( i ) + ( mod1 / mod3 ) * mod2 );

	if(apply)
		m_target->ModAttackPowerMods( realamount );
	else
		m_target->ModAttackPowerMods( -realamount );
	
	m_target->CalcDamage();

	return true;
}


void SetupDeathKnightSpells(ScriptMgr * mgr)
{
	mgr->register_creature_script(24207, &ArmyofDeadGhoul::Create);
    mgr->register_dummy_spell(50842, &Pestilence);
	uint32 DeathStrikeIds[] =
	{
		49998, // Rank 1
		49999, // Rank 2
		45463, // Rank 3
		49923, // Rank 4
		49924, // Rank 5
		0,
	};
	mgr->register_dummy_spell(DeathStrikeIds, &DeathStrike);

	
	mgr->register_dummy_aura( 47476, &Strangulate );
	mgr->register_dummy_aura( 49913, &Strangulate );
	mgr->register_dummy_aura( 49914, &Strangulate );
	mgr->register_dummy_aura( 49915, &Strangulate );
	mgr->register_dummy_aura( 49916, &Strangulate );

	mgr->register_dummy_spell( 48982, &RuneTap );
	mgr->register_dummy_spell( 46584, &RaiseDead );
	mgr->register_dummy_spell( 49576, &DeathGrip );
	mgr->register_dummy_spell( 50452, &BloodWorms );

	mgr->register_dummy_spell( 52375, &DeathCoil );
	mgr->register_dummy_spell( 49892, &DeathCoil );
	mgr->register_dummy_spell( 49893, &DeathCoil );
	mgr->register_dummy_spell( 49894, &DeathCoil );
	mgr->register_dummy_spell( 49895, &DeathCoil );

	uint32 bladedarmorids[] = {
		48978,
		49390,
		49391,
		49392,
		49393,
		0
	};
	mgr->register_dummy_aura( bladedarmorids, &BladedArmor );
}