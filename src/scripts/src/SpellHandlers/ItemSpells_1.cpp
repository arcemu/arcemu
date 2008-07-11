/****************************************************************************
 *
 * SpellHandler Plugin
 * Copyright (c) 2007 Team Ascent
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter to Creative Commons,
 * 543 Howard Street, 5th Floor, San Francisco, California, 94105, USA.
 *
 * EXCEPT TO THE EXTENT REQUIRED BY APPLICABLE LAW, IN NO EVENT WILL LICENSOR BE LIABLE TO YOU
 * ON ANY LEGAL THEORY FOR ANY SPECIAL, INCIDENTAL, CONSEQUENTIAL, PUNITIVE OR EXEMPLARY DAMAGES
 * ARISING OUT OF THIS LICENSE OR THE USE OF THE WORK, EVEN IF LICENSOR HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 *
 */

#include "StdAfx.h"
#include "Setup.h"
#include "../EAS/EasyFunctions.h"

/*
	How to add a new item spell to the dummy spell handler:

		1) Add a new function to handle the spell at the end of this file but before the
		   SetupItemSpells_1() function. SetupItemSpells_1() must always be the last function.

		2) Register the dummy spell by adding a new line to the end of the list in the
		   SetupItemSpells_1() function.

	Please look at how the other spells are handled and try to use the
	same variable names and formatting style in your new spell handler.
*/

// *****************************************************************************

bool GnomishTransporter(uint32 i, Spell * pSpell)
{
	if(!pSpell->p_caster) return true;

	pSpell->p_caster->EventAttackStop();
	pSpell->p_caster->SafeTeleport(1, 0, LocationVector(-7169.41f, -3838.63f, 8.72f));
	return true;
}

// -----------------------------------------------------------------------------

bool NoggenFoggerElixr(uint32 i, Spell * pSpell)
{
	if(!pSpell->p_caster) return true;

	uint32 chance = RandomUInt(2);

	switch(chance)
	{
	case 0:
		pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(16591), true);
		break;
	case 1:
		pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(16593), true);
		break;
	case 2:
		pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(16595), true);
		break;
	}
	return true;
}

// -----------------------------------------------------------------------------

bool HallowsEndCandy(uint32 i, Spell * pSpell)
{
	if(!pSpell->p_caster) return true;

	int newspell = 24924 + RandomUInt(3);

	SpellEntry *spInfo = dbcSpell.LookupEntry(newspell);
	if(!spInfo) return true;

	pSpell->p_caster->CastSpell(pSpell->p_caster, spInfo, true);
	return true;
}

// -----------------------------------------------------------------------------

bool DeviateFish(uint32 i, Spell * pSpell)
{
	if(!pSpell->p_caster) return true;

	int newspell = 8064 + RandomUInt(4);

	SpellEntry *spInfo = dbcSpell.LookupEntry(newspell);
	if(!spInfo) return true;

	pSpell->p_caster->CastSpell(pSpell->p_caster, spInfo, true);
	return true;
}

// -----------------------------------------------------------------------------

bool CookedDeviateFish(uint32 i, Spell * pSpell)
{
	if(!pSpell->p_caster) return true;

	int chance = 0;
	int newspell = 0;

	chance = RandomUInt(1);

	switch(chance)
	{
	case 0:
		newspell = 8219; // Flip Out (60 min) (turns you into a ninja)
		break;
	case 1:
		newspell = 8221; // Yaaarrrr (60 min) (turns you into a pirate)
		break;
	}

	if(newspell)
	{
		SpellEntry *spInfo = dbcSpell.LookupEntry(newspell);
		if(!spInfo) return true;

		pSpell->p_caster->CastSpell(pSpell->p_caster, spInfo, true);
	}
	return true;
}

// -----------------------------------------------------------------------------

bool HolidayCheer(uint32 i, Spell * pSpell)
{
	if(!pSpell->m_caster) return true;

	Unit *target;
	float dist = pSpell->GetRadius(i);

	for(ObjectSet::iterator itr = pSpell->m_caster->GetInRangeSetBegin(); itr != pSpell->m_caster->GetInRangeSetEnd(); ++itr)
	{
		if((*itr)->IsUnit())
			target = (Unit*)*itr;
		else
			continue;

		if(pSpell->m_caster->CalcDistance(target) > dist || isAttackable(pSpell->m_caster, target))
			continue;

		target->Emote(EMOTE_ONESHOT_LAUGH);
	}
	return true;
}

