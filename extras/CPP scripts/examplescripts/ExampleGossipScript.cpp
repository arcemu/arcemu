#include "StdAfx.h"
#include "Setup.h"

#define MY_NPC_ID 1
#define INITIAL_NPC_TEXT 1  // The text the NPC starts as saying

class SCRIPT_DECL FirstGossipScript : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player * plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), INITIAL_NPC_TEXT, plr);
		
		Menu->AddItem(0, "Beam me up, Scotty!", 1);

		if(AutoSend)
			Menu->SendTo(plr);
	}

	void GossipSelectOption(Object* pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
	{
		switch(IntId)
		{
			case 1:
			{
				plr->SafeTeleport(530, 5.0f, 4.0f, 3.0f); // map, x, y, z

				static_cast<Creature*>(pObject)->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Aye, sir!");
			} break;
		}
	}

	void Destroy()
	{
		delete this;
	}
}

// You will need to call SetupExample(mgr); in Setup.h
void SetupExample(ScriptMgr * mgr)
{
	GossipScript * myGossipScript = (GossipScript*)new FirstGossipScript;
	mgr->register_gossip_script(MY_NPC_ID, myGossipScript);
}