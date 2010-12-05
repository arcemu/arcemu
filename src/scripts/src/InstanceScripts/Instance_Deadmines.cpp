/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
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

#include "Setup.h"

/************************************************************************/
/* Instance_Deadmines.cpp Script										*/
/************************************************************************/
/*
	Might be better to just setup one creature on a waypoint and have the others follow that creature.
*/
static Location PatrolCoords[] = 
{
	{},
	// Rhahkzor Patrol Waypoints
	{(-181.924896f+RandomFloat(10)-10), (-416.992462f+RandomFloat(10)-10), 54.698444f, 4.061967f},
	{(-192.621002f+RandomFloat(10)-10), (-451.197998f+RandomFloat(10)-10), 54.432598f, 1.668860f},

	// Sneed Patrol Waypoints
	{(-262.295929f+RandomFloat(10)-10), (-482.195892f+RandomFloat(10)-10), 49.447025f, 3.162687f},
	{(-290.320068f+RandomFloat(10)-10), (-532.999023f+RandomFloat(10)-10), 49.432594f, 4.736625f},

	// Gilnid Patrol Waypoints
	{(-237.536804f+RandomFloat(10)-10), (-578.147095f+RandomFloat(10)-10), 51.227661f, 1.223522f}, 
	{(-184.804901f+RandomFloat(10)-10), (-564.235657f+RandomFloat(10)-10), 51.229397f, 5.297391f}, 
	{(-232.678970f+RandomFloat(10)-10), (-590.744568f+RandomFloat(10)-10), 19.306545f, 1.889554f},
};

/*	ELITE/RARES OUTSIDE OF INSTANCE */
class NobleAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(NobleAI, MoonScriptCreatureAI);
    NobleAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		// Fireball
		AddSpell(9053, Target_Current, 12, 3, 0);

		// Chains of Ice
		AddSpell(512, Target_Current, 12, 1.3f, 0);

		// Polymorph: Chicken
		AddSpell(228, Target_Current, 12, 0, 10);
	}
};

class MarisaAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MarisaAI, MoonScriptCreatureAI);
    MarisaAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		// Fireball
		AddSpell(9053, Target_Current, 12, 3, 0);
	}
};

class ThistlenettleAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ThistlenettleAI, MoonScriptCreatureAI);
    ThistlenettleAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		// Draw of Thistlenettle
		AddSpell(5219, Target_Current, 12, 2, 0);
	}
};

/* TRASH MOBS */
class DefiasEvokerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DefiasEvokerAI, MoonScriptCreatureAI);
    DefiasEvokerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		// Flamestrike
		AddSpell(11829, Target_Destination, 12, 3, 0);
		// Frost Armor
		AddSpell(12544, Target_Self, 100, 0, 1800);
		// Quick Flame Ward
		AddSpell(4979, Target_RandomFriendly, 12, 1.5f, 10);
	}
};

class DefiasTaskmasterAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DefiasTaskmasterAI, MoonScriptCreatureAI);
    DefiasTaskmasterAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		// Piercing Shot
		AddSpell(6685, Target_Current, 12, 0, 30, 5, 30, true);
		// Shoot
		AddSpell(6660, Target_Current, 12, 0, 0, 0, 35, true);
	}
};

// ToDo: Make Faded cast on leaving combat
class DefiasBlackguardAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DefiasBlackguardAI, MoonScriptCreatureAI);
    DefiasBlackguardAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		// Faded
		AddSpell(6408, Target_Self, 0, 0, 0);
	}
};


class RhahkZorAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(RhahkZorAI, MoonScriptCreatureAI);
    RhahkZorAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		// Rhahk'Zor Slam
		AddSpell(6304, Target_Current, 8, 0, 3);
		AddEmote(Event_OnCombatStart, "Van Cleef pay big for your heads!", Text_Yell, 5774);
	}

	/*
		OnDied: Factory Door opens, 2 Patrols are spawned.
				
		Added a random spawn chance for Miner Johnson
			sql: DELETE FROM `creature_spawns` WHERE `entry` = '3586';
	*/
    void OnDied(Unit*  pKiller)
    {
        // Find "Factory Door"
        GameObject*  pDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(
           -190.860092f, -456.332184f, 54.496822f, 13965);
		
		if(pDoor)
			pDoor->SetState(0);

		// Defias Evoker
		SetupPatrol(1729);

		// Defias Taskmaster
		SetupPatrol(4417);

		// Miner Johnson Rare Spawn -- Really should be a database thing
		float ChanceToSpawn = RandomFloat(100);
		if(ChanceToSpawn >= 30 && ChanceToSpawn <= 60) // 30% ? 
			SpawnCreature(3586, -150.723999f, -531.729004f, 49.725700f, 1.918860f, false);

		ParentClass::OnDied(pKiller);
    }

	void SetupPatrol(uint32 entry)
	{
		MoonScriptCreatureAI* Patrol = NULL;
		Patrol = SpawnCreature(entry, (-98.025314f+RandomFloat(10)-10), (-396.978333f+RandomFloat(10)-10), 58.609558f, 2.973405f);
		if(Patrol == NULL)
			return;
		Patrol->AddWaypoint(Patrol->CreateWaypoint(1, 2500, Flag_Walk, PatrolCoords[1]));
		Patrol->AddWaypoint(Patrol->CreateWaypoint(2, 2500, Flag_Walk, PatrolCoords[2]));
		Patrol->ForceWaypointMove(1);
		Patrol = NULL;
	}
};