// -----------------------------------------------------------------------------

bool NetOMatic(uint32 i, Spell * pSpell)
{
	Unit *target = pSpell->GetUnitTarget();
	if(!pSpell->p_caster || !target) return true;

	SpellEntry *spInfo = dbcSpell.LookupEntry(13099);
	if(!spInfo) return true;

	int chance = RandomUInt(99)+1;

	if(chance < 51) // nets target: 50%
		pSpell->p_caster->CastSpell(target, spInfo, true);

	else if(chance < 76) // nets you instead: 25%
		pSpell->p_caster->CastSpell(pSpell->p_caster, spInfo, true);

	else // nets you and target: 25%
	{
		pSpell->p_caster->CastSpell(pSpell->p_caster, spInfo, true);
		pSpell->p_caster->CastSpell(target, spInfo, true);
	}
	return true;
}

// -----------------------------------------------------------------------------

bool BanishExile(uint32 i, Spell * pSpell)
{
	Unit* target = pSpell->GetUnitTarget();
	if(!pSpell->p_caster || !target) return true;

	pSpell->p_caster->SpellNonMeleeDamageLog(target, pSpell->GetProto()->Id, target->GetUInt32Value(UNIT_FIELD_HEALTH), true);
	return true;
}

// -----------------------------------------------------------------------------

/*bool ForemansBlackjack(uint32 i, Spell *pSpell)
{
	Unit* target = pSpell->GetUnitTarget();
	if(!pSpell->p_caster || !target || target->GetTypeId() != TYPEID_UNIT) return true;

	// play sound and animation
	WorldPacket data(SMSG_PLAY_OBJECT_SOUND, 12);
	data << uint32(6197) << target->GetGUID();
	pSpell->p_caster->SendMessageToSet(&data, true);

	// send chat message
	char msg[100];
	sprintf(msg, "Ow! Ok, I'll get back to work, %s", pSpell->p_caster->GetName());
	target->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, msg);

	Creature* c_target = (Creature*)target;
	if(!c_target) return true;

	uint32 creatureID = c_target->GetEntry();

  // check to see that we have the correct creature and increment the quest log
	if(creatureID == 10556)
		sQuestMgr.OnPlayerKill(pSpell->p_caster, c_target);

	return true;
} */

// -----------------------------------------------------------------------------

bool NetherWraithBeacon(uint32 i, Spell *pSpell)
{
	if(!pSpell->p_caster) return true;

	float SSX = pSpell->p_caster->GetPositionX();
	float SSY = pSpell->p_caster->GetPositionY();
	float SSZ = pSpell->p_caster->GetPositionZ();
	float SSO = pSpell->p_caster->GetOrientation();

	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(22408, SSX, SSY, SSZ, SSO, true, false, 0, 0);
	return true;
}

// -----------------------------------------------------------------------------

bool SymbolOfLife(uint32 i, Spell *pSpell)
{
	/*  // commented out until the SpellTargetDummy (38) targeting is fixed
	Unit* target = pSpell->GetUnitTarget();
	if(!pSpell->p_caster || !target || target->GetTypeId() != TYPEID_UNIT) return true;

	Creature* c_target = (Creature*)target;
	if(!c_target) return true;

	uint32 creatureID = c_target->GetEntry();

  // check to see we that have the correct creature and increment the quest log
	if(creatureID == 17542 || creatureID == 6172)
		sQuestMgr.OnPlayerKill(pSpell->p_caster, c_target);
	*/
	return true;
}

// -----------------------------------------------------------------------------

bool NighInvulnBelt(uint32 i, Spell *pSpell)
{
	if(!pSpell->p_caster) return true;

	int chance = RandomUInt(99)+1;

	if(chance > 14) // Buff - Nigh-Invulnerability - 30456
		pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(30456), true);
	else // Malfunction - Complete Vulnerability - 30457
		pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(30457), true);

	return true;
}

// -----------------------------------------------------------------------------

