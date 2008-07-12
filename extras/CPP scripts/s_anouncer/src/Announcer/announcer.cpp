#include "StdAfx.h"
#include "Setup.h"

#define ANNOUNCE_INTERVAL					5000		//X milliseconds
#define ANNOUNCE_RANGE_SQR					(200*200)	//players in range of the anouncer will hear announcement
#define MSG_LANG							0 
#define UNIT_ENTRY_FOR_SPAWN_AS_ANNOUNCER	644
#define MSG_TO_ANNOUNCE						"The owner forgot to change me when compiled me"
#define MSG_TYPE							CHAT_MSG_WHISPER

/*
	CHAT_MSG_ADDON									= -1,
	CHAT_MSG_SYSTEM                                 = 0,    //28,   CHAT_MSG_SYSTEM                 = 0x00,         0
	CHAT_MSG_SAY									= 1,
	CHAT_MSG_PARTY									= 2,
	CHAT_MSG_RAID									= 3,
	CHAT_MSG_GUILD									= 4,
	CHAT_MSG_OFFICER								= 5,
	CHAT_MSG_YELL									= 6,
	CHAT_MSG_WHISPER								= 7,
	CHAT_MSG_WHISPER_MOB							= 8,//CHAT_MSG_WHISPER_INFORM
	CHAT_MSG_WHISPER_INFORM							= 9,//CHAT_MSG_REPLY
	CHAT_MSG_EMOTE									= 10,
	CHAT_MSG_TEXT_EMOTE								= 11,
	CHAT_MSG_MONSTER_SAY							= 12,
	CHAT_MSG_MONSTER_PARTY							= 13,
	CHAT_MSG_MONSTER_YELL							= 14,
	CHAT_MSG_MONSTER_WHISPER						= 15,
	CHAT_MSG_MONSTER_EMOTE							= 16,
	CHAT_MSG_CHANNEL								= 17,
	CHAT_MSG_CHANNEL_JOIN							= 18,
	CHAT_MSG_CHANNEL_LEAVE							= 19,
	CHAT_MSG_CHANNEL_LIST							= 20,
	CHAT_MSG_CHANNEL_NOTICE							= 21,
	CHAT_MSG_CHANNEL_NOTICE_USER					= 22,
	CHAT_MSG_AFK									= 23,
	CHAT_MSG_DND									= 24,
	CHAT_MSG_IGNORED								= 25,
	CHAT_MSG_SKILL									= 26,
	CHAT_MSG_LOOT									= 27,
    CHAT_MSG_MONEY                                  = 28,
    CHAT_MSG_OPENING                                = 29,
    CHAT_MSG_TRADESKILLS                            = 30,
    CHAT_MSG_PET_INFO                               = 31,
    CHAT_MSG_COMBAT_MISC_INFO                       = 32,
    CHAT_MSG_COMBAT_XP_GAIN                         = 33,
    CHAT_MSG_COMBAT_HONOR_GAIN                      = 34,
    CHAT_MSG_COMBAT_FACTION_CHANGE                  = 35,
	CHAT_MSG_BG_EVENT_NEUTRAL						= 36,
	CHAT_MSG_BG_EVENT_ALLIANCE						= 37,
	CHAT_MSG_BG_EVENT_HORDE							= 38,
	CHAT_MSG_RAID_LEADER							= 39,
	CHAT_MSG_RAID_WARNING							= 40,
	CHAT_MSG_RAID_WARNING_WIDESCREEN				= 41,
    CHAT_MSG_RAID_BOSS_EMOTE                        = 42,
	CHAT_MSG_FILTERED								= 43,
	CHAT_MSG_BATTLEGROUND							= 44,
	CHAT_MSG_BATTLEGROUND_LEADER					= 45,
	CHAT_MSG_RESTRICTED								= 46,	
*/

bool IsInrangeSQ( Object *t1, Object *t2,float sq_r)
{
	if( !t1 || !t2 )
		return false;

	float distx=t1->GetPositionX()-t2->GetPositionX();
	float disty=t1->GetPositionY()-t2->GetPositionY();
	float dist = distx*distx + disty*disty; 
	return dist <= sq_r;
}


class Announcer : public CreatureAIScript
{
public:

	//constructor
    Announcer(Creature* pCreature) : CreatureAIScript(pCreature) 
	{
		RegisterAIUpdateEvent( ANNOUNCE_INTERVAL );
		_unit->SetUInt32Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NOT_ATTACKABLE_2 );
	}

	//this will be called with ANNOUNCE_INTERVAL interval
	void AIUpdate()
	{
		MapMgr *mgr = _unit->GetMapMgr();
		if( mgr )
		{
			PlayerStorageMap::iterator itr;
			WorldPacket *data;
			for( itr = mgr->m_PlayerStorage.begin(); itr!=mgr->m_PlayerStorage.end();itr++)
			{
				if( itr->second //can't imagine this getting to null though :P
					&& itr->second->isAlive() 
					&& itr->second->IsInWorld()
					&& IsInrangeSQ( _unit, itr->second, ANNOUNCE_RANGE_SQR)
					)
				{
					//normal monster yell but only if plaer is in visual range
//					_unit->SendChatMessageToPlayer( CHAT_MSG_MONSTER_SAY , MSG_LANG, "just testing4",itr->second);
					//to make text appear on middle of monitor
//					itr->second->SendAreaTriggerMessage("just testing5");
					//no sender jsut the message
//					itr->second->BroadcastMessage("just testing6");
					//can have multiple message types like self wisper :D
					data = sChatHandler.FillMessageData(MSG_TYPE, LANG_UNIVERSAL, MSG_TO_ANNOUNCE, itr->second->GetGUID(), itr->second->bGMTagOn ? 4 : 0  );
					itr->second->GetSession()->SendPacket(data);
				}
			}

		}
	}

	//create an anouncer object for this creature
    static CreatureAIScript *Create(Creature * c) { return new Announcer(c); }

	//destroy the anouncer object
    void Destroy() { delete (Announcer*)this; }

private:
};

void SetupAnnouncer(ScriptMgr * mgr)
{
	//this is to register the anouncer object to our creatures
    mgr->register_creature_script(UNIT_ENTRY_FOR_SPAWN_AS_ANNOUNCER, &Announcer::Create);
}
