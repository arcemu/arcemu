/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "StdAfx.h"

void HonorHandler::AddHonorPointsToPlayer(Player* pPlayer, uint32 uAmount)
{
	pPlayer->HandleProc(PROC_ON_GAIN_EXPIERIENCE, pPlayer, NULL);
	pPlayer->m_procCounter = 0;

	if(pPlayer->GetMapId() == MAP_ARENA_NAGRAND || pPlayer->GetMapId() == MAP_ARENA_BLADES_EDGE || pPlayer->GetMapId() == MAP_ARENA_RUINS_OF_LORDAERON)
		return;
	pPlayer->m_honorPoints += uAmount;
	pPlayer->m_honorToday += uAmount;
	if(pPlayer->m_honorPoints > sWorld.m_limits.honorpoints) 	
	pPlayer->m_honorPoints = sWorld.m_limits.honorpoints;

	RecalculateHonorFields(pPlayer);
}

int32 HonorHandler::CalculateHonorPointsForKill(uint32 playerLevel, uint32 victimLevel)
{

	uint32 k_level = playerLevel;
	uint32 v_level = victimLevel;

	uint32 k_grey = 0;

	if(k_level > 5 && k_level < 40)
	{
		k_grey = k_level - 5 - int(std::floor(((float)k_level) / 10.0f));
	}
	else
	{
		k_grey = k_level - 1 - int(std::floor(((float)k_level) / 5.0f));
	}

	if(v_level <= k_grey)
		return 0;

	float honor_points = -0.53177f + 0.59357f * exp((k_level + 23.54042f) / 26.07859f);
	honor_points *= World::getSingleton().getRate(RATE_HONOR);
	return int(honor_points);
}

