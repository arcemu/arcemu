/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2005-2007 Arcemu Team 
 * Copyright (C) 2007 Moon++ Team <http://www.moonplusplus.com/>
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

#include "StdAfx.h"
#include "Setup.h"
#include "Base.h"

/*

//Kil'Jaeden sound IDs saved for future use
//Some of them are used anytime during the raid progression in the instance (no mechanism for this yet)
//Some others are used in the actual Kil'Jaeden encounter

12527 Sunwell Plateau - Kil Jaeden "Spawning"
12495 Sunwell Plateau - Kil Jaeden - "All my plans have led to this"
12496 Sunwell Plateau - Kil Jaeden - "Stay on task, do not waste time"
12497 Sunwell Plateau - Kil Jaeden - "I've waited long enough!"
12498 Sunwell Plateau - Kil Jaeden - "Fail me, and suffer for eternity!"
12499 Sunwell Plateau - Kil Jaeden - "Drain the girl, drain her power, untill there is nothing but an ...something... shell"
12500 Sunwell Plateau - Kil Jaeden - Very long thing, basiclly he tells us that he will take control over the Burning Legion.
12501 Sunwell Plateau - Kil Jaeden - "Another step towards destruction!"

*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Sunblade Protector
#define CN_SUNBLADE_PROTECTOR				25507
#define SUNBLADE_PROTECTOR_FEL_LIGHTNING	46480

class SunbladeProtectorAI : public ArcScriptCreatureAI
{
	ARCSCRIPT_FACTORY_FUNCTION(SunbladeProtectorAI, ArcScriptCreatureAI);
	SunbladeProtectorAI(Creature* pCreature) : ArcScriptCreatureAI(pCreature)
	{
		AddSpell(SUNBLADE_PROTECTOR_FEL_LIGHTNING, Target_RandomPlayer, 100, 0, 15, 0, 60);
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Shadowsword Assassin
#define CN_SHADOWSWORD_ASSASSIN						25484
#define SHADOWSWORD_ASSASSIN_ASSASSINS_MARK			46459
#define SHADOWSWORD_ASSASSIN_AIMED_SHOT				46460
#define SHADOWSWORD_ASSASSIN_SHADOWSTEP				46463
#define SHADOWSWORD_ASSASSIN_GREATER_INVISIBILITY	16380

class ShadowswordAssassinAI : public ArcScriptCreatureAI
{
	ARCSCRIPT_FACTORY_FUNCTION(ShadowswordAssassinAI, ArcScriptCreatureAI);
	ShadowswordAssassinAI(Creature* pCreature) : ArcScriptCreatureAI(pCreature)
	{
		AddSpell(SHADOWSWORD_ASSASSIN_ASSASSINS_MARK, Target_RandomPlayer, 100, 0, 15, 0, 100);
		AddSpell(SHADOWSWORD_ASSASSIN_AIMED_SHOT, Target_Current, 15, 4, 6, 5, 35, true);
		AddSpell(SHADOWSWORD_ASSASSIN_SHADOWSTEP, Target_RandomPlayer, 15, 0, 50, 0, 40);
		AddSpell(SHADOWSWORD_ASSASSIN_GREATER_INVISIBILITY, Target_Self, 5, 0, 180);
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Shadowsword Commander
#define CN_SHADOWSWORD_COMMANDER			25837
#define SHADOWSWORD_COMMANDER_SHIELD_SLAM	46762
#define SHADOWSWORD_COMMANDER_BATTLESHOUT	46763

class ShadowswordCommanderAI : public ArcScriptCreatureAI
{
	ARCSCRIPT_FACTORY_FUNCTION(ShadowswordCommanderAI, ArcScriptCreatureAI);
	ShadowswordCommanderAI(Creature* pCreature) : ArcScriptCreatureAI(pCreature)
	{
		AddSpell(SHADOWSWORD_COMMANDER_SHIELD_SLAM, Target_Current, 10, 0, 10);
		AddSpell(SHADOWSWORD_COMMANDER_BATTLESHOUT, Target_Self, 20, 0, 25);
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Kalecgos
#define CN_KALECGOS					24850
#define KALECGOS_FROST_BREATH		44799
#define KALECGOS_SPECTRAL_BLAST		44866
#define KALECGOS_ARCANE_BUFFET		45018

void SpellFunc_Kalecgos_WildMagic(SpellDesc* pThis, ArcScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);

class KalecgosAI : public ArcScriptBossAI
{
    ARCSCRIPT_FACTORY_FUNCTION(KalecgosAI, ArcScriptBossAI);
	KalecgosAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{
		AddSpell(KALECGOS_FROST_BREATH, Target_Current, 10, 1, 12, 0, 30);
		AddSpellFunc(SpellFunc_Kalecgos_WildMagic, Target_RandomPlayer, 15, 0, 10, 0, 100);
		AddSpell(KALECGOS_SPECTRAL_BLAST, Target_Self, 25, 0, 25, 0, 50);
		AddSpell(KALECGOS_ARCANE_BUFFET, Target_Self, 100, 0, 8);

		//Emotes
		AddEmote(Event_OnCombatStart, "I need... your help... Cannot... resist him... much longer...", Text_Yell);
		AddEmote(Event_OnTargetDied, "In the name of Kil'jaeden!", Text_Yell);
		AddEmote(Event_OnTargetDied, "You were warned! ", Text_Yell);
		AddEmote(Event_OnDied, "I am forever in your debt. Once we have triumphed over Kil'jaeden, this entire world will be in your debt as well.", Text_Yell, 11135);
	}
};

void SpellFunc_Kalecgos_WildMagic(SpellDesc* pThis, ArcScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType)
{
	KalecgosAI* Kalecgos = ( pCreatureAI ) ? (KalecgosAI*)pCreatureAI : NULL;
	if( Kalecgos )
	{
		//TODO
/*
		#define SP_WILD_MAGIC_1			44978
		#define SP_WILD_MAGIC_2			45001
		#define SP_WILD_MAGIC_3			45002
		#define SP_WILD_MAGIC_4			45004
		#define SP_WILD_MAGIC_5			45006
		#define SP_WILD_MAGIC_6			45010*/
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Sathrovarr the Corruptor
#define CN_SATHROVARR_THE_CORRUPTOR							24892
#define SATHROVARR_THE_CORRUPTOR_CURSE_OF_BOUNDLESS_AGONY	45034
#define SATHROVARR_THE_CORRUPTOR_SHADOW_BOLT_VOLLEY			38840
#define SATHROVARR_THE_CORRUPTOR_CORRUPTING_STRIKE			45029

