/*
 * Moon++ Scripts for Ascent MMORPG Server
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
 * along with this program.  If not, see <http://www.gnu.mOrg/licenses/>.
 */

#include "StdAfx.h"
#include "Setup.h"
#include "../Common/Base.h"
#include "../Common/Instance_Base.h"

/************************************************************************/
/* Event_DarkPortal.cpp Script											*/
/************************************************************************/

//------------------------------- TO-DO --------------------------------//

/* 5. Add special NPC spawning - no clear informations.
 *																	NO INFOS
 */

/* 6. Additional fights (I saw some on youtube movies - like Silvermoon
 *    magisters fighting with infernals and so, but I don't have infos
 *	  about those.
 */

/* 7. Npc special emotes/yells/texts - no clear informations about
 *    situations.													MOSTLY DONE
 */

// ----------------------------- Defines ------------------------------ //
// Event settings

#define MIN_TIME_TO_SUMMON				3000	// it says the smallest diff between one and another portal spawning
#define SPECIAL_UNIT_CHECK_TIME			5000
#define SUMMON_INFERNAL_TIME			50000
#define DEMON_RESPAWN_TIME				180000
#define TEN_DEMON_WAVE_DELAY			50000
#define SPAWN_NEW_UNIT_PORTAL			5000

// Option that fixed core crash related to chain aggro ( loops 'On Combat Start' event and causes 0Kb crashes )
#define DISABLE_CHAIN_AGGRO

// Evil side
#define CN_PIT_COMMANDER				18945
#define CN_FEL_SOLDIER					18944	// +
#define CN_WRATH_MASTER					19005	// + no spells
#define CN_INFERNAL_SIEGEBREAKER		18946	// + no spells

// Horde Side
#define CN_LIEUTENANT_GENERAL_ORION		19253	// + no spells
#define CN_MELGROMM_HIGHMOUNTAIN		18969	// +
#define CN_ORGRIMMAR_GRUNT				18950	// +
#define CN_ORGRIMMAR_SHAMAN				18972	// +
#define CN_DARKSPEAR_AXE_THROWER		18970	// +
#define CN_UNDERCITY_MAGE				18971	// +
#define CN_SILVERMOON_MAGISTER			19006	// + no spells
#define	CN_THUNDER_BLUFF_HUNTER			00000	// --- Thunder Bluff Hunters spawned by Melgromm Highmountain - can't find those

// Alliance Side
#define CN_COMMANDER_DURON				19229	// + no spells
#define CN_JUSTINIUS_THE_HARBINGER		18966	// +
#define CN_LORD_MARSHAL_RAYNOR			19385	// + no spells
#define CN_STORMWIND_SOLDIER			18948	// +
#define CN_DARNASSIAN_ARCHER			18965	// +
#define CN_IRONFORGE_PALADIN			18986	// +
#define CN_STORMWIND_MAGE				18949	// +
#define CN_AZUREMYST_VINDICATOR			19407	// + no spells ---- Spawned by Justinius the Harbinger
#define CN_STORMWIND_MARSHAL			19386	// + no spells ---- Spawned by Lord Marshal Raynor

typedef std::vector< pair< uint32, MoonScriptCreatureAI* > >	AIContainer;
typedef std::map< MoonScriptCreatureAI*, uint32 >				AIMap;


class PitCommanderAI;
class JustiniusTheHarbingerAI;
class MelGrommHighmountainAI;
class InfernalSiegebreakerAI;

//==============================================================================================//
//======================================= Demon Stuff ==========================================//
//==============================================================================================//

// Infernals' spawn points
struct Coords Infernals[]=
{
	{ -261.202820f, 1097.126099f, 41.666908f, 4.543878f },
	{ -255.785507f, 1097.852295f, 41.666908f, 4.680013f },
	{ -247.238403f, 1098.785767f, 41.666908f, 4.697028f },
	{ -241.768570f, 1097.750366f, 41.666908f, 4.580745f }
};

