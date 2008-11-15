/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008 Arcemu Team
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

#include "StdAfx.h"
#include "Setup.h"
#include "Base.h"

//MagisterTerrace Script 
/*
For PriestessDelrissa's Friends:) the others were scripted in lua.
*/

//Kagani Nightstrike
//NEEEEDS MORE WORK
#define BOSS_KaganiNightstrike	      24557
#define KAGANI_NIGHTSTRIKE_Eviscerate 46189
#define KAGANI_NIGHTSTRIKE_KidneyShot 27615
//#define KAGANI_NIGHTSTRIKE_Gouge	  12540 
//#define KAGANI_NIGHTSTRIKE_Kick	  27613

class Kagani_NightstrikeAI : public ArcScriptBossAI
{
    ARCSCRIPT_FACTORY_FUNCTION(Kagani_NightstrikeAI, ArcScriptBossAI);
	Kagani_NightstrikeAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{
		//TODO: Add the other spells
		AddSpell(KAGANI_NIGHTSTRIKE_Eviscerate, Target_RandomPlayer, 80, 0, 7, 0, 30);
		AddSpell(KAGANI_NIGHTSTRIKE_KidneyShot, Target_Current, 80, 0, 20, 0, 30);
		//AddSpell(KAGANI_NIGHTSTRIKE_Gouge, Target_Current, 80, 0, 0, 0, 0, 30); //Missing Functions
		//AddSpell(KAGANI_NIGHTSTRIKE_Kick, Target_Current, 80, 0, 0, 0, 0, 30); //Missing Functions

	}
};

//Ellrys Duskhallow
#define BOSS_EllrysDuskhallow			14558
#define EllrysDuskhallow_Immolate		44267
#define EllrysDuskhallow_ShadowBolt		12471
#define EllrysDuskhallow_CurseofAgony	14875
#define EllrysDuskhallow_Fear			38595

class Ellrys_DuskhallowAI : public ArcScriptBossAI
{
	ARCSCRIPT_FACTORY_FUNCTION(Ellrys_DuskhallowAI, ArcScriptBossAI);
	Ellrys_DuskhallowAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{
		AddSpell(EllrysDuskhallow_Immolate,Target_Current, 75, 2, 15, 0, 30);
		AddSpell(EllrysDuskhallow_ShadowBolt,Target_RandomPlayer, 75, 3, 5, 4, 40);
		AddSpell(EllrysDuskhallow_CurseofAgony, Target_RandomPlayer, 75, 0, 4, 0, 30);
		AddSpell(EllrysDuskhallow_Fear, Target_RandomPlayer, 75, 1.5, 9, 0, 20);
	}
};

//Eramas Brightblaze 
#define BOSS_EramasBrightblaze						24554
//#define ERAMAS_BRIGHTBLAZE_DRINK_HEALING_POTION 15503
#define ERAMAS_BRIGHTBLAZE_KNOCKDOWN				11428
#define ERAMAS_BRIGHTBLAZE_SNAP_KICK				46182

class Eramas_BrightblazeAI : public ArcScriptBossAI
{
	ARCSCRIPT_FACTORY_FUNCTION(Eramas_BrightblazeAI, ArcScriptBossAI);
	Eramas_BrightblazeAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{
	AddSpell(ERAMAS_BRIGHTBLAZE_KNOCKDOWN, Target_Current, 25, 0, 5, 0, 5);
	AddSpell(ERAMAS_BRIGHTBLAZE_SNAP_KICK, Target_SecondMostHated, 40, 0, 2, 0, 5);
	}
};

//Priestess Delrissa 

#define BOSS_Priestess_Delrissa				24560
#define PRIESTESS_DELRISSA_DISPEL_MAGIC		27609
#define PRIESTESS_DELRISSA_FLASH_HEAL		17843
#define PRIESTESS_DELRISSA_SHADOWWORD_PAIN  15654
#define PRIESTESS_DELRISSA_POWERWORD_SHIELD 44291
#define PRIESTESS_DELRISSA_RENEW			44174