class SathrovarrTheCorruptorAI : public ArcScriptBossAI
{
    ARCSCRIPT_FACTORY_FUNCTION(SathrovarrTheCorruptorAI, ArcScriptBossAI);
	SathrovarrTheCorruptorAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{
		AddSpell(SATHROVARR_THE_CORRUPTOR_CURSE_OF_BOUNDLESS_AGONY, Target_RandomPlayer, 20, 0, 12, 0, 40); 
		AddSpell(SATHROVARR_THE_CORRUPTOR_SHADOW_BOLT_VOLLEY, Target_RandomPlayerApplyAura, 20, 1, 25, 0, 40);
		AddSpell(SATHROVARR_THE_CORRUPTOR_CORRUPTING_STRIKE, Target_Current, 30, 0, 5, 0, 10);

		//Emotes
		AddEmote(Event_OnCombatStart, "Gyahaha... There will be no reprieve. My work here is nearly finished.", Text_Yell);
		AddEmote(Event_OnTargetDied, "Pitious mortal!", Text_Yell);
		AddEmote(Event_OnTargetDied, "Haven't you heard? I always win!", Text_Yell);
		AddEmote(Event_OnDied, "I'm... never on... the losing... side...", Text_Yell);
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Brutallus
#define CN_BRUTALLUS			24882
#define BRUTALLUS_METEOR_SLASH	45150
#define BRUTALLUS_BURN			45141
#define BRUTALLUS_STOMP			45185
#define BRUTALLUS_BERSERK		26662
#define FIRE_WALL				43113
#define CN_FELMYST				25038

class BrutallusAI : public ArcScriptBossAI
{
    ARCSCRIPT_FACTORY_FUNCTION(BrutallusAI, ArcScriptBossAI);
	BrutallusAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{
		AddSpell(BRUTALLUS_METEOR_SLASH, Target_Self, 100, 1, 12);
		AddSpell(BRUTALLUS_BURN, Target_RandomPlayer, 50, 0, 20);
		AddSpell(BRUTALLUS_STOMP, Target_Current, 25, 0, 30);

		//6min Enrage
		SetEnrageInfo(AddSpell(BRUTALLUS_BERSERK, Target_Self, 0, 0, 0, 0, 0, false, "So much for a real challenge... Die!"), 360000);

		//Emotes
		AddEmote(Event_OnCombatStart, "Ah, more lambs to the slaughter!", Text_Yell);
		AddEmote(Event_OnTargetDied, "Perish, insect!", Text_Yell);
		AddEmote(Event_OnTargetDied, "You are meat!", Text_Yell);
		AddEmote(Event_OnTargetDied, "Too easy!", Text_Yell);
		AddEmote(Event_OnDied, "Gah! Well done... Now... this gets... interesting...", Text_Yell);
		AddEmote(Event_OnTaunt, "Bring the fight to me!", Text_Yell);
		AddEmote(Event_OnTaunt, "Another day, another glorious battle!", Text_Yell);
		AddEmote(Event_OnTaunt, "I live for this!", Text_Yell);
	}

	void OnLoad()
	{
		//_unit->CastSpellAoF(x, y, z, dbcSpell.LookupEntry(FIRE_WALL), true);
		ParentClass::OnLoad();
	}

	void OnCombatStart(Unit* pTarget)
	{
		
		ParentClass::OnCombatStart(pTarget);
	}

	void OnDied(Unit* pKiller)
	{
		//SpawnCreature(CN_FELMYST, 1871, 650, 71, 0, false); //spawns
		ParentClass::OnDied(pKiller);
	}

	void AIUpdate()
	{
		
		ParentClass::AIUpdate();
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Felmyst
#define FELMYST_CLEAVE				19983
#define FELMYST_CORROSION			45866
#define FELMYST_DEMONIC_VAPOR		45402
#define FELMYST_GAS_NOVA			45855
#define FELMYST_NOXIOUS_FUME		47002
#define FELMYST_ENCAPSULATE			45662
#define FELMYST_FOG_OF_CORRUPTION	45717
#define FELMYST_ENRAGE				26662	//Using same as Brutallus for now, need to find actual spell id

class FelmystAI : public ArcScriptBossAI
{
    ARCSCRIPT_FACTORY_FUNCTION(FelmystAI, ArcScriptBossAI);
	FelmystAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{
		//Phase 1 spells
		AddPhaseSpell(1, AddSpell(FELMYST_CLEAVE, Target_Current, 6, 0, 10, 0, 5));
		AddPhaseSpell(1, AddSpell(FELMYST_GAS_NOVA, Target_Self, 25, 1, 18));
		AddPhaseSpell(1, AddSpell(FELMYST_ENCAPSULATE, Target_RandomPlayer, 25, 7, 30, 0, 30));
		AddPhaseSpell(1, AddSpell(FELMYST_CORROSION, Target_Current, 20, 0.75f, 35, 0, 30, false, "Choke on your final breath!"));

		//Phase 2 spells
		AddPhaseSpell(2, AddSpell(FELMYST_DEMONIC_VAPOR, Target_RandomPlayer, 10, 0, 20));

		//Phase 3 spells
		//Fog of corruption is the actual breath Felmyst does during his second phase, probably we'll have to spawn it like a creature.
		//AddSpell(FELMYST_FOG_OF_CORRUPTION, Target_RandomPlayerApplyAura, 15, 0, 20, 0, 10); Does not support by the core.

		//10min Enrage
		SetEnrageInfo(AddSpell(FELMYST_ENRAGE, Target_Self, 0, 0, 0, 0, 0, false, "No more hesitation! Your fates are written!"), 600000);

		//Emotes
		AddEmote(Event_OnCombatStart, "Glory to Kil'jaeden! Death to all who oppose!", Text_Yell);
		AddEmote(Event_OnTargetDied, "I kill for the master! ", Text_Yell);
		AddEmote(Event_OnTargetDied, "The end has come!", Text_Yell);
		AddEmote(Event_OnDied, "Kil'jaeden... will... prevail...", Text_Yell);
		AddEmote(Event_OnTaunt, "I am stronger than ever before!", Text_Yell);
	}

	void OnLoad()
	{
		//Create waypoints and move
		ParentClass::OnLoad();
	}

	void OnCombatStart(Unit* pTarget)
	{
		ApplyAura(FELMYST_NOXIOUS_FUME);
		ParentClass::OnCombatStart(pTarget);
	}

	void OnDied(Unit* pKiller)
	{
		//remove firewall
		ParentClass::OnDied(pKiller);
	}

	void AIUpdate()
	{
		
		ParentClass::AIUpdate();
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Lady Sacrolash
#define CN_LADY_SACROLASH				25165
#define CN_GRAND_WARLOCK_ALYTHESS		25166
#define LADY_SACROLASH_DARK_TOUCHED		45347
#define LADY_SACROLASH_SHADOW_BLADES	45248
#define LADY_SACROLASH_SHADOW_NOVA		45329
#define LADY_SACROLASH_CONFOUNDING_BLOW	45256
#define LADY_SACROLASH_ENRAGE			26662	//Using same as Brutallus for now, need to find actual spell id

class LadySacrolashAI : public ArcScriptBossAI
{
    ARCSCRIPT_FACTORY_FUNCTION(LadySacrolashAI, ArcScriptBossAI);
	LadySacrolashAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{
		AddSpell(LADY_SACROLASH_DARK_TOUCHED, Target_RandomPlayerApplyAura, 50, 0, 10, 0, 50);
		AddSpell(LADY_SACROLASH_SHADOW_BLADES, Target_Current, 25, 1.5, 5, 0, 50);
		AddSpell(LADY_SACROLASH_SHADOW_NOVA, Target_RandomPlayer, 15, 3.5, 20, 0, 50, false, "Shadow to the aid of fire!");
		AddSpell(LADY_SACROLASH_CONFOUNDING_BLOW, Target_RandomPlayer, 10, 0, 15, 0, 50);
		SetEnrageInfo(AddSpell(LADY_SACROLASH_ENRAGE, Target_Self, 0, 0, 0, 0, 0, 0, "Time is a luxury you no longer possess!"), 360000);

		//Emotes
		AddEmote(Event_OnTargetDied, "Shadows, engulf!", Text_Yell);
		AddEmote(Event_OnDied, "I... fade.", Text_Yell);
	}

	void OnCombatStart(Unit* pTarget)
	{
		mGrandWarlockAlythess = GetNearestCreature(CN_GRAND_WARLOCK_ALYTHESS);
		ParentClass::OnCombatStart(pTarget);
	}

	void OnDied(Unit* pKiller)
	{
		if( mGrandWarlockAlythess && mGrandWarlockAlythess->IsAlive() )
		{
			mGrandWarlockAlythess->Emote("Sacrolash!", Text_Yell);
		}
		ParentClass::OnDied(pKiller);
	}

	ArcScriptCreatureAI* mGrandWarlockAlythess;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Grand Warlock Alythess
#define GRAND_WARLOCK_ALYTHESS_PYROGENICS		45230
#define GRAND_WARLOCK_ALYTHESS_FLAME_TOUCHED	45348
#define GRAND_WARLOCK_ALYTHESS_CONFLAGRATION	45342
#define GRAND_WARLOCK_ALYTHESS_BLAZE			45235
#define GRAND_WARLOCK_ALYTHESS_FLAME_SEAR		46771
#define GRAND_WARLOCK_ALYTHESS_ENRAGE			26662	//Using same as Brutallus for now, need to find actual spell id

class GrandWarlockAlythessAI : public ArcScriptBossAI
{
    ARCSCRIPT_FACTORY_FUNCTION(GrandWarlockAlythessAI, ArcScriptBossAI);
	GrandWarlockAlythessAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{
		AddSpell(GRAND_WARLOCK_ALYTHESS_PYROGENICS, Target_Self, 100, 0, 10, 0, 50);
		AddSpell(GRAND_WARLOCK_ALYTHESS_FLAME_TOUCHED, Target_RandomPlayerApplyAura, 10, 0, 30, 0, 50);
		AddSpell(GRAND_WARLOCK_ALYTHESS_CONFLAGRATION, Target_RandomPlayer, 15, 3.5, 25, 0, 50, false, "Fire to the aid of shadow!");
		AddSpell(GRAND_WARLOCK_ALYTHESS_BLAZE, Target_RandomPlayer, 30, 2.5, 0, 0, 50);
		AddSpell(GRAND_WARLOCK_ALYTHESS_FLAME_SEAR, Target_RandomPlayer, 20, 0, 0, 0, 50);
		SetEnrageInfo(AddSpell(GRAND_WARLOCK_ALYTHESS_ENRAGE, Target_Self, 0, 0, 0, 0, 0, false, "Your luck has run its course!"), 360000);

		//Emotes
		AddEmote(Event_OnTargetDied, "Fire, consume!", Text_Yell);
		AddEmote(Event_OnDied, "I... fade.", Text_Yell);
	}

	void OnCombatStart(Unit* pTarget)
	{
		mLadySacrolash = GetNearestCreature(CN_LADY_SACROLASH);
		ParentClass::OnCombatStart(pTarget);
	}

	void OnDied(Unit* pKiller)
	{
		if( mLadySacrolash && mLadySacrolash->IsAlive() )
		{
			mLadySacrolash->Emote("Alythess! Your fire burns within me!", Text_Yell);
		}
		ParentClass::OnDied(pKiller);
	}

	ArcScriptCreatureAI* mLadySacrolash;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//M'uru
#define CN_MURU						25741
#define CN_SHADOWSWORD_BERSERKER	25798
#define CN_SHADOWSWORD_FURY_MAGE	25799
#define CN_VOID_SENTINEL			25772
#define CN_ENTROPIUS				25840
#define MURU_NEGATIVE_ENERGY		46285
#define MURU_DARKNESS				45996
#define MURU_SUMMON_BERSERKER		46037
#define MURU_SUMMON_FURY_MAGE		46038
#define MURU_SUMMON_VOID_SENTINEL	45988

class MuruAI : public ArcScriptBossAI
{
	ARCSCRIPT_FACTORY_FUNCTION(MuruAI, ArcScriptBossAI);
	MuruAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{
		AddSpell(MURU_NEGATIVE_ENERGY, Target_SecondMostHated, 25, 0, 1, 0, 40);
		AddSpell(MURU_DARKNESS, Target_Current, 25, 0, 10, 0, 10);

		/*AddEmote(Event_OnTargetDied, "Fire, consume!", Text_Yell);
		AddEmote(Event_OnDied, "I... fade.", Text_Yell);*/
	}

	void OnCombatStart(Unit* mTarget)
	{
		SetAllowMelee(false);
		SetCanMove(false);
		//doors
		ParentClass::OnCombatStart(mTarget);
	}

	void OnLoad()
	{
		spawnplace = 1;
		summoningst = false;
		timmer = 0;
		SetAllowMelee(false);
		SetCanMove(false);
		ParentClass::OnLoad();
	}

	void AIUpdate()
	{
		SetAllowMelee(false);
		SetCanMove(false);
		timmer++;
		if (summoningst == true && (timmer == 20 || timmer == 60 || timmer == 100))
		{
			SpawnCreature(CN_SHADOWSWORD_BERSERKER, 1871, 650, 71, 0, false);
			SpawnCreature(CN_SHADOWSWORD_FURY_MAGE, 1871, 650, 71, 0, false);
			SpawnCreature(CN_SHADOWSWORD_FURY_MAGE, 1748, 700, 71, 0, false);
			SpawnCreature(CN_SHADOWSWORD_BERSERKER, 1748, 700, 71, 0, false);
		}

		if (timmer == 120)
		{
			timmer = 0;
			summoningst = true;
			switch (spawnplace)
			{
			case 1:
				{
				SpawnCreature(CN_VOID_SENTINEL, 1800, 652, 71, 0, false);
				++spawnplace;
				}break;
			case 2:
				{
				SpawnCreature(CN_VOID_SENTINEL, 1798, 605, 71, 0, false);
				++spawnplace;
				}break;
			case 3:
				{
				SpawnCreature(CN_VOID_SENTINEL, 1826, 650, 71, 0, false);
				++spawnplace;
				}break;
			case 4:
				{
				SpawnCreature(CN_VOID_SENTINEL, 1783, 625, 71, 0, false);
				++spawnplace;
				}break;
			case 5:
				{
				SpawnCreature(CN_VOID_SENTINEL, 1816, 595, 71, 0, false);
				++spawnplace;
				}break;
			case 6:
				{
				SpawnCreature(CN_VOID_SENTINEL, 1844, 641, 71, 0, false);
				spawnplace = 1;
				}break;
			}
		}

		if (GetHealthPercent() <= 35)
		{
			SpawnCreature(CN_ENTROPIUS, true);
			Despawn(100,0);
		}

		ParentClass::AIUpdate();
	}

	void OnDied(Unit * mKiller)
	{
		ParentClass::OnDied(mKiller);
	}
protected:
	int timmer, spawnplace;
	bool summoningst;
};

#define BERSERK_FLURRY 46160

class ShadowswordBerserkerAI : public ArcScriptCreatureAI
{
	ARCSCRIPT_FACTORY_FUNCTION(ShadowswordBerserkerAI, ArcScriptCreatureAI);
	ShadowswordBerserkerAI(Creature* pCreature) : ArcScriptCreatureAI(pCreature)
	{
		AddSpell(BERSERK_FLURRY, Target_Current, 30, 0, 30);
	}

	void OnCombatStart(Unit* mTarget)
	{
		ParentClass::OnCombatStart(mTarget);
	}

	void OnLoad()
	{
		_unit->GetAIInterface()->m_moveRun = true;
		Creature *Muruu = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1815, 625, 69, CN_MURU);
		if (Muruu)
		{
			MoveTo(Muruu);
		}
		ParentClass::OnLoad();
	}

	void AIUpdate()
	{
		ParentClass::AIUpdate();
	}

	void OnDied(Unit * mKiller)
	{
		Despawn(100,0);
		ParentClass::OnDied(mKiller);
	}

protected:
	Creature *Muruu;

};

#define FEL_FIREBALL 46101
#define SPELL_FURY 46102

class ShadowswordFuryMageAI : public ArcScriptCreatureAI
{
	ARCSCRIPT_FACTORY_FUNCTION(ShadowswordFuryMageAI, ArcScriptCreatureAI);
	ShadowswordFuryMageAI(Creature* pCreature) : ArcScriptCreatureAI(pCreature)
	{
		AddSpell(FEL_FIREBALL, Target_RandomPlayer, 100, 2, 2, 4, 40); 
		AddSpell(SPELL_FURY, Target_Self, 30, 0, 50); 
	}

	void OnCombatStart(Unit* mTarget)
	{
		ParentClass::OnCombatStart(mTarget);
	}

	void OnLoad()
	{
		_unit->GetAIInterface()->m_moveRun = true;
		Creature *Muruu = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1815, 625, 69, CN_MURU);
		if (Muruu)
		{
			MoveTo(Muruu);
		}
		ParentClass::OnLoad();
	}

	void AIUpdate()
	{
		ParentClass::AIUpdate();
	}

	void OnDied(Unit * mKiller)
	{
		Despawn(100,0);
		ParentClass::OnDied(mKiller);
	}

protected:
	Creature *Muruu;
};

#define SHADOW_PULSE 46087
#define WOID_BLAST 46161

class VoidSentinelAI : public ArcScriptCreatureAI
{
	ARCSCRIPT_FACTORY_FUNCTION(VoidSentinelAI, ArcScriptCreatureAI);
	VoidSentinelAI(Creature* pCreature) : ArcScriptCreatureAI(pCreature)
	{
		AddSpell(SHADOW_PULSE, Target_Current, 50, 0, 3);
		AddSpell(WOID_BLAST, Target_Current, 20, 1.50f, 15);
	}

	void OnCombatStart(Unit* mTarget)
	{
		ParentClass::OnCombatStart(mTarget);
	}

	void OnLoad()
	{
		_unit->GetAIInterface()->m_moveRun = true;
		Creature *Muruu = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1815, 625, 69, CN_MURU);
		if (Muruu)
		{
			MoveTo(Muruu);
		}
		ParentClass::OnLoad();
	}

	void AIUpdate()
	{
		ParentClass::AIUpdate();
	}

	void OnDied(Unit * mKiller)
	{
		Despawn(100,0);
		ParentClass::OnDied(mKiller);
	}

protected:
	Creature *Muruu;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Entropius
#define ENTROPIUS_NEGATIVE_ENERGY	46289 //SpellID taken from Wowhead
#define ENTROPIUS_NEGATIVE_ENERGY1	46008
#define ENTROPIUS_NEGATIVE_ENERGY2	46285
#define ENTROPIUS_BLACK_HOLE		46282 //SpellID taken from Wowhead

class EntropiusAI : public ArcScriptBossAI
{
	ARCSCRIPT_FACTORY_FUNCTION(EntropiusAI, ArcScriptBossAI);
	EntropiusAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{
		AddSpell(ENTROPIUS_NEGATIVE_ENERGY1, Target_RandomDestination, 15, 0, 1, 0, 40);
		AddSpell(ENTROPIUS_BLACK_HOLE, Target_RandomDestination, 10, 0, 10, 0, 40);

		/*AddEmote(Event_OnTargetDied, "Fire, consume!", Text_Yell);
		AddEmote(Event_OnDied, "I... fade.", Text_Yell);*/
	}

	void OnCombatStart(Unit* mTarget)
	{
		ParentClass::OnCombatStart(mTarget);
	}

	void OnLoad()
	{
		ParentClass::OnLoad();
	}

	void AIUpdate()
	{
		ParentClass::AIUpdate();
	}

	void OnDied(Unit * mKiller)
	{
		//doors
		ParentClass::OnDied(mKiller);
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CN_HAND_OF_THE_DECEIVER 25588
#define ANVEENA_PRISON 46367

#define CN_VOLATILE_FELFIRE_FIEND 25603
/* Hand of the Deceiver's spells and cosmetics */ 
#define SPELL_SHADOW_BOLT_VOLLEY 45770 // ~30 yard range Shadow Bolt Volley for ~2k(?) damage
#define SPELL_SHADOW_INFUSION 45772 // They gain this at 20% - Immunity to Stun/Silence and makes them look angry!
#define SPELL_FELFIRE_PORTAL 46875 // Creates a portal that spawns Felfire Fiends (LIVE FOR THE SWARM!1 FOR THE OVERMIND!)
#define SPELL_SHADOW_CHANNELING 46757 // Channeling animation out of combat
#define SPELL_SACRIFICE_OF_ANVEENA 46474 // This is cast on Kil'Jaeden when Anveena sacrifices herself into the Sunwell

class HandOfTheDeceiverAI : public ArcScriptBossAI
{
	ARCSCRIPT_FACTORY_FUNCTION(HandOfTheDeceiverAI, ArcScriptBossAI);
	HandOfTheDeceiverAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{
		AddSpell(SPELL_FELFIRE_PORTAL, Target_RandomPlayer, 40, 0, 10, 0, 30);
		AddSpell(SPELL_SHADOW_BOLT_VOLLEY, Target_Current, 60, 0, 3, 0, 30);
	}

	void OnLoad()
	{
		infusion = false;
		ParentClass::OnLoad();
	}

	void AIUpdate()
	{
		if (!IsInCombat())
			CastSpellOnTarget(_unit, Target_Self, dbcSpell.LookupEntry(SPELL_SHADOW_CHANNELING), false);
		
		if (timmer == 6 && IsInCombat())
		{
			SpawnCreature(CN_VOLATILE_FELFIRE_FIEND, _unit->GetPositionX()+1, _unit->GetPositionY()+1, _unit->GetPositionZ(), 0, true);
			timmer = 0;
		}

		if(GetHealthPercent()<=25 && infusion == false)
		{
			ApplyAura(SPELL_SHADOW_INFUSION);
			infusion = true;
		}
		timmer++;
		ParentClass::AIUpdate();
	}

	void OnDied(Unit * mKiller)
	{
		Despawn(100,0);
		ParentClass::OnDied(mKiller);
	}

protected:
	bool infusion;
	int timmer;
};

/* Volatile Felfire Fiend's spells */
#define SPELL_FELFIRE_FISSION 45779 // Felfire Fiends explode when they die or get close to target.

class VolatileFelfireFiendAI : public ArcScriptBossAI
{
	ARCSCRIPT_FACTORY_FUNCTION(VolatileFelfireFiendAI, ArcScriptBossAI);
	VolatileFelfireFiendAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{
	}

	void OnLoad()
	{
		BoomTarget = GetBestPlayerTarget(TargetFilter_NotCurrent);
		MoveTo(BoomTarget);
	}

	void AIUpdate()
	{
		if ( _unit->CalcDistance(BoomTarget) < 5 && IsInCombat())
			_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELL_FELFIRE_FISSION), true);
		ParentClass::AIUpdate();
	}

	void OnDied(Unit * mKiller)
	{
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELL_FELFIRE_FISSION), true);
		Despawn(100,0);
		ParentClass::OnDied(mKiller);
	}

protected:
	Unit *BoomTarget;
};

#define CREATURE_KALECGOS 25319 // Helps the raid throughout the fight

class BlueDragonAI : public ArcScriptBossAI
{
	ARCSCRIPT_FACTORY_FUNCTION(BlueDragonAI, ArcScriptBossAI);
	BlueDragonAI(Creature* pCreature) : ArcScriptBossAI(pCreature){}

	void OnLoad()
	{
		_unit->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.20f);
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		ParentClass::OnLoad();
	}

	void AIUpdate()
	{
		ParentClass::AIUpdate();
	}

	void OnDied(Unit * mKiller)
	{
		Despawn(100,0);
		ParentClass::OnDied(mKiller);
	}
protected:
	int timmer;
};

#define CREATURE_SINISTER_REFLECTION 25708
#define SR_CURSE 46190
#define SR_SHOCK 47071
#define SR_FIREBALL 47074
#define SR_HAMMER 37369
#define SR_HEMORRAGE 45897
#define SR_HOLY_SHOCK 38921
#define SR_SMITE 47077
#define SR_CLIP 47168
#define SR_MOONFIRE 47072
#define SR_MULTI_SHOT 48098
#define SR_SHADOWBOLT 47076
#define SR_SHOT 16496
#define SR_WHIRLWIND 17207
#define SR_RENEW 47079

class SinisterReflectionAI : public ArcScriptBossAI
{
	ARCSCRIPT_FACTORY_FUNCTION(SinisterReflectionAI, ArcScriptBossAI);
	SinisterReflectionAI(Creature* pCreature) : ArcScriptBossAI(pCreature){}

	void OnLoad()
	{
		_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 1771);
		_unit->SetFloatValue(OBJECT_FIELD_SCALE_X, 2.0f);
		switch (RandomUInt(9))
		{
			case 0:
			case WARRIOR:
				{
					AddSpell(SR_SHOT, Target_RandomPlayer, 60, 1, 4, 0, 30);
					AddSpell(SR_WHIRLWIND, Target_Current, 60, 1, 4, 0, 5);
				}break;
			case PALADIN:
				{
					AddSpell(SR_HAMMER, Target_Current, 60, 1, 10, 0, 5);
					AddSpell(SR_HOLY_SHOCK, Target_RandomPlayer, 60, 1, 4, 0, 20);
				}break;
			case HUNTER:
				{
					AddSpell(SR_CLIP, Target_Current, 60, 1, 10, 0, 5);
					AddSpell(SR_SHOT, Target_RandomPlayer, 60, 1, 4, 0, 35);
				}break;
			case ROGUE:
				{
					AddSpell(SR_HEMORRAGE, Target_Current, 60, 1, 4, 0, 5);
					AddSpell(SR_SHOT, Target_RandomPlayer, 60, 1, 4, 0, 30);
				}break;
			case PRIEST:
				{
					AddSpell(SR_SMITE, Target_RandomPlayer, 60, 1, 4, 0, 30);
					AddSpell(SR_RENEW, Target_ClosestFriendly, 60, 1, 8, 0, 30);
				}break;
			case SHAMAN:
				{
					AddSpell(SR_SHOCK, Target_RandomPlayer, 60, 1, 4, 0, 30);
				}break;
			case MAGE:
				{
					AddSpell(SR_FIREBALL, Target_RandomPlayer, 60, 2, 2, 0, 30);
				}break;
			case WARLOCK:
				{
					AddSpell(SR_SHADOWBOLT, Target_RandomPlayer, 60, 2, 3, 0, 30);
					AddSpell(SR_CURSE, Target_RandomPlayer, 60, 1, 8, 0, 30);
				}break;
			case 6:
				{
					AddSpell(SR_MOONFIRE, Target_RandomPlayer, 60, 1, 3, 0, 30);
				}break;
		}
		ParentClass::OnLoad();
	}

	void OnCombatStart(Unit* mTarget)
	{
		ParentClass::OnCombatStart(mTarget);
	}

	void AIUpdate()
	{
		ParentClass::AIUpdate();
	}

	void OnDied(Unit * mKiller)
	{
		Despawn(100,0);
		ParentClass::OnDied(mKiller);
	}

protected:
	Unit *Jatekos;
};

#define CN_SHIELD_ORB 25502
#define SHIELD_ORB_SHADOWBOLT 45680

class ShieldOrbAI : public ArcScriptBossAI
{
	ARCSCRIPT_FACTORY_FUNCTION(ShieldOrbAI, ArcScriptBossAI);
	ShieldOrbAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{}

	void OnCombatStart(Unit* mTarget)
	{
		SetCanMove(false);
		ParentClass::OnCombatStart(mTarget);
	}

	void OnLoad()
	{
		_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 1771);
		SetFlyMode(true);
		SetCanMove(false);
		ParentClass::OnLoad();
	}

	void OnDied(Unit * mKiller)
	{
		Despawn(100,0);
		ParentClass::OnDied(mKiller);
	}

	void AIUpdate()
	{
		_unit->Root();
		_unit->CastSpell(Target_RandomPlayer, dbcSpell.LookupEntry(SHIELD_ORB_SHADOWBOLT), true);
		ParentClass::AIUpdate();
	}
	
};

#define CREATURE_KILJAEDEN 25315 // Give it to 'em KJ!
#define CREATURE_ANVEENA 26046 // Embodiment of the Sunwell
#define SPELL_ANVEENA_PRISON 46367 // She hovers locked within a bubble

class AnvennaAI : public ArcScriptBossAI
{
	ARCSCRIPT_FACTORY_FUNCTION(AnvennaAI, ArcScriptBossAI);
	AnvennaAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{}

