/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008-2009 Arcemu Team
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

/************************************************************************/
/* Raid_Karazhan.cpp Script												*/
/************************************************************************/

struct Coords
{
	float x;
	float y;
	float z;
	float o;
};

// Partially by Plexor (I used a spell before, but changed to his method)
class SCRIPT_DECL Berthold : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4037, Plr);

		Menu->AddItem( 0, "What is this place?", 1 );
		Menu->AddItem( 0, "Where is Medivh?", 2 );
		Menu->AddItem( 0, "How do you navigate the tower?", 3 );

		//Killing the Shade of Aran makes a teleport to medivh's available from Berthold the Doorman.
		Unit* soa = pObject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-11165.2f, -1912.13f, 232.009f, 16524);
		if(!soa || !soa->isAlive())
			Menu->AddItem( 0, "Please teleport me to the Guardian's Library.", 4);

		if(AutoSend)
			Menu->SendTo(Plr);
	}

	void GossipSelectOption(Object* pObject, Player * Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		switch (IntId)
		{
		case 0:
			GossipHello(pObject, Plr, true);
			break;

		case 4:
			Plr->SafeTeleport(Plr->GetMapId(), Plr->GetInstanceID(), -11165.123f, -1911.13f, 232.009f, 2.3255f);
			break;
		}
	}

	void Destroy()
	{
		delete this;
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Attumen the Huntsman (and Midnight)
#define CN_MIDNIGHT					16151
#define CN_ATTUMEN					15550
#define ATTUMEN_SHADOW_CLEAVE		29832
#define ATTUMEN_BERSERKER_CHARGE	22886
#define ATTUMEN_INTANGIBLE_PRESENCE	29833

class AttumenTheHuntsmanAI : public ArcScriptBossAI
{
	ARCSCRIPT_FACTORY_FUNCTION(AttumenTheHuntsmanAI, ArcScriptBossAI);
	AttumenTheHuntsmanAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{
		mMidnight = NULL;

		//All phase spells
		AddSpell(ATTUMEN_SHADOW_CLEAVE, Target_Current, 15, 0, 6, 0, 5, true);
		AddSpell(ATTUMEN_INTANGIBLE_PRESENCE, Target_Current, 15, 0, 12, 0, 5, true);

		//Phase 2 spells
		AddPhaseSpell(2, AddSpell(ATTUMEN_BERSERKER_CHARGE, Target_RandomPlayerNotCurrent, 15, 0, 6, 15, 40, true));

		//Emotes
		AddEmote(Event_OnCombatStart, "Cowards! Wretches!", Text_Yell, 9167);
		AddEmote(Event_OnCombatStart, "Who dares attack the steed of the Huntsman?", Text_Yell, 9298);
		AddEmote(Event_OnCombatStart, "Perhaps you would rather test yourselves against a more formidable opponent!", Text_Yell, 9299);
		AddEmote(Event_OnTargetDied, "It was... inevitable.", Text_Yell, 9169);
		AddEmote(Event_OnTargetDied, "Another trophy to add to my collection!", Text_Yell, 9300);
		AddEmote(Event_OnDied, "Always knew... someday I would become... the hunted.", Text_Yell, 9165);
		AddEmote(Event_OnTaunt, "Such easy sport.", Text_Yell, 9170);
		AddEmote(Event_OnTaunt, "Amatures! Do not think you can best me! I kill for a living.", Text_Yell, 9304);

		AggroNearestUnit(); //Aggro on spawn
	}

	void OnCombatStart(Unit* pTarget)
	{
		mMidnight = (ArcScriptBossAI*)GetNearestCreature(CN_MIDNIGHT);
		ParentClass::OnCombatStart(pTarget);
	}

	void OnCombatStop(Unit* pTarget)
	{
		Despawn(10000);
		ParentClass::OnCombatStop(pTarget);
	}

	void AIUpdate()
	{
		if( GetPhase() == 1 )
		{
			if( mMidnight && mMidnight->IsAlive() && GetHealthPercent() <= 25 && !IsCasting() )
			{
				SetPhase(2);
				SetAllowMelee(false);
				SetAllowSpell(false);
				Emote("Come Midnight, let's disperse this petty rabble!", Text_Yell, 9168);
				mMidnight->SetPhase(2);
				mMidnight->MoveTo(this);
				mMidnight->SetAllowMelee(false);
			}
		}
		ParentClass::AIUpdate();
	}

	ArcScriptBossAI* mMidnight;
};

class MidnightAI : public ArcScriptBossAI
{
	ARCSCRIPT_FACTORY_FUNCTION(MidnightAI, ArcScriptBossAI);
	MidnightAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{
		mAttumen = NULL;
	}

	void OnCombatStart(Unit* pTarget)
	{
		mAttumen = NULL;
		ParentClass::OnCombatStart(pTarget);
	}

	void OnCombatStop(Unit* pTarget)
	{
		mAttumen = NULL;
		SetAllowMelee(true);
		SetAllowSpell(true);
		ParentClass::OnCombatStop(pTarget);
	}

	void OnTargetDied(Unit* pTarget)
	{
		if( mAttumen && mAttumen->IsAlive() )
		{
			mAttumen->Emote("Well done Midnight!", Text_Yell, 9173);
		}
		ParentClass::OnTargetDied(pTarget);
	}

	void AIUpdate()
	{
		if( GetPhase() == 1 )
		{
			if( !mAttumen && GetHealthPercent() <= 95 && !IsCasting() )
			{
				Emote("Midnight calls for her master!", Text_Emote);
				mAttumen = (ArcScriptBossAI*)SpawnCreature(CN_ATTUMEN);
			}
			else if( mAttumen && mAttumen->IsAlive() && GetHealthPercent() <= 25 && !IsCasting() )
			{
				SetPhase(2);
				MoveTo(mAttumen);
				SetAllowMelee(false);
				mAttumen->SetPhase(2);
				mAttumen->SetAllowMelee(false);
				mAttumen->SetAllowSpell(false);
				mAttumen->Emote("Come Midnight, let's disperse this petty rabble!", Text_Yell, 9168);
			}
		}
		else if( GetPhase() == 2 )
		{
			if( mAttumen && mAttumen->IsAlive() )
			{
				if( GetRange(mAttumen) <= 15 )
				{
					mAttumen->Regenerate();
					mAttumen->SetDisplayId(16040);
					mAttumen->ClearHateList();
					mAttumen->SetAllowMelee(true);
					mAttumen->SetAllowSpell(true);
					mAttumen = NULL;
					Despawn();
				}
				else MoveTo(mAttumen);
			}
		}
		ParentClass::AIUpdate();
	}

	ArcScriptBossAI* mAttumen;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Moroes
#define CN_MOROES		15687
#define MOROES_GOUGE	28456
#define MOROES_VANISH	29448
#define MOROES_BLIND	34654
#define MOROES_ENRAGE	37023
#define MOROES_GARROTE	37066

class MoroesAI : public ArcScriptBossAI
{
	ARCSCRIPT_FACTORY_FUNCTION(MoroesAI, ArcScriptBossAI);
	MoroesAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{
		//Initialize timers
		mVanishTimer = mGarroteTimer = INVALIDATE_TIMER;

		//Phase 1 spells
		AddPhaseSpell(1, AddSpell(MOROES_GOUGE, Target_Current, 20, 0, 10, 0, 5));
		AddPhaseSpell(1, AddSpell(MOROES_BLIND, Target_ClosestPlayerNotCurrent, 20, 0, 10, 0, 10, true));
		mVanish = AddSpell(MOROES_VANISH, Target_Self, 0, 12, 0);
		mVanish->AddEmote("Now, where was I? Oh yes...", Text_Yell, 9215);
		mVanish->AddEmote("You rang?", Text_Yell, 9316);
		mEnrage = AddSpell(MOROES_ENRAGE, Target_Self, 0, 0, 0);

		//Phase 2 spells
		mGarrote = AddSpell(MOROES_GARROTE, Target_RandomPlayer, 0, 0, 0);

		//Emotes
		AddEmote(Event_OnCombatStart, "Hm, unannounced visitors. Preparations must be made...", Text_Yell, 9211);
		AddEmote(Event_OnDied, "How terribly clumsy of me...", Text_Yell, 9213);
		AddEmote(Event_OnTargetDied, "One more for dinner this evening.", Text_Yell, 9214);
		AddEmote(Event_OnTargetDied, "Time... Never enough time.", Text_Yell, 9314);
		AddEmote(Event_OnTargetDied, "I've gone and made a mess.", Text_Yell, 9315);
	}

	void OnCombatStart(Unit *pTarget)
	{
		mEnrage->mEnabled = true;
		mVanishTimer = AddTimer(35000);	//First vanish after 35sec
		ParentClass::OnCombatStart(pTarget);
	}

	void OnDied(Unit* pKiller)
	{
		RemoveAuraOnPlayers(MOROES_GARROTE);
		ParentClass::OnDied(pKiller);
	}

	void AIUpdate()
	{
		if( GetPhase() == 1 )
		{
			if( mEnrage->mEnabled && GetHealthPercent() <= 30 && !IsCasting() )
			{
				CastSpell(mEnrage);
				mEnrage->mEnabled = false;
			}
			else if( IsTimerFinished(mVanishTimer) && !IsCasting() )
			{
				SetPhase(2, mVanish);
				mGarroteTimer = AddTimer(12000);
				ResetTimer(mVanishTimer, 35000);
			}
		}
		else if( GetPhase() == 2 )
		{
			if( IsTimerFinished(mGarroteTimer) && !IsCasting() )
			{
				SetPhase(1, mGarrote);
				RemoveAura(MOROES_VANISH);
				RemoveTimer(mGarroteTimer);
			}
		}
		ParentClass::AIUpdate();
	}

	SpellDesc*	mVanish;
	SpellDesc*	mGarrote;
	SpellDesc*	mEnrage;
	int32		mVanishTimer, mGarroteTimer;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Maiden of Virtue
#define CN_MAIDENOFVIRTUE			16457
#define MAIDENOFVIRTUE_REPENTANCE	29511
#define MAIDENOFVIRTUE_HOLY_GROUND	29512
#define MAIDENOFVIRTUE_HOLY_FIRE	29522
#define MAIDENOFVIRTUE_HOLY_WRATH	32445

class MaidenOfVirtueAI : public ArcScriptBossAI
{
	ARCSCRIPT_FACTORY_FUNCTION(MaidenOfVirtueAI, ArcScriptBossAI);
	MaidenOfVirtueAI(Creature* pCreature) : ArcScriptBossAI(pCreature)
	{
		//Spells
		AddSpell(MAIDENOFVIRTUE_HOLY_GROUND, Target_Self, 100, 0, 3);
		AddSpell(MAIDENOFVIRTUE_HOLY_FIRE, Target_RandomPlayer, 25, 1, 15, 0, 50);
		AddSpell(MAIDENOFVIRTUE_HOLY_WRATH, Target_RandomPlayer, 25, 0, 20, 0, 100);
		mRepentance = AddSpell(MAIDENOFVIRTUE_REPENTANCE, Target_Self, 25, 0.6f, 30);
		mRepentance->AddEmote("Cast out your corrupt thoughts.", Text_Yell, 9313);
		mRepentance->AddEmote("Your impurity must be cleansed.", Text_Yell, 9208);

		//Emotes
		AddEmote(Event_OnCombatStart, "Your behavior will not be tolerated.", Text_Yell, 9204);
		AddEmote(Event_OnTargetDied, "Ah ah ah...", Text_Yell, 9207);
		AddEmote(Event_OnTargetDied, "This is for the best.", Text_Yell, 9312);
		AddEmote(Event_OnTargetDied, "Impure thoughts lead to profane actions.", Text_Yell, 9311);
		AddEmote(Event_OnDied, "Death comes. Will your conscience be clear?", Text_Yell, 9206);
	}

	void OnCombatStart(Unit* pTarget)
	{
		mRepentance->TriggerCooldown();	//No repentance at the beginning of the fight
		ParentClass::OnCombatStart(pTarget);
	}

	SpellDesc* mRepentance;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//The Big Bad Wolf
#define CN_BIGBADWOLF					17521

#define TERRIFYING_HOWL					30752
#define WIDE_SWIPE						6749
// Combines display visual + buff
#define REDRIDINGHOOD_DEBUFF			30768
#define PBS_TAUNT						30755	// Picnic Basket Smell (taunt)

class BigBadWolfAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(BigBadWolfAI);
	bool m_spellcheck[4];
	SP_AI_Spell spells[4];

	BigBadWolfAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 3;
		for(int i = 0; i < nrspells; i++)
			m_spellcheck[i] = false;

		spells[0].info = dbcSpell.LookupEntry(TERRIFYING_HOWL);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 50.0f;
		spells[0].cooldown = 30;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(WIDE_SWIPE);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].perctrigger = 50.0f;
		spells[1].instant = true;
		spells[1].cooldown = 20;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(REDRIDINGHOOD_DEBUFF);
		spells[2].targettype = TARGET_RANDOM_SINGLE;
		spells[2].perctrigger = 50.0f;
		spells[2].instant = true;
		spells[2].cooldown = 45;
		spells[2].attackstoptimer = 1000;
		spells[2].mindist2cast = 0.0f;
		spells[2].maxdist2cast = 60.0f;
		spells[2].soundid = 9278;
		spells[2].speech = "Run away little girl, run away!";

		spells[3].info = dbcSpell.LookupEntry(PBS_TAUNT);
		spells[3].targettype = TARGET_RANDOM_SINGLE;
		spells[3].perctrigger = 0.0f;
		spells[3].instant = true;
		spells[3].cooldown = 0;
		spells[3].attackstoptimer = 1000;
	}

	void OnCombatStart(Unit* mTarget)
	{
		for(int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;

		ThreatAdd = false;
		m_threattimer = 20;

		_unit->PlaySoundToSet(9276);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "All the better to own you with!");
		RegisterAIUpdateEvent(1000);

	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		_unit->PlaySoundToSet(9275);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Aarrhhh.");
		RemoveAIUpdateEvent();

		GameObject* DoorLeftO = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10917.1445f, -1774.05f, 90.478f, 184279);
		GameObject* DoorRightO = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10872.195f, -1779.42f, 90.45f, 184278);

		if(DoorLeftO)
			DoorLeftO->SetByte(GAMEOBJECT_BYTES_1, 0, 0);

		if(DoorRightO)
			DoorRightO->SetByte(GAMEOBJECT_BYTES_1, 0, 0);
	}

	void OnTargetDied(Unit* mTarget)
	{
		_unit->PlaySoundToSet(9277);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Mmmm... delicious.");
	}

	void AIUpdate()
	{
		if(ThreatAdd == true)
		{
			m_threattimer--;
		}
		else if(!m_threattimer)
		{
			_unit->GetAIInterface()->taunt(RTarget, false);
			ThreatAdd = false;
			m_threattimer = 19;
		}

		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;

				if (m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					case TARGET_RANDOM_SINGLE:
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Red Riding Hood cast");
							std::vector<Player *> TargetTable;
							for(set<Player*>::iterator itr = _unit->GetInRangePlayerSetBegin(); 
								itr != _unit->GetInRangePlayerSetEnd(); ++itr)
							{
								Player *RandomTarget = NULL;
								RandomTarget = static_cast< Player* >(*itr);
								if(RandomTarget && RandomTarget->isAlive())
									TargetTable.push_back(RandomTarget);
								RandomTarget = NULL;
							}

							if (!TargetTable.size())
								return;

							size_t RandTarget = rand()%TargetTable.size();

							RTarget = TargetTable[RandTarget];

							if (!RTarget)
								return;

							_unit->CastSpell(RTarget, spells[2].info, spells[2].instant);
							_unit->GetAIInterface()->taunt(RTarget, true);
							ThreatAdd = true;
							m_threattimer = 19;
						}break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

protected:

	int nrspells;
	int m_threattimer;
	bool ThreatAdd;
	Unit *RTarget;
};


#define TERRIFYING_HOWL 30752
#define MORPH_LITTLE_RED_RIDING_HOOD 30768
#define DEBUFF_LITTLE_RED_RIDING_HOOD 30756

class THEBIGBADWOLFAI : public CreatureAIScript
{
public:
ADD_CREATURE_FACTORY_FUNCTION(THEBIGBADWOLFAI);
bool m_spellcheck[3];
SP_AI_Spell spells[3];

THEBIGBADWOLFAI(Creature* pCreature) : CreatureAIScript(pCreature)
{
//SpellEntry *infoImmunity;
nrspells = 3;
for(int i=0;i<nrspells;i++)
{
m_spellcheck[i] = false;
}

spells[0].info = dbcSpell.LookupEntry(TERRIFYING_HOWL);
spells[0].targettype = TARGET_VARIOUS;
spells[0].instant = true;
spells[0].cooldown = 10;
spells[0].perctrigger = 0.0f;
spells[0].attackstoptimer = 1000;

spells[1].info = dbcSpell.LookupEntry(MORPH_LITTLE_RED_RIDING_HOOD);
spells[1].targettype = TARGET_ATTACKING;
spells[1].instant = true;
spells[1].cooldown = 30;
spells[1].perctrigger = 0.0f;
spells[1].attackstoptimer = 1000;

spells[2].info = dbcSpell.LookupEntry(DEBUFF_LITTLE_RED_RIDING_HOOD);
spells[2].targettype = TARGET_ATTACKING;
spells[2].instant = true;
spells[2].cooldown = 30;
spells[2].perctrigger = 0.0f;
spells[2].attackstoptimer = 1000;
spells[2].soundid = 9278;
spells[2].speech = "Run away little girl, run away!";
}

void OnCombatStart(Unit* mTarget)
{
CastTime();
_unit->PlaySoundToSet(9276);
_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The better to own you with!");
RegisterAIUpdateEvent(1000);
}

void OnCombatStop(Unit *mTarget)
{
GameObject* DoorLeft = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10917.1445f, -1774.05f, 90.478f, 184279);
GameObject* DoorRight = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10872.195f, -1779.42f, 90.45f, 184278);
GameObject* Curtain = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10894.17f, -1774.218f, 90.477f, 183932);

