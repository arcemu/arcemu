#ifndef __LUAFUNCTIONS_H
#define __LUAFUNCTIONS_H

/************************************************************************/
/* SCRIPT FUNCTIONS DECLARATIONS                                        */
/************************************************************************/
// Item Gossip Stuff
int luaItem_GossipCreateMenu(lua_State * L, Item * ptr);
int luaItem_GossipMenuAddItem(lua_State * L, Item * ptr);
int luaItem_GossipSendMenu(lua_State * L, Item * ptr);
int luaItem_GossipComplete(lua_State * L, Item * ptr);
int luaItem_GossipSendPOI(lua_State * L, Item * ptr);
// Unit Gossip Stuff
int luaUnit_GossipCreateMenu(lua_State * L, Unit * ptr);
int luaUnit_GossipMenuAddItem(lua_State * L, Unit * ptr);
int luaUnit_GossipSendMenu(lua_State * L, Unit * ptr);
int luaUnit_GossipComplete(lua_State * L, Unit * ptr);
int luaUnit_GossipSendPOI(lua_State * L, Unit * ptr);

int luaUnit_GetName(lua_State * L, Unit * ptr);
int luaUnit_SendChatMessage(lua_State * L, Unit * ptr);
int luaUnit_MoveTo(lua_State * L, Unit * ptr);
int luaUnit_SetMovementType(lua_State * L, Unit * ptr);
int luaUnit_CastSpell(lua_State * L, Unit * ptr);
int luaUnit_CastSpellOnTarget(lua_State * L, Unit * ptr);
int luaUnit_FullCastSpell(lua_State * L, Unit * ptr);
int luaUnit_FullCastSpellOnTarget(lua_State * L, Unit * ptr);
int luaUnit_SpawnGameObject(lua_State * L, Unit * ptr);
int luaUnit_SpawnCreature(lua_State * L, Unit * ptr);
int luaUnit_GetX(lua_State * L, Unit * ptr);
int luaUnit_GetY(lua_State * L, Unit * ptr);
int luaUnit_GetZ(lua_State * L, Unit * ptr);
int luaUnit_GetO(lua_State * L, Unit * ptr);
int luaUnit_IsPlayer(lua_State * L, Unit * ptr);
int luaUnit_IsCreature(lua_State * L, Unit * ptr);
int luaUnit_RegisterEvent(lua_State * L, Unit * ptr);
int luaUnit_RemoveEvents(lua_State * L, Unit * ptr);
int luaUnit_SendBroadcastMessage(lua_State * L, Unit * ptr);
int luaUnit_SendAreaTriggerMessage(lua_State * L, Unit * ptr);
int luaUnit_KnockBack(lua_State * L, Unit * ptr);
int luaUnit_MarkQuestObjectiveAsComplete(lua_State * L, Unit * ptr);
int luaUnit_LearnSpell(lua_State * L, Unit* ptr);
int luaUnit_UnlearnSpell(lua_State * L, Unit * ptr);
int luaUnit_HasFinishedQuest(lua_State * L, Unit * ptr);
int luaUnit_GetItemCount(lua_State * L, Unit * ptr);
int luaUnit_IsInCombat(lua_State * L, Unit * ptr);
int luaUnit_GetMainTank(lua_State * L, Unit * ptr);
int luaUnit_GetAddTank(lua_State * L, Unit * ptr);
int luaUnit_ClearThreatList(lua_State * L, Unit * ptr);
int luaUnit_GetTauntedBy(lua_State * L, Unit * ptr);
int luaUnit_SetTauntedBy(lua_State * L, Unit * ptr);
int luaUnit_GetSoulLinkedWith(lua_State * L, Unit * ptr);
int luaUnit_SetSoulLinkedWith(lua_State * L, Unit * ptr);
int luaUnit_ChangeTarget(lua_State * L, Unit * ptr);
int luaUnit_GetHealthPct(lua_State * L, Unit * ptr);
int luaUnit_GetManaPct(lua_State * L, Unit * ptr);
int luaUnit_Emote(lua_State * L, Unit * ptr);
int luaUnit_SetHealthPct(lua_State * L, Unit * ptr);
int luaUnit_Despawn(lua_State * L, Unit * ptr);
int luaUnit_GetUnitBySqlId(lua_State * L, Unit * ptr);
int luaUnit_PlaySoundToSet(lua_State * L, Unit * ptr);
int luaUnit_RemoveAura(lua_State * L, Unit * ptr);
int luaUnit_StopMovement(lua_State * L, Unit * ptr);
int luaUnit_GetInstanceID(lua_State * L, Unit * ptr);
int luaUnit_GetClosestPlayer(lua_State * L, Unit * ptr);
int luaUnit_GetRandomPlayer(lua_State * L, Unit * ptr);
int luaUnit_GetRandomFriend(lua_State * L, Unit * ptr);
int luaUnit_AddItem(lua_State * L, Unit * ptr);
int luaUnit_RemoveItem(lua_State * L, Unit * ptr);
int luaUnit_CreateCustomWaypointMap(lua_State * L, Unit * ptr);
int luaUnit_CreateWaypoint(lua_State * L, Unit * ptr);
int luaUnit_DestroyCustomWaypointMap(lua_State * L, Unit * ptr);
int luaUnit_MoveToWaypoint(lua_State * L, Unit * ptr);
int luaUnit_SetCombatCapable(lua_State * L, Unit * ptr);
int luaUnit_SetCombatMeleeCapable(lua_State * L, Unit * ptr);
int luaUnit_SetCombatRangedCapable(lua_State * L, Unit * ptr);
int luaUnit_SetCombatSpellCapable(lua_State * L, Unit * ptr);
int luaUnit_SetCombatTargetingCapable(lua_State * L, Unit * ptr);
int luaUnit_SetNPCFlags(lua_State * L, Unit * ptr);
int luaUnit_SetModel(lua_State * L, Unit * ptr);
int luaUnit_SetScale(lua_State * L, Unit * ptr);
int luaUnit_SetFaction(lua_State * L, Unit * ptr);
int luaUnit_SetStandState(lua_State * L, Unit * ptr);
int luaUnit_TeleportUnit(lua_State * L, Unit * ptr);

