import arcemu

from arcemu import Aura
from arcemu import GameObject
from arcemu import GossipMenu
from arcemu import Guild
from arcemu import Item
from arcemu import Unit
from arcemu import Player
from arcemu import Quest
from arcemu import Spell
from arcemu import WorldSession

def onNewCharacter( charRace, charClass, session, name ):
	print( "New character created by " + session.getAccountName() + " (" + str( session.getAccountId() ) +  ")" + " Character name: " + name + ", race:" + str( charRace ) + ", class:" + str( charClass ) )
	return True

def onFirstEnterWorld( player ):
	print( "Player '" + player.getName() + "' has entered the world for the first time!." )

def onEnterWorld( player ):
	print( "Player '" + player.getName() + "' has entered the world." )
	
def onGuildJoin( player, guild ):
	print( "Player '" + player.getName() + "' has joined guild " + guild.getGuildName() + " (" + guild.getGuildId() + ")" )
	
def onPlayerDeath( player ):
	print( "Player '" + player.getName() + "' has died." )
	
def onPlayerRepop( player ):
	print( "Player '" + player.getName() + "' has been repopped." )
	return True
	
def onPlayerResurrect( player ):
	print( "Player '" + player.getName() + "' has resurrected." )
	return True

def onEmote( pPlayer, emote, pUnit ):
	print( "Player emote ", emote )
	
	if pPlayer is not None:
		print( "The player's name is: ", pPlayer.getName() )
		pPlayer.sendChatMessage( arcemu.CHAT_MSG_SAY, arcemu.LANG_UNIVERSAL, "Hello my name is " + pPlayer.getName() )
	
	if pUnit is not None:
		print( "The unit's name is: ", pUnit.getName() )
		pUnit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, 'Hello, my name is ' + pUnit.getName() + '. Nice to meet you.' )
		
		
def onEnterCombat( pPlayer, pUnit ):
	print( "Player " + pPlayer.getName() + " entered combat with Unit " + pUnit.getName() )
	
def onCastSpell( player, spellId, spell ):
	print( "Player " + player.getName() + " has cast spell " + str( spellId ) + " (" + spell.getName() + ")" )

def onLogoutRequest( player ):
	print( "Player '" + player.getName() + "' has requested to log out." )
	return True
	
def onLogout( player ):
	print( "Player '" + player.getName() + "' has logged out." )
	
def onAcceptQuest( player, quest, questGiver ):
	print( "Player '" + player.getName() + "' has accepted quest " + quest.getTitle() + "(" + str( quest.getId() ) + ")" )

	if isinstance(questGiver, Unit):
		print( "Unit questgiver name: " + questGiver.getName() )
	elif isinstance( questGiver, GameObject ):
		print( "GameObject questgiver name: " + questGiver.getName() )
	elif isinstance( questGiver, Item ):
		print( "Item questgiver name: " + questGiver.getName() )
	
def onZoneChange( player, oldZone, newZone ):
	print( "Player '" + player.getName() + "' has changed zone from " + str( oldZone ) + " to " + str( newZone ) )
	
def onChatMessage( player, type, lang, message, misc ):
	print( "Player '" + player.getName() + "': " + message + "( misc:" + misc + ")" + "(lang:" + str(lang) + ")(type:" + str(type) + ")" )
	return True
	
def onLoot( player, unit, money, itemId ):
	print( "Player " + player.getName() + " has looted " + str( money ) + " coins and item " + str( itemId ) + " from " + unit.getName() )
	
def onGuildCreate( leader, guild ):
	print( "Player " + player.getName() + " has created guild " + guild.getGuildName() )

def onFullLogin( player ):
	print( "Player '" + player.getName() + " has fully logged in." )
	
def onCharacterCreated( player ):
	print( "A new character '" + player.getName() + " has been created!" )
	
def onQuestCancelled( player, quest ):
	print( "Player '" + player.getName() + "' has cancelled quest " + quest.getTitle() + "(" + str( quest.getId() ) + ")" )
	
def onQuestFinished( player, quest, questFinisher ):
	print( "Player '" + player.getName() + "' has finished quest " + quest.getTitle() + "(" + str( quest.getId() ) + ")" )
	if isinstance(questFinisher, Unit):
		print( "Unit questFinisher name: " + questFinisher.getName() )
	elif isinstance( questFinisher, GameObject ):
		print( "GameObject questFinisher name: " + questFinisher.getName() )
	elif isinstance( questFinisher, Item ):
		print( "Item questFinisher name: " + questFinisher.getName() )
	
def onHonorableKill( killer, victim ):
	print( killer.getName() + " has killed " + victim.getName() + " with honor" )
	
def onArenaFinish( player, arenaTeam, victory, rated ):
	print( "Player '" + player.getName() + " has finished arena. Arena team: " + arenaTeam + ", victory: " + str( victory ) + ", rated: " + str( rated ) )
	
def onObjectLoot( player, target, money, itemId ):
	print( "Player " + player.getName() + " has looted " + str( money ) + " coins and item " + str( itemId ) )
	
	if isinstance( target, Unit ):
		print( "Looted unit: " + target.getName() )
	elif isinstance( target, Player ):
		print( "Looted player: " + target.getName() )
	elif isinstance( target, GameObject ):
		print( "Looted GameObject: " + target.getName() )
	elif isinstance( target, Item ):
		print( "Looted item name: " + target.getName() )
		
def onAreaTrigger( player, areaTriggerId ):
	print( "Player '" + player.getName() + " has reached areatrigger " + str( areaTriggerId ) )
	
def onLevelUp( player ):
	print( "Player '" + player.getName() + " has reached a new level." )
	
