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

bool Starfall(uint32 i, Spell* pSpell)
{
	Unit* m_caster = pSpell->u_caster;
	if( m_caster == NULL )
		return true;
	uint8 am = 0;
	for (Object::InRangeSet::iterator itr = m_caster->GetInRangeSetBegin(); itr != m_caster->GetInRangeSetEnd(); ++itr)
	{
		if(!(*itr)->IsUnit())
			continue;
		Unit* Target = TO< Unit* >((*itr));
		if(isAttackable(Target, m_caster) && m_caster->CalcDistance((*itr)) <= pSpell->GetRadius(i))
		{
			m_caster->CastSpell(Target, pSpell->CalculateEffect(i, Target), true);
			++am;
			if(am >= 2)
				return true;
		}
	}
	return true;
}

bool ImprovedLeaderOfThePack( uint32 i, Spell *s ){
	if ( s->p_caster == NULL )
		return false;
	
	s->p_caster->AddProcTriggerSpell(34299, 34299, s->p_caster->GetGUID(), 100, PROC_ON_CRIT_ATTACK | static_cast<uint32>(PROC_TARGET_SELF), 0, NULL, NULL);

	return true;
}

bool Mangle( uint32 i, Aura *a, bool apply ){
	Unit *m_target = a->GetTarget();

	int32 val;

	if( apply )
		val = 30;
	else
		val = -30;

	m_target->ModDamageTakenByMechPCT[MECHANIC_BLEEDING] += val / 100.0f;

	return true;
}

bool PredatoryStrikes( uint32 i, Aura *a, bool apply ){
	Unit *m_target = a->GetTarget();
	int32 realamount = 0;


	realamount = ( a->GetModAmount( i ) * m_target->getLevel() ) / 100;

	if(apply)
	{
		a->SetPositive();
		m_target->ModAttackPowerMods( realamount );
	}
	else
		m_target->ModAttackPowerMods( -realamount);
	
	m_target->CalcDamage();
	
	return true;
}

bool Furor( uint32 i, Aura *a, bool apply ){
	Unit *u_target = a->GetTarget();

	if( !u_target->IsPlayer() )
		return true;
	Player *p_target = TO_PLAYER( u_target );

	if( p_target == NULL )
		return true;
	
	if( apply )
		p_target->m_furorChance += a->GetModAmount( i );
	else
		p_target->m_furorChance -= a->GetModAmount( i );
	
	return true;
}

bool Tranquility( uint32 i, Aura *a, bool apply ){
	if(apply)
		sEventMgr.AddEvent( a, &Aura::EventPeriodicHeal1, (uint32)a->GetModAmount( i ), EVENT_AURA_PERIODIC_HEAL, 2000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	else
		sEventMgr.RemoveEvents( a, EVENT_AURA_PERIODIC_HEAL);

	return true;
}

bool Hurricane( uint32 i, Aura *a, bool apply ){
	if(apply)
		sEventMgr.AddEvent( a, &Aura::EventPeriodicDamage, (uint32) a->GetModAmount( i ), EVENT_AURA_PERIODIC_DAMAGE, 1000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	else
		sEventMgr.RemoveEvents( a, EVENT_AURA_PERIODIC_DAMAGE);

	return true;
}

bool LifeBloom( uint32 i, Aura *a, bool apply ){
	Unit *m_target = a->GetTarget();
	
	if(apply)
		return true;
	
	// apply ONCE only.
	if( a->m_ignoreunapply )
		return true;
	
	Unit * pCaster = a->GetUnitCaster();
	if( pCaster == NULL )
		pCaster = m_target;
	
	// Remove other Lifeblooms - but do NOT handle unapply again
	bool expired = true;
	for(uint32 x=MAX_POSITIVE_AURAS_EXTEDED_START;x<MAX_POSITIVE_AURAS_EXTEDED_END;x++)
	{
		if( m_target->m_auras[x])
		{
			if( m_target->m_auras[x]->GetSpellId() == a->GetSpellId() )
			{
				m_target->m_auras[x]->m_ignoreunapply = true;
				if( m_target->m_auras[x]->GetTimeLeft() )
					expired = false;
				m_target->m_auras[x]->Remove();
			}
		}
	}
	
	if( expired )
	{
		Spell *spell = new Spell(pCaster, a->GetSpellProto(), true, NULL);
		spell->SetUnitTarget( m_target );
		spell->Heal( a->GetModAmount( i ) );
		delete spell;
	}

	return true;
}

bool LeaderOfThePack( uint32 i, Aura *a, bool apply ){
	Unit *u_target = a->GetTarget();

	if( !u_target->IsPlayer() )
		return true;

	Player *p_target = TO_PLAYER( u_target );
	
	if( apply )
		p_target->AddShapeShiftSpell( 24932 );
	else
		p_target->RemoveShapeShiftSpell( 24932 );
	
	return true;
}

void SetupDruidSpells(ScriptMgr * mgr)
{
	uint32 StarfallIds[] =
	{
		50286, // Rank 1
		53196, // Rank 2
		53197, // Rank 3
		53198, // Rank 4
		0,
	};
	mgr->register_dummy_spell(StarfallIds, &Starfall); 

	mgr->register_dummy_spell( 34297, &ImprovedLeaderOfThePack );
	mgr->register_dummy_spell( 34300, &ImprovedLeaderOfThePack );

	uint32 mangleids[] = {
		33878,
		33986,
		33987,
		33876,
		33982,
		33983,
		0
	};
	mgr->register_dummy_aura( mangleids, &Mangle );

	uint32 predatorystrikesids[] = {
		16972,
		16974,
		16975,
		0
	};
	mgr->register_dummy_aura( predatorystrikesids, &PredatoryStrikes );

	uint32 furorids[] = {
		17056,
		17058,
		17059,
		17060,
		17061,
		0
	};
	mgr->register_dummy_aura( furorids, &Furor );

	uint32 tranquilityids[] = {
		740,
		8918,
		9862,
		9863,
		21791,
		25817,
		26983,
		34550,
		48446,
		48447,
		0
	};
	mgr->register_dummy_aura( tranquilityids, &Tranquility );

	uint32 hurricaneids[] = {
		16914,
		17401,
		17402,
		27012,
		0
	};
	mgr->register_dummy_aura( hurricaneids, &Hurricane );

	uint32 lifebloomids[] = {
		33763,
		48450,
		48451,
		0
	};
	mgr->register_dummy_aura( lifebloomids, &LifeBloom );

	mgr->register_dummy_aura( 17007, &LeaderOfThePack );
}
