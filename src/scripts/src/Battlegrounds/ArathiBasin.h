/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2011 <http://www.ArcEmu.org/>
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

#define AB_BUFF_RESPAWN_TIME 90000

enum ABBuffs
{
    AB_BUFF_STABLES						= 0,
    AB_BUFF_BLACKSMITH					= 1,
    AB_BUFF_FARM						= 2,
    AB_BUFF_LUMBERMILL					= 3,
    AB_BUFF_MINE						= 4,
    AB_NUM_BUFFS						= 5,
};

enum ABControlPoints
{
    AB_CONTROL_POINT_STABLE				= 0,
    AB_CONTROL_POINT_FARM				= 1,
    AB_CONTROL_POINT_BLACKSMITH			= 2,
    AB_CONTROL_POINT_MINE				= 3,
    AB_CONTROL_POINT_LUMBERMILL			= 4,
    AB_NUM_CONTROL_POINTS				= 5,
};

enum ABSpawnTypes
{
    AB_SPAWN_TYPE_NEUTRAL				= 0,
    AB_SPAWN_TYPE_ALLIANCE_ASSAULT		= 1,
    AB_SPAWN_TYPE_HORDE_ASSAULT			= 2,
    AB_SPAWN_TYPE_ALLIANCE_CONTROLLED	= 3,
    AB_SPAWN_TYPE_HORDE_CONTROLLED		= 4,
    AB_NUM_SPAWN_TYPES					= 5,
};

#define BASE_RESOURCES_GAIN 10
#define RESOURCES_WARNING_THRESHOLD 1400
#define RESOURCES_WINVAL 1600

uint32 buffentries[3] = { 180380, 180362, 180146 };

/* AB Battleground Data */

static float GraveyardLocations[AB_NUM_CONTROL_POINTS][3] =
{
	{ 1201.869507f, 1163.130615f, -56.285969f },												// STABLES
	{ 834.726379f, 784.978699f, -57.081944f },													// FARM
	{ 1016.588318f, 955.184692f, -42.828693f },													// BLACKSMITH
	{ 1211.523682f, 781.556946f, -82.709511f },													// MINE
	{ 772.755676f, 1213.113770f, 15.797392f },													// LUMBERMILL
};

static float NoBaseGYLocations[2][3] =
{
	{ 1354.699951f, 1270.270020f, -11.129100f },												// ALLIANCE
	{ 713.710022f, 638.364014f, -10.599900f },													// HORDE
};

static const char* ControlPointNames[AB_NUM_CONTROL_POINTS] =
{
	"Stable",																					// STABLE
	"Farm",																						// FARM
	"Blacksmith",																				// BLACKSMITH
	"Mine",																						// MINE
	"Lumber Mill",																				// LUMBERMILL
};

static uint32 ControlPointGoIds[AB_NUM_CONTROL_POINTS][AB_NUM_SPAWN_TYPES] =
{
	// NEUTRAL    ALLIANCE-ATTACK    HORDE-ATTACK    ALLIANCE-CONTROLLED    HORDE_CONTROLLED
	{ 180087, 180085, 180086, 180076, 180078 },			// STABLE
	{ 180089, 180085, 180086, 180076, 180078 },			// FARM
	{ 180088, 180085, 180086, 180076, 180078 },			// BLACKSMITH
	{ 180091, 180085, 180086, 180076, 180078 },			// MINE
	{ 180090, 180085, 180086, 180076, 180078 },			// LUMBERMILL
};

static float ControlPointCoordinates[AB_NUM_CONTROL_POINTS][4] =
{
	{ 1166.779541f, 1200.147583f, -56.701763f, -2.251474f },									// STABLE
	{ 806.2484741f, 874.2167358f, -55.9936981f, 0.8377580f },									// FARM
	{ 977.0503540f, 1046.5208740f, -44.8276138f, 0.5410520f },									// BLACKSMITH
	{ 1146.9224854f, 848.1899414f, -110.9200210f, 2.4260077f },									// MINE
	{ 856.141907f, 1148.902100f, 11.184692f, -2.303835f },										// LUMBERMILL
};

static float ControlPointRotations[AB_NUM_CONTROL_POINTS][2] =
{
	{ 0.9025853f, -0.4305111f },																// STABLE
	{ 0.4067366f, 0.9135454f },																	// FARM
	{ 0.2672384f, 0.9636304f },																	// BLACKSMITH
	{ 0.9366722f, 0.3502073f },																	// MINE
	{ 0.9135455f, -0.4067366f },																// LUMBERMILL
};

static float BuffCoordinates[AB_NUM_CONTROL_POINTS][4] =
{
	{ 1185.56616210938f, 1184.62854003906f, -56.3632850646973f, 2.30383467674255f },			// STABLE
	{ 990.113098144531f, 1008.73028564453f, -42.6032752990723f, 0.820304811000824f },			// FARM
	{ 816.906799f, 842.339844f, -56.538746f, 3.272740f },										// BLACKSMITH
	{ 808.846252441406f, 1185.41748046875f, 11.9216051101685f, -0.663225054740906f },			// MINE
	{ 1147.09057617188f, 816.836242675781f, -98.3989562988281f, -0.226892784237862f },			// LUMBERMILL
};

static float BuffRotations[AB_NUM_CONTROL_POINTS][2] =
{
	{ 0.913545489311218f, 0.406736612319946f },													// STABLE
	{ 0.39874908328056f, 0.917060077190399f },													// FARM
	{ 0.913545489311218f, 0.406736612319946f },													// BLACKSMITH
	{ 0.325568109750748f, -0.945518612861633f },												// MINE
	{ 0.113203197717667f, -0.993571877479553f },												// LUMBERMILL
};

