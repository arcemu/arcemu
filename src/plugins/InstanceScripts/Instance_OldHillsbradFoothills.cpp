/*
* ArcScripts for ArcEmu MMORPG Server
* Copyright (C) 2010 ArcEmu Team <http://www.arcemu.org/>
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

/* Script Comment:
 * Everything before escort event is made to work as blizzlike as possible!
 * This is still in development and is tagged[WIP].
 * It is suggested to apply SQL updates to see this script fully working!
 * Have fun ~Azolex
 */

#include "Setup.h"

static Location Fires[] =
{
	{ 2160.68f,     235.382f,   53.8946f,   3.55550f },
	{ 2162.26f,     237.439f,   56.7303f,   5.97846f },
	{ 2192.94f,     258.437f,   54.0479f,   4.17990f },
	{ 2196.59f,     256.212f,   54.5663f,   4.15633f },
	{ 2207.03f,     259.549f,   62.1383f,   5.43653f },
	{ 2225.57f,     248.524f,   62.2857f,   5.66430f },
	{ 2179.05f,     262.558f,   62.2233f,   0.62989f },
	{ 2071.52f,     108.279f,   54.6802f,   3.91757f },
	{ 2160.76f,     246.790f,   62.035f,    0.82231f },
	{ 2195.19f,     257.229f,   58.0637f,   4.13277f },
	{ 2072.30f,     110.435f,   56.8422f,   2.74732f },
	{ 2061.12f,     95.2094f,   62.4747f,   2.04439f },
	{ 2067.92f,     121.267f,   62.7211f,   1.08228f },
	{ 2110.83f,     46.0729f,   62.0335f,   2.71590f },
	{ 2118.41f,     52.8182f,   56.0043f,   2.89655f },
	{ 2120.10f,     51.7093f,   54.6008f,   5.81823f },
	{ 2082.69f,     71.9957f,   55.8388f,   2.77088f },
	{ 2087.61f,     57.1432f,   62.1681f,   2.78659f },
	{ 2083.42f,     70.0828f,   53.7298f,   4.82470f },
	{ 2077.26f,     140.631f,   62.9179f,   1.05479f },
	{ 2070.20f,     75.6493f,   61.9173f,   2.12293f }
};

static Location ThrallWP1[] = // pre 2nd boss
{
	{ 0, 0, 0, 0 },
	{ 2230.29f,	115.049f,	82.2946f,	4.22934f },
	{ 2233.85f,	112.592f,	82.3021f,	5.77657f },
	{ 2231.99f,	108.086f,	82.6628f,	2.69702f },
	{ 2216.39f,	116.700f,	89.4553f,	2.59256f },
	{ 2208.22f,	103.356f,	89.4553f,	2.65854f },
	{ 2198.24f,	109.313f,	89.4553f,	2.60356f },
	{ 2203.42f,	119.908f,	89.4553f,	1.01862f },
	{ 2208.59f,	129.058f,	87.9559f,	2.60984f },
	{ 2195.72f,	136.440f,	88.2164f,	2.62162f },
	{ 2177.87f,	145.974f,	88.2163f,	2.61770f },
	{ 2171.95f,	149.297f,	87.8722f,	4.18456f },
	{ 2164.01f,	137.472f,	84.8832f,	3.90811f },
	{ 2150.95f,	127.355f,	77.7717f,	3.75103f },
	{ 2143.23f,	126.945f,	75.7064f,	2.34045f },
	{ 2139.72f,	135.942f,	72.8990f,	1.76868f },
	{ 2139.86f,	147.191f,	69.7701f,	1.55820f },
	{ 2141.66f,	155.228f,	66.9579f,	1.09010f },
	{ 2146.55f,	164.604f,	64.9990f,	2.45041f },
	{ 2143.41f,	169.553f,	66.2217f,	2.61220f },
	{ 2131.21f,	177.859f,	68.9637f,	2.53759f },
	{ 2118.25f,	186.881f,	68.8476f,	2.53366f },
	{ 2109.50f,	192.808f,	66.2216f,   2.66718f },
	{ 2098.07f,	196.281f,	65.2279f,	2.96406f },
	{ 2088.29f,	206.624f,	64.8821f,	2.43470f },
	{ 2072.24f,	220.331f,	64.8736f,	2.43470f },
	{ 2058.74f,	239.180f,	63.7724f,	2.32396f },
	{ 2054.77f,	243.723f,	63.2682f,	2.39857f }
};

#define MAX_THRALLWP1 27

enum Data
{
    OHF_DATA_NOT_STARTED = 1,
    OHF_DATA_IN_PROGRESS = 2,
    OHF_DATA_PERFORMED = 3,
    OHF_DATA_DONE = 4
};

