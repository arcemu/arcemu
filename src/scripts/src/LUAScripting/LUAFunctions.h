/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008 Arcemu Team
 * Copyright (C) 2007 Moon++ <http://www.moonplusplus.com/>
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

#ifndef __LUAFUNCTIONS_H
#define __LUAFUNCTIONS_H

/************************************************************************/
/* SCRIPT FUNCTIONS DECLARATIONS                                        */
/************************************************************************/
//////////////////////////////////////////////////////////////////
///////////////////UNIT COMMAND LIST//////////////////////////////
//////////////////////////////////////////////////////////////////

///////////////////
///Gossip Stuff////
///////////////////
int luaItem_GossipCreateMenu(lua_State * L, Item * ptr);
int luaItem_GossipMenuAddItem(lua_State * L, Item * ptr);
int luaItem_GossipSendMenu(lua_State * L, Item * ptr);
int luaItem_GossipComplete(lua_State * L, Item * ptr);
int luaItem_GossipSendPOI(lua_State * L, Item * ptr);
int luaUnit_GossipCreateMenu(lua_State * L, Unit * ptr);
int luaUnit_GossipMenuAddItem(lua_State * L, Unit * ptr);
int luaUnit_GossipSendMenu(lua_State * L, Unit * ptr);
int luaUnit_GossipComplete(lua_State * L, Unit * ptr);
int luaUnit_GossipSendPOI(lua_State * L, Unit * ptr);
int luaGameObject_GossipCreateMenu(lua_State * L, GameObject * ptr);
int luaGameObject_GossipMenuAddItem(lua_State * L, GameObject * ptr);
int luaGameObject_GossipSendMenu(lua_State * L, GameObject * ptr);
int luaGameObject_GossipComplete(lua_State * L, GameObject * ptr);
int luaGameObject_GossipSendPOI(lua_State * L, GameObject * ptr);
////////////
//GetStuff//
////////////

;int luaUnit_GetPlayerRace(lua_State * L, Unit * ptr);
int luaUnit_GetCurrentSpellId(lua_State * L, Unit * ptr);
int luaUnit_GetStanding(lua_State * L, Unit * ptr);
int luaUnit_GetMainTank(lua_State * L, Unit * ptr);
int luaUnit_GetAddTank(lua_State * L, Unit * ptr);
int luaUnit_GetX(lua_State * L, Unit * ptr);
int luaUnit_GetY(lua_State * L, Unit * ptr);
int luaUnit_GetZ(lua_State * L, Unit * ptr);
int luaUnit_GetO(lua_State * L, Unit * ptr);
int luaUnit_GetTauntedBy(lua_State * L, Unit * ptr);
int luaUnit_GetSoulLinkedWith(lua_State * L, Unit * ptr);
int luaUnit_GetItemCount(lua_State * L, Unit * ptr);
int luaUnit_GetName(lua_State * L, Unit * ptr);
int luaUnit_GetHealthPct(lua_State * L, Unit * ptr);
int luaUnit_GetManaPct(lua_State * L, Unit * ptr);
int luaUnit_GetInstanceID(lua_State * L, Unit * ptr);
int luaUnit_GetClosestPlayer(lua_State * L, Unit * ptr);
int luaUnit_GetRandomPlayer(lua_State * L, Unit * ptr);
int luaUnit_GetRandomFriend(lua_State * L, Unit * ptr);
int luaUnit_GetUnitBySqlId(lua_State * L, Unit * ptr);
int luaUnit_GetPlayerClass(lua_State * L, Unit * ptr);
int luaUnit_GetHealth(lua_State * L, Unit * ptr);
int luaUnit_GetMaxHealth(lua_State * L, Unit * ptr);
int luaUnit_GetCreatureNearestCoords(lua_State * L, Unit * ptr);
int luaUnit_GetGameObjectNearestCoords(lua_State *L, Unit * ptr);
int luaUnit_GetDistance(lua_State * L, Unit * ptr);
int luaUnit_GetGUID(lua_State * L, Unit * ptr);
int luaUnit_GetZoneId(lua_State *L, Unit * ptr);
int luaUnit_GetMaxMana(lua_State * L, Unit * ptr);
int luaUnit_GetMana(lua_State * L, Unit * ptr);
int luaUnit_GetCurrentSpell(lua_State * L, Unit * ptr);
int luaUnit_GetSpawnO(lua_State * L, Unit * ptr);
int luaUnit_GetSpawnZ(lua_State * L, Unit * ptr);
int luaUnit_GetSpawnY(lua_State * L, Unit * ptr);
int luaUnit_GetSpawnX(lua_State * L, Unit * ptr);
int luaUnit_GetInRangePlayersCount(lua_State * L, Unit * ptr);
int luaUnit_GetUInt32Value(lua_State * L, Unit * ptr);
int luaUnit_GetUInt64Value(lua_State * L, Unit * ptr);
int luaUnit_GetFloatValue(lua_State * L, Unit * ptr);
int luaUnit_GetAIState(lua_State * L, Unit * ptr);
int luaUnit_GetCurrentSpell(lua_State * L, Unit * ptr);
int luaUnit_GetInRangeGameObjects(lua_State * L, Unit * ptr);
int luaUnit_GetInRangePlayers(lua_State * L, Unit * ptr);
int luaUnit_GetAITargets(lua_State * L, Unit * ptr);
int luaUnit_GetUnitByGUID(lua_State * L, Unit * ptr);
int luaUnit_GetInRangeObjectsCount(lua_State * L, Unit * ptr);
int luaUnit_GetAITargetsCount(lua_State * L, Unit * ptr);
int luaUnit_GetUnitToFollow(lua_State * L, Unit * ptr);
int luaUnit_GetNextTarget(lua_State * L, Unit * ptr);
int luaUnit_GetPetOwner(lua_State * L, Unit * ptr);
int luaUnit_GetEntry(lua_State * L, Unit * ptr);
int luaUnit_GetFaction(lua_State * L, Unit *ptr);
int luaUnit_GetThreatByPtr(lua_State * L, Unit * ptr);
int luaUnit_GetInRangeFriends(lua_State * L, Unit * ptr);
int luaUnit_GetPowerType(lua_State * L, Unit * ptr);
int luaUnit_GetMapId(lua_State * L, Unit * ptr);
int luaUnit_GetFactionStanding(lua_State * L, Unit * ptr);
int luaUnit_GetPlayerLevel(lua_State * L, Unit * ptr);


