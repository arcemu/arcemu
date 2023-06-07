/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2022 <http://www.ArcEmu.org/>
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
 
 #ifndef SERVERHOOK_HANDLER_H_
 #define SERVERHOOK_HANDLER_H_

/// Implements server event hook handlers.
/// These are basically global functions that Arcemu core calls when something happens.
/// Each of the methods call the Python function(s) that are registered to handle them
class ServerHookHandler
{
public:
	static bool hookOnNewCharacter( uint32 charRace, uint32 charClass, WorldSession* session, const char *name );
	static void hookOnKillPlayer( Player* killer, Player *victim );
	static void hookOnFirstEnterWorld( Player *player );
	static void hookOnEnterWorld( Player* player );
	static void hookOnGuildJoin( Player* player, Guild* guild );
	static void hookOnPlayerDeath( Player* player );
	static bool hookOnPlayerRepop( Player* player );
	static void hookOnEmote( Player* player, uint32 emote, Unit* unit );
	static void hookOnEnterCombat( Player* player, Unit* unit );
	static bool hookOnCastSpell( Player* player, SpellEntry* spe, Spell* spell );
	static bool hookOnLogoutRequest( Player* player );
	static void hookOnLogout( Player* player );
	static void hookOnAcceptQuest( Player* player, Quest* quest, Object* questGiver );
	static void hookOnZoneChange( Player* player, uint32 oldZone, uint32 newZone );
	static bool hookOnChatMessage( Player* player, uint32 type, uint32 lang, const char* message, const char* misc );
	static void hookOnLoot( Player* player, Unit* unit, uint32 money, uint32 itemId );
	static void hookOnGuildCreate( Player* leader, Guild* guild );
	static void hookOnFullLogin( Player* player );
	static void hookOnCharacterCreated( Player* player );
	static void hookOnQuestCancelled( Player* player, Quest* quest );
	static void hookOnQuestFinished( Player* player, Quest* quest, Object* questFinisher );
	static void hookOnHonorableKill( Player* killer, Player* victim );
	static void hookOnArenaFinish( Player* player, ArenaTeam* arenaTeam, bool victory, bool rated );
	static void hookOnObjectLoot( Player* player, Object* target, uint32 money, uint32 itemId );
	static void hookOnAreaTrigger( Player* player, uint32 areaTriggerId );
	static void hookOnLevelUp( Player* player );
	static bool hookOnPreUnitDie( Unit* killer, Unit *victim );
	static void hookOnAdvanceSkillLine( Player* player, uint32 skill, uint32 value );
	static void hookOnDuelFinished( Player* winner, Player* loser );
	static void hookOnAuraRemove( Aura* aura );
	static bool hookOnPlayerResurrect( Player* player );
	static void hookOnTransportArrived( GameObject *go, uint32 route );
};

#endif
