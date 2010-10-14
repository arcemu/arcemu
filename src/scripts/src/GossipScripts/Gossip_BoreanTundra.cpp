#include "Setup.h"

class TiareGossipScript : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		Menu->AddItem(0, "Teleport me to Amber Ledge!", 1); 
		Menu->SendTo(Plr);
	}

	void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * EnteredCode)
    {
		Creature* Tiare = TO_CREATURE((pObject));		
		plr->Gossip_Complete();
		Tiare->CastSpell(plr, dbcSpell.LookupEntry(50135), true);
	}			

};

void SetupBoreanTundraGossip(ScriptMgr * mgr)
{
	GossipScript * TiareGossip = new TiareGossipScript;
	mgr->register_gossip_script(30051, TiareGossip);		// Tiare
}