bool ReindeerTransformation(uint32 i, Spell *pSpell)
{
	if(!pSpell->p_caster) return true;

	if(pSpell->p_caster->GetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID) != 0)
	{
		if(pSpell->p_caster->m_setflycheat)
			pSpell->p_caster->SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID, 22724);
		else
			pSpell->p_caster->SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID, 15902);
		/*if(pSpell->p_caster->m_flySpeed >= 4.1f)
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(44827), true); //310% flying Reindeer
		else if (pSpell->p_caster->m_flySpeed >= 3.8f)
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(44825), true); //280% flying Reindeer
		else if (pSpell->p_caster->m_flySpeed >= 1.6f)
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(44824), true); //60% flying Reindeer
		else if (pSpell->p_caster->m_walkSpeed >= 2.0f)
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(25859), true); //100% ground Reindeer
		else
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(25858), true); //60% ground Reindeer*/
	}
	return true;
}

// -----------------------------------------------------------------------------

bool SummonCritterDummy(uint32 i, Spell *pSpell)
{
	// the reason these spells have to be scripted is because they require a
	// reagent to summon the critter pet, but don't require one to dismiss it

	if(!pSpell->p_caster) return true;

	uint32 currentCritterID = 0;

	if(pSpell->p_caster->critterPet && pSpell->p_caster->critterPet->GetCreatureName())
		currentCritterID = pSpell->p_caster->critterPet->GetCreatureName()->Id;

	uint32 newspell = 0;

	switch(pSpell->GetProto()->Id)
	{
		case 26469: // Snowman Kit
		{
			if(currentCritterID == 15710) // do we already have this critter summoned?
				newspell = 26468; // if so, dismiss it
			else
				newspell = 26045; // otherwise summon it
		}	break;

		case 26528: // Jingling Bell
		{
			if(currentCritterID == 15706) // do we already have this critter summoned?
				newspell = 26530; // if so, dismiss it
			else
				newspell = 26529; // otherwise summon it
		}	break;

		case 26532: // Green Helper Box
		{
			if(currentCritterID == 15698) // do we already have this critter summoned?
				newspell = 26534; // if so, dismiss it
			else
				newspell = 26533; // otherwise summon it
		}	break;

		case 26541: // Red Helper Box
		{
			if(currentCritterID == 15705) // do we already have this critter summoned?
				newspell = 26537; // if so, dismiss it
			else
				newspell = 26536; // otherwise summon it
		}	break;
	}

	SpellEntry *spInfo = dbcSpell.LookupEntry(newspell);
	if(!spInfo) return true;

	pSpell->p_caster->CastSpell(pSpell->p_caster, spInfo, false); // these spells have to check items, so "triggeredspell" must be false
	return true;
}

// -----------------------------------------------------------------------------

bool WinterWondervolt(uint32 i, Spell * pSpell)
{
	Unit* target = pSpell->GetUnitTarget();

	if(!target || target->GetTypeId() != TYPEID_PLAYER) return true;

	target->CastSpell(target, dbcSpell.LookupEntry(26274), true);

	return true;
}

// -----------------------------------------------------------------------------

bool WinterWondervoltAura(uint32 i, Aura* pAura, bool apply)
{
	Unit *u_caster = pAura->GetUnitCaster();

	if(!u_caster || !u_caster->IsPlayer()) return true;

	if(apply)
	{
		uint32 displayId;
		uint32 chance = RandomUInt(7);

		if(u_caster->getGender() == 1) displayId = 15795 + chance; // female 0-7
		else if(chance == 0)           displayId = 15687;          // male   0
		else                           displayId = 15802 + chance; // male   1-7

		u_caster->SetUInt32Value(UNIT_FIELD_DISPLAYID, displayId);
	}
	else
		u_caster->SetUInt32Value(UNIT_FIELD_DISPLAYID, u_caster->GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID));

	return true;
}

// -----------------------------------------------------------------------------