// Demons' spawn points
struct Coords DSPoints[]=
{
	{ -259.384735f, 1207.090210f, 37.783020f, 3.815325f },	// <- 1
	{ -255.924347f, 1214.213379f, 35.689049f, 3.397272f },
	{ -252.891357f, 1209.680542f, 36.874710f, 4.853663f },	// <-- 2
	{ -244.423294f, 1207.830566f, 38.780174f, 4.997303f },
	{ -249.017365f, 1212.447998f, 36.301140f, 1.153214f },	// <--- 3
	{ -243.170227f, 1212.201172f, 36.496788f, 5.213856f },
	{ -237.440628f, 1207.207397f, 37.915062f, 4.767401f },	// <---- 4
	{ -235.196548f, 1211.153564f, 36.487946f, 2.763414f },
	{ -232.128296f, 1202.827637f, 40.078438f, 0.422579f },	// <----- 5
	{ -231.589218f, 1209.054321f, 37.514587f, 4.626029f }
};

// Demons' path from Pit Commander's back to dark portal
struct Coords PitDemonPaths[]=
{
	{},
	{ -259.824768f, 1193.424561f, 41.702896f, 4.637918f },	// <- 1S
	{ -264.705597f, 1176.797485f, 41.786797f, 4.065013f },
	{ -297.176361f, 1155.448364f, 41.229752f, 4.708117f },
	{ -266.808868f, 1091.026733f, 43.115368f, 4.761819f },
	{ -268.872498f, 1073.627075f, 54.306843f, 4.695586f },
	{ -261.097046f, 1014.932922f, 54.326466f, 4.722487f },
	{ -261.324615f,  969.228638f, 84.328705f, 4.714633f },
	{ -261.324615f,  896.572571f, 84.380699f, 4.738189f },	// <- 1F
	{ -257.960693f, 1188.684570f, 41.988678f, 4.680151f },
	{ -280.667053f, 1156.924194f, 41.421211f, 4.066449f },
	{ -292.243652f, 1159.064819f, 41.168304f, 4.718767f },
	{ -263.203674f, 1090.460327f, 43.528366f, 4.659718f },
	{ -263.701752f, 1074.675659f, 54.010033f, 4.717880f },
	{ -258.547886f, 1014.932922f, 54.326466f, 4.722487f },
	{ -258.380275f,  969.228638f, 84.328705f, 4.714633f },
	{ -258.380275f,  896.572571f, 84.380699f, 4.738189f },
	{ -258.138306f, 1190.368774f, 41.925529f, 4.405354f },	// <-- 2S
	{ -261.418549f, 1173.848755f, 41.693317f, 4.586868f },
	{ -271.486603f, 1155.338623f, 41.648853f, 4.708117f },
	{ -258.496887f, 1092.138062f, 42.462128f, 4.778837f },
	{ -260.949005f, 1074.773193f, 53.971958f, 4.700864f },
	{ -255.998726f, 1014.932922f, 54.326466f, 4.722487f },
	{ -255.663504f,  969.228638f, 84.328705f, 4.714633f },
	{ -255.663504f,  896.572571f, 84.380699f, 4.738189f },	// <-- 2F
	{ -254.217911f, 1191.097290f, 42.226845f, 4.165057f },
	{ -263.735260f, 1174.469849f, 41.721027f, 4.570626f },
	{ -267.121826f, 1159.238770f, 41.640697f, 4.809984f },
	{ -255.944672f, 1090.757080f, 43.405441f, 4.776000f },
	{ -258.346588f, 1073.799194f, 54.306805f, 4.684153f },
	{ -253.449566f, 1014.932922f, 54.326466f, 4.722487f },
	{ -252.946733f,  969.228638f, 84.328705f, 4.714633f },
	{ -252.946733f,  896.572571f, 84.380699f, 4.738189f },
	{ -248.362381f, 1187.077271f, 42.552185f, 4.510992f },	// <--- 3S
	{ -248.353027f, 1172.465576f, 41.577877f, 4.624440f },
	{ -249.298492f, 1153.972900f, 41.650188f, 4.700939f },
	{ -253.146927f, 1089.656372f, 44.164940f, 4.773167f },
	{ -255.567947f, 1073.581177f, 54.307045f, 4.714306f },
	{ -250.900406f, 1014.932922f, 54.326466f, 4.722487f },
	{ -250.229962f,  969.228638f, 84.328705f, 4.714633f },
	{ -250.229962f,  896.572571f, 84.380699f, 4.738189f },	// <--- 3F
	{ -244.554230f, 1194.357300f, 42.886631f, 4.500755f },
	{ -248.103027f, 1180.660645f, 41.850349f, 4.704960f },
	{ -248.586914f, 1158.138184f, 41.619884f, 4.685106f },
	{ -250.886108f, 1089.793945f, 44.096958f, 4.773167f },
	{ -250.981888f, 1074.039429f, 54.307217f, 4.717141f },
	{ -248.351246f, 1014.932922f, 54.326466f, 4.722487f },
	{ -247.513191f,  969.228638f, 84.328705f, 4.714633f },
	{ -247.513191f,  896.572571f, 84.380699f, 4.738189f },
	{ -235.939011f, 1187.490479f, 42.089699f, 4.754903f },	// <---- 4S
	{ -231.975601f, 1167.140015f, 41.504807f, 5.202579f },
	{ -221.789551f, 1155.126343f, 41.600170f, 4.708117f },
	{ -244.984070f, 1091.562134f, 42.983116f, 4.708852f },
	{ -247.069885f, 1073.525757f, 54.307018f, 4.748340f },
	{ -245.802086f, 1014.932922f, 54.326466f, 4.722487f },
	{ -244.796420f,  969.228638f, 84.328705f, 4.714633f },
	{ -244.796420f,  896.572571f, 84.380699f, 4.738189f },	// <---- 4F
	{ -233.501907f, 1192.402588f, 42.064373f, 4.805360f },
	{ -219.749039f, 1167.461060f, 41.554111f, 4.666389f },
	{ -220.225464f, 1158.493164f, 41.573982f, 4.621010f },
	{ -242.115265f, 1089.942505f, 44.088120f, 4.708852f },
	{ -242.313522f, 1074.069946f, 54.306789f, 4.722817f },
	{ -243.252926f, 1014.932922f, 54.326466f, 4.722487f },
	{ -242.079649f,  969.228638f, 84.328705f, 4.714633f },
	{ -242.079649f,  896.572571f, 84.380699f, 4.738189f },
	{ -229.465118f, 1185.047363f, 42.061714f, 4.904214f },	// <----- 5S
	{ -221.545197f, 1175.243774f, 41.702663f, 5.599072f },
	{ -199.750961f, 1155.032104f, 41.520683f, 4.708117f },
	{ -238.349487f, 1092.800171f, 42.229187f, 4.708852f },
	{ -240.603683f, 1073.694580f, 54.308067f, 4.766930f },
	{ -240.073766f, 1014.932922f, 54.326466f, 4.722487f },
	{ -239.362878f,  969.228638f, 84.328705f, 4.714633f },
	{ -239.362878f,  896.572571f, 84.380699f, 4.738189f },	// <----- 5F
	{ -228.239334f, 1184.733032f, 42.055592f, 4.672062f },
	{ -204.227600f, 1169.488403f, 42.624237f, 4.638264f },
	{ -204.140060f, 1161.501465f, 42.566750f, 4.723349f },
	{ -234.986465f, 1092.102661f, 42.726883f, 4.708852f },
	{ -235.440399f, 1074.475586f, 54.306778f, 4.681848f },
	{ -238.154602f, 1014.932922f, 54.326466f, 4.722487f },
	{ -236.873672f,  969.283508f, 84.328705f, 4.714633f },
	{ -236.873672f,  896.572571f, 84.380699f, 4.738189f }
};