////////////////////////
//Bools(Is? commands)///
////////////////////////
int luaUnit_IsPlayerAttacking(lua_State * L, Unit * ptr);
int luaUnit_IsPlayerMoving(lua_State * L, Unit * ptr);
//int luaUnit_IsPlayerAtWar(lua_State * L, Unit * ptr);
int luaUnit_IsPlayer(lua_State * L, Unit * ptr);
int luaUnit_IsCreature(lua_State * L, Unit * ptr);
int luaUnit_IsInCombat(lua_State * L, Unit * ptr);
int luaUnit_IsAlive(lua_State * L, Unit * ptr);
int luaUnit_IsDead(lua_State * L, Unit * ptr);
int luaUnit_IsInWorld(lua_State * L, Unit * ptr);
int luaUnit_IsCreatureMoving(lua_State * L, Unit * ptr );
int luaUnit_IsFlying(lua_State * L, Unit * ptr);
int luaUnit_IsInFront(lua_State * L, Unit * ptr);
int luaUnit_IsInBack(lua_State * L, Unit * ptr);
int luaUnit_IsPacified(lua_State * L, Unit * ptr);
int luaUnit_IsFeared(lua_State * L, Unit * ptr);
int luaUnit_IsStunned(lua_State * L, Unit * ptr);
int luaUnit_HasInRangeObjects(lua_State * L, Unit * ptr);
int luaUnit_IsInWater(lua_State * L, Unit * ptr);
//int luaUnit_IsInArc(lua_State * L, Unit * ptr);
int luaUnit_IsPet(lua_State * L, Unit * ptr);
int luaUnit_MoveFly(lua_State * L, Unit * ptr);
int luaUnit_NoRespawn(lua_State * L, Unit * ptr);
int luaUnit_HasItem(lua_State * L, Unit * ptr);
int luaUnit_FlyCheat(lua_State * L, Unit * ptr);

