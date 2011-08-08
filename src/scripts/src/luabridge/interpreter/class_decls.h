#pragma once

class LuaGameObject : public GameObjectAIScript
{
	public:

		PObjectBinding m_binding;

		LuaGameObject(GameObject*);
		~LuaGameObject();
		ARCEMU_INLINE GameObject* getGO() const { return _gameobject; }

		void OnCreate();
		void OnSpawn();
		void OnDespawn();
		void OnLootTaken(Player*, ItemPrototype*);
		void OnActivate(Player*);
		void AIUpdate();
		void Destroy();
};

class LuaCreature : public CreatureAIScript
{
	public:
		PObjectBinding m_binding;
		LuaCreature(Creature*);
		~LuaCreature();

		void OnCombatStart(Unit*);
		void OnCombatStop(Unit*);
		void OnTargetDied(Unit*);
		void OnDied(Unit*);
		void OnTargetParried(Unit*);
		void OnTargetDodged(Unit*);
		void OnTargetBlocked(Unit*, int32);
		void OnTargetCritHit(Unit*, int32);
		void OnParried(Unit*);
		void OnDodged(Unit*);
		void OnBlocked(Unit*, int32);
		void OnCritHit(Unit*, int32);
		void OnHit(Unit*, float);
		void OnAssistTargetDied(Unit*);
		void OnFear(Unit*, uint32);
		void OnFlee(Unit*);
		void OnCallForHelp();
		void OnLoad();
		void OnReachWP(uint32, bool);
		void OnLootTaken(Player*, ItemPrototype*);
		void AIUpdate();
		void OnEmote(Player*, EmoteType);
		void OnDamageTaken(Unit*, uint32);
		void Destroy();
};

class LuaInstance : public InstanceScript
{
	public:
		uint32 m_instanceId;
		MapMgr* mgr_;
		PObjectBinding m_binding;

		LuaInstance(MapMgr*);
		~LuaInstance();

		void OnPlayerDeath(Player*, Unit*);
		void OnPlayerEnter(Player*);
		void OnAreaTrigger(Player*, uint32);
		void OnZoneChange(Player*, uint32, uint32);
		void OnCreatureDeath(Creature*, Unit*);
		void OnCreaturePushToWorld(Creature*);
		void OnGameObjectActivate(GameObject*, Player*);
		void OnGameObjectPushToWorld(GameObject*);
		void OnLoad();
		void Destroy();
};

class LuaGossip : public Arcemu::Gossip::Script
{
	public:
		uint32 id_;
		PObjectBinding m_unit_gossip_binding;
		PObjectBinding m_item_gossip_binding;
		PObjectBinding m_go_gossip_binding;

		LuaGossip(uint32 id);
		~LuaGossip();

		void OnHello(Object*, Player*);
		void OnSelectOption(Object*, Player*, uint32, const char*);
		void Destroy();

};

class LuaQuest : public QuestScript
{
	public:
		uint32 entry;
		PObjectBinding m_binding;

		LuaQuest(uint32);
		~LuaQuest();

		void OnQuestStart(Player*, QuestLogEntry*);
		void OnQuestComplete(Player*, QuestLogEntry*);
		void OnQuestCancel(Player*);
		void OnGameObjectActivate(uint32, Player*, QuestLogEntry*);
		void OnCreatureKill(uint32, Player*, QuestLogEntry*);
		void OnExploreArea(uint32, Player*, QuestLogEntry*);
		void OnPlayerItemPickup(uint32, uint32, Player*, QuestLogEntry*);
};

