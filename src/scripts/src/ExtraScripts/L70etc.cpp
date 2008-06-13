//////////////////////////////////////////////////////
/* Original written by Scarface						//
 *													//
 * This script can be easily edited to be			//
 * used for your server. Throughout the script		//
 * are comments on what each thing does so that		//
 * you can edit this. You can create custom			//
 * teleporting locations and menus.					//
 *													//
 * Originally written for use with Moon++			//
 */													//
//													//
//////////////////////////////////////////////////////
// REDONE BY ROFLWAFFLES (zerohero)

#include "StdAfx.h"
#include "Setup.h"


// --- L70etc band members ---
#define SAMURO			23625
#define BERGRISST		23619
#define MAIKYL			23624
#define SIGNICIOUS		23626
#define THUNDERSKINS	23623
#define TARGETGROUND	48000
#define TARGETAIR		48001
#define UNDEAD			48002
#define UNDEAD2			48003
#define UNDEAD3			48004
#define TRIGGER			48005

/*
 * EMOTE_ONESHOT_CUSTOMSPELL01 = 402,
 * EMOTE_ONESHOT_CUSTOMSPELL02 = 403,
 * EMOTE_ONESHOT_CUSTOMSPELL03 = 404,
 * EMOTE_ONESHOT_CUSTOMSPELL04 = 405,
 * EMOTE_ONESHOT_CUSTOMSPELL05 = 406,
 * EMOTE_ONESHOT_CUSTOMSPELL06 = 407,
 8/


// Spells
#define SPELLFLARE			42505
#define SPELLFIRE			42501
#define SPOTLIGHT			39312
#define SPELLEARTH			42499
#define SPELLLLIGHTNING		42510
#define SPELLLLIGHTNING2	42507
#define SPELLSTORM			42500
#define CONSECRATION		26573
//Consecration
#define SINGERSLIGHT		42510

class SamAI : public CreatureAIScript{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(SamAI);
		SamAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_melee = true;
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFLARE), true);
			timer = 0;
			RegisterAIUpdateEvent(1000);
		}
	void OnCombatStart(Unit* mTarget){ RegisterAIUpdateEvent(1000); }
	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}
	void OnDied(Unit * mKiller){ RemoveAIUpdateEvent(); }
	void OnSpawn(){_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFLARE), true);}
	void OnDespawn(){ RemoveAIUpdateEvent(); }
	void AIUpdate()
	{
		switch (timer)
		{
			case 1:  _unit->PlaySoundToSet(11803); break;
			case 2:  _unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFLARE), true); break;
			case 19:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL05); break;
			case 30:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL03); break;
			case 65:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL04); break;
			case 70:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL03); break;
			case 84:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL06); break;
			case 112: _unit->CastSpell(_unit, dbcSpell.LookupEntry(SINGERSLIGHT), true); break;
			case 123: _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL03); break;
			case 137: _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL06); break;
			case 142: _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL01); break;
			case 180: _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL01); break;
			case 229: _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL02); break;
			case 239: _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL06); break;
			case 259: _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL06); break;
			case 279: _unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFLARE), true); break;
			case 280: _unit->Despawn(1000,300000); break;
		}
		timer++;
	}
	protected:
		uint32 timer;
};

class BerAI : public CreatureAIScript{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(BerAI);
		BerAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_melee = true;
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFLARE), true);
			timer = 0;
			RegisterAIUpdateEvent(1000);
	}
	void OnCombatStart(Unit* mTarget){ RegisterAIUpdateEvent(1000); }
	void OnCombatStop(Unit *mTarget){
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
	}
	void OnDied(Unit * mKiller){ RemoveAIUpdateEvent(); }
	void OnSpawn(){ _unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFLARE), true); }
	void OnDespawn(){ RemoveAIUpdateEvent(); }
	void AIUpdate(){
			switch (timer){
						case 0: _unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFLARE), true); break;
			  case 10:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL03); break;
			  case 30:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL04); break;
			  case 34:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL04); break;
			  case 38:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL03); break;
			  case 104: _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL01); break;
			  case 123: _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL01); break;
			  case 140: _unit->CastSpell(_unit, dbcSpell.LookupEntry(SPOTLIGHT), true); break;
			  case 145: _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL03); break;
			  case 168: _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL01); break;
			  case 229: _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL02); break;
			  case 230: _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL06); break;
			  case 279: _unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFLARE), true); break;
			  case 280: _unit->Despawn(1000,300050); break;
			}
			timer++;
	}
  protected:
	uint32 timer;
};

class SigAI : public CreatureAIScript{
  public:
	ADD_CREATURE_FACTORY_FUNCTION(SigAI);
	SigAI(Creature* pCreature) : CreatureAIScript(pCreature){
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_melee = true;
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			timer = 0;
			RegisterAIUpdateEvent(1000);
	}
	void OnCombatStart(Unit* mTarget){ RegisterAIUpdateEvent(1000); }
	void OnCombatStop(Unit *mTarget){
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
	}
	void OnDied(Unit * mKiller){ RemoveAIUpdateEvent(); }
	void OnSpawn(){ _unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFLARE), true); }
	void OnDespawn(){ RemoveAIUpdateEvent(); }
	void AIUpdate(){
			switch (timer){
							case 0: _unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFLARE), true); break;
			case 10:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL03); break;
			case 30:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL04); break;
			case 34:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL04); break;
			case 38:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL03); break;
			case 70:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL01); break;
			case 85:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL01); break;
			case 123: _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL01); break;
			case 140: _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL01); break;
			case 165: _unit->CastSpell(_unit, dbcSpell.LookupEntry(SPOTLIGHT), true); break;
			case 166: _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL02); break;
			case 168: _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL03); break;
			case 180: _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL03); break;
			case 193: _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL04); break;
			case 229: _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL06); break;
			case 259: _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL04); break;
			case 279: _unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFLARE), true); break;
			case 280: _unit->Despawn(1000,300050); break;
			}
			timer++;
	}
	protected:
			uint32 timer;
};

class MaiAI : public CreatureAIScript{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(MaiAI);
		MaiAI(Creature* pCreature) : CreatureAIScript(pCreature){
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_melee = true;
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
				timer = 0;
			RegisterAIUpdateEvent(1000);
	}
	void OnCombatStart(Unit* mTarget){ RegisterAIUpdateEvent(1000); }
	void OnCombatStop(Unit *mTarget){
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
	}
	void OnDied(Unit * mKiller){ RemoveAIUpdateEvent(); }
	void OnSpawn(){ _unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFLARE), true); }
	void OnDespawn(){ RemoveAIUpdateEvent(); }
	void AIUpdate(){
			switch (timer){
							case 0: _unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFLARE), true); break;
			  case 10:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL03); break;
			  case 30:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL03); break;
			  case 45:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL04); break;
			  case 70:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL03); break;
			  case 85:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL01); break;
			  case 95:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL01); break;
			  case 102: _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL01); break;
			  case 115: _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL01); break;
			  case 123: _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL02); break;
			  case 165: _unit->CastSpell(_unit, dbcSpell.LookupEntry(SPOTLIGHT), true); break;
			  case 192: _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL03); break;
			  case 203: _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL03); break;
			  case 229: _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL04); break;
			  case 279: _unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFLARE), true); break;
			  case 280: _unit->Despawn(1000,300050); break;
			}
			timer++;
	}
  protected:
	uint32 timer;
};


class ThuAI : public CreatureAIScript{
  public:
	ADD_CREATURE_FACTORY_FUNCTION(ThuAI);
	ThuAI(Creature* pCreature) : CreatureAIScript(pCreature){
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_melee = true;
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			timer = 0;
			RegisterAIUpdateEvent(1000);
	}
	void OnCombatStart(Unit* mTarget){ RegisterAIUpdateEvent(1000); }
	void OnCombatStop(Unit *mTarget){
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
	}
	void OnDied(Unit * mKiller){ RemoveAIUpdateEvent(); }
	void OnSpawn(){ _unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFLARE), true); }
	void OnDespawn(){
			RemoveAIUpdateEvent();
			_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFLARE), true);
	}
	void AIUpdate(){
			switch (timer){
							case 2: _unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFLARE), true); break;
							case 3:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL04); break;
			  case 14:  _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "ARE YOU READY TO ROCK?!?!");
			  case 17:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL04); break;
			  case 42:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL04); break;
			  case 55:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL03); break;
			  case 62:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL04); break;
			  case 63:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL04); break;
			  case 64:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL04); break;
			  case 75:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL04); break;
			  case 76:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL04); break;
			  case 77:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL04); break;
			  case 88:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL04); break;
			  case 99:  _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL04); break;
			  case 110: _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL04); break;
			  case 137: _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL02); break;
			  case 140: _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL04); break;
			  case 142: _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "WERE GANA ROCK YOU CRAAAAAAZY!!!");
			  case 313: _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL04); break;
			  case 194: _unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL04); break;
			  case 200:{
				_unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL01);
				_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 401);
				break;
			  }
			  case 279: _unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFLARE), true); break;
			  case 281: _unit->Despawn(1000,301000); break;
			}
			timer++;
	}
  protected:
	uint32 timer;
};



class UndeadAI : public CreatureAIScript{
  public:
	ADD_CREATURE_FACTORY_FUNCTION(UndeadAI);
	UndeadAI(Creature* pCreature) : CreatureAIScript(pCreature){
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_melee = true;
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			timer = 0;
			RegisterAIUpdateEvent(1000);
	}
	void OnCombatStart(Unit* mTarget){ RegisterAIUpdateEvent(1000); }
	void OnCombatStop(Unit *mTarget){
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
	}
	void OnDied(Unit * mKiller){ RemoveAIUpdateEvent(); }
	void OnSpawn(){ _unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFLARE), true); }
	void OnDespawn(){
			RemoveAIUpdateEvent();
			_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFLARE), true);
	}
	void AIUpdate(){
			switch (timer){
							case 2:  _unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_DANCE); break;
							case 280: _unit->Emote(EMOTE_ONESHOT_APPLAUD); break;
							case 281: _unit->Despawn(1000,301000); break;
				}
			timer++;
	}
	protected:
			uint32 timer;
};


class Undead2AI : public CreatureAIScript{
  public:
	ADD_CREATURE_FACTORY_FUNCTION(Undead2AI);
	Undead2AI(Creature* pCreature) : CreatureAIScript(pCreature){
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_melee = true;
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			timer = 0;
			RegisterAIUpdateEvent(1000);
	}
	void OnCombatStart(Unit* mTarget){ RegisterAIUpdateEvent(1000); }
	void OnCombatStop(Unit *mTarget){
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
	}
	void OnDied(Unit * mKiller){ RemoveAIUpdateEvent(); }
	void OnSpawn(){ _unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFLARE), true); }
	void OnDespawn(){
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "THAT WAS GREAT!");
			RemoveAIUpdateEvent();
			_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFLARE), true);
	}
	void AIUpdate(){
			switch (timer){
			  case 2:  _unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_DANCE); break;
							case 280: _unit->Emote(EMOTE_ONESHOT_CHEER); break;
			  case 281: _unit->Despawn(1000,301000); break;
			}
			timer++;
	}
	protected:
			uint32 timer;
};


class Undead3AI : public CreatureAIScript{
  public:
	ADD_CREATURE_FACTORY_FUNCTION(Undead3AI);
	Undead3AI(Creature* pCreature) : CreatureAIScript(pCreature){
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_melee = true;
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			timer = 0;
			RegisterAIUpdateEvent(1000);
	}
	void OnCombatStart(Unit* mTarget){ RegisterAIUpdateEvent(1000); }
	void OnCombatStop(Unit *mTarget){
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
	}
	void OnDied(Unit * mKiller){ RemoveAIUpdateEvent(); }
	void OnSpawn(){ _unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFLARE), true); }
	void OnDespawn(){
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "THAT WAS GREAT!");
			RemoveAIUpdateEvent();
			_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFLARE), true);
	}
	void AIUpdate(){
			switch (timer){
			  case 2:  _unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_DANCE); break;
							case 279: _unit->Emote(EMOTE_ONESHOT_CHEER); break;
							case 280: _unit->Despawn(1000,301000); break;
			}
			timer++;
	}
	protected:
			uint32 timer;
};


class TriggerAI : public CreatureAIScript{
  public:
	ADD_CREATURE_FACTORY_FUNCTION(TriggerAI);
	TriggerAI(Creature* pCreature) : CreatureAIScript(pCreature){
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_melee = true;
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			timer = 0;
			RegisterAIUpdateEvent(1000);
	}
	void OnCombatStart(Unit* mTarget){ RegisterAIUpdateEvent(1000); }
	void OnCombatStop(Unit *mTarget){
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
	}
	void OnDied(Unit * mKiller){ RemoveAIUpdateEvent(); }
	void OnSpawn(){ _unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFLARE), true); }
	void OnDespawn(){
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "THAT WAS GREAT!");

					RemoveAIUpdateEvent();
			_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFLARE), true);
	}
	void AIUpdate(){
			switch (timer){
							case 1: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			  case 8: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 15: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 21: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 28: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 35: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 41: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 48: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 55: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			  case 62: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 69: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 76: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 81: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 89: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 96: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 101: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			  case 108: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 115: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 121: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 128: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 135: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 141: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 148: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 155: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			  case 162: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 169: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 176: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 181: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 189: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 196: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 201: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 208: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 215: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 221: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 228: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 235: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 241: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 248: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 255: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			  case 262: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 269: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
							case 276: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			  case 281: _unit->Despawn(1000,301000); break;
			}
			timer++;
	}
	protected:
			uint32 timer;
};

class Trigger2AI : public CreatureAIScript{
  public:
	ADD_CREATURE_FACTORY_FUNCTION(Trigger2AI);
	Trigger2AI(Creature* pCreature) : CreatureAIScript(pCreature){
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_melee = true;
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			timer = 0;
			RegisterAIUpdateEvent(1000);
	}
	void OnCombatStart(Unit* mTarget){ RegisterAIUpdateEvent(1000); }
	void OnCombatStop(Unit *mTarget){
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
	}
	void OnDied(Unit * mKiller){ RemoveAIUpdateEvent(); }
	void OnSpawn(){ _unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFLARE), true); }
	void OnDespawn()
	{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "THAT WAS GREAT!");
			RemoveAIUpdateEvent();
			_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFLARE), true);
	}
	void AIUpdate()
	{
		switch (timer)
		{
			case 3: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 10: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 18: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 24: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 22: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 38: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 44: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 52: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 58: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 68: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 69: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 76: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 85: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 90: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 96: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 107: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 109: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 125: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 127: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 129: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 132: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 144: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 149: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 159: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 166: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 169: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 176: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 183: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 186: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 194: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 204: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 209: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 218: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 223: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 228: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 235: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 241: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 248: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 252: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 263: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 266: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 274: _unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSECRATION), true); break;
			case 281: _unit->Despawn(1000,301000); break;
		}
		timer++;
	}
protected:
	uint32 timer;
};

class Effectsground : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(Effectsground);
	Effectsground(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->GetAIInterface()->m_canMove = false;
		_unit->GetAIInterface()->disable_melee = true;
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		timer = 0;
		RegisterAIUpdateEvent(1000);
	}
	void OnCombatStart(Unit* mTarget)
	{
		RegisterAIUpdateEvent(1000);
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
	void OnSpawn()
	{
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		timer = 0;
	}
	void OnDespawn()
	{
		RemoveAIUpdateEvent();
	}
	void AIUpdate()
	{
		switch (timer)
		{
			case 2:
			{
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLLLIGHTNING), true);
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLLLIGHTNING2), true);
			}break;
			case 6:
			{
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLEARTH), true);
			}break;
			case 8:
			{
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFIRE), true);
			}break;
			case 72:
			{
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLLLIGHTNING), true);
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLLLIGHTNING2), true);
			}break;
			case 76:
			{
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLEARTH), true);
			}break;
			case 78:
			{
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFIRE), true);
			}break;
			case 125:
			{
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLLLIGHTNING), true);
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLLLIGHTNING2), true);
			}break;
			case 128:
			{
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLEARTH), true);
			}break;
			case 132:
			{
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFIRE), true);
			}break;
			case 232:
			{
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLLLIGHTNING), true);
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLLLIGHTNING2), true);
			}break;
			case 236:
			{
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLEARTH), true);
			}break;
			case 238:
			{
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFIRE), true);
			}break;
			case 245:
			{
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLLLIGHTNING), true);
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLLLIGHTNING2), true);
			}break;
			case 249:
			{
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLEARTH), true);
			}break;
			case 251:
			{
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFIRE), true);
			}break;
			case 279:
			{
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLFLARE), true);
			}break;
			case 280:
			{
				_unit->Despawn(1000,300000);
			}break;
			timer++;
	}
protected:
	uint32 timer;
};

class Effectsair : public CreatureAIScript{
public:
	ADD_CREATURE_FACTORY_FUNCTION(Effectsair);
	Effectsair(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->GetAIInterface()->m_canMove = false;
		_unit->GetAIInterface()->disable_melee = true;
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		timer = 0;
		RegisterAIUpdateEvent(1000);
	}
	void OnCombatStart(Unit* mTarget){ RegisterAIUpdateEvent(1000); }
	void OnCombatStop(Unit *mTarget){
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
	}
	void OnDied(Unit * mKiller){ RemoveAIUpdateEvent(); }
	void OnSpawn(){
			timer = 0;
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
	}
	void OnDespawn()
	{
		RemoveAIUpdateEvent();
	}
	void AIUpdate()
	{
		switch(timer)
		{
			case 1:
			{
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLSTORM), true);
			}break;
			case 70:
			{
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLSTORM), true);
			}break;
			case 123:
			{
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLSTORM), true);
			}break;
			case 230:
			{
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLSTORM), true);
			}break;
			case 243:
			{
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELLSTORM), true);
			}break;
			case 280:
			{
				_unit->Despawn(1000,300000);
			}break;
		}
		timer++;
	}
protected:
	uint32 timer;
};

void SetupL70etc(ScriptMgr * mgr)
{
	mgr->register_creature_script(SAMURO, &SamAI::Create);
	mgr->register_creature_script(BERGRISST, &BerAI::Create);
	mgr->register_creature_script(MAIKYL, &MaiAI::Create);
	mgr->register_creature_script(SIGNICIOUS, &SigAI::Create);
	mgr->register_creature_script(THUNDERSKINS, &ThuAI::Create);
	mgr->register_creature_script(TARGETGROUND, Effectsground::Create);
	mgr->register_creature_script(TARGETAIR, Effectsair::Create);
	mgr->register_creature_script(UNDEAD, &UndeadAI::Create);
	mgr->register_creature_script(UNDEAD2, &Undead2AI::Create);
	mgr->register_creature_script(UNDEAD3, &Undead3AI::Create);
	mgr->register_creature_script(TRIGGER, &TriggerAI::Create);
}
*/