bool ScryingCrystal(uint32 i, Spell *pSpell)
{
	QuestLogEntry *en = pSpell->p_caster->GetQuestLogForEntry(9824);
	if(pSpell->p_caster->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(pSpell->p_caster->GetPositionX(), 
		pSpell->p_caster->GetPositionY(), pSpell->p_caster->GetPositionZ(), 300078) && en)
	{
		
		if(en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
		{
			en->SetMobCount(0, 1);
			en->SendUpdateAddKill(0);
			en->UpdatePlayerFields();
			return false;
		}
	}
	else if(pSpell->p_caster->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(pSpell->p_caster->GetPositionX(), 
		pSpell->p_caster->GetPositionY(), pSpell->p_caster->GetPositionZ(), 300142) && en)
	{
		if(en->GetMobCount(1) < en->GetQuest()->required_mobcount[1])
		{
			en->SetMobCount(1, 1);
			en->SendUpdateAddKill(1);
			en->UpdatePlayerFields();
			return false;
		}
	}
	return true;
}

bool MinionsOfGurok(uint32 i, Spell *pSpell)
{
	Unit* target = pSpell->GetUnitTarget();
	if(!pSpell->p_caster || !target || target->GetTypeId() != TYPEID_UNIT || target->GetEntry() != 17157) return true;

	((Creature*)target)->Despawn(500, 360000);

	float SSX = target->GetPositionX();
	float SSY = target->GetPositionY();
	float SSZ = target->GetPositionZ();
	float SSO = target->GetOrientation();

	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(18181, SSX+rand()%8-4, SSY+rand()%8-4, SSZ, SSO, true, false, 0, 0);
	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(18181, SSX+rand()%8-4, SSY+rand()%8-4, SSZ, SSO, true, false, 0, 0);
	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(18181, SSX+rand()%8-4, SSY+rand()%8-4, SSZ, SSO, true, false, 0, 0);

	return true;
}

bool PurifyBoarMeat(uint32 i, Spell *pSpell)
{
	uint32 bormeat = RandomUInt(2);
	switch(bormeat)
	{
	case 0:
		{
			pSpell->p_caster->CastSpell(pSpell->p_caster, 29277, true);
		}break;
	case 1:
		{
			pSpell->p_caster->CastSpell(pSpell->p_caster, 29278, true);
		}break;
	}
	
	return true;
}

bool WarpRiftGenerator(uint32 i, Spell * pSpell)
{
	if(!pSpell->p_caster) return true;

	float SSX = pSpell->p_caster->GetPositionX();
	float SSY = pSpell->p_caster->GetPositionY();
	float SSZ = pSpell->p_caster->GetPositionZ();
	float SSO = pSpell->p_caster->GetOrientation();

	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(16939,SSX,SSY,SSZ,SSO,true,false,0,0);

	return true;
}


// *****************************************************************************
//                HALLOWED WAND                                                
// *****************************************************************************

bool HallowedWandNinja(uint32 i, Spell *pSpell)
{
	if(!pSpell->p_caster) return true;
	Unit *target = pSpell->GetUnitTarget();
	if(pSpell->p_caster->getGender() == 1)
	{
		pSpell->p_caster->CastSpell(target, dbcSpell.LookupEntry(24711), true);  //female
	}
	else
	{
		pSpell->p_caster->CastSpell(target, dbcSpell.LookupEntry(24710), true);   //male
	}
	return true;
}

bool HallowedWandPirate(uint32 i, Spell *pSpell)
{
	if(!pSpell->p_caster) return true;
	Unit *target = pSpell->GetUnitTarget();
	if(pSpell->p_caster->getGender() == 1)
	{
		pSpell->p_caster->CastSpell(target, dbcSpell.LookupEntry(24709), true);  //female
	}
	else
	{
		pSpell->p_caster->CastSpell(target, dbcSpell.LookupEntry(24708), true);   //male
	}
	return true;
}

bool HallowedWandLeperGnome(uint32 i, Spell *pSpell)
{
	if(!pSpell->p_caster) return true;
	Unit *target = pSpell->GetUnitTarget();
	if(pSpell->p_caster->getGender() == 1)
	{
		pSpell->p_caster->CastSpell(target, dbcSpell.LookupEntry(24713), true);  //female
	}
	else
	{
		pSpell->p_caster->CastSpell(target, dbcSpell.LookupEntry(24712), true);   //male
	}
	return true;
}

bool HallowedWandGhost(uint32 i, Spell *pSpell)
{
	if(!pSpell->p_caster) return true;
	Unit *target = pSpell->GetUnitTarget();
	if(pSpell->p_caster->getGender() == 1)
	{
		pSpell->p_caster->CastSpell(target, dbcSpell.LookupEntry(24736), true);  //female
	}
	else
	{
		pSpell->p_caster->CastSpell(target, dbcSpell.LookupEntry(24735), true);   //male
	}
	return true;
}

bool JumpTron(uint32 i, Spell *pSpell)
{
	if(!pSpell->p_caster) return true;
	pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(33382), true);
	return true;
}

bool SonicBoom(uint32 i, Spell *pSpell)
{
	if(!pSpell->p_caster) return true;
	pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(33666), true);
	return true;
}