class Priestess_DelrissaAI : public ArcScriptBossAI
{
	ARCSCRIPT_FACTORY_FUNCTION(Priestess_DelrissaAI, ArcScriptBossAI);
	Priestess_DelrissaAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{
		AddSpell(PRIESTESS_DELRISSA_DISPEL_MAGIC, Target_RandomFriendly, 35, 0, 5, 0, 30);
		AddSpell(PRIESTESS_DELRISSA_FLASH_HEAL, Target_RandomFriendly, 40, 1.5, 7, 0, 40);
		AddSpell(PRIESTESS_DELRISSA_SHADOWWORD_PAIN, Target_RandomPlayer, 45, 0, 18, 0, 30); 
		AddSpell(PRIESTESS_DELRISSA_POWERWORD_SHIELD, Target_RandomFriendly, 32, 0, 15, 0, 40);
		AddSpell(PRIESTESS_DELRISSA_RENEW, Target_RandomFriendly, 30, 0, 18, 0, 40);
	}
	};

#define BOSS_YAZZAI					24561
#define YAZZAI_POLYMORPH			13323
//#define YAZZAI_ICE_BLOCK			27619
#define YAZZAI_BLIZZARD				44178
//#define YAZZAI_ICE_LANCE			46194
#define YAZZAI_CONE_OF_COLD			38384
#define YAZZAI_FROSTBOLT			15530

class YazzaiAI : public ArcScriptBossAI
{
	ARCSCRIPT_FACTORY_FUNCTION(YazzaiAI, ArcScriptBossAI);
	YazzaiAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{
		AddSpell(YAZZAI_POLYMORPH, Target_RandomPlayer, 30, 1.5, 16, 0, 30);
		//AddSpell(YAZZAI_ICE_BLOCK, Target_Self, 20, 0, 300, 0, 1);
		AddSpell(YAZZAI_BLIZZARD, Target_RandomPlayer, 25, 0, 20, 0, 30);
		AddSpell(YAZZAI_CONE_OF_COLD, Target_Self, 10, 0, 19, 0, 1);
		AddSpell(YAZZAI_FROSTBOLT, Target_RandomPlayer, 80, 3, 14, 0, 40);

	
	}
};

#define WARLORD_SALARIS						24559
#define WARLORD_SALARIS_INTERCEPT			27577
#define WARLORD_SALARIS_DISARM				27581
#define WARLORD_SALARIS_PIERCING_HOWL		23600
#define WARLORD_SALARIS_FRIGHTENING_SHOUT	19134
#define WARLORD_SALARIS_HAMSTRING			27584
//#define WARLORD_SALARIS_BATTLE_SHOUT		27578
#define WARLORD_SALARIS_MORTAL_STRIKE		44268

class Warlord_SalarisAI : public ArcScriptBossAI
{
	ARCSCRIPT_FACTORY_FUNCTION(Warlord_SalarisAI, ArcScriptBossAI);
	Warlord_SalarisAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{
		//AddSpell(uint32 pSpellId, TargetType pTargetType, float pChance, float pCastTime, int32 pCooldown, float pMinRange, float pMaxRange
		AddSpell(WARLORD_SALARIS_INTERCEPT, Target_RandomPlayer , 25, 0, 8, 8, 25 );
		AddSpell(WARLORD_SALARIS_DISARM, Target_Current, 100, 0, 60, 0, 5 );
		AddSpell(WARLORD_SALARIS_PIERCING_HOWL, Target_Self, 22, 0, 17, 0, 1);
		AddSpell(WARLORD_SALARIS_FRIGHTENING_SHOUT, Target_RandomPlayer, 30, 0, 9, 0, 10);
		AddSpell(WARLORD_SALARIS_HAMSTRING, Target_ClosestPlayer, 10, 0, 20, 0, 5);
		AddSpell(WARLORD_SALARIS_MORTAL_STRIKE, Target_Current, 100, 0, 6, 0, 5);

	}

};

#define BOSS_GARAXXAS				24555
#define GARAXXAS_AIMED_SHOT			44271
#define GARAXXAS_SHOOT				15620
#define GARAXXAS_CONCUSSIV_SHOT		27634
#define GARAXXAS_MULTI_SHOT			44285
#define GARAXXAS_WING_CLIP			44286