//==============================================================================================//
//====================================== Common Stuff ==========================================//
//==============================================================================================//

// Commander/Lieutenant positions to script special units spawning
struct Coords AHChecks[]=
{
	{ -265.506012f, 1028.310059f, 54.324100f, 5.455820f },	// ---- Commander Duron
	{ -232.919998f, 1028.270020f, 54.407799f, 3.892080f }	// ---- Lieutenant General Orion
};

// Special units' last WPs
struct Coords SWP[]=
{
	{ -269.433014f, 1072.020020f, 54.490700f, 1.505257f },	// ---- Justinius the Harbinger
	{ -230.486786f, 1072.293945f, 54.353722f, 1.658060f }	// ---- Melgromm Highmountain
};

//==============================================================================================//
//===================================== Alliance Stuff =========================================//
//==============================================================================================//

// Alliance units spawn pos
struct Coords ASPoints[]=
{
	{ -337.971008f, 963.656582f, 54.461300f, 1.675520f }
};

// Alliance spawned units' ways (last WP is before stairs)
struct Coords AWP[]=
{
	{},
	{ -329.585999f, 989.763977f, 54.327599f, 1.181110f },
	{ -318.148010f, 1017.619995f, 54.331100f, 1.181110f },
	{ -312.653015f, 1020.869995f, 54.353600f, 0.146198f},
	{ -309.266998f, 1021.359985f, 54.362099f, 0.146198f },
	{ -294.350006f, 1023.559998f, 54.399700f, 0.146198f },
	{ -280.778992f, 1025.560059f, 54.424801f, 0.146203f },
	{ -261.795013f, 1054.939941f, 54.310101f, 1.121552f },
	{ -261.808014f, 1055.189941f, 54.410099f, 1.621150f },
	{ -262.640015f, 1071.699951f, 54.308201f, 1.591097f }

