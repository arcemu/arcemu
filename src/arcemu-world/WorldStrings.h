/*
* EasyWoW MMORPG Server
* Copyright (C) 2014 <http://github.com/EasyWoW>
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
 
#ifndef WorldStrings
#define WorldStrings
 
enum WorldStringsEntries
{
    WORLDSTRING_BROWSE_GOODS                    = 1,    //I would like to browse your goods.
    WORLDSTRING_I_SEEK                          = 2,    //I seek
    WORLDSTRING_MAGE                            = 3,    //mage
    WORLDSTRING_SHAMAN                          = 4,    //shaman
    WORLDSTRING_WARRIOR                         = 5,    //warrior
    WORLDSTRING_PALADIN                         = 6,    //paladin
    WORLDSTRING_WARLOCK                         = 7,    //warlock
    WORLDSTRING_HUNTER                          = 8,    //hunter
    WORLDSTRING_ROGUE                           = 9,    //rogue
    WORLDSTRING_DRUID                           = 10,   //druid
    WORLDSTRING_PRIEST                          = 11,   //priest
    WORLDSTRING_TRAINING                        = 12,   //training
    WORLDSTRING_TRAIN_BEAST                     = 13,   //Train me in the ways of the beast.
    WORLDSTRING_GIVE_RIDE                       = 14,   //Give me a ride.
    WORLDSTRING_MAKE_BID                        = 15,   //I would like to make a bid.
    WORLDSTRING_INN                             = 16,   //Make this inn your home.
    WORLDSTRING_CHECK_DEPOSIT_BOX               = 17,   //I would like to check my deposit box.
    WORLDSTRING_BRING_BACK_TO_LIFE              = 18,   //Bring me back to life.
    WORLDSTRING_HOW_TO_CREATE_GUILD             = 19,   //How do I create a guild/arena team?
    WORLDSTRING_CREATE_GUILD_CREST              = 20,   //I want to create a guild crest.
    WORLDSTRING_JOIN_BG                         = 21,   //I would like to go to the battleground.
    WORLDSTRING_RESET_MY_TALENTS                = 22,   //I would like to reset my talents.
    WORLDSTRING_UNTRAIN_MY_PET                  = 23,   //I wish to untrain my pet.
    WORLDSTRING_I_UNDERSTAND_CONTINUE           = 24,   //I understand, continue.
    WORLDSTRING_YES_PLEASE_DO                   = 25,   //Yes, please do.
    WORLDSTRING_INSTANCE_UNVAIBLE               = 26,   //This instance is unavailable.
    WORLDSTRING_MUST_HAVE_TBC                   = 27,   //You must have The Burning Crusade Expansion to access this content.
    WORLDSTRING_HEROIC_NOT_AVAIBLE              = 28,   //Heroic mode unavailable for this instance.
    WORLDSTRING_MUST_BE_IN_RAID                 = 29,   //You must be in a raid group to pass through here.
    WORLDSTRING_DO_NOT_HAVE_ATTUNEMENT          = 30,   //You do not have the required attunement to pass through here.
    WORLDSTRING_MUST_BE_AT_LEVEL                = 31,   //You must be at least level %u to pass through here.
    WORLDSTRING_MUST_BE_IN_PARTY                = 32,   //You must be in a party to pass through here.
    WORLDSTRING_MUST_BE_70_HEROIC               = 33,   //You must be level 70 to enter heroic mode.
    WORLDSTRING_UNKNOWN1                        = 34,   //-
    WORLDSTRING_MUST_HAVE_ITEM_TO_PASS          = 35,   //You must have the item, `%s` to pass through here.
    WORLDSTRING_MUST_HAVE_UNKNOWN_ITEM_TO_PASS  = 36,   //You must have the item, UNKNOWN to pass through here.
    WORLDSTRING_WHAT_I_CAN_TEACH_YOU            = 37,   //What can I teach you, $N?
    WORLDSTRING_ALTERAC_VALLEY                  = 38,   //Alterac Valley
    WORLDSTRING_WARSONG_GULCH                   = 39,   //Warsong Gulch
    WORLDSTRING_ARATCHI_BASIN                   = 40,   //Arathi Basin
    WORLDSTRING_ARENA_2V2                       = 41,   //Arena 2v2
    WORLDSTRING_ARENA_3V3                       = 42,   //Arena 3v3
    WORLDSTRING_ARENA_5V5                       = 43,   //Arena 5v5
    WORLDSTRING_EYE_OF_THE_STORM                = 44,   //Eye of the Storm
    WORLDSTRING_UNKNOWN_BG                      = 45,   //Unknown Battleground
    WORLDSTRING_MINUTE_LEFT_UNTIL_BATTLE        = 46,   //One minute until the battle for %s begins!
    WORLDSTRING_THIRTY_SEC_LEFT_UNTIL_BATTLE    = 47,   //Thirty seconds until the battle for %s begins!
    WORLDSTRING_FIFTEEN_SEC_LEFT_UNTIL_BATTLE   = 48,   //Fifteen seconds until the battle for %s begins!
    WORLDSTRING_BATTLE_HAS_BEGUN                = 49,   //The battle for %s has begun!
    WORLDSTRING_ARENA                           = 50,   //Arena
    WORLDSTRING_JOIN_INVALID_INSTANCEID         = 51,   //You have tried to join an invalid instance id.
    WORLDSTRING_DELETED_BG_QUENE_NOT_VALID      = 52,   //Your queue on battleground instance id %u is no longer valid. Reason: Instance Deleted.
    WORLDSTRING_JOIN_BG_ENDED                   = 53,   //You cannot join this battleground as it has already ended.
    WORLDSTRING_JOIN_BG_NOT_EXISTS              = 54,   //Your queue on battleground instance %u is no longer valid, the instance no longer exists.
    WORLDSTRING_BG_RAID_GROUPS_NOT_SUPPORTED    = 55,   //Sorry, raid groups joining battlegrounds are currently unsupported.
    WORLDSTRING_MUST_BE_GROUP_LEADER_ARENA      = 56,   //You must be the party leader to add a group to an arena.
    WORLDSTRING_MUST_BE_IN_TEAM                 = 57,   //You must be in a team to join rated arena.
    WORLDSTRING_TOO_MANY_PLAYERS_IN_GROUP_ARENA = 58,   //You have too many players in your party to join this type of arena.
    WORLDSTRING_SOME_PARTY_MEMBERS_NOT_70       = 59,   //Sorry, some of your party members are not level 70.
    WORLDSTRING_PLAYERS_QUENED_OR_JOINED_BG     = 60,   //One or more of your party members are already queued or inside a battleground.
    WORLDSTRING_MEMBER_NOT_IN_TEAM              = 61,   //One or more of your party members are not members of your team.
    WORLDSTRING_WELCOME_TO                      = 62,   //Welcome to
    WORLDSTRING_HORDE                           = 63,   //Horde
    WORLDSTRING_ALLIANCE                        = 64,   //Alliance
    WORLDSTRING_WELCOME_NEW_CHALLENGER_JOINED   = 65,   //[ |cff00ccffAttention|r ] Welcome! A new challenger (|cff00ff00{%d}|r - |cffff0000%s|r) has arrived and joined into |cffff0000%s|r,their force has already been increased.
    WORLDSTRING_INSTANCE_RESET_SCHEDULED        = 66,   //This instance is scheduled to reset on
    WORLDSTRING_AUTO_LOOT_IS_PASSING            = 67,   //Auto loot passing is now %s
    WORLDSTRING_ON                              = 68,   //On
    WORLDSTRING_OFF                             = 69,   //Off
    WORLDSTRING_HEY_HOW_I_CAN_HELP_YOU          = 70,   //Hey there, $N. How can I help you?
    WORLDSTRING_ALREADY_IN_ARENA_TEAM           = 71,   //You are already in an arena team.
    WORLDSTRING_NAME_IS_ALREADY_IN_USE          = 72,   //That name is already in use.
    WORLDSTRING_ALREADY_HAVE_CHARTER            = 73,   //You already have an arena charter.
    WORLDSTRING_GUILD_NAME_ALREADY_EXISTS       = 74,   //A guild with that name already exists.
    WORLDSTRING_ALREADY_HAVE_GUILD_CHARTER      = 75,   //You already have a guild charter.
    WORLDSTRING_ITEM_NOT_FOUND                  = 76,   //Item not found.
    WORLDSTRING_TARGET_WRONG_FACTION            = 77,   //Target is of the wrong faction.
    WORLDSTRING_TARGET_CANNOT_SIGN_CHARTER      = 78,   //Target player cannot sign your charter for one or more reasons.
    WORLDSTRING_ALREADY_SIGNED_ON_CHARTER       = 79,   //You have already signed that charter.
    WORLDSTRING_DONT_HAVE_REQUIRED_SIGNATURES   = 80,   //You don't have the required amount of signatures to turn in this petition.
    WORLDSTRING_DEATHKNIGHT                     = 81,   //Deathknight
    WORLDSTRING_LEARN_ABOUT_DUAL_TALENT_SPEC    = 82,   //Learn about Dual Talent Specialization.
    WORLDSTRING_PURCHASE_DUAL_TALENT_SPEC       = 83,   //Purchase a Dual Talent Specialization.
    WORLDSTRING_ARE_YOU_SURE_PURCHASE_DUAL_TALENT = 84,  //Are you sure you would like to purchase your second talent specialization
    WORLDSTRING_NOT_ENOUGH_MONEY_FOR_DUAL_SPEC  = 85    //You do not have enough gold to purchase a dual spec.
 };
 
 #endif