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
/* Instance_HellfireRamparts.cpp Script		                            */
/************************************************************************/

/*****************************/
/*                           */
/*         Boss AIs          */
/*                           */
/*****************************/

// Watchkeeper GargolmarAI
// "Do you smell that? Fresh meat has somehow breached our citadel. Be wary of any intruders." should be on some areatrigger

#define CN_WATCHKEEPER_GARGOLMAR 17306

#define SURGE			34645
#define MORTAL_WOUND	30641
#define MORTAL_WOUND_H	36814
#define	OVERPOWER		32154
#define RETALIATION		22857

class WatchkeeperGargolmarAI : public MoonScriptBossAI
{
		MOONSCRIPT_FACTORY_FUNCTION(WatchkeeperGargolmarAI, MoonScriptBossAI);
		WatchkeeperGargolmarAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
		{
			AddSpell(SURGE, Target_RandomUnit, 20, 0, 15, 5, 40, false, "Back off, pup!", Text_Yell, 10330);
			AddSpell(OVERPOWER, Target_Current, 10, 0, 5);
			mRetaliation = AddSpell(RETALIATION, Target_Self, 0, 0, 0);

			if(IsHeroic())
				AddSpell(MORTAL_WOUND_H, Target_Current, 15, 0, 12);
			else
				AddSpell(MORTAL_WOUND, Target_Current, 15, 0, 12);

			AddEmote(Event_OnCombatStart, "What do we have here? ...", Text_Yell, 10331);
			AddEmote(Event_OnCombatStart, "Heh... this may hurt a little.", Text_Yell, 10332);
			AddEmote(Event_OnCombatStart, "I'm gonna enjoy this!", Text_Yell, 10333);
			AddEmote(Event_OnTargetDied, "Say farewell!", Text_Yell, 10334);
			AddEmote(Event_OnTargetDied, "Much too easy!", Text_Yell, 10335);

			mCalledForHelp = 0;
			_retaliation = false;
		};

		void OnDied(Unit* mKiller)
		{
			_unit->PlaySoundToSet(10336);
			ParentClass::OnDied(mKiller);
		};

		void AIUpdate()
		{
			if(_unit->GetHealthPct() <= 40 && !mCalledForHelp)
			{
				Emote("Heal me! QUICKLY!", Text_Yell, 10329);
				mCalledForHelp = true;
			};

			if(_unit->GetHealthPct() <= 20 && !_retaliation)
			{
				_retaliation = true;
				_unit->setAttackTimer(1500, false);
				CastSpellNowNoScheduling(mRetaliation);
			};

			ParentClass::AIUpdate();
		};

		bool		mCalledForHelp;
		bool		_retaliation;
		SpellDesc*	mRetaliation;
};


//Omor the Unscarred
#define CN_OMOR_THE_UNSCARRED						17308
#define OMOR_THE_UNSCARRED_DEMONIC_SHIELD			31901
#define OMOR_THE_UNSCARRED_SUMMON_FIENDISH_HOUND	30707
#define OMOR_THE_UNSCARRED_SHADOW_WHIP				30638
#define OMOR_THE_UNSCARRED_SHADOW_BOLT				30686
#define OMOR_THE_UNSCARRED_SHADOW_BOLT2				39297
#define OMOR_THE_UNSCARRED_TREACHEROUS_AURA			30695
#define OMOR_THE_UNSCARRED_BANE_OF_TREACHERY		37566