	// ... to add
};

// Alliance spawned units' WPs from stairs to formation positions (melee)
struct Coords AMWP[]=
{
	{ -272.106995f, 1098.439941f, 42.094299f, 1.343900f },
	{ -268.051819f, 1098.389771f, 41.736443f, 1.637637f },	// custom
	{ -263.979004f, 1098.479980f, 41.854698f, 1.535890f },
	{ -258.976013f, 1098.489990f, 41.849998f, 1.640610f },
	{ -254.639008f, 1098.430054f, 41.849998f, 1.762780f },
	{ -271.437012f, 1093.290039f, 42.185299f, 1.378810f },
	{ -266.118988f, 1093.030029f, 41.976101f, 1.500980f },
	{ -261.527008f, 1093.329956f, 41.849998f, 1.588250f },
	{ -257.638000f, 1093.209961f, 41.941299f, 1.675520f },
	{ -253.451996f, 1092.979980f, 42.139801f, 1.762780f }
};

// Alliance spawned units' WPs from stairs to formation positions (ranged)
struct Coords ARWP[]=
{
	{ -270.035004f, 1087.030029f, 45.921700f, 1.431170f },
	{ -265.329987f, 1087.189941f, 45.865299f, 1.518440f },
	{ -260.299011f, 1087.140015f, 45.944801f, 1.588250f },
	{ -270.035004f, 1084.189941f, 47.854801f, 1.518440f },	// custom
	{ -265.510010f, 1084.189941f, 47.854801f, 1.518440f },
	{ -260.299011f, 1084.189941f, 47.854801f, 1.518440f }	// custom
};

// Alliance spawned units' WPs from stairs to formation positions (casters)
struct Coords ACWP[]=
{
	{ -273.237000f, 1075.800049f, 54.494099f, 1.309757f },
	{ -276.255005f, 1084.689941f, 54.515301f, 1.309757f }
};

//==============================================================================================//
//======================================= Horde Stuff ==========================================//
//==============================================================================================//

// Horde units spawn pos
struct Coords HSPoints[]=
{
	{ -161.376007f, 966.457608f, 54.463501f, 1.745330f }
};

// Horde spawned units' ways (last WP is before stairs)
struct Coords HWP[]=
{
	{},
	{ -173.104004f, 1002.599976f, 54.326099f, 1.867840f },
	{ -178.179993f, 1016.710022f, 54.323601f, 2.424430f },
	{ -186.614594f, 1024.242676f, 54.237057f, 3.075874f },
	{ -213.940994f, 1025.630005f, 54.322102f, 3.086890f },
	{ -219.587997f, 1029.859985f, 54.359200f, 2.372290f },	// not sure if this one should exist
	{ -224.069000f, 1034.199951f, 54.418598f, 2.372290f },
	{ -228.707001f, 1041.520020f, 54.433102f, 2.086830f },	// about this one too
	{ -233.727005f, 1050.369995f, 54.433300f, 2.086830f },
	{ -234.591003f, 1055.890015f, 54.433399f, 1.602190f },
	{ -235.057999f, 1070.739990f, 54.312000f, 2.351221f }
	