if(DoorLeft)
DoorLeft->SetByte(GAMEOBJECT_BYTES_1, 0, 1);

if(DoorRight)
DoorRight->SetByte(GAMEOBJECT_BYTES_1, 0, 0);

if(Curtain)
Curtain->SetByte(GAMEOBJECT_BYTES_1, 0, 1);

CastTime();
_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
_unit->GetAIInterface()->SetAIState(STATE_IDLE);
RemoveAIUpdateEvent();
}

void OnDied(Unit * mKiller)
{
GameObject* DoorLeft = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10917.1445f, -1774.05f, 90.478f, 184279);
GameObject* DoorRight = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10872.195f, -1779.42f, 90.45f, 184278);
GameObject* Curtain = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10894.17f, -1774.218f, 90.477f, 183932);

if(DoorLeft)
DoorLeft->SetByte(GAMEOBJECT_BYTES_1, 0, 0);

if(DoorRight)
DoorRight->SetByte(GAMEOBJECT_BYTES_1, 0, 0);

// Make sure the curtain stays up
if(Curtain)
Curtain->SetByte(GAMEOBJECT_BYTES_1, 0, 0);

CastTime();
_unit->PlaySoundToSet(9275);
_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "AArrhhh.");
RemoveAIUpdateEvent();
}

void OnTargetDied(Unit* mTarget)
{
_unit->PlaySoundToSet(9277);
_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Mmmm... delicious.");
}

void AIUpdate()
{
float val = (float)RandomFloat(100.0f);
SpellCast(val);
}

void CastTime()
{
for(int i=0;i<nrspells;i++)
spells[i].casttime = spells[i].cooldown;
}

void SpellCast(float val)
{
if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
{
float comulativeperc = 0;
Unit *target = NULL;
for(int i=0;i<nrspells;i++)
{
spells[i].casttime--;

if (m_spellcheck[i])
{
spells[i].casttime = spells[i].cooldown;
target = _unit->GetAIInterface()->GetNextTarget();
switch(spells[i].targettype)
{
case TARGET_SELF:
case TARGET_VARIOUS:
_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
case TARGET_ATTACKING:
_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
case TARGET_DESTINATION:
_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
}

if (spells[i].speech != "")
{
_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
_unit->PlaySoundToSet(spells[i].soundid); 
}

m_spellcheck[i] = false;
return;
}

if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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


uint32 WayStartBBW[1000000];

#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, Plr); \
	Menu->SendTo(Plr);

class SCRIPT_DECL BarnesGS : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		if(WayStartBBW[pObject->GetInstanceID()] == 5)
		{
			//Splendid! Marvelous! An excellent performance!    (when opera event is over)
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 8975, Plr);
		}
		else
		{	
			//Finally, everything is in place. Are you ready for your big stage debut?
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 8970, Plr);
			Menu->AddItem( 0, "I'm not an actor.", 1 );
			if(AutoSend)
				Menu->SendTo(Plr);

		}
	}

	void GossipSelectOption(Object* pObject, Player * Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		switch (IntId)
		{
		case 0:
			GossipHello(pObject, Plr, true);
			break;

		case 1:
			{
				//Don't worry, you'll be fine. You look like a natural!
				GossipMenu * Menu;
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 8971, Plr);
				Menu->AddItem( 0, "Ok, I'll give it a try, then.", 2 );
				Menu->SendTo(Plr);
			}break;
		case 2:
			{
				Creature *pCreature = static_cast< Creature* >(pObject);
				pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Splendid. I'm going to get the audience ready. Break a leg!");
				pCreature->CastSpell(pObject->GetGUID(), 32616, false);
				pCreature->GetAIInterface()->StopMovement(0);
				pCreature->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
				pCreature->GetAIInterface()->setMoveType(11);
				pCreature->GetAIInterface()->setWaypointToMove(0);
				pCreature->SetUInt32Value(UNIT_NPC_FLAGS, 0);
				pCreature->PlaySoundToSet(9357);
				WayStartBBW[pCreature->GetInstanceID()] = 2;
			}break;
		}
	}

	void Destroy()
	{
		delete this;
	}
};

class SCRIPT_DECL GrandMother : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7245, Plr);

		Menu->AddItem( 0, "What phat lewts you have Grandmother!", 1);

		if(AutoSend)
			Menu->SendTo(Plr);
	}

	void GossipSelectOption(Object* pObject, Player * Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		switch (IntId)
		{
		case 0:
			GossipHello(pObject, Plr, true);
			break;

		case 1:
			{
				static_cast< Creature* >(pObject)->Despawn(100, 0);
				Creature *pop = pObject->GetMapMgr()->GetInterface()->SpawnCreature(17521, pObject->GetPositionX(), pObject->GetPositionY(), 
					pObject->GetPositionZ(), 0, true, true, 0, 0);
				pop->GetAIInterface()->AttackReaction(Plr, 1, 0);
				break;
			}
		}
	}

	void Destroy()
	{
		delete this;
	}
};

/*	Alot of the code for this script was taken from M4ksiu and his Black Temple script, 
who I'd like to thank for his contributions to the scripting scene.	*/
static Coords Barnes[] = 
{
	{ },
	{ -10873.91f, -1780.177f, 90.50f, 3.3f },
	{ -10895.299805f, -1783.349976f, 90.50f, 4.5f },
	{ -10873.91f, -1780.177f, 90.50f, 3.3f },
	{ -10868.4f, -1781.63f, 90.50f, 1.24f }
};

class BarnesAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(BarnesAI);

	BarnesAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(1, 0, 0));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(2, 43000, 0));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(3, 0, 0));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(4, 0, 0));

		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);

		WayStartBBW[_unit->GetInstanceID()] = 1;

		eventRand = 0;
		switch(RandomUInt(2))
		{
		case 0:
			eventRand = 0;
			break;
		case 1:
			eventRand = 1;
			break;
		case 2:
			eventRand = 2;
			break;
		}
	}

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		switch (iWaypointId)
		{
		case 0:
			_unit->GetAIInterface()->setWaypointToMove(1);
			WayStartBBW[_unit->GetInstanceID()] = 3;
			break;
		case 1:
			break;
		case 2:
			cleanStage();
			WayStartBBW[_unit->GetInstanceID()] = 4;
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(19525, Barnes[2].x, Barnes[2].y, Barnes[2].z, Barnes[2].o, true, false, 0, 0)->Despawn(43000, 0);
			_unit->SetFacing(4.5f);
			switch(eventRand)
			{
			case 0:
				BarnesSpeakRed();
				break;
			case 1:
				BarnesSpeakRJ();
				break;
			case 2:
				BarnesSpeakWOZ();
				break;
			}
			break;
		case 3:
			switch(eventRand)
			{
			case 0:
				EventRed();
				break;
			case 1:
				EventRJ();
				break;
			case 2:
				EventWOZ();
				break;
			}
			_unit->SetUInt32Value(UNIT_FIELD_FLAGS, 1);
			WayStartBBW[_unit->GetInstanceID()] = 5;
			break;
		}
	}

	void cleanStage()
	{
		GameObject* DoorLeft = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10917.1445f, -1774.05f, 90.478f, 184279);
		GameObject* DoorRight = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10872.195f, -1779.42f, 90.45f, 184278);
		GameObject* Curtain = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10894.17f, -1774.218f, 90.477f, 183932);

		if(DoorLeft)
			DoorLeft->SetByte(GAMEOBJECT_BYTES_1, 0, 1);

		if(DoorRight)
			DoorRight->SetByte(GAMEOBJECT_BYTES_1, 0, 0);

		if(Curtain)
			Curtain->SetByte(GAMEOBJECT_BYTES_1, 0, 1);

		Creature* Julianne	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10883.0f, -1751.81f, 90.4765f, 17534);
		Creature* Romulo	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10883.0f, -1751.81f, 90.4765f, 17533);
		Creature* BigBadWolf= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10883.0f, -1751.81f, 90.4765f, 17521);
		Creature* Grandma	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10883.0f, -1751.81f, 90.4765f, 17603);

		Creature* Dorothee	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10897.650f, -1755.8311f, 90.476f, 17535); //Dorothee
		Creature* Strawman	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10904.088f, -1754.8988f, 90.476f, 17543); //Strawman
		Creature* Roar		= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10891.115f, -1756.4898f, 90.476f, 17546);//Roar
		Creature* Tinman	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10884.501f, -1757.3249f, 90.476f, 17547); //Tinman

		GameObject* House = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10883.0f, -1751.81f, 90.4765f, 183493);
		GameObject* Tree  = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10877.7f, -1763.18f, 90.4771f, 183492);
		GameObject* Tree2 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10906.7f, -1750.01f, 90.4765f, 183492);
		GameObject* Tree3 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10909.5f, -1761.79f, 90.4773f, 183492);
		//GameObject* BackDrop = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10890.9f, -1744.06f, 90.4765f, 183491);

		if(Julianne)
			Julianne->SafeDelete();
		if(Romulo)
			Romulo->SafeDelete();

		if(BigBadWolf)
			BigBadWolf->SafeDelete();
		if(Grandma)
			Grandma->SafeDelete();

		if(Dorothee)
			Dorothee->SafeDelete();
		if(Strawman)
			Strawman->SafeDelete();
		if(Roar)
			Roar->SafeDelete();
		if(Tinman)
			Tinman->SafeDelete();

		if(House)
			House->GetMapMgr()->GetInterface()->DeleteGameObject(House);
		if(Tree)
			Tree->GetMapMgr()->GetInterface()->DeleteGameObject(Tree);
		if(Tree2)
			Tree2->GetMapMgr()->GetInterface()->DeleteGameObject(Tree2);
		if(Tree3)
			Tree3->GetMapMgr()->GetInterface()->DeleteGameObject(Tree3);
		//if(BackDrop)
		//	BackDrop->GetMapMgr()->GetInterface()->DeleteGameObject(BackDrop);
	}

	void BarnesSpeakWOZ()
	{
		// Start text
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Good evening, ladies and gentleman. Welcome to this evening's presentation!");
		_unit->PlaySoundToSet(9175);
		// Timed text 1
		sEventMgr.AddEvent(static_cast< Unit* >(_unit), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL, 
			"Tonight, we plumb the depths of the human soul as we join a lost, lonely girl trying desperately, with the help of her loyal companions, to find her way home.", 
			EVENT_UNIT_CHAT_MSG, 7000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		sEventMgr.AddEvent(static_cast< Object* >(_unit), &Object::PlaySoundToSet, (uint32)9338, EVENT_UNK, 7000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		// Timed text 2
		sEventMgr.AddEvent(static_cast< Unit* >(_unit), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL,
			"But she is pursued by a wicked, malevolent crone!", 
			EVENT_UNIT_CHAT_MSG, 23000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		sEventMgr.AddEvent(static_cast< Object* >(_unit), &Object::PlaySoundToSet, (uint32)9339, EVENT_UNK, 23000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		// Timed text 3
		sEventMgr.AddEvent(static_cast< Unit* >(_unit), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL, 
			"Will she survive? Will she prevail? Only time will tell. And now: On with the show!", 
			EVENT_UNIT_CHAT_MSG, 32000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		sEventMgr.AddEvent(static_cast< Object* >(_unit), &Object::PlaySoundToSet, (uint32)9340, EVENT_UNK, 32000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		// Applause
		sEventMgr.AddEvent(static_cast< Object* >(_unit), &Object::PlaySoundToSet, (uint32)9332, EVENT_UNK, 41000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}

	void EventWOZ()
	{
		GameObject* DoorLeft = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10917.1445f, -1774.05f, 90.478f, 184279);
		GameObject* DoorRight = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10872.195f, -1779.42f, 90.45f, 184278);
		GameObject* Curtain = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10894.17f, -1774.218f, 90.477f, 183932);

		if(DoorLeft)
			DoorLeft->SetByte(GAMEOBJECT_BYTES_1, 0, 1);

		if(DoorRight)
			DoorRight->SetByte(GAMEOBJECT_BYTES_1, 0, 1);

		if(Curtain)
			Curtain->SetByte(GAMEOBJECT_BYTES_1, 0, 0);

		_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID, 16616);

		_unit->GetMapMgr()->GetInterface()->SpawnCreature(17535, -10897.650f, -1755.8311f, 90.476f, 4.61f, false, true, 0, 0); //Dorothee
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(17543, -10904.088f, -1754.8988f, 90.476f, 4.61f, false, true, 0, 0); //Strawman
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(17546, -10891.115f, -1756.4898f, 90.476f, 4.61f, false, true, 0, 0);//Roar
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(17547, -10884.501f, -1757.3249f, 90.476f, 4.61f, false, true, 0, 0); //Tinman
	}

	void BarnesSpeakRJ()
	{
		// Start text
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Good evening, ladies and gentleman. Welcome to this evening's presentation!");
		_unit->PlaySoundToSet(9175);
		// Timed text 1
		sEventMgr.AddEvent(static_cast< Unit* >(_unit), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL, 
			"Tonight we explore a tale of forbidden love!", EVENT_UNIT_CHAT_MSG, 6000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		sEventMgr.AddEvent(static_cast< Object* >(_unit), &Object::PlaySoundToSet, (uint32)9341, EVENT_UNK, 6000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		// Timed text 2
		sEventMgr.AddEvent(static_cast< Unit* >(_unit), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL,
			"But beware, for not all love stories end happily. As you may find out, sometimes love pricks like a thorn.", 
			EVENT_UNIT_CHAT_MSG, 19300, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		sEventMgr.AddEvent(static_cast< Object* >(_unit), &Object::PlaySoundToSet, (uint32)9342, EVENT_UNK, 19300, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		// Timed text 3

		sEventMgr.AddEvent(static_cast< Unit* >(_unit), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL, 
			"But don't take it from me. See for yourself what tragedy lies ahead when the paths of star-crossed lovers meet. And now: On with the show!", 
			EVENT_UNIT_CHAT_MSG, 32000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		sEventMgr.AddEvent(static_cast< Object* >(_unit), &Object::PlaySoundToSet, (uint32)9343, EVENT_UNK, 32000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		// Applause
		sEventMgr.AddEvent(static_cast< Object* >(_unit), &Object::PlaySoundToSet, (uint32)9332, EVENT_UNK, 41000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}

	void EventRJ()
	{
		GameObject* DoorLeft = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10917.1445f, -1774.05f, 90.478f, 184279);
		GameObject* DoorRight = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10872.195f, -1779.42f, 90.45f, 184278);
		GameObject* Curtain = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10894.17f, -1774.218f, 90.477f, 183932);

		if(DoorLeft)
			DoorLeft->SetByte(GAMEOBJECT_BYTES_1, 0, 1);

		if(DoorRight)
			DoorRight->SetByte(GAMEOBJECT_BYTES_1, 0, 1);

		if(Curtain)
			Curtain->SetByte(GAMEOBJECT_BYTES_1, 0, 0);

		_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID, 16616);
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(17534, -10891.582f, -1755.5177f, 90.476f, 4.61f, false, true, 0, 0); //Spawn Julianne
	}

	void BarnesSpeakRed()
	{
		// Start text
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Good evening, ladies and gentleman. Welcome to this evening's presentation!");
		_unit->PlaySoundToSet(9175);
		// Timed text 1
		sEventMgr.AddEvent(static_cast< Unit* >(_unit), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL, 
			"Tonight things are not what they seems for tonight your eyes may not be trusted.", 
			EVENT_UNIT_CHAT_MSG, 7000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		sEventMgr.AddEvent(static_cast< Object* >(_unit), &Object::PlaySoundToSet, (uint32)9335, EVENT_UNK, 7000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		// Timed text 2
		sEventMgr.AddEvent(static_cast< Unit* >(_unit), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL,
			"Take for instance this quiet elderly woman waiting for a visit from her granddaughter. Surely there is nothing to fear from this sweet gray-haired old lady.", 
			EVENT_UNIT_CHAT_MSG, 17000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		sEventMgr.AddEvent(static_cast< Object* >(_unit), &Object::PlaySoundToSet, (uint32)9336, EVENT_UNK, 17000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		// Timed text 3
		sEventMgr.AddEvent(static_cast< Unit* >(_unit), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL, 
			"But don't let me pull the wool over your eyes. See for yourself what lies beneath those covers. And now: On with the show!", 
			EVENT_UNIT_CHAT_MSG, 32000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		sEventMgr.AddEvent(static_cast< Object* >(_unit), &Object::PlaySoundToSet, (uint32)9337, EVENT_UNK, 32000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		// Applause
		sEventMgr.AddEvent(static_cast< Object* >(_unit), &Object::PlaySoundToSet, (uint32)9332, EVENT_UNK, 41000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}

	void EventRed()
	{
		GameObject* DoorLeft = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10917.1445f, -1774.05f, 90.478f, 184279);
		GameObject* DoorRight = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10872.195f, -1779.42f, 90.45f, 184278);
		GameObject* Curtain = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10894.17f, -1774.218f, 90.477f, 183932);
		GameObject* House = _unit->GetMapMgr()->GetInterface()->SpawnGameObject(183493, -10883.0f, -1751.81f, 90.4765f, -1.72788f, false, 0, 0);
		GameObject* Tree = _unit->GetMapMgr()->GetInterface()->SpawnGameObject(183492, -10877.7f, -1763.18f, 90.4771f, -1.6297f, false, 0, 0);
		GameObject* Tree2 = _unit->GetMapMgr()->GetInterface()->SpawnGameObject(183492, -10906.7f, -1750.01f, 90.4765f, -1.69297f, false, 0, 0);
		GameObject* Tree3 = _unit->GetMapMgr()->GetInterface()->SpawnGameObject(183492, -10909.5f, -1761.79f, 90.4773f, -1.65806f, false, 0, 0);
		GameObject* BackDrop = _unit->GetMapMgr()->GetInterface()->SpawnGameObject(183491, -10890.9f, -1744.06f, 90.4765f, -1.67552f, false, 0, 0);

		if(DoorLeft)
			DoorLeft->SetByte(GAMEOBJECT_BYTES_1, 0, 1);

		if(DoorRight)
			DoorRight->SetByte(GAMEOBJECT_BYTES_1, 0, 1);

		if(Curtain)
			Curtain->SetByte(GAMEOBJECT_BYTES_1, 0, 0);

		// Back Right - House
		if(House)
		{
			House->SetFloatValue(GAMEOBJECT_PARENTROTATION_02, 0.760406f);
			House->SetFloatValue(GAMEOBJECT_PARENTROTATION_03, -0.649448f);
			House->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
		}

		// Front Right - Tree
		if(Tree)
		{
			Tree->SetFloatValue(GAMEOBJECT_PARENTROTATION_02, 0.748956f);
			Tree->SetFloatValue(GAMEOBJECT_PARENTROTATION_03, -0.66262f);
		}

		// Back Left - Tree 2
		if(Tree2)
		{
			Tree2->SetFloatValue(GAMEOBJECT_PARENTROTATION_02, 0.648956f);
			Tree2->SetFloatValue(GAMEOBJECT_PARENTROTATION_03, -0.66262f);
		}

		// Front Left - Tree 3
		if(Tree3)
		{
			Tree3->SetFloatValue(GAMEOBJECT_PARENTROTATION_02, 0.737277f);
			Tree3->SetFloatValue(GAMEOBJECT_PARENTROTATION_03, -0.67559f);
		}

		// Back - Back Drop
		if(BackDrop)
		{
			BackDrop->SetFloatValue(GAMEOBJECT_PARENTROTATION_02, 0.743145f);
			BackDrop->SetFloatValue(GAMEOBJECT_PARENTROTATION_03, -0.669131f);
		}

		_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID, 16616);
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(17603, -10891.582f, -1755.5177f, 90.476f, 4.61f, false, true, 0, 0);

	}

	inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
	{
		WayPoint * wp = _unit->CreateWaypointStruct();
		wp->id = id;
		wp->x = Barnes[id].x;
		wp->y = Barnes[id].y;
		wp->z = Barnes[id].z;
		wp->o = Barnes[id].o;
		wp->waittime = waittime;
		wp->flags = flags;
		wp->forwardemoteoneshot = 0;
		wp->forwardemoteid = 0;
		wp->backwardemoteoneshot = 0;
		wp->backwardemoteid = 0;
		wp->forwardskinid = 0;
		wp->backwardskinid = 0;
		return wp;
	}

	void Destroy()
	{
		delete this;
	}

protected:
	int eventRand;
};


class StageLight : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(StageLight);
	StageLight(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;
		_unit->m_noRespawn = true;
		_unit->CastSpell(_unit, 34126, true);
	}
};

// The Curator + Astral Flare
#define CN_CURATOR			15691
#define CN_ASTRALFLARE		17096

#define HATEFUL_BOLT		30383
#define EVOCATION			30254
#define C_ENRAGE			28747
#define BERSERK				26662

class CuratorAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CuratorAI);
	bool m_spellcheck[4];
	SP_AI_Spell spells[4];

	CuratorAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 2;
		for(int i = 0; i < nrspells; i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(HATEFUL_BOLT);
		spells[0].instant = true;
		spells[0].cooldown = 12;

		spells[1].info = dbcSpell.LookupEntry(BERSERK);
		spells[1].instant = true;
		spells[1].cooldown = 720;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(EVOCATION);
		spells[2].instant = false;
		spells[2].cooldown = 0;
		spells[2].attackstoptimer = 19000;

		spells[3].info = dbcSpell.LookupEntry(C_ENRAGE);
		spells[3].instant = true;
		spells[3].cooldown = 0;
		spells[3].attackstoptimer = 1000;
	}

	void OnCombatStart(Unit* mTarget)
	{
		for(int i = 0; i < nrspells; i++)
			spells[i].casttime = spells[i].cooldown;

		evocation = false;
		enrage = false;
		uint32 t = (uint32)time(NULL);
		Timer = t + 10;
		_unit->PlaySoundToSet(9183);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The Menagerie is for guests only.");
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		_unit->PlaySoundToSet(9184);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "This Curator is no longer op... er... ation... al.");
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit * mTarget)
	{
		if(_unit->GetHealthPct() > 0)
		{
			uint32 sound = 0;
			char *text = 0;
			switch(RandomUInt(1))
			{
			case 0:
				sound = 9187;
				text = "Do not touch the displays.";
				break;
			case 1:
				sound = 9308;
				text = "You are not a guest.";
				break;
			}
			_unit->PlaySoundToSet(sound);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, text);
		}
	}

	void AIUpdate()
	{
		if(!evocation)
		{
			if(_unit->GetManaPct() <= 10)
			{
				_unit->Root();
				_unit->setAttackTimer(spells[1].attackstoptimer, false);
				_unit->PlaySoundToSet(9186);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your request cannot be processed.");
				_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
				evocation = true;
			}
			else if(!enrage && _unit->GetHealthPct() <= 16)
			{
				_unit->CastSpell(_unit, spells[3].info, spells[3].instant);
				_unit->PlaySoundToSet(9185);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Failure to comply will result in offensive action.");
				enrage = true;
			}
			else
				Trigger();
		}
		else if(_unit->GetManaPct() > 94)
		{
			_unit->Unroot();
			evocation = false;
		}
	}

	void Trigger()
	{
		uint32 t = (uint32)time(NULL);
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget() && t > spells[0].casttime)
		{
			Unit *target = _unit->GetAIInterface()->GetSecondHated();
			_unit->CastSpell(target, spells[0].info, spells[0].instant);
			target = NULL;
			spells[0].casttime = t + spells[0].cooldown;
		}
		else if(_unit->GetAIInterface()->GetNextTarget() && !enrage && !evocation && t > Timer && 
			_unit->GetUInt32Value(UNIT_FIELD_MAXPOWER1) != 0)
		{
			AstralSpawn();
			Timer = t + 10;
		}
	}

	void AstralSpawn()
	{
		std::vector<Player*> Target_List;
		for(set<Player*>::iterator itr = _unit->GetInRangePlayerSetBegin(); 
			itr != _unit->GetInRangePlayerSetEnd(); ++itr)
		{
			Player *RandomTarget = NULL;
			RandomTarget = static_cast< Player* >(*itr);
			if(RandomTarget && RandomTarget->isAlive() && isHostile(_unit, (*itr)))
				Target_List.push_back(RandomTarget);
			RandomTarget = NULL;
		}
		if(!Target_List.size())
			return;

		size_t Target = rand()%Target_List.size();
		Unit *RTarget = Target_List[Target];
		if(!RTarget)
			return;

		uint32 sound = 0;
		char *text = 0;
		switch(RandomUInt(1))
		{
		case 0:
			sound = 9188;
			text = "Gallery rules will be strictly enforced.";
			break;
		case 1:
			sound = 9309;
			text = "This curator is equipped for gallery protection.";
			break;
		}
		_unit->PlaySoundToSet(sound);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, text);

		_unit->SetUInt32Value(UNIT_FIELD_POWER1, _unit->GetUInt32Value(UNIT_FIELD_POWER1) - (_unit->GetUInt32Value(UNIT_FIELD_MAXPOWER1) / 10));
		float dX = _unit->GetPositionX();
		float dY = _unit->GetPositionY();
		Creature *AstralFlare = NULL;
		switch(RandomUInt(3))
		{
		case 0:
			{
				AstralFlare = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ASTRALFLARE, dX+3, 
					dY+3, _unit->GetPositionZ(), 0, true, false, 0, 0);
				AstralFlare->GetAIInterface()->AttackReaction(RTarget, 1, 0);
				AstralFlare = NULL;
			}break;
		case 1:
			{
				AstralFlare = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ASTRALFLARE, dX+3, 
					dY-3, _unit->GetPositionZ(), 0, true, false, 0, 0);
				AstralFlare->GetAIInterface()->AttackReaction(RTarget, 1, 0);
				AstralFlare = NULL;
			}break;
		case 2:
			{
				AstralFlare = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ASTRALFLARE, dX-3, 
					dY-3, _unit->GetPositionZ(), 0, true, false, 0, 0);
				AstralFlare->GetAIInterface()->AttackReaction(RTarget, 1, 0);
				AstralFlare = NULL;
			}break;
		case 3:
			{
				AstralFlare = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ASTRALFLARE, dX-3, 
					dY+3, _unit->GetPositionZ(), 0, true, false, 0, 0);
				AstralFlare->GetAIInterface()->AttackReaction(RTarget, 1, 0);
				AstralFlare = NULL;
			}break;
		}
		Target_List.clear();
	}

