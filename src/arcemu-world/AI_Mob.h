#ifndef MOB_AI_H
#define MOB_AI_H

enum MOBAI_MISCATTRIBUTES
{
	/* Is our unit in an instance map type */
	ATTR_ISIN_INSTANCE = 0x01,
	/* For those mobs that reset hp when they leave combat */
	ATTR_CAN_RESETHP = 0x02,
	/* Set when we added/removed a threat entry, we update our most and second hated. */
	ATTR_UPDATE_MOST_N_SECOND_HATED = 0x04,
};
enum AI_Agent
{
	AGENT_NULL = 0x0,
	AGENT_MELEE = 0x01,	//	Enables melee and melee spells, enabled by defualt.
	AGENT_RANGED = 0x02, //	Enables ranged and ranged spells.
	AGENT_SPELL = 0x04,	// Enables use of spells, enabled by default.
	AGENT_PANIC = 0x08,	// Enables panic events
	AGENT_CALLFORHELP = 0x10,	// Enables calling for help.
	AGENT_AUTO_SHOT = 17, //The agent that will define auto shot spells. Only 1 spell can have this, the rest are ignored if they do.

};

class SERVER_DECL MobAI : public AIInterface
{
protected:

	/************** PROTECTED VARIABLES *********************/
	typedef HM_NAMESPACE::hash_map<uint64, int32> TargetMap;
	typedef std::set<uint64> AssistTargetSet;
	typedef std::list<AI_Spell*> SpellStore;
	typedef std::deque<AI_Spell*> SpellDeck;

	/*	Deques which we can queu" spells one or more spells to cast. */
	SpellDeck m_spellDeck;
	SpellDeck m_rangedSpellDeck;
	SpellDeck m_meleeSpellDeck;

	/*
		This is used by ranged capable mobs, they will cast this spell during ranged attacks.
		If it's null, they result to generic SPELL_RANGED_GENERAL
		This should be the only ai agent with agent ranged. The rest will count as spells
	*/
	AI_Spell * m_ShootSpell;
	
	/* Lock used when accessing ai targets, might remove this. */
	FastMutex m_aiTargetsLock;
	
	/* a map that stores guids and threat values of our targets. */
	TargetMap m_aiTargets;

	/* a map that stores guids to our assist targets */
	AssistTargetSet m_assistTargets;

	/*	bit 0 - is in instance
		bit 1 - skip reset hp
		bit 2 - update most hated and second hated
		bit 3 - delete next spell
	*/
	uint8 misc_attributes;

	/*
		Bit 0-2 = waypoint movement mode(ex. dontmovewp,moverandomwp,movecircle)
		Bit 3 = move backwards
		Bit 4 = show forward waypoints
		Bit 5 = show backward waypoints.
	*/
	uint8 m_WaypointFlags;

	/*	The waypoint that we are currently at/moving to */
	uint8 m_currentWaypoint;

	/*	Bit 0 - Agent Melee
		Bit 1 - Agent ranged
		Bit 2 - Agent Spell
		Bit 3 - Agent Flee
		Bit 4 - Agent Call for Help
	*/
	uint8 m_aiAgent;

	/*	Our out of combat range value in yards. */
	uint32 m_autoWipeRange;

	/*	Our primary target that we should be attacking */
	uint64 m_nextTarget;

	/*	Our main tanks guid and threat */
	uint64 m_mostHated;
	int32 m_mostHatedThreat;

	/*	Our off tanks guid and threat  */
	uint64 m_secondHated;
	int32 m_secondHatedThreat;
	
	/*
		Our current targets previous location.
		We use this to minimize movement packets when we are attacking and our target is too far or too close. */
	LocationVector m_prevTargetLocation;

	std::list<spawn_timed_emotes*>::iterator next_timed_emote;
	uint32	timed_emote_expire;

	/*	A container that stores all the spells we currently have. The addSpell(AI_Spell*) function adds to this list. */
	SpellStore m_spells;

	/* A map that stores all the waypoints we currently have */
	WayPointMap * m_waypoints;

	TimedEmoteList * timed_emotes;

	/*************	PROTECTED METHODS *********************/
	void OnUpdate();

	/* Called from HandleEvent() when we engage a target and we aren't alrdy fighting. */
	void OnCombatStart(Unit * enemy);

	/* Called from HandleEvent() when we no longer have targets in our threat list. */
	void OnCombatStop(Unit * enemy);

