/*
===============================
The Darkmoon Faire Project
Author: Nexis
Team: Sun++ (www.sunplusplus.info)
===============================
*/

#include "Setup.h"

// Blastenheimer 5000 Ultra Cannon
class Blastenheimer5000 : public GameObjectAIScript
{
	public:
		Blastenheimer5000(GameObject* pGameObject) : GameObjectAIScript(pGameObject)
		{
			mPlayerGuid = 0;
		};

		static GameObjectAIScript* Create(GameObject* pGameObject)
		{
			return new Blastenheimer5000(pGameObject);
		};

		void OnActivate(Player* pPlayer)
		{
			if(mPlayerGuid != 0)
				return;

			// Do you think we should look for ai owner ?
			/*GameObject* BlastCannon = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 180515 );
			if ( BlastCannon != NULL )
				BlastCannon->SetUInt32Value( GAMEOBJECT_FLAGS, 33 );*/

			pPlayer->CastSpell(pPlayer, 24832, true);
			pPlayer->SetMovement(MOVE_ROOT, 1);
			_gameobject->PlaySoundToSet(8476);
			mPlayerGuid = static_cast<uint32>(pPlayer->GetGUID());
			RegisterAIUpdateEvent(2200);
		};

		void AIUpdate()
		{
			Player* CurrentPlayer = objmgr.GetPlayer(mPlayerGuid);
			if(CurrentPlayer == NULL)
			{
				RemoveAIUpdateEvent();
				mPlayerGuid = 0;
				return;
			};

			if(CurrentPlayer->GetMapId() == 530) 		// Shattrath
			{
				CurrentPlayer->SafeTeleport(530, 0, -1742.640869f, 5454.712402f, -7.928009f, 4.606363f);
			}
			else if(CurrentPlayer->GetMapId() == 0) 		// Elwynn Forest
			{
				CurrentPlayer->SafeTeleport(0, 0, -9569.150391f, -14.753426f, 68.051422f, 4.874008f);
			}
			else if(CurrentPlayer->GetMapId() == 1) 		// Mulgore
			{
				CurrentPlayer->SafeTeleport(1, 0, -1326.711914f, 86.301125f, 133.093918f, 3.510725f);
			};

			CurrentPlayer->SetMovement(MOVE_UNROOT, 1);
			CurrentPlayer->CastSpell(CurrentPlayer, 42867, true);   // 24742
			_gameobject->SetUInt32Value(GAMEOBJECT_FLAGS, 0);
			mPlayerGuid = 0;
			RemoveAIUpdateEvent();
		};

	protected:

		uint32	mPlayerGuid;
};

/*
Spells:
=====================
Cannon - 24933
Mortor - 25003
Drop Mine - 39685, 25024
Nitrous Boost - 27746



// Tonk Control Consoles
class TonkControlConsole : public GameObjectAIScript
{
public:
	TonkControlConsole(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject* GO) { return new TonkControlConsole(GO); }

	// Click the Console
	void OnActivate(Player* pPlayer)
	{
		// Pre-flight checks
		GameObject* tonkConsole = NULL;
		tonkConsole = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 180524);

		// Open and disable the Tonk Console
		tonkConsole->SetUInt32Value(GAMEOBJECT_FLAGS, 1);
		tonkConsole->SetState(0);

		// Spawn Steam Tonk
		pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(19405, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), true, false, 0, 0)->Despawn(310000, 0);;

		// Store the tonk just spawned
		Creature* pTonk = NULL;
		pTonk = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 19405);

		// Cast the tonk control spell on the tonk
		pPlayer->CastSpell(pTonk, 33849, false);

		// Start checks to see if player still has aura
		RegisterAIUpdateEvent(1000);

		Plr = pPlayer;
		Tonk = pTonk;
		Console = tonkConsole;
	}

	void AIUpdate()
	{
		if (!Plr->HasAura(33849) || Tonk->isDead())
		{
			// Kill then Despawn Tonk after 10 seconds
			Plr->CastSpell(Tonk, 5, false); // Kill spell
			Plr->CastSpell(Plr, 2880, false); // Stun Player
			Plr->RemoveAura(33849);
			Tonk->Despawn(10000,0);

			// Close the console so others can access it
			Console->SetUInt32Value(GAMEOBJECT_FLAGS, 0);
			Console->SetState(1);
			RemoveAIUpdateEvent();
		}
	}

protected:
	Player* Plr;
	Creature* Tonk;
	GameObject* Console;
};
*/

void SetupDarkmoonFaireObjects(ScriptMgr* mgr)
{
	mgr->register_gameobject_script(180515, &Blastenheimer5000::Create);			// Blastenheimer 5000 Ultra Cannon
	// mgr->register_gameobject_script(180524, &TonkControlConsole::Create);			// Tonk Control Console
}
