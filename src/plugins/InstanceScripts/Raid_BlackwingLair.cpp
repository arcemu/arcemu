/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
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

/************************************************************************/
/* Raid_BlackwingLair.cpp Script										*/
/************************************************************************/

// --- Death Talon Pack ---

// Death Talon Captain AI
#define CN_DTCAPTAIN 12467

#define MARK_OF_FLAMES 25050 // 1 target
#define MARK_OF_DETONATION 22438 // 1 target

class DTcaptainAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(DTcaptainAI);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		DTcaptainAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			// -- Number of spells to add --
			nrspells = 2;

			// --- Initialization ---
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}
			// ----------------------

			// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not!
			spells[0].info = dbcSpell.LookupEntry(MARK_OF_FLAMES);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = false;
			spells[0].perctrigger = 15.0f;
			spells[0].attackstoptimer = 1000; // 1sec

			spells[1].info = dbcSpell.LookupEntry(MARK_OF_DETONATION);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = false;
			spells[1].perctrigger = 4.0f;
			spells[1].attackstoptimer = 2000; // 1sec
		}

		void OnCombatStart(Unit* mTarget)
		{
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					if(!spells[i].perctrigger) continue;

					if(m_spellcheck[i])
					{
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}
						m_spellcheck[i] = false;
						return;
					}

					if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};





/*
Each Pack contains exactly one captain.
The Captain is a very powerful, well-armored lvl 62+ mob with ~240k HP.
They cast a debuff called "Mark of Flames" to random members of the raid, increasing fire damage taken by 1000 for 2 min.
Also they apply a magic debuff called "Mark of Detonation" to their current target, inflicting ~700 fire damage to ALL allies within 30 yards of the afflicted player each time he is struck by a melee attack.
They cannot be slept or otherwise crowd controlled, and must be tanked instead.
The Captain can also dispel any Hibernate effect cast on Death Talon Wyrmkin.
*/

// Death Talon Flamescale AI
#define CN_DTFLAMESCALE 12463

#define FLAME_SHOCK 22423 //1 target, ranged like

class DTflamescaleAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(DTflamescaleAI);
		SP_AI_Spell spells[1];
		bool m_spellcheck[1];

		DTflamescaleAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			// -- Number of spells to add --
			nrspells = 1;

			// --- Initialization ---
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}
			// ----------------------

			// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not!
			spells[0].info = dbcSpell.LookupEntry(FLAME_SHOCK);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = false;
			spells[0].perctrigger = 100.0f;
			spells[0].attackstoptimer = 1000; // 1sec

		}

		void OnCombatStart(Unit* mTarget)
		{
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					if(!spells[i].perctrigger) continue;

					if(m_spellcheck[i])
					{
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}
						m_spellcheck[i] = false;
						return;
					}

					if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};

/*
Each Pack contains one or two of these, randomly determined for each pack.
Flamescales are moderately tough lvl 62+ mobs with ~120k HP.
They have a periodic ranged attack called "Flame Shock" which inflicts ~2000 fire damage and
applies a magic DoT that inflicts an additional ~350 fire damage every 3 sec for 12 sec.
Combined with the Mark of Flames, this attack and DoT can become quite dangerous.
They also have a randomly targeted charge that knocks back those affected.
They cannot be slept or otherwise crowdcontrolled, and must be tanked instead.
*/

// Death Talon Seether AI

/*
Each Pack contains one or two.
Seethers are moderately tough level 62+ mobs with ~110k HP.
They go into a frenzy state to inflict additional melee damage (can be removed with tranquilizing shot!), and have a frontal cleave attack.
They cannot be slept or otherwise crowdcontrolled, and must be tanked instead.
As of patch 1.12 Seethers now have a Flame Buffet ability that deals roughly 1k damage per stack. Be aware of this if you kite these.
*/

// Death Talon Wyrmkin AI
#define CN_DTWYRMKIN 12465

#define FIREBALL_VOLLEY 36742 // various targets

class DTwyrmkinAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(DTwyrmkinAI);
		SP_AI_Spell spells[1];
		bool m_spellcheck[1];

		DTwyrmkinAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			// -- Number of spells to add --
			nrspells = 1;

			// --- Initialization ---
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}
			// ----------------------

			// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not!
			spells[0].info = dbcSpell.LookupEntry(FIREBALL_VOLLEY);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = false;
			spells[0].perctrigger = 40.0f;
			spells[0].attackstoptimer = 1000; // 1sec

		}

		void OnCombatStart(Unit* mTarget)
		{
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					if(!spells[i].perctrigger) continue;

					if(m_spellcheck[i])
					{
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}
						m_spellcheck[i] = false;
						return;
					}

					if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};
/*
Each Pack contains one or two. Wyrmkin are relatively fragile level 61+ mobs with ~90k HP.
Their primary form of attack is a "Fireball Volley" that they use frequently, sending out powerful fireballs
doing ~1100 damage apiece to all targets within LoS of the Wyrmkin.
You can break LoS with any active Wyrmkin to avoid being hit by this attack.
Although physically weak, Wyrmkin are a large threat to the raid if allowed to use their Volley attack unchecked.
They can be put to sleep by a druid's hibernate ability, but due to their high level they will tend to resist or break free of this effect fairly often.
"Fireball Volley" can be prevented by chain fearing or chain stunning the mob while the rest of the raid nukes it down.
*/


// -------------------------------

// --- Blackwing Lair Lab Packs ---

// Blackwing Technician AI
#define CN_TECHNICIAN 13996

#define GRANADE 30217

class TechnicianAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(TechnicianAI);
		SP_AI_Spell spells[1];
		bool m_spellcheck[1];

		TechnicianAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			// -- Number of spells to add --
			nrspells = 1;

			// --- Initialization ---
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}
			// ----------------------

			// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not!
			spells[0].info = dbcSpell.LookupEntry(GRANADE);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = false;
			spells[0].perctrigger = 25.0f;
			spells[0].attackstoptimer = 1000; // 1sec

		}

		void OnCombatStart(Unit* mTarget)
		{
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					if(!spells[i].perctrigger) continue;

					if(m_spellcheck[i])
					{
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}
						m_spellcheck[i] = false;
						return;
					}

					if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};
/*
Relatively low HP. Has a ranged aoe grenade that inflicts moderate damage to anyone in the area of effect.
*/

// Blackwing Warlock AI
#define CN_BLACK_WARLOCK 12459

#define RAIN_OF_FIRE 19717
#define SHADOW_BOLT 36986

class BlackWarlockAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(BlackWarlockAI);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		BlackWarlockAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			// -- Number of spells to add --
			nrspells = 2;

			// --- Initialization ---
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}
			// ----------------------

			// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not!
			spells[0].info = dbcSpell.LookupEntry(RAIN_OF_FIRE);
			spells[0].targettype = TARGET_DESTINATION;
			spells[0].instant = false;
			spells[0].perctrigger = 10.0f;
			spells[0].attackstoptimer = 6000; // 1sec

			spells[1].info = dbcSpell.LookupEntry(SHADOW_BOLT);
			spells[1].targettype = TARGET_DESTINATION;
			spells[1].instant = true;
			spells[1].perctrigger = 15.0f;
			spells[1].attackstoptimer = 1000; // 1sec

		}

		void OnCombatStart(Unit* mTarget)
		{
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					if(!spells[i].perctrigger) continue;

					if(m_spellcheck[i])
					{
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}
						m_spellcheck[i] = false;
						return;
					}

					if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};


/*
Casts a Rain of Fire that does 925 to 1075 damage per tick, also summons Enraged Felguards.
Their summon spell, 'Demon Portal' takes 30 seconds to generate a felguard once it's been cast.
Summoned creatures must be kept banished while everyone else needs to run out of the rain of fire quickly.
They are immune to polymorph and have roughly 100k health.
The summonables should remain banished until the rest of the lab pack has been killed, at which point a tank can pick it up.
Felguards can also be Feared if there arent enough Banishes to go around, or if the raid is just that ballsy.
The Warlocks also cast a 1700 damage shadow bolt.
*/

// Death Talon Overseer AI

