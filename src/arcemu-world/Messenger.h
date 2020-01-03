/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2019 <http://www.ArcEmu.org/>
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

#ifndef _MESSENGER_H
#define _MESSENGER_H

class Object;

class Messenger
{
public:
	/// Log miss, block, etc for spell based attacks in the client
	static void SendSpellLog( Object* Caster, Object* Target, uint32 Ability, uint8 SpellLogType );

	/// Log spell damage in the client
	static void SendSpellNonMeleeDamageLog( Object* Caster, Object* Target, uint32 SpellID, uint32 Damage, uint8 School, uint32 AbsorbedDamage, uint32 ResistedDamage, bool PhysicalDamage, uint32 BlockedDamage, bool CriticalHit, bool bToSet );

	/// Log melee damage in the client
	static void SendAttackerStateUpdate( Object* Caster, Object* Target, dealdamage* Dmg, uint32 Damage, uint32 Abs, uint32 BlockedDamage, uint32 HitStatus, uint32 VState );

	/// Log environmental damage in the client (e.g.: fall, fire, lava daamage)
	static void SendEnvironmentalDamageLog( Unit* unit, uint8 type, uint32 damage );

	/// Log damageshield damage in the client (e.g.: Fiery aura damages on damage)
	static void SendDamageShieldLog( Unit* unit, Unit* attacker, const DamageProc &ds );

	/// Plays a sound effect to the players around the object
	static void PlaySoundToSet( Object* object, uint32 sound_entry );

	/// Plays a sound effect to the player
	static void PlaySoundToPlayer( Player* player, uint32 soundId );

	/// Send AI reaction to client ("aggro sound" like growling)
	static void SendAIReaction( Object* object, uint32 reaction = 2 );

	/// Destroys this Object for the players' clients that are nearby ( client removes object from the screen )
	static void SendDestroyObjectToSet( Object* object );

	/// Sends a gameobject animation to the player
	static void SendGameObjectCustomAnim( GameObject* go, uint32 anim, Player* player );

	/// Sends the gameobject despawn animation. Removes gameobject from client screen.
	static void SendGameObjectDespawnAnim( Object *object );

	/// Destructible building damage. Notifies the surrounding clients about the GameObject taking damage.
	static void SendBuildingDamageToSet( GameObject* go, uint32 damage, uint64 AttackerGUID, uint64 ControllerGUID, uint32 SpellID );

	/// Shows the loading screen when switching maps for transports
	static void SendTransferPendingTransport( Player* player, uint32 oldMap, uint32 newMap, uint32 transportEntry );

	/// Shows the loading screen when switching maps
	static void SendTransferPending( Player *player, uint32 mapId );

	/// Finishes the loading on the loading screen
	static void SendNewWorld( Player* player, uint32 mapId, const LocationVector &location );

	/// Aborts the loading screen, and displays a message as to why
	static void SendTransferAborted( Player* player, uint32 mapId, uint32 cause );

	/// Tells the client to move the player somewhere within the current map
	static void SendTeleportAck( Player *player, const LocationVector &location );

	/// Tells the client to play a cinematic
	static void SendTriggerCinematic( Player* player, uint32 cinematicId );

	/// Tells the client to stop the attack animation.
	static void SendStopAttackToSet( Unit* attacker, Unit* victim );

	/// Tells the client to stop the attack animation.
	static void SendStopAttackToSet( Unit* attacker, uint64 victim );

	/// Tells the client to stop the attack animation.
	static void SendStopAttackToPlayer( Player* player, Unit* victim );

	/// Tells the client to start the attack animation.
	static void SendStartAttackToSet( Unit* attacker, Unit* victim );

	/// Root the unit. It will not be able to move.
	static void SendRootToSet( Unit* unit );

	/// Unroot the unit. It will be able to move again.
	static void SendUnRootToSet( Unit* unit );

	/// Unused in Arcemu
	static void SendPlaySpellVisualToSet( Unit* target, uint32 spellVisual );

	/// Shows emote
	static void SendEmoteMessageToSet( Unit* target, uint32 emote );

	/// Power update (mana,rage,etc bar)
	static void SendPowerUpdate( Unit* unit, uint32 amount, bool self );

	/// Send all auras to the client
	static uint32 SendFullAuraUpdate( Unit* unit );

	/// Send an update about just one aura to the client
	static void SendAuraUpdate( Unit* unit, uint32 AuraSlot, bool remove );

	/// Enables flight in the client
	static void SendEnableFlightMessage( Unit* unit );

	/// Disables flight in the client
	static void SendDisableFlightMessage( Unit* unit );

	/// Sends aura log entry to the client ( abc suffers N school damage from xyz's spell )
	static void SendPeriodicAuraLog( const WoWGuid & CasterGUID, Unit* target, uint32 SpellID, uint32 School, uint32 Amount, uint32 abs_dmg, uint32 resisted_damage, uint32 Flags, bool is_critical );