	/* Called from HandleEvent() when we die. */
	void OnDeath(Unit * enemy);

	/* Agent Handlers */
	void OnAgentSpell();
	void OnAgentRanged();
	void OnAgentMelee();
	void OnAgentCallForHelp();
	void OnAgentPanic();

	/*	Returns in yards, our distance from our return coords */
	float calcDistanceFromHome();

public:
	MobAI(Unit*);
	~MobAI() 
	{
		SpellStore::iterator itr = m_spells.begin();
		for(; itr != m_spells.end(); ++itr)
			delete (*itr);
		m_spells.clear();
	}
	/*	Used when we either want to attack a new target(therefore generating on combat start event) or
		if we just want to add the specified target into our threat list */
	void AttackReaction(Unit * pUnit, uint32 damage_dealt, uint32 spellID);

	/*	Same as attack reaction but is not capable of generating on combat start */
	void HealReaction(Unit* caster, Unit* victim, SpellEntry* sp, uint32 amount);

	/* This implements several event based actions specified by `event` parameter. */
	void HandleEvent(uint32 event, Unit* pUnit, uint32 misc1);

	

	/*	Updates our threatlist and assist target list, stale entries are removed here.
		Our most hated and second hated are also updated here */
	void updateTargets();

	/*	When we are in an attacking state, this is called to perform actions preset agent.
		We also validate our current target here */
	void updateCombat(uint32);

	void _UpdateTimer(uint32);

	/*	When we reach a waypoint, this is called, it mostly used for calling of the script event 
		but, also used to delay our creatures movement by the waypoint's parameters. */
	void OnReachWp();

	/*	Function used to handle any waypoint movement. */
	void handleWaypointMovement();


	/* Spell Methods */

	/*	This makes an internal copy of the spell for private use */
	void addSpell(AI_Spell*);

	/*	Removes an internal copy similar to the target spell */
	void removeSpell(AI_Spell*);

	/*	Linearly goes through our list of spells updating the cooldowns based on
		the differential time. The first spell to have finished his cd is returned to be cast. 
		*/
	void updateSpellCooldowns();

	Unit * getNextTarget();
	void setNextTarget(Unit *);

	/*	Used when we are about to cast a spell. 
		It simply gets a target based on the spell type and target type. 
		*/
	Unit * getSpellTarget(AI_Spell*);

	/*	Returns a random assist target if we have any.
		Used by those spells w/ target type TTYPE_RANDOMASSIST.
		*/
	Unit * getRandomAssistTarget(AI_Spell * sp = NULL);
	/*
		Returns a random friendly unit local to this unit.
		Used by spells w/ target type TTYPE_RANDOMFRIENDLY
	*/
	Unit * getRandomFriendly(AI_Spell * sp = NULL);
	/*
		Returns a attackable target.
		Used by spells w/ target type TTYPE_RANDOMTARGET
		*/
	Unit * getRandomTarget(AI_Spell * sp = NULL);

	/*	Simply returns a reference to top spell in our queue. */
	AI_Spell * getNextSpell() { return m_spellDeck.front(); }
	void setNextSpell(AI_Spell* );

	/* Threat Methods */
	
	/* For adding new targets to the threat list. Comes in handy because it also 
	updates whether we need to update our most hated and second hated. */
	bool addNewTarget(uint64 guid, uint32 threat);
	
	/*	Calculates threat that the specified parameters would generate */
	uint32	calcThreat(uint32 damage, SpellEntry * sp, Unit* Attacker);

	/*	Gets the current threat of the guid specified or 0 if we have none */
	uint32	getThreatByGUID(uint64 guid);

	/* Gets the current threat of the object specified or 0 if we have none */
	uint32	getThreatByPtr(Unit* obj);

	/*	Returns a reference to our most hated target in the threat list */
	Unit *	getMostHated();

	/*	Searches for new targets when we no longer have targets in our list and we aren't allowed to return until there are no inrange possible targets */
	Unit *	findTarget();

	/*	Searches for friendly units within the specified range, for each threat list entry we have, we add it to that friendly unit's threat list. */
	bool	findFriends(float);

	/*	Returns a reference to our second hated in our threat list */
	Unit *	getSecondHated();

	/*	Modifies the threat of the guid in our threat list. Returns true when successful or false when we don't have the target in our list */
	bool	modThreatByGUID(uint64 guid, int32 mod);
	bool	modThreatByPtr(Unit* obj, int32 mod);

