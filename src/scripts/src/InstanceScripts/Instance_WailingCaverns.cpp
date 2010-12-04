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
/* Instance_WailinCaverns.cpp Script		                               			     */
/************************************************************************/

// Devouring Ectoplasm AI by Soulshifter

#define CN_DEVOURING_ECTOPLASM		3638
class DevouringEctoplasmAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DevouringEctoplasmAI, MoonScriptCreatureAI);		
    DevouringEctoplasmAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		// Summon Evolving Ectoplasm
		AddSpell(7952, Target_Self, 10, 0, 600);	
	}
};

// Druid of the Fang AI by Soulshifter
#define CN_DRUID_FANG				3840
class DruidFangAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DruidFangAI, MoonScriptCreatureAI);	
    DruidFangAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		// Serpent Form
		SerpentForm = AddSpell(8041, Target_Self, 0, 0, 0);

		// Healing Touch
		HealingTouch = AddSpell(5187, Target_Self, 0, 2.5, 0);

		// Lightning Bolt
		LightningBolt = AddSpell(9532, Target_Current, 30, 3, 0);

		// Druid's Slumber
		DruidsSlumber = AddSpell(8040, Target_RandomPlayerNotCurrent, 20, 2.5, 0);
	}

    void AIUpdate()
    {
		if(GetHealthPercent() <= 50 && SerpentForm->mEnabled == true)
		{
			CastSpellNowNoScheduling(SerpentForm);
			SerpentForm->mEnabled = false;
			LightningBolt->mEnabled = false;
			DruidsSlumber->mEnabled = false;
		} // If they dont have serpent form aura then re-enable normal spells
		else if(SerpentForm->mEnabled == false && !GetUnit()->HasAura(8041))
		{
			LightningBolt->mEnabled = true;
			DruidsSlumber->mEnabled = true;
		}

		if(GetHealthPercent() <= 5 && HealingTouch->mEnabled == true)
		{
			// Remove Serpent Form
			RemoveAura(8041);
			CastSpellNowNoScheduling(HealingTouch);
			HealingTouch->mEnabled = false;
		}
		
		ParentClass::AIUpdate();
    }

	SpellDesc* SerpentForm;
	SpellDesc* LightningBolt;
	SpellDesc* DruidsSlumber;
	SpellDesc* HealingTouch;
};

// BOSSES

// Lady Anacondra AI by Soulshifter
#define CN_LADY_ANACONDRA			3671
#define SPELL_LIGHTNING_BOLT      	915 
#define SPELL_SLEEP			     	1090

class LadyAnacondraAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(LadyAnacondraAI, MoonScriptCreatureAI);	
    LadyAnacondraAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddEmote(Event_OnCombatStart, "None can stand against the Serpent Lords!", Text_Yell, 5786);
		// Lightning Bolt
		AddSpell(9532, Target_Current, 30, 3, 0);
		// Sleep
		AddSpell(700, Target_RandomPlayerNotCurrent, 10, 1.5, 20);
	}
};

// Lord Cobrahn AI by Soulshifter
#define CN_LORD_COBRAHN				3669
#define SPELL_LIGHTNING_BOLT      	915 
#define SPELL_POISON		     	34969
#define SPELL_SERPENTFORM			7965

class LordCobrahnAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(LordCobrahnAI, MoonScriptCreatureAI);
    LordCobrahnAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddEmote(Event_OnCombatStart, "You will never wake the dreamer!", Text_Yell, 5785);
		// Lightning Bolt
		LightningBolt = AddSpell(9532, Target_Current, 30, 3, 0);
		// Poison -- Spell ID Needs checked
		AddSpell(34969, Target_Current, 15, 0, 0);
		// Cobrahn Serpent Form
		SerpentForm = AddSpell(7965, Target_Self, 0, 0, 0);
	}

    void AIUpdate()
    {
		if(GetHealthPercent() <= 20 && SerpentForm->mEnabled == true)
		{
			CastSpellNowNoScheduling(SerpentForm);
			SerpentForm->mEnabled = false;
			// Disable Lightning Bolt
			LightningBolt->mEnabled = false;
		}
		else if(GetHealthPercent() <= 20 && SerpentForm->mEnabled == false && !GetUnit()->HasAura(7965))
		{
			// Enable Lightning Bolt
			LightningBolt->mEnabled = true;
		}
		ParentClass::AIUpdate();
    }
	SpellDesc* LightningBolt;
	SpellDesc* SerpentForm;
};