bool BagOfCandies(uint32 i, Spell *pSpell)
{
	if(!pSpell->p_caster) return true;
    Player *plr = (Player*)pSpell->u_caster;

	uint32 item = RandomUInt(7);

	switch(item)
	{
	case 0:
		sEAS.AddItem(21817, plr);
		break;
	case 1:
		sEAS.AddItem(21818, plr);
		break;
	case 2:
		sEAS.AddItem(21819, plr);
		break;
	case 3:
		sEAS.AddItem(21820, plr);
		break;
	case 4:
		sEAS.AddItem(21821, plr);
		break;
	case 5:
		sEAS.AddItem(21822, plr);
		break;
	case 6:
		sEAS.AddItem(21823, plr);
		break;
	}
	return true;
}

bool MirrensDrinkingHat(uint32 i, Spell *pSpell)
{
	if(!pSpell->p_caster) return true;
    Player *plr = (Player*)pSpell->u_caster;

	uint32 item = RandomUInt(2);
	switch(item)
	{
	case 0:
		sEAS.AddItem(23585, plr);
		break;
	case 1:
		sEAS.AddItem(23586, plr);
		break;
	case 2:
		sEAS.AddItem(23584, plr);
		break;
	}
	return true;
}

bool ElunesCandle(uint32 i, Spell *pSpell)
{
	if(!pSpell->p_caster) return true;

	Unit *target = pSpell->GetUnitTarget();
	pSpell->p_caster->CastSpell(target, dbcSpell.LookupEntry(26636), true);
	return true;
}

bool DarkmoonCard_Madness(uint32 i, Spell *pSpell)
{
	Player *caster = pSpell->p_caster;
	if(!pSpell->p_caster) return true;

	if(caster->getClass() == PALADIN || caster->getClass() == DRUID)
	{
		uint32 chance = RandomUInt(8);

		switch(chance)
		{
		case 0:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(39511), true);
			break;
		case 1:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(40997), true);
			break;
		case 2:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(40998), true);
			break;
		case 3:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(40999), true);
			break;
		case 4:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(41002), true);
			break;
		case 5:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(41005), true);
			break;
		case 6:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(41009), true);
			break;
		case 7:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(41011), true);
			break;
		case 8:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(41409), true);
			break;
		}
	}

	if(caster->getClass() == ROGUE || caster->getClass() == WARRIOR)
	{
		uint32 chance = RandomUInt(5);

		switch(chance)
		{
		case 0:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(39511), true);
			break;
		case 1:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(40997), true);
			break;
		case 2:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(40998), true);
			break;
		case 3:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(41002), true);
			break;
		case 4:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(41005), true);
			break;
		case 5:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(41011), true);
			break;
		}
	}

	if(caster->getClass() == PRIEST || caster->getClass() == SHAMAN)
	{
		uint32 chance = RandomUInt(6);

		switch(chance)
		{
		case 0:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(40999), true);
			break;
		case 1:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(41002), true);
			break;
		case 2:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(41005), true);
			break;
		case 3:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(41009), true);
			break;
		case 4:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(41011), true);
			break;
		case 5:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(41406), true);
			break;
		case 6:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(41409), true);
			break;
		}
	}

	if(caster->getClass() == MAGE || caster->getClass() == WARLOCK)
	{
		uint32 chance = RandomUInt(6);

		switch(chance)
		{
		case 0:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(40999), true);
			break;
		case 1:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(41002), true);
			break;
		case 2:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(41005), true);
			break;
		case 3:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(41009), true);
			break;
		case 4:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(41011), true);
			break;
		case 5:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(41406), true);
			break;
		case 6:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(41409), true);
			break;
		}
	}

	if(caster->getClass() == HUNTER)
	{
		uint32 chance = RandomUInt(7);

		switch(chance)
		{
		case 0:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(40997), true);
			break;
		case 1:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(40999), true);
			break;
		case 2:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(41002), true);
			break;
		case 3:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(41005), true);
			break;
		case 4:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(41009), true);
			break;
		case 5:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(41011), true);
			break;
		case 6:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(41406), true);
			break;
		case 7:
			pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(41409), true);
			break;
		}
	}
	return true;
}

