/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009-2010 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
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
/* Instance_ScarletMonastery.cpp Script									*/
/************************************************************************/

/*
	Graveyard
*/

// Interrogator Vishas
#define VISHAS 3983
#define SHADOW_WORD 2767

class VishasAI : public MoonScriptCreatureAI
{
	public:
		MOONSCRIPT_FACTORY_FUNCTION(VishasAI, MoonScriptCreatureAI);
		VishasAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(SHADOW_WORD, Target_RandomPlayer, 20, 0, 8);

			AddEmote(Event_OnCombatStart, "Tell me... tell me everything!", Text_Yell, 5847);
			AddEmote(Event_OnTargetDied, "Purged by pain!", Text_Yell, 5848);

			m_uiSay = 0;
		};

		void OnCombatStop(Unit* pTarget)
		{
			m_uiSay = 0;

			ParentClass::OnCombatStop(pTarget);
		};

		void AIUpdate()
		{
			if(GetHealthPercent() <= 75 && m_uiSay == 0)
			{
				Emote("Naughty secrets!", Text_Yell, 5849);
				m_uiSay = 1;
			};

			if(GetHealthPercent() <= 25 && m_uiSay == 1)
			{
				Emote("I'll rip the secrets from your flesh!", Text_Yell, 5850);
				m_uiSay = 2;
			};

			ParentClass::AIUpdate();
		};

	private:
		uint8 m_uiSay;
};

// Bloodmage Thalnos
#define THALNOS 4543
#define SHADOW_BOLT 9613
#define FLAME_SPIKE 8814

class ThalnosAI : public MoonScriptCreatureAI
{
	public:
		MOONSCRIPT_FACTORY_FUNCTION(ThalnosAI, MoonScriptCreatureAI);
		ThalnosAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(SHADOW_BOLT, Target_RandomPlayer, 20, 3.0f, 2);
			AddSpell(FLAME_SPIKE, Target_RandomPlayerDestination, 20, 3.0f, 14);

			AddEmote(Event_OnCombatStart, "We hunger for vengeance.", Text_Yell, 5844);
			AddEmote(Event_OnTargetDied, "More... More souls.", Text_Yell, 5845);

			m_bEmoted = false;
		};

		void OnCombatStop(Unit* pTarget)
		{
			m_bEmoted = false;

			ParentClass::OnCombatStop(pTarget);
		};

		void AIUpdate()
		{
			if(GetHealthPercent() <= 50 && m_bEmoted == false)
			{
				Emote("No rest, for the angry dead.", Text_Yell, 5846);
				m_bEmoted = true;
			};

			ParentClass::AIUpdate();
		};

	private:
		bool m_bEmoted;
};

/*
	Library
*/
//Houndmaster Loksey

#define LOKSEY 3974

class LokseyAI : public MoonScriptCreatureAI
{
	public:
		MOONSCRIPT_FACTORY_FUNCTION(LokseyAI, MoonScriptCreatureAI);
		LokseyAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(6742, Target_Self, 5, 0, 40);
			AddEmote(Event_OnCombatStart, "Release the hounds!", Text_Yell, 5841);
		};
};

// Arcanist Doan

#define DOAN 6487
#define SHIELD 9438
#define NOVA 9435
#define POLY 13323
#define SILENCE 30225
#define ARCANE_EXP 9433

class DoanAI : public MoonScriptCreatureAI
{
	public:
		MOONSCRIPT_FACTORY_FUNCTION(DoanAI, MoonScriptCreatureAI);
		DoanAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(SILENCE, Target_Self, 25, 1.5f, 14);
			AddSpell(POLY, Target_SecondMostHated, 15, 1.5f, 10);
			AddSpell(ARCANE_EXP, Target_Self, 20, 0, 10);

			AddEmote(Event_OnCombatStart, "You will not defile these mysteries!", Text_Yell, 5842);

			m_bShielded = false;
		};

		void OnDamageTaken(Unit* mAttacker, uint32 fAmount)
		{
			if(GetHealthPercent() <= 50 && !m_bShielded)
				Shield();
		};

		void Shield()
		{
			_unit->CastSpell(_unit, SHIELD , true);
			Emote("Burn in righteous fire!", Text_Yell, 5843);
			_unit->CastSpell(_unit, NOVA, false);
			m_bShielded = true;
		};

		void OnCombatStop(Unit* pTarget)
		{
			m_bShielded = false;

			ParentClass::OnCombatStop(pTarget);
		};

	private:
		bool m_bShielded;
};

/*
	Armory
*/

// Herod
#define HEROD 3975
#define WHIRLWINDSPELL 9632
#define CHARGE 22911
#define ENRAGESPELL 8269
#define AGGRO4 5830
#define KILL 5831
#define WHIRLWIND 5832
#define HEROD_ENRAGE 5833