// Lord Pythas AI by Soulshifter
#define CN_LORD_PYTHAS				3670
class LordPythasAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(LordPythasAI, MoonScriptCreatureAI);
    LordPythasAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddEmote(Event_OnCombatStart, "The coils of death... Will crush you!", Text_Yell, 5787);
		// Lightning Bolt
		AddSpell(9532, Target_Current, 30, 3, 0);
		// Sleep
		AddSpell(700, Target_RandomPlayer, 10, 1.5, 0);
		// Thunderclap
		AddSpell(8147, Target_Self, 20, 0, 5);
	}
};

// Lord Serpentis AI by Soulshifter

#define CN_LORD_SERPENTIS			3673
class LordSerpentisAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(LordSerpentisAI, MoonScriptCreatureAI);
    LordSerpentisAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddEmote(Event_OnCombatStart, "I am the serpent king, i can do anything!", Text_Yell, 5788);
		// Lightning Bolt
		AddSpell(9532, Target_Current, 30, 3, 0);
		// Sleep
		AddSpell(700, Target_RandomPlayer, 10, 1.5, 0);		
	}
};

// Verdan the Everliving AI by Soulshifter

#define CN_VERDAN_EVERLIVING		5775
class VerdanEverlivingAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(VerdanEverlivingAI, MoonScriptCreatureAI);	
    VerdanEverlivingAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		// Grasping Vines
		AddSpell(8142, Target_Current, 30, 1, 0);		
	}
};

// Skum AI by Soulshifter
#define CN_SKUM						3674
class SkumAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SkumAI, MoonScriptCreatureAI);
    SkumAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		// Chained Bolt
		AddSpell(6254, Target_Current, 50, 1.8f, 0);			
	}
   
    void AIUpdate()
    {
		if(GetHealthPercent() <= 10 && GetBehavior() != Behavior_Flee)
		{
			Emote("Skum tries to run away in fear", Text_Emote);
			SetBehavior(Behavior_Flee);
			SetAllowMelee(false);
			SetAllowRanged(false);
			SetAllowSpell(false);
			MoveTo(-262.829742f, -299.363159f, -68.293579f, true);
		}
		ParentClass::AIUpdate();
	}
};

// Mutanus the Devourer AI by Soulshifter
#define CN_MUTANUS				3654
class MutanusAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MutanusAI, MoonScriptCreatureAI);		
    MutanusAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		// Thundercrack
		AddSpell(8150, Target_Self, 15, 0, 0);
		// Terrify
		AddSpell(7399, Target_RandomPlayer, 15, 0, 4);		
	}
};



// Wailing Caverns Event
// Discipline of Naralex Gossip by Soulshifter
#define CN_NARALEX				3679
#define CN_DIS_NARALEX			3678	
static Location ToNaralex[] = 
{
	{  },
	{ -132.498077f, 125.888153f, -78.418182f, 0.244260f },
	{ -123.892235f, 130.538422f, -78.808937f, 0.519935f },
	{ -116.654480f, 142.935806f, -80.233383f, 1.149039f },
	{ -111.656868f, 156.927307f, -79.880676f, 1.344603f },
	{ -108.829506f, 169.213165f, -79.752487f, 1.344603f },
	{ -107.592789f, 183.854782f, -79.735558f, 1.500112f },
	{ -106.628258f, 197.477676f, -80.526184f, 1.500112f },
	{ -109.725700f, 215.487885f, -85.336075f, 1.650287f },
	{ -106.663147f, 225.879135f, -88.962914f, 0.215201f },
	{ -90.607216f, 228.829071f, -91.022133f, 6.067203f },
	{ -79.377800f, 219.999466f, -93.990906f, 5.482866f },
	{ -69.134697f, 209.446045f, -93.404358f, 5.482866f },
	{ -53.198994f, 204.919601f, -95.677971f, 6.071915f },
	{ -38.501598f, 211.024460f, -96.222626f, 0.559205f },
	{ -39.211544f, 197.527161f, -96.574646f, 4.658991f },
	{ -40.258022f, 177.948105f, -96.374756f, 4.658991f },
	{ -41.385948f, 156.845230f, -94.969429f, 4.658991f },
	{ -49.557240f, 145.598206f, -93.284225f, 4.084079f },
	{ -52.191185f, 133.269424f, -90.334198f, 4.501911f },
	{ -53.070702f, 122.185814f, -89.757874f, 5.128569f },
	{ -47.618214f, 115.986847f, -87.939827f, 5.562199f },
	{ -36.105568f, 109.539597f, -87.755760f, 5.772686f },
	{ -23.849794f, 109.712982f, -89.580704f, 0.014146f },
	{ -15.070121f, 119.703346f, -89.904770f, 0.849840f },
	{ -6.799855f, 134.471298f, -89.574089f, 1.060297f },
	{ 1.530990f, 143.322433f, -89.091454f, 0.589058f },
	{ 11.134405f, 149.739365f, -88.872955f, 0.589058f },
	{ 21.220901f, 156.479080f, -89.180771f, 0.802686f },
	{ 31.682161f, 167.308456f, -88.896530f, 0.802686f },
	{ 43.933167f, 179.990555f, -88.922348f, 0.802686f },
	{ 51.662514f, 195.831421f, -89.649101f, 1.116846f },
	{ 70.554794f, 204.757950f, -92.880386f, 0.441403f },
	{ 85.016724f, 211.591156f, -92.615730f, 0.441403f },
	{ 99.523796f, 213.738951f, -96.214615f, 0.047919f },
	{ 112.235191f, 214.378525f, -98.332832f, 0.679379f },
	{ 118.665100f, 220.504974f, -98.305420f, 1.254290f },
	{ 121.259758f, 228.493378f, -97.359711f, 1.632852f },
	{ 116.031120f, 236.451187f, -96.007195f, 3.089230f }
};