// Sneeds Shredder
class SneedsShredderAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SneedsShredderAI, MoonScriptCreatureAI);
    SneedsShredderAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		// Distracting Pain
		AddSpell(3603, Target_Current, 8, 0, 45);
		// Terrify
		AddSpell(7399, Target_Current, 8, 0, 15);
	}

    void OnDied(Unit*  pKiller)
    {
		// Spawn Sneed
		SpawnCreature(643);
		ParentClass::OnDied(pKiller);
	}
};

// Sneed -> open door on death.
class SneedAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SneedAI, MoonScriptCreatureAI);
	SneedAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		// Disarm
		AddSpell(6713, Target_Current, 8, 0, 5);
	}
    
	/*
		OnDied: Mast Room Door opens, 3 Patrols are spawned.
	*/
	void OnDied(Unit*  pKiller)
    {
        // Find "Mast Room Door"
        GameObject*  pDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(
            -289.691650f, -535.988953f, 49.440678f, 16400);
        if(pDoor)
			pDoor->SetState(0);
		
		// Defias Evoker
		SetupPatrol(1729);
		// Defias Taskmaster x 2
		SetupPatrol(4417);
		SetupPatrol(4417);

		ParentClass::OnDied(pKiller);
    }

	void SetupPatrol(uint32 entry)
	{
		MoonScriptCreatureAI* Patrol = NULL;
		Patrol = SpawnCreature(entry, (-216.863632f+RandomFloat(10)-10), (514.258423f+RandomFloat(10)-10), 51.537754f, 1.480364f);
		if(Patrol == NULL)
			return;
		Patrol->AddWaypoint(Patrol->CreateWaypoint(1, 2500, Flag_Walk, PatrolCoords[3]));
		Patrol->AddWaypoint(Patrol->CreateWaypoint(2, 2500, Flag_Walk, PatrolCoords[4]));
		Patrol->ForceWaypointMove(1);
		Patrol = NULL;
	}
};


// Gilnid -> Opens Foundary door on death.      
class GilnidAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GilnidAI, MoonScriptCreatureAI);
	GilnidAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		// Molten Metal
		AddSpell(5213, Target_Current, 30, 2, 15);
		// Melt Ore
		AddSpell(5159, Target_Current, 30, 1.5, 20);
	}
	
	/*
		OnDied: Foundry Door opens, 4 Patrols are spawned.
	*/
    void OnDied(Unit*  pKiller)
    {		
		// Find "Foundry Door"
		GameObject*  pDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-176.569f, -577.640991f, 19.311600f, 16399);
        if(pDoor)
			pDoor->SetState(0);

		// Defias Taskmaster x 2
		SetupPatrol(4417);
		SetupPatrol(4417);
		// Defias Evoker x 2
		SetupPatrol(1729);
		SetupPatrol(1729);

		ParentClass::OnDied(pKiller);
    }

	void SetupPatrol(uint32 entry)
	{
		MoonScriptCreatureAI* Patrol = NULL;
		Patrol = SpawnCreature(entry, (-265.543121f+RandomFloat(10)-10), (-578.010925f+RandomFloat(10)-10), 50.516342f, 6.246144f);
		if(Patrol == NULL)
			return;

		Patrol->AddWaypoint(Patrol->CreateWaypoint(1, 2500, WALK, PatrolCoords[5]));
		Patrol->AddWaypoint(Patrol->CreateWaypoint(2, 2500, WALK, PatrolCoords[6]));
		Patrol->AddWaypoint(Patrol->CreateWaypoint(3, 2500, WALK, PatrolCoords[7]));
		Patrol->ForceWaypointMove(1);
		Patrol = NULL;
	}
};