enum DataIndex
{
    OHF_PHASE_1 = 0, // pre bosss spawn
    OHF_PHASE_2 = 1, // 1st boss
    OHF_PHASE_3 = 2, // pre 2nd boss, trall escort part 1
    OHF_PHASE_4 = 3, // 2nd boss
    OHF_PHASE_5 = 4, // pre 3rd boss, trall escort part 2
    OHF_PHASE_6 = 5, // 3rd boss
    OHF_PHASE_DONE = 6, // Event done

    OHF_END = 7
};

enum OHF_ENTRIES
{
    MAP_OLD_HILSBRAD        = 560,

    GO_LODGE_ABLAZE         = 182589,
    GO_FIRE                 = 183816,

    CN_EROZION              = 18723,
    CN_BRAZEN               = 18725,
    CN_LIEUTENANT_DRAKE     = 17848,
    CN_THRALL               = 17876
};

class OldHilsbradInstance : public MoonInstanceScript
{
	private:
		int32 m_numBarrel;
		uint32 m_phaseData[OHF_END];

	public:
		MOONSCRIPT_INSTANCE_FACTORY_FUNCTION(OldHilsbradInstance, MoonInstanceScript);
		OldHilsbradInstance(MapMgr* pMapMgr) : MoonInstanceScript(pMapMgr)
		{
			m_numBarrel = 0;

			for(int i = 0; i < OHF_END; ++i)
				m_phaseData[i] = OHF_DATA_NOT_STARTED;
		};

		void OnPlayerEnter(Player* pPlayer)
		{
			if(pPlayer->getGender() == 0)
				pPlayer->CastSpell(pPlayer, 35482, true);   // Human Male illusion
			else
				pPlayer->CastSpell(pPlayer, 35483, true);   // Human Female illusion
		};

		void SetData(uint32 pIndex, uint32 pData)
		{
			if(pIndex >= OHF_END)
				return;

			if(pIndex == OHF_PHASE_2)
				mInstance->GetWorldStatesHandler().SetWorldStateForZone( 2367, 0, WORLDSTATE_OLD_HILLSBRAD_BARRELS, 0 );			

			m_phaseData[pIndex] = pData;
		};

		uint32 GetData(uint32 pIndex)
		{
			if(pIndex >= OHF_END)
				return 0;

			return m_phaseData[pIndex];
		};

		void OnGameObjectActivate(GameObject* pGameObject, Player* pPlayer)
		{
			if(pGameObject->GetEntry() != GO_LODGE_ABLAZE || GetData(OHF_PHASE_1) == OHF_DATA_DONE)
				return;

			pGameObject->Despawn(1000, 0);
			m_numBarrel++;
			pGameObject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone( 2367, 0, WORLDSTATE_OLD_HILLSBRAD_BARRELS, m_numBarrel);
			if(m_numBarrel != 5)
				return;

			SetData(OHF_PHASE_1, OHF_DATA_DONE);

			for(PlayerStorageMap::iterator itr = mInstance->m_PlayerStorage.begin(); itr !=  mInstance->m_PlayerStorage.end(); ++itr)
			{
				pPlayer = itr->second;

				QuestLogEntry* qle = pPlayer->GetQuestLogForEntry(10283);
				if(!qle)
					continue;

				qle->SetMobCount(0, qle->GetMobCount(0) + 1);
				qle->SendUpdateAddKill(0);
				qle->UpdatePlayerFields();
			};

			GameObject* pGO = NULL;
			for(uint8 i = 0; i < 21; i++)
			{
				pGO = SpawnGameObject(GO_FIRE, Fires[i].x, Fires[i].y, Fires[i].z, Fires[i].o);
				sEAS.GameobjectDelete(pGO, 10 * 60 * 1000);
			}

			SpawnCreature(CN_LIEUTENANT_DRAKE, 2118.310303f, 89.565969f, 52.453037f, 2.027089f);
		}
};

enum eGossipTexts
{
    EROZION_ON_HELLO = 10475,
    EROZION_ON_FINISH = 10474,
    BRAZAN_ON_HELLO = 9779,
    BRAZAN_NEED_ITEM = 9780,
    THRALL_ON_HELLO = 9568
};

class ErozionGossip : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* Plr)
		{
			GossipMenu* Menu;
			OldHilsbradInstance* pInstance = dynamic_cast<OldHilsbradInstance*>(pObject->GetMapMgr()->GetScript());

			if(!pInstance)
				return;

			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), EROZION_ON_HELLO, Plr);

			if(pInstance->GetData(OHF_PHASE_1) != OHF_DATA_DONE && !Plr->HasItemCount(25853, 1))
				Menu->AddItem(0, "I need a pack of Incendiary Bombs.", 1);

			// It should give another menu if instance is done id: 10474, NYI

			Menu->SendTo(Plr);
		}

		void GossipSelectOption(Object* pObject, Player*  Plr, uint32 Id, uint32 IntId, const char* Code)
		{
			switch(IntId)
			{
				case 1:
					Item* pBombs = objmgr.CreateItem(25853, Plr);
					if(pBombs)
						Plr->GetItemInterface()->AddItemToFreeSlot(pBombs);
					break;
			}
		}

};