	/// Sends a heal aura log entry to the client ( abc gains N heealth from xyz's spell )
	static void SendPeriodicHealAuraLog( const WoWGuid & CasterGUID, Unit* target, uint32 SpellID, uint32 healed, uint32 over_healed, bool is_critical );

	/// Hop on vehicle animation
	static void SendHopOnVehicle( Unit* unit, Unit* vehicleowner, uint32 seat );

	/// Hop off vehicle animation
	static void SendHopOffVehicle( Unit* unit, Unit *vehicleowner, LocationVector &landposition );

	/// Sends a message that sets the facing of the Unit in the client
	static void SendSetFacing( Unit* unit, float orientation );

	/// Sends a chat message to the Unit's set. Used for making NPCs say things
	static void SendChatMessageToSet( Unit* unit, uint8 emote, uint32 lang, const string& name, const string& message );

	/// Sends a chat message to a player. Used for making NPCs say things
	static void SendChatMessageToPlayer( Unit* unit, Player* player, uint8 emote, uint32 lang, const string& name, const string& message );

	/// Sets the speed of the object in the client
	static void SendSetSpeed( Object* object, uint8 type, float speed );

	static void SendEmptyPetSpellsToPlayer( Player* player );

	/// Tells the client a pet spell cast has failed
	static void SendPetCastFailed( Player* player, uint32 spellid, uint8 fail );

	/// Tells the client if the pet cannot do something
	static void SendPetActionFeedback( Player* player, PetActionFeedback feedback );

	/// Send the pet spellbar to the player
	static void SendPetSpells( Player* player, Pet* pet );

	/// Sends the pet talents to the client
	static void SendPetTalents( Player* player, uint32 pointsleft, const vector< pair< uint32, uint8 > > talents );

	/// Sends the spellbar of this creature to the players
	static void SendCreatureSpells( Player* player, Creature* creature );

	/// Updates the item's remaining duration time in the client
	static void SendItemDurationUpdate( Player* player, uint64 guid, uint32 remainingTime );

	/// Sends a log entry for the client about the player enchanting an item
	static void SendEnchantmentLog( Player* player, uint32 itemId, uint32 enchantmentId );

	/// Sends the time left for expiring enchants
	static void SendEnchantTimeUpdate( Player* player, uint64 itemGUID, uint32 slot, uint32 duration );

	/// Sent when the player enters a vehicle
	static void SendVehicleControl( Player* player );

	/// Sent when the player enters or exits a vehicle (value is 1 on enter, 0 on exit), or posesses. "Switch mover"
	static void SendClientControlUpdate( Player* player, const WoWGuid& guid, uint8 value );

	/// Sends an inventory change error message to the client
	static void SendInventoryChangeError( Player* player, uint64 sourceItem, uint64 destinationItem, uint8 error, uint32 requiredLevel );

	/// Tells the client that the player has learned a new spell?
	static void SendLearnedSpell( Player* player, uint32 spell );

	/// According to comments it tells the client the player can now move.
	static void SendTimeSyncRequest( Player* player );

	/// Sets the player movement type in the client
	static void SendSetPlayerMovementType( Player* player, uint8 type, uint32 flags );

	/// Set player speeds in the client
	static void SendSetPlayerSpeeds( Player* player, uint8 type, float speed, uint32 changeCount );

	/// Sent before repopping at a graveyard
	static void SendPreResurrect( Player* player );

	/// Sent after repopping, so probably spirit healer location
	static void SendDeathReleaseLocation( Player* player, uint32 mapid, const LocationVector &location );

	/// Tells the client how long the player has to wait until resurrection
	static void SendCorpseReclaimDelay( Player* player, uint32 delay );

	/// Removes a spell from the spellbook
	static void SendRemoveSpell( Player* player, uint32 spell );

	/// Overwrites a spell in the spellbook
	static void SendSupersedeSpell( Player* player, uint32 oldspell, uint32 newspell );

	/// Clears the cooldown on the spell in the client
	static void SendClearSpellCooldown( Player* player, uint32 spell );

	/// Pops up the duel request dialog in the client
	static void SendDuelRequest( Player* challengedPlayer, uint64 challengerGUID, uint64 flagGUID );

	/// Announces the duel winner
	static void SendDuelWinner( Player* winner, const char* loserName, uint8 condition );

	/// Completes the duel
	static void SendDuelComplete( Player* winner, bool broadcast );

	/// Tells the client the player has earned a title
	static void SendTitleEarned( Player* player, uint32 title, uint32 set );

	/// Tells the client that the player has explored a new area, and earned XP for it
	static void SendExploreXP( Player* player, uint32 area, uint32 xp );

	/// Pops up the summon request dialog in the client
	static void SendSummonRequest( Player* player, uint64 summonerGUID, uint32 zoneId, uint32 time );

	/// Tells the client to show a movie
	static void SendTriggerMovie( Player* player, uint32 movie );

	static void SendMeetingStoneSetQueue( Player *player, uint32 dungeon, uint8 status );

	/// Sends a Message Of The Day type message to the client
	static void SendMOTD( Player* player, uint32 value, const char* text );
};

#endif