def onPreDie( killer, victim ):
	print( killer.getName() + " is killing " + victim.getName() )
	victim.sendChatMessage( 12, 0, killer.getName() + " is killing me softly...  ...with his spell." )
	
def onAdvanceSkillLine( player, skill, value ):
	print( "Player '" + player.getName() + "' has advanced skill " + str( skill ) + " to level " + str( value ) )
	
def onDuelFinished( winner, loser ):
	print( winner.getName() + " has beaten " + loser.getName() + " in a duel" )
	
def onKillPlayer( killer, victim ):
	print( killer.getName() + " is killing " + victim.getName() )
	
def onAuraRemove( aura ):
	print( "Aura removed. Spell name: " + aura.getSpellName() + ", Spell Id:" + str( aura.getSpellId() ) + ", Aura slot:" + str( aura.getAuraSlot() ) )
	
def mohawk_onHello( unit, event, player ):
	print( player.getName() + " said gossip hello to " + unit.getName() )
	print( "Event id: " + str( event ) )
	
	menu = GossipMenu( 1, unit, 0 )
	menu.addItem( 0, "First menu item", 1, 0 )
	menu.addItem( 0, "Second menu item", 2, 0 )
	menu.addItem( 0, "Third menu item", 3, 0 )
	menu.sendToPlayer( player )
	
arcemu.RegisterUnitGossipEvent( 31111, 1, mohawk_onHello )

arcemu.RegisterServerHook( arcemu.SERVER_HOOK_EVENT_ON_NEW_CHARACTER, onNewCharacter )
arcemu.RegisterServerHook( arcemu.SERVER_HOOK_EVENT_ON_KILL_PLAYER, onKillPlayer )
arcemu.RegisterServerHook( arcemu.SERVER_HOOK_EVENT_ON_FIRST_ENTER_WORLD, onFirstEnterWorld )
arcemu.RegisterServerHook( arcemu.SERVER_HOOK_EVENT_ON_ENTER_WORLD, onEnterWorld )
arcemu.RegisterServerHook( arcemu.SERVER_HOOK_EVENT_ON_GUILD_JOIN, onGuildJoin )
arcemu.RegisterServerHook( arcemu.SERVER_HOOK_EVENT_ON_DEATH, onPlayerDeath )
arcemu.RegisterServerHook( arcemu.SERVER_HOOK_EVENT_ON_REPOP, onPlayerRepop )
arcemu.RegisterServerHook( arcemu.SERVER_HOOK_EVENT_ON_EMOTE, onEmote )
arcemu.RegisterServerHook( arcemu.SERVER_HOOK_EVENT_ON_ENTER_COMBAT, onEnterCombat )
arcemu.RegisterServerHook( arcemu.SERVER_HOOK_EVENT_ON_CAST_SPELL, onCastSpell )
# Not implementing event 11 (tick)
arcemu.RegisterServerHook( arcemu.SERVER_HOOK_EVENT_ON_LOGOUT_REQUEST, onLogoutRequest )
arcemu.RegisterServerHook( arcemu.SERVER_HOOK_EVENT_ON_LOGOUT, onLogout )
arcemu.RegisterServerHook( arcemu.SERVER_HOOK_EVENT_ON_QUEST_ACCEPT, onAcceptQuest )
arcemu.RegisterServerHook( arcemu.SERVER_HOOK_EVENT_ON_ZONE, onZoneChange )
arcemu.RegisterServerHook( arcemu.SERVER_HOOK_EVENT_ON_CHAT, onChatMessage )
arcemu.RegisterServerHook( arcemu.SERVER_HOOK_EVENT_ON_LOOT, onLoot )
arcemu.RegisterServerHook( arcemu.SERVER_HOOK_EVENT_ON_GUILD_CREATE, onGuildCreate )
arcemu.RegisterServerHook( arcemu.SERVER_HOOK_EVENT_ON_FULL_LOGIN, onFullLogin )
arcemu.RegisterServerHook( arcemu.SERVER_HOOK_EVENT_ON_CHARACTER_CREATE, onCharacterCreated )
arcemu.RegisterServerHook( arcemu.SERVER_HOOK_EVENT_ON_QUEST_CANCELLED, onQuestCancelled )
arcemu.RegisterServerHook( arcemu.SERVER_HOOK_EVENT_ON_QUEST_FINISHED, onQuestFinished )
arcemu.RegisterServerHook( arcemu.SERVER_HOOK_EVENT_ON_HONORABLE_KILL, onHonorableKill )
arcemu.RegisterServerHook( arcemu.SERVER_HOOK_EVENT_ON_ARENA_FINISH, onArenaFinish )
arcemu.RegisterServerHook( arcemu.SERVER_HOOK_EVENT_ON_OBJECTLOOT, onObjectLoot )
arcemu.RegisterServerHook( arcemu.SERVER_HOOK_EVENT_ON_AREATRIGGER, onAreaTrigger )
arcemu.RegisterServerHook( arcemu.SERVER_HOOK_EVENT_ON_POST_LEVELUP, onLevelUp )
arcemu.RegisterServerHook( arcemu.SERVER_HOOK_EVENT_ON_PRE_DIE, onPreDie )
arcemu.RegisterServerHook( arcemu.SERVER_HOOK_EVENT_ON_ADVANCE_SKILLLINE, onAdvanceSkillLine )
arcemu.RegisterServerHook( arcemu.SERVER_HOOK_EVENT_ON_DUEL_FINISHED, onDuelFinished )
arcemu.RegisterServerHook( arcemu.SERVER_HOOK_EVENT_ON_AURA_REMOVE, onAuraRemove )
arcemu.RegisterServerHook( arcemu.SERVER_HOOK_EVENT_ON_RESURRECT, onPlayerResurrect )