#define MR_SMITE					646
#define SMITE_STOMP					6432
#define SMITES_HAMMER				6436		
#define SMITE_SLAM					6435

class MrSmiteAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION( MrSmiteAI, MoonScriptBossAI );
	MrSmiteAI( Creature* pCreature ) : MoonScriptBossAI( pCreature )
	{
		AddSpell( SMITE_SLAM, Target_Current, 25, 0.0f, 15, 0.0f, 8.0f, true );
		mStomp = AddSpell( SMITE_STOMP, Target_Self, 0, 0, 0 );
		mWaitAtChest = INVALIDATE_TIMER;
		SetWieldWeapon( true );
	};

	void OnCombatStop( Unit* pTarget )
    {
		if ( GetPhase() == 4 )
			RemoveAura( SMITES_HAMMER );
		if ( !IsAlive() )
			SetWieldWeapon( false );

		SetPhase( 1 );
        SwitchWeapons();
		RemoveTimer( mWaitAtChest );
		ParentClass::OnCombatStop( pTarget );
    };

    void AIUpdate()
    {
        if ( GetHealthPercent() <= 66 && GetPhase() == 1 )
        {
			Emote( "You landlubbers are tougher than I thought. I'll have to improvise!", Text_Yell, 5778 );
            SetPhase( 2, mStomp );
		}
        else if ( GetHealthPercent() <= 33 && GetPhase() == 3 )
        {
			Emote( "D'ah! Now you're making me angry!", Text_Yell, 5779 );
            SetPhase( 4, mStomp );
        };

		if ( GetPhase() == 2 || GetPhase() == 4 )
		{
			if ( NearChest() )
				SwitchWeapons();
			else if ( _unit->GetAIInterface()->getAIState() != STATE_SCRIPTMOVE )
			{
				MoveToChest();
				return;
			}
			else
				return;
		};

		if ( IsTimerFinished( mWaitAtChest ) )
			MoveToPlayer();

		ParentClass::AIUpdate();
    };

	void MoveToChest()
	{
		if ( GetCanEnterCombat() )
			_unit->GetAIInterface()->SetAllowedToEnterCombat( false );

		StopMovement();
		_unit->GetAIInterface()->SetAIState( STATE_SCRIPTMOVE );
		MoveTo( 1.100060f, -780.026367f, 9.811194f );
	};

	void MoveToPlayer()
	{
		_unit->GetAIInterface()->SetAllowedToEnterCombat( true );
		_unit->GetAIInterface()->SetAIState( STATE_SCRIPTIDLE );
	};

	bool NearChest()
	{
		if ( _unit->GetPositionX() == 1.100060f && _unit->GetPositionY() == -780.026367f )
			return true;
		else if ( _unit->GetAIInterface()->getAIState() != STATE_SCRIPTMOVE )
		{
			// Too small distance - let's prevent from blocking
			float XDiff, YDiff;
			XDiff = _unit->GetPositionX() - 1.100060f;
			YDiff = _unit->GetPositionY() + 780.026367f;
			float Distance = static_cast< float >( sqrt( XDiff * XDiff + YDiff * YDiff ) );
			if ( Distance <= 5.0f )
				return true;
		};

		return false;
	};

    void SwitchWeapons()
    {	
		// CREDITS to Skyboat on ascentemu.com/forums  he had some of this info on one of his releases
        switch ( GetPhase() )
        {
        case 1: // Phase 1 (Default)
			SetDisplayWeaponIds( 5192, 0 );
			_unit->SetBaseAttackTime(MELEE,_unit->GetBaseAttackTime(MELEE) );	// 1483 is taken from NCDB creature_proto
			break;
        case 2: // Phase 2
			SetDisplayWeaponIds( 5196, 5196 );
			_unit->SetBaseAttackTime(MELEE,_unit->GetBaseAttackTime(MELEE) / 2 );
			break;
        case 4: // Phase 4
			// Is base attack time change needed if we use aura ?
			SetDisplayWeaponIds( 7230, 0 );
			_unit->SetBaseAttackTime(MELEE,_unit->GetBaseAttackTime(MELEE) * 2 );
			ApplyAura( SMITES_HAMMER );
			break;
        };

		// Wait at the chest for 4.5seconds -- Still needs work
		_unit->setAttackTimer( 4500, false );
		mWaitAtChest = AddTimer( 4500 );
		SetPhase( GetPhase() + 1 );
    };

	SpellDesc*	mStomp;
	int32		mWaitAtChest;
};