class HerodAI : public MoonScriptCreatureAI
{
	public:
		MOONSCRIPT_FACTORY_FUNCTION(HerodAI, MoonScriptCreatureAI);
		HerodAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(WHIRLWINDSPELL, Target_Self, 12, 0, 12)->AddEmote("Blades of Light!", Text_Yell, 5832);
			AddSpell(CHARGE, Target_RandomPlayer, 6, 0, 20);

			AddEmote(Event_OnCombatStart, "Ah, I've been waiting for a real challenge!", Text_Yell, 5830);
			AddEmote(Event_OnTargetDied, "Ha, is that all?", Text_Yell, 5831);

			m_bEnraged = false;
		};

		void OnCombatStop(Unit* pTarget)
		{
			m_bEnraged = false;
			RemoveAura(ENRAGESPELL);

			ParentClass::OnCombatStop(pTarget);
		};

		void AIUpdate()
		{
			if(GetHealthPercent() <= 40 && m_bEnraged == false)
			{
				ApplyAura(ENRAGESPELL);
				Emote("Light, give me strength!", Text_Yell,  5833);
			};

			ParentClass::AIUpdate();
		};

		bool    m_bEnraged;
};

/*
	Cathedral
*/

// Scarlet Commander Mograine

#define COMMANDER_MOGRAINE 3976
#define HAMMER 32416
#define CRUSADER 14517
#define RESTALK 5835

class MograineAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(MograineAI);

		SP_AI_Spell spells[3];

		bool m_spellcheck[3];

		MograineAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 3;
			mPhase = 0;

			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(SHIELD);
			spells[0].targettype = TARGET_SELF;
			spells[0].instant = true;
			spells[0].perctrigger = 5.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(HAMMER);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].perctrigger = 10.0f;
			spells[1].attackstoptimer = 1000;

			spells[2].info = dbcSpell.LookupEntry(CRUSADER);
			spells[2].targettype = TARGET_ATTACKING;
			spells[2].instant = true;
			spells[2].perctrigger = 30.0f;
			spells[2].attackstoptimer = 1000;
			Timer = 0;

		}

		void OnCombatStart(Unit* mTarget)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Infidels. They must be purified!");

			_unit->PlaySoundToSet(5835);

			RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		}

		void OnTargetDied(Unit* mTarget)
		{

			if(_unit->GetHealthPct() > 0)
			{
				int RandomSpeach;
				RandomFloat(1000);
				RandomSpeach = rand() % 2;
				switch(RandomSpeach)
				{
					case 0:

						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Unworthy.");

						_unit->PlaySoundToSet(5836);
						break;

				}
			}
		}


		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			GameObject*  pDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(1173.01f, 1389.91f, 31.9723f, 104600);
			if(pDoor == 0)
				return;

			// Open the door
			pDoor->SetState(0);

			RemoveAIUpdateEvent();
		}


		void AIUpdate()
		{
			Timer = Timer + 1;

			/*if (Timer == 30000000000)
			{
			    _unit->CastSpell(_unit, spells[1].info, spells[1].instant);
			}

			else
			{*/
			float val = RandomFloat(100.0f);
			SpellCast(val);
//}
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

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
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
		uint32 mPhase;
		int nrspells;
		int Timer;
};

// High Inquisitor Whitemane

#define WHITEMANE 3977
#define SMITE 9481
#define SLEEP 9256
#define RESURRECTION 25435
#define RESTALK2 5840

class WhitemaneAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(WhitemaneAI);
		SP_AI_Spell spells[3];
		bool m_spellcheck[3];

		WhitemaneAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 3;
			mPhase = 0;

			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(SMITE);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = false;
			spells[0].perctrigger = 15.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(SLEEP);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].perctrigger = 0.0f;
			spells[1].attackstoptimer = 1000;

			spells[2].info = dbcSpell.LookupEntry(RESURRECTION);
			spells[2].targettype = TARGET_VARIOUS; //Can't seem to get her to cast it on Mograine...
			spells[2].instant = false;
			spells[2].perctrigger = 0.0f;
			spells[2].attackstoptimer = 1000;
			spells[2].soundid = RESTALK2;
			spells[2].speech = "Arise, my champion!";
			Timer = 0;
		}

		void OnCombatStart(Unit* mTarget)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Mograine has fallen? You shall pay for this treachery!");

			_unit->PlaySoundToSet(5838);

			RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		}

		void OnTargetDied(Unit* mTarget)
		{

			if(_unit->GetHealthPct() > 0)
			{
				int RandomSpeach;
				RandomFloat(1000);
				RandomSpeach = rand() % 2;
				switch(RandomSpeach)
				{
					case 0:

						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The light has spoken!");

						_unit->PlaySoundToSet(5839);
						break;

				}
			}
		}

		void OnDamageTaken(Unit* mAttacker, uint32 fAmount)
		{
			if(fAmount < 5) return;
			// <50% hp -> We go to phase 1
			if(_unit->GetHealthPct() <= 50 && mPhase == 0)
			{
				ChangeToPhase1();
			}
		}

		void ChangeToPhase1()
		{
			// Set phase var
			mPhase = 1;

			// Play sound, and send text.
			/*_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Light, give me strength!");
			       ^^ Notes for myself */
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Arise, my champion!");
			//MoveToRes();
			CastSleep();
			CastRes();
		}

		/*void MoveToRes()
		{
			_unit->GetAIInterface ()->MoveTo(1154.859009,1403.924683,32.250183,3.466254);
		}*/

		void CastSleep()
		{
			_unit->CastSpell(_unit, spells[1].info, spells[1].instant);
		}

		void CastRes()
		{
			_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
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
			Timer = Timer + 1;

			/*if (Timer == 30000000000)
			{
				 //_unit->CastSpell(_unit, spells[1].info, spells[1].instant);
			}

			else
			{*/
			float val = RandomFloat(100.0f);
			SpellCast(val);
			//}
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

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
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
		uint32 mPhase;  // NPC has 2 phases
		int nrspells;
		int Timer;
};


