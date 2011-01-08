#pragma once
#include "StdAfx.h"
#include "LUAEngine.h"
#include "lua_creature.h"

class LuaCreature : public CreatureAIScript
{
public:
	PObjectBinding m_binding;

	LuaCreature(Creature* creature) : CreatureAIScript(creature), m_binding(NULL) {}
	~LuaCreature() {}
	ARCEMU_INLINE void SetUnit(Creature * ncrc) { _unit = ncrc; }
	void OnCombatStart(Unit* mTarget)
	{
		NULL_BINDING_CHECK
		lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_ENTER_COMBAT]);
		push_creature(_unit);
		push_int(CREATURE_EVENT_ON_ENTER_COMBAT);
		push_unit(mTarget);
		lua_engine::ExecuteLuaFunction(3);

		RELEASE_LOCK
	}

	void OnCombatStop(Unit* mTarget)
	{
		NULL_BINDING_CHECK

		lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_LEAVE_COMBAT]);
		push_creature(_unit);
		push_int(CREATURE_EVENT_ON_LEAVE_COMBAT);
		push_unit(mTarget);
		lua_engine::ExecuteLuaFunction(3);

		RELEASE_LOCK
	}

	void OnTargetDied(Unit* mTarget)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_TARGET_DIED]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_TARGET_DIED);
		push_unit(mTarget);
		lua_engine::ExecuteLuaFunction(3);

		RELEASE_LOCK
	}

	void OnDied(Unit *mKiller)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_DIED]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_DIED);
		push_unit(mKiller);
		lua_engine::ExecuteLuaFunction(3);

		RELEASE_LOCK
	}
	void OnTargetParried(Unit* mTarget)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_TARGET_PARRIED]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_TARGET_PARRIED);
		push_unit(mTarget);
		lua_engine::ExecuteLuaFunction(3);

		RELEASE_LOCK
	}
	void OnTargetDodged(Unit* mTarget)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_TARGET_DODGED]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_TARGET_DODGED);
		push_unit(mTarget);
		lua_engine::ExecuteLuaFunction(3);

		RELEASE_LOCK
	}
	void OnTargetBlocked(Unit* mTarget, int32 iAmount)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_TARGET_BLOCKED]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_TARGET_BLOCKED);
		push_unit(mTarget);
		push_int(iAmount);
		lua_engine::ExecuteLuaFunction(4);

		RELEASE_LOCK
	}
	void OnTargetCritHit(Unit* mTarget, int32 fAmount)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_TARGET_CRIT_HIT]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_TARGET_CRIT_HIT);
		push_unit(mTarget);
		push_int(fAmount);
		lua_engine::ExecuteLuaFunction(4);
		RELEASE_LOCK
	}
	void OnParried(Unit* mTarget)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_PARRY]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_PARRY);
		push_unit(mTarget);
		lua_engine::ExecuteLuaFunction(3);

		RELEASE_LOCK
	}
	void OnDodged(Unit* mTarget)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_DODGED]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_DODGED);
		push_unit(mTarget);
		lua_engine::ExecuteLuaFunction(3);
		RELEASE_LOCK
	}
	void OnBlocked(Unit* mTarget, int32 iAmount)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_BLOCKED]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_BLOCKED);
		push_unit(mTarget);
		push_int(iAmount);
		lua_engine::ExecuteLuaFunction(4);
		RELEASE_LOCK
	}
	void OnCritHit(Unit* mTarget, int32 fAmount)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_CRIT_HIT]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_CRIT_HIT);
		push_unit(mTarget);
		push_int(fAmount);
		lua_engine::ExecuteLuaFunction(4);
		RELEASE_LOCK
	}
	void OnHit(Unit* mTarget, float fAmount)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_HIT]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_HIT);
		push_unit(mTarget);
		push_float(fAmount);
		lua_engine::ExecuteLuaFunction(4);

		RELEASE_LOCK
	}
	void OnAssistTargetDied(Unit* mAssistTarget)
	{

		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_ASSIST_TARGET_DIED]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_ASSIST_TARGET_DIED);
		push_unit(mAssistTarget);
		lua_engine::ExecuteLuaFunction(3);

		RELEASE_LOCK
	}
	void OnFear(Unit* mFeared, uint32 iSpellId)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_FEAR]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_FEAR);
		push_unit(mFeared);
		push_int(iSpellId);
		lua_engine::ExecuteLuaFunction(4);

		RELEASE_LOCK
	}
	void OnFlee(Unit* mFlee)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_FLEE]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_FLEE);
		push_unit(mFlee);
		lua_engine::ExecuteLuaFunction(3);

		RELEASE_LOCK
	}
	void OnCallForHelp()
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_CALL_FOR_HELP]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_CALL_FOR_HELP);
		lua_engine::ExecuteLuaFunction(2);

		RELEASE_LOCK
	}
	void OnLoad()
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_LOAD]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_LOAD);
		lua_engine::ExecuteLuaFunction(2);

		RELEASE_LOCK
			uint32 iid = _unit->GetInstanceID();
		if (_unit->GetMapMgr() == NULL || _unit->GetMapMgr()->GetMapInfo()->type == INSTANCE_NULL)
			iid = 0;
		/*OnLoadInfo.push_back(_unit->GetMapId());
		OnLoadInfo.push_back(iid);
		OnLoadInfo.push_back(GET_LOWGUID_PART(_unit->GetGUID()));*/
	}
	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_REACH_WP]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_REACH_WP);
		push_int(iWaypointId);
		push_bool(bForwards);
		lua_engine::ExecuteLuaFunction(4);

		RELEASE_LOCK
	}
	void OnLootTaken(Player* pPlayer, ItemPrototype *pItemPrototype)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_LOOT_TAKEN]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_LOOT_TAKEN);
		push_unit(pPlayer);
		push_int(pItemPrototype->ItemId);
		lua_engine::ExecuteLuaFunction(4);
		RELEASE_LOCK
	}
	void AIUpdate()
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_AIUPDATE]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_AIUPDATE);
		lua_engine::ExecuteLuaFunction(2);

		RELEASE_LOCK
	}
	void OnEmote(Player * pPlayer, EmoteType Emote)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_EMOTE]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_EMOTE);
		push_unit(pPlayer);
		push_int( (int)Emote);
		lua_engine::ExecuteLuaFunction(4);

		RELEASE_LOCK
	}
	void OnDamageTaken(Unit* mAttacker, uint32 fAmount)
	{
		NULL_BINDING_CHECK

			lua_engine::BeginLuaFunctionCall(m_binding->refs[CREATURE_EVENT_ON_DAMAGE_TAKEN]);
		push_unit(_unit);
		push_int(CREATURE_EVENT_ON_DAMAGE_TAKEN);
		push_unit(mAttacker);
		push_int(fAmount);
		lua_engine::ExecuteLuaFunction(4);
		RELEASE_LOCK
	}
	void StringFunctionCall(int fRef)
	{

		NULL_BINDING_CHECK
		lua_engine::BeginLuaFunctionCall( (lua_function)fRef);
		push_unit(_unit);
		lua_engine::ExecuteLuaFunction(1);
		RELEASE_LOCK
	}
	void Destroy()
	{
		{
			li::CreatureInterfaceMap::iterator it;
			li::CreatureInterfaceMap::iterator itr = lua_instance->m_creatureInterfaceMap.find(_unit->GetEntry() ), itend = lua_instance->m_creatureInterfaceMap.upper_bound(_unit->GetEntry() );
			for(;itr != itend;)
			{
				it = itr++;
				if(it->second != NULL && it->second == this)
					lua_instance->m_creatureInterfaceMap.erase(it);
			}
		}
		{
			//Function Ref clean up
			li::ObjectFRefMap::iterator it, itr = lua_instance->m_creatureFRefs.find(_unit->GetLowGUID() );
			li::ObjectFRefMap::iterator itend = lua_instance->m_creatureFRefs.upper_bound(_unit->GetLowGUID() );
			while(itr != itend)
			{
				it = itr++;
				sEventMgr.RemoveEvents(_unit, itr->second->head_node->val.obj_ref+EVENT_LUA_CREATURE_EVENTS);
				cleanup_varparam(it->second, lua_state);
					
			}
		}
		delete this;
	}
};

