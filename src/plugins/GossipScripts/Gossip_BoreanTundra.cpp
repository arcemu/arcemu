#include "Setup.h"

class TiareGossipScript : public Arcemu::Gossip::Script
{
	public:
		void OnHello(Object* pObject, Player* Plr)
		{
			Arcemu::Gossip::Menu::SendQuickMenu(pObject->GetGUID(), 1, Plr, 1, 0, "Teleport me to Amber Ledge!");
		}

		void OnSelectOption(Object* pObject, Player* plr, uint32 Id, const char* EnteredCode)
		{
			Arcemu::Gossip::Menu::Complete(plr);
			TO_CREATURE(pObject)->CastSpell(plr, dbcSpell.LookupEntry(50135), true);
		}
		void Destroy() { delete this; }

};

void SetupBoreanTundraGossip(ScriptMgr* mgr)
{
	mgr->register_creature_gossip(30051, new TiareGossipScript);		// Tiare
}