// High Inquisitor Fairbanks

#define FAIRBANKS 4542
#define BLOOD 40412 //Need a better spell
#define PWS 11647 //PWS = Power Word: Shield 

class FairbanksAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(FairbanksAI);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];
		FairbanksAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 2;
			mPhase = 0;

			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(BLOOD);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].perctrigger = 15.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(PWS);
			spells[1].targettype = TARGET_SELF;
			spells[1].instant = true;
			spells[1].perctrigger = 15.0f;
			spells[1].attackstoptimer = 1000;
			Timer = 0;
		}

		void OnCombatStart(Unit* mTarget)
		{
			RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		}

		void OnTargetDied(Unit* mTarget)
		{

			if(_unit->GetHealthPct() > 0)
			{
				int RandomSpeach;
				RandomFloat(1000);
				RandomSpeach = rand() % 2;
				switch(RandomSpeach)
				{
					case 0:

						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ha! Had enough?");

						_unit->PlaySoundToSet(0000);
						break;

				}
			}
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
			Timer = Timer + 1;

			if(Timer == 20000)
			{
				_unit->CastSpell(_unit, spells[1].info, spells[1].instant);
			}

			else
			{
				float val = RandomFloat(100.0f);
				SpellCast(val);
			}
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

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
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
		uint32 mPhase;  // NPC has 2 phases
		int nrspells;
		int Timer;
};

class ScarletTorch : public GameObjectAIScript
{
	public:
		ScarletTorch(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
		static GameObjectAIScript* Create(GameObject* GO) { return new ScarletTorch(GO); }

		void OnActivate(Player* pPlayer)
		{
			GameObject* SecretDoor = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(1167.79f, 1347.26f, 31.5494f, 97700);
			if(SecretDoor != NULL)
			{
				if(SecretDoor->GetState() == 1)
					SecretDoor->SetState(0);
				else
					SecretDoor->SetState(1);
			}
		}
};

class ArmoryLever : public GameObjectAIScript
{
	public:
		ArmoryLever(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
		static GameObjectAIScript* Create(GameObject* GO) { return new ArmoryLever(GO); }

		void OnActivate(Player* pPlayer)
		{
			GameObject* ArmoryDoor = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(2886.31f, -827.261f, 160.336f, 101851);
			if(ArmoryDoor != NULL)
			{
				if(ArmoryDoor->GetState() == 1)
					ArmoryDoor->SetState(0);
				else
					ArmoryDoor->SetState(1);
			}
		}
};

class CathedralLever: public GameObjectAIScript
{
	public:
		CathedralLever(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
		static GameObjectAIScript* Create(GameObject* GO) { return new CathedralLever(GO); }

		void OnActivate(Player* pPlayer)
		{
			GameObject* CathedralDoor = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(2908.18f, -818.203f, 160.332f, 101850);
			if(CathedralDoor != NULL)
			{
				if(CathedralDoor->GetState() == 1)
					CathedralDoor->SetState(0);
				else
					CathedralDoor->SetState(1);
			}
		}
};

void SetupScarletMonastery(ScriptMgr* mgr)
{
	mgr->register_creature_script(LOKSEY, &LokseyAI::Create);
	mgr->register_creature_script(VISHAS, &VishasAI::Create);
	mgr->register_creature_script(THALNOS, &ThalnosAI::Create);
	mgr->register_creature_script(COMMANDER_MOGRAINE, &MograineAI::Create);
	mgr->register_creature_script(WHITEMANE, &WhitemaneAI::Create);
	mgr->register_creature_script(FAIRBANKS, &FairbanksAI::Create);
	mgr->register_creature_script(HEROD, &HerodAI::Create);
	mgr->register_creature_script(DOAN, &DoanAI::Create);
	mgr->register_gameobject_script(97701, &ScarletTorch::Create);
	mgr->register_gameobject_script(101853, &CathedralLever::Create);
	mgr->register_gameobject_script(101852, &ArmoryLever::Create);
}
