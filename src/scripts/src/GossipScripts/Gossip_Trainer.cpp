/*
 * Moon++ Scripts for Ascent MMORPG Server
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

class MasterHammersmith : public Arcemu::Gossip::Script
{
	public:
		void OnHello(Object* pObject, Player* plr)
		{
			Arcemu::Gossip::Menu menu(pObject->GetGUID(), 7245);
			menu.AddItem(Arcemu::Gossip::ICON_TRAINER, "Please teach me how to become a hammersmith, Lilith.", 1);
			menu.AddItem(Arcemu::Gossip::ICON_TRAINER, "I wish to unlearn Hammersmithing!", 2);

			menu.Send(plr);
		}

		void OnSelectOption(Object* pObject, Player* plr, uint32 Id, const char* Code)
		{
			uint32 textid = 0;
			if(1 == Id)
			{
				if(!plr->_HasSkillLine(164) || plr->_GetSkillLineCurrent(164, false) < 300)
					textid = 20001;
				else if(!plr->HasSpell(9787))
					textid = 20002;
				else if(plr->HasSpell(17040))
					textid = 20003;
				else if(plr->HasSpell(17041) || plr->HasSpell(17039) || plr->HasSpell(9788))
					textid = 20004;
				else
				{
					if(!plr->HasGold(600))
						textid = 20005;
					else
					{
						//pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Make good use of this knowledge." );
						textid = 20006;
						TO_CREATURE(pObject)->CastSpell(plr, 39099, true);
						plr->ModGold(-600);
					}
				}
			}
			else
			{
				if(!plr->HasSpell(17040))
					textid = 20007;

				else if((!plr->HasGold(250000) && plr->getLevel() <= 50) || (!plr->HasGold(500000) && plr->getLevel() > 50 && plr->getLevel() <= 65)
				        || (!plr->HasGold(1000000) && plr->getLevel() > 65))
					textid = 20008;

				else
				{
					int32 unlearnGold;
					if(plr->getLevel() <= 50)
						unlearnGold = 250000;
					if(plr->getLevel() > 50 && plr->getLevel() <= 65)
						unlearnGold = 500000;
					if(plr->getLevel() > 65)
						unlearnGold = 1000000;

					plr->ModGold(-unlearnGold);
					plr->removeSpell(17040, false, false, 0);
					textid = 20009;
				}
			}
			Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), textid, plr);
		}

		void Destroy() { delete this; }

};

class MasterSwordsmith : public Arcemu::Gossip::Script
{
	public:
		void OnHello(Object* pObject, Player* plr)
		{
			Arcemu::Gossip::Menu menu(pObject->GetGUID(), 7247);
			menu.AddItem(Arcemu::Gossip::ICON_TRAINER, "Please teach me how to become a swordsmith, Seril.", 1);
			menu.AddItem(Arcemu::Gossip::ICON_TRAINER, "I wish to unlearn Swordsmithing!", 2);
			menu.Send(plr);
		}

		void OnSelectOption(Object* pObject, Player* plr, uint32 Id, const char* Code)
		{
			uint32 textid = 0;
			if(1 == Id)
			{
				if(!plr->_HasSkillLine(164) || plr->_GetSkillLineCurrent(164, false) < 300)
					textid = 20001;

				else if(!plr->HasSpell(9787))
					textid = 20002;

				else if(plr->HasSpell(17039))
					textid = 20003;

				else if(plr->HasSpell(17041) || plr->HasSpell(17040) || plr->HasSpell(9788))
					textid = 20004;
				else
				{
					if(!plr->HasGold(600))
						textid = 20005;
					else
					{
						//pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Make good use of this knowledge." );
						textid = 20006;
						TO_CREATURE(pObject)->CastSpell(plr, 39097, true);
						plr->ModGold(-600);
					}
				}
			}
			else
			{
				if(!plr->HasSpell(17039))
					textid = 20007;

				else if((!plr->HasGold(250000) && plr->getLevel() <= 50) || (!plr->HasGold(500000) && plr->getLevel() > 50 && plr->getLevel() <= 65)
				        || (!plr->HasGold(1000000) && plr->getLevel() > 65))
					textid = 20008;
				else
				{
					int32 unlearnGold;
					if(plr->getLevel() <= 50)
						unlearnGold = 250000;
					if(plr->getLevel() > 50 && plr->getLevel() <= 65)
						unlearnGold = 500000;
					if(plr->getLevel() > 65)
						unlearnGold = 1000000;

					plr->ModGold(-unlearnGold);
					plr->removeSpell(17039, false, false, 0);
					textid = 20009;
				}
			}
			Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), textid, plr);
		}

};

class MasterAxesmith : public Arcemu::Gossip::Script
{
	public:
		void OnHello(Object* pObject, Player* plr)
		{
			Arcemu::Gossip::Menu menu(pObject->GetGUID(), 7243);
			menu.AddItem(Arcemu::Gossip::ICON_TRAINER, "Please teach me how to become a axesmith, Kilram.", 1);
			menu.AddItem(Arcemu::Gossip::ICON_TRAINER, "I wish to unlearn Axesmithing!", 2);
			menu.Send(plr);
		}

		void OnSelectOption(Object* pObject, Player* plr, uint32 Id, const char* Code)
		{
			uint32 textid = 0;
			if(1 == Id)
			{
				if(!plr->_HasSkillLine(164) || plr->_GetSkillLineCurrent(164, false) < 300)
					textid = 20001;

				else if(!plr->HasSpell(9787))
					textid = 20002;

				else if(plr->HasSpell(17041))
					textid = 20003;

				else if(plr->HasSpell(17039) || plr->HasSpell(17040) || plr->HasSpell(9788))
					textid = 20004;
				else
				{
					if(!plr->HasGold(600))
						textid = 20005;

					else
					{
						//pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Make good use of this knowledge." );
						textid = 20006;
						TO_CREATURE(pObject)->CastSpell(plr, 39098, true);
						plr->ModGold(-600);
					}
				}
			}
			else
			{
				if(!plr->HasSpell(17041))
					textid = 20007;

				else if((!plr->HasGold(250000) && plr->getLevel() <= 50) || (!plr->HasGold(500000) && plr->getLevel() > 50 && plr->getLevel() <= 65)
				        || (!plr->HasGold(1000000) && plr->getLevel() > 65))
					textid = 20008;
				else
				{
					int32 unlearnGold;
					if(plr->getLevel() <= 50)
						unlearnGold = 250000;
					if(plr->getLevel() > 50 && plr->getLevel() <= 65)
						unlearnGold = 500000;
					if(plr->getLevel() > 65)
						unlearnGold = 1000000;

					plr->ModGold(-unlearnGold);
					plr->removeSpell(17041, false, false, 0);
					textid = 20009;
				}
			}
			Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), textid, plr);
		}

};

void SetupTrainerScript(ScriptMgr* mgr)
{
	mgr->register_creature_gossip(11191, new MasterHammersmith);		// Lilith the Lithe
	mgr->register_creature_gossip(11193, new MasterSwordsmith);		// Seril Scourgebane
	mgr->register_creature_gossip(11192, new MasterAxesmith);			// Kilram
}