	void OnLoad()
	{
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		SetFlyMode(true);
		_unit->CastSpell(_unit, SPELL_ANVEENA_PRISON, true);
		KilJaedan = GetNearestCreature(CREATURE_KILJAEDEN);
		ParentClass::OnLoad();
	}

	void AIUpdate()
	{
		if (KilJaedan->GetHealthPercent() <= 25)
		{
			//die
			Despawn(100,0);
		}
		ParentClass::AIUpdate();
	}
	void OnDied(Unit * mKiller)
	{
		Despawn(100,0);
		ParentClass::OnDied(mKiller);
	}
	ArcScriptCreatureAI* KilJaedan;
};

#define CREATURE_ARMAGEDDON_TARGET 25735 // This mob casts meteor on itself.. I think
#define SPELL_ARMAGEDDON 45909 // Meteor spell
#define SPELL_ARMAGEDDON_VISUAL 45911 // Does the hellfire visual to indicate where the meteor missle lands

class ArmageddonAI : public ArcScriptBossAI
{
	ARCSCRIPT_FACTORY_FUNCTION(ArmageddonAI, ArcScriptBossAI);
	ArmageddonAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{}

	void OnLoad()
	{
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->CastSpell(_unit, SPELL_ARMAGEDDON, false);
		_unit->CastSpell(_unit, SPELL_ARMAGEDDON_VISUAL, false);
		ParentClass::OnLoad();
	}

