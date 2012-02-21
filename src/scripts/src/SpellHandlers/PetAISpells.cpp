#include "Setup.h"

class ArmyOfTheDeadGhoulAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(ArmyOfTheDeadGhoulAI);
		ArmyOfTheDeadGhoulAI(Creature* c) : CreatureAIScript(c)
		{
			_unit->GetAIInterface()->m_canMove = false;
		}

		void OnLoad()
		{

			RegisterAIUpdateEvent(200);

			if(_unit->IsSummon())
			{
				Summon* s = TO< Summon* >(_unit);

				float parent_bonus = s->GetOwner()->GetDamageDoneMod(SCHOOL_NORMAL) * 0.04f ;

				s->SetMinDamage(s->GetMinDamage() + parent_bonus);
				s->SetMaxDamage(s->GetMaxDamage() + parent_bonus);
			}
		}

		void AIUpdate()
		{
			_unit->CastSpell(_unit->GetGUID(), 20480, false);
			RemoveAIUpdateEvent();
			_unit->GetAIInterface()->m_canMove = true;
		}

	private:

};

class ShadowFiendAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(ShadowFiendAI);
		ShadowFiendAI(Creature* c) : CreatureAIScript(c)
		{
		}

		void OnLoad()
		{

			if(_unit->IsPet())
			{
				Pet* s = TO< Pet* >(_unit);
				Player* owner = s->GetPetOwner();


				float owner_bonus = static_cast< float >(owner->GetDamageDoneMod(SCHOOL_SHADOW) * 0.375f); // 37.5%
				s->BaseAttackType = SCHOOL_SHADOW; // Melee hits are supposed to do damage with the shadow school
				s->SetBaseAttackTime(MELEE, 1500); // Shadowfiend is supposed to do 10 attacks, sometimes it can be 11
				s->SetMinDamage(s->GetMinDamage() + owner_bonus);
				s->SetMaxDamage(s->GetMaxDamage() + owner_bonus);
				s->BaseDamage[0] += owner_bonus;
				s->BaseDamage[1] += owner_bonus;

				Unit* uTarget = s->GetMapMgr()->GetUnit(owner->GetTargetGUID());
				if((uTarget != NULL) && isAttackable(owner, uTarget))
				{
					s->GetAIInterface()->AttackReaction(uTarget, 1);
					s->GetAIInterface()->setNextTarget(uTarget);
				}
			}
		}

	private:

};

class MirrorImageAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(MirrorImageAI);
		MirrorImageAI(Creature* c) : CreatureAIScript(c)
		{
		}

		void OnLoad()
		{
			if(_unit->IsSummon())
			{
				Summon* s = TO< Summon* >(_unit);
				Unit* owner = s->GetOwner();

				owner->CastSpell(_unit, 45204, true);   // clone me
				owner->CastSpell(_unit, 58838, true);   // inherit threat list

				// Mage mirror image spell
				if(_unit->GetCreatedBySpell() == 58833)
				{
					_unit->SetMaxHealth(2500);
					_unit->SetHealth(2500);
					_unit->SetMaxPower(POWER_TYPE_MANA, owner->GetMaxPower(POWER_TYPE_MANA));
					_unit->SetPower(POWER_TYPE_MANA, owner->GetPower(POWER_TYPE_MANA));

					SpellRange* range = NULL;

					AI_Spell sp1;

					sp1.entryId = 59638;
					sp1.spell = dbcSpell.LookupEntryForced(sp1.entryId);
					sp1.spellType = STYPE_DAMAGE;
					sp1.agent = AGENT_SPELL;
					sp1.spelltargetType = TTYPE_SINGLETARGET;
					sp1.cooldown = 0;
					sp1.cooldowntime = 0;
					sp1.Misc2 = 0;
					sp1.procCount = 0;
					sp1.procChance = 100;
					range = dbcSpellRange.LookupEntry(sp1.spell->rangeIndex);
					sp1.minrange = GetMinRange(range);
					sp1.maxrange = GetMaxRange(range);

					_unit->GetAIInterface()->addSpellToList(&sp1);

					AI_Spell sp2;
					sp2.entryId = 59637;
					sp2.spell = dbcSpell.LookupEntryForced(sp2.entryId);
					sp2.spellType = STYPE_DAMAGE;
					sp2.agent = AGENT_SPELL;
					sp2.spelltargetType = TTYPE_SINGLETARGET;
					sp2.cooldown = 0;
					sp2.cooldowntime = 0;
					sp2.Misc2 = 0;
					sp2.procCount = 0;
					sp2.procChance = 100;
					range = dbcSpellRange.LookupEntry(sp2.spell->rangeIndex);
					sp2.minrange = GetMinRange(range);
					sp2.maxrange = GetMaxRange(range);

					_unit->GetAIInterface()->addSpellToList(&sp2);

				}
			}

		}

	private:
};


class DancingRuneWeaponAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(DancingRuneWeaponAI);
		DancingRuneWeaponAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{

		}

		void OnLoad()
		{
			_unit->SetDisplayId(_unit->GetCreatureInfo()->Female_DisplayID);
			_unit->SetBaseAttackTime(MELEE, 2000);

			if(_unit->IsSummon())
			{
				Summon* s = TO< Summon* >(_unit);

				Unit* owner = s->GetOwner();

				if(owner->IsPlayer())
				{
					Player* pOwner = TO< Player* >(owner);

					Item* item = pOwner->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
					if(item != NULL)
					{

						for(int s = 0; s < 5; s++)
						{
							if(item->GetProto()->Spells[s].Id == 0)
								continue;

							if(item->GetProto()->Spells[s].Trigger == CHANCE_ON_HIT)
								procSpell[s] = item->GetProto()->Spells[s].Id;
						}

						s->SetEquippedItem(MELEE, item->GetEntry());
						s->SetBaseAttackTime(MELEE, item->GetProto()->Delay);

					}

					pOwner->SetPower(POWER_TYPE_RUNIC_POWER, 0);
				}

				s->SetMinDamage(owner->GetDamageDoneMod(SCHOOL_NORMAL));
				s->SetMaxDamage(owner->GetDamageDoneMod(SCHOOL_NORMAL));
			}
		}

		void OnDied(Unit* mKiller) { RemoveAIUpdateEvent(); }


		void OnCombatStart(Unit* mTarget)
		{
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
			dpsCycle = 0;
		}

		void OnCombatStop(Unit* mTarget)
		{
			RemoveAIUpdateEvent();
			dpsCycle = 0;
		}

		void AIUpdate()
		{
			Unit* curtarget = _unit->GetAIInterface()->getNextTarget();
			if(_unit->GetCurrentSpell() == NULL && curtarget)
			{
				switch(dpsCycle)
				{
					case 0:
						dpsSpell = 49921; // Plague Strike
						break;
					case 1:
						dpsSpell = 49909; // Icy Touch
						break;
					case 2:
					case 3:
						dpsSpell = 55262; // Heart Strike x 2
						break;
					case 4:
						dpsSpell = 51425; // Obliterate
						break;
					case 5:
						dpsSpell = 49895; // Death Coil
						break;
					case 6:
					case 7:
						dpsSpell = 51425; // Obliterate x 2
						break;
					case 8:
					case 9:
						dpsSpell = 55262; // Heart Strike x 2
						break;
					case 10:
					case 11:
						dpsSpell = 49895; // Death Coil x 2
						break;
				}
				dpsCycle++;
				if(dpsCycle > 11)
					dpsCycle = 0;

				SpellEntry* MyNextSpell = dbcSpell.LookupEntryForced(dpsSpell);
				if(MyNextSpell != NULL)
					_unit->CastSpell(curtarget, MyNextSpell, true);

			}
		}

		void OnHit(Unit* mTarget, float fAmount)
		{
			for(int p = 0; p < 5; p++)
			{
				if(procSpell[p] != 0)
				{
					SpellEntry* mProc = dbcSpell.LookupEntryForced(procSpell[p]);
					if(!mProc)
						return;
					int x = rand() % 100;
					uint32 proc = mProc->procChance;
					if(proc < 1)
						proc = 10; // Got to be fair :P

					if((uint32)x <= proc)
					{
						Unit* Vic = mProc->self_cast_only ? _unit : mTarget;
						_unit->CastSpell(Vic, mProc, true);
					}
				}
			}
		}
	private:
		int dpsCycle;
		int dpsSpell;
		int procSpell[5];
};

class FrostBroodVanquisherAI : public CreatureAIScript{
public:
	ADD_CREATURE_FACTORY_FUNCTION( FrostBroodVanquisherAI );
	FrostBroodVanquisherAI( Creature *c ) : CreatureAIScript( c ){
	}

	void OnLoad(){
		_unit->SetByte( UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_UNK_2 );
	}

	void OnLastPassengerLeft( Unit *passenger ){
		if( _unit->GetSummonedByGUID() == passenger->GetGUID() )
			_unit->Despawn( 1 * 1000, 0 );
	}
};

void SetupPetAISpells(ScriptMgr* mgr)
{
	mgr->register_creature_script(24207, &ArmyOfTheDeadGhoulAI::Create);
	mgr->register_creature_script(19668, &ShadowFiendAI::Create);
	mgr->register_creature_script(27893, &DancingRuneWeaponAI::Create);
	mgr->register_creature_script(31216, &MirrorImageAI::Create);
	mgr->register_creature_script( 28670, &FrostBroodVanquisherAI::Create );
};