//////////////////////
///////OTHERS/////////
//////////////////////
int luaUnit_AdvanceSkill(lua_State * L, Unit * ptr);
int luaUnit_AddSkill(lua_State * L, Unit * ptr);
int luaUnit_RemoveSkill(lua_State * L, Unit * ptr);
int luaUnit_PlaySpellVisual(lua_State * L, Unit * ptr);
int luaUnit_RemoveThreatByPtr(lua_State * L, Unit * ptr);
int luaUnit_EventCastSpell(lua_State * L, Unit * ptr);
int luaUnit_AttackReaction(lua_State * L, Unit * ptr);
int luaUnit_DismissPet(lua_State * L, Unit * ptr);
int luaUnit_HandleEvent(lua_State * L, Unit * ptr);
int luaUnit_SetMoveRunFlag(lua_State * L, Unit * ptr);
int luaUnit_SendChatMessage(lua_State * L, Unit * ptr);
int luaUnit_MoveTo(lua_State * L, Unit * ptr);
int luaUnit_SetMovementType(lua_State * L, Unit * ptr);
int luaUnit_CastSpell(lua_State * L, Unit * ptr);
int luaUnit_CastSpellOnTarget(lua_State * L, Unit * ptr);
int luaUnit_FullCastSpell(lua_State * L, Unit * ptr);
int luaUnit_FullCastSpellOnTarget(lua_State * L, Unit * ptr);
int luaUnit_SpawnGameObject(lua_State * L, Unit * ptr);
int luaUnit_SpawnCreature(lua_State * L, Unit * ptr);
int luaUnit_RegisterEvent(lua_State * L, Unit * ptr);
int luaUnit_RemoveEvents(lua_State * L, Unit * ptr);
int luaUnit_SendBroadcastMessage(lua_State * L, Unit * ptr);
int luaUnit_SendAreaTriggerMessage(lua_State * L, Unit * ptr);
int luaUnit_KnockBack(lua_State * L, Unit * ptr);
int luaUnit_MarkQuestObjectiveAsComplete(lua_State * L, Unit * ptr);
int luaUnit_LearnSpell(lua_State * L, Unit* ptr);
int luaUnit_UnlearnSpell(lua_State * L, Unit * ptr);
int luaUnit_HasFinishedQuest(lua_State * L, Unit * ptr);
int luaUnit_ClearThreatList(lua_State * L, Unit * ptr);
int luaUnit_ChangeTarget(lua_State * L, Unit * ptr);
int luaUnit_Emote(lua_State * L, Unit * ptr);
int luaUnit_Despawn(lua_State * L, Unit * ptr);
int luaUnit_PlaySoundToSet(lua_State * L, Unit * ptr);
int luaUnit_RemoveAura(lua_State * L, Unit * ptr);
int luaUnit_StopMovement(lua_State * L, Unit * ptr);
int luaUnit_AddItem(lua_State * L, Unit * ptr);
int luaUnit_RemoveItem(lua_State * L, Unit * ptr);
int luaUnit_CreateCustomWaypointMap(lua_State * L, Unit * ptr);
int luaUnit_CreateWaypoint(lua_State * L, Unit * ptr);
int luaUnit_DestroyCustomWaypointMap(lua_State * L, Unit * ptr);
int luaUnit_MoveToWaypoint(lua_State * L, Unit * ptr);
int luaUnit_TeleportUnit(lua_State * L, Unit * ptr);
int luaUnit_ClearHateList(lua_State * L, Unit * ptr);
int luaUnit_WipeHateList(lua_State * L, Unit * ptr);
int luaUnit_WipeTargetList(lua_State * L, Unit * ptr);
int luaUnit_WipeCurrentTarget(lua_State * L, Unit * ptr);
int luaUnit_CastSpellAoF(lua_State * L, Unit * ptr);
int luaUnit_RemoveAllAuras(lua_State *L, Unit * ptr);
int luaUnit_StopChannel(lua_State * L, Unit * ptr);
int luaUnit_ChannelSpell(lua_State * L, Unit * ptr);
int luaUnit_ReturnToSpawnPoint(lua_State * L, Unit * ptr);
int luaUnit_HasAura(lua_State * L, Unit * ptr);
int luaUnit_Land(lua_State * L, Unit * ptr);
int luaUnit_CancelSpell(lua_State * L, Unit * ptr);
int luaUnit_Root(lua_State * L, Unit * ptr);
int luaUnit_Unroot(lua_State * L, Unit * ptr);
int luaUnit_CalcDistance(lua_State * L, Unit * ptr);
int luaUnit_ModUInt32Value(lua_State * L, Unit * ptr);
int luaUnit_ModFloatValue(lua_State * L, Unit * ptr);
int luaUnit_SendData(lua_State * L, Unit * ptr);
int luaUnit_InitPacket(lua_State * L, Unit * ptr);
int luaUnit_AddDataToPacket(lua_State * L, Unit * ptr);
int luaUnit_AddGuidDataToPacket(lua_State * L, Unit * ptr);
int luaUnit_AdvanceQuestObjective(lua_State * L, Unit * ptr);
int luaUnit_Heal(lua_State * L, Unit * ptr);
int luaUnit_Energize(lua_State * L, Unit * ptr);
int luaUnit_SendChatMessageAlternateEntry(lua_State * L, Unit * ptr);
int luaUnit_SendChatMessageToPlayer(lua_State * L, Unit * ptr);
int luaUnit_Strike(lua_State * L, Unit * ptr);
int luaUnit_Kill(lua_State * L, Unit * ptr);
int luaUnit_DealDamage(lua_State * L, Unit * ptr);
int luaUnit_CreateGuardian(lua_State * L, Unit * ptr);
int luaUnit_CalcToDistance(lua_State * L, Unit * ptr);
int luaUnit_CalcAngle(lua_State * L, Unit * ptr);
int luaUnit_CalcRadAngle(lua_State * L, Unit * ptr);
int luaUnit_IsInvisible(lua_State * L, Unit * ptr);
int luaUnit_IsInvincible(lua_State * L, Unit * ptr);
int luaUnit_ResurrectPlayer(lua_State * L, Unit * ptr);
int luaUnit_KickPlayer(lua_State * L, Unit * ptr);
int luaUnit_CanCallForHelp(lua_State * L, Unit * ptr);
int luaUnit_CallForHelpHp(lua_State * L, Unit * ptr);
int luaUnit_RemoveFromWorld(lua_State * L, Unit * ptr);
int luaUnit_SpellNonMeleeDamageLog(lua_State * L, Unit * ptr);
int luaUnit_ModThreat(lua_State * L, Unit * ptr);
int luaUnit_AddAssistTargets(lua_State * L, Unit * ptr);
int luaUnit_RemoveAurasByMechanic(lua_State * L, Unit * ptr);
int luaUnit_RemoveAurasType(lua_State * L, Unit * ptr);
int luaUnit_AddAuraVisual(lua_State * L, Unit * ptr);