	void AIUpdate()
	{
		ParentClass::AIUpdate();
	}
	void OnDied(Unit * mKiller)
	{
		Despawn(100,0);
		ParentClass::OnDied(mKiller);
	}
	ArcScriptCreatureAI* KilJaedan;
};

 
/* Kil'Jaeden's spells and cosmetics */ 
#define SPELL_TRANS 23188 // Morph. Surprisingly, this seems to be the right spell.. (Where is it used?)
#define SPELL_REBIRTH 44200 // Emerge from the Sunwell
#define SPELL_SOUL_FLAY_DAMAGE 45442 // 9k Shadow damage over 3 seconds. Spammed throughout all the fight.
#define SPELL_SOUL_FLAY_MOVEMENT 47106 // -50% Movement component of the above. Players cast it on selves unless this spell is fixed.
#define SPELL_LEGION_LIGHTNING 45664 // Chain Lightning, 4 targets, ~3k Shadow damage, 1.5k mana burn
#define SPELL_FIRE_BLOOM 45641 // Places a debuff on 5 raid members, which causes them to deal 2k Fire damage to nearby allies and selves. MIGHT NOT WORK
#define SPELL_SINISTER_REFLECTION 45891 // Summon shadow copies of 5 raid members that fight against KJ's enemies
#define SPELL_COPY_WEAPON 41055 // }
#define SPELL_COPY_WEAPON2 41054 // }
#define SPELL_COPY_OFFHAND 45206 // }- Spells used in Sinister Reflection creation
#define SPELL_COPY_OFFHAND_WEAPON 45205 // }
#define SPELL_SHADOW_SPIKE 45885 // Bombard random raid members with Shadow Spikes (Very similar to Void Reaver orbs)
#define SPELL_FLAME_DART 45740 // Bombards the raid with flames every 3(?) seconds
#define SPELL_DARKNESS_OF_A_THOUSAND_SOULS 45657 // Begins a 8-second channeling, after which he will deal 50'000 damage to the raid
 
/* Anveena's spells and cosmetics (Or, generally, everything that has "Anveena" in name) */ 
#define SPELL_ANVEENA_ENERGY_DRAIN 46410 // Sunwell energy glow animation (Control mob uses this)
 
/*** Other Spells (used by players, etc) ***/
#define SPELL_VENGEANCE_OF_THE_BLUE_FLIGHT 45839 // Possess the blue dragon from the orb to help the raid.
#define SPELL_ENTROPIUS_BODY 46819 // Visual for Entropius at the Epilogue
 
/*** Creatures used in the encounter ***/
#define CREATURE_HAND_OF_THE_DECEIVER 25588 // Adds found before KJ emerges
#define CREATURE_FELFIRE_PORTAL 25603 // Portal spawned be Hand of the Deceivers
#define CREATURE_VOLATILE_FELFIRE_FIEND 25598 // Fiends spawned by the above portal
#define CREATURE_SHIELD_ORB 25502 // Shield orbs circle the room raining shadow bolts on raid
#define CREATURE_THE_CORE_OF_ENTROPIUS 26262 // Used in the ending cinematic?
#define CREATURE_POWER_OF_THE_BLUE_DRAGONFLIGHT 25319 // NPC that players possess when using the Orb of the Blue Dragonflight
 
/*** GameObjects ***/
#define GAMEOBJECT_ORB_OF_THE_BLUE_DRAGONFLIGHT 188415
 
/*** Speech and sounds***/
// These are used throughout Sunwell and Magisters(?). Players can hear this while running through the instances.
#define SAY_KJ_OFFCOMBAT1 "All my plans have led to this!"
#define SOUND_KJ_OFFCOMBAT1 12495
#define SAY_KJ_OFFCOMBAT2 "Stay on task! Do not waste time!"
#define SOUND_KJ_OFFCOMBAT2 12496
#define SAY_KJ_OFFCOMBAT3 "I have waited long enough!"
#define SOUND_KJ_OFFCOMBAT3 12497
#define SAY_KJ_OFFCOMBAT4 "Fail me and suffer for eternity!"
#define SOUND_KJ_OFFCOMBAT4 12498
#define SAY_KJ_OFFCOMBAT5 "Drain the girl! Drain her power until there is nothing but a vacant shell!"
#define SOUND_KJ_OFFCOMBAT5 12499
 
// Encounter speech and sounds
#define SAY_KJ_EMERGE "The expendible have perished... So be it! Now I shall succeed where Sargeras could not! I will bleed this wretched world and secure my place as the true master of the Burning Legion. The end has come! Let the unraveling of this world commence!" 
#define SOUND_KJ_EMERGE 12500
#define SAY_KJ_SLAY1 "Another step towards destruction!"
#define SOUND_KJ_SLAY1 12501
#define SAY_KJ_SLAY2 "Anak-ky'ri!" // TODO: WTB Eredun dictionary
#define SOUND_KJ_SLAY2 12502
#define SAY_KJ_REFLECTION1 "Who can you trust?"
#define SOUND_KJ_REFLECTION1 12503
#define SAY_KJ_REFLECTION2 "The enemy is upon you!"
#define SOUND_KJ_REFLECTION2 12504
#define SAY_KJ_DARKNESS1 "Chaos!"
#define SOUND_KJ_DARKNESS1 12505
#define SAY_KJ_DARKNESS2 "Destruction!"
#define SOUND_KJ_DARKNESS2 12506
#define SAY_KJ_DARKNESS3 "Oblivion!"
#define SOUND_KJ_DARKNESS3 12507
#define SAY_KJ_PHASE3 "I will not be denied! This world shall fall!"
#define SOUND_KJ_PHASE3 12508
#define SAY_KJ_PHASE4 "Do not harbor false hope. You cannot win!"
#define SOUND_KJ_PHASE4 12509
#define SAY_KJ_PHASE5 "Aggghh! The powers of the Sunwell... turned... against me! What have you done? WHAT HAVE YOU DONE?"
#define SOUND_KJ_PHASE5 12510
 
/*** Kalecgos - Anveena speech at the beginning of Phase 5; Anveena's sacrifice ***/
#define SAY_KALECGOS_AWAKEN "Anveena, you must awaken, this world needs you!"
#define SOUND_KALECGOS_AWAKEN 12445
#define SAY_ANVEENA_IMPRISONED "I serve only the Master now."
#define SOUND_ANVEENA_IMPRISONED 12511
#define SAY_KALECGOS_LETGO "You must let go! You must become what you were always meant to be! The time is now, Anveena!"
#define SOUND_KALECGOS_LETGO 12446
#define SAY_ANVEENA_LOST "But I'm... lost... I cannot find my way back!"
#define SOUND_ANVEENA_LOST 12512
#define SAY_KALECGOS_FOCUS "Anveena, I love you! Focus on my voice, come back for me now! Only you can cleanse the Sunwell!"
#define SOUND_KALECGOS_FOCUS 12447
#define SAY_ANVEENA_KALEC "Kalec... Kalec?"
#define SOUND_ANVEENA_KALEC 12513
#define SAY_KALECGOS_FATE "Yes, Anveena! Let fate embrace you now!"
#define SOUND_KALECGOS_FATE 12448 
#define SAY_ANVEENA_GOODBYE "The nightmare is over, the spell is broken! Goodbye, Kalec, my love!"
#define SOUND_ANVEENA_GOODBYE 12514
#define SAY_KALECGOS_GOODBYE "Goodbye, Anveena, my love. Few will remember your name, yet this day you change the course of destiny. What was once corrupt is now pure. Heroes, do not let her sacrifice be in vain."
#define SOUND_KALECGOS_GOODBYE 12450
#define SAY_KALECGOS_ENCOURAGE "Strike now, heroes, while he is weakened! Vanquish the Deceiver!"
#define SOUND_KALECGOS_ENCOURAGE 12449


void SinisterReflect();
void ArmageddonSpawn();
void BlueShild();
void phaseTester();

class KilJaedenAI : public ArcScriptBossAI
{
	ARCSCRIPT_FACTORY_FUNCTION(KilJaedenAI, ArcScriptBossAI);
	KilJaedenAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{
		AddPhaseSpell(1, AddSpell(SPELL_LEGION_LIGHTNING, Target_RandomPlayer, 20, 2, 3, 0, 50));
		AddPhaseSpell(1, AddSpell(SPELL_SOUL_FLAY_DAMAGE, Target_RandomPlayer, 20, 2.5, 3, 0, 50));
		AddPhaseSpell(1, AddSpell(SPELL_FIRE_BLOOM, Target_RandomPlayer, 20, 1, 15, 0, 20));
	}