/*
Large dragonkins that are resistant to most schools of magic, and highly vulnerable to one other school.
Although tough, they will go down quickly once their weakness is deduced. It can be one of the following classes: Shadow, Nature, Arcane, Fire, Frost.
They have three main attacks. They hit for 800-1000 every 2 seconds, they cleave for 1k every 10ish, and do a fire blast attack for 1k fire damage every 10 seconds.
As these do only targetted damage (minus cleave) they can be easily held for last in these packs.
*/

// Blackwing Spellbinder AI

/*
Immune to magical attacks, can only be meleed.
The hit for something like 400 dps. They cast greater polymorphs and drop flamestrikes that hit for 1600-1900 damage and have a DoT tick for 400 damage.
The flamestrikes have a 5 yd aoe, so melee dps wants to stand at the very edge of the mobs hit box to avoid being affected.
Typically dealt with in tandem with Overseers.
There are rumors that the polymorph is only targetted against people that are not on the spellbinders aggro table.
That said, as of patch 2.0, Ive been polyd while dpsing these.
It would appear more accurate to say that the polymorph is targetted at people who do not have the spellbinders aggro.
*/

// -------------------------------

// --- BOSS'S ---

// Broodlord Lashlayer AI
#define CN_LASHLAYER 12017

#define FIRE_NOVA_LASH 39001 //BLAST WAVE
#define MORTAL_STRIKE 9347
#define KNOCK_BACK 20686

class LashlayerAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(LashlayerAI);
		SP_AI_Spell spells[1];
		bool m_spellcheck[1];

		LashlayerAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			// -- Number of spells to add --
			nrspells = 1;

			// --- Initialization ---
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}
			// ----------------------

			// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not!
			spells[0].info = dbcSpell.LookupEntry(FIRE_NOVA_LASH);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = false;
			spells[0].perctrigger = 15.0f;
			spells[0].attackstoptimer = 1000; // 1sec

		}

		void OnCombatStart(Unit* mTarget)
		{
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "None of your kind should be here. You have doomed only yourselves!");
			_unit->PlaySoundToSet(8286);
			//_unit->PlaySoundToSet(8287);
		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					if(!spells[i].perctrigger) continue;

					if(m_spellcheck[i])
					{
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}
						m_spellcheck[i] = false;
						return;
					}

					if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};


// Firemaw AI
#define CN_FIREMAW 11983

#define WING_BUFFET 37319
#define FLAME_BUFFET 23341

class FiremawAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(FiremawAI);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		FiremawAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			// -- Number of spells to add --
			nrspells = 2;

			// --- Initialization ---
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}
			// ----------------------

			// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not!
			spells[0].info = dbcSpell.LookupEntry(WING_BUFFET);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = false;
			spells[0].perctrigger = 10.0f;
			spells[0].attackstoptimer = 1000; // 1sec

			spells[1].info = dbcSpell.LookupEntry(FLAME_BUFFET);
			spells[1].targettype = TARGET_VARIOUS;
			spells[1].instant = false;
			spells[1].perctrigger = 15.0f;
			spells[1].attackstoptimer = 1000; // 1sec

		}

		void OnCombatStart(Unit* mTarget)
		{
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					if(!spells[i].perctrigger) continue;

					if(m_spellcheck[i])
					{
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}
						m_spellcheck[i] = false;
						return;
					}

					if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};

// Ebonroc AI

#define CN_EBONROC 14601

#define SHADOW_OF_EBONROC 23340

class EbonrocAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(EbonrocAI);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		EbonrocAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			// -- Number of spells to add --
			nrspells = 2;

			// --- Initialization ---
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}
			// ----------------------

			// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not!
			spells[0].info = dbcSpell.LookupEntry(WING_BUFFET);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = false;
			spells[0].perctrigger = 10.0f;
			spells[0].attackstoptimer = 1000; // 1sec

			spells[1].info = dbcSpell.LookupEntry(SHADOW_OF_EBONROC);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = false;
			spells[1].perctrigger = 15.0f;
			spells[1].attackstoptimer = 1000; // 1sec

		}

		void OnCombatStart(Unit* mTarget)
		{
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					if(!spells[i].perctrigger) continue;

					if(m_spellcheck[i])
					{
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}
						m_spellcheck[i] = false;
						return;
					}

					if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};

// Flamegor AI
#define CN_FLAMEGOR 11981

#define FIRE_NOVA 23462


class FlamegorAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(FlamegorAI);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		FlamegorAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			// -- Number of spells to add --
			nrspells = 2;

			// --- Initialization ---
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}
			// ----------------------

			// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not!
			spells[0].info = dbcSpell.LookupEntry(WING_BUFFET);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = false;
			spells[0].perctrigger = 10.0f;
			spells[0].attackstoptimer = 1000; // 1sec

			spells[1].info = dbcSpell.LookupEntry(FIRE_NOVA);
			spells[1].targettype = TARGET_VARIOUS;
			spells[1].instant = true;
			spells[1].perctrigger = 20.0f;
			spells[1].attackstoptimer = 1000; // 1sec

		}

		void OnCombatStart(Unit* mTarget)
		{
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					if(!spells[i].perctrigger) continue;

					if(m_spellcheck[i])
					{
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}
						m_spellcheck[i] = false;
						return;
					}

					if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};




/*
He does a Blastwave / AoE Knockback similar to the Scholomance Handlers (in the undead dragonkin room).
It is fire based and does quite a hefty amount of damage.

Like most nasty bosses, he has a cleave that your cloth wearers are going to want to avoid.
While not as nasty as Vael's, it's still pretty nasty.

Possibly the most problematic attack that Broodlord has is an ungodly Mortal Strike that he will do frequently on your Main Tank.
This can crit for up to 8000 damage on plate and 15000+ on cloth. It also leaves the MS debuff that reduces healing, which is Not Good.
It is recommended that priests power word shield the tank when he applies the MS debuff.

The trickiest part of Broodlord is a single target knockback that he will do on whomever is highest on his threat list.
After he knocks a target back, that targets aggro is reduced by 50%.
Over the course of this fight, he will do this repeatedly on all of your tanks.
*/

// QUOTE -> "None of your kind should be here. You have doomed only yourselves!"
// sound A_BroodlordLashlayerAggro -> 8286
// sound A_BroodlordLashlayerLeashTrigge -> 8287


// Vaelastrasz the corruptAI
#define CN_VAELASTRASZ 13020

#define ESSENCE_OF_THE_RED 23513
#define FLAME_BREATH 18435
#define BURNING_ADRENALINE 18173

class VaelastraszAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(VaelastraszAI);
		SP_AI_Spell spells[3];
		bool m_spellcheck[3];

		VaelastraszAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			// -- Number of spells to add --
			nrspells = 3;

			// --- Initialization ---
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}
			// ----------------------

			// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not!
			spells[0].info = dbcSpell.LookupEntry(ESSENCE_OF_THE_RED);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = false;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000; // 1sec
			m_spellcheck[0] = true;

			spells[1].info = dbcSpell.LookupEntry(FLAME_BREATH);
			spells[1].targettype = TARGET_VARIOUS;
			spells[1].instant = false;
			spells[1].perctrigger = 15.0f;
			spells[1].attackstoptimer = 3000; // 1sec

			spells[2].info = dbcSpell.LookupEntry(BURNING_ADRENALINE);
			spells[2].targettype = TARGET_ATTACKING;
			spells[2].instant = false;
			spells[2].perctrigger = 3.0f;
			spells[2].attackstoptimer = 2000; // 1sec

			_unit->SetHealth((uint32)(_unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH) * 0.3f));

		}

		void OnCombatStart(Unit* mTarget)
		{
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
			_unit->CastSpell(_unit, spells[0].info, spells[0].instant);
		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnTargetDied(Unit* mTarget)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Forgive me, your death only adds to my failure.");
			_unit->PlaySoundToSet(8284);
		}

		void OnDied(Unit* mKiller)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "At last the agony ends. I have failed you my Queen... I have failed us all...");
			//	_unit->PlaySoundToSet(); Missing SoundSet
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			if(_unit->GetHealthPct() <= 15 && m_spellcheck[0])
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Nefarius' hate has made me stronger than ever before. You should have fled, while you could, mortals! The fury of Blackrock courses through my veins!");
				_unit->PlaySoundToSet(8285);
				m_spellcheck[0] = false;
			}


			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					if(!spells[i].perctrigger) continue;

					if(m_spellcheck[i])
					{
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}
						m_spellcheck[i] = false;
						return;
					}

					if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};