class GaraxxasAI : public ArcScriptBossAI
{
	ARCSCRIPT_FACTORY_FUNCTION(GaraxxasAI, ArcScriptBossAI);
	GaraxxasAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{
		AddSpell(GARAXXAS_AIMED_SHOT, Target_RandomPlayer, 90, 3, 6, 5, 35);
		AddSpell(GARAXXAS_SHOOT, Target_RandomPlayer, 90, 2.5, 5, 5, 30);
		AddSpell(GARAXXAS_CONCUSSIV_SHOT, Target_RandomPlayer, 40, 0, 8, 5, 35);
		AddSpell(GARAXXAS_MULTI_SHOT, Target_RandomPlayer, 25, 0, 12, 5, 30);
		AddSpell(GARAXXAS_WING_CLIP, Target_Current, 30, 0, 9, 0, 5);


	}
};

#define BOSS_APOKO					24553
#define APOKO_FROST_SHOCK			21401
#define APOKO_LESSER_HEALING_WAVE	44256
#define APOKO_PURGE					27626

class ApokoAI : public ArcScriptCreatureAI
{
	ARCSCRIPT_FACTORY_FUNCTION(ApokoAI, ArcScriptCreatureAI);
	ApokoAI(Creature* pCreature) : ArcScriptCreatureAI(pCreature)
	{
		AddSpell(APOKO_FROST_SHOCK, Target_RandomPlayer, 40, 0, 8, 0, 20);
		AddSpell(APOKO_LESSER_HEALING_WAVE, Target_RandomFriendly, 50, 1.5, 10, 0, 40);
		AddSpell(APOKO_PURGE, Target_RandomUnit, 20, 0, 40, 0, 30);

	}
};

#define BOSS_ZELFAN					24556
#define ZELFAN_GOBLIN_DRAGON_GUN	44272
#define ZELFAN_HIGH_EXPLOSIV_SHEEP	44276
#define ZELFAN_ROCKET_LAUNCH		44137

class ZelfanAI : public ArcScriptCreatureAI
{
	ARCSCRIPT_FACTORY_FUNCTION(ZelfanAI, ArcScriptCreatureAI);
	ZelfanAI(Creature* pCreature) : ArcScriptCreatureAI(pCreature)
	{
      AddSpell(ZELFAN_GOBLIN_DRAGON_GUN, Target_Current, 90, 0, 15, 0, 5);
	  AddSpell(ZELFAN_HIGH_EXPLOSIV_SHEEP, Target_Self, 90, 2, 80);
	  AddSpell(ZELFAN_ROCKET_LAUNCH, Target_RandomPlayer, 99, 3.5, 60, 0, 45);


	}
};

#define BOSS_Kaelthas				24664
#define KAELTHAS_FIREBALL			44189
#define KAELTHAS_PHOENIX			44194
//#define KAELTHAS_FLAME_STRIKE		46162
#define KAELTHAS_GRAVITY_LAPSE		44224


class KaelThasMTAI : public ArcScriptBossAI
{
	ARCSCRIPT_FACTORY_FUNCTION(KaelThasMTAI, ArcScriptBossAI);
	KaelThasMTAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{
		//AddPhaseSpell(1, AddSpell(FELMYST_CLEAVE, Target_Current, 6, 0, 10, 0, 5));
       AddPhaseSpell(1, AddSpell(KAELTHAS_FIREBALL, Target_RandomPlayer, 40, 2, 6, 0, 40));

	   
	}
};

void SetupMagistersTerrace(ScriptMgr* pScriptMgr) 
{
	pScriptMgr->register_creature_script(BOSS_KaganiNightstrike, &Kagani_NightstrikeAI::Create);
	pScriptMgr->register_creature_script(BOSS_EllrysDuskhallow, &Ellrys_DuskhallowAI::Create);
	pScriptMgr->register_creature_script(BOSS_EramasBrightblaze, &Eramas_BrightblazeAI::Create);
	pScriptMgr->register_creature_script(BOSS_Priestess_Delrissa, &Priestess_DelrissaAI::Create);
	pScriptMgr->register_creature_script(BOSS_YAZZAI, &YazzaiAI::Create);
	pScriptMgr->register_creature_script(WARLORD_SALARIS, &Warlord_SalarisAI::Create);
	pScriptMgr->register_creature_script(BOSS_GARAXXAS, &GaraxxasAI::Create);
	pScriptMgr->register_creature_script(BOSS_APOKO, &ApokoAI::Create);
	pScriptMgr->register_creature_script(BOSS_ZELFAN, &ZelfanAI::Create);
	pScriptMgr->register_creature_script(BOSS_Kaelthas, &KaelThasMTAI::Create);
}