// *****************************************************************************
bool LunaclawAll(uint32 i, Spell *pSpell)
{
	if(!pSpell->p_caster) return true;

	float SSX = pSpell->p_caster->GetPositionX();
	float SSY = pSpell->p_caster->GetPositionY();
	float SSZ = pSpell->p_caster->GetPositionZ();
	float SSO = pSpell->p_caster->GetOrientation();

	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(12138, SSX, SSY, SSZ, SSO, true, false, 0, 0);
	return true;
}
// *****************************************************************************
bool LunaclawHord(uint32 i, Spell *pSpell)
{
	if(!pSpell->p_caster) return true;

	float SSX = pSpell->p_caster->GetPositionX();
	float SSY = pSpell->p_caster->GetPositionY();
	float SSZ = pSpell->p_caster->GetPositionZ();
	float SSO = pSpell->p_caster->GetOrientation();

	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(12138, SSX, SSY, SSZ, SSO, true, false, 0, 0);
	return true;
}
// *****************************************************************************
/*bool Triage(uint32 i, Spell *pSpell)
{
	// Unit* target = pSpell->GetUnitTarget();
	// if(!pSpell->p_caster || !target || target->GetTypeId() != TYPEID_UNIT) return true;
	 if(!pSpell->p_caster) return true;
		pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(746), true);
		QuestLogEntry *en = pSpell->p_caster->GetQuestLogForEntry(6624);
			
			if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
		{
		uint32 newcount = en->GetMobCount(0) + 1;
		en->SetMobCount(0, newcount);
		en->SendUpdateAddKill(0);
		en->UpdatePlayerFields();
		} 
	// Creature* c_target = (Creature*)target;
	// if(!c_target) return true;

	// uint32 creatureID = c_target->GetEntry();

	// if(creatureID == 12937)
		// sQuestMgr.OnPlayerKill(pSpell->p_caster, c_target);
		
	return true;
}
*/
bool Make_a_Wish(uint32 i, Spell *pSpell)
{
	if(pSpell->u_caster->GetTypeId()!=TYPEID_PLAYER)
		return true;
	uint32 spell_id = 0;
	uint32 chance = RandomUInt(4);
	switch(chance)
	{
	case 0: spell_id = 33053; break;
    case 1: spell_id = 33057; break;
    case 2: spell_id = 33059; break;
    case 3: spell_id = 33062; break;
    case 4: spell_id = 33064; break;
	}
	pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(spell_id), true);
	return true;
}

bool Poultryizer(uint32 i, Spell *pSpell)
{
	if(!pSpell->p_caster) return true;

	int chance = RandomUInt(99)+1;

	if(chance > 20) 
		pSpell->p_caster->CastSpell(pSpell->GetUnitTarget(), dbcSpell.LookupEntry(30501), true);
	else 
		pSpell->p_caster->CastSpell(pSpell->GetUnitTarget(), dbcSpell.LookupEntry(30504), true);
	return true;
}

bool Transporter_Arrival(uint32 i, Spell *pSpell)
{
	if(!pSpell->p_caster) return true;

	int chance = RandomUInt(119);
	if ( chance < 19 )                           
		pSpell->p_caster->CastSpell(pSpell->p_caster,dbcSpell.LookupEntry(23444),true);
	else if ( chance < 99 )                     
		pSpell->p_caster->CastSpell(pSpell->p_caster,dbcSpell.LookupEntry(23445),true);
	else                                    
		pSpell->p_caster->CastSpell(pSpell->p_caster,dbcSpell.LookupEntry(36902),true);
	return true;
}

bool Gnomish_Battle_Chicken(uint32 i, Spell *pSpell)
{
	if(!pSpell->p_caster) return true;
	pSpell->p_caster->CastSpell(pSpell->p_caster,dbcSpell.LookupEntry(13166),true);
	return true;
}

bool Blessing_of_Faith(uint32 i, Spell *pSpell)
{
	if(!pSpell->p_caster || !pSpell->GetUnitTarget()) return true;
	uint32 spell_id = 0;
	switch(pSpell->GetUnitTarget()->getClass())
	{
	case DRUID:   spell_id = 37878; break;
	case PALADIN: spell_id = 37879; break;
	case PRIEST:  spell_id = 37880; break;
	case SHAMAN:  spell_id = 37881; break;
	default: return true; 
	}
	pSpell->p_caster->CastSpell(pSpell->GetUnitTarget(), dbcSpell.LookupEntry(spell_id), true);
	return true;
}

