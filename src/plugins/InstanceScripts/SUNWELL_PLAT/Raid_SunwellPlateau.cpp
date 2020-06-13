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

class SunbladeProtectorAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(SunbladeProtectorAI, MoonScriptCreatureAI);
		SunbladeProtectorAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
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

class ShadowswordAssassinAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(ShadowswordAssassinAI, MoonScriptCreatureAI);
		ShadowswordAssassinAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
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

class ShadowswordCommanderAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(ShadowswordCommanderAI, MoonScriptCreatureAI);
		ShadowswordCommanderAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(SHADOWSWORD_COMMANDER_SHIELD_SLAM, Target_Current, 10, 0, 10);
			AddSpell(SHADOWSWORD_COMMANDER_BATTLESHOUT, Target_Self, 20, 0, 25);
		}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Kalecgos
/*#define CN_KALECGOS					24850
#define KALECGOS_FROST_BREATH		44799
#define KALECGOS_SPECTRAL_BLAST		44866
#define KALECGOS_ARCANE_BUFFET		45018

void SpellFunc_Kalecgos_WildMagic(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);

class KalecgosAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(KalecgosAI, MoonScriptBossAI);
	KalecgosAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(KALECGOS_FROST_BREATH, Target_Current, 10, 1, 12, 0, 30);
		AddSpellFunc(SpellFunc_Kalecgos_WildMagic, Target_RandomPlayer, 15, 0, 10, 0, 100);
		AddSpell(KALECGOS_SPECTRAL_BLAST, Target_Self, 25, 0, 25, 0, 50);
		AddSpell(KALECGOS_ARCANE_BUFFET, Target_Self, 100, 0, 8);

		//Emotes
		AddEmote(Event_OnCombatStart, "I need... your help... Cannot... resist him... much longer...", Text_Yell, 12428);
		AddEmote(Event_OnTargetDied, "In the name of Kil'jaeden!", Text_Yell, 12425);
		AddEmote(Event_OnTargetDied, "You were warned! ", Text_Yell, 12426);
		AddEmote(Event_OnDied, "I am forever in your debt. Once we have triumphed over Kil'jaeden, this entire world will be in your debt as well.", Text_Yell, 12431);
	}
};

void SpellFunc_Kalecgos_WildMagic(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType)
{
	KalecgosAI* Kalecgos = ( pCreatureAI ) ? (KalecgosAI*)pCreatureAI : NULL;
	if( Kalecgos )
	{
		//TODO

		#define SP_WILD_MAGIC_1			44978
		#define SP_WILD_MAGIC_2			45001
		#define SP_WILD_MAGIC_3			45002
		#define SP_WILD_MAGIC_4			45004
		#define SP_WILD_MAGIC_5			45006
		#define SP_WILD_MAGIC_6			45010
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Sathrovarr the Corruptor
#define CN_SATHROVARR_THE_CORRUPTOR							24892
#define SATHROVARR_THE_CORRUPTOR_CURSE_OF_BOUNDLESS_AGONY	45034
#define SATHROVARR_THE_CORRUPTOR_SHADOW_BOLT_VOLLEY			38840
#define SATHROVARR_THE_CORRUPTOR_CORRUPTING_STRIKE			45029

class SathrovarrTheCorruptorAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(SathrovarrTheCorruptorAI, MoonScriptBossAI);
	SathrovarrTheCorruptorAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(SATHROVARR_THE_CORRUPTOR_CURSE_OF_BOUNDLESS_AGONY, Target_RandomPlayer, 20, 0, 12, 0, 40);
		AddSpell(SATHROVARR_THE_CORRUPTOR_SHADOW_BOLT_VOLLEY, Target_RandomPlayerApplyAura, 20, 1, 25, 0, 40);
		AddSpell(SATHROVARR_THE_CORRUPTOR_CORRUPTING_STRIKE, Target_Current, 30, 0, 5, 0, 10);

		//Emotes
		AddEmote(Event_OnCombatStart, "Gyahaha... There will be no reprieve. My work here is nearly finished.", Text_Yell, 12451);
		AddEmote(Event_OnTargetDied, "Pitious mortal!", Text_Yell, 12455);
		AddEmote(Event_OnTargetDied, "Haven't you heard? I always win!", Text_Yell, 12456);
		AddEmo(Event_OnDied, "I'm... never on... the losing... side...", Text_Yell, 12452);
	}
};
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Brutallus
#define CN_BRUTALLUS			24882
#define BRUTALLUS_METEOR_SLASH	45150
#define BRUTALLUS_BURN			45141
#define BRUTALLUS_STOMP			45185
#define BRUTALLUS_BERSERK		26662

class BrutallusAI : public MoonScriptBossAI
{
		MOONSCRIPT_FACTORY_FUNCTION(BrutallusAI, MoonScriptBossAI);
		BrutallusAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
		{
			AddSpell(BRUTALLUS_METEOR_SLASH, Target_Self, 100, 1, 12);
			AddSpell(BRUTALLUS_BURN, Target_RandomPlayer, 50, 0, 20);
			AddSpell(BRUTALLUS_STOMP, Target_Current, 25, 0, 30);

			//6min Enrage
			SetEnrageInfo(AddSpell(BRUTALLUS_BERSERK, Target_Self, 0, 0, 0, 0, 0, false, "So much for a real challenge... Die!", Text_Yell, 12470), 360000);

			//Emotes
			AddEmote(Event_OnCombatStart, "Ah, more lambs to the slaughter!", Text_Yell, 12463);
			AddEmote(Event_OnTargetDied, "Perish, insect!", Text_Yell, 12464);
			AddEmote(Event_OnTargetDied, "You are meat!", Text_Yell, 12465);
			AddEmote(Event_OnTargetDied, "Too easy!", Text_Yell, 12466);
			AddEmote(Event_OnDied, "Gah! Well done... Now... this gets... interesting...", Text_Yell, 12471);
			AddEmote(Event_OnTaunt, "Bring the fight to me!", Text_Yell, 12467);
			AddEmote(Event_OnTaunt, "Another day, another glorious battle!", Text_Yell, 12468);
			AddEmote(Event_OnTaunt, "I live for this!", Text_Yell, 12469);
		}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Felmyst
#define CN_FELMYST					25038
#define FELMYST_CLEAVE				19983
#define FELMYST_CORROSION			45866
#define FELMYST_DEMONIC_VAPOR		45402
#define FELMYST_GAS_NOVA			45855
#define FELMYST_NOXIOUS_FUME		47002
#define FELMYST_ENCAPSULATE			45662
#define FELMYST_FOG_OF_CORRUPTION	45717
#define FELMYST_ENRAGE				26662	//Using same as Brutallus for now, need to find actual spell id

class FelmystAI : public MoonScriptBossAI
{
		MOONSCRIPT_FACTORY_FUNCTION(FelmystAI, MoonScriptBossAI);
		FelmystAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
		{
			//Phase 1 spells
			AddPhaseSpell(1, AddSpell(FELMYST_CLEAVE, Target_Current, 6, 0, 10, 0, 5));
			AddPhaseSpell(1, AddSpell(FELMYST_GAS_NOVA, Target_Self, 25, 1, 18));
			AddPhaseSpell(1, AddSpell(FELMYST_ENCAPSULATE, Target_RandomPlayer, 25, 7, 30, 0, 30));
			AddPhaseSpell(1, AddSpell(FELMYST_CORROSION, Target_Current, 20, 0.75f, 35, 0, 30, false, "Choke on your final breath!", Text_Yell, 12478));

			//Phase 2 spells
			AddPhaseSpell(2, AddSpell(FELMYST_DEMONIC_VAPOR, Target_RandomPlayer, 10, 0, 20));

			//Phase 3 spells
			//Fog of corruption is the actual breath Felmyst does during his second phase, probably we'll have to spawn it like a creature.
			//AddSpell(FELMYST_FOG_OF_CORRUPTION, Target_RandomPlayerApplyAura, 15, 0, 20, 0, 10); Does not support by the core.

			//10min Enrage
			SetEnrageInfo(AddSpell(FELMYST_ENRAGE, Target_Self, 0, 0, 0, 0, 0, false, "No more hesitation! Your fates are written!", Text_Yell, 12482), 600000);

			//Emotes
			AddEmote(Event_OnCombatStart, "Glory to Kil'jaeden! Death to all who oppose!", Text_Yell, 12477);
			AddEmote(Event_OnTargetDied, "I kill for the master! ", Text_Yell, 12480);
			AddEmote(Event_OnTargetDied, "The end has come!", Text_Yell, 12481);
			AddEmote(Event_OnDied, "Kil'jaeden... will... prevail...", Text_Yell, 12483);
			AddEmote(Event_OnTaunt, "I am stronger than ever before!", Text_Yell, 12479);
		}

		void OnCombatStart(Unit* pTarget)
		{
			ApplyAura(FELMYST_NOXIOUS_FUME);
			ParentClass::OnCombatStart(pTarget);
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

class LadySacrolashAI : public MoonScriptBossAI
{
		MOONSCRIPT_FACTORY_FUNCTION(LadySacrolashAI, MoonScriptBossAI);
		LadySacrolashAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
		{
			AddSpell(LADY_SACROLASH_DARK_TOUCHED, Target_RandomPlayerApplyAura, 50, 0, 10, 0, 50);
			AddSpell(LADY_SACROLASH_SHADOW_BLADES, Target_Current, 25, 1.5, 5, 0, 50);
			AddSpell(LADY_SACROLASH_SHADOW_NOVA, Target_RandomPlayer, 15, 3.5, 20, 0, 50, false, "Shadow to the aid of fire!", Text_Yell, 12485);
			AddSpell(LADY_SACROLASH_CONFOUNDING_BLOW, Target_RandomPlayer, 10, 0, 15, 0, 50);
			SetEnrageInfo(AddSpell(LADY_SACROLASH_ENRAGE, Target_Self, 0, 0, 0, 0, 0, 0, "Time is a luxury you no longer possess!", Text_Yell, 0), 360000); // Wasn't able to find sound for this text

			//Emotes
			AddEmote(Event_OnTargetDied, "Shadows, engulf!", Text_Yell, 12486);
			AddEmote(Event_OnDied, "I... fade.", Text_Yell, 0); // Wasn't able to find sound for this text
		}

		void OnDied(Unit* pKiller)
		{
			MoonScriptCreatureAI* mGrandWarlockAlythess = GetNearestCreature(CN_GRAND_WARLOCK_ALYTHESS);
			if(mGrandWarlockAlythess != NULL && mGrandWarlockAlythess->IsAlive())
			{
				mGrandWarlockAlythess->Emote("Sacrolash!", Text_Yell, 12492);
			}
			ParentClass::OnDied(pKiller);
		}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Grand Warlock Alythess
#define GRAND_WARLOCK_ALYTHESS_PYROGENICS		45230
#define GRAND_WARLOCK_ALYTHESS_FLAME_TOUCHED	45348
#define GRAND_WARLOCK_ALYTHESS_CONFLAGRATION	45342
#define GRAND_WARLOCK_ALYTHESS_BLAZE			45235
#define GRAND_WARLOCK_ALYTHESS_FLAME_SEAR		46771
#define GRAND_WARLOCK_ALYTHESS_ENRAGE			26662	//Using same as Brutallus for now, need to find actual spell id

class GrandWarlockAlythessAI : public MoonScriptBossAI
{
		MOONSCRIPT_FACTORY_FUNCTION(GrandWarlockAlythessAI, MoonScriptBossAI);
		GrandWarlockAlythessAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
		{
			AddSpell(GRAND_WARLOCK_ALYTHESS_PYROGENICS, Target_Self, 100, 0, 10, 0, 50);
			AddSpell(GRAND_WARLOCK_ALYTHESS_FLAME_TOUCHED, Target_RandomPlayerApplyAura, 10, 0, 30, 0, 50);
			AddSpell(GRAND_WARLOCK_ALYTHESS_CONFLAGRATION, Target_RandomPlayer, 15, 3.5, 25, 0, 50, false, "Fire to the aid of shadow!", Text_Yell, 12489);
			AddSpell(GRAND_WARLOCK_ALYTHESS_BLAZE, Target_RandomPlayer, 30, 2.5, 0, 0, 50);
			AddSpell(GRAND_WARLOCK_ALYTHESS_FLAME_SEAR, Target_RandomPlayer, 20, 0, 0, 0, 50);
			SetEnrageInfo(AddSpell(GRAND_WARLOCK_ALYTHESS_ENRAGE, Target_Self, 0, 0, 0, 0, 0, false, "Your luck has run its course!", Text_Yell, 12493), 360000);

			//Emotes
			AddEmote(Event_OnTargetDied, "Fire, consume!", Text_Yell, 12490);
			AddEmote(Event_OnDied, "I... fade.", Text_Yell, 0); // Wasn't able to find sound for this text
		}

		void OnDied(Unit* pKiller)
		{
			MoonScriptCreatureAI* mLadySacrolash = GetNearestCreature(CN_LADY_SACROLASH);
			if(mLadySacrolash != NULL && mLadySacrolash->IsAlive())
			{
				mLadySacrolash->Emote("Alythess! Your fire burns within me!", Text_Yell, 12488);
			}
			ParentClass::OnDied(pKiller);
		}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//M'uru
#define CN_MURU						25741
#define CN_SHADOWSWORD_BERSERKER	25798
#define CN_SHADOWSWORD_FURY_MAGE	25799
#define CN_VOID_SENTINEL			25772
#define MURU_NEGATIVE_ENERGY		46008	//patch 2.4.2: this spell shouldn't cause casting pushback (to be fixed in core)
#define MURU_DARKNESS				45996
#define MURU_SUMMON_BERSERKER		46037
#define MURU_SUMMON_FURY_MAGE		46038
#define MURU_SUMMON_VOID_SENTINEL	45988

class MuruAI : public MoonScriptBossAI
{
		MOONSCRIPT_FACTORY_FUNCTION(MuruAI, MoonScriptBossAI);
		MuruAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
		{
			AddSpell(MURU_NEGATIVE_ENERGY, Target_Self, 25, 0, 0);
			AddSpell(MURU_DARKNESS, Target_Self, 20, 0, 45);

			//AddSpell(MURU_SUMMON_BERSERKER, Target_, 15, 3.5, 25, 0, 50);  Most of Databases don't the SQL for this yet. Also I am not sure what function are for summoning Spells :).
			//AddSpell(MURU_SUMMON_FURY_MAGE, Target_, 30, 2.5, 0, 0, 50);
			//AddSpell(MURU_SUMMON_VOID_SENTINEL, Target_, 20, 0, 0, 0, 50);
		}
};

class ShadowswordBerserkerAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(ShadowswordBerserkerAI, MoonScriptCreatureAI);
		ShadowswordBerserkerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) {}
};

class ShadowswordFuryMageAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(ShadowswordFuryMageAI, MoonScriptCreatureAI);
		ShadowswordFuryMageAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) {}
};

class VoidSentinelAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(VoidSentinelAI, MoonScriptCreatureAI);
		VoidSentinelAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) {}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Entropius
#define CN_ENTROPIUS 25840

class EntropiusAI : public MoonScriptBossAI
{
		MOONSCRIPT_FACTORY_FUNCTION(EntropiusAI, MoonScriptBossAI);
		EntropiusAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
		{
			//TODO
		}
};

void SetupSunwellPlateau(ScriptMgr* pScriptMgr)
{
	pScriptMgr->register_creature_script(CN_SUNBLADE_PROTECTOR, &SunbladeProtectorAI::Create);

	//pScriptMgr->register_creature_script(CN_KALECGOS, &SUNWELL_KALECGOS::DRAGON_KALECGOS::Create);
	//pScriptMgr->register_creature_script(CN_SATHROVAR, &SUNWELL_KALECGOS::SATHROVAR::Create);
	//pScriptMgr->register_creature_script(CN_DARK_ELF, &SUNWELL_KALECGOS::DARK_ELF::Create);
	//pScriptMgr->register_dummy_spell(KALECGOS_SPECTRAL_TELEPORT, &SUNWELL_KALECGOS::HandleSpectralTeleport);
	pScriptMgr->register_creature_script(CN_BRUTALLUS, &BrutallusAI::Create);
	pScriptMgr->register_creature_script(CN_FELMYST, &FelmystAI::Create);
	pScriptMgr->register_creature_script(CN_LADY_SACROLASH, &LadySacrolashAI::Create);
	pScriptMgr->register_creature_script(CN_GRAND_WARLOCK_ALYTHESS, &GrandWarlockAlythessAI::Create);
	pScriptMgr->register_creature_script(CN_MURU, &MuruAI::Create);
	pScriptMgr->register_creature_script(CN_SHADOWSWORD_BERSERKER, &ShadowswordBerserkerAI::Create);
	pScriptMgr->register_creature_script(CN_SHADOWSWORD_FURY_MAGE, &ShadowswordFuryMageAI::Create);
	pScriptMgr->register_creature_script(CN_VOID_SENTINEL, &VoidSentinelAI::Create);
	pScriptMgr->register_creature_script(CN_ENTROPIUS, &EntropiusAI::Create);
}
