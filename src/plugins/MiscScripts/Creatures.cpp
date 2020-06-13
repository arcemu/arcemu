/*
* Moon++ Scripts for Ascent MMORPG Server
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

//Crimson Hammersmith
class CrimsonHammersmith : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(CrimsonHammersmith);
		CrimsonHammersmith(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnCombatStart(Unit* mTarget)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Who Dares Disturb Me");
		}
};

//Corrupt Minor Manifestation Water Dead
class Corrupt_Minor_Manifestation_Water_Dead : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(Corrupt_Minor_Manifestation_Water_Dead);
		Corrupt_Minor_Manifestation_Water_Dead(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnDied(Unit* mKiller)
		{
			float SSX = _unit->GetPositionX();
			float SSY = _unit->GetPositionY();
			float SSZ = _unit->GetPositionZ();
			float SSO = _unit->GetOrientation();

			Creature* NewCreature = _unit->GetMapMgr()->GetInterface()->SpawnCreature(5895, SSX, SSY + 1, SSZ, SSO, true, false, 0, 0);
			if(NewCreature != NULL)
				NewCreature->Despawn(600000, 0);
		}
};

class SavannahProwler : public CreatureAIScript
{
	public:
		SavannahProwler(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnLoad()
		{
			uint8 chance = RandomUInt(3);

			if(chance == 1)
				_unit->SetStandState(STANDSTATE_SLEEP);
		}

		void OnCombatStart(Unit* pTarget)
		{
			if(_unit->GetStandState() == STANDSTATE_SLEEP)
				_unit->SetStandState(0);
		}

		static CreatureAIScript* Create(Creature* c) { return new SavannahProwler(c); }
};

//Lazy Peons
class PeonSleepingAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(PeonSleepingAI);
		PeonSleepingAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			RegisterAIUpdateEvent(3000 + RandomUInt(180000));
		};

		void AIUpdate()
		{
			_unit->CastSpell(_unit, 17743, true);
			RemoveAIUpdateEvent();
		};
};

class KirithAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(KirithAI);
		KirithAI(Creature* pCreature) : CreatureAIScript(pCreature)  {}

		void OnDied(Unit* mKiller)
		{
			if(mKiller->IsPlayer())
			{
				Creature* NewCreature = _unit->GetMapMgr()->GetInterface()->SpawnCreature(7729, _unit->GetPositionX() + 2, _unit->GetPositionY() + 2, _unit->GetPositionZ(), _unit->GetOrientation(), true, false, 0, 0);
				if(NewCreature != NULL)
					NewCreature->Despawn(3 * 6 * 1000, 0);
			}
		}
};

class AllianceGryphon : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(AllianceGryphon);

		AllianceGryphon(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnCombatStart(Unit* mTarget)
		{
			if(!mTarget->IsPlayer())
				return;

			Creature* NewCreature = _unit->GetMapMgr()->GetInterface()->SpawnCreature(9526, _unit->GetPositionX() + RandomFloat(5.0f), _unit->GetPositionY() + RandomFloat(5.0f), _unit->GetPositionZ(), _unit->GetOrientation(), true, false, 0, 0);
			if(NewCreature != NULL)
				NewCreature->Despawn(360000, 0);

			NewCreature = _unit->GetMapMgr()->GetInterface()->SpawnCreature(9526, _unit->GetPositionX() - RandomFloat(5.0f), _unit->GetPositionY() - RandomFloat(5.0f), _unit->GetPositionZ(), _unit->GetOrientation(), true, false, 0, 0);
			if(NewCreature != NULL)
				NewCreature->Despawn(360000, 0);
		}
};

class AllianceHippogryph : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(AllianceHippogryph);

		AllianceHippogryph(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnCombatStart(Unit* mTarget)
		{
			if(!mTarget->IsPlayer())
				return;

			Creature* NewCreature = _unit->GetMapMgr()->GetInterface()->SpawnCreature(9527, _unit->GetPositionX() + RandomFloat(5.0f), _unit->GetPositionY() + RandomFloat(5.0f), _unit->GetPositionZ(), _unit->GetOrientation(), true, false, 0, 0);
			if(NewCreature != NULL)
				NewCreature->Despawn(360000, 0);

			NewCreature = _unit->GetMapMgr()->GetInterface()->SpawnCreature(9527, _unit->GetPositionX() - RandomFloat(5.0f), _unit->GetPositionY() - RandomFloat(5.0f), _unit->GetPositionZ(), _unit->GetOrientation(), true, false, 0, 0);
			if(NewCreature != NULL)
				NewCreature->Despawn(360000, 0);
		}
};

class HordeWyvern : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(HordeWyvern);
		HordeWyvern(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnCombatStart(Unit* mTarget)
		{
			if(!mTarget->IsPlayer())
				return;

			Creature* NewCreature = _unit->GetMapMgr()->GetInterface()->SpawnCreature(9297, _unit->GetPositionX() + RandomFloat(5.0f), _unit->GetPositionY() + RandomFloat(5.0f), _unit->GetPositionZ(), _unit->GetOrientation(), true, false, 0, 0);
			if(NewCreature != NULL)
				NewCreature->Despawn(360000, 0);

			NewCreature = _unit->GetMapMgr()->GetInterface()->SpawnCreature(9297, _unit->GetPositionX() - RandomFloat(5.0f), _unit->GetPositionY() - RandomFloat(5.0f), _unit->GetPositionZ(), _unit->GetOrientation(), true, false, 0, 0);
			if(NewCreature != NULL)
				NewCreature->Despawn(360000, 0);
		}
};

class HordeBat : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(HordeBat);
		HordeBat(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnCombatStart(Unit* mTarget)
		{
			if(!mTarget->IsPlayer())
				return;

			Creature* NewCreature = _unit->GetMapMgr()->GetInterface()->SpawnCreature(9521, _unit->GetPositionX() + RandomFloat(5.0f), _unit->GetPositionY() + RandomFloat(5.0f), _unit->GetPositionZ(), _unit->GetOrientation(), true, false, 0, 0);
			if(NewCreature != NULL)
				NewCreature->Despawn(360000, 0);

			_unit->GetMapMgr()->GetInterface()->SpawnCreature(9521, _unit->GetPositionX() - RandomFloat(5.0f), _unit->GetPositionY() - RandomFloat(5.0f), _unit->GetPositionZ(), _unit->GetOrientation(), true, false, 0, 0);
			if(NewCreature != NULL)
				NewCreature->Despawn(360000, 0);
		}
};

class TyrandeWhisperwind : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(TyrandeWhisperwind);
		TyrandeWhisperwind(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnCombatStart(Unit* mTarget)
		{
			_unit->PlaySoundToSet(5885);
		}
};

/*--------------------------------------------------------------------------------------------------------*/