	// ... to add
};

// Horde spawned units' WPs from stairs to formation positions (melee)
struct Coords HMWP[]=
{
	{ -229.492004f, 1098.260010f, 41.849998f, 1.413720f },
	{ -233.701004f, 1098.250000f, 41.849998f, 1.413720f },
	{ -237.951996f, 1098.130005f, 41.849998f, 1.570800f },
	{ -242.307999f, 1098.020020f, 41.849998f, 1.483530f },
	{ -246.106003f, 1097.790039f, 41.849998f, 1.413720f },
	{ -231.356003f, 1094.290039f, 41.849998f, 1.588250f },
	{ -236.117996f, 1094.290039f, 41.849998f, 1.518440f },
	{ -240.848007f, 1094.250000f, 41.849998f, 1.448620f },
	{ -245.259995f, 1094.140015f, 41.849998f, 1.378810f },
	{ -249.309998f, 1094.050049f, 41.849998f, 1.326450f }
};

// Horde spawned units' WPs from stairs to formation positions (ranged)
struct Coords HRWP[]=
{
	{ -232.173996f, 1087.959961f, 45.621899f, 1.570800f },	// custom
	{ -235.501999f, 1087.959961f, 45.621899f, 1.361360f },	// custom
	{ -238.819000f, 1087.959961f, 45.621899f, 1.361360f },
	{ -232.123993f, 1085.030029f, 47.635399f, 1.483530f },
	{ -235.501999f, 1084.930054f, 47.670101f, 1.431170f },
	{ -239.162003f, 1084.719971f, 47.769100f, 1.361360f }
};

// Horde spawned units' WPs from stairs to formation positions (ranged)
struct Coords HCWP[]=
{
	{ -227.608994f, 1074.969971f, 54.492901f, 1.855608f },
	{ -225.987000f, 1083.229980f, 54.437401f, 1.855608f }	// custom
};

//==============================================================================================//
//======================================= Other Stuff ==========================================//
//==============================================================================================//

struct Coords AHSPoints[]=
{
	{ -337.971008f, 963.656982f, 54.461300f, 1.675520f },	// ---- Alliance Spawn Point
	{ -161.376007f, 966.458008f, 54.463501f, 1.745330f },	// ---- Horde Spawn Point
};

//==============================================================================================//
//==================================== Instance Class ==========================================//
//==============================================================================================//

#define MAP_OUTLAND									530

class OutlandScript : public MoonInstanceScript
{
public:
	MOONSCRIPT_INSTANCE_FACTORY_FUNCTION( OutlandScript, MoonInstanceScript );
	OutlandScript( MapMgrPointer pMapMgr );

	void Destroy();
};

//==============================================================================================//
//======================================= AI Classes ===========================================//
//==============================================================================================//

#define CN_DARK_PORTAL_EVENT_TRIGGER				18625
#define DARK_PORTAL_EVENT_TRIGGER_SUMMON_INFERNALS	33393
#define DARK_PORTAL_EVENT_TRIGGER_SUMMON_INFERNALS2	33240
#define DARK_PORTAL_EVENT_TRIGGER_REVIVE_SELF		32343

class DarkPortalEventTriggerAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION( DarkPortalEventTriggerAI, MoonScriptCreatureAI );
    DarkPortalEventTriggerAI( CreaturePointer pCreature );
	friend class MelGrommHighmountainAI;
	friend class OrgrimmarGruntAI;
	friend class OrgrimmarShamanAI;
	friend class DarkspearAxeThrowerAI;
	friend class UndercityMageAI;
	friend class JustiniusTheHarbingerAI;
	friend class StormwindSoldierAI;
	friend class IronforgePaladinAI;
	friend class DarnassianArcherAI;
	friend class StormwindMageAI;
	friend class PitCommanderAI;
	friend class InfernalSiegebreakerAI;
	friend class FelSoldierAI;
	friend class WrathMasterAI;

	// Standard function used every 'ai frequency time'
    void AIUpdate();

	// Function to check Commander/Lieutenant units' states and spawn special units if needed
	void SpecialUnitChecks();

	// Bool used to check if we can spawn any unit for this formation to prevent its messing
	bool IsRegimentFull( AIContainer pVectorSet, size_t pSetSize );

	// Function used to check if any alliance unit should be spawned
	void SpawnAllianceUnit();

	// Function used to check if any horde unit should be spawned
	void SpawnHordeUnit();

	// Function used to add unit to Chain Aggro Container
	void AddUnitToChainAggro( UnitPointer pUnit );

	// Function used to check Pit Commander's state
	void PitCommanderChecks();

	// Function used to summon infernals
	void PitCommanderInfernalSummon();

	// Function we use to send 5 demons to attack Dark Portal defenders
	void PitCommanderAttackCmd();

	// Void used to spawn demons
	void PitCommanderSpawn();

	// Procedure that removes all event spawns
	void Destroy();

	// Timers
	int32						mPitCommanderRespawnTimer;
	int32						mAllianceSummonTimer;
	int32						mHordeSummonTimer;
	int32						mAllianceMoveTimer;
	int32						mHordeMoveTimer;
	int32						mInfernalSummonTimer;
	int32						mDemonRespawnTimer;
	int32						mJustiMoveTimer;
	int32						mMelgrMoveTimer;
	int32						mSpecialUnitCheckTimer;

	// Ids
	uint32						mInfernalSpawnPointId;

	// Event additions
	bool						mHasSummonedInfernals;
	bool						mResurrected;
	bool						mResurrect;

	// Chain-aggro related variables
	ChainAggroEntity*			mFormationChain;

	// Creature AI pointers
	PitCommanderAI*				mPitCommander;
	MoonScriptCreatureAI*		mAlliance;
	MoonScriptCreatureAI*		mHorde;
	JustiniusTheHarbingerAI*	mJusti;
	MelGrommHighmountainAI*		mMelgr;

	typedef unordered_set< InfernalSiegebreakerAI* > AIInfernalSet;

	AIMap						mFightingDemonSet;
	AIMap						mQueuedDemonSet;
	AIInfernalSet				mInfernalTable;
	AIContainer					mAllianceMelee;
	AIContainer					mAllianceRanged;
	AIContainer					mAllianceCaster;
	AIContainer					mHordeMelee;
	AIContainer					mHordeRanged;
	AIContainer					mHordeCaster;
};


//=====================================================================================//
//===================================Horde Units=======================================//
//=====================================================================================//

// Can't find his OnCombatStart sound id - _unit->PlaySoundToSet(?); 
#define MELGROMM_HIGHMOUNTAIN_EARTH_SHOCK					22885
#define MELGROMM_HIGHMOUNTAIN_CHAIN_LIGHTNING				33643
#define MELGROMM_HIGHMOUNTAIN_CHAIN_HEAL					33642
#define MELGROMM_HIGHMOUNTAIN_STRENGHT_OF_THE_STORM_TOTEM	33570
#define MELGROMM_HIGHMOUNTAIN_MAGMA_FLOW_TOTEM				33560

// Additional
#define MELGROMM_HIGHMOUNTAIN_PERMANENT_FEIGN_DEATH			31261
#define MELGROMM_HIGHMOUNTAIN_REVIVE_SELF					32343

class MelGrommHighmountainAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION( MelGrommHighmountainAI, MoonScriptCreatureAI );
    MelGrommHighmountainAI( CreaturePointer pCreature );

    void		OnCombatStop( UnitPointer pTarget );
	void		OnReachWP( uint32 pWaypointId, bool pForwards );
	void		Destroy();

	DarkPortalEventTriggerAI*	mTriggerAI;
	uint32						mLastWP;
};

#define ORGRIMMAR_GRUNT_IRON_BITE	32962

class OrgrimmarGruntAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION( OrgrimmarGruntAI, MoonScriptCreatureAI );
    OrgrimmarGruntAI( CreaturePointer pCreature );

	void		OnCombatStop( UnitPointer pTarget );
	void		OnReachWP( uint32 pWaypointId, bool pForwards );
	void		Destroy();

	DarkPortalEventTriggerAI*	mTriggerAI;
	uint32						mLastWP;
};

#define ORGRIMMAR_SHAMAN_FLAME_SHOCK		15616
#define ORGRIMMAR_SHAMAN_BLOODLUST			16170
#define ORGRIMMAR_SHAMAN_LIGHTNING_SHIELD	20545
#define ORGRIMMAR_SHAMAN_SLOW				18972

class OrgrimmarShamanAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION( OrgrimmarShamanAI, MoonScriptCreatureAI );
    OrgrimmarShamanAI( CreaturePointer pCreature );

	void		OnCombatStop( UnitPointer pTarget );
	void		OnReachWP( uint32 pWaypointId, bool pForwards );
	void		Destroy();

	DarkPortalEventTriggerAI*	mTriggerAI;
	uint32						mLastWP;
};

#define DARKSPEAR_AXE_THROWER_THROW		10277

class DarkspearAxeThrowerAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION( DarkspearAxeThrowerAI, MoonScriptCreatureAI );
    DarkspearAxeThrowerAI( CreaturePointer pCreature );

	void		OnCombatStart( UnitPointer pTarget );
	void		OnCombatStop( UnitPointer pTarget );
    void		AIUpdate();
	void		OnReachWP( uint32 pWaypointId, bool pForwards );
	void		Destroy();

	DarkPortalEventTriggerAI*	mTriggerAI;
	uint32						mLastWP;
};

#define UNDERCITY_MAGE_ICEBOLT		33463
#define UNDERCITY_MAGE_FIREBALL		33417
#define UNDERCITY_MAGE_BLIZZARD		33624

class UndercityMageAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(UndercityMageAI, MoonScriptCreatureAI);
    UndercityMageAI(CreaturePointer pCreature);

    void		OnCombatStart( UnitPointer pTarget );
    void		OnCombatStop( UnitPointer pTarget );
    void		AIUpdate();
	void		OnReachWP( uint32 pWaypointId, bool pForwards );
	void		Destroy();

	DarkPortalEventTriggerAI*	mTriggerAI;
	uint32						mLastWP;
};

//=====================================================================================//
//=================================Alliance Units======================================//
//=====================================================================================//

#define JUSTINIUS_THE_HARBINGER_FLASH_OF_LIGHT				33641
#define JUSTINIUS_THE_HARBINGER_CONSECRATION				33559
#define JUSTINIUS_THE_HARBINGER_JUDGEMENT_OF_COMMAND		33554
#define JUSTINIUS_THE_HARBINGER_DIVINE_SHIELD				33581
#define JUSTINIUS_THE_HARBINGER_GREATER_BLESSING_OF_MIGHT	33564

void SpellFunc_Justinius_Consecration( SpellDesc *pThis, MoonScriptCreatureAI *pCreatureAI, UnitPointer pTarget, TargetType pType );

class JustiniusTheHarbingerAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION( JustiniusTheHarbingerAI, MoonScriptCreatureAI );
    JustiniusTheHarbingerAI( CreaturePointer pCreature );

	void		OnCombatStart( UnitPointer pTarget );
    void		OnCombatStop( UnitPointer pTarget );
	bool		CanCastConsecration();
	void		OnReachWP( uint32 pWaypointId, bool pForwards );
	void		Destroy();

	SpellDesc*					mConsecration;
	DarkPortalEventTriggerAI*	mTriggerAI;
	uint32						mLastWP;
};

#define STORMWIND_SOLDIER_STRIKE				33626
#define STORMWIND_SOLDIER_DEMORALIZING_SHOUT	23511

class StormwindSoldierAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION( StormwindSoldierAI, MoonScriptCreatureAI );
    StormwindSoldierAI( CreaturePointer pCreature );

	void		OnCombatStop( UnitPointer pTarget );
	void		OnReachWP( uint32 pWaypointId, bool pForwards );
	void		Destroy();

	DarkPortalEventTriggerAI*	mTriggerAI;
	uint32						mLastWP;
};