	void OnLoad()
	{
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_KJ_OFFCOMBAT1);
		_unit->PlaySoundToSet(SOUND_KJ_OFFCOMBAT1);
		HandOfTheDeceiver1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_HAND_OF_THE_DECEIVER, 1678.00f, 610.00f, 28.00f, 0.00f, true, false, 0, 0);
		HandOfTheDeceiver2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_HAND_OF_THE_DECEIVER, 1712.00f, 604.00f, 28.00f, 0.00f, true, false, 0, 0);
		HandOfTheDeceiver3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_HAND_OF_THE_DECEIVER, 1684.00f, 651.00f, 28.00f, 0.00f, true, false, 0, 0);
		SpawnCreature(CREATURE_ANVEENA, 1698, 629, 62, 0, true);
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, ( false ) ? 0 : UNIT_FLAG_NOT_ATTACKABLE_9);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->m_invisible = true;
		ApplyAura(42866);
		Darkness = dbcSpell.LookupEntry(SPELL_DARKNESS_OF_A_THOUSAND_SOULS);
		starter = 0;
		ParentClass::OnLoad();
	}

	void OnDied(Unit * mKiller)
	{
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Nooooooooooooo!");
		ParentClass::OnDied(mKiller);
	}

	void AIUpdate()
	{
		_unit->Root();
		GetAllowMelee();
		SetBehavior(Behavior_Spell);

		phaseTester();

		switch (starter)
		{
		case 3:
			{
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(35177), true);
			}break;
		case 16:
			{
				SetPhase(1);
				_unit->SetUInt64Value(UNIT_FIELD_FLAGS, ( true ) ? 0 : UNIT_FLAG_NOT_ATTACKABLE_9);
				_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
				ShieldOrb1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_SHIELD_ORB, 1678.00f, 610.00f, 48.00f, 0.00f, true, true, 0, 0);
				//BlueDragon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CREATURE_POWER_OF_THE_BLUE_DRAGONFLIGHT, 1720.00f, 642.00f, 28.00f, 0.00f, true, true, 0, 0);
				phase = 2;
				starter++;
			}break;
		}

		if (starter > 0 && starter < 16)
			starter++;

		if (phase == 4)
		{
			switch (dtimmer)
			{
				case 2:
					{
						ArmageddonSpawn();
						dtimmer++;
					}break;
				case 10:
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL,"Who can you trust?");
						_unit->PlaySoundToSet(12503);
						SinisterReflect();
						dtimmer++;
					}break;
				case 104:
				{
					SetPhase(2);
					ClearHateList();
					SetAllowMelee(true);
					_unit->Root();
					_unit->SetUInt64Value(UNIT_FIELD_FLAGS, ( false ) ? 0 : UNIT_FLAG_NOT_ATTACKABLE_9);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_KJ_DARKNESS1);
					_unit->PlaySoundToSet(SOUND_KJ_DARKNESS1);
					_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 407);
					dtimmer++;
					}break;
				case 112:
				{
					SetAllowMelee(true);
					BlueShild();
					dtimmer++;
				}break;
				case 120:
				{
					CastSpellOnTarget(_unit, Target_RandomPlayer, Darkness, true);
					_unit->Root();
					SetAllowMelee(true);
					dtimmer++;
					}break;
				case 122:
				{
					SetPhase(1);
					_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
					_unit->SetUInt64Value(UNIT_FIELD_FLAGS, ( true ) ? 0 : UNIT_FLAG_NOT_ATTACKABLE_9);
					ClearHateList();
					dtimmer = 0;
					}break;
				default:
				{
					_unit->Root();
					dtimmer++;
				}break;
			}
		}

		if (phase == 5)
		{
			switch (dtimmer)
			{
				case 10:
				{
					SinisterReflect();
					dtimmer++;
				}break;
				case 54:
				{
					SetPhase(2);
					ClearHateList();
					SetAllowMelee(true);
					_unit->Root();
					_unit->SetUInt64Value(UNIT_FIELD_FLAGS, ( false ) ? 0 : UNIT_FLAG_NOT_ATTACKABLE_9);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_KJ_DARKNESS2);
					_unit->PlaySoundToSet(SOUND_KJ_DARKNESS2);
					_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 407);
					dtimmer++;
					}break;
				case 62:
				{
					BlueShild();
					SetAllowMelee(true);
					dtimmer++;
				}break;
				case 70:
				{
					CastSpellOnTarget(_unit, Target_RandomPlayer, Darkness, true);
					SetAllowMelee(true);
					_unit->Root();
					dtimmer++;
					}break;
				case 72:
				{
					SetPhase(1);
					_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
					_unit->SetUInt64Value(UNIT_FIELD_FLAGS, ( true ) ? 0 : UNIT_FLAG_NOT_ATTACKABLE_9);
					ClearHateList();
					dtimmer = 0;
					}break;
				default:
				{
					SetAllowMelee(true);
					_unit->Root();
					dtimmer++;
				}break;
			}
		}
		ParentClass::AIUpdate();
	}

	void phaseTester()
	{

		_unit->Root();
		GetAllowMelee();
		SetBehavior(Behavior_Spell);

		if ((HandOfTheDeceiver1 && HandOfTheDeceiver2 && HandOfTheDeceiver3) && (HandOfTheDeceiver1->isDead() && HandOfTheDeceiver2->isDead() && HandOfTheDeceiver3->isDead()) && (GetPhase() != 1) && (GetPhase() != 2) && (starter == 0))
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_KJ_EMERGE);
			_unit->PlaySoundToSet(SOUND_KJ_EMERGE);
			RemoveAura(42866);
			_unit->m_invisible = false;
			starter = 1;
		}

		switch(GetPhase())
		{
		case 1:
			{
				if(GetHealthPercent()<=85 && phase == 2)
				{
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_KJ_PHASE3);
					_unit->PlaySoundToSet(SOUND_KJ_PHASE3);
					AddPhaseSpell(1, AddSpell(SPELL_SINISTER_REFLECTION, Target_RandomPlayer, 10, 1.5f, 4, 0, 35));
					AddPhaseSpell(1, AddSpell(SPELL_SHADOW_SPIKE, Target_RandomDestination, 10, 2.0f, 6, 0, 35));
					AddPhaseSpell(1, AddSpell(SPELL_FLAME_DART, Target_RandomPlayer, 10, 1.3f, 4, 0, 35));
					ShieldOrb2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_SHIELD_ORB, 1712.00f, 604.00f, 48.00f, 0.00f, true, true, 0, 0);
					phase++;
				}

				if(GetHealthPercent()<=55 && phase == 3)
				{
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_KJ_PHASE4);
					_unit->PlaySoundToSet(SOUND_KJ_PHASE4);
					ShieldOrb3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_SHIELD_ORB, 1684.00f, 651.00f, 48.00f, 0.00f, true, true, 0, 0);
					dtimmer = 0;
					phase++;
				}

				if(GetHealthPercent()<=25 && phase == 4 && (dtimmer > 54 || dtimmer == 0))
				{
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_KJ_PHASE5);
					_unit->PlaySoundToSet(SOUND_KJ_PHASE5);
					ShieldOrb4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_SHIELD_ORB, 1720.00f, 642.00f, 48.00f, 0.00f, true, true, 0, 0);
					CastSpellOnTarget(_unit, Target_Self, dbcSpell.LookupEntry(SPELL_SACRIFICE_OF_ANVEENA), true);
					dtimmer = 0;
					phase++;
				}
			}break;
		}
	}

	void BlueShild()
	{
		for (set<Player*>::iterator itr = _unit->GetInRangePlayerSetBegin(); itr != _unit->GetInRangePlayerSetEnd(); ++itr) 
			{
				Player *pPlayer = static_cast<Player*>(*itr);
				if (pPlayer->isAlive() && _unit->CalcDistance(pPlayer) <= 16.0f)
					pPlayer->CastSpell(pPlayer, dbcSpell.LookupEntry(38916), true);
			}

		_unit->Root();
		GetAllowMelee();
		SetBehavior(Behavior_Spell);
	}

	void SinisterReflect()
	{
		_unit->Root();
		SetBehavior(Behavior_Spell);

		SpawnCreature(CREATURE_SINISTER_REFLECTION, 1729.00f, 634.00f, 28.00f, 0.00f, true);
		SpawnCreature(CREATURE_SINISTER_REFLECTION, 1701.00f, 658.00f, 28.00f, 0.00f, true);
		SpawnCreature(CREATURE_SINISTER_REFLECTION, 1637.00f, 647.00f, 28.00f, 0.00f, true);
		SpawnCreature(CREATURE_SINISTER_REFLECTION, 1672.00f, 612.00f, 28.00f, 0.00f, true);
		SpawnCreature(CREATURE_SINISTER_REFLECTION, 1702.00f, 597.00f, 28.00f, 0.00f, true);
	}

	void ArmageddonSpawn()
	{
		int val = RandomUInt(4);
		switch (val)
		{
		case 1:
			SpawnCreature(CREATURE_ARMAGEDDON_TARGET, 1678.00f, 610.00f, 28.00f, 0.00f, true);
		case 2:
			SpawnCreature(CREATURE_ARMAGEDDON_TARGET, 1678.00f, 610.00f, 28.00f, 0.00f, true);
		case 3:
			SpawnCreature(CREATURE_ARMAGEDDON_TARGET, 1678.00f, 610.00f, 28.00f, 0.00f, true);
		case 4:
			SpawnCreature(CREATURE_ARMAGEDDON_TARGET, 1678.00f, 610.00f, 28.00f, 0.00f, true);
		}
	}