/////////////////////
//////SetStuff///////
/////////////////////

int luaUnit_SetPlayerStanding(lua_State * L, Unit * ptr);
int luaUnit_SetPlayerLevel(lua_State * L, Unit * ptr);
int luaUnit_SetPlayerAtWar(lua_State * L, Unit * ptr);
int luaUnit_SetCreatureName(lua_State * L, Unit * ptr);
int luaUnit_SetDeathState(lua_State * L, Unit * ptr);
int luaUnit_SetPowerType(lua_State * L, Unit * ptr);
int luaUnit_SetAttackTimer(lua_State * L, Unit * ptr);
int luaUnit_SetMana(lua_State * L, Unit * ptr);
int luaUnit_SetMaxMana(lua_State * L, Unit * ptr);
int luaUnit_SetHealth(lua_State * L, Unit * ptr);
int luaUnit_SetMaxHealth(lua_State * L, Unit * ptr);
int luaUnit_SetFlying(lua_State * L, Unit * ptr);
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
int luaUnit_SetTauntedBy(lua_State * L, Unit * ptr);
int luaUnit_SetSoulLinkedWith(lua_State * L, Unit * ptr);
int luaUnit_SetInFront(lua_State * L, Unit * ptr);
int luaUnit_SetHealthPct(lua_State * L, Unit * ptr);
int luaUnit_SetOutOfCombatRange(lua_State * L, Unit * ptr);
int luaUnit_ModifyRunSpeed(lua_State * L, Unit * ptr);
int luaUnit_ModifyWalkSpeed(lua_State * L, Unit * ptr);
int luaUnit_ModifyFlySpeed(lua_State * L, Unit * ptr);
int luaUnit_SetRotation(lua_State * L, Unit * ptr);
int luaUnit_SetOrientation(lua_State * L, Unit * ptr);
int luaUnit_SetUInt32Value(lua_State * L, Unit * ptr);
int luaUnit_SetUInt64Value(lua_State * L, Unit * ptr);
int luaUnit_SetFloatValue(lua_State * L, Unit * ptr);
int luaUnit_SetUnitToFollow(lua_State * L, Unit * ptr);
int luaUnit_SetNextTarget(lua_State * L, Unit * ptr);
int luaUnit_SetPetOwner(lua_State * L, Unit * ptr);
int luaUnit_SetFacing(lua_State * L, Unit * ptr);

////////////////////////////////////////////////////////////
/////////////////END UNIT COMMAND LIST//////////////////////
////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
/////////////////////GAMEOBJECT COMMAND LIST///////////////
///////////////////////////////////////////////////////////