	/*	Removes the threat of the guid in our list. Does nothing if we don't have the guid in our list. */
	void    removeThreatByGUID(uint64 guid);
	void	removeThreatByPtr(Unit* obj);

	ARCEMU_INLINE void lockTargets(bool lock) 
	{ 
		lock? m_aiTargetsLock.Acquire(): m_aiTargetsLock.Release(); 
	};

	/*	Manually adds an assist target to our assist list. */
	void addAssistTarget(Unit * nAssist);

	/*	Clears all threat list entries.	Can trigger on leavecombat */
	void wipeHateList();

	/*	The count of all enemies we have */
	size_t getAITargetsCount() { return m_aiTargets.size(); }

	void EventAiInterfaceParamsetFinish();
	void EventChangeFaction( Unit *ForceAttackersToHateThisInstead= NULL );	//we have to tell our current enemies to stop attacking us, we should also forget about our targets

	/*********		Waypoint Methods	********/

	/*	Sets our waypoint move type. We move to our waypoints defined by the behavior of this type. */
	ARCEMU_INLINE void setWPMoveType(MovementType mType) { m_WaypointFlags |= (mType & WAYPOINT_MOVETYPEMASK); }

	/*	Returns our current waypoint move type. */
	ARCEMU_INLINE uint32 getWPMoveType() { return (m_WaypointFlags & WAYPOINT_MOVETYPEMASK); }

	/*	Returns the waypoint id of the waypoint we are currently at or moving to.	*/
	uint8 getCurrentWaypoint() { return m_currentWaypoint; }

	/*	Changes the previous id specified by oldwpid  to newwpid, notice this may change the order we move to our waypoints based on our move type.	*/
	void changeWaypointID(uint32 oldwpid, uint32 newwpid);

	/*	Adds a new waypoint to our list, TODO: Implement */
	bool addWaypoint(WayPoint* wp) { return false; }
	
	/*	Saves our current waypoints into the DB	*/
	bool saveWaypoints() { return false; }

	/*	Returns true if we have any waypoints	*/
	bool hasWaypoints() { return getWaypointsCount() != 0; }

	//bool showWayPoints(Player* pPlayer, bool Backwards);
	//bool hideWayPoints(Player* pPlayer);
	/*	Returns a waypoint registered by the wpid.	*/
	WayPoint * getWaypoint(uint8 wpid) { return NULL; }

	/*	Deletes an existing wp specified by wpid */
	void deleteWaypoint(uint8 wpid) {}

	/*	Deletes all our previous waypoints	*/
	void deleteWaypoints() {}

	/*	Sets the waypoint to move to specified by the id, notice the creature will only move to it, if the waypoint move type is MOVEMENTTYPE_WANTEDWP. */
	void setWaypointToMove(uint8 id) { m_currentWaypoint = id; }

	/*	Returns our current waypoints count. */
	size_t getWaypointsCount()
	{
		size_t cnt = 0;
		if(m_waypoints && !m_waypoints->empty())
			cnt = m_waypoints->size()-1;
		return cnt;
	}

	/*	Returns our current waypoint flags	*/
	uint32 getWaypointflags() { return uint32(m_WaypointFlags); }

	/*	If we are currently showing waypoints, this functionality was removed. */
	bool isWpShowingForward() { return (m_WaypointFlags & WAYPOINT_SHOWFORWARD) != 0; }
	void showWpForward(bool show = true) { 
		if(show)
			m_WaypointFlags |= WAYPOINT_SHOWFORWARD;
		else
			m_WaypointFlags &= ~WAYPOINT_SHOWFORWARD;
	}
	bool isWpShowingBackWard() { return (m_WaypointFlags & WAYPOINT_SHOWBACKWARD) != 0; }
	void showWpBackWard(bool show = true) {
		if(show)
			m_WaypointFlags |= WAYPOINT_SHOWBACKWARD;
		else
			m_WaypointFlags &= ~WAYPOINT_SHOWBACKWARD;
	}
	bool isWpMovingBackWards() { return (m_WaypointFlags & WAYPOINT_MOVEBACKWARD) != 0; }
	void setWpMoveBackWards(bool show = true) {
		if(show)
			m_WaypointFlags |= WAYPOINT_MOVEBACKWARD;
		else
			m_WaypointFlags &= ~WAYPOINT_MOVEBACKWARD;
	}

