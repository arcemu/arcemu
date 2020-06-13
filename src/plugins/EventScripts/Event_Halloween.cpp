/*
Event : Halloween
Author: Angelis
Team  : Sun++
*/

#include "Setup.h"

//Black Cat
class BlackCat : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(BlackCat, MoonScriptCreatureAI);
		BlackCat(Creature* pCreature) : MoonScriptCreatureAI(pCreature) {}

		void OnDied(Unit* pKiller)
		{
			pKiller->CastSpell(pKiller, 39477, true);
			ParentClass::OnDied(pKiller);
		}
};

// HEADLESS HORSEMAN ENCOUNTER
static LocationExtra WaypointGoldshire[] =
{
	{  },
	{ -9502.733398f, 31.395960f, 60.433193f, 1.217366f, Flag_Fly }, //1
	{ -9493.925781f, 55.272415f, 60.433193f, 0.781469f, Flag_Fly },
	{ -9483.589844f, 63.685684f, 60.433193f, 6.224273f, Flag_Fly },
	{ -9463.258789f, 62.515587f, 60.433193f, 6.204639f, Flag_Fly },
	{ -9457.368164f, 48.343132f, 66.931587f, 4.641701f, Flag_Fly },
	{ -9458.772461f, 27.414370f, 77.199722f, 4.001603f, Flag_Fly },
	{ -9473.457031f, 29.496262f, 77.199722f, 1.394081f, Flag_Fly },
	{ -9471.234275f, 44.239151f, 75.393852f, 1.241714f, Flag_Fly },
	{ -9459.474609f, 81.118446f, 71.725540f, 1.720021f, Flag_Fly },
	{ -9467.220703f, 88.311104f, 71.786453f, 2.572178f, Flag_Fly },	//10
	{ -9486.188477f, 83.939690f, 82.718826f, 3.569634f, Flag_Fly }, //11 Starting round (3 rounds left)
	{ -9506.228516f, 36.876194f, 89.180916f, 6.167746f, Flag_Fly },
	{ -9437.569396f, 34.403599f, 75.426025f, 1.270783f, Flag_Fly },
	{ -9448.488281f, 85.930862f, 75.290497f, 2.909909f, Flag_Fly },
	{ -9477.427734f, 86.952667f, 70.950249f, 3.318317f, Flag_Fly }, //15
	{ -9486.188477f, 83.939690f, 82.718826f, 3.569634f, Flag_Fly }, //16 Next round (2 rounds left)
	{ -9506.228516f, 36.876194f, 89.180916f, 6.167746f, Flag_Fly },
	{ -9437.569396f, 34.403599f, 75.426025f, 1.270783f, Flag_Fly },
	{ -9448.488281f, 85.930862f, 75.290497f, 2.909909f, Flag_Fly },
	{ -9477.427734f, 86.952667f, 70.950249f, 3.318317f, Flag_Fly }, //20
	{ -9486.188477f, 83.939690f, 82.718826f, 3.569634f, Flag_Fly }, //21 Next round (1 rounds left)
	{ -9506.228516f, 36.876194f, 89.180916f, 6.167746f, Flag_Fly },
	{ -9437.569396f, 34.403599f, 75.426025f, 1.270783f, Flag_Fly },
	{ -9448.488281f, 85.930862f, 75.290497f, 2.909909f, Flag_Fly },
	{ -9477.427734f, 86.952667f, 70.950249f, 3.318317f, Flag_Fly }, //25
	{ -9486.188477f, 83.939690f, 82.718826f, 3.569634f, Flag_Fly }, //26 Next round (0 rounds left)
	{ -9506.228516f, 36.876194f, 89.180916f, 6.167746f, Flag_Fly },
	{ -9437.569396f, 34.403599f, 75.426025f, 1.270783f, Flag_Fly },
	{ -9448.488281f, 85.930862f, 75.290497f, 2.909909f, Flag_Fly },
	{ -9477.427734f, 86.952667f, 70.950249f, 3.318317f, Flag_Fly } // 30
};

// Headless HorsemanAI
#define CN_HEADLESS_HORSEMAN				23682
#define HEADLESS_HORSEMAN_CLEAVE			42587
#define HEADLESS_HORSEMAN_CONFLAGRATION		42380

class HeadlessHorsemanAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(HeadlessHorsemanAI, MoonScriptCreatureAI);
		HeadlessHorsemanAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			//Scarlet Monastery Boss
		}
};

// Headless Horseman - Fire
#define CN_HEADLESS_HORSEMAN_FIRE				23537

class HeadlessHorsemanFireAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(HeadlessHorsemanFireAI, MoonScriptCreatureAI);
		HeadlessHorsemanFireAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			_unit->CastSpell(_unit, 42971, true);
		}
};

// Shade of the HorsemanAI
#define CN_SHADE_OF_THE_HORSEMAN				23543
#define SHADE_OF_THE_HORSEMAN_SUMMON			42394  //Don't think this one is the correct spell
/*
 * Research
 * NPC:
 * http://www.wowhead.com/?npc=23537 Headless Horseman - Fire (DND) should be invisible? created by ->
 * http://www.wowhead.com/?spell=42118
 * I guess this is the target of the water spells
 * Need to check all visual auras for these http://www.wowhead.com/?search=horseman#uncategorized-spells
 */
class ShadeOfTheHorsemanAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(ShadeOfTheHorsemanAI, MoonScriptCreatureAI);
		ShadeOfTheHorsemanAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			SetCanEnterCombat(false);
			_unit->SetMount(22653);
			//Spells
			mSummon = AddSpell(SHADE_OF_THE_HORSEMAN_SUMMON, Target_Self, 0, 0, 0);

			//Emotes
			AddEmote(Event_OnDied, "So eager you are, for my blood to spill. Yet to vanquish me, 'tis my head you must kill!", Text_Yell, 11969);
			Emote("Prepare yourselves, the bells have tolled! Shelter your weak, your young, and your old! Each of you shall pay the final sum. Cry for mercy, the reckoning has come!", Text_Yell, 11966);	//On Spawn?

			switch(_unit->GetMapMgr()->GetAreaID(_unit->GetPositionX(), _unit->GetPositionY()))
			{
				case 87: // Goldshire
					{
						WPCount = 30;
						WayPoints = WaypointGoldshire;
					}
					break;
			}

			for(int i = 1 ; i <= WPCount ; ++i)
			{
				AddWaypoint(CreateWaypoint(i, 0, WayPoints[i].addition, WayPoints[i]));
			}
		}

		void OnReachWP(uint32 iWaypointId, bool bForwards)
		{
			if(iWaypointId == uint32(WPCount))   // Reached end
			{
				StopWaypointMovement();
				if(GetNearestCreature(CN_HEADLESS_HORSEMAN_FIRE) == NULL)     // CASE players win
				{
					Emote("My flames have died, left not a spark! I shall send you now to the lifeless dark!", Text_Yell, 11968);
					Despawn(30000, 0); //Despawn after 30 secs
				}
				else // CASE players lost
				{
					Emote("Fire consumes! You've tried and failed. Let there be no doubt, justice prevailed!", Text_Yell, 11967);
					Despawn(12000, 0); //Despawn after 12 secs
				}
			}
			else
			{
				switch(_unit->GetMapMgr()->GetAreaID(_unit->GetPositionX(), _unit->GetPositionY()))
				{
					case 87: // Goldshire
						{
							if(iWaypointId == 6)
							{
								_unit->CastSpell(_unit, 42118, true);
							}
						}
						break;
				}
			}
			ParentClass::OnReachWP(iWaypointId, bForwards);
		}

		void OnDied(Unit* pKiller)
		{
			GameObject* Pumpkin = sEAS.SpawnGameobject(TO_PLAYER(pKiller), 2883, _unit->GetPositionX() + RandomFloat(5.0f), _unit->GetPositionY() + RandomFloat(5.0f), _unit->GetPositionZ(), 0, 1, 0, 0, 0, 0);
			if(Pumpkin != NULL)
				_unit->CastSpell(Pumpkin->GetGUID(), 42277, true);

			ParentClass::OnDied(pKiller);
		}

		int8		WPCount;
		LocationExtra*		WayPoints;
		SpellDesc*	mSummon;
};

// Headless Horseman - Wisp Invis
#define CN_HEADLESS_HORSEMAN_WISP_INVIS				24034//						42394

class HeadlessHorsemanWispInvisAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(HeadlessHorsemanWispInvisAI, MoonScriptCreatureAI);
		HeadlessHorsemanWispInvisAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) {}

		void AIUpdate()
		{
			time_t tiempo;
			struct tm* tmPtr;
			tiempo = UNIXTIME;
			tmPtr = localtime(&tiempo);
			if(tmPtr->tm_min == 0 && (tmPtr->tm_hour % 4) == 0)   // All check for the time
			{
				mHeadlessHorseman = GetNearestCreature(CN_SHADE_OF_THE_HORSEMAN);
				if(mHeadlessHorseman == NULL)
				{
					SpawnCreature(CN_SHADE_OF_THE_HORSEMAN, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation());
					SetAIUpdateFreq(4 * 60 * 1000);
				}
			}
			ParentClass::AIUpdate();
		}

		MoonScriptCreatureAI*	mHeadlessHorseman;
};

class WaterBarrel : public GameObjectAIScript
{
	public:
		WaterBarrel(GameObject*  goinstance) : GameObjectAIScript(goinstance) {}
		static GameObjectAIScript* Create(GameObject* GO) { return new WaterBarrel(GO); }

		void OnActivate(Player* pPlayer)
		{
			SlotResult slotresult;
			ItemPrototype* proto = ItemPrototypeStorage.LookupEntry(32971);
			if(!proto)
				return;

			slotresult = pPlayer->GetItemInterface()->FindFreeInventorySlot(proto);

			if(!slotresult.Result)
			{
				pPlayer->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_INVENTORY_FULL);
				return;
			}
			else
			{
				if(pPlayer->GetItemInterface()->GetItemCount(32971, false) == 0)
				{
					Item* itm = objmgr.CreateItem(32971, pPlayer);
					pPlayer->GetItemInterface()->SafeAddItem(itm, slotresult.ContainerSlot, slotresult.Slot);
				}
				else
				{
					pPlayer->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_INVENTORY_FULL);
					return;
				}
			}
		}
};

void SetupHalloween(ScriptMgr* mgr)
{
	mgr->register_creature_script(22816, &BlackCat::Create);
	mgr->register_creature_script(CN_HEADLESS_HORSEMAN, &HeadlessHorsemanAI::Create);
	mgr->register_creature_script(CN_HEADLESS_HORSEMAN_WISP_INVIS, &HeadlessHorsemanWispInvisAI::Create);
	mgr->register_creature_script(CN_HEADLESS_HORSEMAN_FIRE, &HeadlessHorsemanFireAI::Create);
	mgr->register_creature_script(CN_SHADE_OF_THE_HORSEMAN, &ShadeOfTheHorsemanAI::Create);

	mgr->register_gameobject_script(186234, &WaterBarrel::Create);
}