protected:
	bool evocation;
	bool enrage;
	bool berserk;
	int nrspells;
	uint32 Timer;
};

// Astral Flare
#define ASTRAL_FLARE_PASSIVE	30234
#define ASTRAL_FLARE_VISUAL		30237
#define ARCING_SEAR				30235

class AstralFlareAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(AstralFlareAI);
	bool m_spellcheck[2];
	SP_AI_Spell spells[2];

	AstralFlareAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(ASTRAL_FLARE_PASSIVE);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = 3;
		spells[0].perctrigger = 100.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(ASTRAL_FLARE_VISUAL);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = 6;
		spells[1].perctrigger = 100.0f;
		spells[1].attackstoptimer = 500;

	}

	void OnCombatStart(Unit* mTarget)
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;

		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
		_unit->Despawn(2500, 0);
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
		_unit->Despawn(2500, 0);
	}

	void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;

				if (m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}

					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

protected:
	int nrspells;
};

// Shade of Aran
#define SHADEOFARAN			16524

#define FROSTBOLT			29954
#define FIREBALL			29953
#define ARCMISSLE			29955
#define CHAINSOFICE			29991
#define DRAGONSBREATH		29964
#define AOE_COUNTERSPELL	29961

#define BLIZZARD			29952
#define FLAME_WREATH		29946 // detonate -> 30004

#define BLINK_CENTER		29967
#define MAGNETIC_PULL		38540
#define MASSSLOW			30035
#define AEXPLOSION			29973

#define MASS_POLYMORPH		29963
#define CONJURE				29975
#define DRINK				30024
#define AOE_PYROBLAST		29978

#define SUMMON_ELEMENTAL_1	37051
#define SUMMON_ELEMENTAL_2	37052
#define SUMMON_ELEMENTAL_3	37053
#define WATERELE			17167
#define WATERBOLT			31012

#define SHADOWOFARAN		18254
#define SHADOWPYRO			29978

enum SUPERSPELL
{
	SUPER_FLAME = 0,
	SUPER_BLIZZARD = 1, 
	SUPER_AOE = 2,
};

class ShadeofAranAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ShadeofAranAI);
	bool m_spellcheck[6];
	SP_AI_Spell spells[6];

	ShadeofAranAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 6;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(FROSTBOLT);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = false;
		spells[0].cooldown = 5;
		spells[0].casttime = 5;
		spells[0].attackstoptimer = 2000;

		spells[1].info = dbcSpell.LookupEntry(FIREBALL);
		spells[1].targettype = TARGET_RANDOM_SINGLE;
		spells[1].instant = false;
		spells[1].cooldown = 5;
		spells[1].casttime = 5;
		spells[1].attackstoptimer = 2000;

		spells[2].info = dbcSpell.LookupEntry(ARCMISSLE);
		spells[2].targettype = TARGET_RANDOM_SINGLE;
		spells[2].instant = false;
		spells[2].cooldown = 10;
		spells[2].casttime = 10;
		spells[2].attackstoptimer = 6000;

		spells[3].info = dbcSpell.LookupEntry(CHAINSOFICE);
		spells[3].targettype = TARGET_RANDOM_SINGLE;
		spells[3].instant = true;
		spells[3].cooldown = RandomUInt(5)+14;
		spells[3].casttime = RandomUInt(5)+14;
		spells[3].attackstoptimer = 1000;

		spells[4].info = dbcSpell.LookupEntry(DRAGONSBREATH);
		spells[4].targettype = TARGET_RANDOM_SINGLE;
		spells[4].instant = true;
		spells[4].cooldown = RandomUInt(5)+16;
		spells[4].casttime = RandomUInt(5)+16;
		spells[4].attackstoptimer = 1000;
		spells[4].maxdist2cast = 15;

		spells[5].info = dbcSpell.LookupEntry(AOE_COUNTERSPELL);
		spells[5].targettype = TARGET_SELF;
		spells[5].instant = true;
		spells[5].cooldown = 13;
		spells[5].casttime = 13;
		spells[5].attackstoptimer = 1000;

		info_flame_wreath = dbcSpell.LookupEntry(FLAME_WREATH);
		info_blink_center = dbcSpell.LookupEntry(BLINK_CENTER);
		info_massslow = dbcSpell.LookupEntry(MASSSLOW);
		info_magnetic_pull = dbcSpell.LookupEntry(MAGNETIC_PULL);
		info_aexplosion = dbcSpell.LookupEntry(AEXPLOSION);
		info_blizzard = dbcSpell.LookupEntry(BLIZZARD);
		info_summon_elemental_1 = dbcSpell.LookupEntry(SUMMON_ELEMENTAL_1);
		info_summon_elemental_2 = dbcSpell.LookupEntry(SUMMON_ELEMENTAL_2);
		info_summon_elemental_3 = dbcSpell.LookupEntry(SUMMON_ELEMENTAL_3);
		info_mass_polymorph = dbcSpell.LookupEntry(MASS_POLYMORPH);
		info_conjure = dbcSpell.LookupEntry(CONJURE);
		info_drink = dbcSpell.LookupEntry(DRINK);
		info_pyroblast = dbcSpell.LookupEntry(AOE_PYROBLAST);
	}

	void OnCombatStart(Unit* mTarget)
	{
		//Atiesh check
		bool HasAtiesh = false;
		if(mTarget->GetTypeId() == TYPEID_PLAYER)
		{
			for(set<Player*>::iterator itr = _unit->GetInRangePlayerSetBegin(); itr != _unit->GetInRangePlayerSetEnd(); ++itr)
			{
				if(*itr)
				{
					if((*itr)->GetItemInterface()->GetItemCount(22589) > 0 ||
						(*itr)->GetItemInterface()->GetItemCount(22630) > 0 || 
						(*itr)->GetItemInterface()->GetItemCount(22631) > 0 || 
						(*itr)->GetItemInterface()->GetItemCount(22632) > 0)
					{
						HasAtiesh = true;
						break;
					}
				}
			}
		}

		if(HasAtiesh)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Where did you get that?! Did HE send you?!");
			_unit->PlaySoundToSet(9249);
		}
		else
		{
			switch(rand()%3)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Please, no more. My son... he's gone mad!");
				_unit->PlaySoundToSet(9241);
				break;

			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'll not be tortured again!");
				_unit->PlaySoundToSet(9323);
				break;

			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Who are you? What do you want? Stay away from me!");
				_unit->PlaySoundToSet(9324);
				break;
			}
		}

		_unit->GetAIInterface()->setCurrentAgent(AGENT_SPELL);
		RegisterAIUpdateEvent(1000);
		m_time_enrage = 900;
		m_time_special = (uint32) RandomUInt(5)+25;
		m_time_pyroblast = 0;
		CastTime();
		drinking = false;
		enraged = false;
		summoned = false;
		explode = false;
		slow = false;
		LastSuperSpell = RandomUInt(100)%3;
		// Door closing
		GameObject *SDoor = NULL;
		SDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-11190.012f, -1881.016f, 231.95f, 184517);
		if(SDoor)
		{
			SDoor->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
			SDoor->SetUInt32Value(GAMEOBJECT_FLAGS, 33);
		}
	}

	void OnCombatStop(Unit *mTarget)
	{
		CastTime();
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
		_unit->SetUInt32Value(UNIT_FIELD_POWER1, _unit->GetUInt32Value(UNIT_FIELD_MAXPOWER1));
		// Door opening
		GameObject *SDoor = NULL;
		SDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-11190.012f, -1881.016f, 231.95f, 184517);
		if(SDoor)
			SDoor->SetUInt32Value(GAMEOBJECT_FLAGS, 34);
	}

	void OnDied(Unit * mKiller)
	{
		CastTime();
		_unit->PlaySoundToSet(9244);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "At last... The nightmare is.. over...");
		RemoveAIUpdateEvent();
		// Door opening
		GameObject *SDoor = NULL;
		SDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-11190.012f, -1881.016f, 231.95f, 184517);
		if(SDoor)
			SDoor->SetUInt32Value(GAMEOBJECT_FLAGS, 34);
	}

	void OnTargetDied(Unit* mTarget)
	{
		switch(rand()%2)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I want this nightmare to be over!");
			_unit->PlaySoundToSet(9250);
			break;

		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Torment me no more!");
			_unit->PlaySoundToSet(9328);
			break;
		}
	}

	void AIUpdate()
	{
		if(FlameWreathTimer)
		{
			FlameWreathTimer--;
			uint32 i = 0;
			for (i = 0; i < 3; i++)
			{
				if(!FlameWreathTarget[i])
					continue;

				Unit* pTarget = _unit->GetMapMgr()->GetUnit(FlameWreathTarget[i]);
				if (pTarget && pTarget->GetDistanceSq(FWTargPosX[i], FWTargPosY[i], _unit->GetPositionZ()) > 3)
				{
					pTarget->CastSpell(pTarget->GetGUID(), 20476, true);
					FlameWreathTarget[i] = 0;
				}
			}
		}

		if(!drinking) 
		{
			if(explode)
			{
				if(slow)
				{
					_unit->CastSpell(_unit, info_massslow, true);
					slow = false;
				}
				else
				{
					_unit->CastSpell(_unit, info_aexplosion, false);
					explode = false;
				}
			}
			else if(!summoned && _unit->GetHealthPct() <= 40)
			{
				_unit->CastSpell(_unit, info_summon_elemental_1, true);
				_unit->CastSpell(_unit, info_summon_elemental_2, true);
				_unit->CastSpell(_unit, info_summon_elemental_3, true);
				_unit->PlaySoundToSet(9251);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'm not finished yet! No, I have a few more tricks up me sleeve.");
				summoned = true;
			}
			else if(_unit->GetManaPct() <= 20 && _unit->GetCurrentSpell() == NULL)
			{
				if(!m_time_pyroblast) 
				{
					_unit->WipeHateList();
					_unit->PlaySoundToSet(9248);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Surely you would not deny an old man a replenishing drink? No, no I thought not.");
					m_time_pyroblast = 10;
					_unit->CastSpell(_unit, info_mass_polymorph, true);
					_unit->setAttackTimer(2000, false);
				}
				else if(_unit->GetStandState() != 1)
				{
					_unit->setAttackTimer(3000, false);
					_unit->CastSpell(_unit, info_conjure, false);
					_unit->SetStandState(1);
					_unit->setEmoteState(EMOTE_ONESHOT_EAT);
				}
				else
					_unit->CastSpell(_unit, info_drink, true);
			}
			else
				SpellTrigger();
		}
		else if (_unit->GetCurrentSpell() == NULL)
		{
			m_time_pyroblast--;
			if(!m_time_pyroblast)
			{
				_unit->SetStandState(0);
				_unit->setEmoteState(0);
				_unit->CastSpell(_unit, info_pyroblast, false);
				drinking = false;
			}

		}
	}

	void SpellTrigger()
	{
		m_time_enrage--;
		m_time_special--;
		if(!enraged && !m_time_enrage)
		{
			_unit->PlaySoundToSet(9247);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You've wasted enough of my time. Let these games be finished!");
			float ERX = 5*cos(RandomFloat(6.28f))+(_unit->GetPositionX());
			float ERY = 5*sin(RandomFloat(6.28f))+(_unit->GetPositionY());
			float ERZ = _unit->GetPositionZ();
			uint32 i = 0;
			for(i = 0; i < 4; i++)
			{
				_unit->GetMapMgr()->GetInterface()->SpawnCreature(SHADOWOFARAN, ERX, ERY, ERZ, 0, true, false, 0, 0);
			}
			ERX = 0;
			ERY = 0;
			ERZ = 0;
			enraged = true;
			return;
		}
		else if(!m_time_special)
		{
			CastSpecial();
			m_time_special = (uint32) RandomUInt(5)+25;
			return;
		}
		else 
		{
			float val = (float)RandomFloat(100.0f);
			SpellCast(val);
		}
	}

	void FlameWreath()
	{
		switch(rand()%2)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'll show you this beaten dog still has some teeth!");
			_unit->PlaySoundToSet(9245);
			break;

		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Burn you hellish fiends!");
			_unit->PlaySoundToSet(9326);
			break;
		}

		FlameWreathTimer = 20;
		FlameWreathTarget[0] = 0;
		FlameWreathTarget[1] = 0;
		FlameWreathTarget[2] = 0;

		std::vector<Player*> Targets;
		std::set<Player*>::iterator hostileItr = _unit->GetInRangePlayerSetBegin();
		for(; hostileItr != _unit->GetInRangePlayerSetEnd(); ++hostileItr) 
		{ 
			Player *RandomTarget = NULL;
			RandomTarget = static_cast< Player* >(*hostileItr);

			if (RandomTarget && RandomTarget->isAlive() && _unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0)
				Targets.push_back(RandomTarget);
		}

		while(Targets.size() > 3)
			Targets.erase(Targets.begin()+rand()%Targets.size());

		uint32 i = 0;
		for (std::vector<Player*>::iterator itr = Targets.begin(); itr != Targets.end(); ++itr)
		{
			if(*itr)
			{
				FlameWreathTarget[i] = (*itr)->GetGUID();
				FWTargPosX[i] = (*itr)->GetPositionX();
				FWTargPosY[i] = (*itr)->GetPositionY();
				_unit->CastSpell((*itr), FLAME_WREATH, true);
			}
		}

	}

	void Blizzard()
	{
		switch(rand()%2)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'll freeze you all!");
			_unit->PlaySoundToSet(9246);
			break;

		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Back to the cold dark with you!");
			_unit->PlaySoundToSet(9327);
			break;
		}

		_unit->CastSpell(_unit, info_blizzard, true);
	}

	void AoEExplosion()
	{
		switch(rand()%2)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Yes, yes, my son is quite powerful... but I have powers of my own!");
			_unit->PlaySoundToSet(9242);
			break;

		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I am not some simple jester! I am Nielas Aran!");
			_unit->PlaySoundToSet(9325);
			break;
		}

		_unit->CastSpell(_unit, info_blink_center, true);
		_unit->CastSpell(_unit, info_magnetic_pull, true);
		explode = true;
		slow = true;
	}

	void CastSpecial()
	{
		int Available[2];

		switch (LastSuperSpell)
		{
		case SUPER_BLIZZARD:
			Available[0] = SUPER_FLAME;
			Available[1] = SUPER_AOE;
			break;

		case SUPER_FLAME:
			Available[0] = SUPER_BLIZZARD;
			Available[1] = SUPER_AOE;
			break;

		case SUPER_AOE:
			Available[0] = SUPER_BLIZZARD;
			Available[1] = SUPER_FLAME;
			break;
		}

		LastSuperSpell = Available[RandomUInt(100)%2];

		switch (LastSuperSpell)
		{
		case SUPER_BLIZZARD:
			Blizzard();
			break;

		case SUPER_FLAME:
			FlameWreath();
			break;

		case SUPER_AOE:
			AoEExplosion();
			break;
		}
	}

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;

				if (m_spellcheck[i])
				{
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					case TARGET_RANDOM_SINGLE:
					case TARGET_RANDOM_DESTINATION:
						CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			std::vector<Player *> TargetTable;
			for(set<Player*>::iterator itr = _unit->GetInRangePlayerSetBegin(); itr != _unit->GetInRangePlayerSetEnd(); ++itr) 
			{ 
				Player* RandomTarget = NULL;
				RandomTarget = static_cast< Player* >(*itr);

				if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget)))
					TargetTable.push_back(RandomTarget);
			}

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit * RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}

protected:
	bool drinking;
	bool enraged;
	bool summoned;
	bool explode;
	bool slow;
	float FWTargPosX[3];
	float FWTargPosY[3];
	int nrspells;
	int LastSuperSpell;
	uint32 m_time_enrage;
	uint32 m_time_special;
	uint32 m_time_pyroblast;
	uint32 m_time_conjure;
	uint32 FlameWreathTimer;
	uint64 FlameWreathTarget[3];
	SpellEntry *info_flame_wreath;
	SpellEntry *info_aexplosion;
	SpellEntry *info_blizzard; 
	SpellEntry *info_magnetic_pull;
	SpellEntry *info_blink_center;
	SpellEntry *info_massslow;
	SpellEntry *info_mass_polymorph;
	SpellEntry *info_conjure;
	SpellEntry *info_drink;
	SpellEntry *info_pyroblast;
	SpellEntry *info_summon_elemental_1;
	SpellEntry *info_summon_elemental_2;
	SpellEntry *info_summon_elemental_3;
};

class WaterEleAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(WaterEleAI);

	WaterEleAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
	}

	void OnCombatStart(Unit *mTarget)
	{
		WaterBolt = (RandomUInt(3)+5);
		RegisterAIUpdateEvent(1250);
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
		_unit->Despawn(20000, 0);
	}

	void AIUpdate()
	{
		WaterBolt--;
		if(!WaterBolt)
		{
			_unit->setAttackTimer(2000, false);
			Unit *target = _unit->GetAIInterface()->GetNextTarget();
			if(target)
				_unit->CastSpell(target->GetGUID(), WATERBOLT, true);
		}
	}

	void Destroy()
	{
		delete this;
	}

protected:
	int WaterBolt;
};

class ShadowofAranAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ShadowofAranAI);

	ShadowofAranAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
	}

	void OnCombatStart(Unit *mTarget)
	{
		ShadowPyro = (RandomUInt(2)+4);
		RegisterAIUpdateEvent(1250);
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		_unit->Despawn(10000, 0);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
		_unit->Despawn(5000, 0);
	}

	void AIUpdate()
	{
		ShadowPyro--;
		if(!ShadowPyro)
		{
			Unit *target = _unit->GetAIInterface()->GetNextTarget();
			_unit->CastSpell(target->GetGUID(), SHADOWPYRO, true);
		}
	}

	void Destroy()
	{
		delete this;
	}

protected:
	int ShadowPyro;
};

// Terestian Illhoof
#define CN_ILLHOOF					15688

#define SHADOW_BOLT					19729
// #define S_DEMONCHAINS				30120
#define S_KILREK						30066
// #define F_PORTAL1					30179
// #define F_PORTAL2					30171
#define I_ENRAGE					32964
#define SACRIFICE					30115

// Kil'Rek
#define CN_KILREK					17229
#define AMPLIFY_FLAMES				30053
#define BROKEN_PACT					30065

// Imps
#define CN_FIENDISH_IMP				17267
#define FIREBALL_IMP				31620

// Demon Chains
#define	CN_DEMONCHAINS				17248
#define	CHAINS_VISUAL				30206

#define CN_FPORTAL					17265

class IllhoofAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(IllhoofAI);
	bool m_spellcheck[2];
	SP_AI_Spell spells[2];

	IllhoofAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 2;
		for(int i = 0; i < nrspells; i++)
			m_spellcheck[i] = false;

		spells[0].info = dbcSpell.LookupEntry(SHADOW_BOLT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 5;
		spells[0].perctrigger = 50.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(I_ENRAGE);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].cooldown = 600;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;
		/*
		spells[2].info = dbcSpell.LookupEntry(S_DEMONCHAINS);
		spells[2].targettype = TARGET_VARIOUS;
		spells[2].instant = true;
		spells[2].cooldown = 45;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;
		*/
	}

	void OnCombatStart(Unit* mTarget)
	{
		uint32 t = (uint32)time(NULL);
		for(int i = 0; i < nrspells; i++)
			spells[i].casttime = spells[i].cooldown;

		DemonChain = t + 45;
		ImpTimer = t + 10;
		ReSummon = false;

		_unit->PlaySoundToSet(9260);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ah, you're just in time. The rituals are about to begin.");
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		clean();
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		clean();
		_unit->PlaySoundToSet(9262);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "My life, is yours. Oh great one.");
		RemoveAIUpdateEvent();
	}

	void clean()
	{
		Creature* portal = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-11249.51f, -1702.182f, 179.237f, CN_FPORTAL);
		Creature* portal2 = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-11239.534f, -1715.338f, 179.237f, CN_FPORTAL);

		if(portal != NULL && portal2 != NULL)
		{
			portal->SafeDelete();
			portal2->SafeDelete();
		}
	}

	void OnTargetDied(Unit* mTarget)
	{
		uint32 sound = 0;
		char *text = 0;
		switch(RandomUInt(1))
		{
		case 0:
			sound = 9264;
			text = "Your blood will anoint my circle.";
			break;
		case 1:
			sound = 9329;
			text = "The great one will be pleased.";
			break;
		}
		_unit->PlaySoundToSet(sound);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, text);
	}

	void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);

		uint32 t = (uint32)time(NULL);
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			if(t > ImpTimer)
			{
				spawnSummoningPortals();
				ImpTimer = -1;
			}
			if(t > DemonChain)
			{
				DemonChain = t + 45;
				PlrSacrifice();
			}
			if(_unit->HasAura(BROKEN_PACT) && !ReSummon)
			{
				ReSummon = true;
				ReKilrek = t + 45;
			}
			else if(ReSummon && t > ReKilrek)
			{
				ReSummon = false;
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(S_KILREK), true);
				_unit->RemoveAura(BROKEN_PACT);
			}
		}
	}

	void spawnSummoningPortals()
	{
		switch(rand()%2)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Come, you dwellers in the dark. Rally to my call!");
			_unit->PlaySoundToSet(9265);
			break;

		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Gather, my pets. There is plenty for all.");
			_unit->PlaySoundToSet(9331);
			break;
		}

		_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FPORTAL, -11249.51f, -1702.182f, 179.237f, 0, false, false, 0, 0);
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FPORTAL, -11239.534f, -1715.338f, 179.237f, 0, false, false, 0, 0);
	}

	void PlrSacrifice()
	{
		switch(rand()%2)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Please, accept this humble offering, oh great one.");
			_unit->PlaySoundToSet(9263);
			break;

		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Let the sacrifice serve his testament to my fealty.");
			_unit->PlaySoundToSet(9330);
			break;
		}

		std::vector<Player *> TargetTable;
		std::set<Player*>::iterator itr = _unit->GetInRangePlayerSetBegin();
		std::advance(itr, 1);
		for(; itr != _unit->GetInRangePlayerSetEnd(); ++itr) 
		{
			if(isHostile(_unit, (*itr)) && (*itr)->GetInstanceID() == _unit->GetInstanceID())
			{
				Player* RandomTarget = NULL;
				RandomTarget = static_cast< Player* >(*itr);
				if (RandomTarget && RandomTarget->isAlive() && isHostile(_unit, RandomTarget))
					TargetTable.push_back(RandomTarget);
				RandomTarget = NULL;
			}
		}
		if (!TargetTable.size())
			return;

		size_t RandTarget = rand()%TargetTable.size();
		Unit * RTarget = TargetTable[RandTarget];

		if (!RTarget) return;

		_unit->CastSpell(RTarget, dbcSpell.LookupEntry(SACRIFICE), false);

		TargetTable.clear();

		float dcX = -11234.7f;
		float dcY = -1698.73f;
		float dcZ = 179.24f;
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_DEMONCHAINS, dcX, dcY, dcZ, 0, true, false, 0, 0);
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i = 0; i < nrspells; i++)
			{
				if(!spells[i].perctrigger) continue;

				if (m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}

					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

protected:

	int nrspells;
	bool ReSummon;
	uint32 ImpTimer;
	uint32 ReKilrek;
	uint32 DemonChain;
};

// Kil'Rek
class KilrekAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(KilrekAI);
	bool m_spellcheck[2];
	SP_AI_Spell spells[2];

	KilrekAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(AMPLIFY_FLAMES);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].cooldown = 5;
		spells[0].perctrigger = 50.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(BROKEN_PACT);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
	}

	void OnCombatStart(Unit* mTarget)
	{
		spells[0].casttime = (uint32)time(NULL) + spells[0].cooldown;

		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();

		Unit *Illhoof = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 15688);
		if(Illhoof != NULL && Illhoof->isAlive())
			Illhoof->CastSpell(Illhoof, spells[1].info, spells[1].instant);
	}

	void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;

				if (m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					case TARGET_RANDOM_FRIEND:
					case TARGET_RANDOM_SINGLE:
					case TARGET_RANDOM_DESTINATION:
						CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}

					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			std::vector<Unit *> TargetTable;
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if((*itr) != _unit && isHostile(_unit, (*itr)) && (*itr)->GetInstanceID() == _unit->GetInstanceID() && ((*itr)->GetTypeId() == TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER))
				{
					Unit* RandomTarget = NULL;
					RandomTarget = static_cast< Unit* >(*itr);

					if (RandomTarget && RandomTarget->isAlive() && _unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0)
						TargetTable.push_back(RandomTarget);
				}
			}

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit * RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}

protected:
	int nrspells;
};