class DofNaralexGossip : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* Plr, bool AutoSend)
	{
		Unit* Fanglord1 = pObject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-151.139008f, 414.367004f, -72.629402f, CN_LORD_COBRAHN);
		Unit* Fanglord2 = pObject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(36.807400f, -241.063995f, -79.498901f, CN_LORD_PYTHAS);
		Unit* Fanglord3 = pObject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-118.710999f, -24.990999f, -28.498501f, CN_LORD_SERPENTIS);
		Unit* Fanglord4 = pObject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-70.788902f, 120.072998f, -89.673599f, CN_LADY_ANACONDRA);
			
		if((!Fanglord1 || !Fanglord1->isAlive()) && (!Fanglord2 || !Fanglord2->isAlive()) && (!Fanglord3 || !Fanglord3->isAlive()) && (!Fanglord4 || !Fanglord4->isAlive()))
		{
			GossipMenu *Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 699, Plr);
			Menu->AddItem(0, "Let's go!", 2);
			Menu->SendTo(Plr);
		}
		else
		{
			GossipMenu *Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 698, Plr);
			Menu->AddItem(0, "I will slay those Fanglords", 1);
			Menu->SendTo(Plr);
		}	
			
	}
	void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		Creature* pCreature = (pObject->IsCreature()) ? TO_CREATURE(pObject) : NULL;
		if(pCreature == NULL)
			return;

		switch(IntId)
		{
		case 0: // Return to start

			GossipHello(pCreature, Plr, true);
			break;

		case 1: // Disciple of Naralex Casts Mark of the Wild on players.
			{
				pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Take this! It will be useful for you. I'll be waiting here when you have slain the 4 Fanglords to awake Naralex!");
				pCreature->CastSpell(Plr, 5232, true);
				pCreature->Emote(EMOTE_ONESHOT_CHEER);
				Plr->Gossip_Complete();
			}
			break;

		case 2: // Start Event
			{
				pCreature->SetUInt32Value(UNIT_NPC_FLAGS, 0);
				pCreature->GetAIInterface()->StopMovement(0);
				pCreature->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
				pCreature->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
				pCreature->GetAIInterface()->setWaypointToMove(2);		
			}
			break;
		}

	}

};

