/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2009 <http://www.ArcEmu.org/>
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

#include "StdAfx.h"
UpdateMask Player::m_visibleUpdateMask;
#define COLLISION_INDOOR_CHECK_INTERVAL 1000
#define CANNON 24933 //39692, 34154
#define MORTAR 25003 //33861 -- Triggers Explosion, 39695 --- Summons Mortar
#define NITROUS 27746 //Needs Scripting
#define FLAMETHROWER 39693 //25027
#define MACHINEGUN 25026
#define DROPMINE 25024
#define SHIELD 27759
static uint32 TonkSpecials[4] = {FLAMETHROWER,MACHINEGUN,DROPMINE,SHIELD};
static const uint8 baseRunes[6] = {0,0,1,1,2,2};

Player::Player( uint32 guid )
:
m_mailBox(guid),
m_achievementMgr(this),
m_finishingmovesdodge(false),
disableAppear(false),
disableSummon(false),
//resurrector(0),
SpellCrtiticalStrikeRatingBonus(0),
SpellHasteRatingBonus(1.0f),
m_lifetapbonus(0),
info(NULL), // Playercreate info
SoulStone(0),
SoulStoneReceiver(0),
misdirectionTarget(0),
bReincarnation(false),
removeReagentCost(false),
ignoreShapeShiftChecks(false),
m_furorChance(0),
Seal(0),
judgespell(0),
m_session(0),
TrackingSpell(0),
m_status(0),
offhand_dmg_mod(0.5),
m_isMoving(false),
strafing(false),
jumping(false),
moving(false),
m_ShapeShifted(0),
m_curTarget(0),
m_curSelection(0),
m_lootGuid(0),
m_Summon(NULL),

m_PetNumberMax(0),
m_lastShotTime(0),

m_onTaxi(false),

m_taxi_pos_x(0),
m_taxi_pos_y(0),
m_taxi_pos_z(0),
m_taxi_ride_time(0),

// Attack related variables
m_blockfromspellPCT(0),
m_critfromspell(0),
m_spellcritfromspell(0),
m_hitfromspell(0),

m_healthfromspell(0),
m_manafromspell(0),
m_healthfromitems(0),
m_manafromitems(0),

m_talentresettimes(0),

m_nextSave(getMSTime() + sWorld.getIntRate(INTRATE_SAVE)),

m_resurrectMana(0),
m_resurrectHealth(0),

m_GroupInviter(0),

Lfgcomment(""),

m_Autojoin(false),
m_AutoAddMem(false),
LfmDungeonId(0),
LfmType(0),

m_currentMovement(MOVE_UNROOT),
m_isGmInvisible(false),

//DK
m_invitersGuid(0),

//Trade
mTradeTarget(0),

//Duel
DuelingWith(NULL),
m_duelCountdownTimer(0),
m_duelStatus(0),
m_duelState(DUEL_STATE_FINISHED),		// finished

//WayPoint
waypointunit(NULL),

//PVP
//PvPTimeoutEnabled(false),

m_banned(false),

//Bind possition
m_bind_pos_x(0),
m_bind_pos_y(0),
m_bind_pos_z(0),
m_bind_mapid(0),
m_bind_zoneid(0),

// Rest
m_timeLogoff(0),
m_isResting(0),
m_restState(0),
m_restAmount(0),
m_afk_reason(""),

m_AllowAreaTriggerPort(true),

// Battleground
m_bgEntryPointMap(0),
m_bgEntryPointX(0),
m_bgEntryPointY(0),
m_bgEntryPointZ(0),
m_bgEntryPointO(0),
m_bgQueueType(0),
m_bgQueueInstanceId(0),
m_bgIsQueued(false),
m_bg(0),
m_bgHasFlag(false),
m_bgEntryPointInstance(0),

// gm stuff
//m_invincible(false),
bGMTagOn(false),
CooldownCheat(false),
CastTimeCheat(false),
PowerCheat(false),
GodModeCheat(false),
FlyCheat(false),

//FIX for professions
weapon_proficiency(0x4000), //2^14
//FIX for shit like shirt etc
armor_proficiency(1),

m_bUnlimitedBreath(false),
m_UnderwaterState(0),
m_UnderwaterTime(180000),
m_UnderwaterMaxTime(180000),
m_UnderwaterLastDmg(getMSTime()),
m_SwimmingTime(0),
m_BreathDamageTimer(0),

//transport shit
m_TransporterGUID(0),
m_TransporterX(0.0f),
m_TransporterY(0.0f),
m_TransporterZ(0.0f),
m_TransporterO(0.0f),
m_TransporterUnk(0.0f),
m_lockTransportVariables(false),

// Autoshot variables
m_AutoShotTarget(0),
m_onAutoShot(false),
m_AutoShotDuration(0),
m_AutoShotAttackTimer(0),
m_AutoShotSpell(NULL),

m_AttackMsgTimer(0),

timed_quest_slot(0),
m_GM_SelectedGO(0),

PctIgnoreRegenModifier(0.0f),
m_retainedrage(0),
DetectedRange(0),

m_targetIcon(0),
bShouldHaveLootableOnCorpse(false),
m_MountSpellId(0),
bHasBindDialogOpen(false),
m_CurrentCharm(0),
m_CurrentTransporter(NULL),
m_SummonedObject(NULL),
m_currentLoot(0),
pctReputationMod(0),
roll(0),
mUpdateCount(0),
mCreationCount(0),
mOutOfRangeIdCount(0)
{
	int i,j;

	// Reset vehicle settings
	ResetVehicleSettings();

	//These should really be done in the unit constructor...
	m_currentSpell = NULL;

	m_H_regenTimer = 0;
	m_P_regenTimer = 0;
	//////////////////////////////////////////////////////////////////////////

	//These should be set in the object constructor..
	m_runSpeed = PLAYER_NORMAL_RUN_SPEED;
	m_walkSpeed = 2.5f;

	m_objectTypeId = TYPEID_PLAYER;
	m_valuesCount = PLAYER_END;
	//////////////////////////////////////////////////////////////////////////

	//Why is there a pointer to the same thing in a dirived class? ToDo: sort this out..
	m_uint32Values = _fields;

	memset(m_uint32Values, 0, (PLAYER_END) * sizeof(uint32));
	m_updateMask.SetCount(PLAYER_END);
	SetUInt32Value( OBJECT_FIELD_TYPE,TYPE_PLAYER|TYPE_UNIT|TYPE_OBJECT);
	SetUInt32Value( OBJECT_FIELD_GUID,guid);
	m_wowGuid.Init(GetGUID());
	SetUInt32Value( UNIT_FIELD_FLAGS_2, UNIT_FLAG2_ENABLE_POWER_REGEN );
	SetFloatValue( PLAYER_RUNE_REGEN_1, 0.100000f );
	SetFloatValue( PLAYER_RUNE_REGEN_01, 0.100000f );
	SetFloatValue( PLAYER_RUNE_REGEN_02, 0.100000f );
	SetFloatValue( PLAYER_RUNE_REGEN_03, 0.100000f );

	for(i=0;i<3;i++)
	{
		LfgType[i]=0;
		LfgDungeonId[i]=0;
	}

	for(i=0;i<28;i++){
		MechanicDurationPctMod[i]=0;
	}

	//Trade
	ResetTradeVariables();

	//Tutorials
	for (i = 0 ; i < 8 ; i++ )
		m_Tutorials[ i ] = 0x00;

	m_playedtime[0]		 = 0;
	m_playedtime[1]		 = 0;
	m_playedtime[2]		 = (uint32)UNIXTIME;

	for(i = 0;i < 7; i++)
	{
		FlatResistanceModifierPos[i] = 0;
		FlatResistanceModifierNeg[i] = 0;
		BaseResistanceModPctPos[i] = 0;
		BaseResistanceModPctNeg[i] = 0;
		ResistanceModPctPos[i] = 0;
		ResistanceModPctNeg[i] = 0;
		SpellDelayResist[i] = 0;
		m_casted_amount[i] = 0;
	}

	for(i = 0; i < 5; i++)
	{
		for(j = 0; j < 7; j++)
		{
			SpellHealDoneByAttribute[i][j] = 0;
		}
		FlatStatModPos[i] = 0;
		FlatStatModNeg[i] = 0;
		StatModPctPos[i] = 0;
		StatModPctNeg[i] = 0;
		TotalStatModPctPos[i] = 0;
		TotalStatModPctNeg[i] = 0;
	}

	for(i = 0; i < 12; i++)
	{
		IncreaseDamageByType[i] = 0;
		IncreaseDamageByTypePCT[i] = 0;
		IncreaseCricticalByTypePCT[i] = 0;
	}

    bCreationBuffer.reserve(40000);
	bUpdateBuffer.reserve(30000);//ought to be > than enough ;)
	mOutOfRangeIds.reserve(1000);


	bProcessPending		 = false;
	for(i = 0; i < 25; ++i)
		m_questlog[i] = NULL;

	m_ItemInterface		 = new ItemInterface(this);
	CurrentGossipMenu	   = NULL;

	SDetector =  new SpeedCheatDetector;

	cannibalize			 = false;

	m_AreaID				= 0;
	m_actionsDirty		  = false;
	cannibalizeCount		= 0;
	rageFromDamageDealt	 = 0;
	rageFromDamageTaken	 = 0;

	m_honorToday			= 0;
	m_honorYesterday		= 0;
	m_honorPoints		   = 0;
	m_killsToday			= 0;
	m_killsYesterday		= 0;
	m_killsLifetime		 = 0;
	m_honorless			 = false;
	m_lastSeenWeather	   = 0;
	m_attacking			 = false;

	myCorpse				= 0;
	bCorpseCreateable	   = true;
	blinked				 = false;
	m_explorationTimer	  = getMSTime();
	linkTarget			  = 0;
	AuraStackCheat			 = false;
	ItemStackCheat = false;
	TriggerpassCheat = false;
	m_pvpTimer			  = 0;
	m_globalCooldown = 0;
	m_unstuckCooldown = 0;
	m_lastHonorResetTime	= 0;
	memset(&mActions, 0, PLAYER_ACTION_BUTTON_SIZE);
	tutorialsDirty = true;
	m_TeleportState = 1;
	m_beingPushed = false;
	for(i = 0; i < NUM_CHARTER_TYPES; ++i)
		m_charters[i]=NULL;
	for(i = 0; i < NUM_ARENA_TEAM_TYPES; ++i)
		m_arenaTeams[i]=NULL;
	for(int i = 0; i < 6; ++i)
	{
		m_runes[i] = baseRunes[i];
		m_runetimer[i] = 0;
	}
	flying_aura = 0;
	resend_speed = false;
	rename_pending = false;
	iInstanceType		   = 0;
	memset(reputationByListId, 0, sizeof(FactionReputation*) * 128);

	m_comboTarget = 0;
	m_comboPoints = 0;

	SetFloatValue(UNIT_FIELD_ATTACK_POWER_MULTIPLIER, 0.0f);
	SetFloatValue(UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER, 0.0f);

	UpdateLastSpeeds();

	m_resist_critical[0]=m_resist_critical[1]=0;
	for(i = 0; i < 3; i++ )
	{
		m_resist_hit[i]		= 0.0f;
		m_attack_speed[i]	= 1.0f;
	}
	ok_to_remove = false;
	m_modphyscritdmgPCT = 0;
	m_RootedCritChanceBonus = 0;

	m_ModInterrMRegenPCT = 0;
	m_ModInterrMRegen =0;
	m_RegenManaOnSpellResist=0;
	m_rap_mod_pct = 0;
	m_modblockabsorbvalue = 0;
	m_modblockvaluefromspells = 0;
	m_summoner = m_summonInstanceId = m_summonMapId = 0;
	m_spellcomboPoints = 0;
	m_pendingBattleground = 0;
	m_deathVision = false;
	m_resurrecter = 0;
	m_retainComboPoints = false;
	last_heal_spell = NULL;
	m_playerInfo = NULL;
	m_sentTeleportPosition.ChangeCoords(999999.0f,999999.0f,999999.0f);
	m_speedChangeCounter=1;
	memset(&m_bgScore,0,sizeof(BGScore));
	m_arenaPoints = 0;
	memset(&m_spellIndexTypeTargets, 0, sizeof(uint64)*NUM_SPELL_TYPE_INDEX);
	m_base_runSpeed = m_runSpeed;
	m_base_walkSpeed = m_walkSpeed;
	m_arenateaminviteguid=0;
	m_arenaPoints=0;
	m_honorRolloverTime=0;
	hearth_of_wild_pct = 0;
	raidgrouponlysent=false;
	loot.gold=0;
	m_waterwalk=false;
	m_setwaterwalk=false;
	m_areaSpiritHealer_guid=0;
	m_CurrentTaxiPath=NULL;
	m_setflycheat = false;
	m_fallDisabledUntil = 0;
	m_lfgMatch = NULL;
	m_lfgInviterGuid = 0;
	m_indoorCheckTimer = 0;
	m_taxiMapChangeNode = 0;
	this->OnLogin();

#ifdef ENABLE_COMPRESSED_MOVEMENT
	m_movementBuffer.reserve(5000);
#endif

	m_requiresNoAmmo = false;
	m_safeFall = 0;
	m_noFallDamage = false;
	z_axisposition = 0.0f;
	m_KickDelay = 0;
	m_passOnLoot = false;
	m_changingMaps = true;
	m_outStealthDamageBonusPct = m_outStealthDamageBonusPeriod = m_outStealthDamageBonusTimer = 0;
	m_vampiricEmbrace = m_vampiricTouch = 0;
	LastSeal = 0;
	m_flyhackCheckTimer = 0;
#ifdef TRACK_IMMUNITY_BUG
	m_immunityTime = 0;
#endif

	m_skills.clear();
	m_wratings.clear();
	m_taxiPaths.clear();
	m_QuestGOInProgress.clear();
	m_removequests.clear();
	m_finishedQuests.clear();
	m_finishedDailies.clear();
	quest_spells.clear();
	quest_mobs.clear();

    m_onStrikeSpells.clear();
    m_onStrikeSpellDmg.clear();
    mSpellOverrideMap.clear();
    mSpells.clear();
    mDeletedSpells.clear();
	mShapeShiftSpells.clear();
	m_Pets.clear();
	m_itemsets.clear();
	m_reputation.clear();
	m_channels.clear();
	m_visibleObjects.clear();
	m_forcedReactions.clear();
	m_friends.clear();
	m_ignores.clear();
	m_hasFriendList.clear();

	loginauras.clear();
	OnMeleeAuras.clear();
	damagedone.clear();
	tocritchance.clear();
	m_visibleFarsightObjects.clear();
	SummonSpells.clear();
	PetSpells.clear();
	delayedPackets.clear();
	gmTargets.clear();
	visiblityChangableSet.clear();
	_splineMap.clear();

	for (i=0; i<NUM_COOLDOWN_TYPES; i++) {
		m_cooldownMap[i].clear();
	}
//	m_achievement_points = 0;
}

void Player::OnLogin()
{

}


Player::~Player ( )
{
	if(!ok_to_remove)
	{
		printf("Player deleted from non-logoutplayer!\n");
		OutputCrashLogLine("Player deleted from non-logoutplayer!");

		printStackTrace(); // Win32 Debug

		objmgr.RemovePlayer(this);
	}

	if(m_session)
	{
		m_session->SetPlayer(0);
		if(!ok_to_remove)
			m_session->Disconnect();
	}

	Player * pTarget;
	if(mTradeTarget != 0)
	{
		pTarget = GetTradeTarget();
		if(pTarget)
			pTarget->mTradeTarget = 0;
	}

	pTarget = objmgr.GetPlayer(GetInviter());
	if(pTarget)
		pTarget->SetInviter(0);

	DismissActivePet();
	mTradeTarget = 0;

	if(DuelingWith != 0)
		DuelingWith->DuelingWith = 0;
	DuelingWith = 0;

	CleanupGossipMenu();
	ASSERT(!IsInWorld());

	// delete m_talenttree

	CleanupChannels();
	for(int i = 0; i < 25; ++i)
	{
		if(m_questlog[i] != NULL)
		{
			delete m_questlog[i];
			m_questlog[i] = NULL;
		}
	}

	for(SplineMap::iterator itr = _splineMap.begin(); itr != _splineMap.end(); ++itr)
		delete itr->second;
	_splineMap.clear();

	if(m_ItemInterface) {
		delete m_ItemInterface;
		m_ItemInterface = NULL;
	}

	for(ReputationMap::iterator itr = m_reputation.begin(); itr != m_reputation.end(); ++itr)
		delete itr->second;
	m_reputation.clear();

	m_objectTypeId = TYPEID_UNUSED;

	if(m_playerInfo)
		m_playerInfo->m_loggedInPlayer=NULL;

	delete SDetector;
	SDetector = NULL;

	while( delayedPackets.size() )
	{
		WorldPacket * pck = delayedPackets.next();
		delete pck;
	}

	/*std::map<uint32,AchievementVal*>::iterator itr;
	for(itr=m_achievements.begin();itr!=m_achievements.end();itr++)
		delete itr->second;*/

	std::map< uint32, PlayerPet* >::iterator itr = m_Pets.begin();
	for( ; itr != m_Pets.end(); itr++ )
		delete itr->second;
	m_Pets.clear();
}

ARCEMU_INLINE uint32 GetSpellForLanguage(uint32 SkillID)
{
	switch(SkillID)
	{
	case SKILL_LANG_COMMON:
		return 668;
		break;

	case SKILL_LANG_ORCISH:
		return 669;
		break;

	case SKILL_LANG_TAURAHE:
		return 670;
		break;

	case SKILL_LANG_DARNASSIAN:
		return 671;
		break;

	case SKILL_LANG_DWARVEN:
		return 672;
		break;

	case SKILL_LANG_THALASSIAN:
		return 813;
		break;

	case SKILL_LANG_DRACONIC:
		return 814;
		break;

	case SKILL_LANG_DEMON_TONGUE:
		return 815;
		break;

	case SKILL_LANG_TITAN:
		return 816;
		break;

	case SKILL_LANG_OLD_TONGUE:
		return 817;
		break;

	case SKILL_LANG_GNOMISH:
		return 7430;
		break;

	case SKILL_LANG_TROLL:
		return 7341;
		break;

	case SKILL_LANG_GUTTERSPEAK:
		return 17737;
		break;

	case SKILL_LANG_DRAENEI:
		return 29932;
		break;
	}

	return 0;
}

///====================================================================
///  Create
///  params: p_newChar
///  desc:   data from client to create a new character
///====================================================================
bool Player::Create(WorldPacket& data )
{
	uint8 race,class_,gender,skin,face,hairStyle,hairColor,facialHair,outfitId;

	// unpack data into member variables
	data >> m_name;

	// correct capitalization
	CapitalizeString(m_name);

	data >> race >> class_ >> gender >> skin >> face;
	data >> hairStyle >> hairColor >> facialHair >> outfitId;

	info = objmgr.GetPlayerCreateInfo(race, class_);
	if(!info)
	{
		// info not found... disconnect
		//sCheatLog.writefromsession(m_session, "tried to create invalid player with race %u and class %u", race, class_);
		m_session->Disconnect();
		// don't use Log.LargeErrorMessage() here, it doesn't handle %s %u in the string.
		if(class_ == DEATHKNIGHT)
			sLog.outError("Account Name: %s tried to create a deathknight, however your playercreateinfo table does not support this class, please update your database.", m_session->GetAccountName().c_str());
		else
			sLog.outError("Account Name: %s tried to create an invalid character with race %u and class %u, if this is intended please update your playercreateinfo table inside your database.", m_session->GetAccountName().c_str(), race, class_);
		return false;
	}

	// check that the account CAN create TBC characters, if we're making some
	if(race >= RACE_BLOODELF && !m_session->_accountFlags & (ACCOUNT_FLAG_XPACK_01 || ACCOUNT_FLAG_XPACK_02))
	{
		//sCheatLog.writefromsession(m_session, "tried to create player with race %u and class %u but no expansion flags", race, class_);
		m_session->Disconnect();
		return false;
	}

	m_mapId = info->mapId;
	m_zoneId = info->zoneId;
	m_position.ChangeCoords(info->positionX, info->positionY, info->positionZ);
	m_bind_pos_x = info->positionX;
	m_bind_pos_y = info->positionY;
	m_bind_pos_z = info->positionZ;
	m_bind_mapid = info->mapId;
	m_bind_zoneid = info->zoneId;
	m_isResting = 0;
	m_restAmount = 0;
	m_restState = 0;

	// set race dbc
	myRace = dbcCharRace.LookupEntry(race);
	myClass = dbcCharClass.LookupEntry(class_);
	if(!myRace || !myClass)
	{
		// information not found
		sCheatLog.writefromsession(m_session, "tried to create invalid player with race %u and class %u, dbc info not found", race, class_);
		m_session->Disconnect();
		return false;
	}

	if(myRace->team_id == 7)
		m_team = 0;
	else
		m_team = 1;

	uint8 powertype = static_cast<uint8>(myClass->power_type);

	// Automatically add the race's taxi hub to the character's taximask at creation time ( 1 << (taxi_node_id-1) )
	// this is defined in table playercreateinfo, field taximask
	memcpy(m_taximask, info->taximask, sizeof(m_taximask));

	// Set Starting stats for char
	//SetFloatValue(OBJECT_FIELD_SCALE_X, ((race==RACE_TAUREN)?1.3f:1.0f));
	SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
	SetUInt32Value(UNIT_FIELD_HEALTH, info->health);
	SetUInt32Value(UNIT_FIELD_POWER1, info->mana );
	//SetUInt32Value(UNIT_FIELD_POWER2, 0 ); // this gets devided by 10
	SetUInt32Value(UNIT_FIELD_POWER3, info->focus );
	SetUInt32Value(UNIT_FIELD_POWER4, info->energy );
	SetUInt32Value(UNIT_FIELD_POWER6, 8);

	SetUInt32Value(UNIT_FIELD_MAXHEALTH, info->health);
	SetUInt32Value(UNIT_FIELD_MAXPOWER1, info->mana );
	SetUInt32Value(UNIT_FIELD_MAXPOWER2, info->rage );
	SetUInt32Value(UNIT_FIELD_MAXPOWER3, info->focus );
	SetUInt32Value(UNIT_FIELD_MAXPOWER4, info->energy );
	SetUInt32Value(UNIT_FIELD_MAXPOWER6, 8);
	SetUInt32Value(UNIT_FIELD_MAXPOWER7, 1000 );

	//THIS IS NEEDED
	SetUInt32Value(UNIT_FIELD_BASE_HEALTH, info->health);
	SetUInt32Value(UNIT_FIELD_BASE_MANA, info->mana );
	SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, info->factiontemplate );

	if(class_ == DEATHKNIGHT)
		SetUInt32Value(PLAYER_CHARACTER_POINTS1, sWorld.DKStartTalentPoints); // Default is 0 in case you do not want to modify it
	else
		SetUInt32Value(PLAYER_CHARACTER_POINTS1,0);
	if(class_ != DEATHKNIGHT || sWorld.StartingLevel > 55)
	{
		SetUInt32Value(UNIT_FIELD_LEVEL, sWorld.StartingLevel );
		if(sWorld.StartingLevel >= 10 && class_ != DEATHKNIGHT )
		SetUInt32Value(PLAYER_CHARACTER_POINTS1,sWorld.StartingLevel - 9);
	}
	else
	{
		SetUInt32Value(UNIT_FIELD_LEVEL, 55 );
		SetUInt32Value(PLAYER_NEXT_LEVEL_XP, 148200);
	}
	UpdateGlyphs();

	SetUInt32Value(PLAYER_CHARACTER_POINTS2, sWorld.MaxProfs);

	SetUInt32Value(UNIT_FIELD_BYTES_0, ( ( race ) | ( class_ << 8 ) | ( gender << 16 ) | ( powertype << 24 ) ) );
	//UNIT_FIELD_BYTES_1	(standstate) | (unk1) | (unk2) | (attackstate)
	SetUInt32Value(UNIT_FIELD_BYTES_2, (U_FIELD_BYTES_FLAG_PVP << 8));
	SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED );
	//SetUInt32Value(UNIT_FIELD_BYTES_2, (0x28 << 8) );
	if(class_ == WARRIOR)
		SetShapeShift(FORM_BATTLESTANCE);

	SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED);
	SetUInt32Value(UNIT_FIELD_STAT0, info->strength );
	SetUInt32Value(UNIT_FIELD_STAT1, info->ability );
	SetUInt32Value(UNIT_FIELD_STAT2, info->stamina );
	SetUInt32Value(UNIT_FIELD_STAT3, info->intellect );
	SetUInt32Value(UNIT_FIELD_STAT4, info->spirit );
	SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 0.388999998569489f );
	SetFloatValue(UNIT_FIELD_COMBATREACH, 1.5f   );
	if(race != 10)
	{
		SetUInt32Value(UNIT_FIELD_DISPLAYID, info->displayId + gender );
		SetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID, info->displayId + gender );
	} else	{
		SetUInt32Value(UNIT_FIELD_DISPLAYID, info->displayId - gender );
		SetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID, info->displayId - gender );
	}
	EventModelChange();
	//SetFloatValue(UNIT_FIELD_MINDAMAGE, info->mindmg );
	//SetFloatValue(UNIT_FIELD_MAXDAMAGE, info->maxdmg );
	SetUInt32Value(UNIT_FIELD_ATTACK_POWER, info->attackpower );
	SetUInt32Value(PLAYER_BYTES, ((skin) | (face << 8) | (hairStyle << 16) | (hairColor << 24)));
	//PLAYER_BYTES_2							   GM ON/OFF	 BANKBAGSLOTS   RESTEDSTATE
   // SetUInt32Value(PLAYER_BYTES_2, (facialHair | (0xEE << 8) | (0x01 << 16) | (0x02 << 24)));
	SetUInt32Value(PLAYER_BYTES_2, (facialHair /*| (0xEE << 8)*/  | (0x02 << 24)));//no bank slot by default!

	//PLAYER_BYTES_3						   DRUNKENSTATE				 PVPRANK
	SetUInt32Value(PLAYER_BYTES_3, ((gender) | (0x00 << 8) | (0x00 << 16) | (GetPVPRank() << 24)));
	SetUInt32Value(PLAYER_NEXT_LEVEL_XP, 400);
	SetUInt32Value(PLAYER_FIELD_BYTES, 0x08 );
	SetFloatValue(UNIT_MOD_CAST_SPEED, 1.0f);
	SetUInt32Value(PLAYER_FIELD_MAX_LEVEL, sWorld.m_levelCap);

	// Gold Starting Amount
	SetUInt32Value(PLAYER_FIELD_COINAGE, sWorld.GoldStartAmount);


	for(uint32 x=0;x<7;x++)
		SetFloatValue(PLAYER_FIELD_MOD_DAMAGE_DONE_PCT+x, 1.00);

	SetUInt32Value(PLAYER_FIELD_WATCHED_FACTION_INDEX, 0xEEEEEEEE);

	m_StableSlotCount = 0;
	Item *item;

	for(std::set<uint32>::iterator sp = info->spell_list.begin();sp!=info->spell_list.end();sp++)
	{
		mSpells.insert((*sp));
	}

	m_FirstLogin = true;

	skilllineentry * se;
	for(std::list<CreateInfo_SkillStruct>::iterator ss = info->skills.begin(); ss!=info->skills.end(); ss++)
	{
		se = dbcSkillLine.LookupEntry(ss->skillid);
		if(se->type != SKILL_TYPE_LANGUAGE)
			_AddSkillLine(se->id, ss->currentval, ss->maxval);
	}
	_UpdateMaxSkillCounts();
	//Chances depend on stats must be in this order!
	//UpdateStats();
	//UpdateChances();

	_InitialReputation();

	// Add actionbars
	for(std::list<CreateInfo_ActionBarStruct>::iterator itr = info->actionbars.begin();itr!=info->actionbars.end();++itr)
	{
		setAction(itr->button, itr->action, itr->type, itr->misc);
	}

	for(std::list<CreateInfo_ItemStruct>::iterator is = info->items.begin(); is!=info->items.end(); is++)
	{
		if ( (*is).protoid != 0)
		{
			item=objmgr.CreateItem((*is).protoid,this);
			if(item)
			{
				item->SetUInt32Value(ITEM_FIELD_STACK_COUNT,(*is).amount);
				if((*is).slot<INVENTORY_SLOT_BAG_END)
				{
					if( !GetItemInterface()->SafeAddItem(item, INVENTORY_SLOT_NOT_SET, (*is).slot) )
						item->DeleteMe();
				}
				else
				{
					if( !GetItemInterface()->AddItemToFreeSlot(item) )
						item->DeleteMe();
				}
			}
		}
	}

	sHookInterface.OnCharacterCreate(this);
	load_health = m_uint32Values[UNIT_FIELD_HEALTH];
	load_mana = m_uint32Values[UNIT_FIELD_POWER1];
	return true;
}


void Player::Update( uint32 p_time )
{
	if(!IsInWorld())
		return;

	Unit::Update( p_time );
	uint32 mstime = getMSTime();

	if(m_attacking)
	{
		// Check attack timer.
		if(mstime >= m_attackTimer)
			_EventAttack(false);

		if( m_dualWield && mstime >= m_attackTimer_1 )
			_EventAttack( true );
	}

	if( m_onAutoShot)
	{
		if( m_AutoShotAttackTimer > p_time )
		{
			//sLog.outDebug( "HUNTER AUTOSHOT 0) %i, %i", m_AutoShotAttackTimer, p_time );
			m_AutoShotAttackTimer -= p_time;
		}
		else
		{
			//sLog.outDebug( "HUNTER AUTOSHOT 1) %i", p_time );
			EventRepeatSpell();
		}
	}
	else if(m_AutoShotAttackTimer > 0)
	{
		if(m_AutoShotAttackTimer > p_time)
			m_AutoShotAttackTimer -= p_time;
		else
			m_AutoShotAttackTimer = 0;
	}

	// Breathing
	if( m_UnderwaterState & UNDERWATERSTATE_UNDERWATER )
	{
		// keep subtracting timer
		if( m_UnderwaterTime )
		{
			// not taking dmg yet
			if(p_time >= m_UnderwaterTime)
				m_UnderwaterTime = 0;
			else
				m_UnderwaterTime -= p_time;
		}

		if( !m_UnderwaterTime )
		{
			// check last damage dealt timestamp, and if enough time has elapsed deal damage
			if( mstime >= m_UnderwaterLastDmg )
			{
				uint32 damage = m_uint32Values[UNIT_FIELD_MAXHEALTH] / 10;
				WorldPacket data(SMSG_ENVIRONMENTALDAMAGELOG, 21);
				data << GetGUID() << uint8(DAMAGE_DROWNING) << damage << uint64(0);
				SendMessageToSet(&data, true);

				DealDamage(this, damage, 0, 0, 0);
				m_UnderwaterLastDmg = mstime + 1000;
			}
		}
	}
	else
	{
		// check if we're not on a full breath timer
		if(m_UnderwaterTime < m_UnderwaterMaxTime)
		{
			// regenning
			m_UnderwaterTime += (p_time * 10);

			if(m_UnderwaterTime >= m_UnderwaterMaxTime)
			{
				m_UnderwaterTime = m_UnderwaterMaxTime;
				StopMirrorTimer(1);
			}
		}
	}

	// Lava Damage
	if(m_UnderwaterState & UNDERWATERSTATE_LAVA)
	{
		// check last damage dealt timestamp, and if enough time has elapsed deal damage
		if(mstime >= m_UnderwaterLastDmg)
		{
			uint32 damage = m_uint32Values[UNIT_FIELD_MAXHEALTH] / 5;
			WorldPacket data(SMSG_ENVIRONMENTALDAMAGELOG, 21);
			data << GetGUID() << uint8(DAMAGE_LAVA) << damage << uint64(0);
			SendMessageToSet(&data, true);

			DealDamage(this, damage, 0, 0, 0);
			m_UnderwaterLastDmg = mstime + 1000;
		}
	}

	// Autosave
	if(mstime >= m_nextSave)
		SaveToDB(false);

	if(m_CurrentTransporter && !m_lockTransportVariables)
	{
		// Update our position, using trnasporter X/Y
		float c_tposx = m_CurrentTransporter->GetPositionX() + m_TransporterX;
		float c_tposy = m_CurrentTransporter->GetPositionY() + m_TransporterY;
		float c_tposz = m_CurrentTransporter->GetPositionZ() + m_TransporterZ;
		SetPosition(c_tposx, c_tposy, c_tposz, GetOrientation(), false);
	}

	// Exploration
	if(mstime >= m_explorationTimer)
	{
		_EventExploration();
		m_explorationTimer = mstime + 3000;
	}

	if(m_pvpTimer)
	{
		if(p_time >= m_pvpTimer)
		{
			RemovePvPFlag();
			m_pvpTimer = 0;
		}
		else
			m_pvpTimer -= p_time;
	}

	if (sWorld.Collision)
	{
		if( mstime >= m_indoorCheckTimer )
		{
			if( CollideInterface.IsIndoor( m_mapId, m_position ) )
			{
				 // this is duplicated check, but some mount auras comes w/o this flag set, maybe due to spellfixes.cpp line:663
				if ( m_MountSpellId )
				{
					RemoveAura( m_MountSpellId );
					m_MountSpellId = 0;
				}
				for(uint32 x=MAX_POSITIVE_AURAS_EXTEDED_START;x<MAX_POSITIVE_AURAS_EXTEDED_END;x++)
				{
					if(m_auras[x] && m_auras[x]->GetSpellProto() && m_auras[x]->GetSpellProto()->Attributes & ATTRIBUTES_ONLY_OUTDOORS )
						RemoveAura( m_auras[x] );
				}
			}
			m_indoorCheckTimer = mstime + COLLISION_INDOOR_CHECK_INTERVAL;
		}

		/*if( mstime >= m_flyhackCheckTimer )
		{
			_FlyhackCheck();
			m_flyhackCheckTimer = mstime + 10000;
		}*/
	}

#ifdef TRACK_IMMUNITY_BUG
	bool immune = false;
	for(uint32 i = 0; i < 7; i++)
		if (SchoolImmunityList[i]) immune = true;

	if (immune) {
		if (m_immunityTime == 0) {
			m_immunityTime = mstime;
		}

		if ((mstime - m_immunityTime) > 15000) {
			sLog.outString("plr guid=%d has been immune for > 15sec: %u %u %u %u %u %u %u, resetting states", GetLowGUID(),
				SchoolImmunityList[0], SchoolImmunityList[1], SchoolImmunityList[2], SchoolImmunityList[3],
				SchoolImmunityList[4], SchoolImmunityList[5], SchoolImmunityList[6]);
			for(uint32 i = 0; i < 7; i++)
				if (SchoolImmunityList[i]) SchoolImmunityList[i] = 0;
		}
	} else {
		m_immunityTime = 0;
	}
#endif
}

void Player::EventDismount(uint32 money, float x, float y, float z)
{
	ModUnsigned32Value( PLAYER_FIELD_COINAGE , -(int32)money );

	SetPosition(x, y, z, true);
	if(!m_taxiPaths.size())
		SetTaxiState(false);

	SetTaxiPath(NULL);
	UnSetTaxiPos();
	m_taxi_ride_time = 0;

	SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID , 0);
	RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);
	RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOCK_PLAYER);

	SetPlayerSpeed(RUN, m_runSpeed);

	sEventMgr.RemoveEvents(this, EVENT_PLAYER_TAXI_INTERPOLATE);

	// Save to database on dismount
	SaveToDB(false);

	// If we have multiple "trips" to do, "jump" on the next one :p
	if(m_taxiPaths.size())
	{
		TaxiPath * p = *m_taxiPaths.begin();
		m_taxiPaths.erase(m_taxiPaths.begin());
		TaxiStart(p, taxi_model_id, 0);
	}
}

void Player::_EventAttack( bool offhand )
{
	if (m_currentSpell)
	{
        if(m_currentSpell->GetProto()->ChannelInterruptFlags != 0) // this is a channeled spell - ignore the attack event
            return;
		m_currentSpell->cancel();
		setAttackTimer(500, offhand);
		return;
	}

	if( IsFeared() || IsStunned() )
		return;

	Unit *pVictim = NULL;
	if(m_curSelection)
		pVictim = GetMapMgr()->GetUnit(m_curSelection);

	//Can't find victim, stop attacking
	if (!pVictim)
	{
		sLog.outDetail("Player::Update:  No valid current selection to attack, stopping attack\n");
		setHRegenTimer(5000); //prevent clicking off creature for a quick heal
		EventAttackStop();
		return;
	}

	if (!canReachWithAttack(pVictim))
	{
		if(m_AttackMsgTimer != 1)
		{
			m_session->OutPacket(SMSG_ATTACKSWING_NOTINRANGE);
			m_AttackMsgTimer = 1;
		}
		setAttackTimer(300, offhand);
	}
	else if(!isInFront(pVictim))
	{
		// We still have to do this one.
		if(m_AttackMsgTimer != 2)
		{
			m_session->OutPacket(SMSG_ATTACKSWING_BADFACING);
			m_AttackMsgTimer = 2;
		}
		setAttackTimer(300, offhand);
	}
	else
	{
		m_AttackMsgTimer = 0;

		// Set to weapon time.
		setAttackTimer(0, offhand);

		//pvp timeout reset
		if(pVictim->IsPlayer())
		{
			if (static_cast< Player* >(pVictim)->cannibalize)
			{
				sEventMgr.RemoveEvents(pVictim, EVENT_CANNIBALIZE);
				pVictim->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
				static_cast< Player* >(pVictim)->cannibalize = false;
			}
		}

		if(this->IsStealth())
		{
			RemoveAura( m_stealth );
			SetStealth(0);
		}

		if (!GetOnMeleeSpell() || offhand)
		{
			Strike( pVictim, ( offhand ? OFFHAND : MELEE ), NULL, 0, 0, 0, false, false );

		}
		else
		{
			SpellEntry *spellInfo = dbcSpell.LookupEntry( GetOnMeleeSpell() );
			Spell *spell = SpellPool.PooledNew();
			if (!spell)
				return;
			spell->Init( this, spellInfo, true, NULL );
			spell->extra_cast_number = GetOnMeleeSpellEcn();
			SpellCastTargets targets;
			targets.m_unitTarget = GetSelection();
			spell->prepare( &targets );
			SetOnMeleeSpell( 0 );
		}
	}
}

void Player::_EventCharmAttack()
{
	if(!m_CurrentCharm)
		return;

	Unit *pVictim = NULL;
	if(!IsInWorld())
	{
		m_CurrentCharm = NULL;
		sEventMgr.RemoveEvents(this,EVENT_PLAYER_CHARM_ATTACK);
		return;
	}

	if(m_curSelection == NULL)
	{
		sEventMgr.RemoveEvents(this, EVENT_PLAYER_CHARM_ATTACK);
		return;
	}

	pVictim= GetMapMgr()->GetUnit(m_curSelection);

	//Can't find victim, stop attacking
	if (!pVictim)
	{
		sLog.outError( "WORLD: "I64FMT" doesn't exist.",m_curSelection);
		sLog.outDetail("Player::Update:  No valid current selection to attack, stopping attack\n");
		this->setHRegenTimer(5000); //prevent clicking off creature for a quick heal
		clearStateFlag(UF_ATTACKING);
		EventAttackStop();
	}
	else
	{
		Unit *currentCharm = GetMapMgr()->GetUnit( m_CurrentCharm );

		if( !currentCharm )
			return;

		if (!currentCharm->canReachWithAttack(pVictim))
		{
			if(m_AttackMsgTimer == NULL)
			{
				//m_session->OutPacket(SMSG_ATTACKSWING_NOTINRANGE);
				// 2 sec till next msg.
				m_AttackMsgTimer = 2000;		
			}
			// Shorten, so there isnt a delay when the client IS in the right position.
			sEventMgr.ModifyEventTimeLeft(this, EVENT_PLAYER_CHARM_ATTACK, 100);
		}
		else if(!currentCharm->isInFront(pVictim))
		{
			if(m_AttackMsgTimer == 0)
			{
				m_session->OutPacket(SMSG_ATTACKSWING_BADFACING);
				m_AttackMsgTimer = 2000;		// 2 sec till next msg.
			}
			// Shorten, so there isnt a delay when the client IS in the right position.
			sEventMgr.ModifyEventTimeLeft(this, EVENT_PLAYER_CHARM_ATTACK, 100);
		}
		else
		{
			//if(pVictim->GetTypeId() == TYPEID_UNIT)
			//	pVictim->GetAIInterface()->StopMovement(5000);

			//pvp timeout reset
			/*if(pVictim->IsPlayer())
			{
				if( static_cast< Player* >( pVictim )->DuelingWith == NULL)//Dueling doesn't trigger PVP
					static_cast< Player* >( pVictim )->PvPTimeoutUpdate(false); //update targets timer

				if(DuelingWith == NULL)//Dueling doesn't trigger PVP
					PvPTimeoutUpdate(false); //update casters timer
			}*/

			if (!currentCharm->GetOnMeleeSpell())
			{
				currentCharm->Strike( pVictim, MELEE, NULL, 0, 0, 0, false, false );
			}
			else
			{
				SpellEntry *spellInfo = dbcSpell.LookupEntry(currentCharm->GetOnMeleeSpell());
				currentCharm->SetOnMeleeSpell(0);
				Spell *spell = SpellPool.PooledNew();
				if (!spell)
					return;
				spell->Init(currentCharm,spellInfo,true,NULL);
				SpellCastTargets targets;
				targets.m_unitTarget = GetSelection();
				spell->prepare(&targets);
				//delete spell;		 // deleted automatically, no need to do this.
			}
		}
	}
}

void Player::EventAttackStart()
{
	m_attacking = true;
	if(m_MountSpellId)
        RemoveAura(m_MountSpellId);
}

void Player::EventAttackStop()
{
	if( m_CurrentCharm != 0 )
		sEventMgr.RemoveEvents(this, EVENT_PLAYER_CHARM_ATTACK);

	m_attacking = false;
}

bool Player::HasOverlayUncovered(uint32 overlayID)
{
	WorldMapOverlay const* overlay = dbcWorldMapOverlayStore.LookupEntry(overlayID);
	if (overlay == 0)
		return false;

	if (overlay->areaID && HasAreaExplored(dbcArea.LookupEntry(overlay->areaID)))
		return true;
	if (overlay->areaID_2 && HasAreaExplored(dbcArea.LookupEntry(overlay->areaID_2)))
		return true;
	if (overlay->areaID_3 && HasAreaExplored(dbcArea.LookupEntry(overlay->areaID_3)))
		return true;
	if (overlay->areaID_4 && HasAreaExplored(dbcArea.LookupEntry(overlay->areaID_4)))
		return true;

	return false;
}

bool Player::HasAreaExplored(AreaTable const * at)
{
	if (at == 0)
		return false;

	int offset = at->explorationFlag / 32;
	offset += PLAYER_EXPLORED_ZONES_1;

	uint32 val = (uint32)(1 << (at->explorationFlag % 32));
	uint32 currFields = GetUInt32Value(offset);

	return (currFields & val) != 0;
}

void Player::_EventExploration()
{
	if (IsDead())
		return;

	if (!IsInWorld())
		return;

	if(m_position.x > _maxX || m_position.x < _minX || m_position.y > _maxY || m_position.y < _minY)
		return;

	if(GetMapMgr()->GetCellByCoords(GetPositionX(),GetPositionY()) == NULL)
		return;

	uint16 AreaId = GetMapMgr()->GetAreaID(GetPositionX(),GetPositionY());

	if(!AreaId || AreaId == 0xFFFF)
		return;

	// AreaId fix for undercity and ironforge.  This will now enable rest for these 2 cities.
	// since they're both on the same map, only 1 map id check
	if (GetMapId() == 0)
	{
		// get position
		float ss_x = m_position.x;
		float ss_y = m_position.y;
		float ss_z = m_position.z;

		// Check for Undercity, Tirisfal Glades, and Ruins of Lordaeron, if neither, skip
		if (AreaId == 153 || AreaId == 85 || m_AreaID == 1497)
		{
			// ruins check
			if (ss_z < 74)
			{
				// box with coord 1536,174 -> 1858,353; and z < 62.5 for reachable areas
				if (ss_y > 174 && ss_y < 353 && ss_x > 1536 && ss_x < 1858)
				{
					AreaId = 1497;
				}
			}
			// inner city check
			if (ss_z < 38)
			{
				// box with coord 1238, 11 -> 1823, 640; and z < 38 for undeground
				if (ss_y > 11 && ss_y < 640 && ss_x > 1238 && ss_x < 1823)
				{
					AreaId = 1497;
				}
			}
			// todo bat tunnel, only goes part way, but should be fine for now
		}
		// Check for Ironforge, and Gates of IronForge.. if neither skip
		if (AreaId == 809 || m_AreaID == 1537) {
			// height check
			if (ss_z > 480)
			{
				// box with coord -5097.3, -828 -> -4570, -1349.3; and z > 480.
				if (ss_y > -1349.3 && ss_y < -828 && ss_x > -5097.3 && ss_x < -4570)
				{
					AreaId = 1537;
				}
			}
		}
	}

	AreaTable * at = dbcArea.LookupEntry(AreaId);
	if(at == 0)
		return;

	/*char areaname[200];
	if(at)
	{
		strcpy(areaname, sAreaStore.LookupString((uint32)at->name));
	}
	else
	{
		strcpy(areaname, "UNKNOWN");
	}
	sChatHandler.BlueSystemMessageToPlr(this,areaname);*/

	int offset = at->explorationFlag / 32;
	offset += PLAYER_EXPLORED_ZONES_1;

	uint32 val = (uint32)(1 << (at->explorationFlag % 32));
	uint32 currFields = GetUInt32Value(offset);

	if(AreaId != m_AreaID)
	{
		m_AreaID = AreaId;
		UpdatePvPArea();
		if(GetGroup())
			GetGroup()->UpdateOutOfRangePlayer(this, 128, true, NULL);
	}

	// Zone update, this really should update to a parent zone if one exists.
	//  Will show correct location on your character screen, as well zoneid in DB will have correct value
	//  for any web sites that access that data.
	if(at->ZoneId == 0 && m_zoneId != AreaId)
	{
		ZoneUpdate(AreaId);
	}
	else if (at->ZoneId != 0 && m_zoneId != at->ZoneId)
	{
		ZoneUpdate(at->ZoneId);
	}


	if(at->ZoneId != 0 && m_zoneId != at->ZoneId)
		ZoneUpdate(at->ZoneId);

	bool rest_on = false;
	// Check for a restable area
	if(at->AreaFlags & AREA_CITY_AREA || at->AreaFlags & AREA_CITY)
	{
		// check faction
		if((at->category == AREAC_ALLIANCE_TERRITORY && GetTeam() == 0) || (at->category == AREAC_HORDE_TERRITORY && GetTeam() == 1) )
		{
			rest_on = true;
		}
		else if(at->category != AREAC_ALLIANCE_TERRITORY && at->category != AREAC_HORDE_TERRITORY)
		{
			rest_on = true;
		}
	}
	else
	{
		//second AT check for subzones.
		if(at->ZoneId)
		{
			AreaTable * at2 = dbcArea.LookupEntry(at->ZoneId);
			if(at2 && (at2->AreaFlags & AREA_CITY_AREA || at2->AreaFlags & AREA_CITY ) )
			{
				if((at2->category == AREAC_ALLIANCE_TERRITORY && GetTeam() == 0) || (at2->category == AREAC_HORDE_TERRITORY && GetTeam() == 1) )
				{
					rest_on = true;
				}
				else if(at2->category != AREAC_ALLIANCE_TERRITORY && at2->category != AREAC_HORDE_TERRITORY)
				{
					rest_on = true;
				}
			}
		}
	}

	if (rest_on)
	{
		if(!m_isResting) ApplyPlayerRestState(true);
	}
	else
	{
		if(m_isResting)
		{
			if (sWorld.Collision) {
				const LocationVector & loc = GetPosition();
				if(!CollideInterface.IsIndoor(GetMapId(), loc.x, loc.y, loc.z + 2.0f))
					ApplyPlayerRestState(false);
			} else {
				ApplyPlayerRestState(false);
			}
		}
	}

	if( !(currFields & val) && !GetTaxiState() && !m_TransporterGUID)//Unexplored Area		// bur: we dont want to explore new areas when on taxi
	{
		SetUInt32Value(offset, (uint32)(currFields | val));

		uint32 explore_xp = at->level * 10;
		explore_xp *= float2int32(sWorld.getRate(RATE_EXPLOREXP));

		WorldPacket data(SMSG_EXPLORATION_EXPERIENCE, 8);
		data << at->AreaId << explore_xp;
		m_session->SendPacket(&data);

		GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA);

		if(getLevel() < GetUInt32Value(PLAYER_FIELD_MAX_LEVEL) && explore_xp)
			GiveXP(explore_xp, 0, false);
	}
}

void Player::EventDeath()
{
	if (m_state & UF_ATTACKING)
		EventAttackStop();

	if (m_onTaxi)
		sEventMgr.RemoveEvents(this, EVENT_PLAYER_TAXI_DISMOUNT);

	if(!IS_INSTANCE(GetMapId()) && !sEventMgr.HasEvent(this,EVENT_PLAYER_FORECED_RESURECT)) //Should never be true
		sEventMgr.AddEvent(this,&Player::RepopRequestedPlayer,EVENT_PLAYER_FORECED_RESURECT,PLAYER_FORCED_RESURECT_INTERVAL,1,0); //in case he forgets to release spirit (afk or something)

	RemoveNegativeAuras();
}

void Player::BuildEnumData( WorldPacket * p_data )
{
	*p_data << GetGUID();
	*p_data << m_name;

	*p_data << GetByte(UNIT_FIELD_BYTES_0,0);//uint8(bytes & 0xff); // race
	*p_data << GetByte(UNIT_FIELD_BYTES_0,1);//uint8((bytes >> 8) & 0xff); // class
	*p_data << GetByte(UNIT_FIELD_BYTES_0,2);//uint8((bytes >> 16) & 0xff); // gender

	*p_data << GetUInt32Value(PLAYER_BYTES);

#ifdef USING_BIG_ENDIAN
	*p_data << GetByte(PLAYER_BYTES_2,3);//uint8(bytes & 0xff); //facialhair
#else
	*p_data << GetByte(PLAYER_BYTES_2,0);//uint8(bytes & 0xff); //facialhair
#endif

	*p_data << uint8(getLevel()); //level

	*p_data << m_zoneId;
	*p_data << m_mapId;
	*p_data << m_position;

	*p_data << GetUInt32Value(PLAYER_GUILDID);// guild

	if(rename_pending)  *p_data << uint32(0x00A04342);  // wtf blizz? :P
	else if(m_banned)   *p_data << (uint32)7;	// Banned (cannot login)
	else if(IsDead())   *p_data << (uint32)8704; // Dead (displaying as Ghost)
	else				*p_data << (uint32)1;	// Alive

	*p_data << (uint8)m_restState;	  // rest state

	// pet stuff
	CreatureInfo *info = NULL;

	if(getClass()==WARLOCK || getClass()==HUNTER)
	{
		QueryResult *result = CharacterDatabase.Query("SELECT entry FROM playerpets WHERE ownerguid=%u AND active=1", GetUInt32Value(OBJECT_FIELD_GUID));

		if(result)
		{
			info = CreatureNameStorage.LookupEntry(result->Fetch()[0].GetUInt32());
			delete result;
		}
	}
	if(info)  //PET INFO uint32 displayid,	uint32 level,		 uint32 familyid
		*p_data << uint32(info->Male_DisplayID) << uint32(getLevel()) << uint32(info->Family);
	else
		*p_data << uint32(0) << uint32(0) << uint32(0);

	for (uint32 i = 0; i < EQUIPMENT_SLOT_END ; i++)//max equipment slot is 18....this is strange
	{
		if (GetItemInterface()->GetInventoryItem(i) != NULL)
		{
			*p_data << (uint32)GetItemInterface()->GetInventoryItem(i)->GetProto()->DisplayInfoID;
			*p_data << (uint8)GetItemInterface()->GetInventoryItem(i)->GetProto()->InventoryType;
		}
		else
		{
			*p_data << (uint32)0;
			*p_data << (uint8)0;
		}
	}
	//blizz send 20 slots for some reason(or no reason :P)

	 *p_data << (uint32)0;
	 *p_data << (uint8)0;
}

/*  Gives numtps talent points to the player  */
void Player::GiveTalent(uint32 numtps){
    uint32 currtps = GetUInt32Value(PLAYER_CHARACTER_POINTS1);
	SetUInt32Value(PLAYER_CHARACTER_POINTS1,currtps+numtps);
	}

///  This function sends the message displaying the purple XP gain for the char
///  It assumes you will send out an UpdateObject packet at a later time.
void Player::GiveXP(uint32 xp, const uint64 &guid, bool allowbonus)
{
	if ( xp < 1 )
		return;

	if(getLevel() >= GetUInt32Value(PLAYER_FIELD_MAX_LEVEL))
		return;

	uint32 restxp = xp;

	//add reststate bonus (except for quests)
	if(m_restState == RESTSTATE_RESTED && allowbonus)
	{
		restxp = SubtractRestXP(xp);
		xp += restxp;
	}

	UpdateRestState();
	SendLogXPGain(guid,xp,restxp,guid == 0 ? true : false);

	/*
	uint32 curXP = GetUInt32Value(PLAYER_XP);
	uint32 nextLvlXP = GetUInt32Value(PLAYER_NEXT_LEVEL_XP);
	uint32 newXP = curXP + xp;
	uint32 level = GetUInt32Value(UNIT_FIELD_LEVEL);
	bool levelup = false;

	if(m_Summon != NULL && m_Summon->GetUInt32Value(UNIT_CREATED_BY_SPELL) == 0)
		m_Summon->GiveXP(xp);

	uint32 TotalHealthGain = 0, TotalManaGain = 0;
	uint32 cl=getClass();
	// Check for level-up
	while (newXP >= nextLvlXP)
	{
		levelup = true;
		// Level-Up!
		newXP -= nextLvlXP;  // reset XP to 0, but add extra from this xp add
		level ++;	// increment the level
		if( level > 9)
		{
			//Give Talent Point
			uint32 curTalentPoints = GetUInt32Value(PLAYER_CHARACTER_POINTS1);
			SetUInt32Value(PLAYER_CHARACTER_POINTS1,curTalentPoints+1);
		}
	}
	*/

	int32 newxp = m_uint32Values[PLAYER_XP] + xp;
	int32 nextlevelxp = lvlinfo->XPToNextLevel;
	uint32 level = m_uint32Values[UNIT_FIELD_LEVEL];
	LevelInfo * li;
	bool levelup = false;

	while(newxp >= nextlevelxp && newxp > 0)
	{
		++level;
		li = objmgr.GetLevelInfo(getRace(), getClass(), level);
		if (li == NULL) return;
		newxp -= nextlevelxp;
		nextlevelxp = li->XPToNextLevel;
		levelup = true;

		if(level > 9)
			ModUnsigned32Value(PLAYER_CHARACTER_POINTS1, 1);

		if(level >= GetUInt32Value(PLAYER_FIELD_MAX_LEVEL))
			break;
	}

	if(level > GetUInt32Value(PLAYER_FIELD_MAX_LEVEL))
		level = GetUInt32Value(PLAYER_FIELD_MAX_LEVEL);

	if(levelup)
	{
		m_playedtime[0] = 0; //Reset the "Current level played time"

		SetUInt32Value(UNIT_FIELD_LEVEL, level);
		LevelInfo * oldlevel = lvlinfo;
		lvlinfo = objmgr.GetLevelInfo(getRace(), getClass(), level);
		if (lvlinfo == NULL) return;
		CalculateBaseStats();

		// Generate Level Info Packet and Send to client
        SendLevelupInfo(
            level,
            lvlinfo->HP - oldlevel->HP,
            lvlinfo->Mana - oldlevel->Mana,
            lvlinfo->Stat[0] - oldlevel->Stat[0],
            lvlinfo->Stat[1] - oldlevel->Stat[1],
            lvlinfo->Stat[2] - oldlevel->Stat[2],
            lvlinfo->Stat[3] - oldlevel->Stat[3],
            lvlinfo->Stat[4] - oldlevel->Stat[4]);

		_UpdateMaxSkillCounts();
		UpdateStats();
		//UpdateChances();
		UpdateGlyphs();

		// Set next level conditions
		SetUInt32Value(PLAYER_NEXT_LEVEL_XP, lvlinfo->XPToNextLevel);

		// ScriptMgr hook for OnPostLevelUp
		sHookInterface.OnPostLevelUp(this);

		// Set stats
		for(uint32 i = 0; i < 5; ++i)
		{
			BaseStats[i] = lvlinfo->Stat[i];
			CalcStat(i);
		}

		// Small chance you die and levelup at the same time, and you enter a weird state.
		if(IsDead())
			ResurrectPlayer();

		//set full hp and mana
		SetUInt32Value(UNIT_FIELD_HEALTH,GetUInt32Value(UNIT_FIELD_MAXHEALTH));
		SetUInt32Value(UNIT_FIELD_POWER1,GetUInt32Value(UNIT_FIELD_MAXPOWER1));

		// if warlock has summoned pet, increase its level too
		if(info->class_ == WARLOCK)
		{
			if((m_Summon != NULL) && (m_Summon->IsInWorld()) && (m_Summon->isAlive())) {
				m_Summon->ModUnsigned32Value(UNIT_FIELD_LEVEL, 1);
				m_Summon->ApplyStatsForLevel();
				m_Summon->UpdateSpellList();
			}
		}
		//Event_Achiement_Received(ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL,0,level);
		GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL);
	}

	// Set the update bit
	SetUInt32Value(PLAYER_XP, newxp);

	HandleProc(PROC_ON_GAIN_EXPIERIENCE, this, NULL);
	m_procCounter = 0;

}

void Player::smsg_InitialSpells()
{
	PlayerCooldownMap::iterator itr, itr2;

	uint16 spellCount = (uint16)mSpells.size();
	size_t itemCount = m_cooldownMap[0].size() + m_cooldownMap[1].size();
	uint32 mstime = getMSTime();
	size_t pos;

	WorldPacket data(SMSG_INITIAL_SPELLS, 5 + (spellCount * 4) + (itemCount * 4) );
	data << uint8(0);
	data << uint16(spellCount); // spell count

	SpellSet::iterator sitr;
	for (sitr = mSpells.begin(); sitr != mSpells.end(); ++sitr)
	{
		// todo: check out when we should send 0x0 and when we should send 0xeeee
		// this is not slot,values is always eeee or 0,seems to be cooldown
		data << uint16(*sitr);				   // spell id
		data << uint16(0x0000);
	}

	pos = data.wpos();
	data << uint16( 0 );		// placeholder

	itemCount = 0;
	for( itr = m_cooldownMap[COOLDOWN_TYPE_SPELL].begin(); itr != m_cooldownMap[COOLDOWN_TYPE_SPELL].end(); )
	{
		itr2 = itr++;

		// don't keep around expired cooldowns
		if( itr2->second.ExpireTime < mstime || (itr2->second.ExpireTime - mstime) < 10000 )
		{
			m_cooldownMap[COOLDOWN_TYPE_SPELL].erase( itr2 );
			continue;
		}

		data << uint16( itr2->first );						// spell id
		data << uint16( itr2->second.ItemId );				// item id
		data << uint16( 0 );								// spell category
		data << uint32( itr2->second.ExpireTime - mstime );	// cooldown remaining in ms (for spell)
		data << uint32( 0 );								// cooldown remaining in ms (for category)

		++itemCount;

#ifdef _DEBUG
		Log.Debug("InitialSpells", "sending spell cooldown for spell %u to %u ms", itr2->first, itr2->second.ExpireTime - mstime);
#endif
	}

	for( itr = m_cooldownMap[COOLDOWN_TYPE_CATEGORY].begin(); itr != m_cooldownMap[COOLDOWN_TYPE_CATEGORY].end(); )
	{
		itr2 = itr++;

		// don't keep around expired cooldowns
		if( itr2->second.ExpireTime < mstime || (itr2->second.ExpireTime - mstime) < 10000 )
		{
			m_cooldownMap[COOLDOWN_TYPE_CATEGORY].erase( itr2 );
			continue;
		}

		data << uint16( itr2->second.SpellId );				// spell id
		data << uint16( itr2->second.ItemId );				// item id
		data << uint16( itr2->first );						// spell category
		data << uint32( 0 );								// cooldown remaining in ms (for spell)
		data << uint32( itr2->second.ExpireTime - mstime );	// cooldown remaining in ms (for category)

		++itemCount;

#ifdef _DEBUG
		Log.Debug("InitialSpells", "sending category cooldown for cat %u to %u ms", itr2->first, itr2->second.ExpireTime - mstime);
#endif
	}


#ifdef USING_BIG_ENDIAN
	*(uint16*)&data.contents()[pos] = swap16((uint16)itemCount);
#else
	*(uint16*)&data.contents()[pos] = (uint16)itemCount;
#endif

	GetSession()->SendPacket(&data);

	uint32 v = 0;
	GetSession()->OutPacket(0x041d, 4, &v);
	//Log::getSingleton( ).outDetail( "CHARACTER: Sent Initial Spells" );
}

void Player::_SavePet(QueryBuffer * buf)
{
	// Remove any existing info
	if(buf == NULL)
		CharacterDatabase.Execute("DELETE FROM playerpets WHERE ownerguid=%u", GetUInt32Value(OBJECT_FIELD_GUID));
	else
		buf->AddQuery("DELETE FROM playerpets WHERE ownerguid=%u", GetUInt32Value(OBJECT_FIELD_GUID));

	if( m_Summon && m_Summon->IsInWorld() && m_Summon->GetPetOwner() == this )	// update PlayerPets array with current pet's info
	{
		PlayerPet*pPet = GetPlayerPet(m_Summon->m_PetNumber);
		if(!pPet || pPet->active == false)
			m_Summon->UpdatePetInfo(true);
		else m_Summon->UpdatePetInfo(false);

		if(!m_Summon->Summon)	   // is a pet
		{
			// save pet spellz
			PetSpellMap::iterator itr = m_Summon->mSpells.begin();
			uint32 pn = m_Summon->m_PetNumber;
			if(buf == NULL)
				CharacterDatabase.Execute("DELETE FROM playerpetspells WHERE petnumber=%u", pn);
			else
				buf->AddQuery("DELETE FROM playerpetspells WHERE petnumber=%u", pn);

			for(; itr != m_Summon->mSpells.end(); ++itr)
			{
				if(buf == NULL)
					CharacterDatabase.Execute("INSERT INTO playerpetspells VALUES(%u, %u, %u, %u)", GetLowGUID(), pn, itr->first->Id, itr->second);
				else
					buf->AddQuery("INSERT INTO playerpetspells VALUES(%u, %u, %u, %u)", GetLowGUID(), pn, itr->first->Id, itr->second);
			}
		}
	}

	std::stringstream ss;

	for(std::map<uint32, PlayerPet*>::iterator itr = m_Pets.begin(); itr != m_Pets.end(); itr++)
	{
		ss.rdbuf()->str("");
		ss << "REPLACE INTO playerpets VALUES('"
			<< GetLowGUID() << "','"
			<< itr->second->number << "','"
			<< itr->second->name << "','"
			<< itr->second->entry << "','"
			<< itr->second->fields << "','"
			<< itr->second->xp << "','"
			<< (itr->second->active ?  1 : 0) + itr->second->stablestate * 10 << "','"
			<< itr->second->level << "','"
			<< itr->second->actionbar << "','"
			<< itr->second->happinessupdate << "','"
			<< itr->second->summon << "','"
			<< (long)itr->second->reset_time << "','"
			<< itr->second->reset_cost << "','"
			<< itr->second->spellid << "')";

		if(buf == NULL)
			CharacterDatabase.ExecuteNA(ss.str().c_str());
		else
			buf->AddQueryStr(ss.str());
	}
}

void Player::_SavePetSpells(QueryBuffer * buf)
{
	// Remove any existing
	if(buf == NULL)
		CharacterDatabase.Execute("DELETE FROM playersummonspells WHERE ownerguid=%u", GetLowGUID());
	else
		buf->AddQuery("DELETE FROM playersummonspells WHERE ownerguid=%u", GetLowGUID());

	// Save summon spells
	map<uint32, set<uint32> >::iterator itr = SummonSpells.begin();
	for(; itr != SummonSpells.end(); ++itr)
	{
		set<uint32>::iterator it = itr->second.begin();
		for(; it != itr->second.end(); ++it)
		{
			if(buf == NULL)
				CharacterDatabase.Execute("INSERT INTO playersummonspells VALUES(%u, %u, %u)", GetLowGUID(), itr->first, (*it));
			else
				buf->AddQuery("INSERT INTO playersummonspells VALUES(%u, %u, %u)", GetLowGUID(), itr->first, (*it));
		}
	}
}

void Player::AddSummonSpell(uint32 Entry, uint32 SpellID)
{
	SpellEntry * sp = dbcSpell.LookupEntry(SpellID);
	map<uint32, set<uint32> >::iterator itr = SummonSpells.find(Entry);
	if(itr == SummonSpells.end())
		SummonSpells[Entry].insert(SpellID);
	else
	{
		set<uint32>::iterator it3;
		for(set<uint32>::iterator it2 = itr->second.begin(); it2 != itr->second.end();)
		{
			it3 = it2++;
			if(dbcSpell.LookupEntry(*it3)->NameHash == sp->NameHash)
				itr->second.erase(it3);
		}
		itr->second.insert(SpellID);
	}
}

void Player::RemoveSummonSpell(uint32 Entry, uint32 SpellID)
{
	map<uint32, set<uint32> >::iterator itr = SummonSpells.find(Entry);
	if(itr != SummonSpells.end())
	{
		itr->second.erase(SpellID);
		if(itr->second.size() == 0)
			SummonSpells.erase(itr);
	}
}

set<uint32>* Player::GetSummonSpells(uint32 Entry)
{
	map<uint32, set<uint32> >::iterator itr = SummonSpells.find(Entry);
	if(itr != SummonSpells.end())
	{
		return &(itr->second);
	}
	return 0;
}

void Player::_LoadPet(QueryResult * result)
{
	m_PetNumberMax=0;
	if(!result)
		return;

	do
	{
		Field *fields = result->Fetch();
		fields = result->Fetch();

		PlayerPet *pet = new PlayerPet;
		pet->number  = fields[1].GetUInt32();
		pet->name	= fields[2].GetString();
		pet->entry   = fields[3].GetUInt32();
		pet->fields  = fields[4].GetString();
		pet->xp	  = fields[5].GetUInt32();
		pet->active  = fields[6].GetInt8()%10 > 0 ? true : false;
		pet->stablestate = fields[6].GetInt8() / 10;
		pet->level   = fields[7].GetUInt32();
		pet->actionbar = fields[8].GetString();
		pet->happinessupdate = fields[9].GetUInt32();
		pet->summon = (fields[10].GetUInt32()>0 ? true : false);
		pet->reset_time = fields[11].GetUInt32();
		pet->reset_cost = fields[12].GetUInt32();
		pet->spellid = fields[13].GetUInt32();

		m_Pets[pet->number] = pet;

		if(pet->number > m_PetNumberMax)
			m_PetNumberMax =  pet->number;
	}while(result->NextRow());
}

void Player::SpawnPet( uint32 pet_number )
{
	std::map< uint32, PlayerPet* >::iterator itr = m_Pets.find( pet_number );
	if( itr == m_Pets.end() )
	{
		sLog.outError("PET SYSTEM: "I64FMT" Tried to load invalid pet %d", GetGUID(), pet_number);
		return;
	}
	Pet *pPet = objmgr.CreatePet( itr->second->entry );
	pPet->LoadFromDB( this, itr->second );
	
	if( itr->second->spellid )
	{
		//if demonic sacrifice auras are still active, remove them
		RemoveAura( 18789 );
		RemoveAura( 18790 );
		RemoveAura( 18791 );
		RemoveAura( 18792 );
		RemoveAura( 35701 );
	}
}
void Player::SpawnActivePet()
{
	if( m_Summon != NULL || getClass() != HUNTER || !isAlive() ) //TODO: only hunters for now
		return;

	std::map< uint32, PlayerPet* >::iterator itr = m_Pets.begin();
	for( ; itr != m_Pets.end(); itr++ )
		if( itr->second->stablestate == STABLE_STATE_ACTIVE && itr->second->active )
		{
			SpawnPet( itr->first );
			return;
		}
}
void Player::DismissActivePet()
{
	if( m_Summon == NULL )
		return;

	if( m_Summon->IsSummon() )
		m_Summon->Dismiss();			// summons
	else
		m_Summon->Remove( true, false );// hunter pets
}

void Player::_LoadPetSpells(QueryResult * result)
{
	std::stringstream query;
	std::map<uint32, std::list<uint32>* >::iterator itr;
	uint32 entry = 0;
	uint32 spell = 0;

	if(result)
	{
		do
		{
			Field *fields = result->Fetch();
			entry = fields[1].GetUInt32();
			spell = fields[2].GetUInt32();
			AddSummonSpell(entry, spell);
		}
		while( result->NextRow() );
	}
}

void Player::addSpell(uint32 spell_id)
{
	SpellSet::iterator iter = mSpells.find(spell_id);
	if(iter != mSpells.end())
		return;

	mSpells.insert(spell_id);
	if(IsInWorld())
	{
#ifdef USING_BIG_ENDIAN
		uint32 swapped = swap32(spell_id);
		m_session->OutPacket(SMSG_LEARNED_SPELL, 4, &swapped);
#else
		m_session->OutPacket(SMSG_LEARNED_SPELL, 4, &spell_id);
#endif
	}

	// Check if we're a deleted spell
	iter = mDeletedSpells.find(spell_id);
	if(iter != mDeletedSpells.end())
		mDeletedSpells.erase(iter);

	// Check if we're logging in.
	if(!IsInWorld())
		return;

	// Add the skill line for this spell if we don't already have it.
	skilllinespell * sk = objmgr.GetSpellSkill(spell_id);
	SpellEntry * spell = dbcSpell.LookupEntry(spell_id);
	if(sk && !_HasSkillLine(sk->skilline))
	{
		skilllineentry * skill = dbcSkillLine.LookupEntry(sk->skilline);
		uint32 max = 1;
		switch(skill->type)
		{
			case SKILL_TYPE_PROFESSION:
				max=75*((spell->RankNumber)+1);
				ModUnsigned32Value( PLAYER_CHARACTER_POINTS2, -1 ); // we are learning a proffesion, so substract a point.
				break;
			case SKILL_TYPE_SECONDARY:
				max=75*((spell->RankNumber)+1);
				break;
			case SKILL_TYPE_WEAPON:
				max=5*getLevel();
				break;
			case SKILL_TYPE_CLASS:
			case SKILL_TYPE_ARMOR:
				if(skill->id == SKILL_LOCKPICKING || skill->id == SKILL_POISONS)
					max=5*getLevel();
				break;
		};

		_AddSkillLine(sk->skilline, 1, max);
		_UpdateMaxSkillCounts();
	}
	m_achievementMgr.UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL, spell_id, 1, 0);
	if(spell->MechanicsType == MECHANIC_MOUNTED) // Mounts
	{
		// miscvalue1==777 for mounts, 778 for pets
		m_achievementMgr.UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_NUMBER_OF_MOUNTS, 777, 0, 0);
	}
	else if(spell->Effect[0]==SPELL_EFFECT_SUMMON) // Companion pet?
	{
		// miscvalue1==777 for mounts, 778 for pets
		// make sure it's a companion pet, not some other summon-type spell
		if(strncmp(spell->Description,"Right Cl", 8) == 0) // "Right Click to summon and dismiss " ...
		{
			m_achievementMgr.UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_NUMBER_OF_MOUNTS, 778, 0, 0);
		}
	}
}

//===================================================================================================================
//  Set Create Player Bits -- Sets bits required for creating a player in the updateMask.
//  Note:  Doesn't set Quest or Inventory bits
//  updateMask - the updatemask to hold the set bits
//===================================================================================================================
void Player::_SetCreateBits(UpdateMask *updateMask, Player *target) const
{
	if(target == this)
	{
		Object::_SetCreateBits(updateMask, target);
	}
	else
	{
		for(uint32 index = 0; index < m_valuesCount; index++)
		{
			if(m_uint32Values[index] != 0 && Player::m_visibleUpdateMask.GetBit(index))
				updateMask->SetBit(index);
		}
	}
}


void Player::_SetUpdateBits(UpdateMask *updateMask, Player *target) const
{
	if(target == this)
	{
		Object::_SetUpdateBits(updateMask, target);
	}
	else
	{
		Object::_SetUpdateBits(updateMask, target);
		*updateMask &= Player::m_visibleUpdateMask;
	}
}


void Player::InitVisibleUpdateBits()
{
	Player::m_visibleUpdateMask.SetCount(PLAYER_END);
	Player::m_visibleUpdateMask.SetBit(OBJECT_FIELD_GUID);
	Player::m_visibleUpdateMask.SetBit(OBJECT_FIELD_TYPE);
	Player::m_visibleUpdateMask.SetBit(OBJECT_FIELD_ENTRY);
	Player::m_visibleUpdateMask.SetBit(OBJECT_FIELD_SCALE_X);

	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_SUMMON);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_SUMMON+1);

	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_TARGET);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_TARGET+1);

	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_HEALTH);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_POWER1);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_POWER2);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_POWER3);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_POWER4);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_POWER5);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_POWER6);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_POWER7);

	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MAXHEALTH);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MAXPOWER1);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MAXPOWER2);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MAXPOWER3);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MAXPOWER4);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MAXPOWER5);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MAXPOWER6);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MAXPOWER7);

	Player::m_visibleUpdateMask.SetBit(UNIT_VIRTUAL_ITEM_SLOT_ID);
	Player::m_visibleUpdateMask.SetBit(UNIT_VIRTUAL_ITEM_SLOT_ID_1);
	Player::m_visibleUpdateMask.SetBit(UNIT_VIRTUAL_ITEM_SLOT_ID_2);

	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_LEVEL);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_FACTIONTEMPLATE);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_BYTES_0);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_FLAGS);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_FLAGS_2);

	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_BASEATTACKTIME);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_BASEATTACKTIME_01);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_BOUNDINGRADIUS);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_COMBATREACH);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_DISPLAYID);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_NATIVEDISPLAYID);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MOUNTDISPLAYID);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_BYTES_1);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MOUNTDISPLAYID);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_PETNUMBER);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_PET_NAME_TIMESTAMP);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_CHANNEL_OBJECT);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_CHANNEL_OBJECT+1);
	Player::m_visibleUpdateMask.SetBit(UNIT_CHANNEL_SPELL);
	Player::m_visibleUpdateMask.SetBit(UNIT_DYNAMIC_FLAGS);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_HOVERHEIGHT);

	Player::m_visibleUpdateMask.SetBit(PLAYER_FLAGS);
	Player::m_visibleUpdateMask.SetBit(PLAYER_BYTES);
	Player::m_visibleUpdateMask.SetBit(PLAYER_BYTES_2);
	Player::m_visibleUpdateMask.SetBit(PLAYER_BYTES_3);
	Player::m_visibleUpdateMask.SetBit(PLAYER_GUILD_TIMESTAMP);
	Player::m_visibleUpdateMask.SetBit(PLAYER_DUEL_TEAM);
	Player::m_visibleUpdateMask.SetBit(PLAYER_DUEL_ARBITER);
	Player::m_visibleUpdateMask.SetBit(PLAYER_DUEL_ARBITER+1);
	Player::m_visibleUpdateMask.SetBit(PLAYER_GUILDID);
	Player::m_visibleUpdateMask.SetBit(PLAYER_GUILDRANK);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_BASE_MANA);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_BYTES_2);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_AURASTATE);

	// Players visible items are not inventory stuff
    for(uint16 i = 0; i < EQUIPMENT_SLOT_END; ++i)
	{
		uint32 offset = i * 18;

		Player::m_visibleUpdateMask.SetBit(PLAYER_VISIBLE_ITEM_1_CREATOR + 0 + offset);
		Player::m_visibleUpdateMask.SetBit(PLAYER_VISIBLE_ITEM_1_CREATOR + 1 + offset);
		Player::m_visibleUpdateMask.SetBit(PLAYER_VISIBLE_ITEM_1_0 + 0 + offset);

		// enchants
		for(uint8 j = 0; j < 12; ++j)
		{
			Player::m_visibleUpdateMask.SetBit(PLAYER_VISIBLE_ITEM_1_0 + 1 + j + offset);
		}

		Player::m_visibleUpdateMask.SetBit(PLAYER_VISIBLE_ITEM_1_PROPERTIES + offset);
		Player::m_visibleUpdateMask.SetBit(PLAYER_VISIBLE_ITEM_1_SEED + offset);
		Player::m_visibleUpdateMask.SetBit(PLAYER_VISIBLE_ITEM_1_PAD + offset);
     }

	Player::m_visibleUpdateMask.SetBit(PLAYER_CHOSEN_TITLE);
}


void Player::DestroyForPlayer( Player *target ) const
{
	Unit::DestroyForPlayer( target );
}

#define IS_ARENA(x) ( (x) >= BATTLEGROUND_ARENA_2V2 && (x) <= BATTLEGROUND_ARENA_5V5 )

void Player::SaveToDB(bool bNewCharacter /* =false */)
{
	bool in_arena = false;
	QueryBuffer * buf = NULL;
	if(!bNewCharacter)
		buf = new QueryBuffer;

	if( m_bg != NULL && IS_ARENA( m_bg->GetType() ) )
		in_arena = true;

	if( m_uint32Values[PLAYER_CHARACTER_POINTS2] > sWorld.MaxProfs )
		m_uint32Values[PLAYER_CHARACTER_POINTS2] = sWorld.MaxProfs;

	//Calc played times
	uint32 playedt = (uint32)UNIXTIME - m_playedtime[2];
	m_playedtime[0] += playedt;
	m_playedtime[1] += playedt;
	m_playedtime[2] += playedt;

	// active cheats
	uint32 active_cheats = 0;
	if( CooldownCheat )
		active_cheats |= 0x01;
	if( CastTimeCheat )
		active_cheats |= 0x02;
	if( GodModeCheat )
		active_cheats |= 0x04;
	if( PowerCheat )
		active_cheats |= 0x08;
	if( FlyCheat )
		active_cheats |= 0x10;
	if( AuraStackCheat )
		active_cheats |= 0x20;
	if( ItemStackCheat )
		active_cheats |= 0x40;
	if( TriggerpassCheat )
		active_cheats |= 0x80;

	std::stringstream ss;
	ss << "REPLACE INTO characters VALUES ("

	<< GetLowGUID() << ", "
	<< GetSession()->GetAccountId() << ","

	// stat saving
	<< "'" << m_name << "', "
	<< uint32(getRace()) << ","
	<< uint32(getClass()) << ","
	<< uint32(getGender()) << ",";

	if(m_uint32Values[UNIT_FIELD_FACTIONTEMPLATE] != info->factiontemplate)
		ss << m_uint32Values[UNIT_FIELD_FACTIONTEMPLATE] << ",";
	else
		ss << "0,";

	ss << uint32(getLevel()) << ","
	<< m_uint32Values[PLAYER_XP] << ","
	<< active_cheats << ","

	// dump exploration data
	<< "'";

	for(uint32 i = 0; i < PLAYER_EXPLORED_ZONES_LENGTH; ++i)
		ss << m_uint32Values[PLAYER_EXPLORED_ZONES_1 + i] << ",";

	ss << "','";

	// dump skill data
	/*for(uint32 i=PLAYER_SKILL_INFO_1_1;i<PLAYER_CHARACTER_POINTS1;i+=3)
	{
		if(m_uint32Values[i])
		{
			ss << m_uint32Values[i] << ","
			  << m_uint32Values[i+1]<< ",";
		}
	}*/

	/*for(uint32 i = PLAYER_SKILL_INFO_1_1; i < PLAYER_CHARACTER_POINTS1; ++i)
		ss << m_uint32Values[i] << " ";
	*/

	for(SkillMap::iterator itr = m_skills.begin(); itr != m_skills.end(); ++itr)
	{
		if(itr->first && itr->second.Skill->type != SKILL_TYPE_LANGUAGE)
		{
			ss << itr->first << ";"
				<< itr->second.CurrentValue << ";"
				<< itr->second.MaximumValue << ";";
		}
	}

	uint32 player_flags = m_uint32Values[PLAYER_FLAGS];
	{
		// Remove un-needed and problematic player flags from being saved :p
		if(player_flags & PLAYER_FLAG_PARTY_LEADER)
			player_flags &= ~PLAYER_FLAG_PARTY_LEADER;
		if(player_flags & PLAYER_FLAG_AFK)
			player_flags &= ~PLAYER_FLAG_AFK;
		if(player_flags & PLAYER_FLAG_DND)
			player_flags &= ~PLAYER_FLAG_DND;
		if(player_flags & PLAYER_FLAG_GM)
			player_flags &= ~PLAYER_FLAG_GM;
		if(player_flags & PLAYER_FLAG_PVP_TOGGLE)
			player_flags &= ~PLAYER_FLAG_PVP_TOGGLE;
		if(player_flags & PLAYER_FLAG_FREE_FOR_ALL_PVP)
			player_flags &= ~PLAYER_FLAG_FREE_FOR_ALL_PVP;
	}

	ss << "', "
	<< m_uint32Values[PLAYER_FIELD_WATCHED_FACTION_INDEX] << ","
	<< m_uint32Values[PLAYER_CHOSEN_TITLE]<< ","
	<< GetUInt64Value(PLAYER_FIELD_KNOWN_TITLES) << ","
	<< GetUInt64Value(PLAYER_FIELD_KNOWN_TITLES1) << ","
	<< m_uint32Values[PLAYER_FIELD_COINAGE] << ",";

	if((getClass()==MAGE) || (getClass()==PRIEST) || (getClass()==WARLOCK))
		ss << (uint32)0 << ","; // make sure ammo slot is 0 for these classes, otherwise it can mess up wand shoot
	else
		ss << m_uint32Values[PLAYER_AMMO_ID] << ",";
	ss << m_uint32Values[PLAYER_CHARACTER_POINTS2] << ",";

	if (m_uint32Values[PLAYER_CHARACTER_POINTS1] > 71 &&  ! GetSession()->HasGMPermissions())
            SetUInt32Value(PLAYER_CHARACTER_POINTS1, 71);
	ss << m_uint32Values[PLAYER_CHARACTER_POINTS1] << ","
	<< load_health << ","
	<< load_mana << ","
	<< uint32(GetPVPRank()) << ","
	<< m_uint32Values[PLAYER_BYTES] << ","
	<< m_uint32Values[PLAYER_BYTES_2] << ","
	<< player_flags << ","
	<< m_uint32Values[PLAYER_FIELD_BYTES] << ",";

	if( in_arena )
	{
		// if its an arena, save the entry coords instead
		ss << m_bgEntryPointX << ", ";
		ss << m_bgEntryPointY << ", ";
		ss << m_bgEntryPointZ << ", ";
		ss << m_bgEntryPointO << ", ";
		ss << m_bgEntryPointMap << ", ";
	}
	else
	{
		// save the normal position
		ss << m_position.x << ", "
			<< m_position.y << ", "
			<< m_position.z << ", "
			<< m_position.o << ", "
			<< m_mapId << ", ";
	}

	ss << m_zoneId << ", '";

	for(uint32 i = 0; i < 12; i++ )
		ss << m_taximask[i] << " ";
	ss << "', "

	<< m_banned << ", '"
	<< CharacterDatabase.EscapeString(m_banreason) << "', "
	<< (uint32)UNIXTIME << ",";

	//online state
	if(GetSession()->_loggingOut || bNewCharacter)
	{
		ss << "0,";
	}else
	{
		ss << "1,";
	}

	ss
	<< m_bind_pos_x			 << ", "
	<< m_bind_pos_y			 << ", "
	<< m_bind_pos_z			 << ", "
	<< m_bind_mapid			 << ", "
	<< m_bind_zoneid			<< ", "

	<< uint32(m_isResting)	  << ", "
	<< uint32(m_restState)	  << ", "
	<< uint32(m_restAmount)	 << ", '"

	<< uint32(m_playedtime[0])  << " "
	<< uint32(m_playedtime[1])  << " "
	<< uint32(playedt)		  << " ', "
	<< uint32(m_deathState)	 << ", "

	<< m_talentresettimes	   << ", "
	<< m_FirstLogin			 << ", "
	<< rename_pending
	<< "," << m_arenaPoints << ","
	<< (uint32)m_StableSlotCount << ",";

	// instances
	if( in_arena )
	{
		ss << m_bgEntryPointInstance << ", ";
	}
	else
	{
		ss << m_instanceId		   << ", ";
	}

	ss << m_bgEntryPointMap	  << ", "
	<< m_bgEntryPointX		<< ", "
	<< m_bgEntryPointY		<< ", "
	<< m_bgEntryPointZ		<< ", "
	<< m_bgEntryPointO		<< ", "
	<< m_bgEntryPointInstance << ", ";

	// taxi
	if(m_onTaxi&&m_CurrentTaxiPath) {
		ss << m_CurrentTaxiPath->GetID() << ", ";
		ss << lastNode << ", ";
		ss << GetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID);
	} else {
		ss << "0, 0, 0";
	}

	ss << "," << (m_CurrentTransporter ? m_CurrentTransporter->GetEntry() : (uint32)0);
	ss << ",'" << m_TransporterX << "','" << m_TransporterY << "','" << m_TransporterZ << "'";
	ss << ",'";

	// Dump spell data to stringstream
	SpellSet::iterator spellItr = mSpells.begin();
	for(; spellItr != mSpells.end(); ++spellItr)
	{
		ss << uint32(*spellItr) << ",";
	}
	ss << "','";
	// Dump deleted spell data to stringstream
	spellItr = mDeletedSpells.begin();
	for(; spellItr != mDeletedSpells.end(); ++spellItr)
	{
		ss << uint32(*spellItr) << ",";
	}

	ss << "','";
	// Dump reputation data
	ReputationMap::iterator iter = m_reputation.begin();
	for(; iter != m_reputation.end(); ++iter)
	{
		ss << int32(iter->first) << "," << int32(iter->second->flag) << "," << int32(iter->second->baseStanding) << "," << int32(iter->second->standing) << ",";
	}
	ss << "','";

	// Add player action bars
	for(uint32 i = 0; i < 120; ++i)
	{
		ss << uint32(mActions[i].Action) << ","
			<< uint32(mActions[i].Misc) << ","
			<< uint32(mActions[i].Type) << ",";
	}
	ss << "','";

	if(!bNewCharacter)
		SaveAuras(ss);

//	ss << LoadAuras;
	ss << "','";

	// Add player finished quests
	set<uint32>::iterator fq = m_finishedQuests.begin();
	for(; fq != m_finishedQuests.end(); ++fq)
	{
		ss << (*fq) << ",";
	}

	ss << "', '";
	DailyMutex.Acquire();
	set<uint32>::iterator fdq = m_finishedDailies.begin();
	for(; fdq != m_finishedDailies.end(); fdq++)
	{
		ss << (*fdq) << ",";
	}
	DailyMutex.Release();
	ss << "', ";
	ss << m_honorRolloverTime << ", ";
	ss << m_killsToday << ", " << m_killsYesterday << ", " << m_killsLifetime << ", ";
	ss << m_honorToday << ", " << m_honorYesterday << ", ";
	ss << m_honorPoints << ", ";
    ss << iInstanceType << ", ";

	// dump glyphs
	ss << "'";

	for(uint32 i = 0; i < 8; ++i)
		ss << m_uint32Values[PLAYER_FIELD_GLYPHS_1 + i] << ",";

	ss << "')";


	if(bNewCharacter)
		CharacterDatabase.WaitExecuteNA(ss.str().c_str());
	else
		buf->AddQueryStr(ss.str());

	//Save Other related player stuff

	// Inventory
	 GetItemInterface()->mSaveItemsToDatabase(bNewCharacter, buf);

	// save quest progress
	_SaveQuestLogEntry(buf);

	// Tutorials
	_SaveTutorials(buf);

	// GM Ticket
	//TODO: Is this really necessary? Tickets will allways be saved on creation, update and so on...
	GM_Ticket* ticket = objmgr.GetGMTicketByPlayer(GetGUID());
	if(ticket != NULL)
		objmgr.SaveGMTicket(ticket, buf);

	// Cooldown Items
	_SavePlayerCooldowns( buf );

	// Pets
	if(getClass() == HUNTER || getClass() == WARLOCK)
	{
		_SavePet(buf);
		_SavePetSpells(buf);
	}
	m_nextSave = getMSTime() + sWorld.getIntRate(INTRATE_SAVE);

	if(buf)
		CharacterDatabase.AddQueryBuffer(buf);

	m_achievementMgr.SaveToDB();
}

void Player::_SaveQuestLogEntry(QueryBuffer * buf)
{
	for(std::set<uint32>::iterator itr = m_removequests.begin(); itr != m_removequests.end(); ++itr)
	{
		if(buf == NULL)
			CharacterDatabase.Execute("DELETE FROM questlog WHERE player_guid=%u AND quest_id=%u", GetLowGUID(), (*itr));
		else
			buf->AddQuery("DELETE FROM questlog WHERE player_guid=%u AND quest_id=%u", GetLowGUID(), (*itr));
	}

	m_removequests.clear();

	for(int i = 0; i < 25; ++i)
	{
		if(m_questlog[i] != NULL)
			m_questlog[i]->SaveToDB(buf);
	}
}

bool Player::canCast(SpellEntry *m_spellInfo)
{
	if (m_spellInfo->EquippedItemClass != 0)
	{
		if(this->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND))
		{
			if((int32)this->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND)->GetProto()->Class == m_spellInfo->EquippedItemClass)
			{
				if (m_spellInfo->EquippedItemSubClass != 0)
				{
					if (m_spellInfo->EquippedItemSubClass != 173555 && m_spellInfo->EquippedItemSubClass != 96 && m_spellInfo->EquippedItemSubClass != 262156)
					{
						if (pow(2.0,(this->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND)->GetProto()->SubClass) != m_spellInfo->EquippedItemSubClass))
							return false;
					}
				}
			}
		}
		else if(m_spellInfo->EquippedItemSubClass == 173555)
			return false;

		if (this->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED))
		{
			if((int32)this->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED)->GetProto()->Class == m_spellInfo->EquippedItemClass)
			{
				if (m_spellInfo->EquippedItemSubClass != 0)
				{
					if (m_spellInfo->EquippedItemSubClass != 173555 && m_spellInfo->EquippedItemSubClass != 96 && m_spellInfo->EquippedItemSubClass != 262156)
					{
						if (pow(2.0,(this->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED)->GetProto()->SubClass) != m_spellInfo->EquippedItemSubClass))							return false;
					}
				}
			}
		}
		else if
			(m_spellInfo->EquippedItemSubClass == 262156)
			return false;
	}
	return true;
}

void Player::RemovePendingPlayer()
{
	if(m_session)
	{
		uint8 respons = 0x42;		// CHAR_LOGIN_NO_CHARACTER
		m_session->OutPacket(SMSG_CHARACTER_LOGIN_FAILED, 1, &respons);
		m_session->m_loggingInPlayer = NULL;
	}

	ok_to_remove = true;
	delete this;
}

bool Player::LoadFromDB(uint32 guid)
{
	AsyncQuery * q = new AsyncQuery( new SQLClassCallbackP0<Player>(this, &Player::LoadFromDBProc) );
	q->AddQuery("SELECT * FROM characters WHERE guid=%u AND forced_rename_pending = 0",guid);
	q->AddQuery("SELECT * FROM tutorials WHERE playerId=%u",guid);
	q->AddQuery("SELECT cooldown_type, cooldown_misc, cooldown_expire_time, cooldown_spellid, cooldown_itemid FROM playercooldowns WHERE player_guid=%u", guid);
	q->AddQuery("SELECT * FROM questlog WHERE player_guid=%u",guid);
	q->AddQuery("SELECT * FROM playeritems WHERE ownerguid=%u ORDER BY containerslot ASC", guid);
	q->AddQuery("SELECT * FROM playerpets WHERE ownerguid=%u ORDER BY petnumber", guid);
	q->AddQuery("SELECT * FROM playersummonspells where ownerguid=%u ORDER BY entryid", guid);
	q->AddQuery("SELECT * FROM mailbox WHERE player_guid = %u", guid);

	// social
	q->AddQuery("SELECT friend_guid, note FROM social_friends WHERE character_guid = %u", guid);
	q->AddQuery("SELECT character_guid FROM social_friends WHERE friend_guid = %u", guid);
	q->AddQuery("SELECT ignore_guid FROM social_ignores WHERE character_guid = %u", guid);

	// queue it!
	m_uint32Values[OBJECT_FIELD_GUID] = guid;
	CharacterDatabase.QueueAsyncQuery(q);
	return true;

   	//CharacterDatabase.Query("SELECT achievement, date FROM character_achievement WHERE guid = '%u'", GetUInt32Value(OBJECT_FIELD_GUID))
}

void Player::LoadFromDBProc(QueryResultVector & results)
{
	uint32 field_index = 2;
#define get_next_field fields[field_index++]

	if(GetSession() == NULL || results.size() < 8)		// should have 8 queryresults for aplayer load.
	{
		RemovePendingPlayer();
		return;
	}

	QueryResult *result = results[0].result;
	if(!result)
	{
		Log.Error ("Player::LoadFromDB",
				"Player login query failed! guid = %u",
				GetLowGUID ());
		RemovePendingPlayer();
		return;
	}

	if (result->GetFieldCount() != 83)
	{
		Log.Error ("Player::LoadFromDB",
				"Expected 83 fields from the database, "
				"but received %u!  You may need to update your character database.",
				(unsigned int) result->GetFieldCount ());
		RemovePendingPlayer();
		return;
	}

	Field *fields = result->Fetch();

	if(fields[1].GetUInt32() != m_session->GetAccountId())
	{
		sCheatLog.writefromsession(m_session, "player tried to load character not belonging to them (guid %u, on account %u)",
			fields[0].GetUInt32(), fields[1].GetUInt32());
		RemovePendingPlayer();
		return;
	}

	uint32 banned = fields[34].GetUInt32();
	if(banned && (banned < 100 || banned > (uint32)UNIXTIME))
	{
		RemovePendingPlayer();
		return;
	}

	// Load name
	m_name = get_next_field.GetString();

	// Load race/class from fields
	setRace(get_next_field.GetUInt8());
	setClass(get_next_field.GetUInt8());
	setGender(get_next_field.GetUInt8());
	uint32 cfaction = get_next_field.GetUInt32();

	// set race dbc
	myRace = dbcCharRace.LookupEntryForced(getRace());
	myClass = dbcCharClass.LookupEntryForced(getClass());
	if(!myClass || !myRace)
	{
		// bad character
		printf("guid %u failed to login, no race or class dbc found. (race %u class %u)\n", (unsigned int)GetLowGUID(), (unsigned int)getRace(), (unsigned int)getClass());
		RemovePendingPlayer();
		return;
	}

	if(myRace->team_id == 7)
	{
		m_bgTeam = m_team = 0;
	}
	else
	{
		m_bgTeam = m_team = 1;
	}

	SetNoseLevel();

	// set power type
	SetPowerType(myClass->power_type);

	// obtain player create info
	info = objmgr.GetPlayerCreateInfo(getRace(), getClass());
	if(!info)
	{
		sLog.outError("%s: player guid %u has no playerCreateInfo!\n", (unsigned int)GetLowGUID());
		RemovePendingPlayer();
		return;
	}

	// set level
	m_uint32Values[UNIT_FIELD_LEVEL] = get_next_field.GetUInt32();
	/*if(m_uint32Values[UNIT_FIELD_LEVEL] > PLAYER_LEVEL_CAP)
		m_uint32Values[UNIT_FIELD_LEVEL] = PLAYER_LEVEL_CAP;*/

	// obtain level/stats information
	lvlinfo = objmgr.GetLevelInfo(getRace(), getClass(), getLevel());

	if(!lvlinfo)
	{
		printf("guid %u level %u class %u race %u levelinfo not found!\n", (unsigned int)GetLowGUID(), (unsigned int)getLevel(), (unsigned int)getClass(), (unsigned int)getRace());
		RemovePendingPlayer();
		return;
	}

	CalculateBaseStats();

	// set xp
	m_uint32Values[PLAYER_XP] = get_next_field.GetUInt32();

	// Load active cheats
	uint32 active_cheats = get_next_field.GetUInt32();
	if(active_cheats & 0x01)
		CooldownCheat = true;
	if(active_cheats & 0x02)
		CastTimeCheat = true;
	if(active_cheats & 0x04)
		GodModeCheat = true;
	if(active_cheats & 0x08)
		PowerCheat = true;
	if(active_cheats & 0x10)
		FlyCheat = true;
	if(active_cheats & 0x20)
		AuraStackCheat = true;
	if(active_cheats & 0x40)
		ItemStackCheat = true;
	if(active_cheats & 0x80)
		TriggerpassCheat = true;

	// Process exploration data.
	LoadFieldsFromString(get_next_field.GetString(), PLAYER_EXPLORED_ZONES_1, PLAYER_EXPLORED_ZONES_LENGTH);

	// Load achievements - trying to do this asynchronously causes a crash :/
	// This needs to be done before loading spells, skills, etc. so that Realm-First! stuff doesn't get spammed to everybody every time the player loads.
	m_achievementMgr.LoadFromDB(CharacterDatabase.Query("SELECT achievement, date FROM character_achievement WHERE guid = '%u'", GetUInt32Value(OBJECT_FIELD_GUID)),CharacterDatabase.Query("SELECT criteria, counter, date FROM character_achievement_progress WHERE guid = '%u'", GetUInt32Value(OBJECT_FIELD_GUID)));

	// Process skill data.
	uint32 Counter = 0;
	char * start = (char*)get_next_field.GetString();//buff;
	char * end;

	// new format
	const ItemProf * prof;
	if(!strchr(start, ' ') && !strchr(start,';'))
	{
		/* no skills - reset to defaults */
		for(std::list<CreateInfo_SkillStruct>::iterator ss = info->skills.begin(); ss!=info->skills.end(); ss++)
		{
			if(ss->skillid && ss->currentval && ss->maxval && !::GetSpellForLanguage(ss->skillid))
				_AddSkillLine(ss->skillid, ss->currentval, ss->maxval);
		}
	}
	else
	{
		char * f = strdup(start);
		start = f;
		if(!strchr(start,';'))
		{
			/* old skill format.. :< */
			uint32 v1,v2,v3;
			PlayerSkill sk;
			for(;;)
			{
				end = strchr(start, ' ');
				if(!end)
					break;

				*end = 0;
				v1 = atol(start);
				start = end + 1;

				end = strchr(start, ' ');
				if(!end)
					break;

				*end = 0;
				v2 = atol(start);
				start = end + 1;

				end = strchr(start, ' ');
				if(!end)
					break;

				v3 = atol(start);
				start = end + 1;
				if(v1 & 0xffff)
				{
					sk.Reset(v1 & 0xffff);
					sk.CurrentValue = v2 & 0xffff;
					sk.MaximumValue = (v2 >> 16) & 0xffff;

					if( !sk.CurrentValue )
						sk.CurrentValue = 1;

					m_skills.insert( make_pair(sk.Skill->id, sk) );
				}
			}
		}
		else
		{
			uint32 v1,v2,v3;
			PlayerSkill sk;
			for(;;)
			{
				end = strchr(start, ';');
				if(!end)
					break;

				*end = 0;
				v1 = atol(start);
				start = end + 1;

				end = strchr(start, ';');
				if(!end)
					break;

				*end = 0;
				v2 = atol(start);
				start = end + 1;

				end = strchr(start, ';');
				if(!end)
					break;

				v3 = atol(start);
				start = end + 1;

				/* add the skill */
				if(v1)
				{
					sk.Reset(v1);
					sk.CurrentValue = v2;
					sk.MaximumValue = v3;

					if( !sk.CurrentValue )
						sk.CurrentValue = 1;

					m_skills.insert(make_pair(v1, sk));
				}
			}
		}
		free(f);
	}

	for(SkillMap::iterator itr = m_skills.begin(); itr != m_skills.end(); ++itr)
	{
		if(itr->first == SKILL_RIDING)
		{
			itr->second.CurrentValue = itr->second.MaximumValue;
		}

		prof = GetProficiencyBySkill(itr->first);
		if(prof)
		{
			if(prof->itemclass==4)
				armor_proficiency|=prof->subclass;
			else
				weapon_proficiency|=prof->subclass;
		}
	}

	// set the rest of the stuff
	m_uint32Values[ PLAYER_FIELD_WATCHED_FACTION_INDEX ]	= get_next_field.GetUInt32();
	m_uint32Values[ PLAYER_CHOSEN_TITLE ]					= get_next_field.GetUInt32();
	SetUInt64Value( PLAYER_FIELD_KNOWN_TITLES, get_next_field.GetUInt64() );
	SetUInt64Value( PLAYER_FIELD_KNOWN_TITLES1, get_next_field.GetUInt64() );
	m_uint32Values[ PLAYER_FIELD_COINAGE ]					= get_next_field.GetUInt32();
	m_uint32Values[ PLAYER_AMMO_ID ]						= get_next_field.GetUInt32();
	m_uint32Values[ PLAYER_CHARACTER_POINTS2 ]				= get_next_field.GetUInt32();
	m_uint32Values[ PLAYER_CHARACTER_POINTS1 ]				= get_next_field.GetUInt32();
	load_health												= get_next_field.GetUInt32();
	load_mana												= get_next_field.GetUInt32();
	SetUInt32Value( UNIT_FIELD_HEALTH, load_health );
	uint8 pvprank = get_next_field.GetUInt8();
	SetUInt32Value( PLAYER_BYTES, get_next_field.GetUInt32() );
	SetUInt32Value( PLAYER_BYTES_2, get_next_field.GetUInt32() );
	SetUInt32Value( PLAYER_BYTES_3, getGender() | ( pvprank << 24 ) );
	SetUInt32Value( PLAYER_FLAGS, get_next_field.GetUInt32() );
	SetUInt32Value( PLAYER_FIELD_BYTES, get_next_field.GetUInt32() );
	//m_uint32Values[0x22]=(m_uint32Values[0x22]>0x46)?0x46:m_uint32Values[0x22];

	m_position.x										= get_next_field.GetFloat();
	m_position.y										= get_next_field.GetFloat();
	m_position.z										= get_next_field.GetFloat();
	m_position.o										= get_next_field.GetFloat();

	m_mapId												= get_next_field.GetUInt32();
	m_zoneId											= get_next_field.GetUInt32();

	// Calculate the base stats now they're all loaded
	for(uint32 i = 0; i < 5; ++i)
		CalcStat(i);

  //  for(uint32 x = PLAYER_SPELL_CRIT_PERCENTAGE1; x < PLAYER_SPELL_CRIT_PERCENTAGE06 + 1; ++x)
	///	SetFloatValue(x, 0.0f);

	for(uint32 x = PLAYER_FIELD_MOD_DAMAGE_DONE_PCT; x < PLAYER_FIELD_MOD_HEALING_DONE_POS; ++x)
		SetFloatValue(x, 1.0f);

	// Normal processing...
//	UpdateMaxSkills();
	UpdateStats();
	//UpdateChances();

	// Initialize 'normal' fields
	//SetFloatValue(OBJECT_FIELD_SCALE_X, ((getRace()==RACE_TAUREN)?1.3f:1.0f));
	SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
	//SetUInt32Value(UNIT_FIELD_POWER2, 0);
	SetUInt32Value(UNIT_FIELD_POWER3, info->focus);
	SetUInt32Value(UNIT_FIELD_POWER4, info->energy );
	SetUInt32Value(UNIT_FIELD_POWER6, 8);
	SetUInt32Value(UNIT_FIELD_MAXPOWER2, info->rage );
	SetUInt32Value(UNIT_FIELD_MAXPOWER3, info->focus );
	SetUInt32Value(UNIT_FIELD_MAXPOWER4, info->energy );
	SetUInt32Value(UNIT_FIELD_MAXPOWER6, 8);
	SetUInt32Value(UNIT_FIELD_MAXPOWER7, 1000 );
	if(getClass() == WARRIOR)
		SetShapeShift(FORM_BATTLESTANCE);

	SetUInt32Value(UNIT_FIELD_BYTES_2, (0x28 << 8) );
	SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED);
	SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 0.388999998569489f );
	SetFloatValue(UNIT_FIELD_COMBATREACH, 1.5f   );

	if(getRace() != 10)
	{
		SetUInt32Value(UNIT_FIELD_DISPLAYID, info->displayId + getGender() );
		SetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID, info->displayId + getGender() );
	} else	{
		SetUInt32Value(UNIT_FIELD_DISPLAYID, info->displayId - getGender() );
		SetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID, info->displayId - getGender() );
	}
	EventModelChange();

	SetFloatValue(UNIT_MOD_CAST_SPEED, 1.0f);
	SetUInt32Value(PLAYER_FIELD_MAX_LEVEL, sWorld.m_levelCap);
	SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, info->factiontemplate);
	if(cfaction)
	{
		SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, info->factiontemplate);
		// re-calculate team
		switch(cfaction)
		{
		case 1:	// human
		case 3:	// dwarf
		case 4: // ne
		case 8:	// gnome
		case 927:	// dreinei
			m_team = m_bgTeam = 0;
			break;

		default:
			m_team = m_bgTeam = 1;
			break;
		}
	}

	LoadTaxiMask( get_next_field.GetString() );

	m_banned = get_next_field.GetUInt32(); //Character ban
	m_banreason = get_next_field.GetString();
	m_timeLogoff = get_next_field.GetUInt32();
	field_index++;

	m_bind_pos_x = get_next_field.GetFloat();
	m_bind_pos_y = get_next_field.GetFloat();
	m_bind_pos_z = get_next_field.GetFloat();
	m_bind_mapid = get_next_field.GetUInt32();
	m_bind_zoneid = get_next_field.GetUInt32();

	m_isResting = get_next_field.GetUInt8();
	m_restState = get_next_field.GetUInt8();
	m_restAmount = get_next_field.GetUInt32();


	std::string tmpStr = get_next_field.GetString();
	m_playedtime[0] = (uint32)atoi((const char*)strtok((char*)tmpStr.c_str()," "));
	m_playedtime[1] = (uint32)atoi((const char*)strtok(NULL," "));

	m_deathState = (DeathState)get_next_field.GetUInt32();
	m_talentresettimes = get_next_field.GetUInt32();
	m_FirstLogin = get_next_field.GetBool();
	rename_pending = get_next_field.GetBool();
	m_arenaPoints = get_next_field.GetUInt32();
	if (m_arenaPoints > 5000) m_arenaPoints = 5000;
	for(uint32 z = 0; z < NUM_CHARTER_TYPES; ++z)
		m_charters[z] = objmgr.GetCharterByGuid(GetGUID(), (CharterTypes)z);
	for(uint32 z = 0; z < NUM_ARENA_TEAM_TYPES; ++z)
	{
		m_arenaTeams[z] = objmgr.GetArenaTeamByGuid(GetLowGUID(), z);
		if(m_arenaTeams[z] != NULL)
		{
			SetUInt32Value(PLAYER_FIELD_ARENA_TEAM_INFO_1_1 + (z*6), m_arenaTeams[z]->m_id);
			if(m_arenaTeams[z]->m_leader == GetLowGUID())
				SetUInt32Value(PLAYER_FIELD_ARENA_TEAM_INFO_1_1 + (z*6) + 1, 0);
			else
				SetUInt32Value(PLAYER_FIELD_ARENA_TEAM_INFO_1_1 + (z*6) + 1, 1);
		}
	}

	m_StableSlotCount = get_next_field.GetUInt32();
	m_instanceId = get_next_field.GetUInt32();
	m_bgEntryPointMap = get_next_field.GetUInt32();
	m_bgEntryPointX = get_next_field.GetFloat();
	m_bgEntryPointY = get_next_field.GetFloat();
	m_bgEntryPointZ = get_next_field.GetFloat();
	m_bgEntryPointO = get_next_field.GetFloat();
	m_bgEntryPointInstance = get_next_field.GetUInt32();

	uint32 taxipath = get_next_field.GetUInt32();
	TaxiPath *path = NULL;
	if(taxipath)
	{
		path = sTaxiMgr.GetTaxiPath(taxipath);
		lastNode = get_next_field.GetUInt32();
		if(path)
		{
			SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID, get_next_field.GetUInt32());
			SetTaxiPath(path);
			m_onTaxi = true;
		}
		else
			field_index++;
	}
	else
	{
		field_index++;
		field_index++;
	}

	m_TransporterGUID = get_next_field.GetUInt32();
	if(m_TransporterGUID)
	{
		Transporter * t = objmgr.GetTransporter(GUID_LOPART(m_TransporterGUID));
		m_TransporterGUID = t ? t->GetGUID() : 0;
	}

	m_TransporterX = get_next_field.GetFloat();
	m_TransporterY = get_next_field.GetFloat();
	m_TransporterZ = get_next_field.GetFloat();

	// Load Spells from CSV data.
	start = (char*)get_next_field.GetString();//buff;
	SpellEntry * spProto;
	while(true)
	{
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		//mSpells.insert(atol(start));
		spProto = dbcSpell.LookupEntryForced(atol(start));
//#define _language_fix_ 1
#ifndef _language_fix_
		if(spProto)
			mSpells.insert(spProto->Id);
#else
		if (spProto)
		{
			skilllinespell * _spell = objmgr.GetSpellSkill(spProto->Id);
			if (_spell)
			{
				skilllineentry * _skill = dbcSkillLine.LookupEntry(_spell->skilline);
				if (_skill && _skill->type != SKILL_TYPE_LANGUAGE)
				{
					mSpells.insert(spProto->Id);
				}
			}
			else
			{
				mSpells.insert(spProto->Id);
			}
		}
#endif
//#undef _language_fix_

		start = end +1;
	}

	start = (char*)get_next_field.GetString();//buff;
	while(true)
	{
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		spProto = dbcSpell.LookupEntryForced(atol(start));
		if(spProto)
			mDeletedSpells.insert(spProto->Id);
		start = end +1;
	}

	// Load Reputatation CSV Data
	start =(char*) get_next_field.GetString();
	FactionDBC * factdbc ;
	FactionReputation * rep;
	uint32 id;
	int32 basestanding;
	int32 standing;
	uint32 fflag;
	while(true)
	{
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		id = atol(start);
		start = end +1;

		end = strchr(start,',');
		if(!end)break;
		*end=0;
		fflag = atol(start);
		start = end +1;

		end = strchr(start,',');
		if(!end)break;
		*end=0;
		basestanding = atoi(start);//atol(start);
		start = end +1;

		end = strchr(start,',');
		if(!end)break;
		*end=0;
		standing  = atoi(start);// atol(start);
		start = end +1;

		// listid stuff
		factdbc = dbcFaction.LookupEntryForced(id);
		if ( factdbc == NULL || factdbc->RepListId < 0 ) continue;
		ReputationMap::iterator rtr = m_reputation.find(id);
		if(rtr != m_reputation.end())
			delete rtr->second;

		rep = new FactionReputation;
		rep->baseStanding = basestanding;
		rep->standing = standing;
		rep->flag = fflag;
		m_reputation[id]=rep;
		reputationByListId[factdbc->RepListId] = rep;
	}

	if(!m_reputation.size())
		_InitialReputation();

	// Load saved actionbars
	start =  (char*)get_next_field.GetString();
	Counter =0;
	while(Counter < 120)
	{
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		mActions[Counter].Action = (uint16)atol(start);
		start = end +1;
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		mActions[Counter].Misc = (uint8)atol(start);
		start = end +1;
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		mActions[Counter++].Type = (uint8)atol(start);
		start = end +1;
	}

	//LoadAuras = get_next_field.GetString();
	start = (char*)get_next_field.GetString();//buff;
	do
	{
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		LoginAura la;
		la.id = atol(start);
		start = end +1;
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		la.dur = atol(start);
		start = end +1;
		end = strchr(start,',');
                if(!end)break;
                *end=0;
                la.positive = (start!=NULL);
                start = end +1;
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		la.charges = atol(start);
		start = end +1;
		loginauras.push_back(la);
	} while(true);

	// Load saved finished quests

	start =  (char*)get_next_field.GetString();
	while(true)
	{
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		m_finishedQuests.insert(atol(start));
		start = end +1;
	}

	start = (char*)get_next_field.GetString();
	while(true)
	{
		end = strchr(start,',');
		if(!end) break;
		*end = 0;
		m_finishedDailies.insert(atol(start));
		start = end +1;
	}

	m_honorRolloverTime = get_next_field.GetUInt32();
	m_killsToday = get_next_field.GetUInt32();
	m_killsYesterday = get_next_field.GetUInt32();
	m_killsLifetime = get_next_field.GetUInt32();

	m_honorToday = get_next_field.GetUInt32();
	m_honorYesterday = get_next_field.GetUInt32();
	m_honorPoints = get_next_field.GetUInt32();
	if (m_honorPoints > 75000) m_honorPoints = 75000;

	RolloverHonor();
    iInstanceType = get_next_field.GetUInt32();

	// Load Glyphs and apply their auras
	LoadFieldsFromString(get_next_field.GetString(), PLAYER_FIELD_GLYPHS_1, 8);
	GlyphPropertyEntry *glyph;
	for(uint32 i=0; i < 8; i++)
	{
		uint32 glyphId = GetUInt32Value(PLAYER_FIELD_GLYPHS_1 + i);
		if(glyphId == 0)
			continue;
		// Get info
		glyph = dbcGlyphProperty.LookupEntry(glyphId);
		if(!glyph || !glyph->SpellID)
			continue;
		LoginAura la;
		la.id = glyph->SpellID;
		la.dur = uint32(-1);
		loginauras.push_back(la);
	}

	HonorHandler::RecalculateHonorFields(this);

	for(uint32 x=0;x<5;x++)
		BaseStats[x]=GetUInt32Value(UNIT_FIELD_STAT0+x);

	_setFaction();

	UpdateGlyphs();

	//class fixes
	switch(getClass())
	{
	case PALADIN:
		armor_proficiency |= ( 1 << 7 );//LIBRAM
		break;
	case DRUID:
		armor_proficiency |= ( 1 << 8 );//IDOL
		break;
	case SHAMAN:
		armor_proficiency |= ( 1 << 9 );//TOTEM
		break;
	case DEATHKNIGHT:
		armor_proficiency |= ( 1 << 10 );//SIGIL
		break;
	case WARLOCK:
	case HUNTER:
		_LoadPet(results[5].result);
        _LoadPetSpells(results[6].result);
	break;
	}

	if(m_session->CanUseCommand('c'))
		_AddLanguages(true);
	else
		_AddLanguages(false);

	OnlineTime	= (uint32)UNIXTIME;
	if(GetGuildId())
		SetUInt32Value(PLAYER_GUILD_TIMESTAMP, (uint32)UNIXTIME);

#undef get_next_field

	// load properties
	_LoadTutorials(results[1].result);
	_LoadPlayerCooldowns(results[2].result);
	_LoadQuestLogEntry(results[3].result);
	m_ItemInterface->mLoadItemsFromDatabase(results[4].result);
	m_mailBox.Load(results[7].result);

	// SOCIAL
	if( results[8].result != NULL )			// this query is "who are our friends?"
	{
		result = results[8].result;
		do
		{
			fields = result->Fetch();
			if( strlen( fields[1].GetString() ) )
				m_friends.insert( make_pair( fields[0].GetUInt32(), strdup(fields[1].GetString()) ) );
			else
				m_friends.insert( make_pair( fields[0].GetUInt32(), (char*)NULL) );

		} while (result->NextRow());
	}

	if( results[9].result != NULL )			// this query is "who has us in their friends?"
	{
		result = results[9].result;
		do
		{
			m_hasFriendList.insert( result->Fetch()[0].GetUInt32() );
		} while (result->NextRow());
	}

	if( results[10].result != NULL )		// this query is "who are we ignoring"
	{
		result = results[10].result;
		do
		{
			m_ignores.insert( result->Fetch()[0].GetUInt32() );
		} while (result->NextRow());
	}

	// END SOCIAL

	// Check skills that player shouldn't have
	if (_HasSkillLine(SKILL_DUAL_WIELD) && !HasSpell(674)) {
		_RemoveSkillLine(SKILL_DUAL_WIELD);
	}

	m_session->FullLogin(this);
	m_session->m_loggingInPlayer = NULL;

	if( !isAlive() )
		myCorpse = objmgr.GetCorpseByOwner(GetLowGUID());

	// check for multiple gems with unique-equipped flag
	uint32 count;
	uint32 uniques[64];
	int nuniques=0;

	for( uint8 x = EQUIPMENT_SLOT_START; x < EQUIPMENT_SLOT_END; ++x )
	{
		ItemInterface *itemi = GetItemInterface();
		Item * it = itemi->GetInventoryItem(x);

		if( it != NULL )
		{
			for( count=0; count<it->GetSocketsCount(); count++ )
			{
				EnchantmentInstance *ei = it->GetEnchantment(2+count);

				if (ei && ei->Enchantment)
				{
					ItemPrototype * ip = ItemPrototypeStorage.LookupEntry(ei->Enchantment->GemEntry);

					if (ip && ip->Flags & ITEM_FLAG_UNIQUE_EQUIP &&
						itemi->IsEquipped(ip->ItemId))
					{
						int i;

						for (i=0; i<nuniques; i++)
						{
							if (uniques[i] == ip->ItemId)
							{
								// found a duplicate unique-equipped gem, remove it
								it->RemoveEnchantment(2+count);
								break;
							}
						}

						if (i == nuniques) // not found
							uniques[nuniques++] = ip->ItemId;
					}
				}
			}
		}
	}
	// Update achievements
	m_achievementMgr.CheckAllAchievementCriteria();
}

void Player::SetPersistentInstanceId(Instance *pInstance)
{
	if(pInstance == NULL)
		return;
	// Skip this handling for flagged GMs.
	if(bGMTagOn)
		return;
	// Bind instance to "my" group.
	if(m_playerInfo && m_playerInfo->m_Group && pInstance->m_creatorGroup == 0)
		pInstance->m_creatorGroup = m_playerInfo && m_playerInfo->m_Group->GetID();
	// Skip handling for non-persistent instances.
	if(!IS_PERSISTENT_INSTANCE(pInstance))
		return;
	// Set instance for group if not done yet.
	if(m_playerInfo && m_playerInfo->m_Group && (m_playerInfo->m_Group->m_instanceIds[pInstance->m_mapId][pInstance->m_difficulty] == 0 || !sInstanceMgr.InstanceExists(pInstance->m_mapId, m_playerInfo->m_Group->m_instanceIds[pInstance->m_mapId][pInstance->m_difficulty])))
	{
		m_playerInfo->m_Group->m_instanceIds[pInstance->m_mapId][pInstance->m_difficulty] = pInstance->m_instanceId;
		m_playerInfo->m_Group->SaveToDB();
	}
	// Instance is not saved yet (no bosskill)
	if(!pInstance->m_persistent)
	{
		SetPersistentInstanceId(pInstance->m_mapId, pInstance->m_difficulty, 0);
	}
	// Set instance id to player.
	else
	{
		SetPersistentInstanceId(pInstance->m_mapId, pInstance->m_difficulty, pInstance->m_instanceId);
	}
	sLog.outDebug("Added player %u to saved instance %u on map %u.", (uint32)GetGUID(), pInstance->m_instanceId, pInstance->m_mapId);
}

void Player::SetPersistentInstanceId(uint32 mapId, uint32 difficulty, uint32 instanceId)
{
	if(mapId >= NUM_MAPS || difficulty >= NUM_INSTANCE_MODES || m_playerInfo == NULL)
		return;
	m_playerInfo->savedInstanceIdsLock.Acquire();
	PlayerInstanceMap::iterator itr = m_playerInfo->savedInstanceIds[difficulty].find(mapId);
	if(itr == m_playerInfo->savedInstanceIds[difficulty].end())
	{
		if(instanceId != 0)
			m_playerInfo->savedInstanceIds[difficulty].insert(PlayerInstanceMap::value_type(mapId, instanceId));
	}
	else
	{
		if(instanceId == 0)
			m_playerInfo->savedInstanceIds[difficulty].erase(itr);
		else
			(*itr).second = instanceId;
	}
	m_playerInfo->savedInstanceIdsLock.Release();
	CharacterDatabase.Execute("REPLACE INTO `instanceids` (`playerguid`, `mapid`, `mode`, `instanceid`) VALUES ( %u, %u, %u, %u )", m_playerInfo->guid, mapId, difficulty, instanceId);
}

void Player::RolloverHonor()
{
	uint32 current_val = (g_localTime.tm_year << 16) | g_localTime.tm_yday;
	if( current_val != m_honorRolloverTime )
	{
		m_honorRolloverTime = current_val;
		m_honorYesterday = m_honorToday;
		m_killsYesterday = m_killsToday;
		m_honorToday = m_killsToday = 0;
	}
}

void Player::_LoadQuestLogEntry(QueryResult * result)
{
	QuestLogEntry *entry;
	Quest *quest;
	Field *fields;
	uint32 questid;
	uint32 baseindex;

	// clear all fields
	for(int i = 0; i < 25; ++i)
	{
		baseindex = PLAYER_QUEST_LOG_1_1 + (i * 4);
		SetUInt32Value(baseindex + 0, 0);
		SetUInt32Value(baseindex + 1, 0);
		SetUInt32Value(baseindex + 2, 0);
		SetUInt32Value(baseindex + 3, 0);
	}

	int slot = 0;

	if(result)
	{
		do
		{
			fields = result->Fetch();
			questid = fields[1].GetUInt32();
			quest = QuestStorage.LookupEntry(questid);
			slot = fields[2].GetUInt32();
			ASSERT(slot != -1);

			// remove on next save if bad quest
			if(!quest)
			{
				m_removequests.insert(questid);
				continue;
			}
			if(m_questlog[slot] != 0)
				continue;

			entry = new QuestLogEntry;
			entry->Init(quest, this, slot);
			entry->LoadFromDB(fields);
			entry->UpdatePlayerFields();

		} while(result->NextRow());
	}
}

QuestLogEntry* Player::GetQuestLogForEntry(uint32 quest)
{
	for(int i = 0; i < 25; ++i)
	{
		if(m_questlog[i] == ((QuestLogEntry*)0x00000001))
			m_questlog[i] = NULL;

		if(m_questlog[i] != NULL)
		{
			if(m_questlog[i]->GetQuest() && m_questlog[i]->GetQuest()->id == quest)
				return m_questlog[i];
		}
	}
	return NULL;
	/*uint32 x = PLAYER_QUEST_LOG_1_1;
	uint32 y = 0;
	for(; x < PLAYER_VISIBLE_ITEM_1_CREATOR && y < 25; x += 3, y++)
	{
		if(m_uint32Values[x] == quest)
			return m_questlog[y];
	}
	return NULL;*/
}

void Player::SetQuestLogSlot(QuestLogEntry *entry, uint32 slot)
{
	m_questlog[slot] = entry;
}

void Player::AddToWorld()
{
	FlyCheat = false;
	m_setflycheat=false;
	// check transporter
	if(m_TransporterGUID && m_CurrentTransporter)
	{
		SetPosition(m_CurrentTransporter->GetPositionX() + m_TransporterX,
			m_CurrentTransporter->GetPositionY() + m_TransporterY,
			m_CurrentTransporter->GetPositionZ() + m_TransporterZ,
			GetOrientation(), false);
	}

	// If we join an invalid instance and get booted out, this will prevent our stats from doubling :P
	if(IsInWorld())
		return;

	m_beingPushed = true;
	Object::AddToWorld();

	// Add failed.
	if(m_mapMgr == NULL)
	{
		// eject from instance
		m_beingPushed = false;
		EjectFromInstance();
		return;
	}

	if(m_session)
		m_session->SetInstance(m_mapMgr->GetInstanceID());
}

void Player::AddToWorld(MapMgr * pMapMgr)
{
	FlyCheat = false;
	m_setflycheat=false;
	// check transporter
	if(m_TransporterGUID && m_CurrentTransporter)
	{
		SetPosition(m_CurrentTransporter->GetPositionX() + m_TransporterX,
			m_CurrentTransporter->GetPositionY() + m_TransporterY,
			m_CurrentTransporter->GetPositionZ() + m_TransporterZ,
			GetOrientation(), false);
	}

	// If we join an invalid instance and get booted out, this will prevent our stats from doubling :P
	if(IsInWorld())
		return;

	m_beingPushed = true;
	Object::AddToWorld(pMapMgr);

	// Add failed.
	if(m_mapMgr == NULL)
	{
		// eject from instance
		m_beingPushed = false;
		EjectFromInstance();
		return;
	}

	if(m_session)
		m_session->SetInstance(m_mapMgr->GetInstanceID());
}

void Player::OnPrePushToWorld()
{
	SendInitialLogonPackets();
	m_achievementMgr.SendAllAchievementData(this);
}

void Player::OnPushToWorld()
{
	uint8 class_ = getClass();
	uint8 startlevel = 1;

	// Process create packet
	ProcessPendingUpdates();

	if(m_TeleportState == 2)   // Worldport Ack
		OnWorldPortAck();

	SpeedCheatReset();
	m_beingPushed = false;
	AddItemsToWorld();
	m_lockTransportVariables = false;

	// delay the unlock movement packet
	WorldPacket * data = new WorldPacket(SMSG_TIME_SYNC_REQ, 4);
	*data << uint32(0);
	delayedPackets.add(data);
	sWorld.mInWorldPlayerCount++;

	// Update PVP Situation
	LoginPvPSetup();
	RemoveByteFlag(UNIT_FIELD_BYTES_2, 1, 0x28);

	if ( m_playerInfo->lastOnline + 900 < UNIXTIME ) // did we logged out for more than 15 minutes?
		m_ItemInterface->RemoveAllConjured();

	Unit::OnPushToWorld();

	if(m_FirstLogin)
	{
		if(class_ == DEATHKNIGHT)
			startlevel = max(55,sWorld.StartingLevel);
		else startlevel = sWorld.StartingLevel;

		sHookInterface.OnFirstEnterWorld(this);
		LevelInfo * Info = objmgr.GetLevelInfo(getRace(), getClass(), startlevel);
		ApplyLevelInfo(Info, startlevel);
		m_FirstLogin = false;
	}

	sHookInterface.OnEnterWorld(this);

	if(m_TeleportState == 1)		// First world enter
		CompleteLoading();

	m_TeleportState = 0;

	if(GetTaxiState())
	{
		if( m_taxiMapChangeNode != 0 )
		{
			lastNode = m_taxiMapChangeNode;
		}

		TaxiStart(GetTaxiPath(),
			GetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID),
			lastNode);

		m_taxiMapChangeNode = 0;
	}

	if(flying_aura && ((m_mapId != 530) && (m_mapId != 571 || !HasSpellwithNameHash(SPELL_HASH_COLD_WEATHER_FLYING))))
	// can only fly in outlands or northrend (northrend requires cold weather flying)
	{
		RemoveAura(flying_aura);
		flying_aura = 0;
	}

	/* send weather */
	sWeatherMgr.SendWeather(this);

	SetUInt32Value( UNIT_FIELD_HEALTH, ( load_health > m_uint32Values[UNIT_FIELD_MAXHEALTH] ? m_uint32Values[UNIT_FIELD_MAXHEALTH] : load_health ));
	SetUInt32Value( UNIT_FIELD_POWER1, ( load_mana > m_uint32Values[UNIT_FIELD_MAXPOWER1] ? m_uint32Values[UNIT_FIELD_MAXPOWER1] : load_mana ));

	if( !GetSession()->HasGMPermissions() )
		GetItemInterface()->CheckAreaItems();

#ifdef ENABLE_COMPRESSED_MOVEMENT
	//sEventMgr.AddEvent(this, &Player::EventDumpCompressedMovement, EVENT_PLAYER_FLUSH_MOVEMENT, World::m_movementCompressInterval, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	MovementCompressor->AddPlayer(this);
#endif

	if( m_mapMgr && m_mapMgr->m_battleground != NULL && m_bg != m_mapMgr->m_battleground )
	{
		m_bg = m_mapMgr->m_battleground;
		m_bg->PortPlayer( this, true );
	}

	if( m_bg != NULL )
		m_bg->OnPlayerPushed( this );

	z_axisposition = 0.0f;
	m_changingMaps = false;
}

void Player::RemoveFromWorld()
{
	if(raidgrouponlysent)
		event_RemoveEvents(EVENT_PLAYER_EJECT_FROM_INSTANCE);

	load_health = m_uint32Values[UNIT_FIELD_HEALTH];
	load_mana = m_uint32Values[UNIT_FIELD_POWER1];

	if(m_bg)
	{
		m_bg->RemovePlayer(this, true);
		m_bg = NULL;
	}

	// Cancel trade if it's active.
	Player * pTarget;
	if(mTradeTarget != 0)
	{
		pTarget = GetTradeTarget();
		if(pTarget)
			pTarget->ResetTradeVariables();

		ResetTradeVariables();
	}
	//clear buyback
	GetItemInterface()->EmptyBuyBack();

	for(uint32 x=0;x<4;x++)
	{
		if(m_TotemSlots[x])
			m_TotemSlots[x]->TotemExpire();
	}

	ClearSplinePackets();

	DismissActivePet();
	RemoveFieldSummon();

	if(m_SummonedObject)
	{
		if(m_SummonedObject->GetInstanceID() != GetInstanceID())
		{
			sEventMgr.AddEvent(m_SummonedObject, &Object::Delete, EVENT_GAMEOBJECT_EXPIRE, 100, 1,0);
		}else
		{
			if(m_SummonedObject->GetTypeId() == TYPEID_PLAYER)
			{

			}
			else
			{
				if(m_SummonedObject->IsInWorld())
				{
					m_SummonedObject->RemoveFromWorld(true);
				}
				delete m_SummonedObject;
			}
		}
		m_SummonedObject = NULL;
	}

	if(IsInWorld())
	{
		RemoveItemsFromWorld();
		Unit::RemoveFromWorld(false);
	}

	sWorld.mInWorldPlayerCount--;
#ifdef ENABLE_COMPRESSED_MOVEMENT
	MovementCompressor->RemovePlayer(this);
	m_movementBufferLock.Acquire();
	m_movementBuffer.clear();
	m_movementBufferLock.Release();
	//sEventMgr.RemoveEvents(this, EVENT_PLAYER_FLUSH_MOVEMENT);

#endif

	if(GetTaxiState())
		event_RemoveEvents( EVENT_PLAYER_TAXI_INTERPOLATE );

	m_changingMaps = true;
	m_playerInfo->lastOnline = UNIXTIME; // don't destroy conjured items yet
}

/*
   ScalingStats DBC entry by level
   FIXME: This should not be neccessary, but I don't know how to lookup dbc rows by the 2nd column
*/
uint32 SSVDBCEByLevel[81] = {
1,1,2,3,4,5,6,7,8,9,10,
81,82,83,84,85,136,137,138,139,61,
141,142,143,144,145,146,147,148,149,150,
151,152,153,154,155,156,157,158,159,160,
161,162,163,164,165,166,167,168,169,170,171,
172,173,174,175,176,177,178,179,180,181,182,
183,184,185,186,187,188,189,190,191,192,193,
194,195,196,197,198,199,200
};

// TODO: perhaps item should just have a list of mods, that will simplify code
void Player::_ApplyItemMods(Item* item, int8 slot, bool apply, bool justdrokedown /* = false */, bool skip_stat_apply /* = false  */)
{
	if (slot >= INVENTORY_SLOT_BAG_END)
		return;

	ASSERT( item );
	ItemPrototype* proto = item->GetProto();

	//fast check to skip mod applying if the item doesnt meat the requirements.
	if( !item->IsContainer() && item->GetUInt32Value( ITEM_FIELD_DURABILITY ) == 0 && item->GetUInt32Value( ITEM_FIELD_MAXDURABILITY ) && justdrokedown == false )
	{
		return;
	}

	//check for rnd prop
	item->ApplyRandomProperties( true );

	//Items Set check
	uint32 setid = proto->ItemSet;

	// These season pvp itemsets are interchangeable and each set group has the same
	// bonuses if you have a full set made up of parts from any of the 3 similar sets
	// you will get the highest sets bonus

	// TODO: make a config for server so they can configure which season is active season

	// * Gladiator's Battlegear
	if( setid == 701 || setid == 736 || setid == 567 )
		setid = 736;

	// * Gladiator's Dreadgear
	if( setid == 702 || setid == 734 || setid == 568 )
		setid = 734;

	// * Gladiator's Earthshaker
	if( setid == 703 || setid == 732 || setid == 578 )
		setid= 732;

	// * Gladiator's Felshroud
	if( setid == 704 || setid == 735 || setid == 615 )
		setid = 735;

	// * Gladiator's Investiture
	if( setid == 705 || setid == 728 || setid == 687 )
		setid = 728;

	// * Gladiator's Pursuit
	if( setid == 706 || setid == 723 || setid == 586 )
		setid = 723;

	// * Gladiator's Raiment
	if( setid == 707 || setid == 729 || setid == 581 )
		setid = 729;

	// * Gladiator's Redemption
	if( setid == 708 || setid == 725 || setid == 690 )
		setid = 725;

	// * Gladiator's Refuge
	if( setid == 709 || setid == 720 || setid == 685 )
		setid = 720;

	// * Gladiator's Regalia
	if( setid == 710 || setid == 724 || setid == 579 )
		setid = 724;

	// * Gladiator's Sanctuary
	if( setid == 711 || setid == 721 || setid == 584 )
		setid = 721;

	// * Gladiator's Thunderfist
	if( setid == 712 || setid == 733 || setid == 580 )
		setid = 733;

	// * Gladiator's Vestments
	if( setid == 713 || setid == 730 || setid == 577 )
		setid = 730;

	// * Gladiator's Vindication
	if( setid == 714 || setid == 726 || setid == 583 )
		setid = 726;

	// * Gladiator's Wartide
	if( setid == 715 || setid == 731 || setid == 686 )
		setid = 731;

	// * Gladiator's Wildhide
	if( setid == 716 || setid == 722 || setid == 585 )
		setid = 722;

	// Set
	if( setid != 0 )
	{
		ItemSetEntry* set = dbcItemSet.LookupEntry( setid );
		ASSERT( set );
		ItemSet* Set = NULL;
		std::list<ItemSet>::iterator i;
		for( i = m_itemsets.begin(); i != m_itemsets.end(); i++ )
		{
			if( i->setid == setid )
			{
				Set = &(*i);
				break;
			}
		}

		if( apply )
		{
			if( Set == NULL )
			{
				Set = new ItemSet;
				memset( Set, 0, sizeof( ItemSet ) );
				Set->itemscount = 1;
				Set->setid = setid;
			}
			else
				Set->itemscount++;

			if( !set->RequiredSkillID || ( _GetSkillLineCurrent( set->RequiredSkillID, true ) >= set->RequiredSkillAmt ) )
			{
				for( uint32 x=0;x<8;x++)
				{
					if( Set->itemscount==set->itemscount[x])
					{//cast new spell
						SpellEntry *info = dbcSpell.LookupEntry( set->SpellID[x] );
						Spell * spell = SpellPool.PooledNew();
						if (!spell)
							return;
						spell->Init( this, info, true, NULL );
						SpellCastTargets targets;
						targets.m_unitTarget = this->GetGUID();
						spell->prepare( &targets );
					}
				}
			}
			if( i == m_itemsets.end() )
			{
				m_itemsets.push_back( *Set );
				delete Set;
			}
		}
		else
		{
			if( Set )
			{
				for( uint32 x = 0; x < 8; x++ )
				if( Set->itemscount == set->itemscount[x] )
				{
					this->RemoveAura( set->SpellID[x], GetGUID() );
				}

				if(!(--Set->itemscount))
				m_itemsets.erase(i);
			}
		}
	}

	// Resistances
	//TODO: FIXME: can there be negative resistances from items?
	if( proto->FireRes )
	{
		if( apply )
			FlatResistanceModifierPos[2] += proto->FireRes;
		else
			FlatResistanceModifierPos[2] -= proto->FireRes;
		CalcResistance( 2 );
	}

	if( proto->NatureRes )
	{
		if( apply )
			FlatResistanceModifierPos[3] += proto->NatureRes;
		else
			FlatResistanceModifierPos[3] -= proto->NatureRes;
		CalcResistance( 3 );
	}

	if( proto->FrostRes )
	{
		if( apply )
			FlatResistanceModifierPos[4] += proto->FrostRes;
		else
			FlatResistanceModifierPos[4] -= proto->FrostRes;
		CalcResistance( 4 );
	}

	if( proto->ShadowRes )
	{
		if( apply )
			FlatResistanceModifierPos[5] += proto->ShadowRes;
		else
			FlatResistanceModifierPos[5] -= proto->ShadowRes;
		CalcResistance( 5 );
	}

	if( proto->ArcaneRes )
	{
		if( apply )
			FlatResistanceModifierPos[6] += proto->ArcaneRes;
		else
			FlatResistanceModifierPos[6] -= proto->ArcaneRes;
		CalcResistance( 6 );
	}
	/* Heirloom scaling items */
	if(proto->ScalingStatsEntry != 0){
		int i = 0;
		ScalingStatDistributionEntry *ssdrow = dbcScalingStatDistribution.LookupEntry( proto->ScalingStatsEntry );
		ScalingStatValuesEntry *ssvrow = dbcScalingStatValues.LookupEntry( SSVDBCEByLevel[ getLevel() ] );
		uint32 StatType;
		uint32 StatMod;
		int32 StatMultiplier;
		int32 StatValue;
		int32 col = 0;

		/* Not going to put a check here since unless you put a random id/flag in the tables these should never return NULL */

		/* Calculating the stats correct for our level and applying them */
		for(i = 0; ssdrow->stat[i] != -1; i++){
			StatType = ssdrow->stat[i];
			StatMod  = ssdrow->statmodifier[i];
			col = GetStatScalingStatValueColumn(proto,SCALINGSTATSTAT);
			if(col == -1) continue;
			StatMultiplier = ssvrow->multiplier[col];
			StatValue = StatMod*StatMultiplier/10000;
			ModifyBonuses(StatType,StatValue,apply);
		}

		if((proto->ScalingStatsFlag & 32768) && i < 10){
			StatType = ssdrow->stat[i];
			StatMod  = ssdrow->statmodifier[i];
			col = GetStatScalingStatValueColumn(proto,SCALINGSTATSPELLPOWER);
			if(col != -1){
				StatMultiplier = ssvrow->multiplier[col];
				StatValue = StatMod*StatMultiplier/10000;
				ModifyBonuses(45,StatValue,apply);
			}
		}

		/* Calculating the Armor correct for our level and applying it */
		col = GetStatScalingStatValueColumn(proto,SCALINGSTATARMOR);
		if(col != -1)
		{
			uint32 scaledarmorval = ssvrow->multiplier[ col ];
			if( apply )BaseResistance[0 ]+= scaledarmorval;
			else  BaseResistance[0] -= scaledarmorval;
			CalcResistance( 0 );
		}

		/* Calculating the damages correct for our level and applying it */
		col = GetStatScalingStatValueColumn(proto,SCALINGSTATDAMAGE);
		if(col != -1){
			uint32 scaleddps = ssvrow->multiplier [ col ];
			float dpsmod = 1.0;

			if (proto->ScalingStatsFlag & 0x1400)
				dpsmod = 0.2f;
			else dpsmod = 0.3f;

			float scaledmindmg = (scaleddps - (scaleddps * dpsmod)) * (proto->Delay/1000);
			float scaledmaxdmg = (scaleddps * (dpsmod+1.0f)) * (proto->Delay/1000);

			if( proto->InventoryType == INVTYPE_RANGED || proto->InventoryType == INVTYPE_RANGEDRIGHT || proto->InventoryType == INVTYPE_THROWN )
			{
				BaseRangedDamage[0] += apply ? scaledmindmg : -scaledmindmg;
				BaseRangedDamage[1] += apply ? scaledmaxdmg : -scaledmaxdmg;
			}
			else
			{
				if( slot == EQUIPMENT_SLOT_OFFHAND )
				{
					BaseOffhandDamage[0] = apply ? scaledmindmg : 0;
					BaseOffhandDamage[1] = apply ? scaledmaxdmg : 0;
				}
				else
				{
					BaseDamage[0] = apply ? scaledmindmg : 0;
					BaseDamage[1] = apply ? scaledmaxdmg : 0;
				}
			}
		}

	/* Normal items */
	} else {
		// Stats
		for( uint32 i = 0; i < proto->itemstatscount; i++ )
		{
			int32 val = proto->Stats[i].Value;
			/*
			if( val == 0 )
				continue;
			*/
			ModifyBonuses( proto->Stats[i].Type, val, apply );
		}

		// Armor
		if( proto->Armor )
		{
			if( apply )BaseResistance[0 ]+= proto->Armor;
			else  BaseResistance[0] -= proto->Armor;
			CalcResistance( 0 );
		}

		// Damage
		if( proto->Damage[0].Min )
		{
			if( proto->InventoryType == INVTYPE_RANGED || proto->InventoryType == INVTYPE_RANGEDRIGHT || proto->InventoryType == INVTYPE_THROWN )
			{
				BaseRangedDamage[0] += apply ? proto->Damage[0].Min : -proto->Damage[0].Min;
				BaseRangedDamage[1] += apply ? proto->Damage[0].Max : -proto->Damage[0].Max;
			}
			else
			{
				if( slot == EQUIPMENT_SLOT_OFFHAND )
				{
					BaseOffhandDamage[0] = apply ? proto->Damage[0].Min : 0;
					BaseOffhandDamage[1] = apply ? proto->Damage[0].Max : 0;
				}
				else
				{
					BaseDamage[0] = apply ? proto->Damage[0].Min : 0;
					BaseDamage[1] = apply ? proto->Damage[0].Max : 0;
				}
			}
		}
	} // end of the scalingstats else branch

	// Misc
	if( apply )
	{
		// Apply all enchantment bonuses
		item->ApplyEnchantmentBonuses();

		for( int k = 0; k < 5; k++ )
		{
			// stupid fucked dbs
			if( item->GetProto()->Spells[k].Id == 0 )
				continue;

			if( item->GetProto()->Spells[k].Trigger == 1 )
			{
				SpellEntry* spells = dbcSpell.LookupEntry( item->GetProto()->Spells[k].Id );
				if( spells->RequiredShapeShift )
				{
					AddShapeShiftSpell( spells->Id );
					continue;
				}

				Spell *spell = SpellPool.PooledNew();
				if (!spell)
					return;
				spell->Init( this, spells ,true, NULL );
				SpellCastTargets targets;
				targets.m_unitTarget = this->GetGUID();
				spell->castedItemId = item->GetEntry();
				spell->prepare( &targets );

			}
			else if( item->GetProto()->Spells[k].Trigger == 2 )
			{
				ProcTriggerSpell ts;
				ts.origId = 0;
				ts.spellId = item->GetProto()->Spells[k].Id;
				ts.procChance = 5;
				ts.caster = this->GetGUID();
				ts.procFlags = PROC_ON_MELEE_ATTACK;
				ts.deleted = false;
				ts.groupRelation[0] = 0;
				ts.groupRelation[1] = 0;
				ts.groupRelation[2] = 0;
				ts.ProcType = 0;
				ts.LastTrigger = 0;
				ts.procCharges = 0;
				m_procSpells.push_front( ts );
			}
		}
	}
	else
	{
		// Remove all enchantment bonuses
		item->RemoveEnchantmentBonuses();
		for( int k = 0; k < 5; k++ )
		{
			if( item->GetProto()->Spells[k].Trigger == 1 )
			{
				SpellEntry* spells = dbcSpell.LookupEntry( item->GetProto()->Spells[k].Id );
				if( spells->RequiredShapeShift )
					RemoveShapeShiftSpell( spells->Id );
				else
					RemoveAura( item->GetProto()->Spells[k].Id );
			}
			else if( item->GetProto()->Spells[k].Trigger == 2 )
			{
				std::list<struct ProcTriggerSpell>::iterator i;
				// Debug: i changed this a bit the if was not indented to the for
				// so it just set last one to deleted looks like unintended behaviour
				// because you can just use end()-1 to remove last so i put the if
				// into the for
				for( i = m_procSpells.begin(); i != m_procSpells.end(); i++ )
				{
					if( (*i).spellId == item->GetProto()->Spells[k].Id && !(*i).deleted )
					{
						//m_procSpells.erase(i);
						i->deleted = true;
						break;
					}
				}
			}
		}
	}

	if( !apply ) // force remove auras added by using this item
	{
		for(uint32 k = MAX_POSITIVE_AURAS_EXTEDED_START; k < MAX_POSITIVE_AURAS_EXTEDED_END; ++k)
		{
			Aura* m_aura = this->m_auras[k];
			if( m_aura != NULL && m_aura->m_castedItemId && m_aura->m_castedItemId == proto->ItemId )
				m_aura->Remove();
		}
	}

	if( !skip_stat_apply )
		UpdateStats();
}


void Player::SetMovement(uint8 pType, uint32 flag)
{
	WorldPacket data(13);

	switch(pType)
	{
	case MOVE_ROOT:
		{
			data.SetOpcode(SMSG_FORCE_MOVE_ROOT);
			data << GetNewGUID();
			data << flag;
			m_currentMovement = MOVE_ROOT;
		}break;
	case MOVE_UNROOT:
		{
			data.SetOpcode(SMSG_FORCE_MOVE_UNROOT);
			data << GetNewGUID();
			data << flag;
			m_currentMovement = MOVE_UNROOT;
		}break;
	case MOVE_WATER_WALK:
		{
			m_setwaterwalk=true;
			data.SetOpcode(SMSG_MOVE_WATER_WALK);
			data << GetNewGUID();
			data << flag;
		}break;
	case MOVE_LAND_WALK:
		{
			m_setwaterwalk=false;
			data.SetOpcode(SMSG_MOVE_LAND_WALK);
			data << GetNewGUID();
			data << flag;
		}break;
	default:break;
	}

	if(data.size() > 0)
		SendMessageToSet(&data, true);
}

void Player::SetPlayerSpeed(uint8 SpeedType, float value)
{
	/*WorldPacket data(18);
	data << GetNewGUID();
	data << m_speedChangeCounter++;

	if(SpeedType == RUN)			// nfi what this is.. :/
		data << uint8(1);

	data << value;*/
	WorldPacket data(50);
	/*if(SpeedType==FLY)
	{
		data << GetNewGUID();
		data << m_speedChangeCounter++;

		if(SpeedType == RUN)			// nfi what this is.. :/
			data << uint8(1);

		data << value;
	}
	else
	{
		data << GetNewGUID();
		data << uint32(0);
		data << uint8(0);
		data << uint32(getMSTime());
		data << GetPosition();
		data << m_position.o;
		data << uint32(0);
		data << value;
	}*/
	if( SpeedType != SWIMBACK )
	{
		data << GetNewGUID();
		data << m_speedChangeCounter++;
		if( SpeedType == RUN )
			data << uint8(1);

		data << value;
	}
	else
	{
		data << GetNewGUID();
		data << uint32(0);
		data << uint8(0);
		data << uint32(getMSTime());
		data << GetPosition();
		data << m_position.o;
		data << uint32(0);
		data << value;
	}

	switch(SpeedType)
	{
	case RUN:
		{
			if(value == m_lastRunSpeed)
				return;

			data.SetOpcode(SMSG_FORCE_RUN_SPEED_CHANGE);
			m_runSpeed = value;
			m_lastRunSpeed = value;
		}break;
	case RUNBACK:
		{
			if(value == m_lastRunBackSpeed)
				return;

			data.SetOpcode(SMSG_FORCE_RUN_BACK_SPEED_CHANGE);
			m_backWalkSpeed = value;
			m_lastRunBackSpeed = value;
		}break;
	case SWIM:
		{
			if(value == m_lastSwimSpeed)
				return;

			data.SetOpcode(SMSG_FORCE_SWIM_SPEED_CHANGE);
			m_swimSpeed = value;
			m_lastSwimSpeed = value;
		}break;
	case SWIMBACK:
		{
			if(value == m_lastBackSwimSpeed)
				break;

			data.SetOpcode(SMSG_FORCE_SWIM_BACK_SPEED_CHANGE);
			m_backSwimSpeed = value;
			m_lastBackSwimSpeed = value;
		}break;
	case FLY:
		{
			if(value == m_lastFlySpeed)
				return;

			data.SetOpcode(SMSG_FORCE_FLIGHT_SPEED_CHANGE);
			m_flySpeed = value;
			m_lastFlySpeed = value;
		}break;
	default:return;
	}

	SendMessageToSet(&data , true);
}

void Player::SendDungeonDifficulty()
{
    WorldPacket data(MSG_SET_DUNGEON_DIFFICULTY, 12);
	data << (uint32)iInstanceType;
    data << (uint32)0x1;
    data << (uint32)InGroup();
    GetSession()->SendPacket(&data);
}

void Player::BuildPlayerRepop()
{
	// Cleanup first
	uint32 AuraIds[] = {20584,9036,8326,0};
	RemoveAuras(AuraIds); // Cebernic: Removeaura just remove once(bug?).

	SetUInt32Value( UNIT_FIELD_HEALTH, 1 );

	SpellCastTargets tgt;
	tgt.m_unitTarget=this->GetGUID();

	if(getRace()==RACE_NIGHTELF)
	{
		SpellEntry *inf=dbcSpell.LookupEntry(9036); // Cebernic:20584 triggered.
		Spell * sp = SpellPool.PooledNew();
		if (!sp)
			return;
		sp->Init(this,inf,true,NULL);
		sp->prepare(&tgt);
	}
	else
	{
		SpellEntry *inf=dbcSpell.LookupEntry(8326);
		Spell * sp = SpellPool.PooledNew();
		if (!sp)
			return;
		sp->Init(this,inf,true,NULL);
		sp->prepare(&tgt);
	}

	StopMirrorTimer(0);
	StopMirrorTimer(1);
	StopMirrorTimer(2);

	SetFlag(PLAYER_FLAGS, 0x10);

	SetMovement(MOVE_UNROOT, 1);
	SetMovement(MOVE_WATER_WALK, 1);
}

void Player::RepopRequestedPlayer()
{
	sEventMgr.RemoveEvents(this, EVENT_PLAYER_CHECKFORCHEATS); // cebernic:-> Remove this first
	sEventMgr.RemoveEvents( this, EVENT_PLAYER_FORECED_RESURECT ); //in case somebody resurrected us before this event happened

	if( myCorpse != NULL ) {
		// Cebernic: wOOo dead+dead = undead ? :D just resurrect player
		myCorpse->ResetDeathClock();
		RepopAtGraveyard( GetPositionX(), GetPositionY(), GetPositionZ(), GetMapId() );
		return;
	}


	if( m_CurrentTransporter != NULL )
	{
		m_CurrentTransporter->RemovePlayer( this );
		m_CurrentTransporter = NULL;
		m_TransporterGUID = 0;

		//ResurrectPlayer();
		RepopAtGraveyard( GetPositionX(), GetPositionY(), GetPositionZ(), GetMapId() );
		return;
	}

	MapInfo * pMapinfo = NULL;

	// Set death state to corpse, that way players will lose visibility
	setDeathState( CORPSE );

	// Update visibility, that way people wont see running corpses :P
	UpdateVisibility();

	// If we're in battleground, remove the skinnable flag.. has bad effects heheh
	RemoveFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE );

	bool corpse = (m_bg != NULL) ? m_bg->CreateCorpse( this ) : true;

	if( corpse )
		CreateCorpse();


	BuildPlayerRepop();


	// Cebernic: don't do this.
  if ( !m_bg || ( m_bg && m_bg->HasStarted() ) )
  {
		pMapinfo = WorldMapInfoStorage.LookupEntry( GetMapId() );
		if( pMapinfo != NULL )
		{
			if( pMapinfo->type == INSTANCE_NULL || pMapinfo->type == INSTANCE_BATTLEGROUND )
			{
				RepopAtGraveyard( GetPositionX(), GetPositionY(), GetPositionZ(), GetMapId() );
			}
			else
			{
				RepopAtGraveyard( pMapinfo->repopx, pMapinfo->repopy, pMapinfo->repopz, pMapinfo->repopmapid );
			}
            switch( pMapinfo->mapid )
            {
                case 533: // Naxx
                case 550: // The Eye
                case 552: // The Arcatraz
                case 553: // The Botanica
                case 554: // The Mechanar
                ResurrectPlayer();
                return;
            }

		}
		else
		{
			// RepopAtGraveyard( GetPositionX(), GetPositionY(), GetPositionZ(), GetMapId() );
			// Cebernic: Mapinfo NULL? let's search from bindposition.
			RepopAtGraveyard( GetBindPositionX(),GetBindPositionY(),GetBindPositionZ(),GetBindMapId( ) );
		}
	}

	if( corpse )
	{
		SpawnCorpseBody();

		if ( myCorpse!=NULL ) myCorpse->ResetDeathClock();

		/* Send Spirit Healer Location */
		WorldPacket data( SMSG_DEATH_RELEASE_LOC, 16 );
		data << m_mapId << m_position;
		m_session->SendPacket( &data );

		/* Corpse reclaim delay */
		WorldPacket data2( SMSG_CORPSE_RECLAIM_DELAY, 4 );
		data2 << (uint32)( CORPSE_RECLAIM_TIME_MS );
		GetSession()->SendPacket( &data2 );
	}

}

void Player::ResurrectPlayer()
{
	sEventMgr.RemoveEvents(this,EVENT_PLAYER_FORECED_RESURECT); // In case somebody resurected us before this event happened
	if( m_resurrectHealth )
		SetUInt32Value( UNIT_FIELD_HEALTH, (uint32)min( m_resurrectHealth, m_uint32Values[UNIT_FIELD_MAXHEALTH] ) );
	if( m_resurrectMana )
		SetPower(0, m_resurrectMana);

	m_resurrectHealth = m_resurrectMana = 0;

	SpawnCorpseBones();

	RemoveNegativeAuras();
	uint32 AuraIds[] = {20584,9036,8326,0};
	RemoveAuras(AuraIds); // Cebernic: removeaura just remove once(bug?).

	RemoveFlag(PLAYER_FLAGS, 0x10);
	setDeathState(ALIVE);
	UpdateVisibility();
	if ( m_resurrecter && IsInWorld()
		// Don't pull players inside instances with this trick. Also fixes the part where you were able to double item bonuses
		&& m_resurrectInstanceID == GetInstanceID()
		)
	{
		SafeTeleport( m_resurrectMapId, m_resurrectInstanceID, m_resurrectPosition );
	}
	m_resurrecter = 0;
	SetMovement(MOVE_LAND_WALK, 1);

	// reinit
	m_lastRunSpeed = 0;
	m_lastRunBackSpeed = 0;
	m_lastSwimSpeed = 0;
	m_lastRunBackSpeed = 0;
	m_lastFlySpeed = 0;

	// Zack : shit on grill. So auras should be removed on player death instead of making this :P
	// We can afford this bullshit atm since auras are lost uppon death -> no immunities
	for(uint32 i = 0; i < 7; i++)
		SchoolImmunityList[i]=0;
	
	SpawnActivePet();
}

void Player::KillPlayer()
{
    if(getDeathState() != ALIVE) //You can't kill what has no life.
        return;
	setDeathState(JUST_DIED);

	// Battleground stuff
	if(m_bg)
		m_bg->HookOnPlayerDeath(this);

	EventDeath();

	m_session->OutPacket(SMSG_CANCEL_COMBAT);
	m_session->OutPacket(SMSG_CANCEL_AUTO_REPEAT);

	SetMovement(MOVE_ROOT, 0);
	StopMirrorTimer(0);
	StopMirrorTimer(1);
	StopMirrorTimer(2);

	SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED); // Player death animation, also can be used with DYNAMIC_FLAGS <- huh???
	SetUInt32Value( UNIT_DYNAMIC_FLAGS, 0x00 );

	if( getClass() == WARRIOR ) // Rage resets on death
		SetPower( POWER_TYPE_RAGE, 0 );
	else if( getClass() == DEATHKNIGHT )
		SetPower( POWER_TYPE_RUNIC_POWER, 0 );

	sHookInterface.OnDeath( this );

}

void Player::CreateCorpse()
{
	Corpse *pCorpse;
	uint32 _uf, _pb, _pb2, _cfb1, _cfb2;

	objmgr.DelinkPlayerCorpses(this);
	if(!this->bCorpseCreateable)
	{
		bCorpseCreateable = true;   // For next time
		return; // No corpse allowed!
	}

	pCorpse = objmgr.CreateCorpse();
	pCorpse->SetInstanceID(GetInstanceID());
	pCorpse->Create(this, GetMapId(), GetPositionX(),
		GetPositionY(), GetPositionZ(), GetOrientation());

	_uf = GetUInt32Value(UNIT_FIELD_BYTES_0);
	_pb = GetUInt32Value(PLAYER_BYTES);
	_pb2 = GetUInt32Value(PLAYER_BYTES_2);

	uint8 race	   = (uint8)(_uf);
	uint8 skin	   = (uint8)(_pb);
	uint8 face	   = (uint8)(_pb >> 8);
	uint8 hairstyle  = (uint8)(_pb >> 16);
	uint8 haircolor  = (uint8)(_pb >> 24);
	uint8 facialhair = (uint8)(_pb2);

	_cfb1 = ((0x00) | (race << 8) | (0x00 << 16) | (skin << 24));
	_cfb2 = ((face) | (hairstyle << 8) | (haircolor << 16) | (facialhair << 24));

	pCorpse->SetZoneId( GetZoneId() );
	pCorpse->SetUInt32Value( CORPSE_FIELD_BYTES_1, _cfb1 );
	pCorpse->SetUInt32Value( CORPSE_FIELD_BYTES_2, _cfb2 );
	pCorpse->SetUInt32Value( CORPSE_FIELD_FLAGS, 4 );
	pCorpse->SetUInt32Value( CORPSE_FIELD_DISPLAY_ID, GetUInt32Value(UNIT_FIELD_DISPLAYID) );

	if(m_bg)
	{
		// Remove our lootable flags
		RemoveFlag(UNIT_DYNAMIC_FLAGS, U_DYN_FLAG_LOOTABLE);
		RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE);

		loot.gold = 0;

		pCorpse->generateLoot();
		if(bShouldHaveLootableOnCorpse)
		{
			pCorpse->SetUInt32Value(CORPSE_FIELD_DYNAMIC_FLAGS, 1); // sets it so you can loot the plyr
		}
		else
		{
			// Hope this works
			pCorpse->SetUInt32Value(CORPSE_FIELD_FLAGS, 60);
		}

		// Now that our corpse is created, don't do it again
		bShouldHaveLootableOnCorpse = false;
	}
	else
	{
		pCorpse->loot.gold = 0;
	}

	uint32 iDisplayID = 0;
	uint16 iIventoryType = 0;
	uint32 _cfi = 0;
	Item * pItem;
	for (int8 i = 0; i < EQUIPMENT_SLOT_END; i++)
	{
		if(( pItem = GetItemInterface()->GetInventoryItem(i)) != 0)
		{
			iDisplayID = pItem->GetProto()->DisplayInfoID;
			iIventoryType = (uint16)pItem->GetProto()->InventoryType;

			_cfi =  (uint16(iDisplayID)) | (iIventoryType)<< 24;
			pCorpse->SetUInt32Value(CORPSE_FIELD_ITEM + i,_cfi);
		}
	}
	// Save corpse in db for future use
	pCorpse->SaveToDB();
}

void Player::SpawnCorpseBody()
{
	Corpse *pCorpse;

	pCorpse = objmgr.GetCorpseByOwner(this->GetLowGUID());
	if(pCorpse && !pCorpse->IsInWorld())
	{
		if(bShouldHaveLootableOnCorpse && pCorpse->GetUInt32Value(CORPSE_FIELD_DYNAMIC_FLAGS) != 1)
			pCorpse->SetUInt32Value(CORPSE_FIELD_DYNAMIC_FLAGS, 1); // sets it so you can loot the plyr

		if(m_mapMgr == 0)
			pCorpse->AddToWorld();
		else
			pCorpse->PushToWorld(m_mapMgr);
	}
	myCorpse = pCorpse;
}

void Player::SpawnCorpseBones()
{
	Corpse *pCorpse;
	pCorpse = objmgr.GetCorpseByOwner(GetLowGUID());
	myCorpse = 0;
	if(pCorpse)
	{
		if (pCorpse->IsInWorld() && pCorpse->GetCorpseState() == CORPSE_STATE_BODY)
		{
			if(pCorpse->GetInstanceID() != GetInstanceID())
			{
				sEventMgr.AddEvent(pCorpse, &Corpse::SpawnBones, EVENT_CORPSE_SPAWN_BONES, 100, 1,0);
			}
			else
				pCorpse->SpawnBones();
		}
		else
		{
			//Cheater!
		}
	}
}

void Player::DeathDurabilityLoss(double percent)
{
	m_session->OutPacket(SMSG_DURABILITY_DAMAGE_DEATH);
	uint32 pDurability;
	uint32 pMaxDurability;
	int32 pNewDurability;
	Item * pItem;

	for (int8 i = 0; i < EQUIPMENT_SLOT_END; i++)
	{
		if((pItem = GetItemInterface()->GetInventoryItem(i)) != 0)
		{
			pMaxDurability = pItem->GetUInt32Value(ITEM_FIELD_MAXDURABILITY);
			pDurability =  pItem->GetUInt32Value(ITEM_FIELD_DURABILITY);
			if(pDurability)
			{
				pNewDurability = (uint32)(pMaxDurability*percent);
				pNewDurability = (pDurability - pNewDurability);
				if(pNewDurability < 0)
					pNewDurability = 0;

				if(pNewDurability <= 0)
				{
					ApplyItemMods(pItem, i, false, true);
				}

				pItem->SetUInt32Value(ITEM_FIELD_DURABILITY,(uint32)pNewDurability);
				pItem->m_isDirty = true;
			}
		}
	}
}

void Player::RepopAtGraveyard(float ox, float oy, float oz, uint32 mapid)
{
	bool first = true;
	// float closestX = 0, closestY = 0, closestZ = 0, closestO = 0;
	StorageContainerIterator<GraveyardTeleport> * itr;

	LocationVector src(ox, oy, oz);
	LocationVector dest(0, 0, 0, 0);
	LocationVector temp;
	float closest_dist = 999999.0f;
	float dist;

	if(m_bg && m_bg->HookHandleRepop(this))
	{
		return;
	}
	else
	{
		uint32 areaid = sInstanceMgr.GetMap(mapid)->GetAreaID(ox,oy);
		AreaTable * at = dbcArea.LookupEntry(areaid);
		if(!at) return;

		// uint32 mzone = ( at->ZoneId ? at->ZoneId : at->AreaId);

		itr = GraveyardStorage.MakeIterator();
		while(!itr->AtEnd())
		{
			GraveyardTeleport *pGrave = itr->Get();
			if((pGrave->MapId == mapid && pGrave->FactionID == GetTeam() || pGrave->MapId == mapid && pGrave->FactionID == 3)
				|| (pGrave->MapId == mapid && pGrave->FactionID == GetTeam() || pGrave->MapId == mapid && pGrave->FactionID == 3))
			{
				temp.ChangeCoords(pGrave->X, pGrave->Y, pGrave->Z);
				dist = src.DistanceSq(temp);
				if( first || dist < closest_dist )
				{
					first = false;
					closest_dist = dist;
					dest = temp;
				}
			}

			if(!itr->Inc())
				break;
		}
		itr->Destruct();
	}

	if(sHookInterface.OnRepop(this) && dest.x != 0 && dest.y != 0 && dest.z != 0)
	{
		SafeTeleport(mapid, 0, dest);
	}


//	//correct method as it works on official server, and does not require any damn sql
//	//no factions! no zones! no sqls! 1word: blizz-like
//	float closestX , closestY , closestZ ;
//	uint32 entries=sWorldSafeLocsStore.GetNumRows();
//	GraveyardEntry*g;
//	uint32 mymapid=mapid
//	float mx=ox,my=oy;
//	float last_distance=9e10;
//
//	for(uint32 x=0;x<entries;x++)
//	{
//		g=sWorldSafeLocsStore.LookupEntry(x);
//		if(g->mapid!=mymapid)continue;
//		float distance=(mx-g->x)*(mx-g->x)+(my-g->y)*(my-g->y);
//		if(distance<last_distance)
//		{
//			closestX=g->x;
//			closestY=g->y;
//			closestZ=g->z;
//			last_distance=distance;
//		}
//
//
//	}
//	if(last_distance<1e10)
//#endif


}

void Player::JoinedChannel(Channel *c)
{
	if( c != NULL )
		m_channels.insert(c);
}

void Player::LeftChannel(Channel *c)
{
	if( c != NULL )
		m_channels.erase(c);
}

void Player::CleanupChannels()
{
	set<Channel *>::iterator i;
	Channel * c;
	for(i = m_channels.begin(); i != m_channels.end();)
	{
		c = *i;
		++i;

		c->Part(this);
	}
}

void Player::SendInitialActions()
{
#ifndef USING_BIG_ENDIAN
	m_session->OutPacket(SMSG_ACTION_BUTTONS, PLAYER_ACTION_BUTTON_SIZE, &mActions);
#else
	/* we can't do this the fast way on ppc, due to endianness */
	WorldPacket data(SMSG_ACTION_BUTTONS, PLAYER_ACTION_BUTTON_SIZE);
	for(uint32 i = 0; i < PLAYER_ACTION_BUTTON_SIZE; ++i)
	{
		data << mActions[i].Action << mActions[i].Type << mActions[i].Misc;
	}
	m_session->SendPacket(&data);
#endif
}

void Player::setAction(uint8 button, uint16 action, uint8 type, uint8 misc)
{
	if( button >= 120 )
		return; //packet hack to crash server
	mActions[button].Action = action;
	mActions[button].Type = type;
	mActions[button].Misc = misc;
}

// Groupcheck
bool Player::IsGroupMember(Player *plyr)
{
	if(m_playerInfo->m_Group != NULL)
		return m_playerInfo->m_Group->HasMember(plyr->m_playerInfo);

	return false;
}

int32 Player::GetOpenQuestSlot()
{
	for (uint32 i = 0; i < 25; ++i)
		if (m_questlog[i] == NULL)
			return i;

	return -1;
}

void Player::AddToFinishedQuests(uint32 quest_id)
{
	//maybe that shouldn't be an assert, but i'll leave it for now
	//ASSERT(m_finishedQuests.find(quest_id) == m_finishedQuests.end());
	//Removed due to crash
	//If it failed though, then he's probably cheating.
	if (m_finishedQuests.find(quest_id) != m_finishedQuests.end())
		return;

	m_finishedQuests.insert(quest_id);
}

bool Player::HasFinishedQuest(uint32 quest_id)
{
	return (m_finishedQuests.find(quest_id) != m_finishedQuests.end());
}


bool Player::GetQuestRewardStatus(uint32 quest_id)
{
	return HasFinishedQuest(quest_id);
}

// From Mangos Project
void Player::_LoadTutorials(QueryResult * result)
{
	if(result)
	{
		 Field *fields = result->Fetch();
		 for (int iI=0; iI<8; iI++)
			 m_Tutorials[iI] = fields[iI + 1].GetUInt32();
	}
	tutorialsDirty = false;
}

void Player::_SaveTutorials(QueryBuffer * buf)
{
	if(tutorialsDirty)
	{
		if(buf == NULL)
			CharacterDatabase.Execute("REPLACE INTO tutorials VALUES('%u','%u','%u','%u','%u','%u','%u','%u','%u')", GetLowGUID(), m_Tutorials[0], m_Tutorials[1], m_Tutorials[2], m_Tutorials[3], m_Tutorials[4], m_Tutorials[5], m_Tutorials[6], m_Tutorials[7]);
		else
			buf->AddQuery("REPLACE INTO tutorials VALUES('%u','%u','%u','%u','%u','%u','%u','%u','%u')", GetLowGUID(), m_Tutorials[0], m_Tutorials[1], m_Tutorials[2], m_Tutorials[3], m_Tutorials[4], m_Tutorials[5], m_Tutorials[6], m_Tutorials[7]);

		tutorialsDirty = false;
	}
}

uint32 Player::GetTutorialInt(uint32 intId )
{
	ASSERT( intId < 8 );
	return m_Tutorials[intId];
}

void Player::SetTutorialInt(uint32 intId, uint32 value)
{
	if(intId >= 8)
		return;

	ASSERT( (intId < 8) );
	m_Tutorials[intId] = value;
	tutorialsDirty = true;
}

//Player stats calculation for saving at lvl up, etc
/*void Player::CalcBaseStats()
{//static_cast< Player* >( this )->getClass() == HUNTER ||
	//TODO take into account base stats at create
	uint32 AP, RAP;
	//Save AttAck power
	if(getClass() == ROGUE || getClass() == HUNTER)
	{
		AP = GetBaseUInt32Value(UNIT_FIELD_STAT0) + GetBaseUInt32Value(UNIT_FIELD_STAT1);
		RAP = (GetBaseUInt32Value(UNIT_FIELD_STAT1) * 2);
		SetBaseUInt32Value(UNIT_FIELD_ATTACK_POWER, AP);
		SetBaseUInt32Value(UNIT_FIELD_RANGED_ATTACK_POWER, RAP);
	}
	else
	{
		AP = (GetBaseUInt32Value(UNIT_FIELD_STAT0) * 2);
		RAP = (GetBaseUInt32Value(UNIT_FIELD_STAT1) * 2);
		SetBaseUInt32Value(UNIT_FIELD_ATTACK_POWER, AP);
		SetBaseUInt32Value(UNIT_FIELD_RANGED_ATTACK_POWER, RAP);
	}

}*/

void Player::UpdateHit(int32 hit)
{
   /*std::list<Affect*>::iterator i;
	Affect::ModList::const_iterator j;
	Affect *aff;
	uint32 in = hit;
	for (i = GetAffectBegin(); i != GetAffectEnd(); i++)
	{
		aff = *i;
		for (j = aff->GetModList().begin();j != aff->GetModList().end(); j++)
		{
			Modifier mod = (*j);
			if ((mod.GetType() == SPELL_AURA_MOD_HIT_CHANCE))
			{
				SpellEntry *spellInfo = sSpellStore.LookupEntry(aff->GetSpellId());
				if (this->canCast(spellInfo))
					in += mod.GetAmount();
			}
		}
	}
	SetHitFromSpell(in);*/
}

float Player::GetDefenseChance(uint32 opLevel)
{
	float chance;

	chance = float( _GetSkillLineCurrent( SKILL_DEFENSE, true ) - ( opLevel * 5.0f ) );
	chance += CalcRating( PLAYER_RATING_MODIFIER_DEFENCE );
	chance = floorf( chance ) * 0.04f; // defense skill is treated as an integer on retail

	return chance;
}

#define BASE_BLOCK_CHANCE 5.0f
#define BASE_PARRY_CHANCE 5.0f

// Gets dodge chances before defense skill is applied
float Player::GetDodgeChance()
{
	uint32 pClass = (uint32)getClass();
	float chance;

	// Base dodge chance
	chance = baseDodge[pClass];

	// Dodge from agility
	chance += float( GetUInt32Value( UNIT_FIELD_STAT1 ) / dodgeRatio[getLevel()-1][pClass] );

	// Dodge from dodge rating
	chance += CalcRating( PLAYER_RATING_MODIFIER_DODGE );

	// Dodge from spells
	chance += GetDodgeFromSpell();

	return max( chance, 0.0f ); // Make sure we dont have a negative chance
}

// Gets block chances before defense skill is applied
// Assumes the caller will check for shields
float Player::GetBlockChance()
{
	float chance;

	// Base block chance
	chance = BASE_BLOCK_CHANCE;

	// Block rating
	chance += CalcRating( PLAYER_RATING_MODIFIER_BLOCK );

	// Block chance from spells
	chance += GetBlockFromSpell();

	return max( chance, 0.0f ); // Make sure we dont have a negative chance
}

// Get parry chances before defense skill is applied
float Player::GetParryChance()
{
	float chance;

	// Base parry chance
	chance = BASE_PARRY_CHANCE;

	// Parry rating
	chance += CalcRating( PLAYER_RATING_MODIFIER_PARRY );

	// Parry chance from spells
	chance += GetParryFromSpell();

	return max( chance, 0.0f ); // Make sure we dont have a negative chance
}

void Player::UpdateChances()
{
	uint32 pClass = (uint32)getClass();
	uint32 pLevel = (getLevel() > PLAYER_LEVEL_CAP) ? PLAYER_LEVEL_CAP : getLevel();

	float tmp = 0;
	float defence_contribution = 0;

	// Avoidance from defense skill
	defence_contribution = GetDefenseChance( pLevel );

	// Dodge
	tmp = GetDodgeChance();
	tmp += defence_contribution;
	tmp = min( max ( tmp, 0.0f ), 95.0f );
	SetFloatValue( PLAYER_DODGE_PERCENTAGE, tmp );

	// Block
	Item* it = this->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_OFFHAND );
	if( it != NULL && it->GetProto()->InventoryType == INVTYPE_SHIELD )
	{
		tmp = GetBlockChance();
		tmp += defence_contribution;
		tmp = min( max( tmp, 0.0f ), 95.0f );
	}
	else
		tmp = 0.0f;

	SetFloatValue( PLAYER_BLOCK_PERCENTAGE, tmp );

	// Parry (can only parry with something in main hand)
	it = this->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
	if( it != NULL )
	{
		tmp = GetParryChance();
		tmp += defence_contribution;
		tmp = min( max( tmp, 0.0f ), 95.0f );
	}
	else
		tmp = 0.0f;

	SetFloatValue( PLAYER_PARRY_PERCENTAGE, tmp );

	// Critical
	gtFloat* baseCrit = dbcMeleeCritBase.LookupEntry(pClass-1);
	gtFloat* CritPerAgi = dbcMeleeCrit.LookupEntry(pLevel - 1 + (pClass-1)*100);

	tmp = 100*(baseCrit->val + GetUInt32Value( UNIT_FIELD_STAT1 ) * CritPerAgi->val);

	float melee_bonus = 0;
	float ranged_bonus = 0;

	if ( tocritchance.size() > 0 ) // Crashfix by Cebernic
	{
		map< uint32, WeaponModifier >::iterator itr = tocritchance.begin();

		Item* tItemMelee = GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
		Item* tItemRanged = GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_RANGED );

		//-1 = any weapon

		for(; itr != tocritchance.end(); ++itr )
		{
			if( itr->second.wclass == ( uint32 )-1 || ( tItemMelee != NULL && ( 1 << tItemMelee->GetProto()->SubClass & itr->second.subclass ) ) )
			{
				melee_bonus += itr->second.value;
			}
			if( itr->second.wclass == ( uint32 )-1 || ( tItemRanged != NULL && ( 1 << tItemRanged->GetProto()->SubClass & itr->second.subclass ) ) )
			{
				ranged_bonus += itr->second.value;
			}
		}
	}

	float cr = tmp + CalcRating( PLAYER_RATING_MODIFIER_MELEE_CRIT ) + melee_bonus;
	SetFloatValue( PLAYER_CRIT_PERCENTAGE, min( cr, 95.0f ) );

	float rcr = tmp + CalcRating( PLAYER_RATING_MODIFIER_RANGED_CRIT ) + ranged_bonus;
	SetFloatValue( PLAYER_RANGED_CRIT_PERCENTAGE, min( rcr, 95.0f ) );

	gtFloat* SpellCritBase  = dbcSpellCritBase.LookupEntry(pClass-1);
	gtFloat* SpellCritPerInt = dbcSpellCrit.LookupEntry(pLevel - 1 + (pClass-1)*100);

	spellcritperc = 100*(SpellCritBase->val + GetUInt32Value( UNIT_FIELD_STAT3 ) * SpellCritPerInt->val) +
		this->GetSpellCritFromSpell() +
		this->CalcRating( PLAYER_RATING_MODIFIER_SPELL_CRIT );
	UpdateChanceFields();
}

void Player::UpdateChanceFields()
{
	// Update spell crit values in fields
	for(uint32 i = 0; i < 7; ++i)
	{
		SetFloatValue(PLAYER_SPELL_CRIT_PERCENTAGE1 + i, SpellCritChanceSchool[i]+spellcritperc);
	}
}

void Player::ModAttackSpeed( int32 mod, ModType type )
{
	if( mod == 0 )
		return;

	if( mod > 0 )
		m_attack_speed[ type ] *= 1.0f + ( ( float )mod / 100.0f );
	else
		m_attack_speed[ type ] /= 1.0f + ( ( float )( - mod ) / 100.0f );

	if( type == MOD_SPELL )
		SetFloatValue( UNIT_MOD_CAST_SPEED, 1.0f / ( m_attack_speed[ MOD_SPELL ] * SpellHasteRatingBonus ) );
}

void Player::UpdateAttackSpeed()
{
	uint32 speed = 2000;
	Item *weap ;

	if( GetShapeShift() == FORM_CAT )
	{
		speed = 1000;
	}
	else if( GetShapeShift() == FORM_BEAR || GetShapeShift() == FORM_DIREBEAR )
	{
		speed = 2500;
	}
	else if( !disarmed )// Regular
	{
		weap = GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND ) ;
		if( weap != NULL )
			speed = weap->GetProto()->Delay;
	}
	SetUInt32Value( UNIT_FIELD_BASEATTACKTIME,
		( uint32 )( (float) speed / ( m_attack_speed[ MOD_MELEE ] * ( 1.0f + CalcRating( PLAYER_RATING_MODIFIER_MELEE_HASTE ) / 100.0f ) ) ) );

	weap = GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_OFFHAND );
	if( weap != NULL && weap->GetProto()->Class == ITEM_CLASS_WEAPON )
	{
		speed = weap->GetProto()->Delay;
		SetUInt32Value( UNIT_FIELD_BASEATTACKTIME_01,
			( uint32 )( (float) speed / ( m_attack_speed[ MOD_MELEE ] * ( 1.0f + CalcRating( PLAYER_RATING_MODIFIER_MELEE_HASTE ) / 100.0f ) ) ) );
	}

	weap = GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_RANGED );
	if( weap != NULL )
	{
		speed = weap->GetProto()->Delay;
		SetUInt32Value( UNIT_FIELD_RANGEDATTACKTIME,
			( uint32 )( (float) speed / ( m_attack_speed[ MOD_RANGED ] * ( 1.0f + CalcRating( PLAYER_RATING_MODIFIER_RANGED_HASTE ) / 100.0f ) ) ) );
	}
}

void Player::UpdateStats()
{
	UpdateAttackSpeed();

	// Formulas from wowwiki

	int32 AP = 0;
	int32 RAP = 0;
	int32 hpdelta = 128;
	int32 manadelta = 128;

	uint32 str = GetUInt32Value(UNIT_FIELD_STAT0);
	uint32 agi = GetUInt32Value(UNIT_FIELD_STAT1);
	uint32 lev = getLevel();

	// Attack power
	uint32 cl = getClass();
	switch (cl)
	{
	case DRUID:
        //(Strength x 2) - 20
        AP = str * 2 - 20;
        //Agility - 10
        RAP = agi - 10;

        if( GetShapeShift() == FORM_MOONKIN )
        {
            //(Strength x 2) + (Character Level x 1.5) - 20
            AP += float2int32( static_cast<float>(lev) * 1.5f );
        }
        if( GetShapeShift() == FORM_CAT )
		{
            //(Strength x 2) + Agility + (Character Level x 2) - 20
            AP += agi + (lev *2);
        }
        if( GetShapeShift() == FORM_BEAR || GetShapeShift() == FORM_DIREBEAR )
        {
            //(Strength x 2) + (Character Level x 3) - 20
            AP += (lev *3);
		}
		break;


	case ROGUE:
		//Strength + Agility + (Character Level x 2) - 20
		AP = str + agi + (lev *2) - 20;
		//Character Level + Agility - 10
		RAP = lev + agi - 10;

		break;


	case HUNTER:
		//Strength + Agility + (Character Level x 2) - 20
		 AP = str + agi + (lev *2) - 20;
		//(Character Level x 2) + Agility - 10
		RAP = (lev *2) + agi - 10;

		break;

	case SHAMAN:
		//(Strength) + (Agility) + (Character Level x 2) - 20
		AP = str + agi + (lev *2) - 20;
		//Agility - 10
		RAP = agi - 10;

		break;

	case PALADIN:
		//(Strength x 2) + (Character Level x 3) - 20
		AP = (str *2) + (lev *3) - 20;
		//Agility - 10
		RAP = agi - 10;

		break;


	case WARRIOR:
	case DEATHKNIGHT:
		//(Strength x 2) + (Character Level x 3) - 20
		AP = (str *2) + (lev *3) - 20;
		//Character Level + Agility - 10
		RAP = lev + agi - 10;

		break;

	default:    //mage,priest,warlock
		AP = agi - 10;
	}

	/* modifiers */
	RAP += int32(float(float(m_rap_mod_pct) * float(float(m_uint32Values[UNIT_FIELD_STAT3]) / 100.0f)));

	if( RAP < 0 )RAP = 0;
	if( AP < 0 )AP = 0;

	SetUInt32Value( UNIT_FIELD_ATTACK_POWER, AP );
	SetUInt32Value( UNIT_FIELD_RANGED_ATTACK_POWER, RAP );

	LevelInfo* lvlinfo = objmgr.GetLevelInfo( this->getRace(), this->getClass(), lev );

	if( lvlinfo != NULL )
	{
		hpdelta = lvlinfo->Stat[2] * 10;
		manadelta = lvlinfo->Stat[3] * 15;
	}

	lvlinfo = objmgr.GetLevelInfo( this->getRace(), this->getClass(), 1 );

	if( lvlinfo != NULL )
	{
		hpdelta -= lvlinfo->Stat[2] * 10;
		manadelta -= lvlinfo->Stat[3] * 15;
	}

	uint32 hp = GetUInt32Value( UNIT_FIELD_BASE_HEALTH );

	int32 stat_bonus = GetUInt32Value( UNIT_FIELD_POSSTAT2 ) - GetUInt32Value( UNIT_FIELD_NEGSTAT2 );
	if ( stat_bonus < 0 )
		stat_bonus = 0; // Avoid of having negative health
	int32 bonus = stat_bonus * 10 + m_healthfromspell + m_healthfromitems;

	uint32 res = hp + bonus + hpdelta;
	uint32 oldmaxhp = GetUInt32Value( UNIT_FIELD_MAXHEALTH );

	if( res < hp )
		res = hp;
	if( sWorld.m_limits.enable && (sWorld.m_limits.healthCap > 0) && (res > sWorld.m_limits.healthCap) && GetSession()->GetPermissionCount() <= 0 ) //hacker?
	{
		char logmsg[256];
		snprintf(logmsg, 256, "has over %lu health (%i)", sWorld.m_limits.healthCap, res);
		sCheatLog.writefromsession(GetSession(), logmsg);
		if(sWorld.m_limits.broadcast) // send info to online GM
		{
			string gm_ann = MSG_COLOR_GREEN;
			gm_ann += "|Hplayer:";
			gm_ann += GetName();
			gm_ann += "|h[";
			gm_ann += GetName();
			gm_ann += "]|h: ";
			gm_ann += MSG_COLOR_YELLOW;
			gm_ann += logmsg;
			sWorld.SendGMWorldText(gm_ann.c_str());
		}
		if(sWorld.m_limits.disconnect)
		{
			GetSession()->Disconnect();
		}
		else // no disconnect, set it to the cap instead
		{
			res = sWorld.m_limits.healthCap;
		}
	}
	SetUInt32Value( UNIT_FIELD_MAXHEALTH, res );

	if( ( int32 )GetUInt32Value( UNIT_FIELD_HEALTH ) > res )
		SetUInt32Value( UNIT_FIELD_HEALTH, res );
	else if( ( cl == DRUID) && ( GetShapeShift() == FORM_BEAR || GetShapeShift() == FORM_DIREBEAR ) )
	{
		res = float2int32( ( float )GetUInt32Value( UNIT_FIELD_MAXHEALTH ) * ( float )GetUInt32Value( UNIT_FIELD_HEALTH ) / float( oldmaxhp ) );
		SetUInt32Value( UNIT_FIELD_HEALTH, res );
	}

	if( cl != WARRIOR && cl != ROGUE && cl != DEATHKNIGHT)
	{
		// MP
		uint32 mana = GetUInt32Value( UNIT_FIELD_BASE_MANA );

		stat_bonus = GetUInt32Value( UNIT_FIELD_POSSTAT3 ) - GetUInt32Value( UNIT_FIELD_NEGSTAT3 );
		if ( stat_bonus < 0 )
			stat_bonus = 0; // Avoid of having negative mana
		bonus = stat_bonus * 15 + m_manafromspell + m_manafromitems ;

		res = mana + bonus + manadelta;
		if( res < mana )
			res = mana;	
		if( sWorld.m_limits.enable && (sWorld.m_limits.manaCap > 0) && (res > sWorld.m_limits.manaCap) && GetSession()->GetPermissionCount() <= 0 ) //hacker?
		{
			char logmsg[256];
			snprintf(logmsg, 256, "has over %lu mana (%i)", sWorld.m_limits.manaCap, res);
			sCheatLog.writefromsession(GetSession(), logmsg);
			if(sWorld.m_limits.broadcast) // send info to online GM
			{
				string gm_ann = MSG_COLOR_GREEN;
				gm_ann += "|Hplayer:";
				gm_ann += GetName();
				gm_ann += "|h[";
				gm_ann += GetName();
				gm_ann += "]|h: ";
				gm_ann += MSG_COLOR_YELLOW;
				gm_ann += logmsg;
				sWorld.SendGMWorldText(gm_ann.c_str());
			}
			if(sWorld.m_limits.disconnect)
			{
				GetSession()->Disconnect();
			}
			else // no disconnect, set it to the cap instead
			{
				res = sWorld.m_limits.manaCap;
			}
		}
		SetUInt32Value( UNIT_FIELD_MAXPOWER1, res );

		if((int32)GetUInt32Value(UNIT_FIELD_POWER1)>res)
			SetUInt32Value(UNIT_FIELD_POWER1,res);

		//Manaregen
		// table from http://www.wowwiki.com/Mana_regeneration
		const static float BaseRegen[80] = {0.034965f, 0.034191f, 0.033465f, 0.032526f, 0.031661f, 0.031076f, 0.030523f, 0.029994f, 0.029307f, 0.028661f, 0.027584f, 0.026215f, 0.025381f, 0.024300f, 0.023345f, 0.022748f, 0.021958f, 0.021386f, 0.020790f, 0.020121f, 0.019733f, 0.019155f, 0.018819f, 0.018316f, 0.017936f, 0.017576f, 0.017201f, 0.016919f, 0.016581f, 0.016233f, 0.015994f, 0.015707f, 0.015464f, 0.015204f, 0.014956f, 0.014744f, 0.014495f, 0.014302f, 0.014094f, 0.013895f, 0.013724f, 0.013522f, 0.013363f, 0.013175f, 0.012996f, 0.012853f, 0.012687f, 0.012539f, 0.012384f, 0.012233f, 0.012113f, 0.011973f, 0.011859f, 0.011714f, 0.011575f, 0.011473f, 0.011342f, 0.011245f, 0.011110f, 0.010999f, 0.010700f, 0.010522f, 0.010290f, 0.010119f, 0.009968f, 0.009808f, 0.009651f, 0.009553f, 0.009445f, 0.009327f, 0.008859f, 0.008415f, 0.007993f, 0.007592f, 0.007211f, 0.006849f, 0.006506f, 0.006179f, 0.005869f, 0.005575f };
		uint32 level = getLevel();
		if(level > 80) level = 80;
		//float amt = ( 0.001f + sqrt((float)Intellect) * Spirit * BaseRegen[level-1] )*PctPowerRegenModifier[POWER_TYPE_MANA];
		
		// Mesmer: new Manaregen forumla.
		uint32 Spirit = GetUInt32Value( UNIT_FIELD_STAT4 );
		uint32 Intellect = GetUInt32Value( UNIT_FIELD_STAT3 );
		float amt = ( 0.001f + sqrt((float)Intellect) * Spirit * BaseRegen[level-1] )* PctPowerRegenModifier[POWER_TYPE_MANA];
		SetFloatValue(UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER, amt + m_ModInterrMRegen * 0.2f);
		SetFloatValue(UNIT_FIELD_POWER_REGEN_INTERRUPTED_FLAT_MODIFIER, amt * m_ModInterrMRegenPCT/100.0f + m_ModInterrMRegen * 0.2f);
	}

	// Spell haste rating
	float haste = 1.0f + CalcRating( PLAYER_RATING_MODIFIER_SPELL_HASTE ) / 100.0f;
	if( haste != SpellHasteRatingBonus )
	{
		float value = GetFloatValue( UNIT_MOD_CAST_SPEED ) * SpellHasteRatingBonus / haste; // remove previous mod and apply current

		SetFloatValue( UNIT_MOD_CAST_SPEED, value );
		SpellHasteRatingBonus = haste;	// keep value for next run
	}


	// Shield Block
	Item* shield = GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_OFFHAND);
	if( shield != NULL && shield->GetProto()->InventoryType == INVTYPE_SHIELD )
	{
		float block_multiplier = ( 100.0f + float( m_modblockabsorbvalue ) ) / 100.0f;
		if( block_multiplier < 1.0f )block_multiplier = 1.0f;

		int32 blockable_damage = float2int32( (float( shield->GetProto()->Block ) + ( float(m_modblockvaluefromspells + GetUInt32Value( PLAYER_RATING_MODIFIER_BLOCK ) )) + ( ( float( str ) / 20.0f ) - 1.0f ) ) * block_multiplier);
		SetUInt32Value( PLAYER_SHIELD_BLOCK, blockable_damage );
	}
	else
	{
		SetUInt32Value( PLAYER_SHIELD_BLOCK, 0 );
	}

	// Expertise
	// Expertise is somewhat tricky. Expertise on items is expertise "rating" where as "expertise"
	// on the character sheet is the rating modified by a factor. The bonus % this value gives is
	// actually the "rating" modified by another factor.
	// eg. 100 expertise rating from items
	// 100 / 3.92 = 25 expertise
	// 100 / 15.77 = 6.3% reduced dodge/parry chances
	SetUInt32Value( PLAYER_EXPERTISE, float2int32( CalcRating( PLAYER_RATING_MODIFIER_EXPERTISE ) ) ); // value displayed in char sheet
	//SetUInt32Value( PLAYER_RATING_MODIFIER_EXPERTISE2, GetUInt32Value( PLAYER_RATING_MODIFIER_EXPERTISE ) );

	// Dynamic aura application, auras 212, 268
	for( uint32 x = MAX_TOTAL_AURAS_START; x < MAX_TOTAL_AURAS_END; x++ )
		if( m_auras[x] != NULL )
		{
			if( m_auras[x]->HasModType( SPELL_AURA_MOD_ATTACK_POWER_BY_STAT_PCT ) ||
				m_auras[x]->HasModType( SPELL_AURA_MOD_RANGED_ATTACK_POWER_BY_STAT_PCT ) )
				m_auras[x]->UpdateModifiers();
		}

	UpdateChances();
	CalcDamage();
}

uint32 Player::SubtractRestXP(uint32 amount)
{
	if(GetUInt32Value(UNIT_FIELD_LEVEL) >= GetUInt32Value(PLAYER_FIELD_MAX_LEVEL))		// Save CPU, don't waste time on this if you've reached max_level
		amount = 0;

	int32 restAmount = m_restAmount - (amount << 1);									// remember , we are dealing with xp without restbonus, so multiply by 2

	if( restAmount < 0)
		m_restAmount = 0;
	else
		m_restAmount = restAmount;

	Log.Debug("REST","Subtracted %d rest XP to a total of %d", amount, m_restAmount);
	UpdateRestState();																	// Update clients interface with new values.
	return amount;
}

void Player::AddCalculatedRestXP(uint32 seconds)
{
	// At level one, players will all start in the normal tier.
	// When a player rests in a city or at an inn they will gain rest bonus at a very slow rate.
	// Eight hours of rest will be needed for a player to gain one "bubble" of rest bonus.
	// At any given time, players will be able to accumulate a maximum of 30 "bubbles" worth of rest bonus which
	// translates into approximately 1.5 levels worth of rested play (before your character returns to normal rest state).
	// Thanks to the comforts of a warm bed and a hearty meal, players who rest or log out at an Inn will
	// accumulate rest credit four times faster than players logged off outside of an Inn or City.
	// Players who log out anywhere else in the world will earn rest credit four times slower.
	// http://www.worldofwarcraft.com/info/basics/resting.html


	// Define xp for a full bar ( = 20 bubbles)
	uint32 xp_to_lvl = uint32(lvlinfo->XPToNextLevel);

	// get RestXP multiplier from config.
	float bubblerate = sWorld.getRate(RATE_RESTXP);

	// One bubble (5% of xp_to_level) for every 8 hours logged out.
	// if multiplier RestXP (from ascent.config) is f.e 2, you only need 4hrs/bubble.
	uint32 rested_xp = uint32(0.05f * xp_to_lvl * ( seconds / (3600 * ( 8 / bubblerate))));

	// if we are at a resting area rest_XP goes 4 times faster (making it 1 bubble every 2 hrs)
	if (m_isResting)
		rested_xp <<= 2;

	// Add result to accumulated rested XP
	m_restAmount += uint32(rested_xp);

	// and set limit to be max 1.5 * 20 bubbles * multiplier (1.5 * xp_to_level * multiplier)
	if (m_restAmount > xp_to_lvl + (uint32)((float)( xp_to_lvl >> 1 ) * bubblerate ))
		m_restAmount = xp_to_lvl + (uint32)((float)( xp_to_lvl >> 1 ) * bubblerate );

	Log.Debug("REST","Add %d rest XP to a total of %d, RestState %d", rested_xp, m_restAmount,m_isResting);

	// Update clients interface with new values.
	UpdateRestState();
}

void Player::UpdateRestState()
{
	if(m_restAmount && GetUInt32Value(UNIT_FIELD_LEVEL) < GetUInt32Value(PLAYER_FIELD_MAX_LEVEL))
		m_restState = RESTSTATE_RESTED;
	else
		m_restState = RESTSTATE_NORMAL;

	// Update RestState 100%/200%
	SetUInt32Value(PLAYER_BYTES_2, ((GetUInt32Value(PLAYER_BYTES_2) & 0x00FFFFFF) | (m_restState << 24)));

	//update needle (weird, works at 1/2 rate)
	SetUInt32Value(PLAYER_REST_STATE_EXPERIENCE, m_restAmount >> 1);
}

void Player::ApplyPlayerRestState(bool apply)
{
	if(apply)
	{
		m_restState = RESTSTATE_RESTED;
		m_isResting = true;
		SetFlag(PLAYER_FLAGS, PLAYER_FLAG_RESTING);	//put zZz icon
	}
	else
	{
		m_isResting = false;
		RemoveFlag(PLAYER_FLAGS,PLAYER_FLAG_RESTING);	//remove zZz icon
	}
	UpdateRestState();
}

#define CORPSE_VIEW_DISTANCE 1600 // 40*40

bool Player::CanSee(Object* obj) // * Invisibility & Stealth Detection - Partha *
{
	if (obj == this)
	   return true;

	uint32 object_type = obj->GetTypeId();

	if(getDeathState() == CORPSE) // we are dead and we have released our spirit
	{
		if(object_type == TYPEID_PLAYER)
		{
			Player *pObj = static_cast< Player* >(obj);

			if(myCorpse && myCorpse->GetDistanceSq(obj) <= CORPSE_VIEW_DISTANCE)
				return !pObj->m_isGmInvisible; // we can see all players within range of our corpse except invisible GMs

			if(m_deathVision) // if we have arena death-vision we can see all players except invisible GMs
				return !pObj->m_isGmInvisible;

			return (pObj->getDeathState() == CORPSE); // we can only see players that are spirits
		}

		if(myCorpse)
		{
			if(myCorpse == obj)
				return true;

			if(myCorpse->GetDistanceSq(obj) <= CORPSE_VIEW_DISTANCE)
				return true; // we can see everything within range of our corpse
		}

		if(m_deathVision) // if we have arena death-vision we can see everything
			return true;

		if(object_type == TYPEID_UNIT)
		{
			Unit *uObj = static_cast<Unit *>(obj);

			return uObj->IsSpiritHealer(); // we can't see any NPCs except spirit-healers
		}

		return false;
	}
	//------------------------------------------------------------------

	switch(object_type) // we are alive or we haven't released our spirit yet
	{
		case TYPEID_PLAYER:
			{
				Player *pObj = static_cast< Player* >(obj);

				if(pObj->m_invisible) // Invisibility - Detection of Players
				{
					if(pObj->getDeathState() == CORPSE)
						return bGMTagOn; // only GM can see players that are spirits

					if(GetGroup() && pObj->GetGroup() == GetGroup() // can see invisible group members except when dueling them
							&& DuelingWith != pObj)
						return true;

					if(pObj->stalkedby == GetGUID()) // Hunter's Mark / MindVision is visible to the caster
						return true;

					if(m_invisDetect[INVIS_FLAG_NORMAL] < 1 // can't see invisible without proper detection
							|| pObj->m_isGmInvisible) // can't see invisible GM
						return bGMTagOn; // GM can see invisible players
				}

				if( m_invisible && pObj->m_invisDetect[m_invisFlag] < 1 ) // Invisible - can see those that detect, but not others
					return m_isGmInvisible;

				if(pObj->IsStealth()) // Stealth Detection (  I Hate Rogues :P  )
				{
					if(GetGroup() && pObj->GetGroup() == GetGroup() // can see stealthed group members except when dueling them
							&& DuelingWith != pObj)
						return true;

					if(pObj->stalkedby == GetGUID()) // Hunter's Mark / MindVision is visible to the caster
						return true;

					if(isInFront(pObj)) // stealthed player is in front of us
					{
						// Detection Range = 5yds + (Detection Skill - Stealth Skill)/5
						if(getLevel() < PLAYER_LEVEL_CAP)
							detectRange = 5.0f + getLevel() + 0.2f * (float)(GetStealthDetectBonus() - pObj->GetStealthLevel());
						else
							detectRange = 75.0f + 0.2f * (float)(GetStealthDetectBonus() - pObj->GetStealthLevel());
						// Hehe... stealth skill is increased by 5 each level and detection skill is increased by 5 each level too.
						// This way, a level 70 should easily be able to detect a level 4 rogue (level 4 because that's when you get stealth)
						//	detectRange += 0.2f * ( getLevel() - pObj->getLevel() );
						if(detectRange < 1.0f) detectRange = 1.0f; // Minimum Detection Range = 1yd
					}
					else // stealthed player is behind us
					{
						if(GetStealthDetectBonus() > 1000) return true; // immune to stealth
						else detectRange = 0.0f;
					}

					detectRange += GetFloatValue(UNIT_FIELD_BOUNDINGRADIUS); // adjust range for size of player
					detectRange += pObj->GetFloatValue(UNIT_FIELD_BOUNDINGRADIUS); // adjust range for size of stealthed player
					//sLog.outString( "Player::CanSee(%s): detect range = %f yards (%f ingame units), cansee = %s , distance = %f" , pObj->GetName() , detectRange , detectRange * detectRange , ( GetDistance2dSq(pObj) > detectRange * detectRange ) ? "yes" : "no" , GetDistanceSq(pObj) );
					if(GetDistanceSq(pObj) > detectRange * detectRange)
						return bGMTagOn; // GM can see stealthed players
				}

				return !pObj->m_isGmInvisible;
			}
		//------------------------------------------------------------------

		case TYPEID_UNIT:
			{
				Unit *uObj = static_cast<Unit *>(obj);

				if(uObj->IsSpiritHealer()) // can't see spirit-healers when alive
					return false;

				if(uObj->m_invisible // Invisibility - Detection of Units
						&& m_invisDetect[uObj->m_invisFlag] < 1) // can't see invisible without proper detection
					return bGMTagOn; // GM can see invisible units

				if( m_invisible && uObj->m_invisDetect[m_invisFlag] < 1 ) // Invisible - can see those that detect, but not others
					return m_isGmInvisible;

				return true;
			}
		//------------------------------------------------------------------

		case TYPEID_GAMEOBJECT:
			{
				GameObject *gObj = static_cast<GameObject *>(obj);

				if(gObj->invisible) // Invisibility - Detection of GameObjects
				{
					uint64 owner = gObj->GetUInt64Value(OBJECT_FIELD_CREATED_BY);

					if(GetGUID() == owner) // the owner of an object can always see it
						return true;

					if(GetGroup())
					{
						PlayerInfo * inf = objmgr.GetPlayerInfo((uint32)owner);
						if(inf && GetGroup()->HasMember(inf))
							return true;
					}

					if(m_invisDetect[gObj->invisibilityFlag] < 1) // can't see invisible without proper detection
						return bGMTagOn; // GM can see invisible objects
				}

				return true;
			}
		//------------------------------------------------------------------

		default:
			return true;
	}
}

void Player::AddInRangeObject(Object* pObj)
{
	//Send taxi move if we're on a taxi
	if (m_CurrentTaxiPath && (pObj->GetTypeId() == TYPEID_PLAYER))
	{
		uint32 ntime = getMSTime();

		if (ntime > m_taxi_ride_time)
			m_CurrentTaxiPath->SendMoveForTime( this, static_cast< Player* >( pObj ), ntime - m_taxi_ride_time);
		/*else
			m_CurrentTaxiPath->SendMoveForTime( this, static_cast< Player* >( pObj ), m_taxi_ride_time - ntime);*/
	}

	Unit::AddInRangeObject(pObj);

	//if the object is a unit send a move packet if they have a destination
	if(pObj->GetTypeId() == TYPEID_UNIT)
	{
		//add an event to send move update have to send guid as pointer was causing a crash :(
		//sEventMgr.AddEvent( static_cast< Creature* >( pObj )->GetAIInterface(), &AIInterface::SendCurrentMove, this->GetGUID(), EVENT_UNIT_SENDMOVE, 200, 1);
		static_cast< Creature* >( pObj )->GetAIInterface()->SendCurrentMove(this);

/*
	//unit based objects, send aura data
	if (pObj->IsUnit())
	{
		Unit* pUnit=static_cast<Unit*>(pObj);

		if (GetSession() != NULL)
		{
			for (uint32 i=0; i<MAX_TOTAL_AURAS_START; ++i)
			{
				if (pUnit->m_auras[i] != NULL)
				{
					uint8 flags = pUnit->m_auras[i]->IsPositive()? 0x1F : 0x09;
					if (pUnit->m_auras[i]->GetDuration() > 0)
						flags |= 0x20;
					WorldPacket data(SMSG_AURA_UPDATE, 28);
					FastGUIDPack(data, pUnit->GetGUID());
					data << pUnit->m_auras[i]->m_visualSlot; //flags unknown
					data << pUnit->m_auras[i]->m_spellProto->Id;
					data << pUnit->m_auras[i]->m_spellProto->maxstack;
					data << flags; //flags2 unknown
					if (flags & 0x20)
					{
						data << pUnit->m_auras[i]->GetDuration();
						data << pUnit->m_auras[i]->GetTimeLeft();
					}
					GetSession()->SendPacket(&data);
				}
			}
		}
	}*/
}
}
void Player::OnRemoveInRangeObject(Object* pObj)
{
	//if (/*!CanSee(pObj) && */IsVisible(pObj))
	//{
		//RemoveVisibleObject(pObj);
	//}

	//object was deleted before reaching here
	if (pObj == NULL)
		return;

	m_visibleObjects.erase(pObj);
	Unit::OnRemoveInRangeObject(pObj);

	if( pObj->GetGUID() == m_CurrentCharm )
	{
		Unit *p =GetMapMgr()->GetUnit( m_CurrentCharm );
		if( !p )
			return;

		UnPossess();
		if(m_currentSpell)
			m_currentSpell->cancel();	   // cancel the spell
		m_CurrentCharm=0;

		if( p->m_temp_summon && p->GetTypeId() == TYPEID_UNIT )
			static_cast< Creature* >( p )->SafeDelete();
	}

	if( m_Summon && pObj == m_Summon )
		DismissActivePet();

	if( pObj->GetGUID() == GetUInt64Value( UNIT_FIELD_SUMMON ) )
		RemoveFieldSummon();

	/* wehee loop unrolling */
/*	if(m_spellTypeTargets[0] == pObj)
		m_spellTypeTargets[0] = NULL;
	if(m_spellTypeTargets[1] == pObj)
		m_spellTypeTargets[1] = NULL;
	if(m_spellTypeTargets[2] == pObj)
		m_spellTypeTargets[2] = NULL;*/
	if(pObj->IsUnit())
	{
		for(uint32 x = 0; x < NUM_SPELL_TYPE_INDEX; ++x)
			if(m_spellIndexTypeTargets[x] == pObj->GetGUID())
				m_spellIndexTypeTargets[x] = 0;
	}
}

void Player::ClearInRangeSet()
{
	m_visibleObjects.clear();
	Unit::ClearInRangeSet();
}

void Player::EventCannibalize(uint32 amount)
{
	uint32 amt = (GetUInt32Value(UNIT_FIELD_MAXHEALTH)*amount)/100;

	uint32 newHealth = GetUInt32Value(UNIT_FIELD_HEALTH) + amt;

	if(newHealth <= GetUInt32Value(UNIT_FIELD_MAXHEALTH))
		SetUInt32Value(UNIT_FIELD_HEALTH, newHealth);
	else
		SetUInt32Value(UNIT_FIELD_HEALTH, GetUInt32Value(UNIT_FIELD_MAXHEALTH));

	cannibalizeCount++;
	if(cannibalizeCount == 5)
		SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);

	WorldPacket data(SMSG_PERIODICAURALOG, 38);
	data << GetNewGUID();				   // caster guid
	data << GetNewGUID();				   // target guid
	data << (uint32)(20577);				// spellid
	data << (uint32)1;					  // unknown?? need resource?
	data << (uint32)FLAG_PERIODIC_HEAL;		// aura school
	data << amt;							// amount of done to target / heal / damage
	data << (uint32)0;					  // unknown in some sniff this was 0x0F
	SendMessageToSet(&data, true);
}

void Player::EventReduceDrunk(bool full)
{
	uint8 drunk = ((GetUInt32Value(PLAYER_BYTES_3) >> 8) & 0xFF);
	if(full) drunk = 0;
	else drunk -= 10;
	SetUInt32Value(PLAYER_BYTES_3, ((GetUInt32Value(PLAYER_BYTES_3) & 0xFFFF00FF) | (drunk << 8)));
	if(drunk == 0) sEventMgr.RemoveEvents(this, EVENT_PLAYER_REDUCEDRUNK);
}

void Player::LoadTaxiMask(const char* data)
{
	vector<string> tokens = StrSplit(data, " ");

	int index;
	vector<string>::iterator iter;

	for (iter = tokens.begin(), index = 0;
		(index < 12) && (iter != tokens.end()); ++iter, ++index)
	{
		m_taximask[index] = atol((*iter).c_str());
	}
}

bool Player::HasQuestForItem(uint32 itemid)
{
	Quest *qst;
	for( uint32 i = 0; i < 25; ++i )
	{
		if( m_questlog[i] != NULL )
		{
			qst = m_questlog[i]->GetQuest();

			// Check the item_quest_association table for an entry related to this item
			QuestAssociationList *tempList = QuestMgr::getSingleton().GetQuestAssociationListForItemId( itemid );
			if( tempList != NULL )
			{
				QuestAssociationList::iterator it;
				for (it = tempList->begin(); it != tempList->end(); ++it)
				{
					if ( ((*it)->qst == qst) && (GetItemInterface()->GetItemCount( itemid ) < (*it)->item_count) )
					{
						return true;
					} // end if
				} // end for
			} // end if

			// No item_quest association found, check the quest requirements
			if( !qst->count_required_item )
				continue;

			for( uint32 j = 0; j < 4; ++j )
				if( qst->required_item[j] == itemid && ( GetItemInterface()->GetItemCount( itemid ) < qst->required_itemcount[j] ) )
					return true;
		}
	}
	return false;
}


//scriptdev2
bool Player::HasItemCount( uint32 item, uint32 count, bool inBankAlso ) const
{
	return (m_ItemInterface->GetItemCount(item, inBankAlso) == count);
}


/*Loot type MUST be
1-corpse, go
2-skinning/herbalism/minning
3-Fishing
*/
void Player::SendLoot(uint64 guid,uint8 loot_type)
{
	Group * m_Group = m_playerInfo->m_Group;

	if( !IsInWorld() )
		return;

	Loot * pLoot = NULL;
	uint32 guidtype = GET_TYPE_FROM_GUID(guid);
	int8 loot_method = -1;

	if(guidtype == HIGHGUID_TYPE_UNIT)
	{
		Creature* pCreature = GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
		if(!pCreature)return;
		pLoot=&pCreature->loot;
		m_currentLoot = pCreature->GetGUID();
		loot_method = pCreature->m_lootMethod;
		if ( loot_method < 0 )
		{
			loot_method = PARTY_LOOT_FFA;
			pCreature->m_lootMethod = PARTY_LOOT_FFA;
		}
	}else if(guidtype == HIGHGUID_TYPE_GAMEOBJECT)
	{
		GameObject* pGO = GetMapMgr()->GetGameObject(GET_LOWGUID_PART(guid));
		if(!pGO)return;
		pGO->SetByte(GAMEOBJECT_BYTES_1, 0,0);
		pLoot=&pGO->loot;
		m_currentLoot = pGO->GetGUID();
	}
	else if((guidtype == HIGHGUID_TYPE_PLAYER) )
	{
		Player *p=GetMapMgr()->GetPlayer((uint32)guid);
		if(!p)return;
		pLoot=&p->loot;
		m_currentLoot = p->GetGUID();
	}
	else if( (guidtype == HIGHGUID_TYPE_CORPSE))
	{
		Corpse *pCorpse = objmgr.GetCorpse((uint32)guid);
		if(!pCorpse)return;
		pLoot=&pCorpse->loot;
		m_currentLoot = pCorpse->GetGUID();
	}
	else if( (guidtype == HIGHGUID_TYPE_ITEM) )
	{
		Item *pItem = GetItemInterface()->GetItemByGUID(guid);
		if(!pItem)
			return;
		pLoot = pItem->loot;
		m_currentLoot = pItem->GetGUID();
	}

	if(!pLoot)
	{
		// something whack happened.. damn cheaters..
		return;
	}

	// add to looter set
	pLoot->looters.insert(GetLowGUID());

	WorldPacket data, data2(28);
	data.SetOpcode (SMSG_LOOT_RESPONSE);


	m_lootGuid = guid;


	data << guid;
	data << loot_type;//loot_type;
	data << pLoot->gold;
	data << (uint8) 0;//loot size reserve


	std::vector<__LootItem>::iterator iter=pLoot->items.begin();
	uint32 count=0;
	uint8 slottype = 0;

	for(uint32 x=0;iter!=pLoot->items.end();iter++,x++)
	{
		if (iter->iItemsCount == 0)
			continue;

		LooterSet::iterator itr = iter->has_looted.find(GetLowGUID());
		if (iter->has_looted.end() != itr)
			continue;

		ItemPrototype* itemProto =iter->item.itemproto;
		if (!itemProto)
			continue;

		// check if it's on ML if so only quest items and ffa loot should be shown based on mob
		if ( loot_method == PARTY_LOOT_MASTER && m_Group && m_Group->GetLooter() != m_playerInfo )
			// pass on all ffa_loot and the grey / white items
			if ( !iter->ffa_loot && !(itemProto->Quality < m_Group->GetThreshold()) )
				continue;

		//quest items check. type 4/5
        //quest items that dont start quests.
        if((itemProto->Bonding == ITEM_BIND_QUEST) && !(itemProto->QuestId) && !HasQuestForItem(itemProto->ItemId))
            continue;
        if((itemProto->Bonding == ITEM_BIND_QUEST2) && !(itemProto->QuestId) && !HasQuestForItem(itemProto->ItemId))
            continue;

        //quest items that start quests need special check to avoid drops all the time.
        if((itemProto->Bonding == ITEM_BIND_QUEST) && (itemProto->QuestId) && GetQuestLogForEntry(itemProto->QuestId))
            continue;
        if((itemProto->Bonding == ITEM_BIND_QUEST2) && (itemProto->QuestId) && GetQuestLogForEntry(itemProto->QuestId))
            continue;

        if((itemProto->Bonding == ITEM_BIND_QUEST) && (itemProto->QuestId) && HasFinishedQuest(itemProto->QuestId))
            continue;
        if((itemProto->Bonding == ITEM_BIND_QUEST2) && (itemProto->QuestId) && HasFinishedQuest(itemProto->QuestId))
            continue;

        //check for starting item quests that need questlines.
        if((itemProto->QuestId && itemProto->Bonding != ITEM_BIND_QUEST && itemProto->Bonding != ITEM_BIND_QUEST2))
        {
            bool HasRequiredQuests = true;
            Quest * pQuest = QuestStorage.LookupEntry(itemProto->QuestId);
            if(pQuest)
            {
				uint32 finishedCount = 0;

                //check if its a questline.
                for(uint32 i = 0; i < pQuest->count_requiredquests; i++)
                {
                    if(pQuest->required_quests[i])
                    {
                        if(!HasFinishedQuest(pQuest->required_quests[i]) || GetQuestLogForEntry(pQuest->required_quests[i]))
                        {
							if (!(pQuest->quest_flags & QUEST_FLAG_ONLY_ONE_REQUIRED)) {
								HasRequiredQuests = false;
								break;
							}
						}
						else
						{
							finishedCount++;
						}
                    }
                }

				if (pQuest->quest_flags & QUEST_FLAG_ONLY_ONE_REQUIRED) {
					if (finishedCount == 0) continue;
				} else {
	                if(!HasRequiredQuests)
    	                continue;
				}
            }
        }

		slottype = 0;
		if(m_Group != NULL && loot_type < 2)
		{
			switch(loot_method)
			{
			case PARTY_LOOT_MASTER:
				slottype = 2;
				break;
			case PARTY_LOOT_GROUP:
			case PARTY_LOOT_RR:
			case PARTY_LOOT_NBG:
				slottype = 1;
				break;
			default:
				slottype = 0;
				break;
			}
			// only quality items are distributed
			if(itemProto->Quality < m_Group->GetThreshold())
			{
				slottype = 0;
			}

			/* if all people passed anyone can loot it? :P */
			if(iter->passed)
				slottype = 0;					// All players passed on the loot

			//if it is ffa loot and not an masterlooter
			if(iter->ffa_loot)
				slottype = 0;
		}

		data << uint8(x);
		data << uint32(itemProto->ItemId);
		data << uint32(iter->iItemsCount);//nr of items of this type
		data << uint32(iter->item.displayid);
		//data << uint32(iter->iRandomSuffix ? iter->iRandomSuffix->id : 0);
		//data << uint32(iter->iRandomProperty ? iter->iRandomProperty->ID : 0);
		if(iter->iRandomSuffix)
		{
			data << Item::GenerateRandomSuffixFactor(itemProto);
			data << uint32(-int32(iter->iRandomSuffix->id));
		}
		else if(iter->iRandomProperty)
		{
			data << uint32(0);
			data << uint32(iter->iRandomProperty->ID);
		}
		else
		{
			data << uint32(0);
			data << uint32(0);
		}

		data << slottype;   // "still being rolled for" flag

		if(slottype == 1)
		{
			if(iter->roll == NULL && !iter->passed)
			{
				int32 ipid = 0;
				uint32 factor=0;
				if(iter->iRandomProperty)
					ipid=iter->iRandomProperty->ID;
				else if(iter->iRandomSuffix)
				{
					ipid = -int32(iter->iRandomSuffix->id);
					factor=Item::GenerateRandomSuffixFactor(iter->item.itemproto);
				}

				if(iter->item.itemproto)
				{
					iter->roll = new LootRoll(60000, (m_Group != NULL ? m_Group->MemberCount() : 1),  guid, x, itemProto->ItemId, factor, uint32(ipid), GetMapMgr());

					data2.Initialize(SMSG_LOOT_START_ROLL);
					data2 << guid;
					data2 << x;
					data2 << uint32(itemProto->ItemId);
					data2 << uint32(factor);
					if(iter->iRandomProperty)
						data2 << uint32(iter->iRandomProperty->ID);
					else if(iter->iRandomSuffix)
						data2 << uint32(ipid);
					else
						data2 << uint32(0);

					data2 << uint32(60000); // countdown
				}

				Group * pGroup = m_playerInfo->m_Group;
				if(pGroup)
				{
					pGroup->Lock();
					for(uint32 i = 0; i < pGroup->GetSubGroupCount(); ++i)
					{
						for(GroupMembersSet::iterator itr = pGroup->GetSubGroup(i)->GetGroupMembersBegin(); itr != pGroup->GetSubGroup(i)->GetGroupMembersEnd(); ++itr)
						{
							if((*itr)->m_loggedInPlayer && (*itr)->m_loggedInPlayer->GetItemInterface()->CanReceiveItem(itemProto, iter->iItemsCount) == 0)
							{
								if( (*itr)->m_loggedInPlayer->m_passOnLoot )
									iter->roll->PlayerRolled( (*itr)->m_loggedInPlayer, 3 );		// passed
								else
									(*itr)->m_loggedInPlayer->GetSession()->SendPacket(&data2);
							}
						}
					}
					pGroup->Unlock();
				}
				else
				{
					GetSession()->SendPacket(&data2);
				}
			}
		}
		count++;
	}
	data.wpos (13);
	data << (uint8)count;

	GetSession ()->SendPacket(&data);
	SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOOTING);
}

void Player::EventAllowTiggerPort(bool enable)
{
	m_AllowAreaTriggerPort = enable;
}

uint32 Player::CalcTalentResetCost(uint32 resetnum)
{

	if(resetnum ==0 )
		return  10000;
	else
	{
		if(resetnum>10)
		return  500000;
		else return resetnum*50000;
	}
}

void Player::SendTalentResetConfirm()
{
	WorldPacket data(MSG_TALENT_WIPE_CONFIRM, 12);
	data << GetGUID();
	data << CalcTalentResetCost(GetTalentResetTimes());
	GetSession()->SendPacket(&data);
}
void Player::SendPetUntrainConfirm()
{
	Pet* pPet = GetSummon();
	if( pPet == NULL )
		return;
	WorldPacket data( SMSG_PET_UNLEARN_CONFIRM, 12 );
	data << pPet->GetGUID();
	data << pPet->GetUntrainCost();
	GetSession()->SendPacket( &data );
}

int32 Player::CanShootRangedWeapon( uint32 spellid, Unit* target, bool autoshot )
{
	SpellEntry* spellinfo = dbcSpell.LookupEntry( spellid );

	if( spellinfo == NULL )
		return -1;
	//sLog.outString( "Canshootwithrangedweapon!?!? spell: [%u] %s" , spellinfo->Id , spellinfo->Name );

	// Check if Morphed
	if( polySpell > 0 )
		return SPELL_FAILED_NOT_SHAPESHIFT;

	// Check ammo
	Item* itm = GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_RANGED );
	ItemPrototype * iprot = ItemPrototypeStorage.LookupEntry(GetUInt32Value(PLAYER_AMMO_ID));
	if( !m_requiresNoAmmo )
	{
		if( itm == NULL )
			return SPELL_FAILED_NO_AMMO;
	
		// Check ammo level
		if( iprot && getLevel()< iprot->RequiredLevel)
			return SPELL_FAILED_LOWLEVEL;

		// Check ammo type
		ItemPrototype * iprot1 = ItemPrototypeStorage.LookupEntry(itm->GetEntry());
		if( iprot && iprot1 && iprot->SubClass != iprot1->AmmoType )
			return SPELL_FAILED_NEED_AMMO;
	}

	// Player has clicked off target. Fail spell.
	if( m_curSelection != m_AutoShotTarget )
		return SPELL_FAILED_INTERRUPTED;

	// Check if target is already dead
	if( target->IsDead() )
		return SPELL_FAILED_TARGETS_DEAD;

	// Check if in line of sight (need collision detection).
	if (sWorld.Collision) {
		if (GetMapId() == target->GetMapId() && !CollideInterface.CheckLOS(GetMapId(),GetPositionNC(),target->GetPositionNC()))
			return SPELL_FAILED_LINE_OF_SIGHT;
	}

	// Check if we aren't casting another spell allready
	if( GetCurrentSpell() )
		return -1;

	// Supalosa - The hunter ability Auto Shot is using Shoot range, which is 5 yards shorter.
	// So we'll use 114, which is the correct 35 yard range used by the other Hunter abilities (arcane shot, concussive shot...)
	uint8 fail = 0;
	uint32 rIndex = autoshot ? 114 : spellinfo->rangeIndex;
	SpellRange* range = dbcSpellRange.LookupEntry( rIndex );
	float minrange = GetMinRange( range );
	float dist = CalcDistance( this, target );
	float maxr = GetMaxRange( range ) + 2.52f;

	if( spellinfo->SpellGroupType )
	{
		SM_FFValue( this->SM_FRange, &maxr, spellinfo->SpellGroupType );
		SM_PFValue( this->SM_PRange, &maxr, spellinfo->SpellGroupType );
#ifdef COLLECTION_OF_UNTESTED_STUFF_AND_TESTERS
		float spell_flat_modifers=0;
		float spell_pct_modifers=0;
		SM_FFValue(this->SM_FRange,&spell_flat_modifers,spellinfo->SpellGroupType);
		SM_FFValue(this->SM_PRange,&spell_pct_modifers,spellinfo->SpellGroupType);
		if(spell_flat_modifers!=0 || spell_pct_modifers!=0)
			printf("!!!!!spell range bonus mod flat %f , spell range bonus pct %f , spell range %f, spell group %u\n",spell_flat_modifers,spell_pct_modifers,maxr,spellinfo->SpellGroupType);
#endif
	}

	//float bonusRange = 0;
	// another hackfix: bonus range from hunter talent hawk eye: +2/4/6 yard range to ranged weapons
	//if(autoshot)
	//SM_FFValue( SM_FRange, &bonusRange, dbcSpell.LookupEntry( 75 )->SpellGroupType ); // HORRIBLE hackfixes :P
	// Partha: +2.52yds to max range, this matches the range the client is calculating.
	// see extra/supalosa_range_research.txt for more info
	//bonusRange = 2.52f;
	//sLog.outString( "Bonus range = %f" , bonusRange );

	// check if facing target
	if(!isInFront(target))
	{
		fail = SPELL_FAILED_UNIT_NOT_INFRONT;
	}

	// Check ammo count
	if( !m_requiresNoAmmo && iprot && itm->GetProto()->SubClass != ITEM_SUBCLASS_WEAPON_WAND )
	{
		uint32 ammocount = GetItemInterface()->GetItemCount(iprot->ItemId);
		if(ammocount == 0)
			fail = SPELL_FAILED_NO_AMMO;
	}

	// Check for too close
	if( spellid != SPELL_RANGED_WAND )//no min limit for wands
		if( minrange > dist )
			fail = SPELL_FAILED_TOO_CLOSE;
	if(sWorld.Collision && GetMapId() == target->GetMapId() && !CollideInterface.CheckLOS(GetMapId(),GetPositionNC(),target->GetPositionNC()))
		fail = SPELL_FAILED_LINE_OF_SIGHT ;
	if( dist > maxr )
	{
		//	sLog.outString( "Auto shot failed: out of range (Maxr: %f, Dist: %f)" , maxr , dist );
		fail = SPELL_FAILED_OUT_OF_RANGE;
	}

	if( spellid == SPELL_RANGED_THROW )
	{
		if( itm != NULL ) // no need for this
			if( itm->GetProto() )
				if( GetItemInterface()->GetItemCount( itm->GetProto()->ItemId ) == 0 )
					fail = SPELL_FAILED_NO_AMMO;
	}
/*  else
	{
		if(GetUInt32Value(PLAYER_AMMO_ID))//for wand
			if(this->GetItemInterface()->GetItemCount(GetUInt32Value(PLAYER_AMMO_ID)) == 0)
				fail = SPELL_FAILED_NO_AMMO;
	}
*/
	if( fail > 0 )// && fail != SPELL_FAILED_OUT_OF_RANGE)
	{
		//SendCastResult( autoshot ? 75 : spellid, fail, 0, 0 );
		packetSMSG_CASTRESULT cr;
		cr.SpellId = autoshot ? 75 : spellid;
		cr.ErrorMessage = fail;
		cr.MultiCast = 0;
		m_session->OutPacket( SMSG_CAST_FAILED, sizeof(packetSMSG_CASTRESULT), &cr );
		if( fail != SPELL_FAILED_OUT_OF_RANGE )
		{
			uint32 spellid2 = autoshot ? 75 : spellid;
			m_session->OutPacket( SMSG_CANCEL_AUTO_REPEAT, 4, &spellid2 );
		}
		//sLog.outString( "Result for CanShootWIthRangedWeapon: %u" , fail );
		//sLog.outDebug( "Can't shoot with ranged weapon: %u (Timer: %u)" , fail , m_AutoShotAttackTimer );
		return fail;
	}

	return 0;
}

void Player::EventRepeatSpell()
{
	if( !m_curSelection || !IsInWorld() )
		return;

	Unit* target = GetMapMgr()->GetUnit( m_curSelection );
	if( target == NULL )
	{
		m_AutoShotAttackTimer = 0; //avoid flooding client with error mesages
		m_onAutoShot = false;
		//sLog.outDebug( "Can't cast Autoshot: Target changed! (Timer: %u)" , m_AutoShotAttackTimer );
		return;
	}

	m_AutoShotDuration = m_uint32Values[UNIT_FIELD_RANGEDATTACKTIME];

	if( m_isMoving )
	{
		//sLog.outDebug( "HUNTER AUTOSHOT 2) %i, %i", m_AutoShotAttackTimer, m_AutoShotDuration );
		//m_AutoShotAttackTimer = m_AutoShotDuration;//avoid flooding client with error mesages
		//sLog.outDebug( "Can't cast Autoshot: You're moving! (Timer: %u)" , m_AutoShotAttackTimer );
		m_AutoShotAttackTimer = 100; // shoot when we can
		return;
	}

	int32 f = this->CanShootRangedWeapon( m_AutoShotSpell->Id, target, true );

	if( f != 0 )
	{
		if( f != SPELL_FAILED_OUT_OF_RANGE )
		{
			m_AutoShotAttackTimer = 0;
			m_onAutoShot=false;
		}
		else if( m_isMoving )
		{
			m_AutoShotAttackTimer = 100;
		}
		else
		{
			m_AutoShotAttackTimer = m_AutoShotDuration;//avoid flooding client with error mesages
		}
		return;
	}
	else
	{
		m_AutoShotAttackTimer = m_AutoShotDuration;

		Spell * sp = SpellPool.PooledNew();
		if (!sp)
			return;
		sp->Init(this, m_AutoShotSpell, true, NULL);
		SpellCastTargets tgt;
		tgt.m_unitTarget = m_curSelection;
		tgt.m_targetMask = TARGET_FLAG_UNIT;
		sp->prepare( &tgt );
	}
}

bool Player::HasSpell(uint32 spell)
{
	return mSpells.find(spell) != mSpells.end();
}

bool Player::HasSpellwithNameHash(uint32 hash)
{
	SpellSet::iterator it,iter;
	for(iter= mSpells.begin();iter != mSpells.end();)
	{
		it = iter++;
		uint32 SpellID = *it;
		SpellEntry *e = dbcSpell.LookupEntry(SpellID);
		if(e->NameHash == hash)
			return true;
	}
	return false;
}

bool Player::HasDeletedSpell(uint32 spell)
{
	return (mDeletedSpells.count(spell) > 0);
}

void Player::removeSpellByHashName(uint32 hash)
{
	SpellSet::iterator it,iter;

	for(iter= mSpells.begin();iter != mSpells.end();)
	{
		it = iter++;
		uint32 SpellID = *it;
		SpellEntry *e = dbcSpell.LookupEntry(SpellID);
		if(e->NameHash == hash)
		{
			if(info->spell_list.find(e->Id) != info->spell_list.end())
				continue;

			RemoveAura(SpellID,GetGUID());
#ifdef USING_BIG_ENDIAN
			uint32 swapped = swap32(SpellID);
			m_session->OutPacket(SMSG_REMOVED_SPELL, 4, &swapped);
#else
			m_session->OutPacket(SMSG_REMOVED_SPELL, 4, &SpellID);
#endif
			mSpells.erase(it);
		}
	}

	for(iter= mDeletedSpells.begin();iter != mDeletedSpells.end();)
	{
		it = iter++;
		uint32 SpellID = *it;
		SpellEntry *e = dbcSpell.LookupEntry(SpellID);
		if(e->NameHash == hash)
		{
			if(info->spell_list.find(e->Id) != info->spell_list.end())
				continue;

			RemoveAura(SpellID,GetGUID());
#ifdef USING_BIG_ENDIAN
			uint32 swapped = swap32(SpellID);
			m_session->OutPacket(SMSG_REMOVED_SPELL, 4, &swapped);
#else
			m_session->OutPacket(SMSG_REMOVED_SPELL, 4, &SpellID);
#endif
			mDeletedSpells.erase(it);
		}
	}
}

bool Player::removeSpell(uint32 SpellID, bool MoveToDeleted, bool SupercededSpell, uint32 SupercededSpellID)
{
	SpellSet::iterator it,iter = mSpells.find(SpellID);
	if(iter != mSpells.end())
	{
		mSpells.erase(iter);
		RemoveAura(SpellID,GetGUID());
	}
	else
	{
   iter = mDeletedSpells.find(SpellID);
    if(iter != mDeletedSpells.end())
    {
			mDeletedSpells.erase(iter);
	  }
    else
    {
	    return false;
    }
	}

	if(MoveToDeleted)
		mDeletedSpells.insert(SpellID);

	if(!IsInWorld())
		return true;

	if(SupercededSpell)
	{
		WorldPacket data(SMSG_SUPERCEDED_SPELL, 8);
		data << SpellID << SupercededSpellID;
		m_session->SendPacket(&data);
	}
	else
	{
#ifdef USING_BIG_ENDIAN
		uint32 swapped = swap32(SpellID);
		m_session->OutPacket(SMSG_REMOVED_SPELL, 4, &swapped);
#else
		m_session->OutPacket(SMSG_REMOVED_SPELL, 4, &SpellID);
#endif
	}

	return true;
}

bool Player::removeDeletedSpell( uint32 SpellID )
{
	SpellSet::iterator it = mDeletedSpells.find( SpellID );
	if ( it == mDeletedSpells.end() )
		return false;

	mDeletedSpells.erase( it );
	return true;
}

void Player::EventActivateGameObject(GameObject* obj)
{
	obj->BuildFieldUpdatePacket(this, GAMEOBJECT_DYNAMIC, 1 | 8);
}

void Player::EventDeActivateGameObject(GameObject* obj)
{
	obj->BuildFieldUpdatePacket(this, GAMEOBJECT_DYNAMIC, 0);
}

void Player::EventTimedQuestExpire(Quest *qst, QuestLogEntry *qle, uint32 log_slot)
{
	WorldPacket fail;
	sQuestMgr.BuildQuestFailed(&fail, qst->id);
	GetSession()->SendPacket(&fail);
	CALL_QUESTSCRIPT_EVENT(qle, OnQuestCancel)(this);
	qle->Finish();
}

//scriptdev2

void Player::AreaExploredOrEventHappens( uint32 questId )
{
	sQuestMgr.AreaExplored(this,questId);
}

void Player::SendInitialLogonPackets()
{
	// Initial Packets... they seem to be re-sent on port.
	m_session->OutPacket(SMSG_SET_REST_START_OBSOLETE, 4, &m_timeLogoff);

    StackWorldPacket<32> data(SMSG_BINDPOINTUPDATE);
    data << m_bind_pos_x;
    data << m_bind_pos_y;
    data << m_bind_pos_z;
    data << m_bind_mapid;
    data << m_bind_zoneid;
    GetSession()->SendPacket( &data );

	//Proficiencies
    //SendSetProficiency(4,armor_proficiency);
    //SendSetProficiency(2,weapon_proficiency);
	packetSMSG_SET_PROFICICENCY pr;
	pr.ItemClass = 4;
	pr.Profinciency = armor_proficiency;
	m_session->OutPacket( SMSG_SET_PROFICIENCY, sizeof(packetSMSG_SET_PROFICICENCY), &pr );
	pr.ItemClass = 2;
	pr.Profinciency = weapon_proficiency;
	m_session->OutPacket( SMSG_SET_PROFICIENCY, sizeof(packetSMSG_SET_PROFICICENCY), &pr );

	//Tutorial Flags
	data.Initialize( SMSG_TUTORIAL_FLAGS );
	for (int i = 0; i < 8; i++)
		data << uint32( m_Tutorials[i] );
	GetSession()->SendPacket(&data);

	//Initial Spells
	smsg_InitialSpells();

	//Initial Actions
	SendInitialActions();

	//Factions
	smsg_InitialFactions();


    /* Some minor documentation about the time field
    // MOVE THIS DOCUMENTATION TO THE WIKI

    minute's = 0x0000003F                  00000000000000000000000000111111
    hour's   = 0x000007C0                  00000000000000000000011111000000
    weekdays = 0x00003800                  00000000000000000011100000000000
    days     = 0x000FC000                  00000000000011111100000000000000
    months   = 0x00F00000                  00000000111100000000000000000000
    years    = 0x1F000000                  00011111000000000000000000000000
    unk	     = 0xE0000000                  11100000000000000000000000000000
    */

	data.Initialize(SMSG_LOGIN_SETTIMESPEED);
	time_t minutes = sWorld.GetGameTime( ) / 60;
	time_t hours = minutes / 60; minutes %= 60;
    time_t gameTime = 0;

    // TODO: Add stuff to handle these variable's

	time_t basetime = UNIXTIME;
	uint32 DayOfTheWeek;
	if(localtime(&basetime)->tm_wday == 0)
		DayOfTheWeek = 6;
	else
		   DayOfTheWeek = localtime(&basetime)->tm_wday - 1;
	uint32 DayOfTheMonth = localtime(&basetime)->tm_mday - 1;
	uint32 CurrentMonth = localtime(&basetime)->tm_mon;
	uint32 CurrentYear = localtime(&basetime)->tm_year - 100;

	#define MINUTE_BITMASK      0x0000003F
    #define HOUR_BITMASK        0x000007C0
    #define WEEKDAY_BITMASK     0x00003800
    #define DAY_BITMASK         0x000FC000
    #define MONTH_BITMASK       0x00F00000
    #define YEAR_BITMASK        0x1F000000
    #define UNK_BITMASK         0xE0000000

    #define MINUTE_SHIFTMASK    0
    #define HOUR_SHIFTMASK      6
    #define WEEKDAY_SHIFTMASK   11
    #define DAY_SHIFTMASK       14
    #define MONTH_SHIFTMASK     20
    #define YEAR_SHIFTMASK      24
    #define UNK_SHIFTMASK       29

    gameTime = ((minutes << MINUTE_SHIFTMASK) & MINUTE_BITMASK);
    gameTime|= ((hours << HOUR_SHIFTMASK) & HOUR_BITMASK);
    gameTime|= ((DayOfTheWeek << WEEKDAY_SHIFTMASK) & WEEKDAY_BITMASK);
    gameTime|= ((DayOfTheMonth << DAY_SHIFTMASK) & DAY_BITMASK);
    gameTime|= ((CurrentMonth << MONTH_SHIFTMASK) & MONTH_BITMASK);
    gameTime|= ((CurrentYear << YEAR_SHIFTMASK) & YEAR_BITMASK);

    data << (uint32)gameTime;
	data << (float)0.0166666669777748f;  // Normal Game Speed
	GetSession()->SendPacket( &data );

	// cebernic for speedhack bug
	m_lastRunSpeed = 0;
	UpdateSpeed();

    WorldPacket ArenaSettings(SMSG_UPDATE_WORLD_STATE, 16);
    ArenaSettings << uint32(0xC77) << uint32(sWorld.Arena_Progress);
    ArenaSettings << uint32(0xF3D) << uint32(sWorld.Arena_Season);
    GetSession()->SendPacket( &ArenaSettings );

	sLog.outDetail("WORLD: Sent initial logon packets for %s.", GetName());
}

void Player::Reset_Spells()
{
	PlayerCreateInfo *info = objmgr.GetPlayerCreateInfo(getRace(), getClass());
	ASSERT(info);

	std::list<uint32> spelllist;

	for(SpellSet::iterator itr = mSpells.begin(); itr!=mSpells.end(); itr++)
	{
		spelllist.push_back((*itr));
	}

	for(std::list<uint32>::iterator itr = spelllist.begin(); itr!=spelllist.end(); itr++)
	{
		removeSpell((*itr), false, false, 0);
	}

	for(std::set<uint32>::iterator sp = info->spell_list.begin();sp!=info->spell_list.end();sp++)
	{
		if(*sp)
		{
			addSpell(*sp);
		}
	}

	// cebernic ResetAll ? don't forget DeletedSpells
  mDeletedSpells.clear();
}

void Player::Reset_Talents()
{
	unsigned int numRows = dbcTalent.GetNumRows();
	for (unsigned int i = 0; i < numRows; i++)		  // Loop through all talents.
	{
		TalentEntry *tmpTalent = dbcTalent.LookupRow(i);
		if(!tmpTalent)
			continue; //should not ocur
		//this is a normal talent (i hope )
		for (int j = 0; j < 5; j++)
		{
			if (tmpTalent->RankID[j] != 0)
			{
				SpellEntry *spellInfo;
				spellInfo = dbcSpell.LookupEntry( tmpTalent->RankID[j] );
				if(spellInfo)
				{
					if (spellInfo->NameHash == SPELL_HASH_DUAL_WIELD)
					{
						if (getClass() != SHAMAN)
							continue; //only shamans should lose it when they reset talents - opti
						if( _HasSkillLine( SKILL_DUAL_WIELD ) )
							_RemoveSkillLine( SKILL_DUAL_WIELD );
					}
					for(int k=0;k<3;k++)
						if(spellInfo->Effect[k] == SPELL_EFFECT_LEARN_SPELL)
						{
							//removeSpell(spellInfo->EffectTriggerSpell[k], false, 0, 0);
							//remove higher ranks of this spell too (like earth shield lvl 1 is talent and the rest is thought from trainer)
							SpellEntry *spellInfo2;
							spellInfo2 = dbcSpell.LookupEntry( spellInfo->EffectTriggerSpell[k] );
							if(spellInfo2)
								removeSpellByHashName(spellInfo2->NameHash);
						}
					//remove them all in 1 shot
					removeSpellByHashName(spellInfo->NameHash);
				}
			}
			else
				break;
		}
	}
	uint32 l=getLevel();
	if(l>9)
	{
		SetUInt32Value(PLAYER_CHARACTER_POINTS1, l - 9);
	}
	else
	{
		SetUInt32Value(PLAYER_CHARACTER_POINTS1, 0);
	}

}

void Player::Reset_ToLevel1()
{
	RemoveAllAuras();

	SetUInt32Value(UNIT_FIELD_LEVEL, 1);
	PlayerCreateInfo *info = objmgr.GetPlayerCreateInfo(getRace(), getClass());
	ASSERT(info);

	SetUInt32Value(UNIT_FIELD_HEALTH, info->health);
	SetPower(POWER_TYPE_MANA, info->mana );
	SetPower(POWER_TYPE_RAGE, 0);
	SetPower(POWER_TYPE_RUNIC_POWER, 0);
	SetUInt32Value(UNIT_FIELD_POWER3, info->focus );
	SetUInt32Value(UNIT_FIELD_POWER4, info->energy );
	SetUInt32Value(UNIT_FIELD_MAXHEALTH, info->health);
	SetUInt32Value(UNIT_FIELD_BASE_HEALTH, info->health);
	SetUInt32Value(UNIT_FIELD_BASE_MANA, info->mana);
	SetUInt32Value(UNIT_FIELD_MAXPOWER1, info->mana );
	SetUInt32Value(UNIT_FIELD_MAXPOWER2, info->rage );
	SetUInt32Value(UNIT_FIELD_MAXPOWER3, info->focus );
	SetUInt32Value(UNIT_FIELD_MAXPOWER4, info->energy );
	SetUInt32Value(UNIT_FIELD_MAXPOWER7, 1000 );
	SetUInt32Value(UNIT_FIELD_STAT0, info->strength );
	SetUInt32Value(UNIT_FIELD_STAT1, info->ability );
	SetUInt32Value(UNIT_FIELD_STAT2, info->stamina );
	SetUInt32Value(UNIT_FIELD_STAT3, info->intellect );
	SetUInt32Value(UNIT_FIELD_STAT4, info->spirit );
	SetUInt32Value(UNIT_FIELD_ATTACK_POWER, info->attackpower );
	SetUInt32Value(PLAYER_CHARACTER_POINTS1,0);
	// resetting level shouldn't change # of available professions (it doesn't change what professions are learned)...
	// SetUInt32Value(PLAYER_CHARACTER_POINTS2,sWorld.MaxProfs);
	for(uint32 x=0;x<7;x++)
		SetFloatValue(PLAYER_FIELD_MOD_DAMAGE_DONE_PCT+x, 1.00);

}

void Player::CalcResistance(uint32 type)
{
	int32 res;
	int32 pos;
	int32 neg;
	ASSERT(type < 7);
	pos=(BaseResistance[type]*BaseResistanceModPctPos[type])/100;
	neg=(BaseResistance[type]*BaseResistanceModPctNeg[type])/100;

	pos+=FlatResistanceModifierPos[type];
	neg+=FlatResistanceModifierNeg[type];
	res=BaseResistance[type]+pos-neg;
	if(type==0)res+=m_uint32Values[UNIT_FIELD_STAT1]*2;//fix armor from agi
	if(res<0)res=0;
	pos+=(res*ResistanceModPctPos[type])/100;
	neg+=(res*ResistanceModPctNeg[type])/100;
	res=pos-neg+BaseResistance[type];
	if(type==0)res+=m_uint32Values[UNIT_FIELD_STAT1]*2;//fix armor from agi

	if( res < 0 )
		res = 1;

	SetUInt32Value(UNIT_FIELD_RESISTANCEBUFFMODSPOSITIVE+type,pos);
	SetUInt32Value(UNIT_FIELD_RESISTANCEBUFFMODSNEGATIVE+type,neg);
	SetUInt32Value(UNIT_FIELD_RESISTANCES+type,res>0?res:0);

	if( GetSummon() )
		GetSummon()->CalcResistance( type );//Re-calculate pet's too.
}


void Player::UpdateNearbyGameObjects()
{
	this->AquireInrangeLock(); //make sure to release lock before exit function !
	for (Object::InRangeSet::iterator itr = GetInRangeSetBegin(); itr != GetInRangeSetEnd(); ++itr)
	{
		Object * obj = (*itr);
		if(obj->GetTypeId() == TYPEID_GAMEOBJECT)
		{
			bool activate_quest_object = false;
			GameObject *go = ((GameObject*)obj);
			QuestLogEntry *qle = NULL;
			GameObjectInfo *info = go->GetInfo();

			bool deactivate = false;
			if(info &&
				(info->goMap.size() || info->itemMap.size()) )
			{
				for(GameObjectGOMap::iterator GOitr = go->GetInfo()->goMap.begin();
					GOitr != go->GetInfo()->goMap.end();
					++GOitr)
				{
					if((qle = GetQuestLogForEntry(GOitr->first->id)) != 0)
					{
						for(uint32 i = 0; i < qle->GetQuest()->count_required_mob; ++i)
						{
							if(qle->GetQuest()->required_mob[i] == go->GetEntry() &&
								qle->GetMobCount(i) < qle->GetQuest()->required_mobcount[i])
							{
								activate_quest_object = true;
								break;
							}
						}
						if(activate_quest_object)
							break;
					}
				}

				if(!activate_quest_object)
				{
					for(GameObjectItemMap::iterator GOitr = go->GetInfo()->itemMap.begin();
						GOitr != go->GetInfo()->itemMap.end();
						++GOitr)
					{
						for(std::map<uint32, uint32>::iterator it2 = GOitr->second.begin();
							it2 != GOitr->second.end();
							++it2)
						{
							if(GetItemInterface()->GetItemCount(it2->first) < it2->second)
							{
								activate_quest_object = true;
								break;
							}
						}
						if(activate_quest_object)
							break;
					}
				}

				if(!activate_quest_object)
				{
					deactivate = true;
				}
			}
			bool bPassed = !deactivate;
			if( go->isQuestGiver() )
			{
				if( go->HasQuests() && go->NumOfQuests() > 0 )
				{
					std::list<QuestRelation*>::iterator itr2 = go->QuestsBegin();
					for( ; itr2 != go->QuestsEnd(); ++itr2 )
					{
						QuestRelation * qr = (*itr2);

						uint32 status = sQuestMgr.CalcQuestStatus(NULL, this, qr->qst, qr->type, false);
						if(	status == QMGR_QUEST_CHAT 
							|| ( qr->type & QUESTGIVER_QUEST_START && ( status == QMGR_QUEST_AVAILABLE || status == QMGR_QUEST_REPEATABLE ) )
							|| ( qr->type & QUESTGIVER_QUEST_END && status == QMGR_QUEST_FINISHED )
							)
						{
							// Activate gameobject
							EventActivateGameObject(go);
							bPassed = true;
							break;
						}
					}
				}
			}
			if(!bPassed)
				EventDeActivateGameObject((GameObject*)(*itr));
		}
	}
	this->ReleaseInrangeLock();
}


void Player::EventTaxiInterpolate()
{
	if(!m_CurrentTaxiPath || m_mapMgr==NULL) return;

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	uint32 ntime = getMSTime();

	if (ntime > m_taxi_ride_time)
		m_CurrentTaxiPath->SetPosForTime(x, y, z, ntime - m_taxi_ride_time, &lastNode, m_mapId);
	/*else
		m_CurrentTaxiPath->SetPosForTime(x, y, z, m_taxi_ride_time - ntime, &lastNode);*/

	if(x < _minX || x > _maxX || y < _minY || y > _maxX)
		return;

	SetPosition(x,y,z,0);
}

void Player::TaxiStart(TaxiPath *path, uint32 modelid, uint32 start_node)
{
	int32 mapchangeid = -1;
	float mapchangex = 0.0f,mapchangey = 0.0f,mapchangez = 0.0f;
	uint32 cn = m_taxiMapChangeNode;

	m_taxiMapChangeNode = 0;

	if(this->m_MountSpellId)
		RemoveAura(m_MountSpellId);
	//also remove morph spells
	if(GetUInt32Value(UNIT_FIELD_DISPLAYID)!=GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID))
	{
		RemoveAllAuraType( SPELL_AURA_TRANSFORM );
		RemoveAllAuraType( SPELL_AURA_MOD_SHAPESHIFT );
	}

	SetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID, modelid );
	SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);
	SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOCK_PLAYER);

	SetTaxiPath(path);
	SetTaxiPos();
	SetTaxiState(true);
	m_taxi_ride_time = getMSTime();

	//uint32 traveltime = uint32(path->getLength() * TAXI_TRAVEL_SPEED); // 36.7407
	float traveldist = 0;

	float lastx = 0, lasty = 0, lastz = 0;
	TaxiPathNode *firstNode = path->GetPathNode(start_node);
	uint32 add_time = 0;

	// temporary workaround for taximodes with changing map
	if (path->GetID() == 766 || path->GetID() == 767 || path->GetID() == 771 || path->GetID() == 772)
	{
		JumpToEndTaxiNode(path);
		return;
	}

	if(start_node)
	{
		TaxiPathNode *pn = path->GetPathNode(0);
		float dist = 0;
		lastx = pn->x;
		lasty = pn->y;
		lastz = pn->z;
		for(uint32 i = 1; i <= start_node; ++i)
		{
			pn = path->GetPathNode(i);
			if(!pn)
			{
				JumpToEndTaxiNode(path);
				return;
			}

			dist += CalcDistance(lastx, lasty, lastz, pn->x, pn->y, pn->z);
			lastx = pn->x;
			lasty = pn->y;
			lastz = pn->z;
		}
		add_time = uint32( dist * TAXI_TRAVEL_SPEED );
		lastx = lasty = lastz = 0;
	}
	size_t endn = path->GetNodeCount();
	if(m_taxiPaths.size())
		endn-= 2;

	for(uint32 i = start_node; i < endn; ++i)
	{
		TaxiPathNode *pn = path->GetPathNode(i);

		// temporary workaround for taximodes with changing map
		if (!pn || path->GetID() == 766 || path->GetID() == 767 || path->GetID() == 771 || path->GetID() == 772)
		{
			JumpToEndTaxiNode(path);
			return;
		}

		if( pn->mapid != m_mapId )
		{
			endn = (i - 1);
			m_taxiMapChangeNode = i;

			mapchangeid = (int32)pn->mapid;
			mapchangex = pn->x;
			mapchangey = pn->y;
			mapchangez = pn->z;
			break;
		}

		if(!lastx || !lasty || !lastz)
		{
			lastx = pn->x;
			lasty = pn->y;
			lastz = pn->z;
		} else {
			float dist = CalcDistance(lastx,lasty,lastz,
				pn->x,pn->y,pn->z);
			traveldist += dist;
			lastx = pn->x;
			lasty = pn->y;
			lastz = pn->z;
		}
	}

	uint32 traveltime = uint32(traveldist * TAXI_TRAVEL_SPEED);

	if( start_node > endn || (endn - start_node) > 200 )
		return;

	WorldPacket data(SMSG_MONSTER_MOVE, 38 + ( (endn - start_node) * 12 ) );
	data << GetNewGUID();
	data << firstNode->x << firstNode->y << firstNode->z;
	data << m_taxi_ride_time;
	data << uint8( 0 );
	data << uint32( 0x00000300 );
	data << uint32( traveltime );

	if(!cn)
		m_taxi_ride_time -= add_time;

	data << uint32( endn - start_node );
//	uint32 timer = 0, nodecount = 0;
//	TaxiPathNode *lastnode = NULL;

	for(uint32 i = start_node; i < endn; i++)
	{
		TaxiPathNode *pn = path->GetPathNode(i);
		if(!pn)
		{
			JumpToEndTaxiNode(path);
			return;
		}

		data << pn->x << pn->y << pn->z;
	}

	SendMessageToSet(&data, true);

	sEventMgr.AddEvent(this, &Player::EventTaxiInterpolate,
		EVENT_PLAYER_TAXI_INTERPOLATE, 900, 0,0);

	if( mapchangeid < 0 )
	{
		TaxiPathNode *pn = path->GetPathNode((uint32)path->GetNodeCount() - 1);
		sEventMgr.AddEvent(this, &Player::EventDismount, path->GetPrice(),
			pn->x, pn->y, pn->z, EVENT_PLAYER_TAXI_DISMOUNT, traveltime, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
	else
	{
		sEventMgr.AddEvent( this, &Player::EventTeleportTaxi, (uint32)mapchangeid, mapchangex, mapchangey, mapchangez, EVENT_PLAYER_TELEPORT, traveltime, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

void Player::JumpToEndTaxiNode(TaxiPath * path)
{
	// this should *always* be safe in case it cant build your position on the path!
	TaxiPathNode * pathnode = path->GetPathNode((uint32)path->GetNodeCount()-1);
	if(!pathnode) return;

	ModUnsigned32Value( PLAYER_FIELD_COINAGE , -(int32)path->GetPrice());

	SetTaxiState(false);
	SetTaxiPath(NULL);
	UnSetTaxiPos();
	m_taxi_ride_time = 0;

	SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID , 0);
	RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);
	RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOCK_PLAYER);

	SetPlayerSpeed(RUN, m_runSpeed);

	SafeTeleport(pathnode->mapid, 0, LocationVector(pathnode->x, pathnode->y, pathnode->z));

	// Start next path if any remaining
	if(m_taxiPaths.size())
	{
		TaxiPath * p = *m_taxiPaths.begin();
		m_taxiPaths.erase(m_taxiPaths.begin());
		TaxiStart(p, taxi_model_id, 0);
	}
}

void Player::RemoveSpellsFromLine(uint32 skill_line)
{
	uint32 cnt = dbcSkillLineSpell.GetNumRows();
	for(uint32 i=0; i < cnt; i++)
	{
		skilllinespell * sp = dbcSkillLineSpell.LookupRow(i);
		if(sp)
		{
			if(sp->skilline == skill_line)
			{
				// Check ourselves for this spell, and remove it..
				if ( !removeSpell(sp->spell, 0, 0, 0) )
					// if we didnt unlearned spell check deleted spells
					removeDeletedSpell( sp->spell );
			}
		}
	}
}

void Player::CalcStat( uint32 type )
{
	int32 res;
	ASSERT( type < 5 );

	int32 pos = (int32)((int32)BaseStats[type] * (int32)StatModPctPos[type] ) / 100 + (int32)FlatStatModPos[type];
	int32 neg = (int32)((int32)BaseStats[type] * (int32)StatModPctNeg[type] ) / 100 + (int32)FlatStatModNeg[type];
	res = pos + (int32)BaseStats[type] - neg;
	if( res <= 0 )
		res = 1;
	pos += ( res * (int32) static_cast< Player* >( this )->TotalStatModPctPos[type] ) / 100;
	neg += ( res * (int32) static_cast< Player* >( this )->TotalStatModPctNeg[type] ) / 100;
	res = pos + BaseStats[type] - neg;
	if( res <= 0 )
		res = 1;

	SetUInt32Value( UNIT_FIELD_POSSTAT0 + type, pos );
	SetUInt32Value( UNIT_FIELD_NEGSTAT0 + type, neg );
	SetUInt32Value( UNIT_FIELD_STAT0 + type, res > 0 ?res : 0 );
	if( type == 1 )
	   CalcResistance( 0 );

	if( GetSummon() && ( type == 2 || type == 3 ) )
		GetSummon()->CalcStat( type );//Re-calculate pet's too
}

void Player::RegenerateMana(bool is_interrupted)
{
	uint32 cur = GetUInt32Value(UNIT_FIELD_POWER1);
	uint32 mm = GetUInt32Value(UNIT_FIELD_MAXPOWER1);
	if(cur >= mm)
		return;
	float wrate = sWorld.getRate(RATE_POWER1); // config file regen rate
	float amt = (is_interrupted) ? GetFloatValue(UNIT_FIELD_POWER_REGEN_INTERRUPTED_FLAT_MODIFIER) : GetFloatValue(UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER);
	amt *= wrate * 2.0f;

	if((amt<=1.0)&&(amt>0))//this fixes regen like 0.98
	{
		if(is_interrupted)
			return;
		++cur;
	}
	else
		cur += float2int32(amt);
//	Unit::SetPower() will call Object::SetUInt32Value(), which will (for players) call SendPowerUpdate(),
//	which can be slightly out-of-sync with client regeneration [with latency] (and wastes packets since client can handle this on its own)
	if(wrate != 1.0) // our config has custom regen rate, so send new amount to player's client
	{
		SetPower(0, cur);
	}
	else // let player's own client handle normal regen rates.
	{
		m_uint32Values[UNIT_FIELD_POWER1] = (cur>=mm)?mm:cur;
		SendPowerUpdate(false); // send update to other in-range players
	}
}

void Player::RegenerateHealth( bool inCombat )
{
	uint32 cur = GetUInt32Value(UNIT_FIELD_HEALTH);
	uint32 mh = GetUInt32Value(UNIT_FIELD_MAXHEALTH);

	if ( cur==0 ) return; // cebernic: bugfix diying but regenerated?

	if(cur >= mh)
		return;

	gtFloat* HPRegenBase = dbcHPRegenBase.LookupEntry(getLevel()-1 + (getClass()-1)*100);
	gtFloat* HPRegen =  dbcHPRegen.LookupEntry(getLevel()-1 + (getClass()-1)*100);
	float amt = (m_uint32Values[UNIT_FIELD_STAT4]*HPRegen->val+HPRegenBase->val*100);

	if (PctRegenModifier)
		amt+= (amt * PctRegenModifier) / 100;

	amt *= 2.0f;
	amt *= sWorld.getRate(RATE_HEALTH);//Apply conf file rate
	//Near values from official
	// wowwiki: Health Regeneration is increased by 33% while sitting.
	if(m_isResting)
		amt = amt * 1.33f;

	if(inCombat)
		amt *= PctIgnoreRegenModifier;

	if(amt != 0)
	{
		if(amt > 0)
		{
			if(amt <= 1.0f)//this fixes regen like 0.98
				cur++;
			else
				cur += float2int32(amt);
			SetUInt32Value(UNIT_FIELD_HEALTH,(cur>=mh) ? mh : cur);
		}
		else
			DealDamage(this, float2int32(-amt), 0, 0, 0);
	}
}

void Player::LooseRage(int32 decayValue)
{
	//Rage is lost at a rate of 3 rage every 3 seconds.
	//The Anger Management talent changes this to 2 rage every 3 seconds.
	uint32 cur = GetUInt32Value(UNIT_FIELD_POWER2);
	uint32 newrage = ((int)cur <= decayValue) ? 0 : cur-decayValue;
	if (newrage > 1000 )
		newrage = 1000;
// Object::SetUInt32Value() will (for players) call SendPowerUpdate(),
// which can be slightly out-of-sync with client rage loss
// config file rage rate is rage gained, not lost, so we don't need that here
//	SetUInt32Value(UNIT_FIELD_POWER2,newrage);
	m_uint32Values[UNIT_FIELD_POWER2] = newrage;
	SendPowerUpdate(false); // send update to other in-range players
}

void Player::RegenerateEnergy()
{
	uint32 cur = GetUInt32Value(UNIT_FIELD_POWER4);
	uint32 mh = GetUInt32Value(UNIT_FIELD_MAXPOWER4);
	if(cur >= mh)
		return;

	float wrate = sWorld.getRate(RATE_POWER4);
	float amt = PctPowerRegenModifier[POWER_TYPE_ENERGY];
	amt *= wrate * 20.0f;

	cur += float2int32(amt);
//	Object::SetUInt32Value() will (for players) call SendPowerUpdate(),
//	which can be slightly out-of-sync with client regeneration [latency] (and wastes packets since client can handle this on its own)
	if(wrate != 1.0f) // our config has custom regen rate, so send new amount to player's client
	{
		SetPower(GetPowerType(), (cur>=mh) ? mh : cur);
	}
	else // let player's own client handle normal regen rates.
	{
		m_uint32Values[UNIT_FIELD_POWER4] = (cur>=mh) ? mh : cur;
		SendPowerUpdate(false); // send update to other in-range players
	}
}

uint32 Player::GeneratePetNumber()
{
	uint32 val = m_PetNumberMax + 1;
	for (uint32 i = 1; i < m_PetNumberMax; i++)
		if(m_Pets.find(i) == m_Pets.end())
			return i;					   // found a free one

	return val;
}

void Player::RemovePlayerPet(uint32 pet_number)
{
	std::map<uint32, PlayerPet*>::iterator itr = m_Pets.find( pet_number );
	if( itr != m_Pets.end() )
	{
		delete itr->second;
		m_Pets.erase(itr);
	}
	CharacterDatabase.Execute("DELETE FROM playerpetspells WHERE ownerguid=%u AND petnumber=%u", GetLowGUID(), pet_number );
}

void Player::_Relocate(uint32 mapid, const LocationVector & v, bool sendpending, bool force_new_world, uint32 instance_id)
{
	//this func must only be called when switching between maps!
	WorldPacket data(41);
	if(sendpending && mapid != m_mapId && force_new_world)
	{
		data.SetOpcode(SMSG_TRANSFER_PENDING);
		data << mapid;
		GetSession()->SendPacket(&data);
	}

	if(m_mapId != mapid || force_new_world)
	{
		uint32 status = sInstanceMgr.PreTeleport(mapid, this, instance_id);
		if(status != INSTANCE_OK)
		{
			data.Initialize(SMSG_TRANSFER_ABORTED);
			data << mapid << status;
			GetSession()->SendPacket(&data);
			return;
		}

		if(instance_id)
			m_instanceId=instance_id;

		if(IsInWorld())
		{
			RemoveFromWorld();
		}

		data.Initialize(SMSG_NEW_WORLD);
		data << (uint32)mapid << v << v.o;
		GetSession()->SendPacket( &data );
		SetMapId(mapid);

	}
	else
	{
		// via teleport ack msg
		WorldPacket * data = BuildTeleportAckMsg(v);
		m_session->SendPacket(data);
		delete data;
	}
	SetPlayerStatus(TRANSFER_PENDING);
	m_sentTeleportPosition = v;
	SetPosition(v);
	SpeedCheatReset();

	z_axisposition = 0.0f;
	//Dismount before teleport
	if( m_MountSpellId )
		RemoveAura( m_MountSpellId );
}


// Player::AddItemsToWorld
// Adds all items to world, applies any modifiers for them.

void Player::AddItemsToWorld()
{
	Item * pItem;
	for(uint8 i = 0; i < INVENTORY_KEYRING_END; i++)
	{
		pItem = GetItemInterface()->GetInventoryItem(i);
		if( pItem != NULL )
		{
			pItem->PushToWorld(m_mapMgr);

			if(i < INVENTORY_SLOT_BAG_END)	  // only equipment slots get mods.
			{
				_ApplyItemMods(pItem, i, true, false, true);
			}

			if(pItem->IsContainer() && GetItemInterface()->IsBagSlot(i))
			{
				for(uint32 e=0; e < pItem->GetProto()->ContainerSlots; e++)
				{
					Item *item = ((Container*)pItem)->GetItem(e);
					if(item)
					{
						item->PushToWorld(m_mapMgr);
					}
				}
			}
		}
	}

	UpdateStats();
}

// Player::RemoveItemsFromWorld
// Removes all items from world, reverses any modifiers.

void Player::RemoveItemsFromWorld()
{
	Item * pItem;
	for(uint32 i = 0; i < INVENTORY_KEYRING_END; i++)
	{
		pItem = m_ItemInterface->GetInventoryItem((int8)i);
		if(pItem)
		{
			if(pItem->IsInWorld())
			{
				if(i < INVENTORY_SLOT_BAG_END)	  // only equipment+bags slots get mods.
				{
					_ApplyItemMods(pItem, i, false, false, true);
				}
				pItem->RemoveFromWorld();
			}

			if(pItem->IsContainer() && GetItemInterface()->IsBagSlot(i))
			{
				for(uint32 e=0; e < pItem->GetProto()->ContainerSlots; e++)
				{
					Item *item = ((Container*)pItem)->GetItem(e);
					if(item && item->IsInWorld())
					{
						item->RemoveFromWorld();
					}
				}
			}
		}
	}

	UpdateStats();
}

uint32 Player::BuildCreateUpdateBlockForPlayer(ByteBuffer *data, Player *target )
{
	int count = 0;
	if(target == this)
	{
		// we need to send create objects for all items.
		count += GetItemInterface()->m_CreateForPlayer(data);
	}
	count += Unit::BuildCreateUpdateBlockForPlayer(data, target);
	return count;
}

void Player::Kick(uint32 delay /* = 0 */)
{
	if(!delay)
	{
		m_KickDelay = 0;
		_Kick();
	} else {
		m_KickDelay = delay;
		sEventMgr.AddEvent(this, &Player::_Kick, EVENT_PLAYER_KICK, 1000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

void Player::_Kick()
{
	if(!m_KickDelay)
	{
		sEventMgr.RemoveEvents(this, EVENT_PLAYER_KICK);
		// remove now
		GetSession()->LogoutPlayer(true);
	} else {
		if(m_KickDelay < 1500)
		{
			m_KickDelay = 0;
		} else {
			m_KickDelay -= 1000;
		}
		sChatHandler.BlueSystemMessageToPlr(this, "You will be removed from the server in %u seconds.", (uint32)(m_KickDelay/1000));
	}
}

void Player::ClearCooldownForSpell(uint32 spell_id)
{
	WorldPacket data(12);
	data.SetOpcode(SMSG_CLEAR_COOLDOWN);
	data << spell_id << GetGUID();
	GetSession()->SendPacket(&data);

	// remove cooldown data from Server side lists
	uint32 i;
	PlayerCooldownMap::iterator itr, itr2;
	SpellEntry * spe = dbcSpell.LookupEntry(spell_id);
	if(!spe) return;

	for(i = 0; i < NUM_COOLDOWN_TYPES; ++i)
	{
		for( itr = m_cooldownMap[i].begin(); itr != m_cooldownMap[i].end(); )
		{
			itr2 = itr++;
			if( ( i == COOLDOWN_TYPE_CATEGORY && itr2->first == spe->Category ) ||
				( i == COOLDOWN_TYPE_SPELL && itr2->first == spe->Id ) )
			{
				m_cooldownMap[i].erase( itr2 );
			}
		}
	}
}

void Player::ClearCooldownsOnLine(uint32 skill_line, uint32 called_from)
{
	// found an easier way.. loop spells, check skill line
	SpellSet::const_iterator itr = mSpells.begin();
	skilllinespell *sk;
	for(; itr != mSpells.end(); ++itr)
	{
		if((*itr) == called_from)	   // skip calling spell.. otherwise spammies! :D
			continue;

		sk = objmgr.GetSpellSkill((*itr));
		if(sk && sk->skilline == skill_line)
			ClearCooldownForSpell((*itr));
	}
}

void Player::ResetAllCooldowns()
{
	uint32 guid = (uint32)GetSelection();

	switch(getClass())
	{
		case WARRIOR:
		{
			ClearCooldownsOnLine(26, guid);
			ClearCooldownsOnLine(256, guid);
			ClearCooldownsOnLine(257 , guid);
		} break;
		case PALADIN:
		{
			ClearCooldownsOnLine(594, guid);
			ClearCooldownsOnLine(267, guid);
			ClearCooldownsOnLine(184, guid);
		} break;
		case HUNTER:
		{
			ClearCooldownsOnLine(50, guid);
			ClearCooldownsOnLine(51, guid);
			ClearCooldownsOnLine(163, guid);
		} break;
		case ROGUE:
		{
			ClearCooldownsOnLine(253, guid);
			ClearCooldownsOnLine(38, guid);
			ClearCooldownsOnLine(39, guid);
		} break;
		case PRIEST:
		{
			ClearCooldownsOnLine(56, guid);
			ClearCooldownsOnLine(78, guid);
			ClearCooldownsOnLine(613, guid);
		} break;

		case DEATHKNIGHT:
		{
			ClearCooldownsOnLine(770, guid);
			ClearCooldownsOnLine(771, guid);
			ClearCooldownsOnLine(772, guid);
		} break;

		case SHAMAN:
		{
			ClearCooldownsOnLine(373, guid);
			ClearCooldownsOnLine(374, guid);
			ClearCooldownsOnLine(375, guid);
		} break;
		case MAGE:
		{
			ClearCooldownsOnLine(6, guid);
			ClearCooldownsOnLine(8, guid);
			ClearCooldownsOnLine(237, guid);
		} break;
		case WARLOCK:
		{
			ClearCooldownsOnLine(355, guid);
			ClearCooldownsOnLine(354, guid);
			ClearCooldownsOnLine(593, guid);
		} break;
		case DRUID:
		{
			ClearCooldownsOnLine(573, guid);
			ClearCooldownsOnLine(574, guid);
			ClearCooldownsOnLine(134, guid);
		} break;

		default: return; break;
	}
}

void Player::PushUpdateData(ByteBuffer *data, uint32 updatecount)
{
	// imagine the bytebuffer getting appended from 2 threads at once! :D
	_bufferS.Acquire();

	// unfortunately there is no guarantee that all data will be compressed at a ratio
	// that will fit into 2^16 bytes ( stupid client limitation on server packets )
	// so if we get more than 63KB of update data, force an update and then append it
	// to the clean buffer.
	if( (data->size() + bUpdateBuffer.size() ) >= 63000 )
		ProcessPendingUpdates();

	mUpdateCount += updatecount;
	bUpdateBuffer.append(*data);

	// add to process queue
	if(m_mapMgr && !bProcessPending)
	{
		bProcessPending = true;
		m_mapMgr->PushToProcessed(this);
	}

	_bufferS.Release();
}

void Player::PushOutOfRange(const WoWGuid & guid)
{
	_bufferS.Acquire();
	mOutOfRangeIds << guid;
	++mOutOfRangeIdCount;

	// add to process queue
	if(m_mapMgr && !bProcessPending)
	{
		bProcessPending = true;
		m_mapMgr->PushToProcessed(this);
	}
	_bufferS.Release();
}

void Player::PushCreationData(ByteBuffer *data, uint32 updatecount)
{
    // imagine the bytebuffer getting appended from 2 threads at once! :D
	_bufferS.Acquire();

	// unfortunately there is no guarantee that all data will be compressed at a ratio
	// that will fit into 2^16 bytes ( stupid client limitation on server packets )
	// so if we get more than 63KB of update data, force an update and then append it
	// to the clean buffer.
	if( (data->size() + bCreationBuffer.size() + mOutOfRangeIds.size() ) >= 63000 )
		ProcessPendingUpdates();

	mCreationCount += updatecount;
	bCreationBuffer.append(*data);

	// add to process queue
	if(m_mapMgr && !bProcessPending)
	{
		bProcessPending = true;
		m_mapMgr->PushToProcessed(this);
	}

	_bufferS.Release();

}

void Player::ProcessPendingUpdates()
{
	_bufferS.Acquire();
    if(!bUpdateBuffer.size() && !mOutOfRangeIds.size() && !bCreationBuffer.size())
	{
		_bufferS.Release();
		return;
	}

	size_t bBuffer_size =  (bCreationBuffer.size() > bUpdateBuffer.size() ? bCreationBuffer.size() : bUpdateBuffer.size()) + 10 + (mOutOfRangeIds.size() * 9);
    uint8 * update_buffer = new uint8[bBuffer_size];
	size_t c = 0;

    //build out of range updates if creation updates are queued
    if(bCreationBuffer.size() || mOutOfRangeIdCount)
    {
        #ifdef USING_BIG_ENDIAN
	        *(uint32*)&update_buffer[c] = swap32(uint32(((mOutOfRangeIds.size() > 0) ? (mCreationCount + 1) : mCreationCount)));	c += 4;
        #else
	        *(uint32*)&update_buffer[c] = ((mOutOfRangeIds.size() > 0) ? (mCreationCount + 1) : mCreationCount);	c += 4;
        #endif
	        //update_buffer[c] = 1;																			   ++c;

            // append any out of range updates
	    if(mOutOfRangeIdCount)
	    {
		    update_buffer[c]				= UPDATETYPE_OUT_OF_RANGE_OBJECTS;			 ++c;
            #ifdef USING_BIG_ENDIAN
		            *(uint32*)&update_buffer[c]	 = swap32(mOutOfRangeIdCount);						  c += 4;
            #else
		            *(uint32*)&update_buffer[c]	 = mOutOfRangeIdCount;						  c += 4;
            #endif
		    memcpy(&update_buffer[c], mOutOfRangeIds.contents(), mOutOfRangeIds.size());   c += mOutOfRangeIds.size();
		    mOutOfRangeIds.clear();
		    mOutOfRangeIdCount = 0;
	    }

        if(bCreationBuffer.size())
			memcpy(&update_buffer[c], bCreationBuffer.contents(), bCreationBuffer.size());  c += bCreationBuffer.size();

        // clear our update buffer
	    bCreationBuffer.clear();
	    mCreationCount = 0;

        // compress update packet
	    // while we said 350 before, i'm gonna make it 500 :D
	    if(c < (size_t)sWorld.compression_threshold || !CompressAndSendUpdateBuffer((uint32)c, update_buffer))
	    {
		    // send uncompressed packet -> because we failed
		    m_session->OutPacket(SMSG_UPDATE_OBJECT, (uint16)c, update_buffer);
	    }
    }

	if(bUpdateBuffer.size())
	{
		c = 0;

		#ifdef USING_BIG_ENDIAN
			*(uint32*)&update_buffer[c] = swap32(uint32(((mOutOfRangeIds.size() > 0) ? (mUpdateCount + 1) : mUpdateCount)));	c += 4;
		#else
			*(uint32*)&update_buffer[c] = ((mOutOfRangeIds.size() > 0) ? (mUpdateCount + 1) : mUpdateCount);	c += 4;
		#endif
		//update_buffer[c] = 1;																			   ++c;
		memcpy(&update_buffer[c], bUpdateBuffer.contents(), bUpdateBuffer.size());  c += bUpdateBuffer.size();

		// clear our update buffer
		bUpdateBuffer.clear();
		mUpdateCount = 0;

		// compress update packet
		// while we said 350 before, i'm gonna make it 500 :D
		if(c < (size_t)sWorld.compression_threshold || !CompressAndSendUpdateBuffer((uint32)c, update_buffer))
		{
			// send uncompressed packet -> because we failed
			m_session->OutPacket(SMSG_UPDATE_OBJECT, (uint16)c, update_buffer);
		}
	}

	bProcessPending = false;
	_bufferS.Release();
	delete [] update_buffer;

	// send any delayed packets
	WorldPacket * pck;
	while(delayedPackets.size())
	{
		pck = delayedPackets.next();
		//printf("Delayed packet opcode %u sent.\n", pck->GetOpcode());
		m_session->SendPacket(pck);
		delete pck;
	}

	// resend speed if needed
	if(resend_speed)
	{
		SetPlayerSpeed(RUN, m_runSpeed);
		SetPlayerSpeed(FLY, m_flySpeed);
		resend_speed=false;
	}
}

bool Player::CompressAndSendUpdateBuffer(uint32 size, const uint8* update_buffer)
{
	uint32 destsize = size + size/10 + 16;
	int rate = sWorld.getIntRate(INTRATE_COMPRESSION);
	if(size >= 40000 && rate < 6)
		rate = 6;

	// set up stream
	z_stream stream;
	stream.zalloc = 0;
	stream.zfree  = 0;
	stream.opaque = 0;

	if(deflateInit(&stream, rate) != Z_OK)
	{
		sLog.outError("deflateInit failed.");
		return false;
	}

	uint8 *buffer = new uint8[destsize];
	//memset(buffer,0,destsize);	/* fix umr - burlex */

	// set up stream pointers
	stream.next_out  = (Bytef*)buffer+4;
	stream.avail_out = destsize;
	stream.next_in   = (Bytef*)update_buffer;
	stream.avail_in  = size;

	// call the actual process
	if(deflate(&stream, Z_NO_FLUSH) != Z_OK ||
		stream.avail_in != 0)
	{
		sLog.outError("deflate failed.");
		delete [] buffer;
		return false;
	}

	// finish the deflate
	if(deflate(&stream, Z_FINISH) != Z_STREAM_END)
	{
		sLog.outError("deflate failed: did not end stream");
		delete [] buffer;
		return false;
	}

	// finish up
	if(deflateEnd(&stream) != Z_OK)
	{
		sLog.outError("deflateEnd failed.");
		delete [] buffer;
		return false;
	}

	// fill in the full size of the compressed stream
#ifdef USING_BIG_ENDIAN
	*(uint32*)&buffer[0] = swap32(size);
#else
	*(uint32*)&buffer[0] = size;
#endif

	// send it
	m_session->OutPacket(SMSG_COMPRESSED_UPDATE_OBJECT, (uint16)stream.total_out + 4, buffer);

	// cleanup memory
	delete [] buffer;

	return true;
}

void Player::ClearAllPendingUpdates()
{
	_bufferS.Acquire();
	bProcessPending = false;
	mUpdateCount = 0;
	bUpdateBuffer.clear();
	_bufferS.Release();
}

void Player::AddSplinePacket(uint64 guid, ByteBuffer* packet)
{
	SplineMap::iterator itr = _splineMap.find(guid);
	if(itr != _splineMap.end())
	{
		delete itr->second;
		_splineMap.erase(itr);
	}
	_splineMap.insert( SplineMap::value_type( guid, packet ) );
}

ByteBuffer* Player::GetAndRemoveSplinePacket(uint64 guid)
{
	SplineMap::iterator itr = _splineMap.find(guid);
	if(itr != _splineMap.end())
	{
		ByteBuffer *buf = itr->second;
		_splineMap.erase(itr);
		return buf;
	}
	return NULL;
}

void Player::ClearSplinePackets()
{
	SplineMap::iterator it2;
	for(SplineMap::iterator itr = _splineMap.begin(); itr != _splineMap.end();)
	{
		it2 = itr;
		++itr;
		delete it2->second;
		_splineMap.erase(it2);
	}
	_splineMap.clear();
}



bool Player::ExitInstance()
{
	if(!m_bgEntryPointX)
		return false;

	RemoveFromWorld();

	SafeTeleport(m_bgEntryPointMap, m_bgEntryPointInstance, LocationVector(
		m_bgEntryPointX, m_bgEntryPointY, m_bgEntryPointZ, m_bgEntryPointO));

	return true;
}

void Player::SaveEntryPoint(uint32 mapId)
{
	if(IS_INSTANCE(GetMapId()))
		return; // dont save if we're not on the main continent.
	//otherwise we could end up in an endless loop :P
	MapInfo * pMapinfo = WorldMapInfoStorage.LookupEntry(mapId);

	if(pMapinfo)
	{
		m_bgEntryPointX = pMapinfo->repopx;
		m_bgEntryPointY = pMapinfo->repopy;
		m_bgEntryPointZ = pMapinfo->repopz;
		m_bgEntryPointO = GetOrientation();
		m_bgEntryPointMap = pMapinfo->repopmapid;
		m_bgEntryPointInstance = GetInstanceID();
	}
	else
	{
		m_bgEntryPointMap	 = 0;
		m_bgEntryPointX		 = 0;
		m_bgEntryPointY		 = 0;
		m_bgEntryPointZ		 = 0;
		m_bgEntryPointO		 = 0;
		m_bgEntryPointInstance  = 0;
	}
}

void Player::CleanupGossipMenu()
{
	if(CurrentGossipMenu)
	{
		delete CurrentGossipMenu;
		CurrentGossipMenu = NULL;
	}
}

void Player::Gossip_Complete()
{
	GetSession()->OutPacket(SMSG_GOSSIP_COMPLETE, 0, NULL);
	CleanupGossipMenu();
}


void Player::CloseGossip()
{
	Gossip_Complete();
}

void Player::PrepareQuestMenu(uint64 guid )
{
	uint32 TextID = 820;
    objmgr.CreateGossipMenuForPlayer(&PlayerTalkClass, guid, TextID, this);
}

void Player::SendGossipMenu( uint32 TitleTextId, uint64 npcGUID )
{
	PlayerTalkClass->SetTextID(TitleTextId);
	PlayerTalkClass->SendTo(this);
}

QuestStatus Player::GetQuestStatus( uint32 quest_id )
{
	uint32 status = sQuestMgr.CalcQuestStatus( this, quest_id);
	switch (status)
	{
		case QMGR_QUEST_NOT_FINISHED: return QUEST_STATUS_INCOMPLETE;
		case QMGR_QUEST_FINISHED: return QUEST_STATUS_COMPLETE;
		case QMGR_QUEST_NOT_AVAILABLE: return QUEST_STATUS_UNAVAILABLE;
	}
	return QUEST_STATUS_UNAVAILABLE;
}

bool Player::IsInCity()
{
	AreaTable * at = dbcArea.LookupEntry(GetAreaID());
	AreaTable * zt = NULL;
	if( at->ZoneId )
		zt = dbcArea.LookupEntry( at->ZoneId );

	bool areaIsCity = at->AreaFlags & AREA_CITY_AREA || at->AreaFlags & AREA_CITY;
	bool zoneIsCity = zt && ( zt->AreaFlags & AREA_CITY_AREA || zt->AreaFlags & AREA_CITY );
	
	return ( areaIsCity || zoneIsCity );
}

void Player::ZoneUpdate(uint32 ZoneId)
{
	if( m_zoneId != ZoneId )
	{
		m_zoneId = ZoneId;
		RemoveAurasByInterruptFlag( AURA_INTERRUPT_ON_LEAVE_AREA );
	}

	/* how the f*ck is this happening */
	if( m_playerInfo == NULL )
	{
		m_playerInfo = objmgr.GetPlayerInfo(GetLowGUID());
		if( m_playerInfo == NULL )
		{
			m_session->Disconnect();
			return;
		}
	}

	m_playerInfo->lastZone = ZoneId;
	sHookInterface.OnZone(this, ZoneId);

	AreaTable * at = dbcArea.LookupEntry(GetAreaID());
	if(at && ( at->category == AREAC_SANCTUARY || at->AreaFlags & AREA_SANCTUARY ) )
	{
		Unit * pUnit = (GetSelection() == 0) ? 0 : (m_mapMgr ? m_mapMgr->GetUnit(GetSelection()) : 0);
		if(pUnit && DuelingWith != pUnit)
		{
			EventAttackStop();
			smsg_AttackStop(pUnit);
		}

		if(m_currentSpell)
		{
			Unit * target = m_currentSpell->GetUnitTarget();
			if(target && target != DuelingWith && target != this)
				m_currentSpell->cancel();
		}
	}

	at = dbcArea.LookupEntryForced( ZoneId );

	if( !m_channels.empty() && at)
	{
		// change to zone name, not area name
		for( std::set<Channel*>::iterator itr = m_channels.begin(),nextitr ; itr != m_channels.end() ; itr = nextitr)
		{
			nextitr = itr; ++nextitr;
			Channel * chn;
			chn = (*itr);
			// Check if this is a custom channel (i.e. global)
			if( !( (*itr)->m_flags & 0x10 ) )
				continue;

			if( chn->m_flags & 0x40 ) // LookingForGroup - constant among all zones
				continue;

			char updatedName[95];
			ChatChannelDBC * pDBC;
			pDBC = dbcChatChannels.LookupEntryForced( chn->m_id );
			if( !pDBC )
			{
				Log.Error( "ChannelMgr" , "Invalid channel entry %u for %s" , chn->m_id , chn->m_name.c_str() );
				return;
			}
			//for( int i = 0 ; i <= 15 ; i ++ )
			//	Log.Notice( "asfssdf" , "%u %s" , i , pDBC->name_pattern[i] );
			snprintf( updatedName , 95 , pDBC->name_pattern[0] , at->name );
			Channel * newChannel = channelmgr.GetCreateChannel( updatedName , NULL , chn->m_id );
			if( newChannel == NULL )
			{
				Log.Error( "ChannelMgr" , "Could not create channel %s!" , updatedName );
				return; // whoops?
			}
			//Log.Notice( "ChannelMgr" , "LEAVING CHANNEL %s" , chn->m_name.c_str() );
			//Log.Notice( "ChannelMgr" , "JOINING CHANNEL %s" , newChannel->m_name.c_str() );
			if( chn != newChannel ) // perhaps there's no need
			{
				// join new channel
				newChannel->AttemptJoin( this , "" );
				// leave the old channel

				chn->Part( this , false );
			}
		}
	}

#ifdef OPTIMIZED_PLAYER_SAVING
	save_Zone();
#endif

	if( m_mapMgr != NULL )
		m_mapMgr->SendInitialStates( this );

	UpdateChannels(ZoneId);
	/*std::map<uint32, AreaTable*>::iterator iter = sWorld.mZoneIDToTable.find(ZoneId);
	if(iter == sWorld.mZoneIDToTable.end())
		return;

	AreaTable *p = iter->second;
	if(p->AreaId != m_AreaID)
	{
		m_AreaID = p->AreaId;
		UpdatePVPStatus(m_AreaID);
	}

	sLog.outDetail("ZONE_UPDATE: Player %s entered zone %s", GetName(), sAreaStore.LookupString((int)p->name));*/
	//UpdatePvPArea();

}
void Player::UpdateChannels(uint16 AreaID)
{
	set<Channel *>::iterator i;
	Channel * c;
	string channelname, AreaName;


	if(GetMapId()==450)
		AreaID = 2917;
	if(GetMapId()==449)
		AreaID = 2918;

	AreaTable * at2 = dbcArea.LookupEntry(GetZoneId());

	//Check for instances?
	if(!AreaID || AreaID == 0xFFFF)
	{
		MapInfo *pMapinfo = WorldMapInfoStorage.LookupEntry(GetMapId());
		if(IS_INSTANCE(GetMapId()))
			AreaName = pMapinfo->name;
		else
			return;//How'd we get here?
	}
	else
	{
		AreaName = at2->name;
		if(AreaName.length() < 2)
		{
			MapInfo *pMapinfo = WorldMapInfoStorage.LookupEntry(GetMapId());
			AreaName = pMapinfo->name;
		}
	}
	m_TeleportState = 1;
	for(i = m_channels.begin(); i != m_channels.end();)
	{
		c = *i;
		i++;

		if(!c->m_general || c->m_name == "LookingForGroup")//Not an updatable channel.
			continue;

		if( strstr(c->m_name.c_str(), "General") )
			channelname = "General";
		else if( strstr(c->m_name.c_str(), "Trade") )
			channelname = "Trade";
		else if( strstr(c->m_name.c_str(), "LocalDefense") )
			channelname = "LocalDefense";
		else if( strstr(c->m_name.c_str(), "GuildRecruitment") )
			channelname = "GuildRecruitment";
		else
			continue;//Those 4 are the only ones we want updated.
		channelname += " - ";
		if( (strstr(c->m_name.c_str(), "Trade") || strstr(c->m_name.c_str(), "GuildRecruitment")) && ( at2->AreaFlags &AREA_CITY || at2->AreaFlags &AREA_CITY_AREA ))
		{
			channelname += "City";
		}
		else
			channelname += AreaName;

		Channel * chn = channelmgr.GetCreateChannel(channelname.c_str(), this, c->m_id);
		if( !chn->HasMember(this) )
		{
			c->Part(this);
			chn->AttemptJoin(this, NULL);
		}
	}
	m_TeleportState = 0;
}
void Player::SendTradeUpdate()
{
	Player * pTarget = GetTradeTarget();
	if( !pTarget )
		return;

	WorldPacket data( SMSG_TRADE_STATUS_EXTENDED, 532 );
	/*data << uint8(1);
	data << uint32(2) << uint32(2);
	data << mTradeGold << uint32(0);*/
	data << uint8(1);
	data << uint32(0x19);
	data << m_tradeSequence;
	data << m_tradeSequence++;
	data << mTradeGold << uint32(0);

	uint8 count = 0;
	// Items
	for( uint32 Index = 0; Index < 7; ++Index )
	{
		Item * pItem = mTradeItems[ Index ];
		if(pItem != 0)
		{
			count++;
			ItemPrototype * pProto = pItem->GetProto();
			ASSERT( pProto != 0 );

			data << uint8( Index );

			data << pProto->ItemId;
			data << pProto->DisplayInfoID;
			data << pItem->GetUInt32Value( ITEM_FIELD_STACK_COUNT );	// Amount		   OK

			// Enchantment stuff
			data << uint32(0);											// unknown
			data << pItem->GetUInt64Value( ITEM_FIELD_GIFTCREATOR );	// gift creator	 OK
			data << pItem->GetUInt32Value( ITEM_FIELD_ENCHANTMENT_1_1 );	// Item Enchantment OK
			for( uint8 i = 2; i < 5; i++ )								// Gem enchantments
			{
				if( pItem->GetEnchantment(i) != NULL && pItem->GetEnchantment(i)->Enchantment != NULL )
					data << pItem->GetEnchantment(i)->Enchantment->Id;
				else
					data << uint32( 0 );
			}
			data << pItem->GetUInt64Value( ITEM_FIELD_CREATOR );		// item creator	 OK
			data << pItem->GetUInt32Value( ITEM_FIELD_SPELL_CHARGES );	// Spell Charges	OK

			data << uint32(0);											// seems like time stamp or something like that
			data << pItem->GetUInt32Value( ITEM_FIELD_RANDOM_PROPERTIES_ID );
			data << pProto->LockId;										// lock ID		  OK
			data << pItem->GetUInt32Value( ITEM_FIELD_MAXDURABILITY );
			data << pItem->GetUInt32Value( ITEM_FIELD_DURABILITY );
		}
	}
	data.resize( 21 + count * 73 );
	pTarget->GetSession()->SendPacket(&data);
}

void Player::RequestDuel(Player *pTarget)
{
	// We Already Dueling or have already Requested a Duel

	if( DuelingWith != NULL )
		return;

	if( m_duelState != DUEL_STATE_FINISHED )
		return;

	SetDuelState( DUEL_STATE_REQUESTED );

	//Setup Duel
	pTarget->DuelingWith = this;
	DuelingWith = pTarget;

	//Get Flags position
	float dist = CalcDistance(pTarget);
	dist = dist * 0.5f; //half way
	float x = (GetPositionX() + pTarget->GetPositionX()*dist)/(1+dist) + cos(GetOrientation()+(float(M_PI)/2))*2;
	float y = (GetPositionY() + pTarget->GetPositionY()*dist)/(1+dist) + sin(GetOrientation()+(float(M_PI)/2))*2;
	float z = (GetPositionZ() + pTarget->GetPositionZ()*dist)/(1+dist);

	//Create flag/arbiter
	GameObject* pGameObj = GetMapMgr()->CreateGameObject(21680);
	pGameObj->CreateFromProto(21680,GetMapId(), x, y, z, GetOrientation());
	pGameObj->SetInstanceID(GetInstanceID());

	//Spawn the Flag
	pGameObj->SetUInt64Value(OBJECT_FIELD_CREATED_BY, GetGUID());
	pGameObj->SetUInt32Value(GAMEOBJECT_FACTION, GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
	pGameObj->SetUInt32Value(GAMEOBJECT_LEVEL, GetUInt32Value(UNIT_FIELD_LEVEL));

	//Assign the Flag
	SetUInt64Value(PLAYER_DUEL_ARBITER,pGameObj->GetGUID());
	pTarget->SetUInt64Value(PLAYER_DUEL_ARBITER,pGameObj->GetGUID());

	pGameObj->PushToWorld(m_mapMgr);

	WorldPacket data(SMSG_DUEL_REQUESTED, 16);
	data << pGameObj->GetGUID();
	data << GetGUID();
	pTarget->GetSession()->SendPacket(&data);
}

void Player::DuelCountdown()
{
	if( DuelingWith == NULL )
		return;

	m_duelCountdownTimer -= 1000;

	if( m_duelCountdownTimer < 0 )
		m_duelCountdownTimer = 0;

	if( m_duelCountdownTimer == 0 )
	{
		// Start Duel.
		SetUInt32Value( UNIT_FIELD_POWER2, 0 );
		DuelingWith->SetUInt32Value( UNIT_FIELD_POWER2, 0 );

		//Give the players a Team
		DuelingWith->SetUInt32Value( PLAYER_DUEL_TEAM, 1 ); // Duel Requester
		SetUInt32Value( PLAYER_DUEL_TEAM, 2 );

		SetDuelState( DUEL_STATE_STARTED );
		DuelingWith->SetDuelState( DUEL_STATE_STARTED );

		sEventMgr.AddEvent( this, &Player::DuelBoundaryTest, EVENT_PLAYER_DUEL_BOUNDARY_CHECK, 500, 0, 0 );
		sEventMgr.AddEvent( DuelingWith, &Player::DuelBoundaryTest, EVENT_PLAYER_DUEL_BOUNDARY_CHECK, 500, 0, 0 );
	}
}

void Player::DuelBoundaryTest()
{
	//check if in bounds
	if(!IsInWorld())
		return;

	GameObject * pGameObject = GetMapMgr()->GetGameObject(GetUInt32Value(PLAYER_DUEL_ARBITER));
	if(!pGameObject)
	{
		EndDuel(DUEL_WINNER_RETREAT);
		return;
	}

	float Dist = CalcDistance((Object*)pGameObject);

	if(Dist > 75.0f)
	{
		// Out of bounds
		if(m_duelStatus == DUEL_STATUS_OUTOFBOUNDS)
		{
			// we already know, decrease timer by 500
			m_duelCountdownTimer -= 500;
			if(m_duelCountdownTimer == 0)
			{
				// Times up :p
				DuelingWith->EndDuel(DUEL_WINNER_RETREAT);
			}
		}
		else
		{
			// we just went out of bounds
			// set timer
			m_duelCountdownTimer = 10000;

			// let us know
#ifdef USING_BIG_ENDIAN
			uint32 swapped = swap32(m_duelCountdownTimer);
			m_session->OutPacket(SMSG_DUEL_OUTOFBOUNDS, 4, &swapped);
#else
			m_session->OutPacket(SMSG_DUEL_OUTOFBOUNDS, 4, &m_duelCountdownTimer);
#endif
			m_duelStatus = DUEL_STATUS_OUTOFBOUNDS;
		}
	}
	else
	{
		// we're in range
		if(m_duelStatus == DUEL_STATUS_OUTOFBOUNDS)
		{
			// just came back in range
			m_session->OutPacket(SMSG_DUEL_INBOUNDS);
			m_duelStatus = DUEL_STATUS_INBOUNDS;
		}
	}
}

void Player::EndDuel(uint8 WinCondition)
{
	if( m_duelState == DUEL_STATE_FINISHED )
	{
		//if loggingout player requested a duel then we have to make the cleanups
		if( GetUInt32Value(PLAYER_DUEL_ARBITER) )
		{
			GameObject* arbiter = m_mapMgr ? GetMapMgr()->GetGameObject(GetUInt32Value(PLAYER_DUEL_ARBITER)) : 0;

			if( arbiter != NULL )
			{
				arbiter->RemoveFromWorld( true );
				delete arbiter;
			}

			//we do not wish to lock the other player in duel state
			DuelingWith->SetUInt64Value( PLAYER_DUEL_ARBITER, 0 );
			DuelingWith->SetUInt32Value( PLAYER_DUEL_TEAM, 0 );
			SetUInt64Value( PLAYER_DUEL_ARBITER, 0 );
			SetUInt32Value( PLAYER_DUEL_TEAM, 0 );
			sEventMgr.RemoveEvents( DuelingWith, EVENT_PLAYER_DUEL_BOUNDARY_CHECK );
			sEventMgr.RemoveEvents( DuelingWith, EVENT_PLAYER_DUEL_COUNTDOWN );
			DuelingWith->DuelingWith = NULL;
			DuelingWith = NULL;
			//the duel did not start so we are not in combat or cast any spells yet.
		}
		return;
	}

	// Remove the events
	sEventMgr.RemoveEvents( this, EVENT_PLAYER_DUEL_COUNTDOWN );
	sEventMgr.RemoveEvents( this, EVENT_PLAYER_DUEL_BOUNDARY_CHECK );

	for( uint32 x = MAX_POSITIVE_AURAS_EXTEDED_START; x < MAX_POSITIVE_AURAS_EXTEDED_END; ++x )
	{
		if( m_auras[x] == NULL )
			continue;
		if( m_auras[x]->WasCastInDuel() )
			m_auras[x]->Remove();
	}

	m_duelState = DUEL_STATE_FINISHED;

	if( DuelingWith == NULL )
		return;

	sEventMgr.RemoveEvents( DuelingWith, EVENT_PLAYER_DUEL_BOUNDARY_CHECK );
	sEventMgr.RemoveEvents( DuelingWith, EVENT_PLAYER_DUEL_COUNTDOWN );

	// spells waiting to hit
	sEventMgr.RemoveEvents(this, EVENT_SPELL_DAMAGE_HIT);

	for( uint32 x = MAX_POSITIVE_AURAS_EXTEDED_START; x < MAX_POSITIVE_AURAS_EXTEDED_END; ++x )
	{
		if( DuelingWith->m_auras[x] == NULL )
			continue;
		if( DuelingWith->m_auras[x]->WasCastInDuel() )
			DuelingWith->m_auras[x]->Remove();
	}

	DuelingWith->m_duelState = DUEL_STATE_FINISHED;

	//Announce Winner
	WorldPacket data( SMSG_DUEL_WINNER, 500 );
	data << uint8( WinCondition );
	data << GetName() << DuelingWith->GetName();
	SendMessageToSet( &data, true );

	data.Initialize( SMSG_DUEL_COMPLETE );
	data << uint8( 1 );
	SendMessageToSet( &data, true );

	//Clear Duel Related Stuff

	GameObject* arbiter = m_mapMgr ? GetMapMgr()->GetGameObject(GetUInt32Value(PLAYER_DUEL_ARBITER)) : 0;

	if( arbiter != NULL )
	{
		arbiter->RemoveFromWorld( true );
		delete arbiter;
	}

	SetUInt64Value( PLAYER_DUEL_ARBITER, 0 );
	DuelingWith->SetUInt64Value( PLAYER_DUEL_ARBITER, 0 );

	SetUInt32Value( PLAYER_DUEL_TEAM, 0 );
	DuelingWith->SetUInt32Value( PLAYER_DUEL_TEAM, 0 );

	EventAttackStop();
	DuelingWith->EventAttackStop();

	// Call off pet
	if( this->GetSummon() != NULL )
	{
		this->GetSummon()->CombatStatus.Vanished();
		this->GetSummon()->GetAIInterface()->SetUnitToFollow( this );
		this->GetSummon()->GetAIInterface()->HandleEvent( EVENT_FOLLOWOWNER, this->GetSummon(), 0 );
		this->GetSummon()->GetAIInterface()->WipeTargetList();
	}

	// removing auras that kills players after if low HP
	/*RemoveNegativeAuras(); NOT NEEDED. External targets can always gank both duelers with DoTs. :D
	DuelingWith->RemoveNegativeAuras();*/
	//Same as above only cleaner.
	for(uint32 x=MAX_NEGATIVE_AURAS_EXTEDED_START;x<MAX_REMOVABLE_AURAS_END;x++)
	{
		if(DuelingWith->m_auras[x])
		{
			if(DuelingWith->m_auras[x]->WasCastInDuel())
			    DuelingWith->m_auras[x]->Remove();
		}
		if(m_auras[x])
		{
			if(m_auras[x]->WasCastInDuel())
			    m_auras[x]->Remove();
		}
	}

	//Stop Players attacking so they don't kill the other player
	m_session->OutPacket( SMSG_CANCEL_COMBAT );
	DuelingWith->m_session->OutPacket( SMSG_CANCEL_COMBAT );

	smsg_AttackStop( DuelingWith );
	DuelingWith->smsg_AttackStop( this );

	DuelingWith->m_duelCountdownTimer = 0;
	m_duelCountdownTimer = 0;

	DuelingWith->DuelingWith = NULL;
	DuelingWith = NULL;
}

void Player::StopMirrorTimer(uint32 Type)
{
#ifdef USING_BIG_ENDIAN
	uint32 swapped = swap32(Type);
	m_session->OutPacket(SMSG_STOP_MIRROR_TIMER, 4, &swapped);
#else
	m_session->OutPacket(SMSG_STOP_MIRROR_TIMER, 4, &Type);
#endif
}

void Player::EventTeleport(uint32 mapid, float x, float y, float z)
{
	SafeTeleport(mapid, 0, LocationVector(x, y, z));
}

void Player::EventTeleportTaxi(uint32 mapid, float x, float y, float z)
{
	if(mapid == 530 && !m_session->HasFlag(ACCOUNT_FLAG_XPACK_01))
	{
		WorldPacket msg(CMSG_SERVER_BROADCAST, 50);
		msg << uint32(3) << "You must have The Burning Crusade Expansion to access this content." << uint8(0);
		m_session->SendPacket(&msg);
		RepopAtGraveyard(GetPositionX(), GetPositionY(), GetPositionZ(), GetMapId());
		return;
	}
	_Relocate(mapid, LocationVector(x, y, z), (mapid==GetMapId() ? false:true), true, 0);
	ForceZoneUpdate();
}

void Player::ApplyLevelInfo(LevelInfo* Info, uint32 Level)
{
	ASSERT(Info != NULL);

	// Apply level
	uint32 PreviousLevel = GetUInt32Value(UNIT_FIELD_LEVEL);
	SetUInt32Value(UNIT_FIELD_LEVEL, Level);

	// Set next level conditions
	SetUInt32Value(PLAYER_NEXT_LEVEL_XP, Info->XPToNextLevel);

	// Set stats
	for(uint32 i = 0; i < 5; ++i)
	{
		BaseStats[i] = Info->Stat[i];
		CalcStat(i);
	}

	// Set health / mana
	SetUInt32Value(UNIT_FIELD_HEALTH, Info->HP);
	SetUInt32Value(UNIT_FIELD_MAXHEALTH, Info->HP);
	SetPower(0, Info->Mana);
	SetUInt32Value(UNIT_FIELD_MAXPOWER1, Info->Mana);

	// Calculate talentpoints
	uint32 TalentPoints = GetUInt32Value(PLAYER_CHARACTER_POINTS1);

	if (Level >= PreviousLevel) {
		// Level is increased - talent points are only added, so no reset
		if (PreviousLevel >= 10) {
			// Every new level up will add a talent point
			TalentPoints += Level - PreviousLevel;
		} else if (Level >= 10) {
			// Only add talent points for new levels above 9
			TalentPoints += Level - 9;
		}
		SetUInt32Value(PLAYER_CHARACTER_POINTS1, TalentPoints);
	} else {
		uint32 removed = 0;
		if (Level >= 10) {
			// Every level decreased removes one talent point
			removed = PreviousLevel - Level;
		} else if (PreviousLevel >= 10) {
			// Removing all talent points from levels
			removed = PreviousLevel - 9;
		}
		if (TalentPoints < removed) {
			// Too few free talent points; resetting talents
			Reset_Talents();
		} else {
			// Remove calculated amount of free talent points
			TalentPoints -= removed;
			SetUInt32Value(PLAYER_CHARACTER_POINTS1, TalentPoints);
		}
	}

	// Set base fields
	SetUInt32Value(UNIT_FIELD_BASE_HEALTH, Info->HP);
	SetUInt32Value(UNIT_FIELD_BASE_MANA, Info->Mana);

	_UpdateMaxSkillCounts();
	UpdateStats();
	//UpdateChances();
	UpdateGlyphs();
	m_playerInfo->lastLevel = Level;

	GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL);
	sLog.outDetail("Player %s set parameters to level %u", GetName(), Level);
}

void Player::BroadcastMessage(const char* Format, ...)
{
	va_list l;
	va_start(l, Format);
	char Message[1024];
	vsnprintf(Message, 1024, Format, l);
	va_end(l);

	WorldPacket * data = sChatHandler.FillSystemMessageData(Message);
	m_session->SendPacket(data);
	delete data;
}
/*
const double BaseRating []= {
	2.5,//weapon_skill_ranged!!!!
	1.5,//defense=comba_r_1
	12,//dodge
	20,//parry=3
	5,//block=4
	10,//melee hit
	10,//ranged hit
	8,//spell hit=7
	14,//melee critical strike=8
	14,//ranged critical strike=9
	14,//spell critical strike=10
	0,//
	0,
	0,
	25,//resilience=14
	25,//resil .... meaning unknown
	25,//resil .... meaning unknown
	10,//MELEE_HASTE_RATING=17
	10,//RANGED_HASTE_RATING=18
	10,//spell_haste_rating = 19???
	2.5,//melee weapon skill==20
	2.5,//melee second hand=21

};
*/
float Player::CalcRating( uint32 index )
{
	uint32 relative_index = index - (PLAYER_FIELD_COMBAT_RATING_1);
	float rating = float(m_uint32Values[index]);
	/*if( relative_index <= 10 || ( relative_index >= 14 && relative_index <= 21 ) )
	{
		double rating = (double)m_uint32Values[index];
		int level = getLevel();
		if( level < 10 )//this is not dirty fix -> it is from wowwiki
			level = 10;
		double cost;
		if( level < 60 )
			cost = ( double( level ) - 8.0 ) / 52.0;
		else
			cost = 82.0 / ( 262.0 - 3.0 *  double( level ) );
		return float( rating / ( BaseRating[relative_index] * cost ) );
	}
	else
		return 0.0f;*/

	uint32 level = m_uint32Values[UNIT_FIELD_LEVEL];
	if( level > 100 )
		level = 100;

	CombatRatingDBC * pDBCEntry = dbcCombatRating.LookupEntryForced( relative_index * 100 + level - 1 );
	if( pDBCEntry == NULL )
		return rating;
	else
		return (rating / pDBCEntry->val);
}

bool Player::SafeTeleport(uint32 MapID, uint32 InstanceID, float X, float Y, float Z, float O)
{
	return SafeTeleport(MapID, InstanceID, LocationVector(X, Y, Z, O));
}

bool Player::SafeTeleport(uint32 MapID, uint32 InstanceID, const LocationVector & vec)
{
	SpeedCheatDelay(10000);

	if ( GetTaxiState() )
	{
		sEventMgr.RemoveEvents( this, EVENT_PLAYER_TELEPORT );
		sEventMgr.RemoveEvents( this, EVENT_PLAYER_TAXI_DISMOUNT );
		sEventMgr.RemoveEvents( this, EVENT_PLAYER_TAXI_INTERPOLATE );
		SetTaxiState( false );
		SetTaxiPath( NULL );
		UnSetTaxiPos();
		m_taxi_ride_time = 0;
		SetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID , 0 );
		RemoveFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI );
		RemoveFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_LOCK_PLAYER );
		SetPlayerSpeed( RUN, m_runSpeed );
	}
	if ( m_TransporterGUID )
	{
		Transporter * pTrans = objmgr.GetTransporter( GUID_LOPART( m_TransporterGUID ) );
		if ( pTrans && !m_lockTransportVariables )
		{
			pTrans->RemovePlayer( this );
			m_CurrentTransporter = NULL;
			m_TransporterGUID = 0;
		}
	}

	bool instance = false;
	MapInfo * mi = WorldMapInfoStorage.LookupEntry(MapID);

	if(InstanceID && (uint32)m_instanceId != InstanceID)
	{
		instance = true;
		this->SetInstanceID(InstanceID);
	}
	else if(m_mapId != MapID)
	{
		instance = true;
	}

	if (sWorld.Collision == 0) {
		// if we are mounted remove it
		if( m_MountSpellId )
			RemoveAura( m_MountSpellId );
	}

	// make sure player does not drown when teleporting from under water
	if (m_UnderwaterState & UNDERWATERSTATE_UNDERWATER)
		m_UnderwaterState &= ~UNDERWATERSTATE_UNDERWATER;

	if(flying_aura && ((m_mapId != 530) && (m_mapId != 571 || !HasSpellwithNameHash(SPELL_HASH_COLD_WEATHER_FLYING))))
	// can only fly in outlands or northrend (northrend requires cold weather flying)
	{
		RemoveAura(flying_aura);
		flying_aura = 0;
	}

	// Lookup map info
	if(mi && mi->flags & WMI_INSTANCE_XPACK_01 && !m_session->HasFlag(ACCOUNT_FLAG_XPACK_01) && !m_session->HasFlag(ACCOUNT_FLAG_XPACK_02))
	{
		WorldPacket msg(SMSG_MOTD, 50);
		msg << uint32(3) << "You must have The Burning Crusade Expansion to access this content." << uint8(0);
		m_session->SendPacket(&msg);
		return false;
	}
	if(mi && mi->flags & WMI_INSTANCE_XPACK_02 && !m_session->HasFlag(ACCOUNT_FLAG_XPACK_02))
	{
		WorldPacket msg(SMSG_MOTD, 50);
		msg << uint32(3) << "You must have Wrath of the Lich King Expansion to access this content." << uint8(0);
		m_session->SendPacket(&msg);
		return false;
	}

	// cebernic: cleanup before teleport
	// seems BFleaveOpcode was breakdown,that will be causing big BUG with player leaving from the BG
	// now this much better:D RemoveAura & BG_DESERTER going to well as you go out from BG.
	if( m_bg && m_bg->GetMapMgr() && GetMapMgr()->GetMapInfo()->mapid != MapID )
	{
		m_bg->RemovePlayer(this, false);
		m_bg = NULL;
	}

	_Relocate(MapID, vec, true, instance, InstanceID);
	SpeedCheatReset();
	ForceZoneUpdate();
	return true;
}

void Player::ForceZoneUpdate()
{
	if(!GetMapMgr()) return;

	uint16 areaId = GetMapMgr()->GetAreaID(GetPositionX(), GetPositionY());
	AreaTable * at = dbcArea.LookupEntry(areaId);
	if(!at) return;

	if(at->ZoneId && at->ZoneId != m_zoneId)
		ZoneUpdate(at->ZoneId);

	GetMapMgr()->SendInitialStates(this);
}

void Player::SafeTeleport(MapMgr * mgr, const LocationVector & vec)
{
	if( mgr ==  NULL )
	   return;

	SpeedCheatDelay(10000);

	if(flying_aura && ((m_mapId != 530) && (m_mapId != 571 || !HasSpellwithNameHash(SPELL_HASH_COLD_WEATHER_FLYING))))
	// can only fly in outlands or northrend (northrend requires cold weather flying)
	{
		RemoveAura(flying_aura);
		flying_aura=0;
	}

	if(IsInWorld())
		RemoveFromWorld();

	m_mapId = mgr->GetMapId();
	m_instanceId = mgr->GetInstanceID();
	WorldPacket data(SMSG_TRANSFER_PENDING, 20);
	data << mgr->GetMapId();
	GetSession()->SendPacket(&data);

	data.Initialize(SMSG_NEW_WORLD);
	data << mgr->GetMapId() << vec << vec.o;
	GetSession()->SendPacket(&data);

	SetPlayerStatus(TRANSFER_PENDING);
	m_sentTeleportPosition = vec;
	SetPosition(vec);
	SpeedCheatReset();
	ForceZoneUpdate();
}

void Player::SetGuildId(uint32 guildId)
{
	if(IsInWorld())
	{
		const uint32 field = PLAYER_GUILDID;
		sEventMgr.AddEvent(((Object*)this), &Object::EventSetUInt32Value, field, guildId, EVENT_PLAYER_SEND_PACKET, 1,
			1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
	else
	{
		SetUInt32Value(PLAYER_GUILDID,guildId);
	}
}

void Player::SetGuildRank(uint32 guildRank)
{
	if(IsInWorld())
	{
		const uint32 field = PLAYER_GUILDRANK;
		sEventMgr.AddEvent(((Object*)this), &Object::EventSetUInt32Value, field, guildRank, EVENT_PLAYER_SEND_PACKET, 1,
			1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
	else
	{
		SetUInt32Value(PLAYER_GUILDRANK,guildRank);
	}
}

void Player::UpdatePvPArea()
{
	AreaTable * at = dbcArea.LookupEntry(m_AreaID);
	if(at == 0)
		return;

#ifdef PVP_REALM_MEANS_CONSTANT_PVP
	//zack : This might be huge crap. I have no idea how it is on blizz but i think a pvp realm should alow me to gank anybody anywhere :(
	if(sWorld.GetRealmType() == REALM_PVP)
	{
		SetPvPFlag();
		return;
	}
#endif

	if( bGMTagOn )
	{
		if(IsPvPFlagged())
			RemovePvPFlag();
		else
			StopPvPTimer();
		RemoveFFAPvPFlag();
		return;
	}

	// This is where all the magic happens :P
	if((at->category == AREAC_ALLIANCE_TERRITORY && GetTeam() == 0) || (at->category == AREAC_HORDE_TERRITORY && GetTeam() == 1))
	{
		if(!HasFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE) && !m_pvpTimer)
		{
			// I'm flagged and I just walked into a zone of my type. Start the 5min counter.
			ResetPvPTimer();
		}
		return;
	}
	else
	{
		//Enemy city check
		if(at->AreaFlags & AREA_CITY_AREA || at->AreaFlags & AREA_CITY)
		{
			if((at->category == AREAC_ALLIANCE_TERRITORY && GetTeam() == 1) || (at->category == AREAC_HORDE_TERRITORY && GetTeam() == 0))
			{
				if(!IsPvPFlagged())
					SetPvPFlag();
				else
					StopPvPTimer();
				return;
			}
		}

		//fix for zone areas.
		if(at->ZoneId)
		{
			AreaTable * at2 = dbcArea.LookupEntry(at->ZoneId);
			if(at2 && (at2->category == AREAC_ALLIANCE_TERRITORY && GetTeam() == 0 || at2->category == AREAC_HORDE_TERRITORY && GetTeam() == 1))
			{
				if(!HasFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE) && !m_pvpTimer)
				{
					// I'm flagged and I just walked into a zone of my type. Start the 5min counter.
					ResetPvPTimer();
				}
				return;
			}
			//enemy territory check
			if(at2 && ( at2->AreaFlags & AREA_CITY_AREA || at2->AreaFlags & AREA_CITY ) )
			{
				if(at2 && (at2->category == AREAC_ALLIANCE_TERRITORY && GetTeam() == 1 || at2->category == AREAC_HORDE_TERRITORY && GetTeam() == 0))
				{
					if(!IsPvPFlagged())
						SetPvPFlag();
					else
						StopPvPTimer();
					return;
				}
			}
		}

		// I just walked into a sanctuary area
		// Force remove flag me if i'm not already.
		if(at->category == AREAC_SANCTUARY || at->AreaFlags & AREA_SANCTUARY)
		{
			if(IsPvPFlagged())
				RemovePvPFlag();
			else
				StopPvPTimer();
			RemoveFFAPvPFlag();
		}
		else
		{
			//contested territory
			if(sWorld.GetRealmType() == REALM_PVP)
			{
				//automaticaly sets pvp flag on contested territorys.
				if(!IsPvPFlagged())
					SetPvPFlag();
				else
					StopPvPTimer();
			}

			if(sWorld.GetRealmType() == REALM_PVE)
			{
				if(HasFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE))
				{
					if(!IsPvPFlagged())
						SetPvPFlag();
				}
				else if(!HasFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE) && IsPvPFlagged() && !m_pvpTimer)
				{
					ResetPvPTimer();
				}
			}

			if(at->AreaFlags & AREA_PVP_ARENA)			/* ffa pvp arenas will come later */
			{
				if(!IsPvPFlagged())
					SetPvPFlag();
				SetFFAPvPFlag();
			}
			else
			{
				RemoveFFAPvPFlag();
			}
		}
	}
}

void Player::BuildFlagUpdateForNonGroupSet(uint32 index, uint32 flag)
{
    Object *curObj;
	this->AquireInrangeLock(); //make sure to release lock before exit function !
	for (Object::InRangeSet::iterator iter = GetInRangeSetBegin(); iter != GetInRangeSetEnd();)
	{
		curObj = *iter;
		iter++;
        if(curObj->IsPlayer())
        {
            Group* pGroup = static_cast< Player* >( curObj )->GetGroup();
            if( !pGroup && pGroup != GetGroup())
            {
                BuildFieldUpdatePacket( static_cast< Player* >( curObj ), index, flag );
            }
        }
    }
	this->ReleaseInrangeLock();
}

void Player::LoginPvPSetup()
{
	// Make sure we know our area ID.
	_EventExploration();

	AreaTable * at = dbcArea.LookupEntry( ( m_AreaID != 0 ) ? m_AreaID : m_zoneId );

	if ( at != NULL && isAlive() && ( at->category == AREAC_CONTESTED || ( GetTeam() == 0 && at->category == AREAC_HORDE_TERRITORY ) || ( GetTeam() == 1 && at->category == AREAC_ALLIANCE_TERRITORY ) ) )
		CastSpell(this, PLAYER_HONORLESS_TARGET_SPELL, true);

#ifdef PVP_REALM_MEANS_CONSTANT_PVP
	//zack : This might be huge crap. I have no idea how it is on blizz but i think a pvp realm should alow me to gank anybody anywhere :(
	if(sWorld.GetRealmType() == REALM_PVP)
    {
		SetPvPFlag();
		return;
    }
#endif
}

void Player::PvPToggle()
{
    if(sWorld.GetRealmType() == REALM_PVE)
    {
	    if(m_pvpTimer > 0)
	    {
		    // Means that we typed /pvp while we were "cooling down". Stop the timer.
		    StopPvPTimer();

		    SetFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);
			RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP);

            if(!IsPvPFlagged())
				SetPvPFlag();
	    }
	    else
	    {
		    if(IsPvPFlagged())
		    {
                AreaTable * at = dbcArea.LookupEntry(m_AreaID);
                if(at && ( at->AreaFlags & AREA_CITY_AREA || at->AreaFlags & AREA_CITY ) )
                {
                    if(at && (at->category == AREAC_ALLIANCE_TERRITORY && GetTeam() == 1 || at->category == AREAC_HORDE_TERRITORY && GetTeam() == 0))
                    {
                    }
                    else
                    {
                        // Start the "cooldown" timer.
			            ResetPvPTimer();
                    }
                }
                else
                {
                    // Start the "cooldown" timer.
			        ResetPvPTimer();
                }
			    RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);
				SetFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP);
		    }
		    else
		    {
			    // Move into PvP state.
			    SetFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);
				RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP);

			    StopPvPTimer();
			    SetPvPFlag();
		    }
	    }
    }
#ifdef PVP_REALM_MEANS_CONSTANT_PVP
	//zack : This might be huge crap. I have no idea how it is on blizz but i think a pvp realm should alow me to gank anybody anywhere :(
	else if(sWorld.GetRealmType() == REALM_PVP)
    {
		SetPvPFlag();
		return;
    }
#else
    else if(sWorld.GetRealmType() == REALM_PVP)
    {
        AreaTable * at = dbcArea.LookupEntry(m_AreaID);
	    if(at == 0)
            return;

	    // This is where all the magic happens :P
        if((at->category == AREAC_ALLIANCE_TERRITORY && GetTeam() == 0) || (at->category == AREAC_HORDE_TERRITORY && GetTeam() == 1))
	    {
            if(m_pvpTimer > 0)
	        {
		        // Means that we typed /pvp while we were "cooling down". Stop the timer.
		        StopPvPTimer();

		        SetFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);
				RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP);

                if(!IsPvPFlagged())
					SetPvPFlag();
	        }
	        else
	        {
		        if(IsPvPFlagged())
		        {
                    // Start the "cooldown" timer.
			        ResetPvPTimer();

			        RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);
					SetFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP);
		        }
		        else
		        {
			        // Move into PvP state.
			        SetFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);
					RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP);

			        StopPvPTimer();
			        SetPvPFlag();
		        }
	        }
        }
        else
        {
            if(at->ZoneId)
            {
                AreaTable * at2 = dbcArea.LookupEntry(at->ZoneId);
                if(at2 && (at2->category == AREAC_ALLIANCE_TERRITORY && GetTeam() == 0 || at2->category == AREAC_HORDE_TERRITORY && GetTeam() == 1))
                {
                    if(m_pvpTimer > 0)
	                {
		                // Means that we typed /pvp while we were "cooling down". Stop the timer.
		                StopPvPTimer();

		                SetFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);
						RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP);

						if(!IsPvPFlagged())
							SetPvPFlag();
	                }
	                else
	                {
		                if(IsPvPFlagged())
		                {
			                // Start the "cooldown" timer.
			                ResetPvPTimer();

			                RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);
							SetFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP);

		                }
		                else
		                {
			                // Move into PvP state.
			                SetFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);
							RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP);

			                StopPvPTimer();
			                SetPvPFlag();
		                }
	                }
                    return;
                }
            }

            if(!HasFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE))
			{
		        SetFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);
				RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP);
			}
            else
            {
			    RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);
				SetFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP);
            }
        }
    }
#endif
}

void Player::ResetPvPTimer()
{
	 m_pvpTimer = sWorld.getIntRate(INTRATE_PVPTIMER);
}

void Player::CalculateBaseStats()
{
	if(!lvlinfo) return;

	memcpy(BaseStats, lvlinfo->Stat, sizeof(uint32) * 5);

	LevelInfo * levelone = objmgr.GetLevelInfo(this->getRace(),this->getClass(),1);
	if (levelone == NULL)
	{
		sLog.outError("%s (%d): NULL pointer", __FUNCTION__, __LINE__);
		return;
	}
	SetUInt32Value(UNIT_FIELD_MAXHEALTH, lvlinfo->HP);
	SetUInt32Value(UNIT_FIELD_BASE_HEALTH, lvlinfo->HP - (lvlinfo->Stat[2]-levelone->Stat[2])*10);
	SetUInt32Value(PLAYER_NEXT_LEVEL_XP, lvlinfo->XPToNextLevel);


	if(GetPowerType() == POWER_TYPE_MANA)
	{
		SetUInt32Value(UNIT_FIELD_BASE_MANA, lvlinfo->Mana - (lvlinfo->Stat[3]-levelone->Stat[3])*15);
		SetUInt32Value(UNIT_FIELD_MAXPOWER1, lvlinfo->Mana);
	}
}

void Player::CompleteLoading()
{
    // cast passive initial spells	  -- grep note: these shouldnt require plyr to be in world
	SpellSet::iterator itr;
	SpellEntry *info;
	SpellCastTargets targets;
	targets.m_unitTarget = this->GetGUID();
	targets.m_targetMask = 0x2;

	// warrior has to have battle stance
	if( getClass() == WARRIOR )
	{
		// battle stance passive
		CastSpell(this, dbcSpell.LookupEntry(2457), true);
	}


	for (itr = mSpells.begin(); itr != mSpells.end(); ++itr)
	{
		info = dbcSpell.LookupEntry(*itr);

		if(	info
			&& (info->Attributes & ATTRIBUTES_PASSIVE)  // passive
			&& !( info->c_is_flags & SPELL_FLAG_IS_EXPIREING_WITH_PET ) //on pet summon talents
			 )
		{
			if( info->RequiredShapeShift )
			{
				if( !( ((uint32)1 << (GetShapeShift()-1)) & info->RequiredShapeShift ) )
					continue;
			}

			Spell * spell = SpellPool.PooledNew();
			if (!spell)
				return;
			spell->Init(this,info,true,NULL);
			spell->prepare(&targets);
		}
	}

	std::list<LoginAura>::iterator i =  loginauras.begin();

	for ( ; i != loginauras.end(); i++ )
	{

		//check if we already have this aura
//		if(this->HasActiveAura((*i).id))
//			continue;
		//how many times do we intend to put this oura on us
/*		uint32 count_appearence=0;
		std::list<LoginAura>::iterator i2 =  i;
		for(;i2!=loginauras.end();i2++)
			if((*i).id==(*i2).id)
			{
				count_appearence++;
			}
*/
		SpellEntry * sp = dbcSpell.LookupEntry((*i).id);

		if ( sp->c_is_flags & SPELL_FLAG_IS_EXPIREING_WITH_PET )
			continue; //do not load auras that only exist while pet exist. We should recast these when pet is created anyway

		Aura * aura = AuraPool.PooledNew();
		if (!aura)
			return;
		aura->Init(sp,(*i).dur,this,this, false);
		//if ( !(*i).positive ) // do we need this? - vojta
		//	aura->SetNegative();

		for ( uint32 x = 0; x < 3; x++ )
		{
			if ( sp->Effect[x]==SPELL_EFFECT_APPLY_AURA )
			{
				aura->AddMod( sp->EffectApplyAuraName[x], sp->EffectBasePoints[x]+1, sp->EffectMiscValue[x], x );
			}
		}

		if ( sp->procCharges > 0 && (*i).charges > 0 )
		{
			Aura * a = NULL;
			for ( uint32 x = 0; x < (*i).charges - 1; x++ )
			{
				a = AuraPool.PooledNew();
				if (!a)
					return;
				a->Init( sp, (*i).dur, this, this, false );
				this->AddAura( a );
				a = NULL;
			}
			if ( m_chargeSpells.find( sp->Id ) == m_chargeSpells.end() && !( sp->procFlags & PROC_REMOVEONUSE ) )
			{
				SpellCharge charge;
				if ( sp->proc_interval == 0 )
					charge.count = (*i).charges;
				else
					charge.count = sp->procCharges;
				charge.spellId = sp->Id;
				charge.ProcFlag = sp->procFlags;
				charge.lastproc = 0;
				m_chargeSpells.insert( make_pair( sp->Id , charge ) );
			}
		}
		this->AddAura( aura );
		//Somehow we should restore number of appearence. Right now i have no idea how :(
//		if(count_appearence>1)
//			this->AddAuraVisual((*i).id,count_appearence-1,a->IsPositive());
	}

	// this needs to be after the cast of passive spells, because it will cast ghost form, after the remove making it in ghost alive, if no corpse.
	//death system checkout
	if(GetUInt32Value(UNIT_FIELD_HEALTH) <= 0 && !HasFlag(PLAYER_FLAGS, PLAYER_FLAG_DEATH_WORLD_ENABLE))
	{
		setDeathState(CORPSE);
	}
	else if(HasFlag(PLAYER_FLAGS, PLAYER_FLAG_DEATH_WORLD_ENABLE))
	{
		// Check if we have an existing corpse.
		Corpse * corpse = objmgr.GetCorpseByOwner(GetLowGUID());
		if(corpse == 0)
		{
			sEventMgr.AddEvent(this, &Player::RepopAtGraveyard, GetPositionX(),GetPositionY(),GetPositionZ(), GetMapId(), EVENT_PLAYER_CHECKFORCHEATS, 1000, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		}
		else
		{
			// Set proper deathstate
			setDeathState(CORPSE);
		}
	}

	if( IsDead() )
	{
		if ( myCorpse!=NULL ) {
			// cebernic: tempfix. This send a counter for player with just logging in.
			// TODO: counter will be follow with death time.
			myCorpse->ResetDeathClock();
			WorldPacket SendCounter( SMSG_CORPSE_RECLAIM_DELAY, 4 );
			SendCounter << (uint32)( CORPSE_RECLAIM_TIME_MS );
			GetSession()->SendPacket( &SendCounter );
		}
		//RepopRequestedPlayer();
		//sEventMgr.AddEvent(this, &Player::RepopRequestedPlayer, EVENT_PLAYER_CHECKFORCHEATS, 2000, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}

	SpawnActivePet();

	// useless logon spell
	Spell *logonspell = SpellPool.PooledNew();
	if (!logonspell)
		return;
	logonspell->Init(this, dbcSpell.LookupEntry(836), false, NULL);
	logonspell->prepare(&targets);

	// Banned
	if(IsBanned())
	{
		Kick(10000);
		BroadcastMessage("This character is not allowed to play.");
		BroadcastMessage("You have been banned for: %s", GetBanReason().c_str());
	}

	if(m_playerInfo->m_Group)
	{
		sEventMgr.AddEvent(this, &Player::EventGroupFullUpdate, EVENT_UNK, 100, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		//m_playerInfo->m_Group->Update();
	}

	if(raidgrouponlysent)
	{
		WorldPacket data2(SMSG_RAID_GROUP_ONLY, 8);
		data2 << uint32(0xFFFFFFFF) << uint32(0);
		GetSession()->SendPacket(&data2);
		raidgrouponlysent=false;
	}

	sInstanceMgr.BuildSavedInstancesForPlayer(this);
	CombatStatus.UpdateFlag();
	//sEventMgr.AddEvent(this,&Player::SendAllAchievementData,EVENT_SEND_ACHIEVEMNTS_TO_PLAYER,ACHIEVEMENT_SEND_DELAY,1,0);
	sEventMgr.AddEvent(static_cast< Unit* >(this),&Unit::UpdatePowerAmm,EVENT_SEND_PACKET_TO_PLAYER_AFTER_LOGIN,LOGIN_CIENT_SEND_DELAY,1,0);
}

void Player::OnWorldPortAck()
{
	//only rezz if player is porting to a instance portal
	MapInfo *pMapinfo = WorldMapInfoStorage.LookupEntry(GetMapId());
	if(IsDead())
	{
		if(pMapinfo)
		{
			if(pMapinfo->type != INSTANCE_NULL)
			{
				// resurrector = 0; // ceberwow: This should be seriously BUG.It makes player statz stackable.
				ResurrectPlayer();
			}
		}
	}

	if(pMapinfo)
	{
		WorldPacket data(4);
		if(pMapinfo->HasFlag(WMI_INSTANCE_WELCOME) && GetMapMgr())
		{
			std::string welcome_msg;
			welcome_msg = string(GetSession()->LocalizedWorldSrv(62))+" ";
			welcome_msg += string(GetSession()->LocalizedMapName(pMapinfo->mapid));
			welcome_msg += ". ";
			if(pMapinfo->type != INSTANCE_NONRAID && !(pMapinfo->type == INSTANCE_ARENA && iInstanceType >= MODE_HEROIC) && m_mapMgr->pInstance)
			{
				/*welcome_msg += "This instance is scheduled to reset on ";
				welcome_msg += asctime(localtime(&m_mapMgr->pInstance->m_expiration));*/
				welcome_msg += string(GetSession()->LocalizedWorldSrv(66))+" ";
				welcome_msg += ConvertTimeStampToDataTime((uint32)m_mapMgr->pInstance->m_expiration);
			}
			sChatHandler.SystemMessage(m_session, welcome_msg.c_str());
		}
	}

	SpeedCheatReset();
}

void Player::ModifyBonuses( uint32 type, int32 val, bool apply )
{
	// Added some updateXXXX calls so when an item modifies a stat they get updated
	// also since this is used by auras now it will handle it for those
	int32 _val = val;
	if( !apply )
		val = -val;

	switch ( type )
		{
		case POWER:
			{
				ModUnsigned32Value( UNIT_FIELD_MAXPOWER1, val );
				m_manafromitems += val;
			}break;
		case HEALTH:
			{
				ModUnsigned32Value( UNIT_FIELD_MAXHEALTH, val );
				m_healthfromitems += val;
			}break;
		case AGILITY:	//modify agility
		case STRENGTH:	//modify strength
		case INTELLECT:	//modify intellect
		case SPIRIT:	//modify spirit
		case STAMINA:	//modify stamina
			{
				uint8 convert[] = {1, 0, 3, 4, 2};
				if( _val > 0 )
					FlatStatModPos[ convert[ type - 3 ] ] += val;
				else
					FlatStatModNeg[ convert[ type - 3 ] ] -= val;
				CalcStat( convert[ type - 3 ] );
			}break;
		case WEAPON_SKILL_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_SKILL, val ); // ranged
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_MAIN_HAND_SKILL, val ); // melee main hand
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_OFF_HAND_SKILL, val ); // melee off hand
			}break;
		case DEFENSE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_DEFENCE, val );
			}break;
		case DODGE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_DODGE, val );
			}break;
		case PARRY_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_PARRY, val );
			}break;
		case SHIELD_BLOCK_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_BLOCK, val );
			}break;
		case MELEE_HIT_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_HIT, val );
			}break;
		case RANGED_HIT_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_HIT, val );
			}break;
		case SPELL_HIT_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_HIT, val );
			}break;
		case MELEE_CRITICAL_STRIKE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_CRIT, val );
			}break;
		case RANGED_CRITICAL_STRIKE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_CRIT, val );
			}break;
		case SPELL_CRITICAL_STRIKE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_CRIT, val );
			}break;
		case MELEE_HIT_AVOIDANCE_RATING:
			{
	//			ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_HIT_AVOIDANCE, val );
			}break;
		case RANGED_HIT_AVOIDANCE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_HIT_AVOIDANCE, val );
			}break;
		case SPELL_HIT_AVOIDANCE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_HIT_AVOIDANCE, val );
			}break;
		case MELEE_CRITICAL_AVOIDANCE_RATING:
			{

			}break;
		case RANGED_CRITICAL_AVOIDANCE_RATING:
			{

			}break;
		case SPELL_CRITICAL_AVOIDANCE_RATING:
			{

			}break;
		case MELEE_HASTE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_HASTE, val );//melee
			}break;
		case RANGED_HASTE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_HASTE, val );//ranged
			}break;
		case SPELL_HASTE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_HASTE, val );//spell
			}break;
		case HIT_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_HIT, val );//melee
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_HIT, val );//ranged
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_HIT, val ); //Spell
			}break;
		case CRITICAL_STRIKE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_CRIT, val );//melee
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_CRIT, val );//ranged
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_CRIT, val ); //spell
			}break;
		case HIT_AVOIDANCE_RATING:// this is guessed based on layout of other fields
			{
//				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_HIT_AVOIDANCE, val );//melee
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_HIT_AVOIDANCE, val );//ranged
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_HIT_AVOIDANCE, val );//spell
			}break;
		case EXPERTISE_RATING:
		case EXPERTISE_RATING_2:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_EXPERTISE, val );
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_EXPERTISE2, val );
				//ModUnsigned32Value( PLAYER_EXPERTISE, val );
			}break;
		case RESILIENCE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_CRIT_RESILIENCE, val );//melee
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_CRIT_RESILIENCE, val );//ranged
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_CRIT_RESILIENCE, val );//spell
			}break;
		case HASTE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_HASTE, val );//melee
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_HASTE, val );//ranged
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_HASTE, val ); // Spell
			}break;
		case ATTACK_POWER:
			{
				ModUnsigned32Value(UNIT_FIELD_ATTACK_POWER_MODS, val);
				ModUnsigned32Value(UNIT_FIELD_RANGED_ATTACK_POWER_MODS, val);
			}break;
		case RANGED_ATTACK_POWER:
			{
				ModUnsigned32Value( UNIT_FIELD_RANGED_ATTACK_POWER_MODS, val );
			}break;
		case FERAL_ATTACK_POWER:
			{

			}break;
		case SPELL_HEALING_DONE:
			{
				for(uint32 school=1;school < 7; ++school)
				{
					HealDoneMod[school] += val;
				}
				ModUnsigned32Value( PLAYER_FIELD_MOD_HEALING_DONE_POS, val );
			}break;
		case SPELL_DAMAGE_DONE:
			{
				for(uint32 school=1;school < 7; ++school)
				{
					ModUnsigned32Value( PLAYER_FIELD_MOD_DAMAGE_DONE_POS + school, val );
				}
			}break;
		case MANA_REGENERATION:
			{
				m_ModInterrMRegen += val;
			}break;
		case ARMOR_PENETRATION_RATING:
			{
//				ModUnsigned32Value(PLAYER_RATING_MODIFIER_ARMOR_PENETRATION_RATING, val);
			}break;
		case SPELL_POWER:
			{
				for( uint8 school = 1; school < 7; ++school )
				{
					ModUnsigned32Value( PLAYER_FIELD_MOD_DAMAGE_DONE_POS + school, val );
					HealDoneMod[school] += val;
				}

				ModUnsigned32Value( PLAYER_FIELD_MOD_HEALING_DONE_POS, uint32( float( val ) * 1.88f ) );
			}break;
		}
}

bool Player::CanSignCharter(Charter * charter, Player * requester)
{
	if(charter==NULL || requester==NULL)
		return false;
	if(charter->CharterType >= CHARTER_TYPE_ARENA_2V2 && m_arenaTeams[charter->CharterType-1] != NULL)
		return false;

	if(charter->CharterType == CHARTER_TYPE_GUILD && IsInGuild())
		return false;

	if(m_charters[charter->CharterType] || requester->GetTeam() != GetTeam() || this == requester )
		return false;
	else
		return true;
}

void Player::SaveAuras(stringstream &ss)
{
	uint32 charges = 0, prevX = 0;
	//cebernic: save all auras why only just positive?
	for ( uint32 x = MAX_POSITIVE_AURAS_EXTEDED_START; x < MAX_NEGATIVE_AURAS_EXTEDED_END; x++ )
	{
		if ( m_auras[x] != NULL && m_auras[x]->GetTimeLeft() > 3000 )
		{
			Aura * aur = m_auras[x];
			for ( uint32 i = 0; i < 3; ++i )
			{
				if(aur->m_spellProto->Effect[i] == SPELL_EFFECT_APPLY_AREA_AURA || aur->m_spellProto->Effect[i] == SPELL_EFFECT_APPLY_AREA_AURA2 || aur->m_spellProto->Effect[i] == SPELL_EFFECT_ADD_FARSIGHT)
				{
					continue;
					break;
				}
			}

			if( aur->pSpellId )
				continue; //these auras were gained due to some proc. We do not save these eighter to avoid exploits of not removing them

			if ( aur->m_spellProto->c_is_flags & SPELL_FLAG_IS_EXPIREING_WITH_PET )
				continue;

			//we are going to cast passive spells anyway on login so no need to save auras for them
			if ( aur->IsPassive() && !( aur->GetSpellProto()->AttributesEx & 1024 ) )
				continue;

			if ( charges > 0 && aur->GetSpellId() != m_auras[prevX]->GetSpellId() )
			{
				ss << m_auras[prevX]->GetSpellId() << "," << m_auras[prevX]->GetTimeLeft() << "," << m_auras[prevX]->IsPositive() << "," << charges << ",";
				charges = 0;
			}

			if ( aur->GetSpellProto()->procCharges == 0 )
				ss << aur->GetSpellId() << "," << aur->GetTimeLeft() << "," << aur->IsPositive() << "," << 0 << ",";
			else
				charges++;

			prevX = x;
		}
	}
	if ( charges > 0 && m_auras[prevX] != NULL )
	{
		ss << m_auras[prevX]->GetSpellId() << "," << m_auras[prevX]->GetTimeLeft() << "," << m_auras[prevX]->IsPositive() << "," << charges << ",";
	}
}

void Player::SetShapeShift(uint8 ss)
{
	uint8 old_ss = GetByte( UNIT_FIELD_BYTES_2, 3 );
	SetByte( UNIT_FIELD_BYTES_2, 3, ss );

	//remove auras that we should not have
	for( uint32 x = MAX_TOTAL_AURAS_START; x < MAX_TOTAL_AURAS_END; x++ )
	{
		if( m_auras[x] != NULL )
		{
			uint32 reqss = m_auras[x]->GetSpellProto()->RequiredShapeShift;
			if( reqss != 0 && m_auras[x]->IsPositive() )
			{
				if( old_ss > 0 && old_ss != 28 )	// 28 = FORM_SHADOW - Didn't find any aura that required this form
													// not sure why all priest spell proto's RequiredShapeShift are set [to 134217728]
				{
					if(  ( ((uint32)1 << (old_ss-1)) & reqss ) &&		// we were in the form that required it
						!( ((uint32)1 << (ss-1) & reqss) ) )			// new form doesnt have the right form
					{
						m_auras[x]->Remove();
						continue;
					}
				}
			}

			if( this->getClass() == DRUID )
			{
				switch( m_auras[x]->GetSpellProto()->MechanicsType)
				{
                case MECHANIC_ROOTED: //Rooted
                case MECHANIC_ENSNARED: //Movement speed
                case MECHANIC_POLYMORPHED:  //Polymorphed
					{
						m_auras[x]->Remove();
					}
					break;
				default:
					break;
				}

				/*Shady: is this check neccessary? anyway m_auras[x]!=NULL check already done in next iteration. Commented*/
				//if( m_auras[x] == NULL )
				//	break;
			}
		}
	}

	// apply any talents/spells we have that apply only in this form.
	set<uint32>::iterator itr;
	SpellEntry * sp;
	Spell * spe;
	SpellCastTargets t(GetGUID());

	for( itr = mSpells.begin(); itr != mSpells.end(); ++itr )
	{
		sp = dbcSpell.LookupEntry( *itr );
		if( sp->apply_on_shapeshift_change || sp->Attributes & 64 )		// passive/talent
		{
			if( sp->RequiredShapeShift && ((uint32)1 << (ss-1)) & sp->RequiredShapeShift )
			{
				spe = SpellPool.PooledNew();
				if (!spe)
					return;
				spe->Init( this, sp, true, NULL );
				spe->prepare( &t );
			}
		}
	}

	// now dummy-handler stupid hacky fixed shapeshift spells (leader of the pack, etc)
	for( itr = mShapeShiftSpells.begin(); itr != mShapeShiftSpells.end(); ++itr )
	{
		sp = dbcSpell.LookupEntry( *itr );
		if( sp->RequiredShapeShift && ((uint32)1 << (ss-1)) & sp->RequiredShapeShift )
		{
			spe = SpellPool.PooledNew();
			if (!spe)
				return;
			spe->Init( this, sp, true, NULL );
			spe->prepare( &t );
		}
	}

	UpdateStats();
	UpdateChances();
}

void Player::CalcDamage()
{
	float delta;
	float r;
	int ss = GetShapeShift();
/////////////////MAIN HAND
		float ap_bonus = GetAP()/14000.0f;
		delta = (float)GetUInt32Value( PLAYER_FIELD_MOD_DAMAGE_DONE_POS ) - (float)GetUInt32Value( PLAYER_FIELD_MOD_DAMAGE_DONE_NEG );

		if(IsInFeralForm())
		{
			float tmp = 1; // multiplicative damage modifier
			for (map<uint32, WeaponModifier>::iterator i = damagedone.begin(); i!=damagedone.end();i++)
			{
				if (i->second.wclass == (uint32) - 1) // applying only "any weapon" modifiers
					tmp += i->second.value;
			}
			uint32 lev = getLevel();
			float feral_damage; // average base damage before bonuses and modifiers
			uint32 x; // itemlevel of the two hand weapon with dps equal to cat or bear dps

			if (ss == FORM_CAT)
			{
				if (lev < 42) x = lev - 1;
				else if (lev < 46) x = lev;
				else if (lev < 49) x = 2 * lev - 45;
				else if (lev < 60) x = lev + 4;
				else x = 64;

				// 3rd grade polinom for calculating blue two-handed weapon dps based on itemlevel (from Hyzenthlei)
				if (x <= 28) feral_damage = 1.563e-03f * x*x*x - 1.219e-01f * x*x + 3.802e+00f * x - 2.227e+01f;
				else if (x <= 41) feral_damage = -3.817e-03f * x*x*x + 4.015e-01f * x*x - 1.289e+01f * x + 1.530e+02f;
				else feral_damage = 1.829e-04f * x*x*x - 2.692e-02f * x*x + 2.086e+00f * x - 1.645e+01f;

				r = feral_damage * 0.79f + delta + ap_bonus * 1000.0f;
				r *= tmp;
				SetFloatValue(UNIT_FIELD_MINDAMAGE,r>0?r:0);

				r = feral_damage * 1.21f + delta + ap_bonus * 1000.0f;
				r *= tmp;
				SetFloatValue(UNIT_FIELD_MAXDAMAGE,r>0?r:0);
			}
			else // Bear or Dire Bear Form
			{
				if (ss == FORM_BEAR) x = lev;
				else x = lev + 5; // DIRE_BEAR dps is slightly better than bear dps
				if (x > 70) x = 70;

				// 3rd grade polinom for calculating green two-handed weapon dps based on itemlevel (from Hyzenthlei)
				if (x <= 30) feral_damage = 7.638e-05f * x*x*x + 1.874e-03f * x*x + 4.967e-01f * x + 1.906e+00f;
				else if (x <= 44) feral_damage = -1.412e-03f * x*x*x + 1.870e-01f * x*x - 7.046e+00f * x + 1.018e+02f;
				else feral_damage = 2.268e-04f * x*x*x - 3.704e-02f * x*x + 2.784e+00f * x - 3.616e+01f;
				feral_damage *= 2.5f; // Bear Form attack speed

				r = feral_damage * 0.79f + delta + ap_bonus * 2500.0f;
				r *= tmp;
				SetFloatValue(UNIT_FIELD_MINDAMAGE,r>0?r:0);

				r = feral_damage * 1.21f + delta + ap_bonus * 2500.0f;
				r *= tmp;
				SetFloatValue(UNIT_FIELD_MAXDAMAGE,r>0?r:0);
			}

			return;
		}

//////no druid ss
		uint32 speed=2000;
		Item *it = GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);

		if(!disarmed)
		{
			if(it)
				speed = it->GetProto()->Delay;
		}

		float bonus=ap_bonus*speed;
		float tmp = 1;
		map<uint32, WeaponModifier>::iterator i;
		for(i = damagedone.begin();i!=damagedone.end();i++)
		{
			if((i->second.wclass == (uint32)-1) || //any weapon
				(it && ((1 << it->GetProto()->SubClass) & i->second.subclass) )
				)
					tmp+=i->second.value;
		}

		r = BaseDamage[0]+delta+bonus;
		r *= tmp;
		SetFloatValue(UNIT_FIELD_MINDAMAGE,r>0?r:0);

		r = BaseDamage[1]+delta+bonus;
		r *= tmp;
		SetFloatValue(UNIT_FIELD_MAXDAMAGE,r>0?r:0);

		uint32 cr = 0;
		if( it )
		{
			if( static_cast< Player* >( this )->m_wratings.size() )
			{
				std::map<uint32, uint32>::iterator itr = m_wratings.find( it->GetProto()->SubClass );
				if( itr != m_wratings.end() )
					cr=itr->second;
			}
		}
		SetUInt32Value( PLAYER_RATING_MODIFIER_MELEE_MAIN_HAND_SKILL, cr );
		/////////////// MAIN HAND END

		/////////////// OFF HAND START
		cr = 0;
		it = static_cast< Player* >( this )->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_OFFHAND);
		if(it)
		{
			if(!disarmed)
			{
				speed =it->GetProto()->Delay;
			}
			else speed  = 2000;

			bonus = ap_bonus * speed;
			i = damagedone.begin();
			tmp = 1;
			for(;i!=damagedone.end();i++)
			{
				if((i->second.wclass==(uint32)-1) || //any weapon
					(( (1 << it->GetProto()->SubClass) & i->second.subclass)  )
					)
					tmp+=i->second.value;
			}

			r = (BaseOffhandDamage[0]+delta+bonus)*offhand_dmg_mod;
			r *= tmp;
			SetFloatValue(UNIT_FIELD_MINOFFHANDDAMAGE,r>0?r:0);
			r = (BaseOffhandDamage[1]+delta+bonus)*offhand_dmg_mod;
			r *= tmp;
			SetFloatValue(UNIT_FIELD_MAXOFFHANDDAMAGE,r>0?r:0);
			if(m_wratings.size ())
			{
				std::map<uint32, uint32>::iterator itr=m_wratings.find(it->GetProto()->SubClass);
				if(itr!=m_wratings.end())
					cr=itr->second;
			}
		}
		SetUInt32Value( PLAYER_RATING_MODIFIER_MELEE_OFF_HAND_SKILL, cr );

/////////////second hand end
///////////////////////////RANGED
		cr=0;
		if((it = GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED)) != 0)
		{
			i = damagedone.begin();
			tmp = 1;
			for(;i != damagedone.end();i++)
			{
				if(
					(i->second.wclass == (uint32)-1) || //any weapon
					( ((1 << it->GetProto()->SubClass) & i->second.subclass)  )
					)
				{
					tmp+=i->second.value;
				}
			}

			if(it->GetProto()->SubClass != 19)//wands do not have bonuses from RAP & ammo
			{
//				ap_bonus = (GetUInt32Value(UNIT_FIELD_RANGED_ATTACK_POWER)+(int32)GetUInt32Value(UNIT_FIELD_RANGED_ATTACK_POWER_MODS))/14000.0;
				//modified by Zack : please try to use premade functions if possible to avoid forgetting stuff
				ap_bonus = GetRAP()/14000.0f;
				bonus = ap_bonus*it->GetProto()->Delay;

				if( GetUInt32Value(PLAYER_AMMO_ID) && !m_requiresNoAmmo )
				{
					ItemPrototype * xproto=ItemPrototypeStorage.LookupEntry(GetUInt32Value(PLAYER_AMMO_ID));
					if(xproto)
					{
						bonus+=((xproto->Damage[0].Min+xproto->Damage[0].Max)*it->GetProto()->Delay)/2000.0f;
					}
				}
			}else bonus =0;

			r = BaseRangedDamage[0]+delta+bonus;
			r *= tmp;
			SetFloatValue(UNIT_FIELD_MINRANGEDDAMAGE,r>0?r:0);

			r = BaseRangedDamage[1]+delta+bonus;
			r *= tmp;
			SetFloatValue(UNIT_FIELD_MAXRANGEDDAMAGE,r>0?r:0);


			if(m_wratings.size ())
			{
				std::map<uint32, uint32>::iterator i=m_wratings.find(it->GetProto()->SubClass);
				if(i != m_wratings.end())
					cr=i->second;
			}

		}
		SetUInt32Value( PLAYER_RATING_MODIFIER_RANGED_SKILL, cr );

/////////////////////////////////RANGED end
		if( GetSummon() )
			GetSummon()->CalcDamage();//Re-calculate pet's too

}

uint32 Player::GetMainMeleeDamage(uint32 AP_owerride)
{
	float r;
	int ss = GetShapeShift();
/////////////////MAIN HAND
	float ap_bonus;
	if(AP_owerride)
		ap_bonus = AP_owerride/14000.0f;
	else
		ap_bonus = GetAP()/14000.0f;
	if(IsInFeralForm())
	{
		if(ss == FORM_CAT)
			r = ap_bonus * 1000.0f;
		else
			r = ap_bonus * 2500.0f;
		return float2int32(r);
	}
//////no druid ss
	uint32 speed=2000;
	Item *it = GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
	if(!disarmed)
	{
		if(it)
			speed = it->GetProto()->Delay;
	}
	r = ap_bonus*speed;
	return float2int32(r);
}

void Player::EventPortToGM(Player *p)
{
	SafeTeleport(p->GetMapId(),p->GetInstanceID(),p->GetPosition());
}

void Player::UpdateComboPoints()
{
	// fuck bytebuffers :D
	unsigned char buffer[10];
	uint16 c = 2;

	// check for overflow
	if(m_comboPoints > 5)
		m_comboPoints = 5;

	if(m_comboPoints < 0)
		m_comboPoints = 0;

	if(m_comboTarget != 0)
	{
		Unit * target = (m_mapMgr != NULL) ? m_mapMgr->GetUnit(m_comboTarget) : NULL;
		if(!target || target->IsDead() || GetSelection() != m_comboTarget)
		{
			buffer[0] = buffer[1] = 0;
		}
		else
		{
			c = FastGUIDPack(m_comboTarget, buffer, 0);
			buffer[c++] = m_comboPoints;
		}
	}
	else
		buffer[0] = buffer[1] = 0;

	m_session->OutPacket(SMSG_UPDATE_COMBO_POINTS, c, buffer);
}

void Player::SendAreaTriggerMessage(const char * message, ...)
{
	va_list ap;
	va_start(ap, message);
	char msg[500];
	vsnprintf(msg, 500, message, ap);
	va_end(ap);

	WorldPacket data(SMSG_AREA_TRIGGER_MESSAGE, 6 + strlen(msg));
	data << (uint32)0 << msg << (uint8)0x00;
	m_session->SendPacket(&data);
}

void Player::removeSoulStone()
{
	if(!this->SoulStone) return;
	uint32 sSoulStone = 0;
	switch(this->SoulStone)
	{
	case 3026:
		{
			sSoulStone = 20707;
		}break;
	case 20758:
		{
			sSoulStone = 20762;
		}break;
	case 20759:
		{
			sSoulStone = 20763;
		}break;
	case 20760:
		{
			sSoulStone = 20764;
		}break;
	case 20761:
		{
			sSoulStone = 20765;
		}break;
	case 27240:
		{
			sSoulStone = 27239;
		}break;
	}
	this->RemoveAura(sSoulStone);
	this->SoulStone = this->SoulStoneReceiver = 0; //just incase
}

void Player::SoftDisconnect()
{
      sEventMgr.RemoveEvents(this, EVENT_PLAYER_SOFT_DISCONNECT);
	  WorldSession *session=GetSession();
      session->LogoutPlayer(true);
	  session->Disconnect();
}

void Player::SetNoseLevel()
{
	// Set the height of the player
	switch (getRace())
	{
		case RACE_HUMAN:
		// female
			if (getGender()) m_noseLevel = 1.72f;
			// male
			else m_noseLevel = 1.78f;
		break;
		case RACE_ORC:
			if (getGender()) m_noseLevel = 1.82f;
			else m_noseLevel = 1.98f;
		break;
		case RACE_DWARF:
		if (getGender()) m_noseLevel = 1.27f;
			else m_noseLevel = 1.4f;
		break;
		case RACE_NIGHTELF:
			if (getGender()) m_noseLevel = 1.84f;
			else m_noseLevel = 2.13f;
		break;
		case RACE_UNDEAD:
			if (getGender()) m_noseLevel = 1.61f;
			else m_noseLevel = 1.8f;
		break;
		case RACE_TAUREN:
			if (getGender()) m_noseLevel = 2.48f;
			else m_noseLevel = 2.01f;
		break;
		case RACE_GNOME:
			if (getGender()) m_noseLevel = 1.06f;
			else m_noseLevel = 1.04f;
		break;
		case RACE_TROLL:
			if (getGender()) m_noseLevel = 2.02f;
			else m_noseLevel = 1.93f;
		break;
		case RACE_BLOODELF:
			if (getGender()) m_noseLevel = 1.83f;
			else m_noseLevel = 1.93f;
		break;
		case RACE_DRAENEI:
			if (getGender()) m_noseLevel = 2.09f;
			else m_noseLevel = 2.36f;
		break;
	}
}

void Player::Possess(Unit * pTarget)
{
	if( m_CurrentCharm)
		return;

	m_CurrentCharm = pTarget->GetGUID();
	if(pTarget->GetTypeId() == TYPEID_UNIT)
	{
		// unit-only stuff.
		pTarget->setAItoUse(false);
		pTarget->GetAIInterface()->StopMovement(0);
		pTarget->m_redirectSpellPackets = this;
	}

	m_noInterrupt++;
	SetUInt64Value(UNIT_FIELD_CHARM, pTarget->GetGUID());
	SetUInt64Value(PLAYER_FARSIGHT, pTarget->GetGUID());

	pTarget->SetUInt64Value(UNIT_FIELD_CHARMEDBY, GetGUID());
	pTarget->SetCharmTempVal(pTarget->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
	pTarget->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
	pTarget->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED_CREATURE);

	SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOCK_PLAYER);

	/* send "switch mover" packet */
	WorldPacket data1( SMSG_CLIENT_CONTROL_UPDATE, 10 );
	data1 << pTarget->GetNewGUID() << uint8(1);
	m_session->SendPacket(&data1);

	/* update target faction set */
	pTarget->_setFaction();
	pTarget->UpdateOppFactionSet();


	/* build + send pet_spells packet */
	if(pTarget->m_temp_summon)
		return;

	if( !( pTarget->IsPet() && static_cast< Pet* >( pTarget ) == m_Summon ) )
	{
		list<uint32> avail_spells;
		//Steam Tonks
		if(pTarget->GetUInt32Value(OBJECT_FIELD_ENTRY) == 15328)
		{
			uint32 rand_spellid = TonkSpecials[RandomUInt(3)];
			avail_spells.push_back(CANNON);
			avail_spells.push_back(MORTAR);
			avail_spells.push_back(rand_spellid);
			avail_spells.push_back(NITROUS);
		}
		else
		{
			for(list<AI_Spell*>::iterator itr = pTarget->GetAIInterface()->m_spells.begin(); itr != pTarget->GetAIInterface()->m_spells.end(); ++itr)
			{
				if((*itr)->agent == AGENT_SPELL)
					avail_spells.push_back((*itr)->spell->Id);
			}
		}
		list<uint32>::iterator itr = avail_spells.begin();

		WorldPacket data(SMSG_PET_SPELLS, pTarget->GetAIInterface()->m_spells.size() * 4 + 20);
		data << pTarget->GetGUID();
		data << uint32(0x00000000);//unk1
		data << uint32(0x00000101);//unk2
		data << uint32(0x00000100);//unk3

		// First spell is attack.
		data << uint32(PET_SPELL_ATTACK);

		// Send the actionbar
		for(uint32 i = 1; i < 10; ++i)
		{
			if(itr != avail_spells.end())
			{
				data << uint16((*itr)) << uint16(DEFAULT_SPELL_STATE);
				++itr;
			}
			else
				data << uint16(0) << uint8(0) << uint8(i+5);
		}
		// Send the rest of the spells.
		data << uint8(avail_spells.size());
		for(itr = avail_spells.begin(); itr != avail_spells.end(); ++itr)
			data << uint16(*itr) << uint16(DEFAULT_SPELL_STATE);

		data << uint64(0);
		m_session->SendPacket(&data);
	}

}

void Player::UnPossess()
{
	if( /*m_Summon ||*/ !m_CurrentCharm)
		return;

	Unit * pTarget = GetMapMgr()->GetUnit( m_CurrentCharm );
	if( !pTarget )
		return;

	m_CurrentCharm = 0;

	SpeedCheatReset();

	if(pTarget->GetTypeId() == TYPEID_UNIT)
	{
		// unit-only stuff.
		pTarget->setAItoUse(true);
		pTarget->m_redirectSpellPackets = 0;
	}

	m_noInterrupt--;
	SetUInt64Value(PLAYER_FARSIGHT, 0);
	SetUInt64Value(UNIT_FIELD_CHARM, 0);
	pTarget->SetUInt64Value(UNIT_FIELD_CHARMEDBY, 0);

	RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOCK_PLAYER);
	pTarget->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED_CREATURE);
	pTarget->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, pTarget->GetCharmTempVal());
	pTarget->_setFaction();
	pTarget->UpdateOppFactionSet();

	/* send "switch mover" packet */
	WorldPacket data(SMSG_CLIENT_CONTROL_UPDATE, 10);
	data << pTarget->GetNewGUID() << uint8(0);
	m_session->SendPacket(&data);

	if(pTarget->m_temp_summon)
		return;

	if( !( pTarget->IsPet() && static_cast< Pet* >( pTarget ) == m_Summon ) )
	{
		data.Initialize( SMSG_PET_SPELLS );
		data << uint64(0);
		data << uint32(0);
		m_session->SendPacket( &data );
	}
}

void Player::SummonRequest(uint32 Requestor, uint32 ZoneID, uint32 MapID, uint32 InstanceID, const LocationVector & Position)
{
	m_summonInstanceId = InstanceID;
	m_summonPos = Position;
	m_summoner = Requestor;
	m_summonMapId = MapID;

	WorldPacket data(SMSG_SUMMON_REQUEST, 16);
	data << uint64(Requestor) << ZoneID << uint32(120000);		// 2 minutes
	m_session->SendPacket(&data);
}

void Player::RemoveFromBattlegroundQueue()
{
	if(!m_pendingBattleground)
		return;

	m_pendingBattleground->RemovePendingPlayer(this);
	sChatHandler.SystemMessage(m_session, "You were removed from the queue for the battleground for not joining after 1 minute 20 seconds.");
	m_pendingBattleground = 0;
}

void Player::_AddSkillLine(uint32 SkillLine, uint32 Curr_sk, uint32 Max_sk)
{
	skilllineentry * CheckedSkill = dbcSkillLine.LookupEntry(SkillLine);
	if (!CheckedSkill) //skill doesn't exist, exit here
		return;

	// force to be within limits
#if PLAYER_LEVEL_CAP==80
	Curr_sk = ( Curr_sk > 450 ? 450 : ( Curr_sk <1 ? 1 : Curr_sk ) );
	Max_sk = ( Max_sk > 450 ? 450 : Max_sk );
#else
	Curr_sk = ( Curr_sk > 375 ? 375 : ( Curr_sk <1 ? 1 : Curr_sk ) );
	Max_sk = ( Max_sk > 375 ? 375 : Max_sk );
#endif
	ItemProf * prof;
	SkillMap::iterator itr = m_skills.find(SkillLine);
	if(itr != m_skills.end())
	{
		if( (Curr_sk > itr->second.CurrentValue && Max_sk >= itr->second.MaximumValue) || (Curr_sk == itr->second.CurrentValue && Max_sk > itr->second.MaximumValue) )
		{
			itr->second.CurrentValue = Curr_sk;
			itr->second.MaximumValue = Max_sk;
			_UpdateMaxSkillCounts();
		}
	}
	else
	{
		PlayerSkill inf;
		inf.Skill = CheckedSkill;
		inf.MaximumValue = Max_sk;
		inf.CurrentValue = ( inf.Skill->id != SKILL_RIDING ? Curr_sk : Max_sk );
		inf.BonusValue = 0;
		m_skills.insert( make_pair( SkillLine, inf ) );
		_UpdateSkillFields();
	}
	//Add to proficiency
	if((prof=(ItemProf *)GetProficiencyBySkill(SkillLine)) != 0)
	{
		packetSMSG_SET_PROFICICENCY pr;
		pr.ItemClass = prof->itemclass;
		if(prof->itemclass==4)
		{
				armor_proficiency|=prof->subclass;
				//SendSetProficiency(prof->itemclass,armor_proficiency);
				pr.Profinciency = armor_proficiency;
		}
		else
		{
				weapon_proficiency|=prof->subclass;
				//SendSetProficiency(prof->itemclass,weapon_proficiency);
				pr.Profinciency = weapon_proficiency;
		}
		m_session->OutPacket( SMSG_SET_PROFICIENCY, sizeof( packetSMSG_SET_PROFICICENCY ), &pr );
	}

	//hackfix for runeforging
	if(SkillLine==SKILL_RUNEFORGING && !HasSpell(53341) && !HasSpell(53343) )
	{
		addSpell( 53341 ); // Rune of Cinderglacier
		addSpell( 53343 ); // Rune of Razorice
	}

	// Displaying bug fix
	_UpdateSkillFields();
	m_achievementMgr.UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL, SkillLine, Max_sk/75, 0);
	m_achievementMgr.UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL, SkillLine, Curr_sk, 0);
}

void Player::_UpdateSkillFields()
{
	uint32 f = PLAYER_SKILL_INFO_1_1;
	/* Set the valid skills */
	for(SkillMap::iterator itr = m_skills.begin(); itr != m_skills.end();)
	{
		if(!itr->first)
		{
			SkillMap::iterator it2 = itr++;
			m_skills.erase(it2);
			continue;
		}

		ASSERT(f <= PLAYER_CHARACTER_POINTS1);
		if(itr->second.Skill->type == SKILL_TYPE_PROFESSION)
		{
			SetUInt32Value(f++, itr->first | 0x10000);
			m_achievementMgr.UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL, itr->second.Skill->id, itr->second.CurrentValue, 0);
		}
		else
		{
			SetUInt32Value(f++, itr->first);
			m_achievementMgr.UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL, itr->second.Skill->id, itr->second.MaximumValue/75, 0);
		}

		SetUInt32Value(f++, (itr->second.MaximumValue << 16) | itr->second.CurrentValue);
		SetUInt32Value(f++, itr->second.BonusValue);
		++itr;
	}

	/* Null out the rest of the fields */
	for(; f < PLAYER_CHARACTER_POINTS1; ++f)
	{
		if(m_uint32Values[f] != 0)
			SetUInt32Value(f, 0);
	}
}

bool Player::_HasSkillLine(uint32 SkillLine)
{
	return (m_skills.find(SkillLine) != m_skills.end());
}

void Player::_AdvanceSkillLine(uint32 SkillLine, uint32 Count /* = 1 */)
{
	SkillMap::iterator itr = m_skills.find(SkillLine);
	if(itr == m_skills.end())
	{
		/* Add it */
		_AddSkillLine(SkillLine, Count, getLevel() * 5);
		_UpdateMaxSkillCounts();
		sHookInterface.OnAdvanceSkillLine(this, SkillLine, Count);
		m_achievementMgr.UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL, SkillLine, _GetSkillLineMax(SkillLine), 0);
		m_achievementMgr.UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL, SkillLine, Count, 0);
	}
	else
	{
		uint32 curr_sk = itr->second.CurrentValue;
		itr->second.CurrentValue = min(curr_sk + Count,itr->second.MaximumValue);
		if (itr->second.CurrentValue != curr_sk)
		{
			_UpdateSkillFields();
			sHookInterface.OnAdvanceSkillLine(this, SkillLine, curr_sk);
		}
		m_achievementMgr.UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL, SkillLine, itr->second.MaximumValue/75, 0);
		m_achievementMgr.UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL, SkillLine, itr->second.CurrentValue, 0);
	}
}

uint32 Player::_GetSkillLineMax(uint32 SkillLine)
{
	SkillMap::iterator itr = m_skills.find(SkillLine);
	return (itr == m_skills.end()) ? 0 : itr->second.MaximumValue;
}

uint32 Player::_GetSkillLineCurrent(uint32 SkillLine, bool IncludeBonus /* = true */)
{
	SkillMap::iterator itr = m_skills.find(SkillLine);
	if(itr == m_skills.end())
		return 0;

	return (IncludeBonus ? itr->second.CurrentValue + itr->second.BonusValue : itr->second.CurrentValue);
}

void Player::_RemoveSkillLine(uint32 SkillLine)
{
	SkillMap::iterator itr = m_skills.find(SkillLine);
	if(itr == m_skills.end())
		return;

	m_skills.erase(itr);
	_UpdateSkillFields();
}

void Player::_UpdateMaxSkillCounts()
{
	bool dirty = false;
	uint32 new_max;
	for(SkillMap::iterator itr = m_skills.begin(); itr != m_skills.end(); ++itr)
	{
		if(itr->second.Skill->type == SKILL_TYPE_WEAPON || itr->second.Skill->id == SKILL_LOCKPICKING || itr->second.Skill->id == SKILL_POISONS)
		{
			new_max = 5 * getLevel();
		}
		else if (itr->second.Skill->type == SKILL_TYPE_LANGUAGE)
		{
			new_max = 300;
		}
		else if (itr->second.Skill->type == SKILL_TYPE_PROFESSION || itr->second.Skill->type == SKILL_TYPE_SECONDARY)
		{
			new_max = itr->second.MaximumValue;
			if (new_max >= 450)
				new_max = 450;
		}
		else
		{
			new_max = 1;
		}

		// force to be within limits
#if PLAYER_LEVEL_CAP==80
		if (new_max > 450)
			new_max = 450;
#else
		if (new_max > 375)
			new_max = 375;
#endif
		if (new_max < 1)
			new_max = 1;


		if(itr->second.MaximumValue != new_max)
		{
			dirty = true;
			itr->second.MaximumValue = new_max;
		}
		if (itr->second.CurrentValue > new_max)
		{
			dirty = true;
			itr->second.CurrentValue = new_max;
		}
	}

	if(dirty)
		_UpdateSkillFields();
}

void Player::_ModifySkillBonus(uint32 SkillLine, int32 Delta)
{
	SkillMap::iterator itr = m_skills.find(SkillLine);
	if(itr == m_skills.end())
		return;

	itr->second.BonusValue += Delta;
	_UpdateSkillFields();
}

void Player::_ModifySkillBonusByType(uint32 SkillType, int32 Delta)
{
	bool dirty = false;
	for(SkillMap::iterator itr = m_skills.begin(); itr != m_skills.end(); ++itr)
	{
		if(itr->second.Skill->type == SkillType)
		{
			itr->second.BonusValue += Delta;
			dirty=true;
		}
	}

	if(dirty)
		_UpdateSkillFields();
}

/** Maybe this formula needs to be checked?
 * - Burlex
 */

float PlayerSkill::GetSkillUpChance()
{
	float diff = float(MaximumValue - CurrentValue);
	return (diff * 100.0f / float(MaximumValue));
}

void Player::_RemoveLanguages()
{
	for(SkillMap::iterator itr = m_skills.begin(), it2; itr != m_skills.end();)
	{
		if(itr->second.Skill->type == SKILL_TYPE_LANGUAGE)
		{
			it2 = itr++;
			m_skills.erase(it2);
		}
		else
			++itr;
	}
}

void PlayerSkill::Reset(uint32 Id)
{
	MaximumValue = 0;
	CurrentValue = 0;
	BonusValue = 0;
	Skill = (Id == 0) ? NULL : dbcSkillLine.LookupEntry(Id);
}

void Player::_AddLanguages(bool All)
{
	/** This function should only be used at login, and after _RemoveLanguages is called.
	 * Otherwise weird stuff could happen :P
	 * - Burlex
	 */

	PlayerSkill sk;
	skilllineentry * en;
	uint32 spell_id;
	static uint32 skills[] = { SKILL_LANG_COMMON, SKILL_LANG_ORCISH, SKILL_LANG_DWARVEN, SKILL_LANG_DARNASSIAN, SKILL_LANG_TAURAHE, SKILL_LANG_THALASSIAN,
		SKILL_LANG_TROLL, SKILL_LANG_GUTTERSPEAK, SKILL_LANG_DRAENEI, 0 };

	if(All)
	{
		for(uint32 i = 0; skills[i] != 0; ++i)
		{
			if(!skills[i])
				break;

            sk.Reset(skills[i]);
			sk.MaximumValue = sk.CurrentValue = 300;
			m_skills.insert( make_pair(skills[i], sk) );
			if((spell_id = ::GetSpellForLanguage(skills[i])) != 0)
				addSpell(spell_id);
		}
	}
	else
	{
		for(list<CreateInfo_SkillStruct>::iterator itr = info->skills.begin(); itr != info->skills.end(); ++itr)
		{
			en = dbcSkillLine.LookupEntry(itr->skillid);
			if(en->type == SKILL_TYPE_LANGUAGE)
			{
				sk.Reset(itr->skillid);
				sk.MaximumValue = sk.CurrentValue = 300;
				m_skills.insert( make_pair(itr->skillid, sk) );
				if((spell_id = ::GetSpellForLanguage(itr->skillid)) != 0)
					addSpell(spell_id);
			}
		}
	}

	_UpdateSkillFields();
}

float Player::GetSkillUpChance(uint32 id)
{
	SkillMap::iterator itr = m_skills.find(id);
	if(itr == m_skills.end())
		return 0.0f;

	return itr->second.GetSkillUpChance();
}

void Player::_RemoveAllSkills()
{
	m_skills.clear();
	_UpdateSkillFields();
}

void Player::_AdvanceAllSkills(uint32 count)
{
	bool dirty=false;
	for(SkillMap::iterator itr = m_skills.begin(); itr != m_skills.end(); ++itr)
	{
		if(itr->second.CurrentValue != itr->second.MaximumValue)
		{
			itr->second.CurrentValue += count;
			if(itr->second.CurrentValue >= itr->second.MaximumValue)
				itr->second.CurrentValue = itr->second.MaximumValue;
			dirty=true;
		}
	}

	if(dirty)
		_UpdateSkillFields();
}

void Player::_ModifySkillMaximum(uint32 SkillLine, uint32 NewMax)
{
	// force to be within limits
#if PLAYER_LEVEL_CAP==80
	NewMax = ( NewMax > 450 ? 450 : NewMax );
#else
	NewMax = ( NewMax > 375 ? 375 : NewMax );
#endif

	SkillMap::iterator itr = m_skills.find(SkillLine);
	if(itr == m_skills.end())
		return;

	if(NewMax > itr->second.MaximumValue)
	{
		if(SkillLine == SKILL_RIDING)
			itr->second.CurrentValue = NewMax;

		itr->second.MaximumValue = NewMax;
		_UpdateSkillFields();
		m_achievementMgr.UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL, SkillLine, NewMax/75, 0);
	}
}

void Player::RemoveSpellTargets(uint32 Type, Unit* target)
{
	if( m_spellIndexTypeTargets[Type] != 0 )
	{
		Unit * pUnit = m_mapMgr ? m_mapMgr->GetUnit(m_spellIndexTypeTargets[Type]) : NULL;
		if( pUnit != NULL && pUnit != target ) //some auras can stack on target. There is no need to remove them if target is same as previous one
		{
			pUnit->RemoveAurasByBuffIndexType(Type, GetGUID());
			m_spellIndexTypeTargets[Type] = 0;
		}
	}
}

void Player::RemoveSpellIndexReferences(uint32 Type)
{
	m_spellIndexTypeTargets[Type] = 0;
}

void Player::SetSpellTargetType(uint32 Type, Unit* target)
{
	m_spellIndexTypeTargets[Type] = target->GetGUID();
}

void Player::RecalculateHonor()
{
	HonorHandler::RecalculateHonorFields(this);
}

//wooot, crapy code rulez.....NOT
void Player::EventTalentHearthOfWildChange(bool apply)
{
	if(!hearth_of_wild_pct)
		return;

	//druid hearth of the wild should add more features based on form
	int tval;
	if(apply)
		tval = hearth_of_wild_pct;
	else tval = -hearth_of_wild_pct;

	uint32 SS=GetShapeShift();

	//increase stamina if :
	if(SS==FORM_BEAR || SS==FORM_DIREBEAR)
	{
		TotalStatModPctPos[STAT_STAMINA] += tval;
		CalcStat(STAT_STAMINA);
		UpdateStats();
		UpdateChances();
	}
	//increase attackpower if :
	else if(SS==FORM_CAT)
	{
		SetFloatValue(UNIT_FIELD_ATTACK_POWER_MULTIPLIER,GetFloatValue(UNIT_FIELD_ATTACK_POWER_MULTIPLIER)+float(tval/200.0f));
		SetFloatValue(UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER, GetFloatValue(UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER)+float(tval/200.0f));
		UpdateStats();


	}
}

/************************************************************************/
/* New Save System                                                      */
/************************************************************************/
#ifdef OPTIMIZED_PLAYER_SAVING

void Player::save_LevelXP()
{
	CharacterDatabase.Execute("UPDATE characters SET level = %u, xp = %u WHERE guid = %u", m_uint32Values[UNIT_FIELD_LEVEL], m_uint32Values[PLAYER_XP], m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_PositionHP()
{
	CharacterDatabase.Execute("UPDATE characters SET current_hp = %u, current_power = %u, positionX = '%f', positionY = '%f', positionZ = '%f', orientation = '%f', mapId = %u, instance_id = %u WHERE guid = %u",
		m_uint32Values[UNIT_FIELD_HEALTH], m_uint32Values[UNIT_FIELD_POWER1+GetPowerType()], m_position.x, m_position.y, m_position.z, m_position.o, m_mapId, m_instanceId, m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_Gold()
{
	CharacterDatabase.Execute("UPDATE characters SET gold = %u WHERE guid = %u", m_uint32Values[PLAYER_FIELD_COINAGE], m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_GuildData()
{
	if(myGuild)
	{
		string escaped_note = m_playerInfo->publicNote ? CharacterDatabase.EscapeString(m_playerInfo->publicNote) : "";
		string escaped_note2 = m_playerInfo->officerNote ?  CharacterDatabase.EscapeString(m_playerInfo->officerNote) : "";
		CharacterDatabase.Execute("UPDATE characters SET guildid=%u, guildRank=%u, publicNote='%s', officerNote='%s' WHERE guid = %u",
			GetGuildId(), GetGuildRank(), escaped_note.c_str(), escaped_note2.c_str(), m_uint32Values[OBJECT_FIELD_GUID]);
	}
	else
	{
		CharacterDatabase.Execute("UPDATE characters SET guildid = 0, guildRank = 0, publicNote = '', officerNote = '' WHERE guid = %u",
			m_uint32Values[OBJECT_FIELD_GUID]);
	}
}

void Player::save_ExploreData()
{
	char buffer[2048] = {0};
	int p = 0;
	for(uint32 i = 0; i < PLAYER_EXPLORED_ZONES_LENGTH; ++i)
	{
		p += sprintf(&buffer[p], "%u,", m_uint32Values[PLAYER_EXPLORED_ZONES_1 + i]);
	}

	ASSERT(p < 2048);
	CharacterDatabase.Execute("UPDATE characters SET exploration_data = '%s' WHERE guid = %u", buffer, m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_Honor()
{
	CharacterDatabase.Execute("UPDATE characters SET honorPointsToAdd = %u, killsToday = %u, killsYesterday = %u, killsLifeTime = %u, honorToday = %u, honorYesterday = %u, honorPoints = %u, arenaPoints = %u WHERE guid = %u",
		m_honorPointsToAdd, m_killsToday, m_killsYesterday, m_killsLifetime, m_honorToday, m_honorYesterday, m_honorPoints, m_arenaPoints, m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_Skills()
{
	char buffer[6000] = {0};
	int p = 0;

	for(SkillMap::iterator itr = m_skills.begin(); itr != m_skills.end(); ++itr)
	{
		if(itr->first && itr->second.Skill->type != SKILL_TYPE_LANGUAGE)
			p += sprintf(&buffer[p], "%u;%u;%u;", itr->first, itr->second.CurrentValue, itr->second.MaximumValue);
	}

	ASSERT(p < 6000);
	CharacterDatabase.Execute("UPDATE characters SET skills = '%s' WHERE guid = %u", buffer, m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_Reputation()
{
	char buffer[10000] = {0};
	int p = 0;

	ReputationMap::iterator iter = m_reputation.begin();
	for(; iter != m_reputation.end(); ++iter)
	{
		p += sprintf(&buffer[p], "%d,%d,%d,%d,",
			iter->first, iter->second->flag, iter->second->baseStanding, iter->second->standing);
	}

	ASSERT(p < 10000);
	CharacterDatabase.Execute("UPDATE characters SET reputation = '%s' WHERE guid = %u", buffer, m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_Actions()
{
	char buffer[2048] = {0};
	int p = 0;

	for(uint32 i = 0; i < 120; ++i)
	{
		p += sprintf(&buffer[p], "%u,%u,%u,", mActions[i].Action, mActions[i].Misc, mActions[i].Type);
	}

	ASSERT(p < 2048);
	CharacterDatabase.Execute("UPDATE characters SET actions = '%s' WHERE guid = %u", buffer, m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_BindPosition()
{
	CharacterDatabase.Execute("UPDATE characters SET bindpositionX = '%f', bindpositionY = '%f', bindpositionZ = '%f', bindmapId = %u, bindzoneId = %u WHERE guid = %u",
		m_bind_pos_x, m_bind_pos_y, m_bind_pos_z, m_bind_mapid, m_bind_zoneid, m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_EntryPoint()
{
	CharacterDatabase.Execute("UPDATE characters SET entrypointmap = %u, entrypointx = '%f', entrypointy = '%f', entrypointz = '%f', entrypointo = '%f', entrypointinstance = %u WHERE guid = %u",
		m_bgEntryPointMap, m_bgEntryPointX, m_bgEntryPointY, m_bgEntryPointZ, m_bgEntryPointO, m_bgEntryPointInstance, m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_Taxi()
{
	char buffer[1024] = {0};
	int p = 0;
	for(uint32 i = 0; i < 12; ++i)
		p += sprintf(&buffer[p], "%u ", m_taximask[i]);

	if(m_onTaxi) {
		CharacterDatabase.Execute("UPDATE characters SET taximask = '%s', taxi_path = %u, taxi_lastnode = %u, taxi_mountid = %u WHERE guid = %u",
			buffer, m_CurrentTaxiPath->GetID(), lastNode, m_uint32Values[UNIT_FIELD_MOUNTDISPLAYID], m_uint32Values[OBJECT_FIELD_GUID]);
	}
	else
	{
		CharacterDatabase.Execute("UPDATE characters SET taximask = '%s', taxi_path = 0, taxi_lastnode = 0, taxi_mountid = 0 WHERE guid = %u",
			buffer, m_uint32Values[OBJECT_FIELD_GUID]);
	}
}

void Player::save_Zone()
{
	CharacterDatabase.Execute("UPDATE characters SET zoneId = %u WHERE guid = %u", m_zoneId, m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_Spells()
{
	char buffer[10000] = {0};
	char buffer2[10000] = {0};
	uint32 p=0,q=0;

	SpellSet::iterator itr = mSpells.begin();
	for(; itr != mSpells.end(); ++itr)
	{
		p += sprintf(&buffer[p], "%u,", *itr);
	}

	for(itr = mDeletedSpells.begin(); itr != mDeletedSpells.end(); ++itr)
	{
		q += sprintf(&buffer2[q], "%u,", *itr);
	}

	CharacterDatabase.Execute("UPDATE characters SET spells = '%s', deleted_spells = '%s' WHERE guid = %u",
		buffer, buffer2, m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_InstanceType()
{
	CharacterDatabase.Execute("UPDATE characters SET instancetype = %u WHERE guid = %u", iInstanceType, m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_Misc()
{
	uint32 playedt = UNIXTIME - m_playedtime[2];
	m_playedtime[0] += playedt;
	m_playedtime[1] += playedt;
	m_playedtime[2] += playedt;

	uint32 player_flags = m_uint32Values[PLAYER_FLAGS];
	{
		// Remove un-needed and problematic player flags from being saved :p
		if(player_flags & PLAYER_FLAG_PARTY_LEADER)
			player_flags &= ~PLAYER_FLAG_PARTY_LEADER;
		if(player_flags & PLAYER_FLAG_AFK)
			player_flags &= ~PLAYER_FLAG_AFK;
		if(player_flags & PLAYER_FLAG_DND)
			player_flags &= ~PLAYER_FLAG_DND;
		if(player_flags & PLAYER_FLAG_GM)
			player_flags &= ~PLAYER_FLAG_GM;
		if(player_flags & PLAYER_FLAG_PVP_TOGGLE)
			player_flags &= ~PLAYER_FLAG_PVP_TOGGLE;
		if(player_flags & PLAYER_FLAG_FREE_FOR_ALL_PVP)
			player_flags &= ~PLAYER_FLAG_FREE_FOR_ALL_PVP;
	}

	CharacterDatabase.Execute("UPDATE characters SET totalstableslots = %u, first_login = 0, TalentResetTimes = %u, playedTime = '%u %u %u ', isResting = %u, restState = %u, restTime = %u, timestamp = %u, watched_faction_index = %u, ammo_id = %u, available_prof_points = %u, available_talent_points = %u, bytes = %u, bytes2 = %u, player_flags = %u, player_bytes = %u",
		GetStableSlotCount(), GetTalentResetTimes(), m_playedtime[0], m_playedtime[1], playedt, uint32(m_isResting), uint32(m_restState), m_restAmount, UNIXTIME,
		m_uint32Values[PLAYER_FIELD_WATCHED_FACTION_INDEX], m_uint32Values[PLAYER_AMMO_ID], m_uint32Values[PLAYER_CHARACTER_POINTS2],
		m_uint32Values[PLAYER_CHARACTER_POINTS1], m_uint32Values[PLAYER_BYTES], m_uint32Values[PLAYER_BYTES_2], player_flags,
		m_uint32Values[PLAYER_FIELD_BYTES],
		m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_PVP()
{
	CharacterDatabase.Execute("UPDATE characters SET pvprank = %u, selected_pvp_title = %u, available_pvp_title = %u WHERE guid = %u",
		uint32(GetPVPRank()), m_uint32Values[PLAYER_CHOSEN_TITLE], GetUInt64Value( PLAYER_FIELD_KNOWN_TITLES ), m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_Auras()
{
	char buffer[10000];
	uint32 p =0;

	for(uint32 x=0;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
	{
		if(m_auras[x])
		{
			Aura *aur=m_auras[x];
			bool skip = false;
			for(uint32 i = 0; i < 3; ++i)
			{
				if(aur->m_spellProto->Effect[i] == SPELL_EFFECT_APPLY_AREA_AURA ||
					aur->m_spellProto->Effect[i] == SPELL_EFFECT_APPLY_AREA_AURA2 ||
					aur->m_spellProto->Effect[i] == SPELL_EFFECT_ADD_FARSIGHT)
				{
					skip = true;
					break;
				}
			}

			// skipped spells due to bugs
			switch(aur->m_spellProto->Id)
			{
			case 12043: // Presence of mind
			case 11129: // Combustion
			case 28682: // Combustion proc
			case 16188: // Natures Swiftness
			case 17116: // Natures Swiftness
			case 34936: // Backlash
			case 35076: // Blessing of A'dal
			case 23333:	// WSG
			case ARENA_PREPARATION:	// battleground preparation
			case 32724: // Arena Flags
			case 32725: // Arena Flags
			case 35774: // Arena Flags
			case 35775: // Arena Flags
				skip = true;
				break;
			}

			//disabled proc spells until proper loading is fixed. Some spells tend to block or not remove when restored
			if(aur->GetSpellProto()->procFlags)
			{
				//				sLog.outDebug("skipping aura %d because has flags %d",aur->GetSpellId(),aur->GetSpellProto()->procFlags);
				skip = true;
			}
			//disabled proc spells until proper loading is fixed. We cannot recover the charges that were used up. Will implement later
			if(aur->GetSpellProto()->procCharges)
			{
				//				sLog.outDebug("skipping aura %d because has proccharges %d",aur->GetSpellId(),aur->GetSpellProto()->procCharges);
				skip = true;
			}
			//we are going to cast passive spells anyway on login so no need to save auras for them
			if(aur->IsPassive() && !(aur->GetSpellProto()->AttributesEx & 1024))
				skip = true;

			if(skip)continue;
			uint32 d=aur->GetTimeLeft();
			if(d>3000)
				p += sprintf(buffer, "%u,%u,", aur->GetSpellId(), d);
		}
	}

	CharacterDatabase.Execute("UPDATE characters SET auras = '%s' WHERE guid = %u", buffer, m_uint32Values[OBJECT_FIELD_GUID]);
}

#endif

void Player::EventGroupFullUpdate()
{
	if(m_playerInfo->m_Group)
	{
		//m_playerInfo->m_Group->Update();
		m_playerInfo->m_Group->UpdateAllOutOfRangePlayersFor(this);
	}
}

void Player::EjectFromInstance()
{
	if(m_bgEntryPointX && m_bgEntryPointY && m_bgEntryPointZ && !IS_INSTANCE(m_bgEntryPointMap))
	{
		if(SafeTeleport(m_bgEntryPointMap, m_bgEntryPointInstance, m_bgEntryPointX, m_bgEntryPointY, m_bgEntryPointZ, m_bgEntryPointO))
			return;
	}

	SafeTeleport(m_bind_mapid, 0, m_bind_pos_x, m_bind_pos_y, m_bind_pos_z, 0);
}

bool Player::HasQuestSpell(uint32 spellid) //Only for Cast Quests
{
	if (quest_spells.size()>0 && quest_spells.find(spellid) != quest_spells.end())
		return true;
	return false;
}
void Player::RemoveQuestSpell(uint32 spellid) //Only for Cast Quests
{
	if (quest_spells.size()>0)
		quest_spells.erase(spellid);
}

bool Player::HasQuestMob(uint32 entry) //Only for Kill Quests
{
	if (quest_mobs.size()>0 && quest_mobs.find(entry) != quest_mobs.end())
		return true;
	return false;
}

bool Player::HasQuest(uint32 entry)
{
	for(uint32 i=0;i<25;i++)
	{
		if ( m_questlog[i] != NULL && m_questlog[i]->GetQuest()->id == entry)
		return true;
	}
	return false;
}

void Player::RemoveQuestMob(uint32 entry) //Only for Kill Quests
{
	if (quest_mobs.size()>0)
		quest_mobs.erase(entry);
}

PlayerInfo::~PlayerInfo()
{
	if(m_Group)
		m_Group->RemovePlayer(this);
}

void Player::CopyAndSendDelayedPacket(WorldPacket * data)
{
	WorldPacket * data2 = new WorldPacket(*data);
	delayedPackets.add(data2);
}

void Player::SendMeetingStoneQueue(uint32 DungeonId, uint8 Status)
{
	WorldPacket data(SMSG_MEETINGSTONE_SETQUEUE, 5);
	data << DungeonId << Status;
	m_session->SendPacket(&data);
}

void Player::PartLFGChannel()
{
	Channel * pChannel = channelmgr.GetChannel("LookingForGroup", this);
	if( pChannel == NULL )
		return;

	/*for(list<Channel*>::iterator itr = m_channels.begin(); itr != m_channels.end(); ++itr)
	{
		if( (*itr) == pChannel )
		{
			pChannel->Part(this);
			return;
		}
	}*/
	if( m_channels.find( pChannel) == m_channels.end() )
		return;

	pChannel->Part( this );
}

//if we charmed or simply summoned a pet, this function should get called
void Player::EventSummonPet( Pet *new_pet )
{
	if ( !new_pet )
		return ; //another wtf error

	SpellSet::iterator it,iter;
	for(iter= mSpells.begin();iter != mSpells.end();)
	{
		it = iter++;
		uint32 SpellID = *it;
		SpellEntry *spellInfo = dbcSpell.LookupEntry(SpellID);
		if( spellInfo->c_is_flags & SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER )
		{
			this->RemoveAllAuras( SpellID, this->GetGUID() ); //this is required since unit::addaura does not check for talent stacking
			SpellCastTargets targets( this->GetGUID() );
			Spell *spell = SpellPool.PooledNew();
			if (!spell)
				return;
			spell->Init(this, spellInfo ,true, NULL);	//we cast it as a proc spell, maybe we should not !
			spell->prepare(&targets);
		}
		if( spellInfo->c_is_flags & SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET )
		{
			this->RemoveAllAuras( SpellID, this->GetGUID() ); //this is required since unit::addaura does not check for talent stacking
			SpellCastTargets targets( new_pet->GetGUID() );
			Spell *spell = SpellPool.PooledNew();
			if (!spell)
				return;
			spell->Init(this, spellInfo ,true, NULL);	//we cast it as a proc spell, maybe we should not !
			spell->prepare(&targets);
		}
	}
	//there are talents that stop working after you gain pet
	for(uint32 x=MAX_TOTAL_AURAS_START;x<MAX_TOTAL_AURAS_END;x++)
		if(m_auras[x] && m_auras[x]->GetSpellProto()->c_is_flags & SPELL_FLAG_IS_EXPIREING_ON_PET)
			m_auras[x]->Remove();
	//pet should inherit some of the talents from caster
	//new_pet->InheritSMMods(); //not required yet. We cast full spell to have visual effect too
}

//if pet/charm died or whatever hapened we should call this function
//!! note function might get called multiple times :P
void Player::EventDismissPet()
{
	for(uint32 x=MAX_TOTAL_AURAS_START;x<MAX_TOTAL_AURAS_END;x++)
		if( m_auras[ x ] )
			if( m_auras [ x ]->GetSpellProto( )->c_is_flags & SPELL_FLAG_IS_EXPIREING_WITH_PET )
				m_auras[ x ]->Remove( );
}

#ifdef ENABLE_COMPRESSED_MOVEMENT

CMovementCompressorThread *MovementCompressor;

void Player::AppendMovementData(uint32 op, uint32 sz, const uint8* data)
{
	//printf("AppendMovementData(%u, %u, 0x%.8X)\n", op, sz, data);
	m_movementBufferLock.Acquire();
	m_movementBuffer << uint8(sz + 2);
	m_movementBuffer << uint16(op);
	m_movementBuffer.append( data, sz );
	m_movementBufferLock.Release();
}

bool CMovementCompressorThread::run()
{
	set<Player*>::iterator itr;
	while(running)
	{
		m_listLock.Acquire();
		for(itr = m_players.begin(); itr != m_players.end(); ++itr)
		{
			(*itr)->EventDumpCompressedMovement();
		}
		m_listLock.Release();
		Sleep(World::m_movementCompressInterval);
	}

	return true;
}

void CMovementCompressorThread::AddPlayer(Player * pPlayer)
{
	m_listLock.Acquire();
	m_players.insert(pPlayer);
	m_listLock.Release();
}

void CMovementCompressorThread::RemovePlayer(Player * pPlayer)
{
	m_listLock.Acquire();
	m_players.erase(pPlayer);
	m_listLock.Release();
}

void Player::EventDumpCompressedMovement()
{
	if( m_movementBuffer.size() == 0 )
		return;

	m_movementBufferLock.Acquire();
	uint32 size = (uint32)m_movementBuffer.size();
	uint32 destsize = size + size/10 + 16;
	int rate = World::m_movementCompressRate;
	if(size >= 40000 && rate < 6)
		rate = 6;
	if(size <= 100)
		rate = 0;			// don't bother compressing packet smaller than this, zlib doesnt really handle them well

	// set up stream
	z_stream stream;
	stream.zalloc = 0;
	stream.zfree  = 0;
	stream.opaque = 0;

	if(deflateInit(&stream, rate) != Z_OK)
	{
		sLog.outError("deflateInit failed.");
		m_movementBufferLock.Release();
		return;
	}

	uint8 *buffer = new uint8[destsize];

	// set up stream pointers
	stream.next_out  = (Bytef*)buffer+4;
	stream.avail_out = destsize;
	stream.next_in   = (Bytef*)m_movementBuffer.contents();
	stream.avail_in  = size;

	// call the actual process
	if(deflate(&stream, Z_NO_FLUSH) != Z_OK ||
		stream.avail_in != 0)
	{
		sLog.outError("deflate failed.");
		delete [] buffer;
		m_movementBufferLock.Release();
		return;
	}

	// finish the deflate
	if(deflate(&stream, Z_FINISH) != Z_STREAM_END)
	{
		sLog.outError("deflate failed: did not end stream");
		delete [] buffer;
		m_movementBufferLock.Release();
		return;
	}

	// finish up
	if(deflateEnd(&stream) != Z_OK)
	{
		sLog.outError("deflateEnd failed.");
		delete [] buffer;
		m_movementBufferLock.Release();
		return;
	}

	// fill in the full size of the compressed stream
#ifdef USING_BIG_ENDIAN
	*(uint32*)&buffer[0] = swap32(size);
#else
	*(uint32*)&buffer[0] = size;
#endif

	// send it
	m_session->OutPacket(763, (uint16)stream.total_out + 4, buffer);
	//printf("Compressed move compressed from %u bytes to %u bytes.\n", m_movementBuffer.size(), stream.total_out + 4);

	// cleanup memory
	delete [] buffer;
	m_movementBuffer.clear();
	m_movementBufferLock.Release();
}
#endif

void Player::AddShapeShiftSpell(uint32 id)
{
	SpellEntry * sp = dbcSpell.LookupEntry( id );
	mShapeShiftSpells.insert( id );

	if( sp->RequiredShapeShift && ((uint32)1 << (GetShapeShift()-1)) & sp->RequiredShapeShift )
	{
		Spell * spe = SpellPool.PooledNew();
		if (!spe)
			return;
		spe->Init( this, sp, true, NULL );
		SpellCastTargets t(this->GetGUID());
		spe->prepare( &t );
	}
}

void Player::RemoveShapeShiftSpell(uint32 id)
{
	mShapeShiftSpells.erase( id );
	RemoveAura( id );
}

// COOLDOWNS
void Player::_Cooldown_Add(uint32 Type, uint32 Misc, uint32 Time, uint32 SpellId, uint32 ItemId)
{
	PlayerCooldownMap::iterator itr = m_cooldownMap[Type].find( Misc );
	if( itr != m_cooldownMap[Type].end( ) )
	{
		if( itr->second.ExpireTime < Time )
		{
			itr->second.ExpireTime = Time;
			itr->second.ItemId = ItemId;
			itr->second.SpellId = SpellId;
		}
	}
	else
	{
		PlayerCooldown cd;
		cd.ExpireTime = Time;
		cd.ItemId = ItemId;
		cd.SpellId = SpellId;

		m_cooldownMap[Type].insert( make_pair( Misc, cd ) );
	}

#ifdef _DEBUG
	Log.Debug("Cooldown", "added cooldown for type %u misc %u time %u item %u spell %u", Type, Misc, Time - getMSTime(), ItemId, SpellId);
#endif
}

void Player::Cooldown_Add(SpellEntry * pSpell, Item * pItemCaster)
{
	uint32 mstime = getMSTime();
	int32 cool_time;

	if( pSpell->CategoryRecoveryTime > 0 && pSpell->Category )
	{
		cool_time = pSpell->CategoryRecoveryTime;
		if( pSpell->SpellGroupType )
		{
			SM_FIValue(SM_FCooldownTime, &cool_time, pSpell->SpellGroupType);
			SM_PIValue(SM_PCooldownTime, &cool_time, pSpell->SpellGroupType);
		}

		_Cooldown_Add( COOLDOWN_TYPE_CATEGORY, pSpell->Category, mstime + cool_time, pSpell->Id, pItemCaster ? pItemCaster->GetProto()->ItemId : 0 );
	}

	if( pSpell->RecoveryTime > 0 )
	{
		cool_time = pSpell->RecoveryTime;
		if( pSpell->SpellGroupType )
		{
			SM_FIValue(SM_FCooldownTime, &cool_time, pSpell->SpellGroupType);
			SM_PIValue(SM_PCooldownTime, &cool_time, pSpell->SpellGroupType);
		}

		_Cooldown_Add( COOLDOWN_TYPE_SPELL, pSpell->Id, mstime + cool_time, pSpell->Id, pItemCaster ? pItemCaster->GetProto()->ItemId : 0 );
	}
}

void Player::Cooldown_AddStart(SpellEntry * pSpell)
{
	if( pSpell->StartRecoveryTime == 0 )
		return;

	uint32 mstime = getMSTime();
	int32 atime = float2int32( float( pSpell->StartRecoveryTime ) / SpellHasteRatingBonus );


	if( atime < 1000 )	// global cooldown is limited to 1s
		atime = 1000;

	if( pSpell->StartRecoveryCategory )		// if we have a different cool category to the actual spell category - only used by few spells
		_Cooldown_Add( COOLDOWN_TYPE_CATEGORY, pSpell->StartRecoveryCategory, mstime + atime, pSpell->Id, 0 );
	/*else if( pSpell->Category )				// cooldowns are grouped
		_Cooldown_Add( COOLDOWN_TYPE_CATEGORY, pSpell->Category, mstime + pSpell->StartRecoveryTime, pSpell->Id, 0 );*/
	else									// no category, so it's a gcd
	{
#ifdef _DEBUG
		Log.Debug("Cooldown", "Global cooldown adding: %u ms", atime );
#endif
		m_globalCooldown = mstime + atime;

	}
}

bool Player::Cooldown_CanCast(SpellEntry * pSpell)
{
	PlayerCooldownMap::iterator itr;
	uint32 mstime = getMSTime();

	if( pSpell->Category )
	{
		itr = m_cooldownMap[COOLDOWN_TYPE_CATEGORY].find( pSpell->Category );
		if( itr != m_cooldownMap[COOLDOWN_TYPE_CATEGORY].end( ) )
		{
			if( mstime < itr->second.ExpireTime )
				return false;
			else
				m_cooldownMap[COOLDOWN_TYPE_CATEGORY].erase( itr );
		}
	}

	itr = m_cooldownMap[COOLDOWN_TYPE_SPELL].find( pSpell->Id );
	if( itr != m_cooldownMap[COOLDOWN_TYPE_SPELL].end( ) )
	{
		if( mstime < itr->second.ExpireTime )
			return false;
		else
			m_cooldownMap[COOLDOWN_TYPE_SPELL].erase( itr );
	}

	if( pSpell->StartRecoveryTime && m_globalCooldown && !this->CooldownCheat /* cebernic:GCD also cheat :D */ )			/* gcd doesn't affect spells without a cooldown it seems */
	{
		if( mstime < m_globalCooldown )
			return false;
		else
			m_globalCooldown = 0;
	}

	return true;
}

void Player::Cooldown_AddItem(ItemPrototype * pProto, uint32 x)
{
	if( pProto->Spells[x].CategoryCooldown <= 0 && pProto->Spells[x].Cooldown <= 0 )
		return;

	ItemSpell * isp = &pProto->Spells[x];
	uint32 mstime = getMSTime();

	if( isp->CategoryCooldown > 0)
		_Cooldown_Add( COOLDOWN_TYPE_CATEGORY, isp->Category, isp->CategoryCooldown + mstime, isp->Id, pProto->ItemId );

	if( isp->Cooldown > 0 )
		_Cooldown_Add( COOLDOWN_TYPE_SPELL, isp->Id, isp->Cooldown + mstime, isp->Id, pProto->ItemId );
}

bool Player::Cooldown_CanCast(ItemPrototype * pProto, uint32 x)
{
	PlayerCooldownMap::iterator itr;
	ItemSpell * isp = &pProto->Spells[x];
	uint32 mstime = getMSTime();

	if( isp->Category )
	{
		itr = m_cooldownMap[COOLDOWN_TYPE_CATEGORY].find( isp->Category );
		if( itr != m_cooldownMap[COOLDOWN_TYPE_CATEGORY].end( ) )
		{
			if( mstime < itr->second.ExpireTime )
				return false;
			else
				m_cooldownMap[COOLDOWN_TYPE_CATEGORY].erase( itr );
		}
	}

	itr = m_cooldownMap[COOLDOWN_TYPE_SPELL].find( isp->Id );
	if( itr != m_cooldownMap[COOLDOWN_TYPE_SPELL].end( ) )
	{
		if( mstime < itr->second.ExpireTime )
			return false;
		else
			m_cooldownMap[COOLDOWN_TYPE_SPELL].erase( itr );
	}

	return true;
}

#define COOLDOWN_SKIP_SAVE_IF_MS_LESS_THAN 10000

void Player::_SavePlayerCooldowns(QueryBuffer * buf)
{
	PlayerCooldownMap::iterator itr;
	PlayerCooldownMap::iterator itr2;
	uint32 i;
	uint32 seconds;
	uint32 mstime = getMSTime();

	// clear them (this should be replaced with an update queue later)
	if( buf != NULL )
		buf->AddQuery("DELETE FROM playercooldowns WHERE player_guid = %u", m_uint32Values[OBJECT_FIELD_GUID] );		// 0 is guid always
	else
		CharacterDatabase.Execute("DELETE FROM playercooldowns WHERE player_guid = %u", m_uint32Values[OBJECT_FIELD_GUID] );		// 0 is guid always

	for( i = 0; i < NUM_COOLDOWN_TYPES; ++i )
	{
		itr = m_cooldownMap[i].begin( );
		for( ; itr != m_cooldownMap[i].end( ); )
		{
			itr2 = itr++;

			// expired ones - no point saving, nor keeping them around, wipe em
			if( mstime >= itr2->second.ExpireTime )
			{
				m_cooldownMap[i].erase( itr2 );
				continue;
			}

			// skip small cooldowns which will end up expiring by the time we log in anyway
			if( ( itr2->second.ExpireTime - mstime ) < COOLDOWN_SKIP_SAVE_IF_MS_LESS_THAN )
				continue;

			// work out the cooldown expire time in unix timestamp format
			// burlex's reason: 30 day overflow of 32bit integer, also
			// under windows we use GetTickCount() which is the system uptime, if we reboot
			// the server all these timestamps will appear to be messed up.

			seconds = (itr2->second.ExpireTime - mstime) / 1000;
			// this shouldnt ever be nonzero because of our check before, so no check needed

			if( buf != NULL )
			{
				buf->AddQuery( "INSERT INTO playercooldowns VALUES(%u, %u, %u, %u, %u, %u)", m_uint32Values[OBJECT_FIELD_GUID],
					i, itr2->first, seconds + (uint32)UNIXTIME, itr2->second.SpellId, itr2->second.ItemId );
			}
			else
			{
				CharacterDatabase.Execute( "INSERT INTO playercooldowns VALUES(%u, %u, %u, %u, %u, %u)", m_uint32Values[OBJECT_FIELD_GUID],
					i, itr2->first, seconds + (uint32)UNIXTIME, itr2->second.SpellId, itr2->second.ItemId );
			}
		}
	}
}

void Player::_LoadPlayerCooldowns(QueryResult * result)
{
	if( result == NULL )
		return;

	// we should only really call getMSTime() once to avoid user->system transitions, plus
	// the cost of calling a function for every cooldown the player has
	uint32 mstime = getMSTime();
	uint32 type;
	uint32 misc;
	uint32 rtime;
	uint32 realtime;
	uint32 itemid;
	uint32 spellid;
	PlayerCooldown cd;

	do
	{
		type = result->Fetch()[0].GetUInt32();
		misc = result->Fetch()[1].GetUInt32();
		rtime = result->Fetch()[2].GetUInt32();
		spellid = result->Fetch()[3].GetUInt32();
		itemid = result->Fetch()[4].GetUInt32();

		if( type >= NUM_COOLDOWN_TYPES )
			continue;

		// remember the cooldowns were saved in unix timestamp format for the reasons outlined above,
		// so restore them back to mstime upon loading

		if( (uint32)UNIXTIME > rtime )
			continue;

		rtime -= (uint32)UNIXTIME;

		if( rtime < 10 )
			continue;

		realtime = mstime + ( ( rtime ) * 1000 );

		// apply it back into cooldown map
		cd.ExpireTime = realtime;
		cd.ItemId = itemid;
		cd.SpellId = spellid;
		m_cooldownMap[type].insert( make_pair( misc, cd ) );

	} while ( result->NextRow( ) );
}

void Player::_FlyhackCheck()
{
	if(!sWorld.antihack_flight || m_TransporterGUID != 0 || GetTaxiState() || (sWorld.no_antihack_on_gm && GetSession()->HasGMPermissions()))
		return;
    return; //disabled
	MovementInfo * mi = GetSession()->GetMovementInfo();
	if(!mi) return; //wtf?

	if (!GetSession())
		return;
	// Falling, CCs, etc. All stuff that could potentially trap a player in mid-air.
	if(!(mi->flags & (MOVEFLAG_FALLING | MOVEFLAG_SWIMMING | MOVEFLAG_LEVITATE | MOVEFLAG_FEATHER_FALL)) &&
		!(m_special_state & (UNIT_STATE_CHARM | UNIT_STATE_FEAR | UNIT_STATE_ROOT | UNIT_STATE_STUN | UNIT_STATE_POLYMORPH | UNIT_STATE_CONFUSE | UNIT_STATE_FROZEN))
		&& !flying_aura && !FlyCheat)
	{
		float t_height = CollideInterface.GetHeight(GetMapId(), GetPositionX(), GetPositionY(), GetPositionZ() + 2.0f);
		if(t_height == 999999.0f || t_height == NO_WMO_HEIGHT )
			t_height = GetMapMgr()->GetLandHeight(GetPositionX(), GetPositionY());
			if(t_height == 999999.0f || t_height == 0.0f) // Can't rely on anyone these days...
				return;

		float p_height = GetPositionZ();

		int32 diff = float2int32(p_height - t_height);
		if(diff < 0)
			diff = -diff;

		if(t_height != p_height && (uint32)diff > sWorld.flyhack_threshold)
		{
			// Fly hax!
			EventTeleport(GetMapId(), GetPositionX(), GetPositionY(), t_height + 2.0f); // relog fix.
			sCheatLog.writefromsession(GetSession(), "Caught fly hacking on map %u hovering %u over the terrain.", GetMapId(), diff);
			WorldPacket data (SMSG_MOVE_UNSET_CAN_FLY, 13);
			data << GetNewGUID();
			data << uint32(5);
			GetSession()->SendPacket(&data);
		}
	}
}

/************************************************************************/
/* SOCIAL                                                               */
/************************************************************************/

void Player::Social_AddFriend(const char * name, const char * note)
{
	WorldPacket data(SMSG_FRIEND_STATUS, 10);
	map<uint32, char*>::iterator itr;

	// lookup the player
	PlayerInfo* info = objmgr.GetPlayerInfoByName(name);
	Player* player = objmgr.GetPlayer(name, false);

	if( info == NULL || ( player != NULL && player->bGMTagOn ) )
	{
		data << uint8(FRIEND_NOT_FOUND);
		m_session->SendPacket(&data);
		return;
	}

	// team check
	if( info->team != m_playerInfo->team  && m_session->permissioncount == 0 && !sWorld.interfaction_friend)
	{
		data << uint8(FRIEND_ENEMY) << uint64(info->guid);
		m_session->SendPacket(&data);
		return;
	}

	// are we ourselves?
	if( info == m_playerInfo )
	{
		data << uint8(FRIEND_SELF) << GetGUID();
		m_session->SendPacket(&data);
		return;
	}

	m_socialLock.Acquire();
	itr = m_friends.find(info->guid);
	if( itr != m_friends.end() )
	{
		data << uint8(FRIEND_ALREADY) << uint64(info->guid);
		m_session->SendPacket(&data);
		m_socialLock.Release();
		return;
	}

	if( info->m_loggedInPlayer != NULL )
	{
		data << uint8(FRIEND_ADDED_ONLINE);
		data << uint64(info->guid);
		if( note != NULL )
			data << note;
		else
			data << uint8(0);

		data << uint8(1);
		data << info->m_loggedInPlayer->GetZoneId();
		data << info->lastLevel;
		data << uint32(info->cl);

		info->m_loggedInPlayer->m_socialLock.Acquire();
		info->m_loggedInPlayer->m_hasFriendList.insert( GetLowGUID() );
		info->m_loggedInPlayer->m_socialLock.Release();
	}
	else
	{
		data << uint8(FRIEND_ADDED_OFFLINE);
		data << uint64(info->guid);
	}

	if( note != NULL )
		m_friends.insert( make_pair(info->guid, strdup(note)) );
	else
		m_friends.insert( make_pair(info->guid, (char*)NULL) );

	m_socialLock.Release();
	m_session->SendPacket(&data);

	// dump into the db
	CharacterDatabase.Execute("INSERT INTO social_friends VALUES(%u, %u, \"%s\")",
		GetLowGUID(), info->guid, note ? CharacterDatabase.EscapeString(string(note)).c_str() : "");
}

void Player::Social_RemoveFriend(uint32 guid)
{
	WorldPacket data(SMSG_FRIEND_STATUS, 10);
	map<uint32, char*>::iterator itr;

	// are we ourselves?
	if( guid == GetLowGUID() )
	{
		data << uint8(FRIEND_SELF) << GetGUID();
		m_session->SendPacket(&data);
		return;
	}

	m_socialLock.Acquire();
	itr = m_friends.find(guid);
	if( itr != m_friends.end() )
	{
		if( itr->second != NULL )
			free(itr->second);

		m_friends.erase(itr);
	}

	data << uint8(FRIEND_REMOVED);
	data << uint64(guid);

	m_socialLock.Release();

	Player * pl = objmgr.GetPlayer( (uint32)guid );
	if( pl != NULL )
	{
		pl->m_socialLock.Acquire();
		pl->m_hasFriendList.erase( GetLowGUID() );
		pl->m_socialLock.Release();
	}

	m_session->SendPacket(&data);

	// remove from the db
	CharacterDatabase.Execute("DELETE FROM social_friends WHERE character_guid = %u AND friend_guid = %u",
		GetLowGUID(), (uint32)guid);
}

void Player::Social_SetNote(uint32 guid, const char * note)
{
	map<uint32,char*>::iterator itr;

	m_socialLock.Acquire();
	itr = m_friends.find(guid);

	if( itr == m_friends.end() )
	{
		m_socialLock.Release();
		return;
	}

	if( itr->second != NULL )
		free(itr->second);

	if( note != NULL )
		itr->second = strdup( note );
	else
		itr->second = NULL;

	m_socialLock.Release();
	CharacterDatabase.Execute("UPDATE social_friends SET note = \"%s\" WHERE character_guid = %u AND friend_guid = %u",
		note ? CharacterDatabase.EscapeString(string(note)).c_str() : "", GetLowGUID(), guid);
}

void Player::Social_AddIgnore(const char * name)
{
	WorldPacket data(SMSG_FRIEND_STATUS, 10);
	set<uint32>::iterator itr;
	PlayerInfo * info;

	// lookup the player
	info = objmgr.GetPlayerInfoByName(name);
	if( info == NULL )
	{
		data << uint8(FRIEND_IGNORE_NOT_FOUND);
		m_session->SendPacket(&data);
		return;
	}

	// are we ourselves?
	if( info == m_playerInfo )
	{
		data << uint8(FRIEND_IGNORE_SELF) << GetGUID();
		m_session->SendPacket(&data);
		return;
	}

	m_socialLock.Acquire();
	itr = m_ignores.find(info->guid);
	if( itr != m_ignores.end() )
	{
		data << uint8(FRIEND_IGNORE_ALREADY) << uint64(info->guid);
		m_session->SendPacket(&data);
		m_socialLock.Release();
		return;
	}

	data << uint8(FRIEND_IGNORE_ADDED);
	data << uint64(info->guid);

	m_ignores.insert( info->guid );

	m_socialLock.Release();
	m_session->SendPacket(&data);

	// dump into db
	CharacterDatabase.Execute("INSERT INTO social_ignores VALUES(%u, %u)", GetLowGUID(), info->guid);
}

void Player::Social_RemoveIgnore(uint32 guid)
{
	WorldPacket data(SMSG_FRIEND_STATUS, 10);
	set<uint32>::iterator itr;

	// are we ourselves?
	if( guid == GetLowGUID() )
	{
		data << uint8(FRIEND_IGNORE_SELF) << GetGUID();
		m_session->SendPacket(&data);
		return;
	}

	m_socialLock.Acquire();
	itr = m_ignores.find(guid);
	if( itr != m_ignores.end() )
	{
		m_ignores.erase(itr);
	}

	data << uint8(FRIEND_IGNORE_REMOVED);
	data << uint64(guid);

	m_socialLock.Release();

	m_session->SendPacket(&data);

	// remove from the db
	CharacterDatabase.Execute("DELETE FROM social_ignores WHERE character_guid = %u AND ignore_guid = %u",
		GetLowGUID(), (uint32)guid);
}

bool Player::Social_IsIgnoring(PlayerInfo * m_info)
{
	bool res;
	m_socialLock.Acquire();
	if( m_ignores.find( m_info->guid ) == m_ignores.end() )
		res = false;
	else
		res = true;

	m_socialLock.Release();
	return res;
}

bool Player::Social_IsIgnoring(uint32 guid)
{
	bool res;
	m_socialLock.Acquire();
	if( m_ignores.find( guid ) == m_ignores.end() )
		res = false;
	else
		res = true;

	m_socialLock.Release();
	return res;
}

void Player::Social_TellFriendsOnline()
{
	if( m_hasFriendList.empty() )
		return;

	WorldPacket data(SMSG_FRIEND_STATUS, 22);
	set<uint32>::iterator itr;
	Player * pl;
	data << uint8( FRIEND_ONLINE ) << GetGUID() << uint8( 1 );
	data << GetAreaID() << getLevel() << uint32(getClass());

	m_socialLock.Acquire();
	for( itr = m_hasFriendList.begin(); itr != m_hasFriendList.end(); ++itr )
	{
		pl = objmgr.GetPlayer(*itr);
		if( pl != NULL )
			pl->GetSession()->SendPacket(&data);
	}
	m_socialLock.Release();
}

void Player::Social_TellFriendsOffline()
{
	if( m_hasFriendList.empty() )
		return;

	WorldPacket data(SMSG_FRIEND_STATUS, 10);
	set<uint32>::iterator itr;
	Player * pl;
	data << uint8( FRIEND_OFFLINE ) << GetGUID() << uint8( 0 );

	m_socialLock.Acquire();
	for( itr = m_hasFriendList.begin(); itr != m_hasFriendList.end(); ++itr )
	{
		pl = objmgr.GetPlayer(*itr);
		if( pl != NULL )
			pl->GetSession()->SendPacket(&data);
	}
	m_socialLock.Release();
}

void Player::Social_SendFriendList(uint32 flag)
{
	WorldPacket data(SMSG_CONTACT_LIST, 500);
	map<uint32,char*>::iterator itr;
	set<uint32>::iterator itr2;
	Player * plr;

	m_socialLock.Acquire();

	data << flag;
	data << uint32( m_friends.size() + m_ignores.size() );
	for( itr = m_friends.begin(); itr != m_friends.end(); ++itr )
	{
		// guid
		data << uint64( itr->first );

		// friend/ignore flag.
		// 1 - friend
		// 2 - ignore
		// 3 - muted?
		data << uint32( 1 );

		// player note
		if( itr->second != NULL )
			data << itr->second;
		else
			data << uint8(0);

		// online/offline flag
		plr = objmgr.GetPlayer( itr->first );
		if( plr != NULL )
		{
			data << uint8( 1 );
			data << plr->GetZoneId();
			data << plr->getLevel();
			data << uint32( plr->getClass() );
		}
		else
			data << uint8( 0 );
	}

	for( itr2 = m_ignores.begin(); itr2 != m_ignores.end(); ++itr2 )
	{
		// guid
		data << uint64( (*itr2) );

		// ignore flag - 2
		data << uint32( 2 );

		// no note
		data << uint8( 0 );
	}

	m_socialLock.Release();
	m_session->SendPacket(&data);
}

void Player::VampiricSpell(uint32 dmg, Unit* pTarget)
{
	float fdmg = float(dmg);
	uint32 bonus;
	int32 perc;
	Group * pGroup = GetGroup();
	SubGroup * pSubGroup = (pGroup != NULL) ? pGroup->GetSubGroup(GetSubGroup()) : NULL;
	GroupMembersSet::iterator itr;

	if( ( !m_vampiricEmbrace && !m_vampiricTouch ) || getClass() != PRIEST )
		return;

	if( m_vampiricEmbrace > 0 && pTarget->m_hasVampiricEmbrace > 0 && pTarget->HasAurasWithNameHash(SPELL_HASH_VAMPIRIC_EMBRACE) )
	{
		perc = 15;
		uint32 spellgroup[3] = {4, 0, 0};
		SM_FIValue(SM_FMiscEffect, &perc, spellgroup);


		bonus = float2int32(fdmg * (float(perc)/100.0f));
		if( bonus > 0 )
		{
			Heal(this, 15286, bonus);

			// loop party
			if( pSubGroup != NULL )
			{
				for( itr = pSubGroup->GetGroupMembersBegin(); itr != pSubGroup->GetGroupMembersEnd(); ++itr )
				{
					if( (*itr)->m_loggedInPlayer != NULL && (*itr) != m_playerInfo && (*itr)->m_loggedInPlayer->isAlive() )
						Heal( (*itr)->m_loggedInPlayer, 15286, bonus );
				}
			}
		}
	}

	if( m_vampiricTouch > 0 && pTarget->m_hasVampiricTouch > 0 && pTarget->HasAurasWithNameHash(SPELL_HASH_VAMPIRIC_TOUCH) )
	{
		perc = 5;

		bonus = float2int32(fdmg * (float(perc)/100.0f));
		if( bonus > 0 )
		{
			Energize(this, 34919, bonus, POWER_TYPE_MANA);

			// loop party
			if( pSubGroup != NULL )
			{
				for( itr = pSubGroup->GetGroupMembersBegin(); itr != pSubGroup->GetGroupMembersEnd(); ++itr )
				{
					if( (*itr)->m_loggedInPlayer != NULL && (*itr) != m_playerInfo && (*itr)->m_loggedInPlayer->isAlive() && (*itr)->m_loggedInPlayer->GetPowerType() == POWER_TYPE_MANA )
						Energize((*itr)->m_loggedInPlayer, 34919, bonus, POWER_TYPE_MANA);
				}
			}
		}
	}
}

void Player::SpeedCheatDelay(uint32 ms_delay)
{
//	SDetector->SkipSamplingUntil( getMSTime() + ms_delay );
	//add tripple latency to avoid client handleing the spell effect with delay and we detect as cheat
//	SDetector->SkipSamplingUntil( getMSTime() + ms_delay + GetSession()->GetLatency() * 3 );
	//add constant value to make sure the effect packet was sent to client from network pool
	SDetector->SkipSamplingUntil( getMSTime() + ms_delay + GetSession()->GetLatency() * 2 + 2000 ); //2 second should be enough to send our packets to client
}

// Reset GM speed hacks after a SafeTeleport
void Player::SpeedCheatReset()
{
	// wtf?
	SDetector->EventSpeedChange();

	/*
	SetPlayerSpeed(RUN, m_runSpeed);
	SetPlayerSpeed(SWIM, m_runSpeed);
	SetPlayerSpeed(RUNBACK, m_runSpeed / 2); // Backwards slower, it's more natural :P

	WorldPacket data(SMSG_FORCE_FLIGHT_SPEED_CHANGE, 16);
	data << GetNewGUID();
	data << uint32(0) << m_flySpeed;
	SendMessageToSet(&data, true);
	*/
}

uint32 Player::GetMaxPersonalRating()
{
	uint32 maxrating = 0;
	int i;

	ASSERT(m_playerInfo != NULL);

	for (i=0; i<NUM_ARENA_TEAM_TYPES; i++)
	{
		if(m_arenaTeams[i] != NULL)
		{
			ArenaTeamMember *m = m_arenaTeams[i]->GetMemberByGuid(m_playerInfo->guid);
			if (m)
			{
				if (m->PersonalRating > maxrating) maxrating = m->PersonalRating;
			}
			else
			{
				sLog.outError("%s: GetMemberByGuid returned NULL for player guid = %u\n", __FUNCTION__, m_playerInfo->guid);
			}
		}
	}

	return maxrating;
}
/***********************************
* Give player full hp/mana
***********************************/

void Player::FullHPMP()
{
	if(IsDead())
		ResurrectPlayer();
    SetUInt32Value(UNIT_FIELD_HEALTH, GetUInt32Value(UNIT_FIELD_MAXHEALTH));
	SetPower(POWER_TYPE_MANA, GetUInt32Value(UNIT_FIELD_MAXPOWER1));
    SetUInt32Value(UNIT_FIELD_POWER4, GetUInt32Value(UNIT_FIELD_MAXPOWER4));
}

/***********************************
* Remove all temporary enchants
***********************************/
void Player::RemoveTempEnchantsOnArena()
{
	ItemInterface *itemi = GetItemInterface();

	for( uint32 x = EQUIPMENT_SLOT_START; x < EQUIPMENT_SLOT_END; ++x ) // all equipment items
	{
		Item * it = itemi->GetInventoryItem(x);

		if( it != NULL )
		{
			it->RemoveAllEnchantments(true);
		}
	}

	for( uint32 x = INVENTORY_SLOT_BAG_START; x < INVENTORY_SLOT_BAG_END; ++x) // all bags items
	{
		Item * it = itemi->GetInventoryItem(x);
		
		if( it != NULL )
		{
			if( it->IsContainer() )
			{
				Container *bag = static_cast<Container*>( it );
				for( uint32 ci = 0; ci < bag->GetProto()->ContainerSlots; ++ci )
				{
					it = bag->GetItem( ci );
					if( it != NULL )
						it->RemoveAllEnchantments(true);
				}
			}
		}
	}

	for( uint32 x = INVENTORY_SLOT_ITEM_START; x < INVENTORY_SLOT_ITEM_END; ++x ) // all inventory items
	{
		Item * it = itemi->GetInventoryItem(x);

		if( it != NULL )
		{
			it->RemoveAllEnchantments(true);
		}
	}
}

void Player::PlaySound( uint32 sound_id )
{
	WorldPacket data(SMSG_PLAY_SOUND, 4);
	data << sound_id;
	GetSession()->SendPacket( &data );
}

//really need to work on the speed of this. This will be called on a lot of events
/*void Player::Event_Achiement_Received(uint32 achievementtype,uint32 pentry,uint32 pvalue)
{
    for (uint32 i=0; i<dbcAchievementCriteriaStore.GetNumRows(); i++)
    {
		AchievementCriteriaEntry *criteria = dbcAchievementCriteriaStore.LookupRow(i);
		uint32 achientry = criteria->ID;
		//check if we need to even know about this criteria
		if( !criteria || criteria->requiredType != achievementtype )
			continue;

		//we will send only this if it is required for this type
		if( pentry && criteria->requiredAchievementRelatedEntry != pentry )
			continue;

		//check if this achievement is even for us
		AchievementEntry *achi = dbcAchievementStore.LookupEntry( criteria->referredAchievement );

		if( !achi
//			|| !( achi->factionFlag == -1 || (isAlliance(this) && achi->factionFlag == 1) || (!isAlliance(this) && achi->factionFlag == 0 ) ) ||
//			|| achi->flags & ACHIEVEMENT_FLAG_COUNTER
//			|| ((criteria->groupFlag & ACHIEVEMENT_CRITERIA_GROUP_NOT_IN_GROUP) && GetGroup() )
			)
			continue;

		if( !m_achievements[ achientry ] )
			m_achievements[ achientry ] = new AchievementVal;

		//check if we can finish = get archievement points on this
//		if( pvalue < criteria->requiredAchievementRelatedCount )
//			SendAchievmentStatus( achientry );
//		else
			if( pvalue >= criteria->requiredAchievementRelatedCount
				&&
				m_achievements[ achientry ]->cur_value < criteria->requiredAchievementRelatedCount
				)
			{
//				m_achievement_points += achi->points;
				m_achievements[ achientry ]->completed_at_stamp = (uint32)UNIXTIME;
				SendAchievmentEarned( criteria->referredAchievement );
			}

		//if we got here then we could update our status
		m_achievements[ achientry ]->cur_value = pvalue;
    }

}

void Player::SendAchievmentEarned( uint32 archiId, uint32 at_stamp )
{
    WorldPacket data( SMSG_ACHIEVEMENT_EARNED, 30);
	data << GetNewGUID();
    data << uint32( archiId );
	if( at_stamp )
		data << uint32( at_stamp ); //seems to be something that increases in time. Also seems to a large enough value for time
	else data << uint32( UNIXTIME );
	data << uint32(0);
	GetSession()->SendPacket(&data);
}*/


//wtf does this do ? How can i check the effect of this anyway ? Made this before SMSG_ACHIEVEMENT_EARNED :P
void Player::ConvertRune(uint8 index, uint8 value)
{
	ASSERT(index < 6);
	m_runes[index] = value;
	if( value >= RUNE_RECHARGE || GetSession() == NULL )
		return;

	WorldPacket data(SMSG_CONVERT_RUNE, 2);
	data << (uint8)index;
	data << (uint8)value;
	GetSession()->SendPacket(&data);

// @Egari:	Rune updates should only be sent to the DK himself, sending it to set will cause graphical glitches on the UI of other DKs.
//	SendMessageToSet(&data, true); 
}

uint32 Player::HasRunes(uint8 type, uint32 count)
{
	uint32 found = 0;
	for(uint32 i=0; i<6 && count != found; ++i)
	{
		if(GetRune(i) == type)
			found++;
	}
	return (count - found);
}

uint32 Player::TakeRunes(uint8 type, uint32 count)
{
	uint8 found = 0;
	for(uint8 i=0; i<6 && count != found; ++i)
	{
		if(GetRune(i) == type)
		{
			ConvertRune(i, RUNE_RECHARGE);
			sEventMgr.AddEvent( this, &Player::ConvertRune, i, baseRunes[i], EVENT_PLAYER_RUNE_REGEN + i, 10000, 1, 0 );
			found++;
		}
	}
	return (count - found);
}

void Player::SendAchievmentStatus( uint32 criteriaid, uint32 new_value, uint32 at_stamp )
{
	/*
	noob warlock
	E2 14 00 00 - some index ? does not seem to be criteria id though
	01 - is this related to the indexes ? Seems to be 1 all the time
	2B - some index ?
	0F 82 43 97 01 - packed guid
	00 00 00 00
	B4 82 94 08
	00 00 00 00
	00 00 00 00

	E2 14 00 00 01 2B 0F 82 43 97 01 00 00 00 00 B4 82 94 08 00 00 00 00 00 00 00 00
	E2 14 00 00 01 53 0F 82 43 97 01 00 00 00 00 B4 82 94 08 00 00 00 00 00 00 00 00
	E3 14 00 00 01 BB 0F 82 43 97 01 00 00 00 00 B4 82 94 08 00 00 00 00 00 00 00 00
	E4 14 00 00 01 15 0F 82 43 97 01 00 00 00 00 B4 82 94 08 00 00 00 00 00 00 00 00
	E5 14 00 00 01 15 0F 82 43 97 01 00 00 00 00 B4 82 94 08 00 00 00 00 00 00 00 00
	E8 14 00 00 01 18 0F 82 43 97 01 00 00 00 00 B4 82 94 08 00 00 00 00 00 00 00 00
	E6 14 00 00 01 16 0F 82 43 97 01 00 00 00 00 B4 82 94 08 00 00 00 00 00 00 00 00
	E7 14 00 00 01 18 0F 82 43 97 01 00 00 00 00 B4 82 94 08 00 00 00 00 00 00 00 00
	EA 14 00 00 01 2A 0F 82 43 97 01 00 00 00 00 B4 82 94 08 00 00 00 00 00 00 00 00
	EA 14 00 00 01 52 0F 82 43 97 01 00 00 00 00 B4 82 94 08 00 00 00 00 00 00 00 00
	EA 14 00 00 01 55 0F 82 43 97 01 00 00 00 00 B4 82 94 08 00 00 00 00 00 00 00 00
	EA 14 00 00 01 57 0F 82 43 97 01 00 00 00 00 B4 82 94 08 00 00 00 00 00 00 00 00
	EA 14 00 00 01 5E 0F 82 43 97 01 00 00 00 00 B4 82 94 08 00 00 00 00 00 00 00 00
	EA 14 00 00 01 62 0F 82 43 97 01 00 00 00 00 B4 82 94 08 00 00 00 00 00 00 00 00
	*/
    /*WorldPacket data( SMSG_CRITERIA_UPDATE, 30);
    data << uint32(criteriaid);
	data << uint8(1);
	data << uint8(1); //this is some ID or something maybe merge with previous value ? Maybe it is value for criteria ?
	data << GetNewGUID();
	data << uint32(0);
	if( at_stamp )
		data << uint32( at_stamp / 8.50194274f ); //seems to be something that increases in time. Also seems to a large enough value for time
	else
		data << uint32( UNIXTIME / 8.50194274f ); //seems to be something that increases in time. Also seems to a large enough value for time
	data << uint32(0); //duration
	data << uint32(0); //duration left
	GetSession()->SendPacket(&data);*/
}
//send completed criterias to client
/*void Player::SendAllAchievementStatus()
{
	std::map<uint32,AchievementVal*>::iterator itr;
	for(itr=m_achievements.begin();itr!=m_achievements.end();itr++)
	{
//		uint32 critentry = itr->first;
//		AchievementCriteriaEntry *criteria = dbcAchievementCriteriaStore.LookupEntry( critentry );
//		if( !critentry ) continue; //wtf ?
//		if( criteria->requiredAchievementRelatedCount > itr->second )
		SendAchievmentStatus( itr->first, itr->second->cur_value, itr->second->completed_at_stamp );
	}
}

void Player::SendAllAchievementEarned()
{
	std::map<uint32,AchievementVal*>::iterator itr;
	for(itr=m_achievements.begin();itr!=m_achievements.end();itr++)
	{
		uint32 critentry = itr->first;
		AchievementCriteriaEntry *criteria = dbcAchievementCriteriaStore.LookupEntry( critentry );
		if( !criteria )
			continue; //wtf ?
		if( criteria->requiredAchievementRelatedCount <= itr->second->cur_value )
			SendAchievmentEarned( criteria->referredAchievement );
	}
}*/

//this packet might get huge in time. Might reach over 1.2 MByte !
/*void Player::SendAllAchievementData()
{
	//we are cheating with these. Something is enabling client side the achievemnts menu
	WorldPacket data(SMSG_ALL_ACHIEVEMENT_DATA, 500);
	uint32 found_finished=0;

	//generate block of finished acheivemnts
	std::map<uint32,AchievementVal*>::iterator itr;
	for(itr=m_achievements.begin();itr!=m_achievements.end();itr++)
	{
		uint32 critentry = itr->first;
		AchievementCriteriaEntry *criteria = dbcAchievementCriteriaStore.LookupEntry( critentry );
		if( !criteria )
			continue; //wtf ?
		if( criteria->requiredAchievementRelatedCount <= itr->second->cur_value )
		{
			found_finished++;
			data << uint32( criteria->referredAchievement );
			data << uint32( itr->second->completed_at_stamp / 8.50194274f); // -sometimes it is durations
		}
	}
	data << uint32( 0xFFFFFFFF ); //maybe a terminator like a null terminated string ?
*/
	//now the block for those : SMSG_CRITERIA_UPDATE
/*
			//sometimes this is duration left
			data << uint32( criteria->ID );
			data << uint8( 1 );
			data << uint8( itr->second->cur_value ); //probably not since it does not fit into this size. Also seems to change from time to time
			data << GetNewGUID();
			data << uint32( 0 ); //no idea. was always 0
			data << uint32( UNIXTIME ); //seems to be something that increases in time. Also seems to a large enough value for time
			data << uint32(0); //duration
			data << uint32(0); //duration left
*/
/*	if( found_finished )
		GetSession()->SendPacket(&data);
}*/

void Player::UpdatePowerAmm()
{
	WorldPacket data(SMSG_POWER_UPDATE, 5);
	FastGUIDPack(data, GetGUID());
	data << uint8( GetPowerType() );
	data << GetUInt32Value( UNIT_FIELD_POWER1 + GetPowerType() );
	SendMessageToSet(&data, true);
}
// Initialize Glyphs or update them after level change
void Player::UpdateGlyphs()
{
    uint32 level = getLevel();

    // Init glyph slots
    if( level >= 15 )
    {
        GlyphSlotEntry * gse;
        uint32 y = 0;
    for( uint32 i = 0; i < dbcGlyphSlot.GetNumRows(); ++i )
    {
        gse = dbcGlyphSlot.LookupRow( i );
        if( gse->Slot > 0 )
        SetUInt32Value( PLAYER_FIELD_GLYPH_SLOTS_1 + y++, gse->Id );
    }
}

	// Enable number of glyphs depending on level
	uint32 glyph_mask = 0;
	if(level == 80)
		glyph_mask = 6;
	else if(level >= 70)
		glyph_mask = 5;
	else if(level >= 50)
		glyph_mask = 4;
	else if(level >= 30)
		glyph_mask = 3;
	else if(level >= 15)
		glyph_mask = 2;
	SetUInt32Value(PLAYER_GLYPHS_ENABLED, (1 << glyph_mask) -1 );
}

// Fills fields from firstField to firstField+fieldsNum-1 with integers from the string
void Player::LoadFieldsFromString(const char * string, uint32 firstField, uint32 fieldsNum)
{
	if(string == NULL)
		return;
	char * end;
	char * start = (char *) string;
	for(uint32 Counter = 0; Counter < fieldsNum; Counter++)
	{
		end = strchr(start,',');
		if(!end)
			break;
		*end = 0;
		SetUInt32Value(firstField + Counter, atol(start));
		start = end + 1;
	}
}

void Player::SetKnownTitle( RankTitles title, bool set )
{
	if( !HasTitle( title ) ^ set )
		return;

	uint64 current = GetUInt64Value( PLAYER_FIELD_KNOWN_TITLES + ( ( title >> 6 ) << 1 ) );
	if( set )
		SetUInt64Value( PLAYER_FIELD_KNOWN_TITLES + ( ( title >> 6 ) << 1 ), current | uint64(1) << ( title % 64) );
	else
		SetUInt64Value( PLAYER_FIELD_KNOWN_TITLES + ( ( title >> 6 ) << 1 ), current & ~uint64(1) << ( title % 64) );

	WorldPacket data( SMSG_TITLE_EARNED, 8 );
	data << uint32( title ) << uint32( set ? 1 : 0 );
	m_session->SendPacket( &data );
}

void Player::SendTriggerMovie( uint32 movieID )
{
	if( m_session )
		m_session->OutPacket( SMSG_TRIGGER_MOVIE, 4, &movieID );
}

uint32 Player::GetInitialFactionId()
{
	PlayerCreateInfo * pci = objmgr.GetPlayerCreateInfo(getRace(), getClass());
	if( pci )
		return pci->factiontemplate;
	else 
		return 35;
}