class ProphetVelen : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(ProphetVelen);
		ProphetVelen(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnCombatStart(Unit* mTarget)
		{
			_unit->PlaySoundToSet(10155);
		}
};

/*--------------------------------------------------------------------------------------------------------*/

class KingMagniBronzebeard : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(KingMagniBronzebeard);
		KingMagniBronzebeard(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnCombatStart(Unit* mTarget)
		{
			_unit->PlaySoundToSet(5896);
		}
};

/*--------------------------------------------------------------------------------------------------------*/

class Thrall : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(Thrall);
		Thrall(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnCombatStart(Unit* mTarget)
		{
			_unit->PlaySoundToSet(5880);
		}
};

/*--------------------------------------------------------------------------------------------------------*/

class CairneBloodhoof : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(CairneBloodhoof);
		CairneBloodhoof(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnCombatStart(Unit* mTarget)
		{
			_unit->PlaySoundToSet(5884);
		}
};

/*--------------------------------------------------------------------------------------------------------*/

class LadySylvanasWindrunner : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(LadySylvanasWindrunner);
		LadySylvanasWindrunner(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnCombatStart(Unit* mTarget)
		{
			_unit->PlaySoundToSet(5886);
		}
};

/*--------------------------------------------------------------------------------------------------------*/

class TrollRoofStalker : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(TrollRoofStalker);
		TrollRoofStalker(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnLoad()
		{
			_unit->CastSpell(_unit, 30991, true);
		};
};

