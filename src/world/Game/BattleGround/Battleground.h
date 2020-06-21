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

class SERVER_DECL CBattleground : public EventableObject
{

		friend class AVNode;
	protected:
		/* Groups */
		Group* m_groups[2];

		time_t m_nextPvPUpdateTime;
		MapMgr* m_mapMgr;
		uint32 m_id;
		uint32 m_type;
		uint32 m_levelGroup;
		uint32 m_deltaRating[2];
		uint32 m_invisGMs;
		uint32 m_honorPerKill;
		uint32 m_zoneid;

	public:
		/* Team->Player Map */
		set<Player*> m_players[2];
		void Lock() { m_mainLock.Acquire(); }
		void Unlock() { m_mainLock.Release(); }
		void AddInvisGM() {Lock(); m_invisGMs++; Unlock();}
		void RemoveInvisGM() {Lock(); m_invisGMs--; Unlock();}
	protected:

		/* PvP Log Data Map */
		map<uint32, BGScore> m_pvpData;

		/* Lock for all player data */
		Mutex m_mainLock;

		/* Player count per team */
		uint32 m_playerCountPerTeam;

		/* "pending" players */
		set<uint32> m_pendPlayers[2];

		/* starting time */
		uint32 m_startTime;
		bool m_started;

		/* countdown stuff */
		uint32 m_countdownStage;

		/* winner stuff */
		bool m_ended;
		uint8 m_winningteam;

		/* resurrect queue */
		map<Creature*, set<uint32> > m_resurrectMap;
		uint32 m_lastResurrect;

		bool m_isWeekend;

	public:
		void AddHonorToTeam( uint32 team, uint32 amount );

		void CastSpellOnTeam( uint32 team, uint32 spell );

		void RemoveAuraFromTeam( uint32 team, uint32 aura );

		void SendChatMessage(uint32 Type, uint64 Guid, const char* Format, ...);

		/* Hook Functions */
		virtual void HookOnPlayerDeath(Player* plr) = 0;

		virtual void HookOnPlayerResurrect( Player *player ){}

		/* Repopping - different battlegrounds have different ways of handling this */
		virtual bool HookHandleRepop(Player* plr) = 0;

		/* In CTF battlegrounds mounting will cause you to lose your flag. */
		virtual void HookOnMount(Player* plr) = 0;

		/* Only used in CTF (as far as I know) */
		virtual void HookFlagDrop(Player* plr, GameObject* obj) = 0;
		virtual void HookFlagStand(Player* plr, GameObject* obj) = 0;
		virtual void HookOnFlagDrop(Player* plr) = 0;

		/* Used when a player kills a player */
		virtual void HookOnPlayerKill(Player* plr, Player* pVictim) = 0;
		virtual void HookOnHK(Player* plr) = 0;

		/* On Area Trigger */
		virtual void HookOnAreaTrigger(Player* plr, uint32 id) = 0;

		/* On Shadow Sight */
		virtual void HookOnShadowSight() = 0;

		/* On Loot Generating */
		virtual void HookGenerateLoot(Player* plr, Object* pCorpse) = 0;

		/* On Unit Killing */
		virtual void HookOnUnitKill(Player* plr, Unit* pVictim) = 0;

		virtual void HookOnUnitDied( Unit *victim ){}

		/* Retrieval Functions */
		uint32 GetId() { return m_id; }
		uint32 GetLevelGroup() { return m_levelGroup; }
		MapMgr* GetMapMgr() { return m_mapMgr; }

		/* Creating a battleground requires a pre-existing map manager */
		CBattleground(MapMgr* mgr, uint32 id, uint32 levelgroup, uint32 type);
		virtual ~CBattleground();

		/* Has it ended? */
		bool HasEnded() { return m_ended; }
		/* Has it started? */
		bool HasStarted() { return m_started; }

		/* Send the pvp log data of all players to this player. */
		void SendPVPData(Player* plr);

		/* Get the starting position for this team. */
		virtual LocationVector GetStartingCoords(uint32 Team) = 0;