namespace lua_engine
{
	CreatureAIScript * createluacreature(Creature * src)
	{
		LuaCreature * script = NULL;
		if(src != NULL)
		{
			uint32 id = src->GetEntry();
			li::ObjectBindingMap::iterator itr = lua_instance->m_unitBinding.find(id);
			PObjectBinding bind = (itr != lua_instance->m_unitBinding.end() ) ? itr->second : NULL;
			if( bind != NULL)
			{
				script = new LuaCreature(src);
				script->m_binding = bind;
				lua_instance->m_creatureInterfaceMap.insert( make_pair(id, script) );
			}
		}
		return script;
	}
	void bindCreatureMethods(luabridge::module & m )
	{
#define prop(name) .property_ro(#name, &CreatureProto::name)
		m	.class_<CreatureProto>("CreatureProto")
			prop(Id)
			prop(MinLevel)
			prop(MaxLevel)
			prop(Faction)
			prop(MinHealth)
			prop(MaxHealth)
			prop(Mana)
			prop(NPCFLags)
			prop(MinDamage)
			prop(MaxDamage)
			prop(boss)
			prop(walk_speed)
			prop(run_speed)
			prop(fly_speed)
			prop(isTrainingDummy)
			prop(guardtype)
			prop(summonguard);
#undef prop
#define prop(name) .property_ro(#name, &CreatureInfo::name)
		m	.class_<CreatureInfo>("CreatureInfo")
			prop(Id)
			prop(Name)
			prop(SubName)
			prop(Type)
			prop(Family)
			prop(Rank);
#undef prop

#define bind(name) .method(#name, &Creature::name)
		m	.class_decl<Creature>("Creature");
		m	.subclass<lua_creature, Unit>("Creature")
			bind(IsVehicle)
			//bind(isGuard)
			//bind(isNeutralGuard)
			.method("RemoveFromWorld", (void (Creature::*)(bool) )&Creature::RemoveFromWorld)
			bind(HasItems)
			bind(GetProto)
			bind(GetItemIdBySlot)
			bind(GetItemAmountBySlot)
			bind(GetSlotByItemId)
			bind(GetItemAmountByItemId)
			bind(HasQuests)
			bind(HasQuest)
			bind(AddQuest)
			bind(DeleteQuest)
			bind(FindQuest)
			bind(GetQuestRelation)
			bind(NumOfQuests)
			bind(isVendor)
			bind(isTrainer)
			bind(isClass)
			bind(isProf)
			bind(isQuestGiver)
			bind(isGossip)
			bind(isTaxi)
			bind(isCharterGiver)
			bind(isGuildBank)
			bind(isBattleMaster)
			bind(isBanker)
			bind(isInnkeeper)
			bind(isSpiritHealer)
			bind(isTabardDesigner)
			bind(isAuctioner)
			bind(isStableMaster)
			bind(isArmorer)
			.property_rw("canregeneratehp", &Creature::m_canRegenerateHP)
			bind(CanSee)
			.property_ro("isSkinned", &Creature::Skinned)
			bind(GetSQL_id)
			bind(GetTotemSlot)
			bind(GetGroup)
			bind(IsPickPocketed)
			bind(GetCreatureInfo)
			bind(SetCreatureProto)
			bind(IsTotem)
			bind(IsExotic)
			.method("IsCritter", &Creature::isCritter)
			.method("IsTrainingDummy", &Creature::isTrainingDummy)
			.method("Despawn", &Creature::Despawn)	
			.property_rw("norespawn", &Creature::m_noRespawn)
			.property_ro("escorter", &Creature::m_escorter)
			.method("RegisterEvent", &lua_creature::RegisterScriptEngineFunction)
			.method("RemoveEvents", &lua_creature::RemoveScriptEngineEvents);
#undef bind
#define BIND(name) .method(#name, &AIInterface::name)
			m.	class_<AIInterface>("AIInterface")
				/*BIND(getUnit)
				BIND(findClosestTarget)
				BIND(findRandomTarget)
				BIND(isMoving)*/
				BIND(GetPetOwner)
				BIND(SetPetOwner)
				.method("SetUnitToFollow", (void(AIInterface::*)(Unit*))&AIInterface::SetUnitToFollow)
				//BIND(SetUnitToFollow)
				BIND(getUnitToFollow)
				BIND(SetFollowDistance)
				//BIND(getFollowDistance)
				BIND(SetUnitToFollowAngle)
				//BIND(getFollowAngle)
				BIND(getUnitToFear)
				//BIND(SetUnitToFear)
				.method("SetUnitToFear", (void(AIInterface::*)(Unit*))&AIInterface::SetUnitToFear)
				/*BIND(setForcedTarget)
				BIND(getForcedTarget)
				BIND(hasForcedTarget)
				BIND(getPanicHp)
				BIND(setPanicHp)
				BIND(getPanicDuration)
				BIND(setPanicDuration)
				BIND(hasPanicked)
				BIND(getCallHelpHp)
				BIND(setCallHelpHp)
				BIND(hasCalledHelp)
				BIND(getCombatDisable)
				BIND(disableCombat)
				BIND(getMeleeDisable)
				BIND(disableMelee)
				BIND(getSpellDisable)
				BIND(disableSpells)
				BIND(getRangedDisable)
				BIND(disableRanged)
				BIND(getTargettingDisable)
				BIND(disableTargetting)*/
				BIND(getSoullinkedWith)
				BIND(GetIsSoulLinked)
				BIND(SetSoulLinkedWith)
				BIND(getAIState)
				//BIND(SetAIState)
				BIND(addSpellToList)
				//BIND(Spell_remove)
				//BIND(Spell_getnext)
				BIND(SetNextSpell)
				BIND(MoveTo)
				//BIND(calcMoveTimeToLocation)
				BIND(StopMovement)
				BIND(hasWaypoints)
				//BIND(Movement_allowmovement)
				.property_rw("canmove", &AIInterface::m_canMove)
				.property_rw("moveRun", &AIInterface::m_moveRun)
				.property_rw("moveFly", &AIInterface::m_moveFly)
				//BIND(Movement_canmove)
				//BIND(Movement_isflying)
				//BIND(calcAggroRange)
				//BIND(calcCombatRange)
				BIND(getNextTarget)
				.method("setNextTarget", (void(AIInterface::*)(Unit*) )&AIInterface::setNextTarget)
				BIND(AttackReaction)
				BIND(HealReaction);
#undef BIND
	}
}