///////////////////////////////////////////////////////////
//D.I.S.C.O AI Script ( entry 27989 )
//"Dancer's Integrated Sonic Celebration Oscillator"
//
//Behavior
//  On spawn it casts 2 spells, which
//  - summon the dancefloor
//  - applies a periodic aura that plays the music
//
//
//////////////////////////////////////////////////////////
class DISCO : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(DISCO);
		DISCO(Creature* c) : CreatureAIScript(c) {}

		void OnLoad()
		{
			_unit->CastSpell(_unit, 50487, false);   // summon disco dancefloor
			_unit->CastSpell(_unit, 50314, false);   // play the music
		}
};

void SetupMiscCreatures(ScriptMgr* mgr)
{
	mgr->register_creature_script(11120, &CrimsonHammersmith::Create);
	mgr->register_creature_script(5894, &Corrupt_Minor_Manifestation_Water_Dead::Create);
	mgr->register_creature_script(3425, &SavannahProwler::Create);
	mgr->register_creature_script(10556, &PeonSleepingAI::Create);
	mgr->register_creature_script(7728, &KirithAI::Create);

	// Gryphon Master
	mgr->register_creature_script(8609, &AllianceGryphon::Create);
	mgr->register_creature_script(18931, &AllianceGryphon::Create);
	mgr->register_creature_script(931, &AllianceGryphon::Create);
	mgr->register_creature_script(4321, &AllianceGryphon::Create);
	mgr->register_creature_script(7823, &AllianceGryphon::Create);
	mgr->register_creature_script(12596, &AllianceGryphon::Create);
	mgr->register_creature_script(2299, &AllianceGryphon::Create);
	mgr->register_creature_script(2835, &AllianceGryphon::Create);
	mgr->register_creature_script(2432, &AllianceGryphon::Create);
	mgr->register_creature_script(352, &AllianceGryphon::Create);
	mgr->register_creature_script(2409, &AllianceGryphon::Create);
	mgr->register_creature_script(16822, &AllianceGryphon::Create);
	mgr->register_creature_script(18809, &AllianceGryphon::Create);
	mgr->register_creature_script(1573, &AllianceGryphon::Create);
	mgr->register_creature_script(2859, &AllianceGryphon::Create);
	mgr->register_creature_script(12617, &AllianceGryphon::Create);
	mgr->register_creature_script(2941, &AllianceGryphon::Create);
	mgr->register_creature_script(21107, &AllianceGryphon::Create);
	mgr->register_creature_script(20234, &AllianceGryphon::Create);
	mgr->register_creature_script(1571, &AllianceGryphon::Create);
	mgr->register_creature_script(523, &AllianceGryphon::Create);
	mgr->register_creature_script(1572, &AllianceGryphon::Create);
	mgr->register_creature_script(18939, &AllianceGryphon::Create);
	mgr->register_creature_script(8018, &AllianceGryphon::Create);
	mgr->register_creature_script(24366, &AllianceGryphon::Create);

	// Hippogryph Master
	mgr->register_creature_script(18937, &AllianceHippogryph::Create);
	mgr->register_creature_script(6706, &AllianceHippogryph::Create);
	mgr->register_creature_script(3841, &AllianceHippogryph::Create);
	mgr->register_creature_script(15177, &AllianceHippogryph::Create);
	mgr->register_creature_script(4267, &AllianceHippogryph::Create);
	mgr->register_creature_script(18789, &AllianceHippogryph::Create);
	mgr->register_creature_script(8019, &AllianceHippogryph::Create);
	mgr->register_creature_script(22485, &AllianceHippogryph::Create);
	mgr->register_creature_script(12577, &AllianceHippogryph::Create);
	mgr->register_creature_script(18785, &AllianceHippogryph::Create);
	mgr->register_creature_script(11138, &AllianceHippogryph::Create);
	mgr->register_creature_script(12578, &AllianceHippogryph::Create);
	mgr->register_creature_script(18788, &AllianceHippogryph::Create);
	mgr->register_creature_script(10897, &AllianceHippogryph::Create);
	mgr->register_creature_script(4407, &AllianceHippogryph::Create);
	mgr->register_creature_script(4319, &AllianceHippogryph::Create);
	mgr->register_creature_script(17554, &AllianceHippogryph::Create);
	mgr->register_creature_script(17555, &AllianceHippogryph::Create);
	mgr->register_creature_script(22935, &AllianceHippogryph::Create);
	mgr->register_creature_script(3838, &AllianceHippogryph::Create);
	mgr->register_creature_script(11800, &AllianceHippogryph::Create);

	// Wyvern Master
	mgr->register_creature_script(19558, &HordeWyvern::Create);
	mgr->register_creature_script(11901, &HordeWyvern::Create);
	mgr->register_creature_script(16587, &HordeWyvern::Create);
	mgr->register_creature_script(6026, &HordeWyvern::Create);
	mgr->register_creature_script(7824, &HordeWyvern::Create);
	mgr->register_creature_script(3615, &HordeWyvern::Create);
	mgr->register_creature_script(3310, &HordeWyvern::Create);
	mgr->register_creature_script(12740, &HordeWyvern::Create);
	mgr->register_creature_script(2861, &HordeWyvern::Create);
	mgr->register_creature_script(2858, &HordeWyvern::Create);
	mgr->register_creature_script(3305, &HordeWyvern::Create);
	mgr->register_creature_script(20762, &HordeWyvern::Create);
	mgr->register_creature_script(18808, &HordeWyvern::Create);
	mgr->register_creature_script(18942, &HordeWyvern::Create);
	mgr->register_creature_script(18807, &HordeWyvern::Create);
	mgr->register_creature_script(8610, &HordeWyvern::Create);
	mgr->register_creature_script(4317, &HordeWyvern::Create);
	mgr->register_creature_script(10378, &HordeWyvern::Create);
	mgr->register_creature_script(15178, &HordeWyvern::Create);
	mgr->register_creature_script(22455, &HordeWyvern::Create);
	mgr->register_creature_script(2995, &HordeWyvern::Create);
	mgr->register_creature_script(6726, &HordeWyvern::Create);
	mgr->register_creature_script(4312, &HordeWyvern::Create);
	mgr->register_creature_script(1387, &HordeWyvern::Create);
	mgr->register_creature_script(18953, &HordeWyvern::Create);
	mgr->register_creature_script(2851, &HordeWyvern::Create);
	mgr->register_creature_script(13177, &HordeWyvern::Create);
	mgr->register_creature_script(12616, &HordeWyvern::Create);
	mgr->register_creature_script(18930, &HordeWyvern::Create);
	mgr->register_creature_script(11139, &HordeWyvern::Create);
	mgr->register_creature_script(11900, &HordeWyvern::Create);
	mgr->register_creature_script(19317, &HordeWyvern::Create);
	mgr->register_creature_script(18791, &HordeWyvern::Create);
	mgr->register_creature_script(4314, &HordeWyvern::Create);
	mgr->register_creature_script(11899, &HordeWyvern::Create);
	mgr->register_creature_script(8020, &HordeWyvern::Create);

	// Bat Master
	mgr->register_creature_script(12636, &HordeBat::Create);
	mgr->register_creature_script(2226, &HordeBat::Create);
	mgr->register_creature_script(4551, &HordeBat::Create);
	mgr->register_creature_script(2389, &HordeBat::Create);

	mgr->register_creature_script(7999, &TyrandeWhisperwind::Create);
	mgr->register_creature_script(17468, &ProphetVelen::Create);
	mgr->register_creature_script(2784, &KingMagniBronzebeard::Create);
	mgr->register_creature_script(4949, &Thrall::Create);
	mgr->register_creature_script(3057, &CairneBloodhoof::Create);
	mgr->register_creature_script(10181, &LadySylvanasWindrunner::Create);

	mgr->register_creature_script(23090, &TrollRoofStalker::Create);

	mgr->register_creature_script(27989, &DISCO::Create);
}