static uint32 AssaultFields[AB_NUM_CONTROL_POINTS][2] =
{
	{ WORLDSTATE_AB_CAPTURING_STABLES_ALLIANCE, WORLDSTATE_AB_CAPTURING_STABLES_HORDE },								// STABLE
	{ WORLDSTATE_AB_CAPTURING_FARM_ALLIANCE, WORLDSTATE_AB_CAPTURING_FARM_HORDE },									// FARM
	{ WORLDSTATE_AB_CAPTURING_BLACKSMITH_ALLIANCE, WORLDSTATE_AB_CAPTURING_BLACKSMITH_HORDE },						// BLACKSMITH
	{ WORLDSTATE_AB_CAPTURING_GOLDMINE_ALLIANCE, WORLDSTATE_AB_CAPTURING_GOLDMINE_HORDE },							// MINE
	{ WORLDSTATE_AB_CAPTURING_LUMBERMILL_ALLIANCE, WORLDSTATE_AB_CAPTURING_LUMBERMILL_HORDE },						// LUMBERMILL
};

static uint32 OwnedFields[AB_NUM_CONTROL_POINTS][2] =
{
	{ WORLDSTATE_AB_CAPTURED_STABLES_ALLIANCE, WORLDSTATE_AB_CAPTURED_STABLES_HORDE },								// STABLE
	{ WORLDSTATE_AB_CAPTURED_FARM_ALLIANCE, WORLDSTATE_AB_CAPTURED_FARM_HORDE },										// FARM
	{ WORLDSTATE_AB_CAPTURED_BLACKSMITH_ALLIANCE, WORLDSTATE_AB_CAPTURED_BLACKSMITH_HORDE },							// BLACKSMITH
	{ WORLDSTATE_AB_CAPTURED_GOLDMINE_ALLIANCE, WORLDSTATE_AB_CAPTURED_GOLDMINE_HORDE },								// MINE
	{ WORLDSTATE_AB_CAPTURED_LUMBERMILL_ALLIANCE, WORLDSTATE_AB_CAPTURED_LUMBERMILL_HORDE },							// LUMBERMILL
};

static uint32 NeutralFields[AB_NUM_CONTROL_POINTS] =
{
	WORLDSTATE_AB_SHOW_STABLE_ICON,
	WORLDSTATE_AB_SHOW_FARM_ICON,
	WORLDSTATE_AB_SHOW_BACKSMITH_ICON,
	WORLDSTATE_AB_SHOW_GOLDMINE_ICON,
	WORLDSTATE_AB_SHOW_LUMBERMILL_ICON,
};

static uint32 ResourceUpdateIntervals[6] =
{
	0,
	12000, // 12 secnods
	9000, // 9 seconds
	6000, // 6 seconds
	3000, // 3 seconds
	1000, // 1 second
};

static uint32 PointBonusPerUpdate[6] =
{
	0,
	10,
	10,
	10,
	10,
	30,
};

static uint32 resourcesToGainBH = 260;
static uint32 resourcesToGainBR = 160;

class ArathiBasin : public CBattleground
{
	public:
		GameObject* m_buffs[AB_NUM_BUFFS];
		GameObject* m_controlPoints[AB_NUM_CONTROL_POINTS];
		GameObject* m_controlPointAuras[AB_NUM_CONTROL_POINTS];

	protected:
		list<GameObject*> m_gates;

		uint32 m_resources[2];
		uint32 m_capturedBases[2];
		uint32 m_lastHonorGainResources[2];
		uint32 m_lastRepGainResources[2];
		int32 m_basesOwnedBy[AB_NUM_CONTROL_POINTS];
		int32 m_basesLastOwnedBy[AB_NUM_CONTROL_POINTS];
		int32 m_basesAssaultedBy[AB_NUM_CONTROL_POINTS];
		Creature* m_spiritGuides[AB_NUM_CONTROL_POINTS];
		bool m_nearingVictory[2];
		uint32 m_lgroup;
		bool DefFlag[AB_NUM_CONTROL_POINTS][2];

	public:
		ArathiBasin(MapMgr* mgr, uint32 id, uint32 lgroup, uint32 t);
		~ArathiBasin();

		void HookOnPlayerDeath(Player* plr);
		void HookFlagDrop(Player* plr, GameObject* obj);
		void HookFlagStand(Player* plr, GameObject* obj);
		void HookOnMount(Player* plr);
		void HookOnAreaTrigger(Player* plr, uint32 id);
		bool HookHandleRepop(Player* plr);
		void OnAddPlayer(Player* plr);
		void OnRemovePlayer(Player* plr);
		void OnCreate();
		void HookOnPlayerKill(Player* plr, Player* pVictim);
		void HookOnUnitKill(Player* plr, Unit* pVictim);
		void HookOnHK(Player* plr);
		void HookOnShadowSight();
		void HookGenerateLoot(Player* plr, Object* pCorpse);
		void SpawnBuff(uint32 x);
		LocationVector GetStartingCoords(uint32 Team);
		void HookOnFlagDrop(Player* plr);

		static CBattleground* Create(MapMgr* m, uint32 i, uint32 l, uint32 t) { return new ArathiBasin(m, i, l, t); }

		uint32 GetNameID() { return 40; }
		void OnStart();

		void EventUpdateResources(uint32 Team);
		bool HookSlowLockOpen(GameObject* pGo, Player* pPlayer, Spell* pSpell);

		/* AB Game Mechanics */
		void SpawnControlPoint(uint32 Id, uint32 Type);
		void CaptureControlPoint(uint32 Id, uint32 Team);
		void AssaultControlPoint(Player* pPlayer, uint32 Id);

		void SetIsWeekend(bool isweekend);
};