// Greenskin
class GreenskinAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GreenskinAI, MoonScriptBossAI);
    GreenskinAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
    {
		// Poisoned Harpoon
		AddSpell(5208, Target_Current, 12, 2, 0);
		// Cleave
		AddSpell(40505, Target_Current, 12, 0, 0);
    }
};

// VanCleef
class VanCleefAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(VanCleefAI, MoonScriptBossAI);
    VanCleefAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
    {
		AddEmote(Event_OnCombatStart, "None may challenge the brotherhood.", Text_Yell, 5780);
    }

    void OnTargetDied(Unit* pTarget)
    {
        char msg[200];
        if(pTarget->IsPlayer())
            sprintf(msg, "And stay down, %s.", TO_PLAYER(pTarget)->GetName());
        else if (pTarget->GetTypeFromGUID() == HIGHGUID_TYPE_PET)
            sprintf(msg, "And stay down, %s.", TO_PET(pTarget)->GetName().c_str());

		Emote(msg, Text_Yell, 5781);
		ParentClass::OnTargetDied(pTarget);
    }

	/*
		At <= 50% He will spawn 2 Defias Blackguards
		ToDo: Make them despawn WHEN he dies, also make them unlootable
	*/
	void AIUpdate()
	{
		if(GetHealthPercent() <= 75 && GetPhase() == 1)
		{
			Emote("Lap dogs, all of you.", Text_Yell, 5782);
			SetPhase(2);
		}
		else if(GetHealthPercent() <= 50 && GetPhase() == 2)
		{
			Emote("Fools! Our cause is righteous.", Text_Yell, 5783);
			// Defias Blackguard x 2
			MoonScriptCreatureAI* Guard = NULL;
			for(int x=0; x<2; x++)
			{
				Guard = SpawnCreature(636);
				if(Guard == NULL)
					continue;
				Guard->SetDespawnWhenInactive(true);
				Guard->GetUnit()->m_noRespawn = true;
				Guard = NULL;
			}
			SetPhase(3);
		}
		else if(GetHealthPercent() <= 25 && GetPhase() == 3)
		{
			Emote("The brotherhood shall remain.", Text_Yell, 5784);
			SetPhase(4);
		}
		ParentClass::AIUpdate();
	}
};


// Cookie
class CookieAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CookieAI, MoonScriptCreatureAI);
    CookieAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		// Acid Splash
		AddSpell(6306, Target_Current, 20, 1, 30);
		// Cookie's Cooking
		AddSpell(5174, Target_Self, 10, 2, 0);
	}
};

/* 
	OnActivate:
		1) Cannon fires at the door and blows it open. (Not Done)
		2) Smite yells.
		3) 2/3 Pirates run at the door to see what it was. (Not Done)
*/
class DefiasCannon : public GameObjectAIScript
{
public:
	DefiasCannon(GameObject*  goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject*  GO) { return new DefiasCannon(GO); }

	void OnActivate(Player*  pPlayer)
	{
		Creature* Smite = _gameobject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-22.8471f, -797.283f, 20.3745f, 646);
		if(Smite)
		{
			Smite->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You there, check out that noise!");
			Smite = NULL;
		}
	}
};

void SetupDeadmines(ScriptMgr * mgr)
{
	/* OUTSIDE INSTANCE */
	mgr->register_creature_script(596, &NobleAI::Create);
	mgr->register_creature_script(599, &MarisaAI::Create);
	mgr->register_creature_script(626, &ThistlenettleAI::Create);

	/* TRASH */
	mgr->register_creature_script(1729, &DefiasEvokerAI::Create);
	mgr->register_creature_script(4417, &DefiasTaskmasterAI::Create);
	mgr->register_creature_script(636, &DefiasBlackguardAI::Create);

	/* OBJECTS */
	mgr->register_gameobject_script(16398, &DefiasCannon::Create);

	/* BOSSES */
	mgr->register_creature_script(644, &RhahkZorAI::Create);
	mgr->register_creature_script(642, &SneedsShredderAI::Create);
	mgr->register_creature_script(643, &SneedAI::Create);
	mgr->register_creature_script(1763, &GilnidAI::Create);
	mgr->register_creature_script(MR_SMITE, &MrSmiteAI::Create);
	mgr->register_creature_script(647, &GreenskinAI::Create);
	mgr->register_creature_script(639, &VanCleefAI::Create);
	mgr->register_creature_script(645, &CookieAI::Create);
}