class BrazenGossip : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* Plr)
		{
			GossipMenu* Menu;

			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), BRAZAN_ON_HELLO, Plr);
			Menu->AddItem(0, "I am ready to go to Durnholde Keep.", 1);

			Menu->SendTo(Plr);
		}

		void GossipSelectOption(Object* pObject, Player*  Plr, uint32 Id, uint32 IntId, const char* Code)
		{
			GossipMenu* Menu;
			switch(IntId)
			{
				case 1:
					{
						if(!Plr->HasItemCount(25853, 1))
						{
							objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), BRAZAN_NEED_ITEM, Plr);
							Menu->SendTo(Plr);
						}
						else
						{
							Plr->TaxiStart(sTaxiMgr.GetTaxiPath(534), 8317, 0);
							Plr->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);
						}
					}
					break;
			}
		}

};

class LieutenantDrakeAI : public MoonScriptCreatureAI
{
		OldHilsbradInstance* pInstance;

	public:
		MOONSCRIPT_FACTORY_FUNCTION(LieutenantDrakeAI, MoonScriptCreatureAI);
		LieutenantDrakeAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			pInstance = dynamic_cast<OldHilsbradInstance*>(GetInstanceScript());
		};

		void OnCombatStart(Unit* pTarget)
		{
			if(pInstance)
				pInstance->SetData(OHF_PHASE_2, OHF_DATA_IN_PROGRESS);

			ParentClass::OnCombatStart(pTarget);
		};

		void OnCombatStop(Unit* pTarget)
		{
			if(pInstance)
				pInstance->SetData(OHF_PHASE_2, OHF_DATA_PERFORMED);

			ParentClass::OnCombatStop(pTarget);
		};
};

class ThrallAI : public MoonScriptCreatureAI // this will be replaced with escortAI
{
		MOONSCRIPT_FACTORY_FUNCTION(ThrallAI, MoonScriptCreatureAI);
		ThrallAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			SetMoveType(Move_DontMoveWP);
			for(int i = 1; i < MAX_THRALLWP1; ++i)
				AddWaypoint(CreateWaypoint(i, 0, Flag_Walk, ThrallWP1[i]));
		};

		void StartEscort(Player* pPlayer)
		{
			GameObject* pGO = GetNearestGameObject();
			if(pGO)
				pGO->SetState(pGO->GetState() == 1 ? 0 : 1);

			_unit->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_NONE);
			SetMoveType(Move_ForwardThenStop);
		};

		void OnCombatStop(Unit* pTarget)
		{
			ParentClass::OnCombatStop(pTarget);
			SetWaypointToMove(m_currentWp);
		};

		void OnReachWP(uint32 iWaypointId, bool bForwards)
		{
			m_currentWp = iWaypointId;
		};

		uint32 m_currentWp;
};

class ThrallGossip : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* Plr)
		{
			GossipMenu* Menu;
			if(!pObject->IsCreature())
				return;

			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), THRALL_ON_HELLO, Plr);
			Menu->AddItem(0, "[PH] Start escort.", 1);

			Menu->SendTo(Plr);
		}

		void GossipSelectOption(Object* pObject, Player*  Plr, uint32 Id, uint32 IntId, const char* Code)
		{
			switch(IntId)
			{
				case 1:
					ThrallAI* pThrall = TO< ThrallAI* >(TO_CREATURE(pObject)->GetScript());
					if(pThrall)
						pThrall->StartEscort(Plr);
					break;
			}
		}

};

void SetupOldHillsbradFoothills(ScriptMgr* mgr)
{
	mgr->register_instance_script(MAP_OLD_HILSBRAD, &OldHilsbradInstance::Create);
	mgr->register_creature_script(CN_LIEUTENANT_DRAKE, &LieutenantDrakeAI::Create);
	mgr->register_creature_script(CN_THRALL, &ThrallAI::Create);

	GossipScript* eGossip = new ErozionGossip;
	mgr->register_gossip_script(CN_EROZION, eGossip);
	GossipScript* bGossip = new BrazenGossip;
	mgr->register_gossip_script(CN_BRAZEN, bGossip);
	GossipScript* tGossip = new ThrallGossip;
	mgr->register_gossip_script(CN_THRALL, tGossip);
}