void HonorHandler::OnPlayerKilled(Player* pPlayer, Player* pVictim)
{
	if(pVictim->m_honorless)
		return;

	if(pPlayer->m_bg)
	{
		if(pVictim->m_bgTeam == pPlayer->m_bgTeam)
			return;

		// patch 2.4, players killed >50 times in battlegrounds won't be worth honor for the rest of that bg
		if(pVictim->m_bgScore.Deaths >= 50)
			return;
	}
	else
	{
		if(pPlayer->GetTeam() == pVictim->GetTeam())
			return;
	}

	// Calculate points
	int32 Points = 0;
	if(pPlayer != pVictim)
		Points = CalculateHonorPointsForKill(pPlayer->getLevel(), pVictim->getLevel());

	if(Points > 0)
	{
		if(pPlayer->m_bg)
		{
			// hackfix for battlegrounds (since the groups there are disabled, we need to do this manually)
			vector<Player*> toadd;
			uint32 t = pPlayer->m_bgTeam;
			toadd.reserve(15);		// shouldn't have more than this
			pPlayer->m_bg->Lock();
			set<Player*> * s = &pPlayer->m_bg->m_players[t];

			for(set<Player*>::iterator itr = s->begin(); itr != s->end(); ++itr)
			{
				// Also check that the player is in range, and the player is alive.
				if((*itr) == pPlayer || ((*itr)->isAlive() && (*itr)->isInRange(pPlayer, 100.0f)))
					toadd.push_back(*itr);
			}

			if(toadd.size() > 0)
			{
				uint32 pts = Points / (uint32)toadd.size();
				for(vector<Player*>::iterator vtr = toadd.begin(); vtr != toadd.end(); ++vtr)
				{
					AddHonorPointsToPlayer(*vtr, pts);

					(*vtr)->m_killsToday++;
					(*vtr)->m_killsLifetime++;
					pPlayer->m_bg->HookOnHK(*vtr);
					if(pVictim)
					{
						// Send PVP credit
						WorldPacket data(SMSG_PVP_CREDIT, 12);
						uint32 pvppoints = pts * 10;
						data << pvppoints << pVictim->GetGUID() << uint32(pVictim->GetPVPRank());
						(*vtr)->GetSession()->SendPacket(&data);
					}
				}
			}

			pPlayer->m_bg->Unlock();
		}
		else
		{
			set<Player*> contributors;
			// First loop: Get all the people in the attackermap.
			pVictim->UpdateOppFactionSet();
			for(std::set<Object*>::iterator itr = pVictim->GetInRangeOppFactsSetBegin(); itr != pVictim->GetInRangeOppFactsSetEnd(); itr++)
			{
				if(!(*itr)->IsPlayer())
					continue;

				bool added = false;
				Player* plr = TO_PLAYER(*itr);
				if(pVictim->CombatStatus.m_attackers.find(plr->GetGUID()) != pVictim->CombatStatus.m_attackers.end())
				{
					added = true;
					contributors.insert(plr);
				}

				if(added && plr->GetGroup())
				{
					Group* pGroup = plr->GetGroup();
					uint32 groups = pGroup->GetSubGroupCount();
					for(uint32 i = 0; i < groups; i++)
					{
						SubGroup* sg = pGroup->GetSubGroup(i);
						if(!sg) continue;

						for(GroupMembersSet::iterator itr2 = sg->GetGroupMembersBegin(); itr2 != sg->GetGroupMembersEnd(); itr2++)
						{
							PlayerInfo* pi = (*itr2);
							Player* gm = objmgr.GetPlayer(pi->guid);
							if(!gm) continue;

							if(gm->isInRange(pVictim, 100.0f))
								contributors.insert(gm);
						}
					}
				}
			}

			for(set<Player*>::iterator itr = contributors.begin(); itr != contributors.end(); itr++)
			{
				Player* pAffectedPlayer = (*itr);
				if(!pAffectedPlayer) continue;

				pAffectedPlayer->m_killsToday++;
				pAffectedPlayer->m_killsLifetime++;
				if(pAffectedPlayer->m_bg)
					pAffectedPlayer->m_bg->HookOnHK(pAffectedPlayer);

				int32 contributorpts = Points / (int32)contributors.size();
				AddHonorPointsToPlayer(pAffectedPlayer, contributorpts);

				sHookInterface.OnHonorableKill(pAffectedPlayer, pVictim);

				WorldPacket data(SMSG_PVP_CREDIT, 12);
				uint32 pvppoints = contributorpts * 10; // Why *10?
				data << pvppoints << pVictim->GetGUID() << uint32(pVictim->GetPVPRank());
				pAffectedPlayer->GetSession()->SendPacket(&data);

				int PvPToken = 0;
				Config.OptionalConfig.GetInt("Extra", "PvPToken", &PvPToken);
				if(PvPToken > 0)
				{
					int PvPTokenID = 0;
					Config.OptionalConfig.GetInt("Extra", "PvPTokenID", &PvPTokenID);
					if(PvPTokenID > 0)
					{
						Item* PvPTokenItem = objmgr.CreateItem(PvPTokenID, pAffectedPlayer);
						if(PvPTokenItem)
						{
							PvPTokenItem->SoulBind();
							pAffectedPlayer->GetItemInterface()->AddItemToFreeSlot(PvPTokenItem);
						}
					}
				}
			}
		}
	}
}

void HonorHandler::RecalculateHonorFields(Player* pPlayer)
{
	pPlayer->SetUInt32Value(PLAYER_FIELD_KILLS, uint16(pPlayer->m_killsToday) | (pPlayer->m_killsYesterday << 16));
	pPlayer->SetUInt32Value(PLAYER_FIELD_TODAY_CONTRIBUTION, pPlayer->m_honorToday);
	pPlayer->SetUInt32Value(PLAYER_FIELD_YESTERDAY_CONTRIBUTION, pPlayer->m_honorYesterday);
	pPlayer->SetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS, pPlayer->m_killsLifetime);
	pPlayer->SetHonorCurrency(pPlayer->m_honorPoints);
	pPlayer->SetArenaCurrency(pPlayer->m_arenaPoints);

	// Currency tab - (Blizz Placeholders)
	pPlayer->UpdateKnownCurrencies(43307, true); //Arena Points
	pPlayer->UpdateKnownCurrencies(43308, true); //Honor Points
}