class OmorTheUnscarredAI : public MoonScriptCreatureAI
{
	public:
		MOONSCRIPT_FACTORY_FUNCTION(OmorTheUnscarredAI, MoonScriptCreatureAI);
		OmorTheUnscarredAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			SpellDesc* pShield = AddSpell(OMOR_THE_UNSCARRED_DEMONIC_SHIELD, Target_Self, 30, 0, 25);
			pShield->mEnabled = false;
			SpellDesc* pSummon = AddSpell(OMOR_THE_UNSCARRED_SUMMON_FIENDISH_HOUND, Target_Self, 8, 1, 20);
			pSummon->AddEmote("Achor-she-ki! Feast my pet! Eat your fill!", Text_Yell, 10277);
			AddSpell(OMOR_THE_UNSCARRED_SHADOW_WHIP, Target_RandomPlayer, 10, 0, 30);
			if(_unit->GetMapMgr()->iInstanceMode != MODE_HEROIC)
			{
				AddSpell(OMOR_THE_UNSCARRED_SHADOW_BOLT, Target_RandomPlayer, 8, 3, 15, 10, 60, true);
				SpellDesc* pAura = AddSpell(OMOR_THE_UNSCARRED_TREACHEROUS_AURA, Target_RandomPlayer, 8, 2, 35, 0, 60, true);
				pAura->AddEmote("A-Kreesh!", Text_Yell, 10278);
			}
			else
			{
				AddSpell(OMOR_THE_UNSCARRED_SHADOW_BOLT2, Target_RandomPlayer, 8, 3, 15, 10, 60, true);
				SpellDesc* pAura = AddSpell(OMOR_THE_UNSCARRED_BANE_OF_TREACHERY, Target_RandomPlayer, 8, 2, 35, 0, 60, true);
				pAura->AddEmote("A-Kreesh!", Text_Yell, 10278);
			}

			AddEmote(Event_OnCombatStart, "I will not be defeated!", Text_Yell, 10279);
			AddEmote(Event_OnCombatStart, "You dare stand against me?", Text_Yell, 10280);	// corrections
			AddEmote(Event_OnCombatStart, "Your incidents will be your death!", Text_Yell, 10281);
			AddEmote(Event_OnTargetDied, "Die weakling!", Text_Yell, 10282);
			AddEmote(Event_OnDied, "It is... not over.", Text_Yell, 10284);
		}

		void OnCombatStart(Unit* pTarget)
		{
			ParentClass::OnCombatStart(pTarget);
			SetCanMove(false);
		}

		void OnCombatStop(Unit* pTarget)
		{
			ParentClass::OnCombatStop(pTarget);
			if(IsAlive())
			{
				Emote("I am victorious!", Text_Yell, 10283);
			}
		}

		void AIUpdate()
		{
			SpellDesc* pShield = FindSpellById(OMOR_THE_UNSCARRED_DEMONIC_SHIELD);
			if(GetHealthPercent() <= 20 && pShield != NULL && !pShield->mEnabled)
			{
				pShield->mEnabled = true;
			}

			Unit* pTarget = _unit->GetAIInterface()->getNextTarget();
			if(pTarget != NULL)
			{
				if(GetRangeToUnit(pTarget) > 10.0f)
				{
					pTarget = GetBestPlayerTarget(TargetFilter_Closest);
					if(pTarget != NULL)
					{
						if(GetRangeToUnit(pTarget) > 10.0f)
						{
							pTarget = NULL;
						}
						else
						{
							ClearHateList();
							_unit->GetAIInterface()->AttackReaction(pTarget, 500);
							_unit->GetAIInterface()->setNextTarget(pTarget);
						}
					}
					else
						return;
				}

				if(pTarget == NULL)
				{
					SpellDesc* pWhip = FindSpellById(OMOR_THE_UNSCARRED_SHADOW_WHIP);	// used for now
					if(pWhip != NULL)
					{
						pWhip->mLastCastTime = 0;
						CastSpellNowNoScheduling(pWhip);
						return;
					}
				}
			}

			ParentClass::AIUpdate();
			SetCanMove(false);
		}
};

void SetupHellfireRamparts(ScriptMgr* mgr)
{
	mgr->register_creature_script(CN_WATCHKEEPER_GARGOLMAR, &WatchkeeperGargolmarAI::Create);
	mgr->register_creature_script(CN_OMOR_THE_UNSCARRED, &OmorTheUnscarredAI::Create);
}