// Fiendish Imp
class FiendishImpAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(FiendishImpAI);
	SP_AI_Spell spells[1]; 
	bool m_spellcheck[1]; 

	FiendishImpAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 1; 
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(FIREBALL_IMP);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].cooldown = 0;
		spells[0].perctrigger = 100.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].casttime = 0;

		Unit *target = NULL;
		target = FindTargetForSpell();
		if (target != NULL)
		{
			_unit->GetAIInterface()->AttackReaction(target, 2500, 0);
		}
	}

	void OnCombatStart(Unit* mTarget)
	{
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(true);

		if (_unit->GetDistance2dSq(mTarget) <= 1225.0f)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_SPELL);
		}

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();

		_unit->SafeDelete();
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
		_unit->SafeDelete();
	}

	void AIUpdate()
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		if (_unit->GetAIInterface()->GetNextTarget() && _unit->GetDistance2dSq(_unit->GetAIInterface()->GetNextTarget()) <= 1225.0f)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_SPELL);
			if (_unit->GetCurrentSpell() == NULL && RandomUInt(10) > 2)
			{
				_unit->setAttackTimer(spells[2].attackstoptimer, false);

				Unit *target = NULL;
				target = _unit->GetAIInterface()->GetNextTarget();

				_unit->CastSpell(target, spells[0].info, spells[0].instant);
				return;
			}
		}

		float val = RandomFloat(100.0f);
		SpellCast(val);
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;

				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					if (!spells[i].instant)
						_unit->GetAIInterface()->StopMovement(1);

					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

	Unit* FindTargetForSpell()
	{
		Unit* target = NULL;
		float distance = 90.0f;

		Unit *pUnit;
		float dist;

		for (std::set<Object*>::iterator itr = _unit->GetInRangeOppFactsSetBegin(); itr != _unit->GetInRangeOppFactsSetEnd(); itr++)
		{
			if((*itr)->GetTypeId() != TYPEID_UNIT && (*itr)->GetTypeId() != TYPEID_PLAYER)
				continue;

			pUnit = static_cast<Unit*>((*itr));

			if(pUnit->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH))
				continue;

			if(pUnit->m_invisible)
				continue;

			if(!pUnit->isAlive() || _unit == pUnit)
				continue;

			dist = _unit->GetDistance2dSq(pUnit);

			if(dist > distance*distance)
				continue;

			target = pUnit;
			break;
		}

		return target;
	}

protected:	

	int nrspells;
};

class DemonChains : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(DemonChains);
	DemonChains(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(CHAINS_VISUAL), true);
		_unit->Root();
		_unit->DisableAI();
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		if(_unit->GetHealthPct() > 0)
			_unit->Despawn(10000, 0);
	}

	void OnDied(Unit * mKiller)
	{
		Unit *uIllhoof = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), 
			_unit->GetPositionZ(), CN_ILLHOOF);
		if(uIllhoof != NULL && uIllhoof->isAlive())
			uIllhoof->RemoveAura(SACRIFICE);

		_unit->Despawn(10000, 0);
	}
};

class FiendPortal : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(FiendPortal);
	FiendPortal(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->Root();

		_unit->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;
		_unit->m_noRespawn = true;

		RegisterAIUpdateEvent(10000);
	}

	void AIUpdate()
	{
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FIENDISH_IMP, _unit->GetPositionX(), _unit->GetPositionY(), 
			_unit->GetPositionZ(), 0, true, false, 0, 0);

	}
};

// Prince Malchezaar
#define CN_MALCHEZAAR			15690
#define CN_INFERNAL				17646
#define CN_AXES					17650
#define CN_DUMMY				17644		

#define ENFEEBLE				30843
#define SHADOWNOVA				30852
#define SW_PAIN					30854
#define THRASH_AURA				12787
#define SUNDER_ARMOR			25225
#define AMPLIFY_DMG				39095 // old 12738
#define SUMMON_AXES				30891
#define WIELD_AXES				30857

// Extra creature info
#define INF_RAIN				33814
#define HELLFIRE				39131
#define DEMONIC_FRENZY			32851

// Item model info
#define AXE_ITEM_MODEL			40066
#define AXE_ITEM_INFO			33488898
#define AXE_ITEM_SLOT			768
/* Emotes:
SPECIAL? - 9223 - 9320
AXETOSS2? - 9317
*/

class MalchezaarAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(MalchezaarAI);
	bool m_spellcheck[9];
	SP_AI_Spell spells[9];

	MalchezaarAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		m_phase = 1;
		nrspells = 5;

		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(SW_PAIN);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 15;	
		spells[0].perctrigger = 50.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(ENFEEBLE);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = 25;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(INF_RAIN);
		spells[2].targettype = TARGET_DESTINATION;
		spells[2].instant = true;
		spells[2].cooldown = 43;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(SUNDER_ARMOR);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = true;
		spells[3].cooldown = 15;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = dbcSpell.LookupEntry(AMPLIFY_DMG);
		spells[4].targettype = TARGET_RANDOM_SINGLE;
		spells[4].instant = true;
		spells[4].cooldown = 20;
		spells[4].perctrigger = 0.0f;
		spells[4].attackstoptimer = 1000;
		spells[4].mindist2cast = 0.0f;

		spells[5].info = dbcSpell.LookupEntry(SHADOWNOVA);
		spells[5].targettype = TARGET_VARIOUS;
		spells[5].instant = false;
		spells[5].cooldown = 4;	
		spells[5].perctrigger = 0.0f;
		spells[5].attackstoptimer = 2000;

		spells[6].info = dbcSpell.LookupEntry(THRASH_AURA);
		spells[6].targettype = TARGET_SELF;
		spells[6].instant = true;
		spells[6].cooldown = 1;
		spells[6].perctrigger = 0.0f;
		spells[6].attackstoptimer = 1000;

		spells[7].info = dbcSpell.LookupEntry(WIELD_AXES);
		spells[7].targettype = TARGET_SELF;
		spells[7].instant = true;
		spells[7].attackstoptimer = 1000;

		spells[8].info = dbcSpell.LookupEntry(SUMMON_AXES);
		spells[8].targettype = TARGET_SELF;
		spells[8].instant = true;
		spells[8].attackstoptimer = 1000;

		// Dummy initialization
		float dumX = -10938.56f;
		float dumY = -2041.26f;
		float dumZ = 305.132f;
		InfernalDummy = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_DUMMY, dumX, dumY, dumZ, 0, true, false, 0, 0);

		// Door initialization
		MDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-11018.5f, -1967.92f, 276.652f, 185134);
	}

	void OnCombatStart(Unit * mTarget)
	{
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;

		uint32 t = (uint32)time(NULL);
		spells[0].casttime = t + spells[0].cooldown;
		spells[1].casttime = t + spells[1].cooldown;
		spells[2].casttime = t + spells[2].cooldown;

		m_spawn_infernal = 0;
		m_infernal = false;

		_unit->PlaySoundToSet(9218);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Madness has brought you here to me. I shall be your undoing.");
		RegisterAIUpdateEvent(1000);

		if(MDoor != NULL)
		{
			MDoor->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
			MDoor->SetUInt32Value(GAMEOBJECT_FLAGS, 33);
		}
	}

	void OnCombatStop(Unit * mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();

		// Reset weapon
		_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, 0);
		
		// Off hand weapon
		_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID+1, 0);
		

		CreatureProto * cp = CreatureProtoStorage.LookupEntry(CN_MALCHEZAAR);
		if(!cp)
			return;

		_unit->SetFloatValue(UNIT_FIELD_MINDAMAGE, cp->MinDamage);
		_unit->SetFloatValue(UNIT_FIELD_MAXDAMAGE, cp->MaxDamage);

		for(int i = 0; i < 5; ++i)
			Enfeeble_Targets[i] = 0;

		InfernalDummy->Despawn(10000, 0);

		// Open door
		if(MDoor != NULL)
			MDoor->SetByte(GAMEOBJECT_BYTES_1, 0, 0);

		Creature *MAxes = NULL;
		MAxes = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), CN_AXES);
		if(MAxes != NULL)
			MAxes->Despawn(1000, 0);		
	}

	void OnDied(Unit * mKiller)
	{
		_unit->PlaySoundToSet(9221);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I refuse to concede defeat. I am a prince of the Eredar! I am...");
		RemoveAIUpdateEvent();

		if(InfernalDummy)
			InfernalDummy->Despawn(10000, 0);

		Creature *MAxes = NULL;
		MAxes = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), 
			_unit->GetPositionZ(), CN_AXES);
		if(MAxes)
			MAxes->Despawn(1000, 0);

		// Open door
		if(MDoor)
			MDoor->SetByte(GAMEOBJECT_BYTES_1, 0, 0);
	}

	void OnTargetDied(Unit * mTarget)
	{
		switch(rand()%3)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You are, but a plaything, unfit even to amuse.");
			_unit->PlaySoundToSet(9319);
			break;

		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your greed, your foolishness has brought you to this end.");
			_unit->PlaySoundToSet(9318);
			break;

		case 2:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Surely you did not think you could win.");
			_unit->PlaySoundToSet(9222);
			break;
		}
	}

	void AIUpdate()
	{	
		switch(m_phase)
		{
		case 1:
			if(_unit->GetHealthPct() <= 60)
				PhaseTwo();
			break;
		case 2:
			PhaseTwo();
			if(_unit->GetHealthPct() <= 30)
				PhaseThree();
			break;
		case 3:
			PhaseThree();
			break;
		default:
			m_phase = 1;
			break;
		}
		uint32 t = (uint32)time(NULL);
		if(t > spells[1].casttime && _unit->GetAIInterface()->GetNextTarget() && _unit->GetCurrentSpell() == NULL)
		{
			Enfeebler();
			spells[1].casttime = t + spells[1].cooldown;
			spells[5].casttime = t + spells[5].cooldown;
		}
		else if(t > m_spawn_infernal && m_infernal == true && _unit->GetAIInterface()->GetNextTarget())
		{
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_INFERNAL, ranX, ranY, 276.0f, 0, true, false, 0, 0);
			m_spawn_infernal = 0;
			m_infernal = false;
		}
		else if(t > spells[5].casttime && _unit->GetAIInterface()->GetNextTarget() && _unit->GetCurrentSpell() == NULL)
		{
			spells[5].casttime = -1;
			_unit->CastSpell(_unit, spells[5].info, spells[5].instant);
			m_enfeebleoff = t + 3;
		}
		else if(t > m_enfeebleoff)
		{
			EnfeebleOff();
			m_enfeebleoff = -1;
		}
		else if(t > spells[2].casttime)
		{
			SummonInfernal();
			if(m_phase==3)
				spells[2].casttime = t + 20;
			else
				spells[2].casttime = t + spells[2].cooldown;
		}
		else
		{
			float val = (float)RandomFloat(100.0f);
			SpellCast(val);
		}
	}

	void PhaseTwo()
	{
		if(_unit->GetHealthPct() <= 60 && m_phase == 1)
		{
			uint32 t = (uint32)time(NULL);
			spells[0].casttime = -1;
			spells[3].casttime = t + spells[3].cooldown;
			spells[3].perctrigger = 50.0f;

			_unit->PlaySoundToSet(9220);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Time is the fire in which you'll burn!");

			_unit->CastSpell(_unit, spells[6].info, spells[6].instant);
			_unit->CastSpell(_unit, spells[7].info, spells[6].instant);

			// Main hand weapon
			_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, AXE_ITEM_MODEL);
			
			// Off hand weapon
			_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID+1, AXE_ITEM_MODEL);
			
			CreatureProto * cp = CreatureProtoStorage.LookupEntry(CN_MALCHEZAAR);
			if(!cp)
				return;

			_unit->SetFloatValue(UNIT_FIELD_MINDAMAGE, 1.5f*cp->MinDamage);
			_unit->SetFloatValue(UNIT_FIELD_MAXDAMAGE, 1.5f*cp->MaxDamage);

			m_phase = 2;
		}
		else
		{
			float val = (float)RandomFloat(100.0f);
			SpellCast(val);
		}
	}

	void PhaseThree()
	{
		if(_unit->GetHealthPct() <= 30 && m_phase == 2)
		{
			uint32 t = (uint32)time(NULL);

			spells[0].targettype = TARGET_RANDOM_SINGLE;
			spells[0].casttime = t + spells[0].cooldown;

			spells[1].casttime = -1;
			spells[1].perctrigger = 0.0f;

			spells[4].casttime = t + spells[4].cooldown;
			spells[4].perctrigger = 50.0f;

			_unit->CastSpell(_unit, spells[8].info, spells[8].instant);

			_unit->RemoveAura(THRASH_AURA);
			_unit->RemoveAura(WIELD_AXES);

			_unit->PlaySoundToSet(9321);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "How can you hope to withstand against such overwhelming power?");

			// Main hand weapon
			_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, 0);
			
			// Off hand weapon
			_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID+1, 0);
			

			CreatureProto * cp = CreatureProtoStorage.LookupEntry(CN_MALCHEZAAR);
			if(!cp)
				return;

			_unit->SetFloatValue(UNIT_FIELD_MINDAMAGE, cp->MinDamage);
			_unit->SetFloatValue(UNIT_FIELD_MAXDAMAGE, cp->MaxDamage);
			m_phase = 3;
		}
		else
		{
			float val = (float)RandomFloat(100.0f);
			SpellCast(val);
		}
	}

	void SummonInfernal()
	{
		switch(rand()%2)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You face not Malchezaar alone, but the legions I command!");
			_unit->PlaySoundToSet(9322);
			break;

		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "All realities, all dimensions are open to me!");
			_unit->PlaySoundToSet(9224);
			break;
		}

		ranX = RandomFloat(113.47f)-11019.37f;
		ranY = RandomFloat(36.951f)-2011.549f;
		InfernalDummy->CastSpellAoF(ranX, ranY, 275.0f, spells[2].info, spells[2].instant); // Shoots the missile
		float dist = InfernalDummy->CalcDistance(ranX, ranY, 275.0f);
		uint32 dtime = (uint32)(dist/spells[2].info->speed);
		m_spawn_infernal = (uint32)time(NULL) + dtime+1;
		m_infernal = true;
	}

	void Enfeebler()
	{
		std::vector<Player*> Targets;
		std::set<Player*>::iterator Itr = _unit->GetInRangePlayerSetBegin();
		std::advance(Itr, 1);
		for( ; Itr != _unit->GetInRangePlayerSetEnd(); ++Itr)
		{
			if(isHostile(_unit, (*Itr)) && (*Itr)->GetInstanceID() == _unit->GetInstanceID())
			{
				Player *RandomTarget = NULL;
				RandomTarget = static_cast< Player* >(*Itr);

				if(RandomTarget->isAlive() && isHostile(_unit, RandomTarget))
					Targets.push_back(RandomTarget);
			}
		}

		while(Targets.size() > 5)
			Targets.erase(Targets.begin()+rand()%Targets.size());

		int i = 0;
		for(std::vector<Player*>::iterator E_Itr = Targets.begin(); E_Itr != Targets.end(); ++E_Itr)
		{
			if(*E_Itr && (*E_Itr)->GetGUID() != _unit->GetAIInterface()->GetMostHated()->GetGUID())
			{
				Enfeeble_Targets[i] = (*E_Itr)->GetGUID();
				Enfeeble_Health[i] = (*E_Itr)->GetUInt32Value(UNIT_FIELD_HEALTH);

				_unit->CastSpell((*E_Itr), spells[1].info, spells[1].instant);
				(*E_Itr)->SetUInt32Value(UNIT_FIELD_HEALTH, 1);
			}
		}
	}

	void EnfeebleOff()
	{
		for(int i = 0; i < 5; ++i)
		{
			Unit *ETarget = _unit->GetMapMgr()->GetUnit(Enfeeble_Targets[i]);
			if(ETarget && ETarget->isAlive())
				ETarget->SetUInt64Value(UNIT_FIELD_HEALTH, Enfeeble_Health[i]);
			Enfeeble_Targets[i] = 0;
			Enfeeble_Health[i] = 0;
		}
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;

				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant);
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					case TARGET_RANDOM_FRIEND:
					case TARGET_RANDOM_SINGLE:
					case TARGET_RANDOM_DESTINATION:
						CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			std::vector<Player *> TargetTable;
			for(set<Player*>::iterator itr = _unit->GetInRangePlayerSetBegin(); itr != _unit->GetInRangePlayerSetEnd(); ++itr) 
			{ 
				Player* RandomTarget = NULL;
				RandomTarget = static_cast< Player* >(*itr);

				if (RandomTarget && RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast)
					TargetTable.push_back(RandomTarget);
			}

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit * RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}

protected:
	float ranX;
	float ranY;
	int nrspells;
	int m_phase;
	bool m_infernal;
	uint32 m_enfeebleoff;
	uint32 m_spawn_infernal;
	uint64 Enfeeble_Targets[5];
	uint64 Enfeeble_Health[5];
	Creature *InfernalDummy;
	GameObject *MDoor;
};

class NetherInfernalAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(NetherInfernalAI);
	SP_AI_Spell spells[1];

	NetherInfernalAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->Root();
		_unit->DisableAI();
		_unit->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;
		_unit->m_noRespawn = true;
		_unit->Despawn(175000, 0);
		RegisterAIUpdateEvent(6000);

		spells[0].info = dbcSpell.LookupEntry(HELLFIRE);
		spells[0].instant = false;
		spells[0].cooldown = 6;
		spells[0].casttime = (uint32)time(NULL) + spells[0].cooldown;

		_unit->CastSpell(_unit, spells[0].info, spells[0].instant);
	}

	void AIUpdate()
	{
		uint32 t = (uint32)time(NULL);
		if(t > spells[0].casttime)
		{
			_unit->CastSpell(_unit, spells[0].casttime, spells[0].instant);
			spells[0].casttime = t + spells[0].cooldown;
		}
	}

	void Destroy()
	{
		delete this;
	}
};

class InfernalDummyAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(InfernalDummyAI);
	InfernalDummyAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(1, 0, 768));
	}

	inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
	{
		WayPoint * wp = _unit->CreateWaypointStruct();
		wp->id = id;
		wp->x = -10938.56f;
		wp->y = -2041.26f;
		wp->z = 305.132f;
		wp->o = 0;
		wp->waittime = waittime;
		wp->flags = flags;
		wp->forwardemoteoneshot = 0;
		wp->forwardemoteid = 0;
		wp->backwardemoteoneshot = 0;
		wp->backwardemoteid = 0;
		wp->forwardskinid = 0;
		wp->backwardskinid = 0;
		return wp;
	}

	void Destroy()
	{
		delete this;
	}
};

class MAxesAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(MAxesAI);
	SP_AI_Spell spells[1];

	MAxesAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);

		spells[0].info = dbcSpell.LookupEntry(DEMONIC_FRENZY);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].cooldown = 1;
	}

	void OnCombatStart(Unit *mTarget)
	{
		RegisterAIUpdateEvent(6000);

		spells[0].casttime = (uint32)time(NULL) + spells[0].cooldown;

		std::vector<Unit *> TargetTable;
		for(set<Player*>::iterator itr = _unit->GetInRangePlayerSetBegin(); itr != _unit->GetInRangePlayerSetEnd(); ++itr) 
		{
			if(isHostile(_unit, (*itr)) && (*itr)->isAlive())
			{
				Player* RandomTarget = NULL;
				RandomTarget = static_cast< Player* >(*itr);

				if (RandomTarget && RandomTarget->isAlive() && isHostile(_unit, RandomTarget))
					TargetTable.push_back(RandomTarget);
			}
		}

		if (!TargetTable.size())
			return;

		size_t RandTarget = rand()%TargetTable.size();

		Unit * RTarget = TargetTable[RandTarget];

		if (!RTarget)
			return;

		_unit->GetAIInterface()->taunt(RTarget, true);
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit *mKiller)
	{
		RemoveAIUpdateEvent();
	}

	void AIUpdate()
	{
		uint32 t = (uint32)time(NULL);
		if (t > spells[0].casttime)
		{
			_unit->CastSpell(_unit, spells[0].info, spells[0].instant);
			spells[0].casttime = t + spells[0].cooldown;
		}
	}
};

// Netherspite
#define CN_NETHERSPITE		15689
#define CN_VOIDZONE			17470

// #define NETHERBURN		30523 not aura
// #define VOIDZONE			28863
#define CONSUMPTION			32251 // used by void zone
#define NETHERBREATH		38524 // old 36631
#define N_BERSERK			38688
#define NETHERBURN			30522

class NetherspiteAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(NetherspiteAI);
	bool m_spellcheck[3];
	SP_AI_Spell spells[3];

	NetherspiteAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}	

		spells[0].info = dbcSpell.LookupEntry(NETHERBREATH);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].cooldown = RandomUInt(5)+30;
		spells[0].perctrigger = 50.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(N_BERSERK);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].cooldown = 540;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(NETHERBURN);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;

		NDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-11186.2f, -1665.14f, 281.398f, 185521);
	}

	void OnCombatStart(Unit* mTarget)
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;

		uint32 t = (uint32)time(NULL);
		VoidTimer = t + 25;
		_unit->CastSpell(_unit, spells[2].info, spells[2].instant);

		RegisterAIUpdateEvent(1000);

		if(NDoor)
		{
			NDoor->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
			NDoor->SetUInt32Value(GAMEOBJECT_FLAGS, 33);
		}
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->RemoveAura(NETHERBURN);

		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();

		if(NDoor)
			NDoor->SetByte(GAMEOBJECT_BYTES_1, 0, 0);
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();

		if(NDoor)
			NDoor->SetByte(GAMEOBJECT_BYTES_1, 0, 0);
	}

	void AIUpdate()
	{
		uint32 t = (uint32)time(NULL);
		if(t > VoidTimer && _unit->GetAIInterface()->GetNextTarget())
		{
			VoidTimer = t + 20;
			std::vector<Unit *> TargetTable;
			for(set<Player*>::iterator itr = _unit->GetInRangePlayerSetBegin(); itr != _unit->GetInRangePlayerSetEnd(); ++itr) 
			{ 
				Unit* RandomTarget = NULL;
				RandomTarget = static_cast< Unit* >(*itr);

				if (RandomTarget && RandomTarget->isAlive() && isHostile(_unit, (*itr)))
					TargetTable.push_back(RandomTarget);
			}

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit * RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;
			float vzX = 5 * cos(RandomFloat(6.28f))+RTarget->GetPositionX();
			float vzY = 5 * cos(RandomFloat(6.28f))+RTarget->GetPositionY();
			float vzZ = RTarget->GetPositionZ();
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_VOIDZONE, vzX, vzY, vzZ, 0, true, false, 0, 0);
			TargetTable.clear();
		}

		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;

				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

protected:
	int nrspells;
	uint32 VoidTimer;
	GameObject *NDoor;
};

class VoidZoneAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(VoidZoneAI);
	SP_AI_Spell spells[1];

	VoidZoneAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->Root();
		_unit->DisableAI();
		_unit->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;
		_unit->m_noRespawn = true;
		_unit->Despawn(30000, 0);

		RegisterAIUpdateEvent(2000);

		spells[0].info = dbcSpell.LookupEntry(CONSUMPTION);
		spells[0].instant = true;
		spells[0].cooldown = 2;
		spells[0].casttime = (uint32)time(NULL) + spells[0].cooldown;

		_unit->CastSpell(_unit, spells[0].info, spells[0].instant);
	}

	void AIUpdate()
	{
		uint32 t = (uint32)time(NULL);
		if(t > spells[0].casttime)
		{
			_unit->CastSpell(_unit, spells[0].casttime, spells[0].instant);
			spells[0].casttime = t + spells[0].cooldown;
		}
	}
};

//------------------------------------
//	-= Nightbane =-
//------------------------------------

/* ToDo:
- Rain of Bones on one random player/pet
- Summons five Restless Skeletons.
*/

#define CN_NIGHTBANE 17225
#define CN_RESTLESS_SKELETON 17261 // not needed if spell works

#define WALK 0
#define RUN 256
#define FLY 768

// ground spells
#define BELLOWING_ROAR 36922
#define CHARRED_EARTH 30129 //Also 30209 (Target Charred Earth) triggers this
#undef CLEAVE
#define CLEAVE 31043 // fixme: correct spell?!
#define SMOLDERING_BREATH 39385
#define TAIL_SWEEP 25653 // TODO: how to use this spell???
#define DISTRACTING_ASH 30280

// air spells
#define DISTRACTING_ASH_FLY 30130 // all guides say ground spell but animation is flying?!
#define RAIN_OF_BONES 37091 // Spell bugged: should debuff with 37098
#define SMOKING_BLAST 37057
#define FIREBALL_BARRAGE 30282
#define SUMMON_BONE_SKELETONS 30170

static Coords coords[] =
{
	{ 0, 0, 0, 0 },
	{ -11173.719727f, -1863.993164f, 130.390396f, 5.343079f }, // casting point
	{ -11125.542969f, -1926.884644f, 139.349365f, 3.982360f },
	{ -11166.404297f, -1950.729736f, 114.714726f, 1.537812f },
	{ -11167.497070f, -1922.315918f, 91.473755f, 1.390549f } // landing point
};

class NightbaneAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(NightbaneAI);
	bool m_spellcheck[5];
	SP_AI_Spell spells[5];

	NightbaneAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 5;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		//ground phase spells
		spells[0].info =  dbcSpell.LookupEntry(BELLOWING_ROAR);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].cooldown = 30; //confirmed
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1500;

		spells[1].info = dbcSpell.LookupEntry(CHARRED_EARTH);
		spells[1].targettype = TARGET_RANDOM_SINGLE;
		spells[1].instant = false;
		spells[1].cooldown = 15;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(CLEAVE);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = false;
		spells[2].cooldown = 7;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(DISTRACTING_ASH);
		spells[3].targettype = TARGET_RANDOM_SINGLE;
		spells[3].instant = false;
		spells[3].cooldown = 60;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = dbcSpell.LookupEntry(SMOLDERING_BREATH);
		spells[4].targettype = TARGET_ATTACKING;
		spells[4].instant = false;
		spells[4].cooldown = 20;
		spells[4].perctrigger = 0.0f;
		spells[4].attackstoptimer = 1000;

		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);

		for (int i = 1; i < 5; i++)
		{
			_unit->GetAIInterface()->addWayPoint(CreateWaypoint(i, 0, FLY));
		}
	}

	void OnCombatStart(Unit* mTarget)
	{
		ResetCastTime();
		m_phase = 0;
		m_currentWP = 4;
		mTailSweepTimer = 25;
		//not sure about this: _unit->PlaySoundToSet(9456);
		//TODO emote: "An ancient being awakens in the distance..."
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
		_unit->GetAIInterface()->m_moveFly = false;
		_unit->GetAIInterface()->m_canMove = true;
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
	}

	void AIUpdate()
	{
		switch(m_phase)
		{
		case 1:
		case 3:
		case 5:
			FlyPhase();
			break;

		case 0:
		case 2:
		case 4:
			GroundPhase();
			break;
		}
	}

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		switch(iWaypointId)
		{
		case 1: //casting point
			{
				_unit->GetAIInterface()->m_canMove = false;
				_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
				_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
				m_currentWP = 1;
			}break;
		case 4: //ground point
			{
				_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
				_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
				Land();
				m_currentWP = 4;
			}break;
		default:
			{
				//move to the next waypoint
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
				_unit->GetAIInterface()->setWaypointToMove(iWaypointId+1);
			}break;
		};
	}

	void FlyPhase()
	{
		if(m_currentWP != 1)
			return;

		m_FlyPhaseTimer--;
		if(!m_FlyPhaseTimer)
		{
			if(_unit->GetCurrentSpell() != NULL)
				_unit->GetCurrentSpell()->cancel();

			_unit->GetAIInterface()->m_canMove = true;
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
			_unit->GetAIInterface()->StopMovement(0);
			_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			_unit->GetAIInterface()->setWaypointToMove(2);
			m_phase++;
			return;
		}

		if(m_FlyPhaseTimer > 15)
			return;

		Unit *target = NULL;

		//first cast
		if(m_FlyPhaseTimer == 15)
		{
			//Casts Rain of Bones on one random player/pet
			//CastSpellOnRandomTarget(5, 0, 40);
			//summon 3 skeletons
			//_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), dbcSpell.LookupEntry(SUMMON_BONE_SKELETONS), true);
			return;
		}

		//Shoots powerful Smoking Blast every second for approximately 15 seconds.
		if (_unit->GetAIInterface()->GetNextTarget() != NULL)
		{
			target = _unit->GetAIInterface()->GetNextTarget();
			_unit->CastSpell(target, dbcSpell.LookupEntry(SMOKING_BLAST), true);
		}

		target = NULL;
		//fireball barrage check
		for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
		{
			if ((*itr)->GetTypeId() == TYPEID_PLAYER && (*itr)->GetInstanceID() == _unit->GetInstanceID())
			{
				target = static_cast< Unit* >(*itr);

				if(_unit->GetDistance2dSq(target) > 2025) //45 yards
				{
					_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), dbcSpell.LookupEntry(FIREBALL_BARRAGE), true);
					break; //stop
				}
			}
		}
	}

	void GroundPhase()
	{
		if(m_currentWP != 4)
			return;

		//Switch if needed
		if((m_phase == 0 && _unit->GetHealthPct() <= 75) 
			|| (m_phase == 2 && _unit->GetHealthPct() <= 50) 
			|| (m_phase == 4 && _unit->GetHealthPct() <= 25))
		{
			if(_unit->GetCurrentSpell() != NULL)
				_unit->GetCurrentSpell()->cancel();

			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);		
			_unit->GetAIInterface()->StopMovement(0);
			_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			_unit->GetAIInterface()->setWaypointToMove(1);
			Fly();
			m_FlyPhaseTimer = 17;
			m_phase++;
			return;
		}

		//Tail Sweep
		mTailSweepTimer--;
		if(!mTailSweepTimer)
		{
			Unit *target = NULL;
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{
				if ((*itr)->GetTypeId() == TYPEID_PLAYER && (*itr)->GetInstanceID() == _unit->GetInstanceID())
				{
					target = static_cast< Unit* >(*itr);

					//cone behind the boss
					if(target->isAlive() && target->isInBack(_unit))
						_unit->CastSpell(target, dbcSpell.LookupEntry(TAIL_SWEEP), true);
				}
			}
			mTailSweepTimer = 25;
		}

		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
	{
		WayPoint * wp = _unit->CreateWaypointStruct();
		wp->id = id;
		wp->x = coords[id].x;
		wp->y = coords[id].y;
		wp->z = coords[id].z;
		wp->o = coords[id].o;
		wp->waittime = waittime;
		wp->flags = flags;
		wp->forwardemoteoneshot = 0;
		wp->forwardemoteid = 0;
		wp->backwardemoteoneshot = 0;
		wp->backwardemoteid = 0;
		wp->forwardskinid = 0;
		wp->backwardskinid = 0;
		return wp;
	}

	void Fly()
	{
		_unit->Emote(EMOTE_ONESHOT_LIFTOFF);

		WorldPacket data(SMSG_MOVE_SET_HOVER, 13);
		data << _unit->GetNewGUID();
		data << uint32(0);
		_unit->SendMessageToSet(&data, false);

		_unit->GetAIInterface()->m_moveFly = true;
	}

	void Land()
	{
		_unit->Emote(EMOTE_ONESHOT_LAND);

		WorldPacket data(SMSG_MOVE_UNSET_HOVER, 13);
		data << _unit->GetNewGUID();
		data << uint32(0);
		_unit->SendMessageToSet(&data, false);

		_unit->GetAIInterface()->m_moveFly = false;
	}

	void ResetCastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;

				if (m_spellcheck[i])
				{
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					case TARGET_RANDOM_SINGLE:
						CastSpellOnRandomTarget(i, 0, 0); break;
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
			/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{
				if (((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID())
				{
					Unit* RandomTarget = NULL;
					RandomTarget = static_cast< Unit* >(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && _unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (!TargetTable.size())
				return;

			Unit * RTarget = *(TargetTable.begin()+rand()%TargetTable.size());

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}

protected:
	int nrspells;
	uint32 m_phase;
	uint32 m_FlyPhaseTimer;
	uint32 m_currentWP;
	uint32 mTailSweepTimer;
};

//Opera Event
//Wizzard of Oz
#define CN_DOROTHEE 17535

#define SP_AOE_FEAR 29321
#define SP_WATER_BOLT 31012
#define SP_SUMMON_TITO 31014

#define TEXT_AGGRO_DOROTHEE "Oh dear, we simply must find a way home! The old wizard could be our only hope! Strawman, Roar, Tinhead, will you... wait! Oh golly, look! We have visitors!"
#define SOUND_AGGRO_DOROTHEE 9195

#define TEXT_DEATH_DOROTHEE "Oh at last, at last. I can go home."
#define SOUND_DEATH_DOROTHEE 9190

#define TEXT_DOROTHEE_TITO_SUMMON "Don't let them hurt us, Tito! Oh, you won't, will you?"
#define SOUND_DOROTHEE_TITO_SUMMON 9191 

#define TEXT_DOROTHEE_TITO_DEATH "Tito, oh Tito, no!"
#define SOUND_DOROTHEE_TITO_DEATH 9192

class DorotheeAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(DorotheeAI);
	bool m_spellcheck[2];
	SP_AI_Spell spells[2];
	uint32 summontito;


	DorotheeAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(SP_AOE_FEAR);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 30;  //correct cooldown?
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(SP_WATER_BOLT);
		spells[1].targettype = TARGET_RANDOM_SINGLE;
		spells[1].instant = false;
		spells[1].perctrigger = 100.0f;
		spells[1].attackstoptimer = 1000;
	}

	void OnCombatStart(Unit* mTarget)
	{
		CastTime();
		RegisterAIUpdateEvent(1000);

		_unit->PlaySoundToSet(SOUND_AGGRO_DOROTHEE);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_AGGRO_DOROTHEE);
	}

	void OnCombatStop(Unit *mTarget)
	{
		CastTime();
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();

		_unit->SafeDelete();
	}

	void OnDied(Unit * mKiller)
	{
		//Check to see if we can spawn The Crone now
		Creature* Dorothee	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10897.650f, -1755.8311f, 90.476f, 17535); //Dorothee
		Creature* Strawman	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10904.088f, -1754.8988f, 90.476f, 17543); //Strawman
		Creature* Roar		= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10891.115f, -1756.4898f, 90.476f, 17546);//Roar
		Creature* Tinman	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10884.501f, -1757.3249f, 90.476f, 17547); //Tinman

		if ((Dorothee == NULL || Dorothee->IsDead()) && (Strawman == NULL || Strawman->IsDead()) && (Roar == NULL || Roar->IsDead()) && (Tinman == NULL || Tinman->IsDead()))
		{
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(18168, -10884.501f, -1757.3249f, 90.476f, 0.0f, false, true, 0, 0);
		}
		//END

		_unit->PlaySoundToSet(SOUND_DEATH_DOROTHEE);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_DEATH_DOROTHEE);
		CastTime();
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit * mTarget)
	{
	}
	void SpawnTito()	// Lacking in collision checks!
	{
		float xchange  = (float)RandomFloat(15.0f);
		float distance = 15.0f;

		float ychange = sqrt(distance*distance - xchange*xchange);

		if (rand()%2 == 1)
			xchange *= -1;
		if (rand()%2 == 1)
			ychange *= -1;

		float newposx = _unit->GetPositionX() + xchange;
		float newposy = _unit->GetPositionY() + ychange;

		tito = _unit->GetMapMgr()->GetInterface()->SpawnCreature(17548, newposx, newposy, _unit->GetPositionZ() + 0.5f, 2.177125f, true, false, 0, 0);
	}
	void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);

		if(titoSpawned && !tito && titoDeadSpeech)
		{
			_unit->PlaySoundToSet(SOUND_DOROTHEE_TITO_DEATH);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_DOROTHEE_TITO_DEATH);
			titoDeadSpeech = false;
		}

		if(summontito > 20 && !titoSpawned)
		{
			_unit->PlaySoundToSet(SOUND_DOROTHEE_TITO_SUMMON);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_DOROTHEE_TITO_SUMMON);
			SpawnTito();
			titoSpawned = true;
			titoDeadSpeech = true;
			return;
		}
		summontito++;
	}

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			std::vector<Unit*> TargetTable;		
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = static_cast< Unit* >(*itr);

					if (RandomTarget && RandomTarget == _unit->GetAIInterface()->GetMostHated() && i == 3)
						continue;

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit * RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			TargetTable.clear();
		}
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;

				if (m_spellcheck[i])
				{
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					case TARGET_RANDOM_FRIEND:
					case TARGET_RANDOM_SINGLE:
					case TARGET_RANDOM_DESTINATION:
						CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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
	Unit* tito;
	bool titoSpawned;
	bool titoDeadSpeech;
};	