// Disciple of Naralex AI by Soulshifter
class DofNaralexAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(DofNaralexAI, MoonScriptBossAI);
    DofNaralexAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
    {
		// --- Initialization ---
		for (int i = 1; i < 39; i++)
			AddWaypoint(CreateWaypoint(i, 0, Flag_Run, ToNaralex[i]));
		SetMoveType(Move_DontMoveWP);

		// Awakening Spell
		Awakening = AddSpell(6271, Target_Self, 0, 0, 0, 0, 0, false, "Step back and be ready!, I'll try to Awake Naralex", Text_Say);

		SpawnTimer = NULL;
	}
    
	void OnReachWP(uint32 iWaypointId, bool bForwards)
    {
		ForceWaypointMove(iWaypointId+1);
		if(GetPhase() == 1 && GetCurrentWaypoint() == 39)		
		{
			_unit->Emote(EMOTE_ONESHOT_TALK);
			SetPhase(2, Awakening);
			SpawnTimer = AddTimer(100000);
		}
		ParentClass::OnReachWP(iWaypointId, bForwards);
	}

	void AIUpdate()
	{
		if(SpawnTimer && IsTimerFinished(SpawnTimer))
		{
			switch(GetPhase())
			{
				case 2: Moccasin(); ResetTimer(SpawnTimer,100000); SetPhase(3); break;
				case 3: Ectoplasm(); ResetTimer(SpawnTimer,100000); SetPhase(4); break;
				case 4: BMutanus(); ResetTimer(SpawnTimer,100000); SetPhase(5); break;
			}
		}
		if(GetPhase() == 5 && (!Mutanus || !Mutanus->GetUnit()->isAlive()))
		{
			MoonScriptCreatureAI* Naralex = GetNearestCreature(3679);
			if(Naralex && Naralex->IsAlive())
			{
				SetDisplayId(17089);
				Naralex->SetDisplayId(17089);
				Naralex->Emote("I am awake... at last", Text_Say, 5789);
				Naralex->GetUnit()->SetStandState(STANDSTATE_STAND);
				SetFlyMode(true);
				Naralex->SetFlyMode(true);
				MoveTo(-6.704030f, 200.308838f, -26.938824f);
				Naralex->MoveTo(-6.704030f, 200.308838f, -26.938824f);
			}
			SetPhase(6);
		}
		ParentClass::AIUpdate();
	}
		
	void Moccasin()
	{
		MoonScriptCreatureAI* Moccasin = NULL;
		Moccasin = SpawnCreature(5762, 134.249207f, 242.194839f, -98.375496f, 3.325373f);
		Moccasin = SpawnCreature(5762, 124.917931f, 255.066635f, -97.796837f, 4.176745f);
		Moccasin = SpawnCreature(5762, 113.077148f, 258.880157f, -97.190590f, 4.688039f);
	}

	void Ectoplasm()
	{
		MoonScriptCreatureAI* Ectoplasm = NULL;
		Ectoplasm = SpawnCreature(5763, 134.249207f, 242.194839f, -98.375496f, 3.325373f);
		Ectoplasm = SpawnCreature(5763, 124.917931f, 255.066635f, -97.796837f, 4.176745f);
		Ectoplasm = SpawnCreature(5763, 113.077148f, 258.880157f, -97.190590f, 4.688039f);
		Ectoplasm = SpawnCreature(5763, 138.794693f, 228.224976f, -100.174332f, 2.471645f);
		Ectoplasm = SpawnCreature(5763, 128.170364f, 225.190247f, -99.392830f, 2.411169f);
		Ectoplasm = SpawnCreature(5763, 136.762009f, 242.685669f, -98.564545f, 3.344223f);
		Ectoplasm = SpawnCreature(5763, 122.403961f, 259.438354f, -98.153984f, 4.366811f);
	}

	void BMutanus()
	{
		Mutanus = SpawnCreature(CN_MUTANUS, 136.337006f, 263.769989f, -102.666000f, 4.002330f);
	}

	int32 SpawnTimer;
	SpellDesc* Awakening;
	MoonScriptCreatureAI* Mutanus;
};
// Deviate Moccasin
class DeviateMoccasinAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DeviateMoccasinAI, MoonScriptCreatureAI);
	DeviateMoccasinAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
	}
};

// Nightmare Ectoplasm
class EctoplasmAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(EctoplasmAI, MoonScriptCreatureAI);
	EctoplasmAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
	}
};

// Naralex State
class Naralex : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(Naralex, MoonScriptCreatureAI);
	Naralex(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		_unit->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		_unit->SetStandState(STANDSTATE_SLEEP);
	}
};

void SetupWailingCaverns(ScriptMgr * mgr)
{
    mgr->register_creature_script(CN_DRUID_FANG, &DruidFangAI::Create);
	mgr->register_creature_script(CN_DEVOURING_ECTOPLASM, &DevouringEctoplasmAI::Create);
	mgr->register_creature_script(CN_LADY_ANACONDRA, &LadyAnacondraAI::Create);
	mgr->register_creature_script(CN_LORD_COBRAHN, &LordCobrahnAI::Create);
	mgr->register_creature_script(CN_LORD_PYTHAS, &LordPythasAI::Create);
	mgr->register_creature_script(CN_LORD_SERPENTIS, &LordSerpentisAI::Create);
	mgr->register_creature_script(CN_VERDAN_EVERLIVING, &VerdanEverlivingAI::Create);
	mgr->register_creature_script(CN_SKUM, &SkumAI::Create);
	mgr->register_creature_script(CN_MUTANUS, &MutanusAI::Create);
	GossipScript * DNaralex = new DofNaralexGossip();
	mgr->register_gossip_script(CN_DIS_NARALEX, DNaralex);
	mgr->register_creature_script(CN_DIS_NARALEX, &DofNaralexAI::Create);
	mgr->register_creature_script(CN_NARALEX, &Naralex::Create);

	// Might be easier to merge this 2 into 1 since they are moving to the same locations..
	mgr->register_creature_script(5762, &DeviateMoccasinAI::Create);
	mgr->register_creature_script(5763, &EctoplasmAI::Create);
}