protected:
	Unit *HandOfTheDeceiver1;
	Unit *HandOfTheDeceiver2;
	Unit *HandOfTheDeceiver3;
	Unit *ShieldOrb1;
	Unit *ShieldOrb2;
	Unit *ShieldOrb3;
	Unit *ShieldOrb4;
	Unit *BlueDragon;
	SpellEntry *Darkness;
	int dtimmer, phase, starter;
};


void SetupSunwellPlateau(ScriptMgr* pScriptMgr)
{
	pScriptMgr->register_creature_script(CN_SUNBLADE_PROTECTOR, &SunbladeProtectorAI::Create);

	pScriptMgr->register_creature_script(CN_KALECGOS, &KalecgosAI::Create);
	pScriptMgr->register_creature_script(CN_SATHROVARR_THE_CORRUPTOR, &SathrovarrTheCorruptorAI::Create);
	pScriptMgr->register_creature_script(CN_BRUTALLUS, &BrutallusAI::Create);
	pScriptMgr->register_creature_script(CN_FELMYST, &FelmystAI::Create);
	pScriptMgr->register_creature_script(CN_LADY_SACROLASH, &LadySacrolashAI::Create);
	pScriptMgr->register_creature_script(CN_GRAND_WARLOCK_ALYTHESS, &GrandWarlockAlythessAI::Create);
	pScriptMgr->register_creature_script(CN_MURU, &MuruAI::Create);
	pScriptMgr->register_creature_script(CN_SHADOWSWORD_BERSERKER, &ShadowswordBerserkerAI::Create);
	pScriptMgr->register_creature_script(CN_SHADOWSWORD_FURY_MAGE, &ShadowswordFuryMageAI::Create);
	pScriptMgr->register_creature_script(CN_VOID_SENTINEL, &VoidSentinelAI::Create);
	pScriptMgr->register_creature_script(CN_ENTROPIUS, &EntropiusAI::Create);
	pScriptMgr->register_creature_script(CN_HAND_OF_THE_DECEIVER, &HandOfTheDeceiverAI::Create);
	pScriptMgr->register_creature_script(CN_VOLATILE_FELFIRE_FIEND, &VolatileFelfireFiendAI::Create);
	pScriptMgr->register_creature_script(CREATURE_KALECGOS, &BlueDragonAI::Create);
	pScriptMgr->register_creature_script(CREATURE_SHIELD_ORB, &ShieldOrbAI::Create);
	pScriptMgr->register_creature_script(CREATURE_SINISTER_REFLECTION, &SinisterReflectionAI::Create);
	pScriptMgr->register_creature_script(CREATURE_ANVEENA, &AnvennaAI::Create);
	pScriptMgr->register_creature_script(CREATURE_ARMAGEDDON_TARGET, &ArmageddonAI::Create);
	pScriptMgr->register_creature_script(CREATURE_KILJAEDEN, &KilJaedenAI::Create);
	
}
