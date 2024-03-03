'''
Test quest script for Arcemu Python Engine
'''

import arcemu
from arcemu import QuestScript

class GoldDustExchange( QuestScript ):
	def __init__( self ):
		QuestScript.__init__( self )
		
	def OnQuestStart( self, player, quest ):
		print( "Started" )
		player.sendChatMessage( arcemu.CHAT_MSG_SAY, arcemu.LANG_UNIVERSAL, "Started quest " + str( quest )  )
		
	def OnQuestComplete( self, player, quest ):
		print( "Completed" )
		player.sendChatMessage( arcemu.CHAT_MSG_SAY, arcemu.LANG_UNIVERSAL, "Completed quest " + str( quest )  )
		
	def OnQuestCancel( self, player ):
		print( "Cancelled" )
		player.sendChatMessage( arcemu.CHAT_MSG_SAY, arcemu.LANG_UNIVERSAL, "Cancelled quest"  )
		
	def OnPlayerItemPickup( self, item, total, player, quest ):
		print( "Picked up item " + str( item ) + " total count is " + str( total ) )
		player.sendChatMessage( arcemu.CHAT_MSG_SAY, arcemu.LANG_UNIVERSAL, "Picked up item " + str( item ) + " total count is " + str( total )  )
		
arcemu.RegisterQuestScript( 47, GoldDustExchange() )
