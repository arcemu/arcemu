/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2009 <http://www.ArcEmu.org/>
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

const float sotaAttackers[2][4] = {
	{ 2445.288f, 849.35f, 12.0f, 3.76f },
	{ 2445.288f, 849.35f, 12.0f, 3.76f },
};

const float sotaDefenders[2][4] = {
	{ 2428.288f, 837.05f, 13.0f, 3.76f },
	{ 2428.288f, 837.05f, 13.0f, 3.76f },
};

const float sotaRepop[2][4] = {
	{ 1600.0f, 58.3f, 11.0f, 2.98f },
	{ 1600.0f, 58.3f, 11.0f, 2.98f },
};

const float sotaBoats[2][4] = {
	{ 2439.4f, 845.38f, 1.0f, 3.76f },
	{ 2439.4f, 845.38f, 1.0f, 3.76f },
};

const float sotaStopBoats[2][4] = {
	{ 1623.34f, 37.0f, 1.0f, 3.9f },
	{ 1623.34f, 37.0f, 1.0f, 3.9f },
};

const float sotaStopBoatsPlayer[2][4] = {
	{ 1624.7f, 42.93f, 12.0f, 2.63f },
	{ 1624.7f, 42.93f, 12.0f, 2.63f },
};

class StrandOfTheAncient : public CBattleground
{
	//Transporter * m_boats[2];
	//TransportPath path;
	GameObject * m_boats[2];
	list<GameObject*> m_gates;
	list<Player *> sota_players;
	PassengerMap boat1Crew;
	PassengerMap boat2Crew;
	//m_players

public:
	static CBattleground * Create(MapMgr * m, uint32 i, uint32 l, uint32 t) { return new StrandOfTheAncient(m, i, l, t); }

	StrandOfTheAncient(MapMgr * mgr, uint32 id, uint32 lgroup, uint32 t) : CBattleground(mgr, id, lgroup, t)
	{
		uint32 mapId = BattlegroundManager.GetMap(BATTLEGROUND_STRAND_OF_THE_ANCIENT);
		m_boats[0] = SpawnGameObject(20808,
			mapId,
			sotaBoats[0][0], sotaBoats[0][1], sotaBoats[0][2], sotaBoats[0][3], 0, 0, 1.0f);

		m_boats[0]->Spawn(mgr);

		/*
		m_boats[1] = SpawnGameObject(20808,
			mapId,
			sotaStopBoats[0][0], sotaStopBoats[0][1], sotaStopBoats[0][2], sotaStopBoats[0][3], 0, 0, 1.0f);

		m_boats[1]->Spawn(mgr);
		*/

	}

	~StrandOfTheAncient()
	{
		//ObjectMgr::getSingleton().DeleteTransport(m_boats[0]);
	}

	uint32 GetNameID() { return 34; } // in world_strings table

	void HookOnAreaTrigger(Player * plr, uint32 id);
	void HookFlagStand(Player * plr, GameObject * obj);
	void HookOnFlagDrop(Player * plr);
	void HookFlagDrop(Player * plr, GameObject * obj);
	void HookOnPlayerKill(Player * plr, Player * pVictim);
	void HookOnHK(Player * plr);
	void OnAddPlayer(Player * plr);
	void OnRemovePlayer(Player * plr);
	LocationVector GetStartingCoords(uint32 team);
	void HookOnPlayerDeath(Player * plr);
	void HookOnMount(Player * plr);
	bool HookHandleRepop(Player * plr);
	void OnCreate();
	void OnStart();
	void HookOnShadowSight();
	void HookGenerateLoot(Player *plr, Object * pOCorpse);
	void HookOnUnitKill(Player * plr, Unit * pVictim);
	void SetIsWeekend(bool isweekend);

protected:
	uint32 m_flagHolders[2];

};