#define IRONFORGE_PALADIN_SEAL_OF_SACRIFICE		13903
#define IRONFORGE_PALADIN_EXORCISM				33632
#define IRONFORGE_PALADIN_HAMMER_OF_JUSTICE		13005

class IronforgePaladinAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION( IronforgePaladinAI, MoonScriptCreatureAI );
    IronforgePaladinAI( CreaturePointer pCreature );

	void		OnCombatStop( UnitPointer pTarget );
	void		OnReachWP( uint32 iWaypointId, bool pForwards );
	void		Destroy();

	DarkPortalEventTriggerAI*	mTriggerAI;
	uint32						mLastWP;
};

#define DARNASSIAN_ARCHER_SHOOT		15620

class DarnassianArcherAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION( DarnassianArcherAI, MoonScriptCreatureAI );
    DarnassianArcherAI( CreaturePointer pCreature );

	void		OnCombatStart( UnitPointer pTarget );
	void		OnCombatStop( UnitPointer pTarget );
    void		AIUpdate();
	void		OnReachWP( uint32 pWaypointId, bool pForwards );
	void		Destroy();

	DarkPortalEventTriggerAI*	mTriggerAI;
	uint32						mLastWP;
};

#define STORMWIND_MAGE_FIREBALL			33417
#define STORMWIND_MAGE_ARCANE_MISSILES	33419
#define STORMWIND_MAGE_BLIZZARD			33624

class StormwindMageAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION( StormwindMageAI, MoonScriptCreatureAI );
    StormwindMageAI( CreaturePointer pCreature );

    void		OnCombatStart( UnitPointer pTarget );
    void		OnCombatStop( UnitPointer pTarget );
    void		AIUpdate();
	void		OnReachWP( uint32 pWaypointId, bool pForwards );
	void		Destroy();

	DarkPortalEventTriggerAI*	mTriggerAI;
	uint32						mLastWP;
};

//=====================================================================================//
//===================================Demon Units=======================================//
//=====================================================================================//

#define PIT_COMMANDER_RAIN_OF_FIRE			33627
#define PIT_COMMANDER_CLEAVE				16044
#define PIT_COMMANDER_SEED_OF_CORRUPTION	37826	// Removed, no clear informations about use AddSpell(MACRO, Target_Self, 5, 0, 20);

class PitCommanderAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION( PitCommanderAI, MoonScriptCreatureAI );
    PitCommanderAI( CreaturePointer pCreature );

	void		Destroy();

	DarkPortalEventTriggerAI*	mTriggerAI;
};
// Not sure about use, but it's reasonable way to cast Suicide spell
#define INFERNAL_SIEGEBREAKER_SUICIDE	7

class InfernalSiegebreakerAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION( InfernalSiegebreakerAI, MoonScriptCreatureAI );
    InfernalSiegebreakerAI( CreaturePointer pCreature );

	void		OnCombatStart( UnitPointer pTarget );
	void		AIUpdate();
	void		Destroy();

	DarkPortalEventTriggerAI*	mTriggerAI;
	SpellDesc*					mSuicide;
};

#define FEL_SOLDIER_CUTDOWN		32009
#define FEL_SODLIER_CLEAVE		15496

class FelSoldierAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION( FelSoldierAI, MoonScriptCreatureAI );
	FelSoldierAI( CreaturePointer pCreature );

	void		OnCombatStop( UnitPointer pTarget );
	void		OnReachWP( uint32 pWaypointId, bool pForwards );
	void		Destroy();

	DarkPortalEventTriggerAI*	mTriggerAI;
	uint32						mLastWP;
};

class WrathMasterAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION( WrathMasterAI, MoonScriptCreatureAI );
	WrathMasterAI( CreaturePointer pCreature );

	void		OnCombatStop( UnitPointer pTarget );
	void		OnReachWP( uint32 pWaypointId, bool pForwards );
	void		Destroy();

	DarkPortalEventTriggerAI*	mTriggerAI;
	uint32						mLastWP;
};

void SetupEventDarkPortal( ScriptMgr* pScriptMgr );