	/*********** Misc Attribute Methods *******************/

	/*	If we are currently in an instance map type */
	bool isInInstance() { return (misc_attributes & ATTR_ISIN_INSTANCE) != 0; }

	/*	Sets or resets if we are in an instance map type.	*/
	void setInInstace(bool set = true) {
		if(set)
			misc_attributes |= ATTR_ISIN_INSTANCE;
		else
			misc_attributes &= ~ATTR_ISIN_INSTANCE;
	}

	/*	If our unit can reset hp on leave combat */
	bool canResetHp() { return (misc_attributes & ATTR_CAN_RESETHP) != 0; }

	/*	Sets or resets our reset hp behavior */
	void setCanResetHp(bool set = true) {
		if(set)
			misc_attributes |= ATTR_CAN_RESETHP;
		else
			misc_attributes &= ~ATTR_CAN_RESETHP;
	}

	/*	If we should update our most and second hated entries	*/
	bool canUpdateMostHated() { return (misc_attributes & ATTR_UPDATE_MOST_N_SECOND_HATED)  != 0; }
	
	/*	Sets or resets this behavior */
	void setCanUpdateMostHated(bool set = true)
	{
		if(set)
			misc_attributes |= ATTR_UPDATE_MOST_N_SECOND_HATED;
		else
			misc_attributes &= ~ATTR_UPDATE_MOST_N_SECOND_HATED;
	}

	/*	Returns our out of combat range in yards */
	uint32 getAutoWipeRange() { return (uint32)m_autoWipeRange; }

	/*	Sets our out of combat range in yards	*/
	void setAutoWipeRange(uint32 range) { m_autoWipeRange = (uint8)range; }

	// Ai agent manipulators

	/*	Returns our current agent flags	*/
	uint8 getAIAgent() { return m_aiAgent; }
	/*	Sets or resets an agent */
	void setAIAgent(AI_Agent bit, bool set = true) 
	{
		if(set)
			m_aiAgent |= bit;
		else
			m_aiAgent &= ~bit;
	}
	
	/*	Retrieves our currently processing agent. */
	uint8 getProcessingAgent() { return (m_behaviorMask & BEHAVIOR_PROCAGENTMASK) >> BEHAVIOR_PROCAGENTSHIFTCNT; }

	/*	Sets the currently processing agent */
	void setProcessingAgent(AI_Agent bit) {
		m_behaviorMask |= (bit << BEHAVIOR_PROCAGENTSHIFTCNT);
	}

	/* Agent Manipulation Methods */
	bool hasMeleeAgent() { return (m_aiAgent & AGENT_MELEE) != 0; }
	void setMeleeAgent(bool set = true) {
		if(set)
			m_aiAgent |= AGENT_MELEE;
		else
			m_aiAgent &= ~AGENT_MELEE;
	}
	bool hasRangedAgent() { return (m_aiAgent & AGENT_RANGED) != 0; }
	void setRangedAgent(bool set = true) {
		if(set)
			m_aiAgent |= AGENT_RANGED;
		else
			m_aiAgent &= ~AGENT_RANGED;
	}
	bool hasSpellAgent() { return (m_aiAgent & AGENT_SPELL) != 0; }
	void setSpellAgent(bool set = true) {
		if(set)
			m_aiAgent |= AGENT_SPELL;
		else
			m_aiAgent &= ~AGENT_SPELL;
	}
	bool hasPanicAgent() { return (m_aiAgent & AGENT_PANIC) != 0; }
	void setPanicAgent(bool set = true) {
		if(set)
			m_aiAgent |= AGENT_PANIC;
		else
			m_aiAgent &= ~AGENT_PANIC;
	}
	bool hasCallHelpAgent() { return (m_aiAgent & AGENT_CALLFORHELP) != 0; }
	void setCallHelpAgent(bool set = true)
	{
		if(set)
			m_aiAgent |= AGENT_CALLFORHELP;
		else
			m_aiAgent &= ~AGENT_CALLFORHELP;
	}

	/*	Returns our current auto shoot spell */
	AI_Spell * getShootSpell() { return m_ShootSpell; }

	/*	Sets the auto shoot spell */
	void setShootSpell(AI_Spell * sSpell) { m_ShootSpell = sSpell; }
};

#define TO_AIMOB(ai) static_cast<MobAI*>(ai)
#endif