//////////////////
/////GET STUFF////
//////////////////
int luaGameObject_GetName(lua_State * L, GameObject * ptr);
int luaGameObject_GetMapId(lua_State * L, GameObject * ptr);
int luaGameObject_GetCreatureNearestCoords(lua_State * L, GameObject * ptr);
int luaGameObject_GetGameObjectNearestCoords(lua_State *L, GameObject * ptr);
//int luaGameObject_GetAreaID(lua_State * L, GameObject * ptr);
int luaGameObject_GetClosestPlayer(lua_State * L, GameObject * ptr);
int luaGameObject_GetZoneId(lua_State *L, GameObject * ptr);
int luaGameObject_GetItemCount(lua_State * L, GameObject * ptr);
int luaGameObject_GetSpawnX(lua_State * L, GameObject * ptr);
int luaGameObject_GetSpawnY(lua_State * L, GameObject * ptr);
int luaGameObject_GetSpawnZ(lua_State * L, GameObject * ptr);
int luaGameObject_GetSpawnO(lua_State * L, GameObject * ptr);
int luaGameObject_GetInRangePlayersCount(lua_State * L, GameObject * ptr);
int luaGameObject_GetEntry(lua_State * L, GameObject * ptr);
int luaGameObject_GetX(lua_State * L, GameObject * ptr);
int luaGameObject_GetY(lua_State * L, GameObject * ptr);
int luaGameObject_GetZ(lua_State * L, GameObject * ptr);
int luaGameObject_GetO(lua_State * L, GameObject * ptr);
int luaGameObject_GetInRangePlayers(lua_State * L, GameObject * ptr);
int luaGameObject_GetInRangeGameObjects(lua_State * L, GameObject * ptr);
int luaGameObject_GetInstanceID(lua_State * L, GameObject * ptr);
int luaGameObject_GetUInt64Value(lua_State * L, GameObject * ptr);
int luaGameObject_GetUInt32Value(lua_State * L, GameObject * ptr);
int luaGameObject_GetFloatValue(lua_State * L, GameObject * ptr);
int luaGameObject_GetGUID(lua_State * L, GameObject* ptr);

//////////////////
//////END/////////
/////////////////

int luaGameObject_Teleport(lua_State * L, GameObject * ptr);
int luaGameObject_AddItem(lua_State * L, GameObject * ptr);
int luaGameObject_Despawn(lua_State * L, GameObject * ptr);
int luaGameObject_IsInWorld(lua_State * L, GameObject * ptr);
int luaGameObject_IsInBack(lua_State * L, GameObject * ptr);
int luaGameObject_IsInFront(lua_State * L, GameObject * ptr);
int luaGameObject_PlaySoundToSet(lua_State * L, GameObject * ptr);
int luaGameObject_SpawnCreature(lua_State * L, GameObject * ptr);
int luaGameObject_SpawnGameObject(lua_State * L, GameObject * ptr);
int luaGameObject_CalcDistance(lua_State * L, GameObject * ptr);
int luaGameObject_SetOrientation(lua_State * L, GameObject * ptr);
int luaGameObject_RemoveFromWorld(lua_State * L, GameObject * ptr);
int luaGameObject_CalcRadAngle(lua_State * L, GameObject * ptr);
int luaGameObject_SetUInt32Value(lua_State * L, GameObject * ptr);
int luaGameObject_SetUInt64Value(lua_State * L, GameObject * ptr);
int luaGameObject_SetFloatValue(lua_State * L, GameObject * ptr);
int luaGameObject_ModUInt32Value(lua_State * L, GameObject * ptr);
int luaGameObject_CastSpell(lua_State * L, GameObject * ptr);
int luaGameObject_FullCastSpell(lua_State * L, GameObject * ptr);
int luaGameObject_CastSpellOnTarget(lua_State * L, GameObject * ptr);
int luaGameObject_FullCastSpellOnTarget(lua_State * L, GameObject * ptr);
//int luaGameObject_EventCastSpell(lua_State * L, GameObject * ptr);

/*
int luaGameObject_GossipObjectCreateMenu(lua_State * L, GameObject * ptr);
int luaGameObject_GossipObjectMenuAddItem(lua_State * L, GameObject * ptr);
int luaGameObject_GossipObjectSendMenu(lua_State * L, GameObject * ptr);
int luaGameObject_GossipObjectComplete(lua_State * L, GameObject * ptr);
int luaGameObject_GossipObjectSendPOI(lua_State * L, GameObject * ptr);
int luaGameObject_ModUInt32Value(lua_State * L, GameObject * ptr);
int luaGameObject_ModFloatValue(lua_State * L, GameObject * ptr);
int luaGameObject_GetFloatValue(lua_State * L, GameObject * ptr);
int luaGameObject_InitPacket(lua_State * L, GameObject * ptr);
int luaGameObject_AddDataToPacket(lua_State * L, GameObject * ptr);
int luaGameObject_AddGuidDataToPacket(lua_State * L, GameObject * ptr);
int luaGameObject_SendData(lua_State * L, GameObject * ptr);
*/

//////////////////////////////////////////////////
////////////////END GAMEOBJECT COMMAND LIST//////////////////
//////////////////////////////////////////////////


#endif


