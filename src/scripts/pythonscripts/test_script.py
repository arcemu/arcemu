import arcemu

from arcemu import GameObject
from arcemu import Guild
from arcemu import Item
from arcemu import Unit
from arcemu import Player
from arcemu import Quest
from arcemu import Spell
from arcemu import WorldSession

def onNewCharacter( charRace, charClass, session, name ):
	print( "New character created by " + session.getAccountName() + " (" + str( session.getAccountId() ) +  ")" + " Character name: " + name + ", race:" + str( charRace ) + ", class:" + str( charClass ) )

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
	
def onPlayerResurrect( player ):
	print( "Player '" + player.getName() + "' has resurrected." )

def onEmote( pPlayer, emote, pUnit ):
	print( "Player emote ", emote )
	
	if pPlayer is not None:
		print( "The player's name is: ", pPlayer.getName() )
		pPlayer.sendChatMessage( 12, 0, "Hello my name is " + pPlayer.getName() )
	
	if pUnit is not None:
		print( "The unit's name is: ", pUnit.getName() )
		pUnit.sendChatMessage( 12, 0, 'Hello, my name is ' + pUnit.getName() )
		
		
def onEnterCombat( pPlayer, pUnit ):
	print( "Player " + pPlayer.getName() + " entered combat with Unit " + pUnit.getName() )
	
def onCastSpell( player, spellId, spell ):
	print( "Player " + player.getName() + " has cast spell " + str( spellId ) + " (" + spell.getName() + ")" )

def onLogoutRequest( player ):
	print( "Player '" + player.getName() + "' has requested to log out." )
	
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

arcemu.info()

arcemu.RegisterServerHook( 1, onNewCharacter )
arcemu.RegisterServerHook( 2, onKillPlayer )
arcemu.RegisterServerHook( 3, onFirstEnterWorld )
arcemu.RegisterServerHook( 4, onEnterWorld )
arcemu.RegisterServerHook( 5, onGuildJoin )
arcemu.RegisterServerHook( 6, onPlayerDeath )
arcemu.RegisterServerHook( 7, onPlayerRepop )
arcemu.RegisterServerHook( 8, onEmote )
arcemu.RegisterServerHook( 9, onEnterCombat )
arcemu.RegisterServerHook( 10, onCastSpell )
# Not implementing event 11 (tick)
arcemu.RegisterServerHook( 12, onLogoutRequest )
arcemu.RegisterServerHook( 13, onLogout )
arcemu.RegisterServerHook( 14, onAcceptQuest )
arcemu.RegisterServerHook( 15, onZoneChange )
arcemu.RegisterServerHook( 16, onChatMessage )
arcemu.RegisterServerHook( 17, onLoot )
arcemu.RegisterServerHook( 18, onGuildCreate )
arcemu.RegisterServerHook( 19, onFullLogin )
arcemu.RegisterServerHook( 20, onCharacterCreated )
arcemu.RegisterServerHook( 21, onQuestCancelled )
arcemu.RegisterServerHook( 22, onQuestFinished )
arcemu.RegisterServerHook( 23, onHonorableKill )

arcemu.RegisterServerHook( 27, onLevelUp )
arcemu.RegisterServerHook( 28, onPreDie )
arcemu.RegisterServerHook( 29, onAdvanceSkillLine )
arcemu.RegisterServerHook( 30, onDuelFinished )

arcemu.RegisterServerHook( 32, onPlayerResurrect )