bool SpellClothSummon(uint32 i, Spell *pSpell)
{
	if(!pSpell->p_caster) return true;

	float SSX = pSpell->p_caster->GetPositionX();
	float SSY = pSpell->p_caster->GetPositionY();
	float SSZ = pSpell->p_caster->GetPositionZ();
	float SSO = pSpell->p_caster->GetOrientation();

	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(17870, SSX, SSY, SSZ, SSO, true, false, 0, 0);
	return true;
}

void SetupItemSpells_1(ScriptMgr * mgr)
{
	mgr->register_dummy_spell( 4130, &BanishExile);             // Essence of the Exile Quest
	mgr->register_dummy_spell( 4131, &BanishExile);             // Essence of the Exile Quest
	mgr->register_dummy_spell( 4132, &BanishExile);             // Essence of the Exile Quest
	mgr->register_dummy_spell( 8063, &DeviateFish);             // Deviate Fish
	mgr->register_dummy_spell( 8213, &CookedDeviateFish);       // Savory Deviate Delight
	mgr->register_dummy_spell( 8593, &SymbolOfLife);            // Paladin's Redemption QuestLine
	mgr->register_dummy_spell(13120, &NetOMatic);               // Net-o-Matic
	mgr->register_dummy_spell(16589, &NoggenFoggerElixr);       // Noggenfogger
	mgr->register_dummy_spell(18974, &LunaclawAll);             // Body and Heart Alliance 6001
	mgr->register_dummy_spell(19138, &LunaclawHord);            // Body and Heart Horde    6002
	//mgr->register_dummy_spell(20804, &Triage);                  // Triage horde/alliance
	mgr->register_dummy_spell(23133, &Gnomish_Battle_Chicken);
	mgr->register_dummy_spell(23453, &GnomishTransporter);      // Gnomish Transporter
	mgr->register_dummy_spell(23448, &Transporter_Arrival);
	mgr->register_dummy_spell(24718, &HallowedWandNinja);
	mgr->register_dummy_spell(24717, &HallowedWandPirate);
	mgr->register_dummy_spell(24719, &HallowedWandLeperGnome);
	mgr->register_dummy_spell(24737, &HallowedWandGhost);
	mgr->register_dummy_spell(24930, &HallowsEndCandy);         // Hallows End Candy
	mgr->register_dummy_spell(25860, &ReindeerTransformation);  // Fresh Holly & Preserved Holly
	mgr->register_dummy_spell(26275, &WinterWondervolt);        // PX-238 Winter Wondervolt Trap
	mgr->register_dummy_aura( 26274, &WinterWondervoltAura);    // PX-238 Winter Wondervolt Transform Aura
	mgr->register_dummy_spell(26374, &ElunesCandle);
	mgr->register_dummy_spell(26074, &HolidayCheer);            // Holiday Cheer
	mgr->register_dummy_spell(26469, &SummonCritterDummy);      // Snowman Kit
	mgr->register_dummy_spell(26528, &SummonCritterDummy);      // Jingling Bell
	mgr->register_dummy_spell(26532, &SummonCritterDummy);      // Green Helper Box
	mgr->register_dummy_spell(26541, &SummonCritterDummy);      // Red Helper Box
	mgr->register_dummy_spell(26678, &BagOfCandies);
	mgr->register_dummy_spell(29200, &PurifyBoarMeat);			// Purify Boar meat spell
	mgr->register_dummy_spell(29830, &MirrensDrinkingHat);
	mgr->register_dummy_spell(30458, &NighInvulnBelt);          // Nigh Invulnerability Belt
	mgr->register_dummy_spell(30507, &Poultryizer);
	mgr->register_dummy_spell(31373, &SpellClothSummon);	
	mgr->register_dummy_spell(32001, &MinionsOfGurok);			// Minions of gurok
	mgr->register_dummy_spell(32042, &ScryingCrystal);			// Violet Scrying Crystal (Quest)
	mgr->register_dummy_spell(33060, &Make_a_Wish);
	mgr->register_dummy_spell(33381, &JumpTron);
	mgr->register_dummy_spell(33923, &SonicBoom);
	mgr->register_dummy_spell(35036, &WarpRiftGenerator);       // Summon a Warp Rift in Void Ridge
	mgr->register_dummy_spell(37877, &Blessing_of_Faith);
	mgr->register_dummy_spell(39105, &NetherWraithBeacon);      // Spellfire Tailor Quest
	mgr->register_dummy_spell(39446, &DarkmoonCard_Madness);
}