#define CN_TITO	17548

#define SP_ANNOYING_YIPPING	31015 

//No kill sound

class TitoAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(TitoAI);
	bool m_spellcheck[1];
	SP_AI_Spell spells[1];


	TitoAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(SP_ANNOYING_YIPPING);  
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true; 
		spells[0].perctrigger = 15.0f;
		spells[0].attackstoptimer = 1000;

		_unit->m_noRespawn = true;
	}

	void OnCombatStart(Unit* mTarget)
	{
		CastTime();
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		CastTime();
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();

		_unit->SafeDelete();
	}

	void OnDied(Unit * mKiller)
	{
		CastTime();
		RemoveAIUpdateEvent();
	}

	void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;

				if (m_spellcheck[i])
				{
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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

#define CN_STRAWMAN	17543

#define SP_BURNING_STRAW	31075
#define SP_BRAIN_BASH	31046


#define TEXT_AGGRO_STRAWMAN	"Now what should I do with you? I simply can't make up my mind."
#define SOUND_AGGRO_STRAWMAN	9254

#define TEXT_DEATH_STRAWMAN	"Don't let them make a mattress... out of me."
#define SOUND_DEATH_STRAWMAN	9256

#define TEXT_KILL_STRAWMAN	"I guess I'm not a failure after all. "
#define SOUND_KILL_STRAWMAN	9257

class StrawmanAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(StrawmanAI);
	bool m_spellcheck[2];
	SP_AI_Spell spells[2];

	StrawmanAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(SP_BURNING_STRAW);//  NEEDS TO BE SO IT ONLY AFFECTS HIM WHEN HE IS HIT BY FIRE DMG!
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(SP_BRAIN_BASH);
		spells[1].targettype = TARGET_RANDOM_SINGLE;
		spells[1].instant = true;
		spells[1].cooldown = 8; //not sure about this
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

	}

	void OnCombatStart(Unit* mTarget)
	{
		CastTime();
		RegisterAIUpdateEvent(1000);

		_unit->PlaySoundToSet(SOUND_AGGRO_STRAWMAN);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_AGGRO_STRAWMAN);
	}

	void OnCombatStop(Unit *mTarget)
	{
		CastTime();
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();

		_unit->SafeDelete();
	}

	void OnDied(Unit * mKiller)
	{
		//Check to see if we can spawn The Crone now
		Creature* Dorothee	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10897.650f, -1755.8311f, 90.476f, 17535);	//Dorothee
		Creature* Strawman	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10904.088f, -1754.8988f, 90.476f, 17543);	//Strawman
		Creature* Roar		= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10891.115f, -1756.4898f, 90.476f, 17546);	//Roar
		Creature* Tinman	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10884.501f, -1757.3249f, 90.476f, 17547);	//Tinman

		if ((Dorothee == NULL || Dorothee->IsDead()) && (Strawman == NULL || Strawman->IsDead()) && (Roar == NULL || Roar->IsDead()) && (Tinman == NULL || Tinman->IsDead()))
		{
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(18168, -10884.501f, -1757.3249f, 90.476f, 0.0f, false, true, 0, 0);
		}
		//END

		_unit->PlaySoundToSet(SOUND_DEATH_STRAWMAN);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_DEATH_STRAWMAN);
		CastTime();
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit * mTarget)
	{
		_unit->PlaySoundToSet(SOUND_KILL_STRAWMAN);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_KILL_STRAWMAN);
	}

	void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;

				if (m_spellcheck[i])
				{
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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

#define CN_TINHEAD	17547

#define SP_CLEAVE		15284
//#define SP_RUST		31086
//dont bother.. spell does not work.. needs fix

#define TEXT_AGGRO_TINHEAD	"I could really use a heart. Say, can I have yours?"
#define SOUND_AGGRO_TINHEAD	9268

#define TEXT_DEATH_TINHEAD	"Back to being an old rust bucket.."
#define SOUND_DEATH_TINHEAD	9270

#define TEXT_KILL_TINHEAD	"Guess I'm not so rusty, after all."
#define SOUND_KILL_TINHEAD	9271

class TinheadAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(TinheadAI);
	bool m_spellcheck[1];
	SP_AI_Spell spells[1];

	TinheadAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(SP_CLEAVE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

		/*spells[1].info = dbcSpell.LookupEntry(RUST);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].cooldown = 60;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;
		*/
	}

	void OnCombatStart(Unit* mTarget)
	{
		CastTime();
		RegisterAIUpdateEvent(1000);

		_unit->PlaySoundToSet(SOUND_AGGRO_TINHEAD);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_AGGRO_TINHEAD);
	}

	void OnCombatStop(Unit *mTarget)
	{
		CastTime();
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();

		_unit->SafeDelete();
	}

	void OnDied(Unit * mKiller)
	{
		//Check to see if we can spawn The Crone now
		Creature* Dorothee	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10897.650f, -1755.8311f, 90.476f, 17535);	//Dorothee
		Creature* Strawman	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10904.088f, -1754.8988f, 90.476f, 17543);	//Strawman
		Creature* Roar		= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10891.115f, -1756.4898f, 90.476f, 17546);	//Roar
		Creature* Tinman	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10884.501f, -1757.3249f, 90.476f, 17547);	//Tinman

		if ((Dorothee == NULL || Dorothee->IsDead()) && (Strawman == NULL || Strawman->IsDead()) && (Roar == NULL || Roar->IsDead()) && (Tinman == NULL || Tinman->IsDead()))
		{
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(18168, -10884.501f, -1757.3249f, 90.476f, 0.0f, false, true, 0, 0);
		}
		//END

		_unit->PlaySoundToSet(SOUND_DEATH_TINHEAD);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_DEATH_TINHEAD);
		CastTime();
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit * mTarget)
	{
		_unit->PlaySoundToSet(SOUND_KILL_TINHEAD);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_KILL_TINHEAD);
	}

	void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;

				if (m_spellcheck[i])
				{
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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

#define CN_ROAR	17546

#define TEXT_AGGRO_ROAR	"Wanna fight? Huh? Do ya? C'mon, I'll fight you with both claws behind my back!"
#define SOUND_AGGRO_ROAR	9227

#define TEXT_DEATH_ROAR "You didn't have to go and do that!"
#define SOUND_DEATH_ROAR	9229

class RoarAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(RoarAI);

	RoarAI(Creature* pCreature) : CreatureAIScript(pCreature) {}

	void OnCombatStart(Unit* mTarget)
	{
		_unit->PlaySoundToSet(SOUND_AGGRO_ROAR);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_AGGRO_ROAR);
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		_unit->SafeDelete();
	}

	void OnDied(Unit * mKiller)
	{
		//Check to see if we can spawn The Crone now
		Creature* Dorothee	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10897.650f, -1755.8311f, 90.476f, 17535); //Dorothee
		Creature* Strawman	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10904.088f, -1754.8988f, 90.476f, 17543); //Strawman
		Creature* Roar		= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10891.115f, -1756.4898f, 90.476f, 17546);//Roar
		Creature* Tinman	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10884.501f, -1757.3249f, 90.476f, 17547); //Tinman

		if((Dorothee == NULL || Dorothee->IsDead()) && (Strawman == NULL || Strawman->IsDead()) && (Roar == NULL || Roar->IsDead()) && (Tinman == NULL || Tinman->IsDead()))
		{
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(18168, -10884.501f, -1757.3249f, 90.476f, 0.0f, false, true, 0, 0);
		}
		//END

		_unit->PlaySoundToSet(SOUND_DEATH_ROAR);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_DEATH_ROAR);
	}
};

#define CN_CRONE 18168

#define SP_SUMMON_CYCLONE 38337
#define SP_CHAIN_LIGHTNING 32337

#define TEXT_AGGRO_CRONE	"Woe to each and every one of you my pretties!"
#define SOUND_AGGRO_CRONE	9179 //needs correct sound id

#define TEXT_KILL_CRONE "Fixed you, didn't I?"
#define SOUND_KILL_CRONE 9280 //needs correct sound id

#define TEXT_DEATH_CRONE "How could you? What a cruel, cruel world!"
#define SOUND_DEATH_CRONE	9178 //needs correct sound id


class CroneAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CroneAI);
	bool m_spellcheck[2];
	SP_AI_Spell spells[2];

	CroneAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(SP_SUMMON_CYCLONE); 
		spells[0].targettype = TARGET_DESTINATION;
		spells[0].instant = true;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(SP_CHAIN_LIGHTNING); 
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 1000;
	}

	void OnCombatStart(Unit* mTarget)
	{
		CastTime();
		RegisterAIUpdateEvent(1000);

		_unit->PlaySoundToSet(SOUND_AGGRO_CRONE);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_AGGRO_CRONE);
	}

	void OnCombatStop(Unit *mTarget)
	{
		if(_unit->GetHealthPct() > 0)
		{
			GameObject* DoorLeft = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10917.1445f, -1774.05f, 90.478f, 184279);
			GameObject* DoorRight = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10872.195f, -1779.42f, 90.45f, 184278);
			GameObject* Curtain = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10894.17f, -1774.218f, 90.477f, 183932);

			if(DoorLeft)
				DoorLeft->SetByte(GAMEOBJECT_BYTES_1, 0, 1);

			if(DoorRight)
				DoorRight->SetByte(GAMEOBJECT_BYTES_1, 0, 0);

			if(Curtain)
				Curtain->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
		}

		CastTime();
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();

		_unit->SafeDelete();
	}

	void OnDied(Unit * mKiller)
	{
		GameObject* DoorLeft = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10917.1445f, -1774.05f, 90.478f, 184279);
		GameObject* DoorRight = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10872.195f, -1779.42f, 90.45f, 184278);
		GameObject* Curtain = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10894.17f, -1774.218f, 90.477f, 183932);

		if(DoorLeft)
			DoorLeft->SetByte(GAMEOBJECT_BYTES_1, 0, 0);

		if(DoorRight)
			DoorRight->SetByte(GAMEOBJECT_BYTES_1, 0, 0);

		// Make sure the curtain stays up
		if(Curtain)
			Curtain->SetByte(GAMEOBJECT_BYTES_1, 0, 0);

		_unit->PlaySoundToSet(SOUND_DEATH_CRONE);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_DEATH_CRONE);
		CastTime();
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit * mTarget)
	{
	}

	void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;

				if (m_spellcheck[i])
				{
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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

#define CN_CYCLONEOZ			22104
#define CYCLONE_VISUAL			32332
#define CYCLONE_KNOCK			38517

class CycloneOZ : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CycloneOZ);
	CycloneOZ(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(CYCLONE_VISUAL), true);
		_unit->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;
		_unit->m_noRespawn = true;
	}

	void OnCombatStart(Unit* mTarget)
	{
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
	}

	void AIUpdate()
	{
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(CYCLONE_KNOCK), true);
	}

protected:
	int nrspells;
};

/*
*
*
*
*
*
*/


//Romulo & Julianne
#define CN_ROMULO 17533

#define SP_BACKWARD_LUNGE 30815
#define SP_DEADLY_SWATHE 30817
#define SP_POISONED_THRUST 30822
#define SP_DARING 30841

#define TEXT_AGGRO_ROMULO "Wilt thou provoke me? Then have at thee, boy!"
#define SOUND_AGGRO_ROMULO 9233

#define TEXT_DEATH_ROMULO "Thou smilest... upon the stroke that... murders me."
#define SOUND_DEATH_ROMULO 9235

#define TEXT_DEATH2_ROMULO "This day's black fate on more days doth depend. This but begins the woe. Others must end."
#define SOUND_DEATH2_ROMULO 9236

#define TEXT_KILL_ROMULO "How well my comfort is revived by this! "
#define SOUND_KILL_ROMULO 9238

#define TEXT_RES_ROMULO "Thou detestable maw, thou womb of death; I enforce thy rotten jaws to open!"
#define SOUND_RES_ROMULO 9237

class RomuloAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(RomuloAI);
	bool m_spellcheck[4];
	SP_AI_Spell spells[4];

	RomuloAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(SP_BACKWARD_LUNGE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].cooldown = 12;
		spells[0].perctrigger = 20.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(SP_DEADLY_SWATHE);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].cooldown = 0;
		spells[1].perctrigger = 20.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(SP_POISONED_THRUST);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = false;
		spells[2].cooldown = 0;
		spells[2].perctrigger = 20.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(SP_DARING);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = false;
		spells[3].cooldown = 0;
		spells[3].perctrigger = 20.0f;
		spells[3].attackstoptimer = 1000;
	}

	void OnCombatStart(Unit* mTarget)
	{
		CastTime();
		RegisterAIUpdateEvent(1000);

		_unit->PlaySoundToSet(SOUND_AGGRO_ROMULO);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_AGGRO_ROMULO);
	}

	void OnCombatStop(Unit *mTarget)
	{
		if(_unit->GetHealthPct() > 0)
		{
			GameObject* DoorLeft = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10917.1445f, -1774.05f, 90.478f, 184279);
			GameObject* DoorRight = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10872.195f, -1779.42f, 90.45f, 184278);
			GameObject* Curtain = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10894.17f, -1774.218f, 90.477f, 183932);

			if(DoorLeft)
				DoorLeft->SetByte(GAMEOBJECT_BYTES_1, 0, 1);

			if(DoorRight)
				DoorRight->SetByte(GAMEOBJECT_BYTES_1, 0, 0);

			if(Curtain)
				Curtain->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
		}

		CastTime();
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
		if(_unit)
			_unit->SafeDelete();
	}

	void OnDied(Unit * mKiller)
	{
		GameObject* DoorLeft = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10917.1445f, -1774.05f, 90.478f, 184279);
		GameObject* DoorRight = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10872.195f, -1779.42f, 90.45f, 184278);
		GameObject* Curtain = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10894.17f, -1774.218f, 90.477f, 183932);

		if(DoorLeft)
			DoorLeft->SetByte(GAMEOBJECT_BYTES_1, 0, 0);

		if(DoorRight)
			DoorRight->SetByte(GAMEOBJECT_BYTES_1, 0, 0);

		// Make sure the curtain stays up
		if(Curtain)
			Curtain->SetByte(GAMEOBJECT_BYTES_1, 0, 0);

		_unit->PlaySoundToSet(SOUND_DEATH_ROMULO);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_DEATH_ROMULO);
		CastTime();
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit * mTarget)
	{
		_unit->PlaySoundToSet(SOUND_KILL_ROMULO);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_KILL_ROMULO);
	}

	void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;

				if (m_spellcheck[i])
				{
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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

#define CN_JULIANNE 17534

#define SP_ETERNAL_AFFECTION 30878
#define SP_POWERFUL_ATTRACTION 30889
#define SP_BINDING_PASSION 30890
#define SP_DEVOTION 30887

#define TEXT_AGGRO_JULIANNE "What devil art thou, that dost torment me thus?"
#define SOUND_AGGRO_JULIANNE 9196

#define TEXT_DEATH_JULIANNE "Romulo, I come! Oh... this do I drink to thee! "
#define SOUND_DEATH_JULIANNE 9198

#define TEXT_DEATH2_JULIANNE "Where is my Lord? Where is my Romulo? Ohh, happy dagger! This is thy sheath! There rust, and let me die!"
#define SOUND_DEATH2_JULIANNE 9199

#define TEXT_KILL_JULIANNE "Parting is such sweet sorrow."
#define SOUND_KILL_JULIANNE 9201

#define TEXT_RES_ROMULO_JULIANNE "Come, gentle night; and give me back my Romulo!"
#define SOUND_RES_ROMULO_JULIANNE 9200

class JulianneAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(JulianneAI);
	bool m_spellcheck[4];
	SP_AI_Spell spells[4];

	JulianneAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(SP_ETERNAL_AFFECTION);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = false;
		spells[0].cooldown = 12;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(SP_POWERFUL_ATTRACTION);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].cooldown = 0;
		spells[1].perctrigger = 5.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(SP_BINDING_PASSION);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = false;
		spells[2].cooldown = 0;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(SP_DEVOTION);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = false;
		spells[3].cooldown = 0;
		spells[3].perctrigger = 5.0f;
		spells[3].attackstoptimer = 1000;
	}

	void OnCombatStart(Unit* mTarget)
	{
		CastTime();
		RegisterAIUpdateEvent(1000);

		_unit->PlaySoundToSet(SOUND_AGGRO_JULIANNE);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_AGGRO_JULIANNE);
	}

	void OnCombatStop(Unit *mTarget)
	{
		if(_unit->GetHealthPct() > 0)
		{
			GameObject* DoorLeft = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10917.1445f, -1774.05f, 90.478f, 184279);
			GameObject* DoorRight = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10872.195f, -1779.42f, 90.45f, 184278);
			GameObject* Curtain = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10894.17f, -1774.218f, 90.477f, 183932);

			if(DoorLeft)
				DoorLeft->SetByte(GAMEOBJECT_BYTES_1, 0, 1);

			if(DoorRight)
				DoorRight->SetByte(GAMEOBJECT_BYTES_1, 0, 0);

			if(Curtain)
				Curtain->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
		}

		CastTime();
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
		if(_unit)
			_unit->SafeDelete();
	}

	void OnDied(Unit * mKiller)
	{
		//_unit->RemoveAllAuras();
		//_unit->setEmoteState(EMOTE_ONESHOT_EAT);
		//_unit->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(17533, -10891.582f, -1755.5177f, 90.476f, 4.61f, false, true, 0, 0);
		_unit->PlaySoundToSet(SOUND_DEATH_JULIANNE);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_DEATH_JULIANNE);
		//_unit->setEmoteState(EMOTE_STATE_DEAD);

		//_unit->PlaySoundToSet(SOUND_DEATH2_JULIANNE);
		//_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_DEATH2_JULIANNE);
		CastTime();
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit * mTarget)
	{
		_unit->PlaySoundToSet(SOUND_KILL_JULIANNE);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_KILL_JULIANNE);
	}

	void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;

				if (m_spellcheck[i])
				{
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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



//Spells Used by booth
#define Chess_Event_Spell_Move					//Not sure Right now, The spell needs to be scripted
#define Chess_Event_Spell_SelectEnemy		30520 //Dont Know, but Sohuld Work
#define Chess_Event_Spell_ChangeFacing		30284 //Need Scripting
#define Chess_Event_Spell_ControlPiece		30019 //After Casting the spell the player should be teleported to the stairs
#define Chess_Event_Spell_MoveMarker		32261 //A Pillat of Light showing that you have moved
//Echo Of Medivh
#define Chess_Event_Medivh					16816

#pragma warning(disable:4305)
#pragma warning(disable:4700)
#pragma warning(disable:4101)

//Horde Pieces and Spells
#define Chess_Event_HKing					21752
//#define Chess_Event_HKing_Ability1		37476
//#define Chess_Event_Hking_Ability2		37472

#define Chess_Event_HQueen					21750
//#define Chess_Event_HQueen_Ability1		37463
//#define Chess_Event_HQueen_Ability2		37469

#define Chess_Event_HBishop					21747
//#define Chess_Event_HBishop_Ability1		37456
//#define Chess_Event_HBishop_Ability2		37461

#define Chess_Event_HKnight					21748
//#define Chess_Event_HKnight_Ability1		37454
//#define Chess_Event_HKnight_Ability2		37502

#define Chess_Event_HRook					21726
//#define Chess_Event_HRook_Ability1		37428
//#define Chess_Event_HRook_Ability2		37434

#define Chess_Event_HPawn					17469

#define Chess_Event_HPawn_Ability1			37413

#define Chess_Event_HPawn_Ability2			37416


//Alliance Pieces And Spells
#define Chess_Event_AKing					21684
//#define Chess_Event_AKing_Ability1		37474
//#define Chess_Event_Aking_Ability2		37471

#define Chess_Event_AQueen					21683
//#define Chess_Event_AQueen_Ability1		37462
//#define Chess_Event_AQueen_Ability2		37465

#define Chess_Event_ABishop					21682
//#define Chess_Event_ABishop_Ability1		37455
//#define Chess_Event_ABishop_Ability2		37459

#define Chess_Event_AKnight					21664
//#define Chess_Event_AKnight_Ability1		37453
//#define Chess_Event_AKnight_Ability2		37498

#define Chess_Event_ARook					21160
//#define Chess_Event_ARook_Ability1		37427
//#define Chess_Event_ARook_Ability2		37432

#define Chess_Event_APawn					17211
//#define Chess_Event_APawn_Ability1		37406
//#define Chess_Event_APawn_Ability2		37414
class ChessAI : public ArcScriptCreatureAI
{
public: ARCSCRIPT_FACTORY_FUNCTION(ChessAI, ArcScriptCreatureAI);
		ChessAI(Creature* pCreature) : ArcScriptCreatureAI(pCreature)
		{
		}
};
class SCRIPT_DECL ChessMedvihGossip : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
	void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		Menu->AddItem(0, "Let The Game Begin", 1);

		if(AutoSend)
			Menu->SendTo(Plr);
	}


	void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		GossipMenu * Menu;
		switch(IntId)
		{
		case 1:  
			{
				ChessAI* Chess;
				//Not Ready Yet
				//Spawn The Horde Side
				//Spawn The Horde King
				Chess->SpawnCreature(21752, -11063.6, -1866.47, 220.667, 3.84835, false);
				//Spawn The Horde Queen
				Chess->SpawnCreature(21750, -11067, -1861.86, 220.667, 3.72268, false);
				//Spawn The 2 Horde Bishops
				Chess->SpawnCreature(21747, -11060.3, -1870.81, 220.667, 3.76195, false);
				Chess->SpawnCreature(21747, -11070.8, -1857.7, 220.667, 3.81692, false);
				//Spawn The 2 Horde Knights
				Chess->SpawnCreature(21748, -11056.7, -1875.07, 220.667, 3.7698, false);
				Chess->SpawnCreature(21748, -11074, -1853.28, 220.667, 3.84199, false);
				//Spawn The 2 Horde Rooks
				Chess->SpawnCreature(21726, -11077.5, -1848.75, 220.667, 3.80665, false);
				Chess->SpawnCreature(21726, -11053.4, -1879.64, 220.667, 3.89304, false);
				//Spawn The 8 Horde Pawns
				Chess->SpawnCreature(17469, -11075, -1861.13, 220.667, 3.80908, false);
				Chess->SpawnCreature(17469, -11078.5, -1856.93, 220.667, 3.80908, false);
				Chess->SpawnCreature(17469, -11057.4, -1883.03, 220.667, 3.813, false);
				Chess->SpawnCreature(17469, -11067.8, -1869.89, 220.667, 3.80908, false);
				Chess->SpawnCreature(17469, -11064.3, -1874.3, 220.667, 3.80908, false);
				Chess->SpawnCreature(17469, -11061, -1878.56, 220.667, 3.80908, false);
				Chess->SpawnCreature(17469, -11071.5, -1865.54, 220.667, 3.80908, false);
				Chess->SpawnCreature(17469, -11082.1, -1852.43, 220.667, 3.80908, false);
				//Spawn The Alliance Side
				//Spawn The Alliance King
				Chess->SpawnCreature(21684, -11094.1, -1890.57, 220.667, 0.62578, false);
				//Spawn The Alliance Queen
				Chess->SpawnCreature(21683, -11097.6, -1886.43, 220.667, 0.723955, false);
				//Spawn The 2 Alliance Bishops
				Chess->SpawnCreature(21682, -11101, -1881.83, 220.667, 0.598288, false);
				Chess->SpawnCreature(21682, -11090.5, -1895.02, 220.667, 0.665045, false);
				//Spawn The 2 Alliance Knights
				Chess->SpawnCreature(21664, -11087.2, -1899.56, 220.667, 0.624056, false);
				Chess->SpawnCreature(21664, -11104.4, -1877.7, 220.667, 0.59264, false);
				//Spawn The 2 Alliance Rooks
				Chess->SpawnCreature(21160, -11108.1, -1872.91, 220.667, 0.679031, false);
				Chess->SpawnCreature(21160, -11083.7, -1903.94, 220.667, 0.671177, false);
				//Spawn The 8 Alliance Pawns
				Chess->SpawnCreature(17211, -11093.4, -1882.59, 220.668, 0.671956,false);
				Chess->SpawnCreature(17211, -11086.2, -1891.53, 220.667, 0.617928,false);
				Chess->SpawnCreature(17211, -11089.7, -1887.03, 220.667, 0.617927,false);
				Chess->SpawnCreature(17211, -11096.8, -1878.21, 220.668, 0.593409,false);
				Chess->SpawnCreature(17211, -11100.2, -1873.96, 220.668, 0.597337,false);
				Chess->SpawnCreature(17211, -11103.7, -1869.63, 220.667, 0.661124,false);
				Chess->SpawnCreature(17211, -11079.4, -1900.17, 220.667, 0.649344,false);
				Chess->SpawnCreature(17211, -11082.9, -1895.75, 220.667, 0.759299,false);
				GossipEnd(pObject,Plr);
				break;
			}
		}
	}
};
class ChessHordeAI : public ArcScriptCreatureAI
{
public:
	ARCSCRIPT_FACTORY_FUNCTION(ChessHordeAI, ArcScriptCreatureAI);

	ChessHordeAI(Creature* pCreature) : ArcScriptCreatureAI(pCreature)
	{
	}
	void OnDied(Unit *pKiller)
	{
		_unit->Despawn(2,0);
	}
};
class ChessAllianceAI : public ArcScriptCreatureAI
{
public:
	ARCSCRIPT_FACTORY_FUNCTION(ChessAllianceAI, ArcScriptCreatureAI);

	ChessAllianceAI(Creature* pCreature) : ArcScriptCreatureAI(pCreature)
	{
	}
	void OnDied(Unit *pKiller)
	{	
		_unit->Despawn(2,0);
	}
};
class ChessPieceAI : public ArcScriptCreatureAI
{
public:
	ARCSCRIPT_FACTORY_FUNCTION(ChessPieceAI, ArcScriptCreatureAI);

	ChessPieceAI(Creature* pCreature) : ArcScriptCreatureAI(pCreature)
	{
	}
	void OnDied(Unit *pKiller)
	{
		_unit->Despawn(2,0);
	}
};
class SCRIPT_DECL ChessPieceGossip : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
	void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);

		Menu->AddItem(0, "I want To Play As You", 1);

		if(AutoSend)
			Menu->SendTo(Plr);
	}


	void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		GossipMenu * Menu;
		switch(IntId)
		{
		case 1:  
			{
				Plr->CastSpell(Plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(Plr->GetPositionX(), Plr->GetPositionY(), Plr->GetPositionZ(),17469),30019,true);
				//Pet * Piece;
				//    Piece->SetActionBarSlot(0,NULL);
				//	Piece->SetActionBarSlot(1,30284);
				//	Piece->SetActionBarSlot(2,NULL);
				//	Piece->SetActionBarSlot(3,NULL);
				//	Piece->SetActionBarSlot(4,NULL);
				//	Piece->SetActionBarSlot(5,NULL);
				//	Piece->SetActionBarSlot(6,NULL);
				//	Piece->SetActionBarSlot(7,NULL);
				//	Piece->SetActionBarSlot(8,NULL);
				//	Piece->SetActionBarSlot(9,NULL);
				//	Piece->SetActionBarSlot(10,NULL);
				break;
			}
		}
	}
};
void SetupKarazhan(ScriptMgr* pScriptMgr)
{
	GossipScript * KBerthold = (GossipScript*) new Berthold();
	GossipScript * ChessGossip = (GossipScript*) new ChessMedvihGossip();
	GossipScript * ChessPieceSay = (GossipScript*) new ChessPieceGossip();
	pScriptMgr->register_gossip_script(16153, KBerthold);
	pScriptMgr->register_gossip_script(16816, ChessGossip);
	//Alliance Gossip For Choosing the Piece
	pScriptMgr->register_gossip_script(17211, ChessPieceSay);
	pScriptMgr->register_gossip_script(21160, ChessPieceSay);
	pScriptMgr->register_gossip_script(21664, ChessPieceSay);
	pScriptMgr->register_gossip_script(21682, ChessPieceSay);
	pScriptMgr->register_gossip_script(21683, ChessPieceSay);
	pScriptMgr->register_gossip_script(21684, ChessPieceSay);
	//Horde Gossip For Choosing the Piece
	pScriptMgr->register_gossip_script(17469, ChessPieceSay);
	pScriptMgr->register_gossip_script(21726, ChessPieceSay);
	pScriptMgr->register_gossip_script(21748, ChessPieceSay);
	pScriptMgr->register_gossip_script(21747, ChessPieceSay);
	pScriptMgr->register_gossip_script(21750, ChessPieceSay);
	pScriptMgr->register_gossip_script(21752, ChessPieceSay);

	pScriptMgr->register_creature_script(Chess_Event_HKing, &ChessHordeAI::Create);
	pScriptMgr->register_creature_script(Chess_Event_AKing, &ChessAllianceAI::Create);
	pScriptMgr->register_creature_script(Chess_Event_Medivh, &ChessAI::Create);


	pScriptMgr->register_creature_script(Chess_Event_HPawn, &ChessPieceAI::Create);
	pScriptMgr->register_creature_script(Chess_Event_APawn, &ChessPieceAI::Create);

	pScriptMgr->register_creature_script(Chess_Event_HRook, &ChessPieceAI::Create);
	pScriptMgr->register_creature_script(Chess_Event_ARook, &ChessPieceAI::Create);

	pScriptMgr->register_creature_script(Chess_Event_HBishop, &ChessPieceAI::Create);
	pScriptMgr->register_creature_script(Chess_Event_ABishop, &ChessPieceAI::Create);

	pScriptMgr->register_creature_script(Chess_Event_HKnight, &ChessPieceAI::Create);
	pScriptMgr->register_creature_script(Chess_Event_AKnight, &ChessPieceAI::Create);

	pScriptMgr->register_creature_script(Chess_Event_HQueen, &ChessPieceAI::Create);
	pScriptMgr->register_creature_script(Chess_Event_AQueen, &ChessPieceAI::Create);

	pScriptMgr->register_creature_script(CN_ATTUMEN, &AttumenTheHuntsmanAI::Create);
	pScriptMgr->register_creature_script(CN_MIDNIGHT, &MidnightAI::Create);
	pScriptMgr->register_creature_script(CN_MOROES, &MoroesAI::Create);
	pScriptMgr->register_creature_script(CN_MAIDENOFVIRTUE, &MaidenOfVirtueAI::Create);

	// Opera event related
	pScriptMgr->register_creature_script(CN_BIGBADWOLF, &BigBadWolfAI::Create);
	pScriptMgr->register_creature_script(CN_ROMULO, &RomuloAI::Create);
	pScriptMgr->register_creature_script(CN_JULIANNE, &JulianneAI::Create);
	pScriptMgr->register_creature_script(19525, &StageLight::Create);
	GossipScript * KGrandMother = (GossipScript*) new GrandMother;
	GossipScript * KBarnes = (GossipScript*) new BarnesGS;
	pScriptMgr->register_gossip_script(16812, KBarnes);
	pScriptMgr->register_gossip_script(17603, KGrandMother);
	pScriptMgr->register_creature_script(16812, &BarnesAI::Create);

	//WoOz here... commented yet to be implemented - kamyn
	pScriptMgr->register_creature_script(CN_DOROTHEE, &DorotheeAI::Create);
	pScriptMgr->register_creature_script(CN_STRAWMAN, &StrawmanAI::Create);
	pScriptMgr->register_creature_script(CN_TINHEAD, &TinheadAI::Create);
	pScriptMgr->register_creature_script(CN_ROAR, &RoarAI::Create);
	pScriptMgr->register_creature_script(CN_TITO, &TitoAI::Create);
	pScriptMgr->register_creature_script(CN_CRONE, &CroneAI::Create);
	pScriptMgr->register_creature_script(CN_CYCLONEOZ, &CycloneOZ::Create);

	pScriptMgr->register_creature_script(CN_CURATOR, &CuratorAI::Create);
	pScriptMgr->register_creature_script(CN_ASTRALFLARE, &AstralFlareAI::Create);

	pScriptMgr->register_creature_script(CN_ILLHOOF, &IllhoofAI::Create);
	pScriptMgr->register_creature_script(CN_KILREK, &KilrekAI::Create);
	pScriptMgr->register_creature_script(CN_FIENDISH_IMP, &FiendishImpAI::Create);
	pScriptMgr->register_creature_script(CN_DEMONCHAINS, &DemonChains::Create);
	pScriptMgr->register_creature_script(CN_FPORTAL, &FiendPortal::Create);

	pScriptMgr->register_creature_script(SHADEOFARAN, &ShadeofAranAI::Create);
	pScriptMgr->register_creature_script(WATERELE, &WaterEleAI::Create);
	pScriptMgr->register_creature_script(SHADOWOFARAN, &ShadowofAranAI::Create);

	pScriptMgr->register_creature_script(CN_NETHERSPITE, &NetherspiteAI::Create);
	pScriptMgr->register_creature_script(CN_VOIDZONE, &VoidZoneAI::Create);

	pScriptMgr->register_creature_script(CN_MALCHEZAAR, &MalchezaarAI::Create);
	pScriptMgr->register_creature_script(CN_INFERNAL, &NetherInfernalAI::Create);
	pScriptMgr->register_creature_script(CN_DUMMY, &InfernalDummyAI::Create);
	pScriptMgr->register_creature_script(CN_AXES, &MAxesAI::Create);

	pScriptMgr->register_creature_script(CN_NIGHTBANE, &NightbaneAI::Create);
}