		/* Send a packet to the entire battleground */
		void DistributePacketToAll(WorldPacket* packet);

		/* send a packet to only this team */
		void DistributePacketToTeam(WorldPacket* packet, uint32 Team);

		/* play sound to this team only */
		void PlaySoundToTeam(uint32 Team, uint32 Sound);

		/* play sound to all */
		void PlaySoundToAll(uint32 Sound);

		/* Full? */
		bool IsFull() { return !(HasFreeSlots(0, m_type) || HasFreeSlots(1, m_type)); }

		/* Are we full? */
		bool HasFreeSlots(uint32 Team, uint32 type);

		/* Add Player */
		void AddPlayer(Player* plr, uint32 team);
		virtual void OnAddPlayer(Player* plr) = 0;

		/* Remove Player */
		void RemovePlayer(Player* plr, bool logout);
		virtual void OnRemovePlayer(Player* plr) = 0;

		/* Port Player */
		void PortPlayer(Player* plr, bool skip_teleport = false);
		virtual void OnCreate() = 0;

		/* Remove pending player */
		void RemovePendingPlayer(Player* plr);

		/* Gets the number of free slots */
		uint32 GetFreeSlots(uint32 t, uint32 type);

		GameObject* SpawnGameObject(uint32 entry, uint32 MapId , float x, float y, float z, float o, uint32 flags, uint32 faction, float scale);
		GameObject* SpawnGameObject( uint32 entry, LocationVector &v, uint32 flags, uint32 faction, float scale );
		Creature* SpawnCreature(uint32 entry, float x, float y, float z, float o, uint32 faction = 0 );
		Creature* SpawnCreature( uint32 entry, LocationVector &v, uint32 faction = 0 );
		void UpdatePvPData();

		uint32 GetStartTime() { return m_startTime; }
		uint32 GetType() { return m_type; }

		// events should execute in the correct context
		int32 event_GetInstanceID();
		void EventCreate();

		virtual uint32 GetNameID() { return 34;}
		void EventCountdown();

		virtual void Start();
		virtual void OnStart() {}
		void Close();
		virtual void OnClose() {}

		void SetWorldState(uint32 Index, uint32 Value);
		Creature* SpawnSpiritGuide(float x, float y, float z, float o, uint32 horde);
		Creature* SpawnSpiritGuide( LocationVector &v, uint32 faction );

		uint32 GetLastResurrect() { return m_lastResurrect; }
		void AddSpiritGuide(Creature* pCreature);
		void RemoveSpiritGuide(Creature* pCreature);
		void QueuePlayerForResurrect(Player* plr, Creature* spirit_healer);
		void RemovePlayerFromResurrect(Player* plr, Creature* spirit_healer);
		void EventResurrectPlayers();
		virtual bool CanPlayerJoin(Player* plr, uint32 type);
		virtual bool CreateCorpse(Player* plr) { return true; }
		virtual bool HookSlowLockOpen(GameObject* pGo, Player* pPlayer, Spell* pSpell) { return false; }
		virtual bool HookQuickLockOpen( GameObject *go, Player *player, Spell *spell ){ return false; }

		void BuildPvPUpdateDataPacket(WorldPacket* data);
		virtual uint8 Rated() { return 0; }
		void OnPlayerPushed(Player* plr);

		virtual void SetIsWeekend(bool isweekend) {}
		void QueueAtNearestSpiritGuide(Player* plr, Creature* old);

		/////////////////////////////////////////////////////////////////////////
		//uint64 GetFlagHolderGUID( uint32 faction )
		//  Tells the GUID of the player who currently holds the flag
		//
		//Parameter(s)
		//  uint32 faction - The faction of the flag holder we are interested in
		//
		//Return Value
		//  Returns a GUID as an uint64 if the flag is held.
		//  Returns 0 otherwise.
		//
		//
		/////////////////////////////////////////////////////////////////////////
		virtual uint64 GetFlagHolderGUID( uint32 faction ) const{ return 0; }
};

