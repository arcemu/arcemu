import arcemu
from arcemu import InstanceScript

class TestInstanceScript( InstanceScript ):
	def __init__( self ):
		InstanceScript.__init__( self )
		
	def Onload( self ):
		print( "Instance loading..." )
		
	def OnDestroy( self ):
		print( "Instance being destroyed..." )
		
	def OnPlayerDeath( self, player, killer ):
		print( "Instance: " + killer.getName() + " has slain " + victim.getName() )
		
	def OnPlayerEnter( self, player ):
		print( "Instance: Player " + player.getName() + " has entered " )
		
	def OnCreatureDeath( self, victim, killer ):
		print( "Instance: Creature " + victim.getName() + " killed by " + killer.getName() )
		
	def OnGameObjectActivate( self, go, player ):
		print( "Instance: GO " + go.getName() + "(" + str( go.getId() ) + ") activated by " + player.getName() )
		
	@staticmethod
	def create():
		return TestInstanceScript()

arcemu.RegisterInstanceScript( 34, TestInstanceScript.create )
