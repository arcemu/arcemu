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

/* To-Do:
	Finish Kritkhir Encounter, needs more blizzlike, may need InstanceScript
	Anuburak
	Add's AI and trash
	*/

#include "Setup.h"


//Krikthir The Gatewatcher BOSS
#define BOSS_KRIKTHIR 28684

#define KRIKTHIR_MINDFLAY 52586
#define KRIKTHIR_CURSEOFFATIGUE 52592
#define KRIKTHIR_ENRAGE 28747

#define KRIKTHIR_MINDFLAY_HC 59367
#define KRIKTHIR_CURSEOFFATIGUE_HC 59368

class KrikthirAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(KrikthirAI, MoonScriptCreatureAI);
		KrikthirAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			if(!IsHeroic())
			{
				AddSpell(KRIKTHIR_CURSEOFFATIGUE, Target_Self, 100, 0, 10);
				AddSpell(KRIKTHIR_MINDFLAY, Target_RandomPlayer, 100, 0, 7, 0, 30);
			}
			else
			{
				AddSpell(KRIKTHIR_CURSEOFFATIGUE_HC, Target_Self, 100, 0, 10);
				AddSpell(KRIKTHIR_MINDFLAY_HC, Target_RandomPlayer, 100, 0, 7, 0, 30);
			};

			AddEmote(Event_OnCombatStart, "This kingdom belongs to the Scourge! Only the dead may enter.", Text_Yell, 14075);
			AddEmote(Event_OnTargetDied, "You were foolish to come.", Text_Yell, 14077);
			AddEmote(Event_OnTargetDied, "As Anub'Arak commands!", Text_Yell, 14078);
			AddEmote(Event_OnDied, "I should be grateful. But I long ago lost the capacity.", Text_Yell, 14087);

			mEnraged = false;
		};

		void AIUpdate()
		{
			if(_unit->GetHealthPct() <= 10 && mEnraged == false)
			{
				ApplyAura(KRIKTHIR_ENRAGE);
				mEnraged = true;
			};

			ParentClass::AIUpdate();
		};

		void OnDied(Unit* pKiller)
		{
			GameObject* Doors = GetNearestGameObject(192395);
			if(Doors != NULL)
				Doors->Despawn(0, 0);

			ParentClass::OnDied(pKiller);
		};

		bool mEnraged;
};

//boss Hadronox
#define BOSS_HADRONOX 28921

#define HADRONOX_WEBGRAB 53406
#define HADRONOX_PIERCEARMOR 53418
#define HADRONOX_LEECHPOISON 53030
#define HADRONOX_ACIDCLOUD 53400

#define HADRONOX_WEBGRAB_HC 59421
#define HADRONOX_LEECHPOISON_HC 59417
#define HADRONOX_ACIDCLOUD_HC 59419

class HadronoxAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(HadronoxAI, MoonScriptCreatureAI);
		HadronoxAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			if(!IsHeroic())
			{
				AddSpell(HADRONOX_WEBGRAB, Target_RandomPlayer, 22, 0, 14, 0, 0);
				AddSpell(HADRONOX_LEECHPOISON, Target_Self, 14, 0, 25, 0, 20);
				AddSpell(HADRONOX_ACIDCLOUD, Target_RandomPlayer, 18, 0, 20, 0, 60);
			}
			else
			{
				AddSpell(HADRONOX_WEBGRAB_HC, Target_RandomPlayer, 22, 0, 14, 0, 0);
				AddSpell(HADRONOX_LEECHPOISON_HC, Target_Self, 14, 0, 25, 0, 20);
				AddSpell(HADRONOX_ACIDCLOUD_HC, Target_RandomPlayer, 18, 0, 20, 0, 60);
			};

			AddSpell(HADRONOX_PIERCEARMOR, Target_ClosestPlayer, 20, 0, 5, 0, 0);
		};

};

//Watcher Gashra.
#define CN_GASHRA 28730

#define GASHRA_WEBWRAP 52086
#define GASHRA_INFECTEDBITE 52469
#define GASHRA_ENRAGE 52470

class GashraAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(GashraAI, MoonScriptCreatureAI);
		GashraAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(GASHRA_WEBWRAP, Target_RandomPlayer, 22, 0, 35, 0, 0);
			AddSpell(GASHRA_INFECTEDBITE, Target_ClosestPlayer, 35, 0, 12, 0, 0);
		};

};

//Watcher Narjil
#define CN_NARJIL 28729

#define NARJIL_WEBWRAP 52086
#define NARJIL_INFECTEDBITE 52469
#define NARJIL_BLINDINGWEBS 52524

class NarjilAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(NarjilAI, MoonScriptCreatureAI);
		NarjilAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(NARJIL_WEBWRAP, Target_RandomPlayer, 22, 0, 35, 0, 0);
			AddSpell(NARJIL_INFECTEDBITE, Target_ClosestPlayer, 35, 0, 12, 0, 0);
			AddSpell(NARJIL_BLINDINGWEBS, Target_ClosestPlayer, 16, 0, 9, 0, 0);
		};

};

//Watcher Silthik
#define CN_SILTHIK 28731

#define SILTHIK_WEBWRAP 52086
#define SILTHIK_INFECTEDBITE 52469
#define SILTHIK_POISONSPRAY 52493

class SilthikAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(SilthikAI, MoonScriptCreatureAI);
		SilthikAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(NARJIL_WEBWRAP, Target_RandomPlayer, 22, 0, 35, 0, 0);
			AddSpell(NARJIL_INFECTEDBITE, Target_ClosestPlayer, 35, 0, 12, 0, 0);
			AddSpell(SILTHIK_POISONSPRAY, Target_RandomPlayer, 30, 0, 15, 0, 0);
		};

};

//Anub'ar Shadowcaster (anub shadowcaster)
#define CN_ANUB_SHADOWCASTER 28733

#define SHADOWCASTER_SHADOWBOLT 52534
#define SHADOWCASTER_SHADOW_NOVA 52535

class AnubShadowcasterAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(AnubShadowcasterAI, MoonScriptCreatureAI);
		AnubShadowcasterAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(SHADOWCASTER_SHADOWBOLT, Target_RandomPlayer, 36, 0, 8);
			AddSpell(SHADOWCASTER_SHADOW_NOVA, Target_Self, 22, 0, 15);
		};

};

void SetupAzjolNerub(ScriptMgr* mgr)
{
	//Bosses
	mgr->register_creature_script(BOSS_KRIKTHIR, &KrikthirAI::Create);
	mgr->register_creature_script(BOSS_HADRONOX, &HadronoxAI::Create);

	// watchers
	mgr->register_creature_script(CN_GASHRA, &GashraAI::Create);
	mgr->register_creature_script(CN_NARJIL, &NarjilAI::Create);
	mgr->register_creature_script(CN_SILTHIK, &SilthikAI::Create);
}