int luaUnit_GetPlayerClass(lua_State * L, Unit * ptr);
int luaUnit_ClearHateList(lua_State * L, Unit * ptr);
int luaUnit_WipeHateList(lua_State * L, Unit * ptr);
int luaUnit_WipeTargetList(lua_State * L, Unit * ptr);
int luaUnit_WipeCurrentTarget(lua_State * L, Unit * ptr);
int luaUnit_GetHealth(lua_State * L, Unit * ptr);
int luaUnit_GetMaxHealth(lua_State * L, Unit * ptr);
int luaUnit_SetHealth(lua_State * L, Unit * ptr);
int luaUnit_SetMaxHealth(lua_State * L, Unit * ptr);
int luaUnit_SetFieldFlags(lua_State * L, Unit * ptr);

int luaGameObject_GetName(lua_State * L, GameObject * ptr);
int luaGameObject_Teleport(lua_State * L, GameObject * ptr);
// GO Gossip Stuff
int luaGameObject_GossipCreateMenu(lua_State * L, GameObject * ptr);
int luaGameObject_GossipMenuAddItem(lua_State * L, GameObject * ptr);
int luaGameObject_GossipSendMenu(lua_State * L, GameObject * ptr);
int luaGameObject_GossipComplete(lua_State * L, GameObject * ptr);
int luaGameObject_GossipSendPOI(lua_State * L, GameObject * ptr);

int luaGameObject_SpawnCreature(lua_State * L, GameObject * ptr);
int luaGameObject_PlayCustomAnim(lua_State * L, GameObject * ptr);
int luaGameObject_PlaySoundToSet(lua_State * L, GameObject * ptr);

#endif