class VaelastraszGossip : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* Plr)
		{
			GossipMenu* Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9903, Plr);

			Menu->SendTo(Plr);

		}

		void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId)
		{

		}

};



/*
Essence of the Red [1]
    This debuff is cast on the raid at the beginning of the encounter and lasts for 3 minutes.
		Restores 500 Mana per second to mana users. Restores 50 Energy per second to Rogues and Cat Form Druids.
		Generates 20 Rage per second for Warriors and Bear Form Druids. It essentially means players have infinite mana/rage/energy for the fight.
		It is not dispellable, but can be removed by ice block or divine shield (which is not advised).

Fire Nova
    Inflicts 555-645 Fire damage to nearby enemies.
	Fire resistance is a must in order for healers to keep the raid alive.
	Player should wear as much quality FR gear as possible, but as a general rule, casters should have a little over 150, melee DPS over 200, and all Main Tanks the full 315 fire Resistance.

Flame Breath
    Inflicts 3063 to 3937 Fire damage to enemies in a cone in front of the caster.
	Must be directed away from the raid by the MT. Every flame breath applies a stacking debuff (also called flame breath) that ticks for 1000ish fire damage every few seconds.
	It's maximum level, which is always reached by the time the MT is burning, ticks for 4600 damage. This debuff plays a huge role. Early in the rotation, the MT takes only 1200dps or so. By the time they are burning, they're taking more like 2000dps.
	The difference is all in this attack.

Burning Adrenaline [2]
    Damage done increased by 100%. Attack speed increased by 100%. Spells become instant cast.
		Reduces max health by 5% every second; eventually causes player to die.

    Vael only casts Burning Adrenaline in two scenarios: He will cast it on random mana users throughout the fight and he will cast it on the current tank every 45 seconds.
	1 tank killed every 45 seconds for 3 minutes --> 3 / 0.75 = 4 tanks.

Cleave
    Cleave attack that hits for 2k. This is a chain cleave, so if positioning is poor it can chain to the entire raid, even to behind him.
*/

/*Lord Victor Nefarius (as he is corrupting Vaelastrasz)

    "Ah, the Heroes, you are persistent aren't you? Your Ally here attempted to match his power against mine....and paid the price. Now he shall serve me. By slaughtering you."
    "Get up little Red Wyrm, and destroy them!"
*/

// sound A_LordVictorNefariusAtStart -> 8279

/*Vaelastrasz (as the corruption slowly takes over)
    "Too late...friends. Nefarius' corruption has taken hold. I cannot...control myself. I beg you Mortals, flee! Flee before I lose all sense of control. The Black Fire rages within my heart. I must release it! FLAME! DEATH! DESTRUCTION! COWER MORTALS BEFORE THE WRATH OF LORD....NO! I MUST FIGHT THIS!"
*/

// sound A_VaelastraszLine1 -> 8281
// sound A_VaelastraszLine2 -> 8282
// sound A_VaelastraszLine2 -> 8283


void SetupBlackwingLair(ScriptMgr* mgr)
{
	mgr->register_creature_script(CN_DTCAPTAIN, &DTcaptainAI::Create);
	mgr->register_creature_script(CN_DTFLAMESCALE, &DTflamescaleAI::Create);
	mgr->register_creature_script(CN_DTWYRMKIN, &DTwyrmkinAI::Create);
	mgr->register_creature_script(CN_TECHNICIAN, &TechnicianAI::Create);
	mgr->register_creature_script(CN_BLACK_WARLOCK, &BlackWarlockAI::Create);
	mgr->register_creature_script(CN_LASHLAYER, &LashlayerAI::Create);
	mgr->register_creature_script(CN_FIREMAW, &FiremawAI::Create);
	mgr->register_creature_script(CN_EBONROC, &EbonrocAI::Create);
	mgr->register_creature_script(CN_FLAMEGOR, &FlamegorAI::Create);
	mgr->register_creature_script(CN_VAELASTRASZ, &VaelastraszAI::Create);
	GossipScript* vg = new VaelastraszGossip;
	mgr->register_gossip_script(CN_VAELASTRASZ, vg);        // Vael Gossip TODO: change the flag to agressive
}
