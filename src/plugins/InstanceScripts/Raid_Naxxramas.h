/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
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

class GrandWidowFaerlinaAI;
class AnubRekhanAI;
class DeathchargerSteedAI;
class PlaguedWarriorAI;
class PlaguedChampionAI;
class PlaguedGuardianAI;
class PlagueFissureGO;
class SporeAI;
class PortalOfShadowsAI;
class DeathKnightUnderstudyAI;

/////////////////////////////////////////////////////////////////////////////////
////// The Arachnid Quarter

/////////////////////////////////////////////////////////////////////////////////
////// Carrion Spinner
#define CN_CARRION_SPINNER					15975
#define CARRION_SPINNER_POISON_BOLT_NORMAL	30043
#define CARRION_SPINNER_POISON_BOLT_HEROIC	56032
#define CARRION_SPINNER_WEB_WRAP			28618 // TODO: PULL EFFECT *FUN*

class CarrionSpinnerAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(CarrionSpinnerAI, MoonScriptCreatureAI);
		CarrionSpinnerAI(Creature* pCreature);
};

/////////////////////////////////////////////////////////////////////////////////
////// Dread Creeper
#define CN_DREAD_CREEPER						15974
#define DREAD_CREEPER_VEIL_OF_SHADOW_NORMAL		53803
#define DREAD_CREEPER_VEIL_OF_SHADOW_HEROIC		28440

class DreadCreeperAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(DreadCreeperAI, MoonScriptCreatureAI);
		DreadCreeperAI(Creature* pCreature);
};

/////////////////////////////////////////////////////////////////////////////////
////// Naxxramas Cultist
#define CN_NAXXRAMAS_CULTIST					15980
#define NAXXRAMAS_CULTIST_KNOCKBACK_NORMAL		53850
#define NAXXRAMAS_CULTIST_KNOCKBACK_HEROIC		53851

class NaxxramasCultistAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(NaxxramasCultistAI, MoonScriptCreatureAI);
		NaxxramasCultistAI(Creature* pCreature);
};
//Necro Stalker AI - was it removed ?
/////////////////////////////////////////////////////////////////////////////////
////// Venom Stalker
#define CN_VENOM_STALKER						15976
#define VENOM_STALKER_POISON_CHARGE_NORMAL		28431
#define VENOM_STALKER_POISON_CHARGE_HEROIC		53809

class VenomStalkerAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(VenomStalkerAI, MoonScriptCreatureAI);
		VenomStalkerAI(Creature* pCreature);
};

/////////////////////////////////////////////////////////////////////////////////
////// Tomb Horror
#define CN_TOMB_HORROR							15979
#define TOMB_HORROR_SPIKE_VOLLEY				28615
#define TOMB_HORROR_CRYPT_SCARAB_SWARM_NORMAL	54313
#define TOMB_HORROR_CRYPT_SCARAB_SWARM_HEROIC	54317
#define TOMB_HORROR_CRYPT_SCARABS_NORMAL		54311
#define TOMB_HORROR_CRYPT_SCARABS_HEROIC		54316

class TombHorrorAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(TombHorrorAI, MoonScriptCreatureAI);
		TombHorrorAI(Creature* pCreature);
};

/////////////////////////////////////////////////////////////////////////////////
////// Naxxramas Acolyte
#define CN_NAXXRAMAS_ACOLYTE							16368
#define NAXXRAMAS_ACOLYTE_SHADOW_BOLT_VOLLEY_NORMAL		56064
#define NAXXRAMAS_ACOLYTE_SHADOW_BOLT_VOLLEY_HEROIC		56065
#define NAXXRAMAS_ACOLYTE_ARCANE_EXPLOSION_NORMAL		56063
#define NAXXRAMAS_ACOLYTE_ARCANE_EXPLOSION_HEROIC		56067
// To check: "total caster" + Explosion only when players are close ?
class NaxxramasAcolyteAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(NaxxramasAcolyteAI, MoonScriptCreatureAI);
		NaxxramasAcolyteAI(Creature* pCreature);
};

/////////////////////////////////////////////////////////////////////////////////
////// Vigilant Shade
#define CN_VIGILANT_SHADE								30085
#define VIGILANT_SHADE_INVISIBILITY						55848
#define VIGILANT_SHADE_SHADOW_BOLT_VOLLEY_NORMAL		55850
#define VIGILANT_SHADE_SHADOW_BOLT_VOLLEY_HEROIC		55851
// Invisiblity should be removed OnCombatStart ?
class VigilantShadeAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(VigilantShadeAI, MoonScriptCreatureAI);
		VigilantShadeAI(Creature* pCreature);

		void OnCombatStart(Unit* pTarget);
		void OnCombatStop(Unit* pTarget);
};

/////////////////////////////////////////////////////////////////////////////////
////// Crypt Reaver
#define CN_CRYPT_REAVER							15978
#define CRYPT_REAVER_CLEAVE						40504
#define CRYPT_REAVER_FRENZY						56625

class CryptReaverAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(CryptReaverAI, MoonScriptCreatureAI);
		CryptReaverAI(Creature* pCreature);
};

/////////////////////////////////////////////////////////////////////////////////
////// Web Wrap
#define CN_WEB_WRAP								16486

class WebWrapAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(WebWrapAI, MoonScriptCreatureAI);
		WebWrapAI(Creature* pCreature);

		void OnCombatStart(Unit* pTarget);
		void OnCombatStop(Unit* pTarget);
		void OnDied(Unit* pKiller);
		void AIUpdate();
		void Destroy();

		uint64	mPlayerGuid;
};

/////////////////////////////////////////////////////////////////////////////////
////// Maexxna Spiderling
#define CN_MAEXXNA_SPIDERLING						17055
#define MAEXXNA_SPIDERLING_NECROTIC_POISON_NORMAL	54121
#define MAEXXNA_SPIDERLING_NECROTIC_POISON_HEROIC	28776

class MaexxnaSpiderlingAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(MaexxnaSpiderlingAI, MoonScriptCreatureAI);
		MaexxnaSpiderlingAI(Creature* pCreature);
};

/////////////////////////////////////////////////////////////////////////////////
////// Maexxna
#define CN_MAEXXNA								15952
#define MAEXXNA_WEB_WRAP						28622
#define MAEXXNA_WEB_SPRAY_NORMAL				29484
#define MAEXXNA_WEB_SPRAY_HEROIC				54125
#define MAEXXNA_POISON_SHOCK_NORMAL				28741
#define MAEXXNA_POISON_SHOCK_HEROIC				54122
#define MAEXXNA_NECROTIC_POISON_NORMAL			54121
#define MAEXXNA_NECROTIC_POISON_HEROIC			28776
#define MAEXXNA_FRENZY_NORMAL					54123
#define MAEXXNA_FRENZY_HEROIC					54124

static Location WebWrapPos[] =
{
	// Left wall
	{ 3515.307861f, -3837.076172f, 302.671753f, 4.388477f },
	{ 3529.401123f, -3841.910889f, 300.766174f, 4.335070f },
	{ 3541.990479f, -3851.308350f, 298.685272f, 3.804141f },
	// Right wall
	{ 3497.262939f, -3949.542969f, 308.138916f, 1.433983f },
	{ 3510.450928f, -3949.871582f, 309.523193f, 1.613839f },
	{ 3523.486572f, -3946.144287f, 309.651611f, 1.973552f }
};

void SpellFunc_MaexxnaWebWrap(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);

class MaexxnaAI : public MoonScriptBossAI
{
		MOONSCRIPT_FACTORY_FUNCTION(MaexxnaAI, MoonScriptBossAI);
		MaexxnaAI(Creature* pCreature);

		void OnCombatStart(Unit* pTarget);
		void OnCombatStop(Unit* pTarget);
		void AIUpdate();

		SpellDesc*			mWebWrapProc;
		bool				mHasEnraged;
		bool				mLeftWall;
		int32				mAddsSummonTimer;
		int32				mWebSprayTimer;
		int32				mWebWrapTimer;
};

/////////////////////////////////////////////////////////////////////////////////
////// Naxxramas Worshipper
#define CN_NAXXRAMAS_WORSHIPPER							16506
#define NAXXRAMAS_WORSHIPPER_FIREBALL_NORMAL			54095
#define NAXXRAMAS_WORSHIPPER_FIREBALL_HEROIC			54096
#define NAXXRAMAS_WORSHIPPER_WIDOW_EMBRACE				28732
#define NAXXRAMAS_WORSHIPPER_MIND_EXHAUSTION			28727

class NaxxramasWorshipperAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(NaxxramasWorshipperAI, MoonScriptCreatureAI);
		NaxxramasWorshipperAI(Creature* pCreature);
		friend class GrandWidowFaerlinaAI;

		void OnCastSpell(uint32 pSpellId);
		void OnDied(Unit* pKiller);
		void AIUpdate();
		void Destroy();

		GrandWidowFaerlinaAI*	mGrandWidow;
		bool					mPossessed;
};

/////////////////////////////////////////////////////////////////////////////////
////// Naxxramas Follower
#define CN_NAXXRAMAS_FOLLOWER							16505
#define NAXXRAMAS_FOLLOWER_BERSERKER_CHARGE_HEROIC		56107
#define NAXXRAMAS_FOLLOWER_SILENCE_HEROIC				54093

void SpellFunc_NaxxramasFollowerCharge(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);

class NaxxramasFollowerAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(NaxxramasFollowerAI, MoonScriptCreatureAI);
		NaxxramasFollowerAI(Creature* pCreature);
		friend class GrandWidowFaerlinaAI;

		void Destroy();

		GrandWidowFaerlinaAI*	mGrandWidow;
		SpellDesc*				mCharge;
};

/////////////////////////////////////////////////////////////////////////////////
////// Grand Widow Faerlina
#define CN_GRAND_WIDOW_FAERLINA							15953
#define GRAND_WIDOW_FAERLINA_POISON_VOLLEY_BOLT_NORMAL	28796
#define GRAND_WIDOW_FAERLINA_POISON_VOLLEY_BOLT_HEROIC	54098
#define GRAND_WIDOW_FAERLINA_FRENZY_NORMAL				28798
#define GRAND_WIDOW_FAERLINA_FRENZY_HEROIC				54100
#define GRAND_WIDOW_RAIN_OF_FIRE_NORMAL					39024
#define GRAND_WIDOW_RAIN_OF_FIRE_HEROIC					58936

static Location Worshippers[ 4 ] =
{
	{ -3.0f, 0, 0, 0 },
	{ -9.0f, 0, 0, 0 },
	{ 3.0f, 0, 0, 0 },
	{ 9.0f, 0, 0, 0 }
};

static Location Followers[ 2 ] =
{
	{ -6.0f, 0, 0, 0 },
	{ 6.0f, 0, 0, 0 }
};

class GrandWidowFaerlinaAI : public MoonScriptBossAI
{
		MOONSCRIPT_FACTORY_FUNCTION(GrandWidowFaerlinaAI, MoonScriptBossAI);
		GrandWidowFaerlinaAI(Creature* pCreature);

		void OnCombatStart(Unit* pTarget);
		void OnCombatStop(Unit* pTarget);
		void AIUpdate();
		void Destroy();

		set< NaxxramasWorshipperAI* >	mWorshippers;
		set< NaxxramasFollowerAI* >		mFollowers;
		SpellDesc*						mFrenzy;
		SpellDesc*						mPoisonVolleyBolt;
		int32							mFrenzyTimer;
		int32							mPoisonVolleyBoltTimer;
};

/////////////////////////////////////////////////////////////////////////////////
////// Crypt Guard
#define CN_CRYPT_GUARD						16573
#define CRYPT_GUARD_ACID_SPLIT_NORMAL		28969
#define CRYPT_GUARD_ACID_SPLIT_HEROIC		56098
#define CRYPT_GUARD_CLEAVE					40504
#define CRYPT_GUARD_FRENZY					8269

class CryptGuardAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(CryptGuardAI, MoonScriptCreatureAI);
		CryptGuardAI(Creature* pCreature);
		friend class AnubRekhanAI;

		void OnCombatStart(Unit* pTarget);
		void AIUpdate();
		void Destroy();

		AnubRekhanAI*	mAnubRekhanAI;
		bool			mEnraged;
};

/////////////////////////////////////////////////////////////////////////////////
////// Corpse Scarab
#define CN_CORPSE_SCARAB						16698

class CorpseScarabAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(CorpseScarabAI, MoonScriptCreatureAI);
		CorpseScarabAI(Creature* pCreature);
		friend class AnubRekhanAI;

		void Destroy();

		AnubRekhanAI*	mAnubRekhanAI;
};

/////////////////////////////////////////////////////////////////////////////////
////// Anub'Rekhan
#define CN_ANUBREKHAN							15956
#define ANUBREKHAN_IMPALE_NORMAL				28783
#define ANUBREKHAN_IMPALE_HEROIC				56090
#define ANUBREKHAN_LOCUST_SWARM_NORMAL			28785
#define ANUBREKHAN_LOCUST_SWARM_HEROIC			54021
#define ANUBREKHAN_SUMMON_CORPSE_SCARABS_5		29105
#define ANUBREKHAN_SUMMON_CORPSE_SCARABS_10		28864
#define ANUBREKHAN_BERSERK						26662

static Location CryptGuards[] =
{
	{ 3300.486572f, -3449.479492f, 287.077850f, 3.883793f },
	{ 3300.568604f, -3503.060059f, 287.077850f, 2.367975f },
	{ 3332.591797f, -3476.102539f, 287.073425f, 0.015707f }
};

void SpellFunc_AnubRekhanCorpseScarabsPlayer(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);
void SpellFunc_AnubRekhanCorpseScarabsCryptGuard(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);

class AnubRekhanAI : public MoonScriptBossAI
{
		MOONSCRIPT_FACTORY_FUNCTION(AnubRekhanAI, MoonScriptBossAI);
		AnubRekhanAI(Creature* pCreature);

		void OnCombatStart(Unit* pTarget);
		void OnCombatStop(Unit* pTarget);
		void AIUpdate();
		void Destroy();

		set< CorpseScarabAI* >	mScarabs;
		set< CryptGuardAI* >	mCryptGuards;
		set< uint32 >			mUsedCorpseGuids;
		SpellDesc*				mLocustSwarm;
		int32					mLocustSwarmTimer;
		int32					mCryptSpawnTimer;
};

/////////////////////////////////////////////////////////////////////////////////
////// The Plague Quarter

/////////////////////////////////////////////////////////////////////////////////
////// Infectious Ghoul
#define CN_INFECTIOUS_GHOUL						16244
#define INFECTIOUS_GHOUL_FLESH_ROT				54709
#define INFECTIOUS_GHOUL_REND_NORMAL			54703
#define INFECTIOUS_GHOUL_REND_HEROIC			54708
#define INFECTIOUS_GHOUL_FRENZY_NORMAL			54701
#define INFECTIOUS_GHOUL_FRENZY_HEROIC			24318

class InfectiousGhoulAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(InfectiousGhoulAI, MoonScriptCreatureAI);
		InfectiousGhoulAI(Creature* pCreature);

		void OnCombatStart(Unit* pTarget);
		void AIUpdate();

		bool	mEnraged;
};

/////////////////////////////////////////////////////////////////////////////////
////// Stoneskin Gargoyle
#define CN_STONESKIN_GARGOYLE					16168
#define STONESKIN_GARGOYLE_ACID_VOLLEY_NORMAL	29325
#define STONESKIN_GARGOYLE_ACID_VOLLEY_HEROIC	54714
#define STONESKIN_GARGOYLE_STONESKIN_NORMAL		28995
#define STONESKIN_GARGOYLE_STONESKIN_HEROIC		54722

class StoneskinGargoyleAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(StoneskinGargoyleAI, MoonScriptCreatureAI);
		StoneskinGargoyleAI(Creature* pCreature);

		bool HasStoneskin();
		void AIUpdate();

		SpellDesc*	mStoneskin;
};

/////////////////////////////////////////////////////////////////////////////////
////// Frenzied Bat
#define CN_FRENZIED_BAT						16036
#define FRENZIED_BAT_FRENZIED_DIVE			54781

class FrenziedBatAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(FrenziedBatAI, MoonScriptCreatureAI);
		FrenziedBatAI(Creature* pCreature);
};

/////////////////////////////////////////////////////////////////////////////////
////// Plague Beast
#define CN_PLAGUE_BEAST						16034
#define PLAGUE_BEAST_PLAGUE_SPLASH_NORMAL	54780
#define PLAGUE_BEAST_PLAGUE_SPLASH_HEROIC	56538
#define PLAGUE_BEAST_MUTATED_SPORES			30110
#define PLAGUE_BEAST_TRAMPLE				5568

class PlagueBeastAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(PlagueBeastAI, MoonScriptCreatureAI);
		PlagueBeastAI(Creature* pCreature);

		void OnCombatStart(Unit* pTarget);
		void OnCombatStop(Unit* pTarget);
};

/////////////////////////////////////////////////////////////////////////////////
////// Eye Stalker
#define CN_EYE_STALKER						16236
#define EYE_STALKER_MIND_FLAY_NORMAL		29407
#define EYE_STALKER_MIND_FLAY_HEROIC		54805

class EyeStalkerAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(EyeStalkerAI, MoonScriptCreatureAI);
		EyeStalkerAI(Creature* pCreature);

		void OnCombatStart(Unit* pTarget);
		void AIUpdate();
};

/////////////////////////////////////////////////////////////////////////////////
////// Noth the Plaguebringer
#define CN_NOTH_THE_PLAGUEBRINGER							15954
#define NOTH_THE_PLAGUEBRINGER_BLINK_HEROIC					29208
#define NOTH_THE_PLAGUEBRINGER_CRIPLE_HEROIC				29212
#define NOTH_THE_PLAGUEBRINGER_CURSE_OF_THE_PLAGUE_NORMAL	29213	// I must check if it's target-limited spell
#define NOTH_THE_PLAGUEBRINGER_CURSE_OF_THE_PLAGUE_HEROIC	54835	// I must check if it's target-limited spell
#define NOTH_THE_PLAGUEBRINGER_BERSERK						47008	// Guessed

static Location SkelPosPhase1[] =
{
	{ 2660.175781f, -3473.315674f, 262.003967f, 5.252077f },
	{ 2717.336426f, -3463.309326f, 262.044098f, 3.703270f },
	{ 2718.720215f, -3524.452881f, 261.943176f, 2.760789f }
};

static Location SkelPosPhase2[] =
{
	{ 2660.932129f, -3474.058105f, 262.004730f, 5.765719f },
	{ 2706.175537f, -3465.862793f, 262.003510f, 4.488667f },
	{ 2723.024170f, -3472.919434f, 262.102020f, 3.809298f },
	{ 2717.878906f, -3518.062988f, 261.905945f, 3.177050f }
};

void SpellFunc_NothToBalconyPhaseSwitch(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);
void SpellFunc_NothFromBalconyPhaseSwitch(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);
void SpellFunc_NothCriple(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);
void SpellFunc_NothBlink(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);

class NothThePlaguebringerAI : public MoonScriptBossAI
{
		MOONSCRIPT_FACTORY_FUNCTION(NothThePlaguebringerAI, MoonScriptBossAI);
		NothThePlaguebringerAI(Creature* pCreature);
		friend class PlaguedWarriorAI;
		friend class PlaguedChampionAI;
		friend class PlaguedGuardianAI;

		void OnCombatStart(Unit* pTarget);
		void OnCombatStop(Unit* pTarget);
		void AIUpdate();
		void Destroy();

		set< PlaguedWarriorAI*>		mWarriors;
		set< PlaguedChampionAI*>	mChampions;
		set< PlaguedGuardianAI* >	mGuardians;
		SpellDesc*					mCriple;
		SpellDesc*					mBlink;
		SpellDesc*					mToBalconySwitch;
		SpellDesc*					mFromBalconySwitch;
		int32						mBlinkTimer;
		int32						mSkeletonTimer;
		int32						mPhaseSwitchTimer;
		uint32						mPhaseCounter;
};

/////////////////////////////////////////////////////////////////////////////////
////// Plagued Warrior
#define CN_PLAGUED_WARRIOR						16984
#define PLAGUED_WARRIOR_STRIKE					12057
#define PLAGUED_WARRIOR_CLEAVE					15496

class PlaguedWarriorAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(PlaguedWarriorAI, MoonScriptCreatureAI);
		PlaguedWarriorAI(Creature* pCreature);

		void Destroy();

		NothThePlaguebringerAI*	mNothAI;
};

/////////////////////////////////////////////////////////////////////////////////
////// Plagued Champion
#define CN_PLAGUED_CHAMPION						16983
#define PLAGUED_CHAMPION_MORTAL_STRIKE_NORMAL	32736
#define PLAGUED_CHAMPION_MORTAL_STRIKE_HEROIC	13737
#define PLAGUED_CHAMPION_SHADOW_SHOCK_NORMAL	30138
#define PLAGUED_CHAMPION_SHADOW_SHOCK_HEROIC	54889

class PlaguedChampionAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(PlaguedChampionAI, MoonScriptCreatureAI);
		PlaguedChampionAI(Creature* pCreature);

		void Destroy();

		NothThePlaguebringerAI*	mNothAI;
};

/////////////////////////////////////////////////////////////////////////////////
////// Plagued Guardian
#define CN_PLAGUED_GUARDIAN							16981
#define PLAGUED_GUARDIAN_ARCANE_EXPLOSION_NORMAL	54890
#define PLAGUED_GUARDIAN_ARCANE_EXPLOSION_HEROIC	54891
#define PLAGUED_GUARDIAN_BLINK						29208

class PlaguedGuardianAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(PlaguedGuardianAI, MoonScriptCreatureAI);
		PlaguedGuardianAI(Creature* pCreature);

		void Destroy();

		NothThePlaguebringerAI*	mNothAI;
};

/////////////////////////////////////////////////////////////////////////////////
////// Heigan the Unclean
#define CN_HEIGAN_THE_UNCLEAN						15936
#define HEIGAN_THE_UNCLEAN_SPELL_DISRUPTION			29310
#define HEIGAN_THE_UNCLEAN_DECREPIT_FEVER_NORMAL	29998
#define HEIGAN_THE_UNCLEAN_DECREPIT_FEVER_HEROIC	55011
#define HEIGAN_THE_UNCLEAN_PLAGUE_CLOUD_CHANNEL		29350
#define HEIGAN_THE_UNCLEAN_PLAGUE_CLOUD_DAMAGE		30122
#define HEIGAN_THE_UNCLEAN_TELEPORT					34673	// Guessed.
#define FISSURE_TRIGGER_ERUPTION				29371

float HeiganPos[2] = {2796, -3707};
const float HeiganEruptionSlope[3] =
{
	(-3685 - HeiganPos[1]) / (2724 - HeiganPos[0]),
	(-3647 - HeiganPos[1]) / (2749 - HeiganPos[0]),
	(-3637 - HeiganPos[1]) / (2771 - HeiganPos[0]),
};

class HeiganTheUncleanAI : public MoonScriptBossAI
{
		MOONSCRIPT_FACTORY_FUNCTION(HeiganTheUncleanAI, MoonScriptBossAI);
		HeiganTheUncleanAI(Creature* pCreature);
		friend class PlagueFissureGO;

		uint32	CalculateTriggerArea(float pPosX, float pPosY);
		void	CallEruptionEvent(int32 pTimerId, int32 pNewTime);
		void	OnCombatStart(Unit* pTarget);
		void	OnCombatStop(Unit* pTarget);
		void	AIUpdate();
		void	Destroy();

		set< pair< uint32, PlagueFissureGO* > >		mFissures;
		int32										mPhaseSwitchTimer;
		int32										mEruptionTimer;
		int32										mEruptionPhase;
		bool										mClockWiseEruption;
};

/////////////////////////////////////////////////////////////////////////////////
////// Plague Fissure
#define GO_PLAGUE_FISSURE		181533

class PlagueFissureGO : public GameObjectAIScript
{
	public:

		static GameObjectAIScript* Create(GameObject* pGameObject);
		PlagueFissureGO(GameObject* pGameObject);

		void DoErrupt();

		// I believe it's nowhere hooked in the core.
		void SetState(uint32 pState);
		void Destroy();
		void ResetHeiganAI() { mHeiganAI = NULL; }
		HeiganTheUncleanAI*	mHeiganAI;
};

/////////////////////////////////////////////////////////////////////////////////
////// Loatheb
#define CN_LOATHEB							16011
#define LOATHEB_NECROTIC_AURA				55593
#define LOATHEB_SUMMON_SPORE				29234
#define LOATHEB_DEATHBLOOM_NORMAL			29865
#define LOATHEB_DEATHBLOOM_HEROIC			55053
#define LOATHEB_DEATHBLOOM_DAMAGE_NORMAL	55594
#define LOATHEB_DEATHBLOOM_DAMAGE_HEROIC	55601
#define LOATHEB_INEVITABLE_DOOM_NORMAL		29204
#define LOATHEB_INEVITABLE_DOOM_HEROIC		55052
#define LOATHEB_BERSERK						26662	// Unused

static Location Spores[] =
{
	{ 2880.517334f, -4027.450684f, 273.680695f, 0.848522f },
	{ 2938.914307f, -4027.245850f, 273.617340f, 2.419318f },
	{ 2938.526611f, -3968.206543f, 273.524963f, 3.829108f },
	{ 2879.754883f, -3968.288574f, 273.633698f, 5.525566f }
};

class LoathebAI : public MoonScriptBossAI
{
		MOONSCRIPT_FACTORY_FUNCTION(LoathebAI, MoonScriptBossAI);
		LoathebAI(Creature* pCreature);
		friend class SporeAI;

		void OnCombatStart(Unit* pTarget);
		void OnCombatStop(Unit* pTarget);
		void AIUpdate();
		void Destroy();

		set< SporeAI* >	mSpores;
		uint32			mDoomStaticTimer;
		int32			mSporeTimer;
		int32			mDoomTimer;
		int32			mDeathbloomTimer;
		bool			mDeathbloomDamagePhase;
};

/////////////////////////////////////////////////////////////////////////////////
////// Spore
#define CN_SPORE						16286
#define SPORE_FUNGAL_CREEP				29232

class SporeAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(SporeAI, MoonScriptCreatureAI);
		SporeAI(Creature* pCreature);

		void OnDied(Unit* pKiller);
		void Destroy();

		LoathebAI* mLoathebAI;
};

/////////////////////////////////////////////////////////////////////////////////
////// The Military Quarter

/////////////////////////////////////////////////////////////////////////////////
////// Death Knight
#define CN_DEATH_KNIGHT						16146
#define DEATH_KNIGHT_BLOOD_PRESENCE			55212
#define DEATH_KNIGHT_DEATH_COIL_NORMAL		55209
#define DEATH_KNIGHT_DEATH_COIL_HEROIC		55320
#define DEATH_KNIGHT_DEATH_COIL_HEAL		55210
#define DEATH_KNIGHT_HYSTERIA				55213

class DeathKnightAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(DeathKnightAI, MoonScriptCreatureAI);
		DeathKnightAI(Creature* pCreature);

		void OnCombatStart(Unit* pTarget);
};

/////////////////////////////////////////////////////////////////////////////////
////// Death Knight Captain
#define CN_DEATH_KNIGHT_CAPTAIN						16145
#define DEATH_KNIGHT_CAPTAIN_UNHOLY_PRESENCE		55222
#define DEATH_KNIGHT_CAPTAIN_RAISE_DEAD				28353
#define DEATH_KNIGHT_CAPTAIN_PLAGUE_STRIKE_NORMAL	55255
#define DEATH_KNIGHT_CAPTAIN_PLAGUE_STRIKE_HEROIC	55321
#define DEATH_KNIGHT_CAPTAIN_WHIRLWIND				28335

class DeathKnightCaptainAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(DeathKnightCaptainAI, MoonScriptCreatureAI);
		DeathKnightCaptainAI(Creature* pCreature);

		void OnCombatStart(Unit* pTarget);
};

/////////////////////////////////////////////////////////////////////////////////
////// Skeletal Construct - wiki says he's in Naxx, but WoWHead claims him to be in Icecrown only

/////////////////////////////////////////////////////////////////////////////////
////// Ghost of Naxxramas
#define CN_GHOST_OF_NAXXRAMAS							16419

class GhostOfNaxxramasAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(GhostOfNaxxramasAI, MoonScriptCreatureAI);
		GhostOfNaxxramasAI(Creature* pCreature);
};

/////////////////////////////////////////////////////////////////////////////////
////// Shade of Naxxramas
#define CN_SHADE_OF_NAXXRAMAS							16164
#define SHADE_OF_NAXXRAMAS_PORTAL_OF_SHADOWS			28383
#define SHADE_OF_NAXXRAMAS_SHADOW_BOLT_VOLLEY_NORMAL	28407
#define SHADE_OF_NAXXRAMAS_SHADOW_BOLT_VOLLEY_HEROIC	55323

class ShadeOfNaxxramasAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(ShadeOfNaxxramasAI, MoonScriptCreatureAI);
		ShadeOfNaxxramasAI(Creature* pCreature);
		friend class PortalOfShadowsAI;

		void OnDied(Unit* pKiller);
		void Destroy();

		set< PortalOfShadowsAI* >	mPortals;
};

/////////////////////////////////////////////////////////////////////////////////
////// Portal of Shadows
#define CN_PORTAL_OF_SHADOWS							16420

class PortalOfShadowsAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(PortalOfShadowsAI, MoonScriptCreatureAI);
		PortalOfShadowsAI(Creature* pCreature);

		void OnCombatStart(Unit* pTarget);
		void OnCombatStop(Unit* pTarget);
		void AIUpdate();
		void Destroy();

		ShadeOfNaxxramasAI*	mShadeAI;
		int32				mSpawnTimer;
};

/////////////////////////////////////////////////////////////////////////////////
////// Necro Knight
#define CN_NECRO_KNIGHT							16165
#define NECRO_KNIGHT_ARCANE_EXPLOSION			15453
#define NECRO_KNIGHT_BLAST_WAVE					30092
#define NECRO_KNIGHT_BLINK						28391
#define NECRO_KNIGHT_CONE_OF_COLD				30095
#define NECRO_KNIGHT_FLAMESTRIKE				30091
#define NECRO_KNIGHT_FROST_NOVA					30094

void SpellFunc_NecroKnightBlink(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);

class NecroKnightAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(NecroKnightAI, MoonScriptCreatureAI);
		NecroKnightAI(Creature* pCreature);
};

/////////////////////////////////////////////////////////////////////////////////
////// Skeletal Smith
#define CN_SKELETAL_SMITH						16193
#define SKELETAL_SMITH_CRUSH_ARMOR				33661
#define SKELETAL_SMITH_DISARM					6713
#define SKELETAL_SMITH_THUNDERCLAP				23931
//#define SKELETAL_SMITH_SUDDER_ARMOR				24317

class SkeletalSmithAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(SkeletalSmithAI, MoonScriptCreatureAI);
		SkeletalSmithAI(Creature* pCreature);
};

/////////////////////////////////////////////////////////////////////////////////
////// Death Knight Cavalier
#define CN_DEATH_KNIGHT_CAVALIER					16163
#define DEATH_KNIGHT_CAVALIER_BONE_ARMOR_NORMAL		55315
#define DEATH_KNIGHT_CAVALIER_BONE_ARMOR_HEROIC		55336
#define DEATH_KNIGHT_CAVALIER_DISMOUNT_DEATHCHARGER	55294
#define DEATH_KNIGHT_CAVALIER_ICY_TOUCH_NORMAL		55313
#define DEATH_KNIGHT_CAVALIER_ICY_TOUCH_HEROIC		55331
#define DEATH_KNIGHT_CAVALIER_STRANGULATE_NORMAL	55314
#define DEATH_KNIGHT_CAVALIER_STRANGULATE_HEROIC	55334
#define DEATH_KNIGHT_CAVALIER_AURA_OF_AGONY			28413
#define DEATH_KNIGHT_CAVALIER_CLEAVE				15284
#define DEATH_KNIGHT_CAVALIER_DEATH_COIL			28412

class DeathKnightCavalierAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(DeathKnightCavalierAI, MoonScriptCreatureAI);
		DeathKnightCavalierAI(Creature* pCreature);
		friend class DeathchargerSteedAI;

		void OnCombatStop(Unit* pTarget);
		void AIUpdate();
		void Destroy();

		DeathchargerSteedAI*	mChargerAI;
		bool					mIsMounted;
};

/////////////////////////////////////////////////////////////////////////////////
////// Deathcharger Steed
#define CN_DEATHCHARGER_STEED					29818
#define DEATHCHARGER_STEED_CHARGE				55317

void SpellFunc_DeathchargerSteedCharge(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);

class DeathchargerSteedAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(DeathchargerSteedAI, MoonScriptCreatureAI);
		DeathchargerSteedAI(Creature* pCreature);
		friend class DeathKnightCavalierAI;

		void OnCombatStop(Unit* pTarget);
		void Destroy();

		DeathKnightCavalierAI*	mDeathKnightAI;
		SpellDesc*				mCharge;
};

/////////////////////////////////////////////////////////////////////////////////
////// Dark Touched Warrior
#define CN_DARK_TOUCHED_WARRIOR					16156
#define DARK_TOUCHED_WARRIOR_WHIRLWIND			55267
//#define DARK_TOUCHED_WARRIOR_WHIRLWIND			55266	// This one disables mob's melee

class DarkTouchedWarriorAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(DarkTouchedWarriorAI, MoonScriptCreatureAI);
		DarkTouchedWarriorAI(Creature* pCreature);

		void OnCombatStart(Unit* pTarget);
		void AIUpdate();

		int32	mResetHateTimer;
};

/////////////////////////////////////////////////////////////////////////////////
////// Risen Squire
#define CN_RISEN_SQUIRE						16154
#define RISEN_SQUIRE_PIERCE_ARMOR			55318

class RisenSquireAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(RisenSquireAI, MoonScriptCreatureAI);
		RisenSquireAI(Creature* pCreature);
};

/////////////////////////////////////////////////////////////////////////////////
////// Unholy Axe
#define CN_UNHOLY_AXE							16194
#define UNHOLY_AXE_MORTAL_STRIKE_NORMAL			16856
#define UNHOLY_AXE_MORTAL_STRIKE_HEROIC			15708
#define UNHOLY_AXE_WHIRLWIND_NORMAL				55463
#define UNHOLY_AXE_WHIRLWIND_HEROIC				24236

class UnholyAxeAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(UnholyAxeAI, MoonScriptCreatureAI);
		UnholyAxeAI(Creature* pCreature);
};

/////////////////////////////////////////////////////////////////////////////////
////// Unholy Sword
#define CN_UNHOLY_SWORD							16216
#define UNHOLY_SWORD_CLEAVE_NORMAL				15284
#define UNHOLY_SWORD_CLEAVE_HEROIC				19632

class UnholySwordAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(UnholySwordAI, MoonScriptCreatureAI);
		UnholySwordAI(Creature* pCreature);
};

/////////////////////////////////////////////////////////////////////////////////
////// Unholy Staff
#define CN_UNHOLY_STAFF							16215
#define UNHOLY_STAFF_ARCANE_EXPLOSION_NORMAL	28450
#define UNHOLY_STAFF_ARCANE_EXPLOSION_HEROIC	55467
#define UNHOLY_STAFF_FROST_NOVA					29849
#define UNHOLY_STAFF_POLYMORPH					29848

class UnholyStaffAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(UnholyStaffAI, MoonScriptCreatureAI);
		UnholyStaffAI(Creature* pCreature);
};

/////////////////////////////////////////////////////////////////////////////////
////// Instructor Razuvious
#define CN_INSTRUCTOR_RAZUVIOUS							16061
#define INSTRUCTOR_RAZUVIOUS_DISRUPTING_SHOUT_NORMAL	55543
#define INSTRUCTOR_RAZUVIOUS_DISRUPTING_SHOUT_HEROIC	29107
#define INSTRUCTOR_RAZUVIOUS_JAGGED_KNIFE				55550
#define INSTRUCTOR_RAZUVIOUS_UNBALANCING_STRIKE			55470
#define INSTRUCTOR_RAZUVIOUS_HOPELESS					29125

class InstructorRazuviousAI : public MoonScriptBossAI
{
		MOONSCRIPT_FACTORY_FUNCTION(InstructorRazuviousAI, MoonScriptBossAI);
		InstructorRazuviousAI(Creature* pCreature);
		friend class DeathKnightUnderstudyAI;

		void OnCombatStart(Unit* pTarget);
		void OnCombatStop(Unit* pTarget);
		void OnDied(Unit* pKiller);
		void AIUpdate();

		set< DeathKnightUnderstudyAI* >	mStudents;
};

InstructorRazuviousAI::InstructorRazuviousAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
{
};

void InstructorRazuviousAI::OnCombatStart(Unit* pTarget)
{
	ParentClass::OnCombatStart(pTarget);
};

void InstructorRazuviousAI::OnCombatStop(Unit* pTarget)
{
	ParentClass::OnCombatStop(pTarget);
};

void InstructorRazuviousAI::OnDied(Unit* pKiller)
{
	ParentClass::OnDied(pKiller);
};

void InstructorRazuviousAI::AIUpdate()
{
	ParentClass::AIUpdate();
};

/////////////////////////////////////////////////////////////////////////////////
////// Death Knight Understudy
#define CN_DEATH_KNIGHT_UNDERSTUDY					16803
#define DEATH_KNIGHT_UNDERSTUDY_BLOOD_STRIKE		61696
#define DEATH_KNIGHT_UNDERSTUDY_BONE_BARRIER		29061
#define DEATH_KNIGHT_UNDERSTUDY_TAUNT				29060

class DeathKnightUnderstudyAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(DeathKnightUnderstudyAI, MoonScriptCreatureAI);
		DeathKnightUnderstudyAI(Creature* pCreature);

		void Destroy();

		InstructorRazuviousAI*	mRazuviousAI;
		bool					mIsControlled;
};

DeathKnightUnderstudyAI::DeathKnightUnderstudyAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
{
	AddSpell(DEATH_KNIGHT_UNDERSTUDY_BLOOD_STRIKE, Target_Current, 10, 0, 4, 0, 8);
	AddSpell(DEATH_KNIGHT_UNDERSTUDY_BONE_BARRIER, Target_Self, 8, 0, 30);
	AddSpell(DEATH_KNIGHT_UNDERSTUDY_TAUNT, Target_Current, 8, 0, 10, 0, 8);

	// Blood Strike
	AI_Spell* NewSpell = new AI_Spell;
	NewSpell->spell = dbcSpell.LookupEntry(DEATH_KNIGHT_UNDERSTUDY_BLOOD_STRIKE);
	NewSpell->agent = AGENT_SPELL;
	NewSpell->entryId = _unit->GetEntry();
	NewSpell->maxrange = GetMaxRange(dbcSpellRange.LookupEntry(NewSpell->spell->rangeIndex));
	NewSpell->minrange = GetMinRange(dbcSpellRange.LookupEntry(NewSpell->spell->rangeIndex));
	NewSpell->spelltargetType = TTYPE_SINGLETARGET;
	NewSpell->spellType = STYPE_DAMAGE;
	NewSpell->cooldown = objmgr.GetPetSpellCooldown(NewSpell->spell->Id);
	NewSpell->cooldowntime = 0;
	NewSpell->autocast_type = AUTOCAST_EVENT_NONE;
	NewSpell->floatMisc1 = 0;
	NewSpell->Misc2 = 0;
	NewSpell->procChance = 0;
	NewSpell->procCount = 0;
	_unit->GetAIInterface()->addSpellToList(NewSpell);
	//_unit->GetAIInterface()->m_spells.push_back( NewSpell );

	// Bone Barrier
	NewSpell = new AI_Spell;
	NewSpell->spell = dbcSpell.LookupEntry(DEATH_KNIGHT_UNDERSTUDY_BONE_BARRIER);
	NewSpell->agent = AGENT_SPELL;
	NewSpell->entryId = _unit->GetEntry();
	NewSpell->maxrange = GetMaxRange(dbcSpellRange.LookupEntry(NewSpell->spell->rangeIndex));
	NewSpell->minrange = GetMinRange(dbcSpellRange.LookupEntry(NewSpell->spell->rangeIndex));
	NewSpell->spelltargetType = TTYPE_CASTER;
	NewSpell->spellType = STYPE_BUFF;
	NewSpell->cooldown = objmgr.GetPetSpellCooldown(NewSpell->spell->Id);
	NewSpell->cooldowntime = 0;
	NewSpell->autocast_type = AUTOCAST_EVENT_NONE;
	NewSpell->floatMisc1 = 0;
	NewSpell->Misc2 = 0;
	NewSpell->procChance = 0;
	NewSpell->procCount = 0;
	_unit->GetAIInterface()->addSpellToList(NewSpell);
	//_unit->GetAIInterface()->m_spells.push_back( NewSpell );

	// Taunt
	NewSpell = new AI_Spell;
	NewSpell->spell = dbcSpell.LookupEntry(DEATH_KNIGHT_UNDERSTUDY_TAUNT);
	NewSpell->agent = AGENT_SPELL;
	NewSpell->entryId = _unit->GetEntry();
	NewSpell->maxrange = GetMaxRange(dbcSpellRange.LookupEntry(NewSpell->spell->rangeIndex));
	NewSpell->minrange = GetMinRange(dbcSpellRange.LookupEntry(NewSpell->spell->rangeIndex));
	NewSpell->spelltargetType = TTYPE_SINGLETARGET;
	NewSpell->spellType = STYPE_BUFF;
	NewSpell->cooldown = objmgr.GetPetSpellCooldown(NewSpell->spell->Id);
	NewSpell->cooldowntime = 0;
	NewSpell->autocast_type = AUTOCAST_EVENT_NONE;
	NewSpell->floatMisc1 = 0;
	NewSpell->Misc2 = 0;
	NewSpell->procChance = 0;
	NewSpell->procCount = 0;
	_unit->GetAIInterface()->addSpellToList(NewSpell);
	//_unit->GetAIInterface()->m_spells.push_back( NewSpell )

	mRazuviousAI = NULL;
};

void DeathKnightUnderstudyAI::Destroy()
{
	if(mRazuviousAI != NULL)
	{
		set< DeathKnightUnderstudyAI* >::iterator Iter = mRazuviousAI->mStudents.find(this);
		if(Iter != mRazuviousAI->mStudents.end())
			mRazuviousAI->mStudents.erase(Iter);

		mRazuviousAI = NULL;
	};

	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// The Construct Quarter

/////////////////////////////////////////////////////////////////////////////////
////// Patchwork Golem
#define CN_PATCHWORK_GOLEM						16017
#define PATCHWORK_GOLEM_CLEAVE					27794
#define PATCHWORK_GOLEM_DISEASE_CLOUD_NORMAL	27793
#define PATCHWORK_GOLEM_DISEASE_CLOUD_HEROIC	28362
#define PATCHWORK_GOLEM_EXECUTE_NORMAL			7160
#define PATCHWORK_GOLEM_EXECUTE_HEROIC			56426
#define PATCHWORK_GOLEM_WAR_STOMP_NORMAL		27758
#define PATCHWORK_GOLEM_WAR_STOMP_HEROIC		56427

class PatchworkGolemAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(PatchworkGolemAI, MoonScriptCreatureAI);
		PatchworkGolemAI(Creature* pCreature);

		void OnCombatStart(Unit* pTarget);
		void OnCombatStop(Unit* pTarget);
};

/////////////////////////////////////////////////////////////////////////////////
////// Bile Retcher
#define CN_BILE_RETCHER							16018
#define BILE_RETCHER_BILE_VOMIT_NORMAL			27807
#define BILE_RETCHER_BILE_VOMIT_HEROIC			54326
#define BILE_RETCHER_BILE_RETCHER_SLAM			27862

class BileRetcherAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(BileRetcherAI, MoonScriptCreatureAI);
		BileRetcherAI(Creature* pCreature);
};

/////////////////////////////////////////////////////////////////////////////////
////// Sewage Slime
#define CN_SEWAGE_SLIME							16375
#define SEWAGE_SLIME_DISEASE_CLOUD				28156

class SewageSlimeAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(SewageSlimeAI, MoonScriptCreatureAI);
		SewageSlimeAI(Creature* pCreature);

		void OnCombatStart(Unit* pTarget);
		void OnCombatStop(Unit* pTarget);
};

/////////////////////////////////////////////////////////////////////////////////
////// Embalming Slime
#define CN_EMBALMING_SLIME						16024
#define EMBALMING_SLIME_EMBALMING_CLOUD			28322

class EmbalmingSlimeAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(EmbalmingSlimeAI, MoonScriptCreatureAI);
		EmbalmingSlimeAI(Creature* pCreature);

		void OnCombatStart(Unit* pTarget);
		void OnCombatStop(Unit* pTarget);
};

/////////////////////////////////////////////////////////////////////////////////
////// Mad Scientist
#define CN_MAD_SCIENTIST						16020
#define MAD_SCIENTIST_GREAT_HEAL_NORMAL			28306
#define MAD_SCIENTIST_GREAT_HEAL_HEROIC			54337
#define MAD_SCIENTIST_MANA_BURN_NORMAL			28301
#define MAD_SCIENTIST_MANA_BURN_HEROIC			54338

class MadScientistAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(MadScientistAI, MoonScriptCreatureAI);
		MadScientistAI(Creature* pCreature);
};

/////////////////////////////////////////////////////////////////////////////////
////// Living Monstrosity
#define CN_LIVING_MONSTROSITY						16021
#define LIVING_MONSTROSITY_FEAR						27990
#define LIVING_MONSTROSITY_LIGHTNING_TOTEM			28294
#define LIVING_MONSTROSITY_CHAIN_LIGHTNING_NORMAL	28293
#define LIVING_MONSTROSITY_CHAIN_LIGHTNING_HEROIC	54334

class LivingMonstrosityAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(LivingMonstrosityAI, MoonScriptCreatureAI);
		LivingMonstrosityAI(Creature* pCreature);
};

/////////////////////////////////////////////////////////////////////////////////
////// Lightning Totem
#define CN_LIGHTNING_TOTEM							16385
#define LIGHTNING_TOTEM_SHOCK_NORMAL				28297
#define LIGHTNING_TOTEM_SHOCK_HEROIC				54333

class LightningTotemAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(LightningTotemAI, MoonScriptCreatureAI);
		LightningTotemAI(Creature* pCreature);

		void OnCombatStart(Unit* pTarget);
		void AIUpdate();
};

/////////////////////////////////////////////////////////////////////////////////
////// Stitched Colossus
#define CN_STITCHED_COLOSSUS					30071
#define STITCHED_COLOSSUS_MASSIVE_STOMP_NORMAL	55821
#define STITCHED_COLOSSUS_MASSIVE_STOMP_HEROIC	55826
#define STITCHED_COLOSSUS_UNSTOPPABLE_ENRAGE	54356

class StitchedColossusAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(StitchedColossusAI, MoonScriptCreatureAI);
		StitchedColossusAI(Creature* pCreature);

		void OnCombatStart(Unit* pTarget);
		void AIUpdate();

		bool	mEnraged;
};

/////////////////////////////////////////////////////////////////////////////////
////// Marauding Geist
#define CN_MARAUDING_GEIST						30083
#define MARAUDING_GEIST_FRENZIED_LEAP			56729

class MaraudingGeistAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(MaraudingGeistAI, MoonScriptCreatureAI);
		MaraudingGeistAI(Creature* pCreature);
};

/////////////////////////////////////////////////////////////////////////////////
////// Patchwerk

void SpellFunc_PatchwerkHatefulStrike(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);

#define CN_PATCHWERK 16028
#define PATCHWERK_FRENZY 28131
#define PATCHWERK_BERSERK 26662
#define PATCHWERK_HATEFUL_STRIKE_10 41926
#define PATCHWERK_HATEFUL_STRIKE_25 59192

class PatchwerkAI : public MoonScriptBossAI
{
		MOONSCRIPT_FACTORY_FUNCTION(PatchwerkAI, MoonScriptBossAI);
		PatchwerkAI(Creature* pCreature);

		void AIUpdate();

		bool	mEnraged;
};

// ---- Abomination Wing ----

// Stitched Spewer AI
#define STICKED_SPEWER 16025

#define CN_SLIME_BOLT 32309
#define CN_UPPERCUT 26007

class StickedSpewerAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(StickedSpewerAI);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		StickedSpewerAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			// -- Number of spells to add --
			nrspells = 2;

			// --- Initialization ---
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}
			// ----------------------

			// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not!
			spells[0].info = dbcSpell.LookupEntry(CN_SLIME_BOLT);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = false;
			spells[0].perctrigger = 10.0f;
			spells[0].attackstoptimer = 2000; // 1sec

			spells[1].info = dbcSpell.LookupEntry(CN_UPPERCUT);
			spells[1].targettype = TARGET_VARIOUS;
			spells[1].instant = false;
			spells[1].perctrigger = 10.0f;
			spells[1].attackstoptimer = 2000; // 1sec

		}

		void OnCombatStart(Unit* mTarget)
		{
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					if(!spells[i].perctrigger) continue;

					if(m_spellcheck[i])
					{
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}
						m_spellcheck[i] = false;
						return;
					}

					if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};



//Surgical Assistant AI
#define CN_SURGICAL_ASSISTANT 16022

#define MIND_FLAY 28310

class SurgicalAssistantAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(SurgicalAssistantAI);
		SP_AI_Spell spells[1];
		bool m_spellcheck[1];

		SurgicalAssistantAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			// -- Number of spells to add --
			nrspells = 1;

			// --- Initialization ---
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}
			// ----------------------

			// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not!
			spells[0].info = dbcSpell.LookupEntry(MIND_FLAY);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = false;
			spells[0].perctrigger = 20.0f;
			spells[0].attackstoptimer = 6000; // 1sec

		}

		void OnCombatStart(Unit* mTarget)
		{
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					if(!spells[i].perctrigger) continue;

					if(m_spellcheck[i])
					{
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}
						m_spellcheck[i] = false;
						return;
					}

					if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};


// Sludge Belcher AI
#define CN_SLUDGE_BELCHER 16029

#define DISEISE_BUFFET 27891
#define SUMMON_BILE_SLIMES 27889 // TODO: GAWD :P

class SludgeBelcherAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(SludgeBelcherAI);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		SludgeBelcherAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			// -- Number of spells to add --
			nrspells = 2;

			// --- Initialization ---
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}
			// ----------------------

			// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not!
			spells[0].info = dbcSpell.LookupEntry(DISEISE_BUFFET);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = false;
			spells[0].perctrigger = 20.0f;
			spells[0].attackstoptimer = 1000; // 1sec

			spells[1].info = dbcSpell.LookupEntry(SUMMON_BILE_SLIMES);
			spells[1].targettype = TARGET_SELF;
			spells[1].instant = false;
			spells[1].perctrigger = 5.0f;
			spells[1].attackstoptimer = 3000; // 1sec

		}

		void OnCombatStart(Unit* mTarget)
		{
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					if(!spells[i].perctrigger) continue;

					if(m_spellcheck[i])
					{
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}
						m_spellcheck[i] = false;
						return;
					}

					if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};

// BOSS'S

// Patchwerk AI


// Grobbulus AI
#define CN_GROBBULUS 15931

#define POISON_CLOUD_GROB 31259 // self
#define SLIME_SPRAY 28157 // various
#define SUMMON_FALLOUT_SLIME 28218 // TODO: cast on slime sprayd targets
#define MUTATING_INJECTION 28169 // TODO: DUMMY AURA

class GrobbulusAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(GrobbulusAI);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		GrobbulusAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			// -- Number of spells to add --
			nrspells = 2;

			// --- Initialization ---
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}
			// ----------------------

			// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not!
			spells[0].info = dbcSpell.LookupEntry(POISON_CLOUD_GROB);
			spells[0].targettype = TARGET_SELF;
			spells[0].instant = false;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000; // 1sec

			spells[1].info = dbcSpell.LookupEntry(SLIME_SPRAY);
			spells[1].targettype = TARGET_VARIOUS;
			spells[1].instant = false;
			spells[1].perctrigger = 15.0f;
			spells[1].attackstoptimer = 3000; // 1sec

		}

		void OnCombatStart(Unit* mTarget)
		{
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
			_unit->CastSpell(_unit, spells[0].info, spells[0].instant);
		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					if(!spells[i].perctrigger) continue;

					if(m_spellcheck[i])
					{
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}
						m_spellcheck[i] = false;
						return;
					}

					if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};


// Gluth AI
#define CN_GLUTH 15932

#define MORTAL_WOUND 28308 // target
#define DECIMATE 28374 // TODO: needs to be scripted
#define TERRIFYING_ROAR 37939
#define FRENZY 38664 // self

class GluthAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(GluthAI);
		SP_AI_Spell spells[3];
		bool m_spellcheck[3];

		GluthAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			// -- Number of spells to add --
			nrspells = 3;

			// --- Initialization ---
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}
			// ----------------------

			// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not!
			spells[0].info = dbcSpell.LookupEntry(MORTAL_WOUND);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = false;
			spells[0].perctrigger = 15.0f;
			spells[0].attackstoptimer = 2000; // 1sec

			spells[1].info = dbcSpell.LookupEntry(TERRIFYING_ROAR);
			spells[1].targettype = TARGET_VARIOUS;
			spells[1].instant = false;
			spells[1].perctrigger = 5.0f;
			spells[1].attackstoptimer = 3000; // 1sec

			spells[2].info = dbcSpell.LookupEntry(FRENZY);
			spells[2].targettype = TARGET_SELF;
			spells[2].instant = false;
			spells[2].perctrigger = 5.0f;
			spells[2].attackstoptimer = 1000; // 1sec

		}

		void OnCombatStart(Unit* mTarget)
		{
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					if(!spells[i].perctrigger) continue;

					if(m_spellcheck[i])
					{
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}
						m_spellcheck[i] = false;
						return;
					}

					if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};

// ---- Deathknight Wing ----

// Bony Construct AI
#define CN_BONY_CONSTRUCT 16167

#define SWEEPING_SLAM 25322
//TODO: Melee Chain Cleave - Strikes enemies in front of the Construct, chaining to anyone in melee range of the targets.

class BonyConstructAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(BonyConstructAI);
		SP_AI_Spell spells[1];
		bool m_spellcheck[1];

		BonyConstructAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			// -- Number of spells to add --
			nrspells = 1;

			// --- Initialization ---
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}
			// ----------------------

			// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not!
			spells[0].info = dbcSpell.LookupEntry(SWEEPING_SLAM);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = false;
			spells[0].perctrigger = 15.0f;
			spells[0].attackstoptimer = 2000; // 1sec

		}

		void OnCombatStart(Unit* mTarget)
		{
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					if(!spells[i].perctrigger) continue;

					if(m_spellcheck[i])
					{
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}
						m_spellcheck[i] = false;
						return;
					}

					if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};



// Death Lord AI
#define CN_DEATH_LORD 16861

#define AURA_OF_AGONY 28413

class DeathLordAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(DeathLordAI);
		SP_AI_Spell spells[1];
		bool m_spellcheck[1];

		DeathLordAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			// -- Number of spells to add --
			nrspells = 1;

			// --- Initialization ---
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}
			// ----------------------

			// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not!
			spells[0].info = dbcSpell.LookupEntry(AURA_OF_AGONY);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = false;
			spells[0].perctrigger = 15.0f;
			spells[0].attackstoptimer = 2000; // 1sec

		}

		void OnCombatStart(Unit* mTarget)
		{
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					if(!spells[i].perctrigger) continue;

					if(m_spellcheck[i])
					{
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}
						m_spellcheck[i] = false;
						return;
					}

					if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};

// -- BOSS'S --

// Instructor Razuvious AI
#define CN_INSTRUCTOR_RAZUVIOUS 16061

#define UNBALANCING_STRIKE 26613
#define DISRUPTING_SHOUT 29107

class RazuviousAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(RazuviousAI);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		RazuviousAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			// -- Number of spells to add --
			nrspells = 2;

			// --- Initialization ---
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}
			// ----------------------

			// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not!
			spells[0].info = dbcSpell.LookupEntry(DISRUPTING_SHOUT);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = false;
			spells[0].perctrigger = 15.0f;
			spells[0].attackstoptimer = 3000; // 1sec

			spells[1].info = dbcSpell.LookupEntry(UNBALANCING_STRIKE);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = false;
			spells[1].perctrigger = 35.0f;
			spells[1].attackstoptimer = 1000; // 1sec

		}

		void OnCombatStart(Unit* mTarget)
		{
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnTargetDied(Unit* mTarget)
		{
			if(Rand(50.0f))
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You should've stayed home!");
				_unit->PlaySoundToSet(8862);
			}
			else
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You disappoint me, students!");
				_unit->PlaySoundToSet(8863);
			}
		}

		void OnDied(Unit* mKiller)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "An honorable... death...");
			_unit->PlaySoundToSet(8860);
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					if(!spells[i].perctrigger) continue;

					if(m_spellcheck[i])
					{
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}
						m_spellcheck[i] = false;
						return;
					}

					if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};
// - The Four Horsemen: -

// Thane Korth'azz AI
#define CN_THANE_KORTHAZZ 16064

#define MARK_OF_KORTHAZZ 28832
#define METEOR 35181 // 1 target

class KorthazzAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(KorthazzAI);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		KorthazzAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			// -- Number of spells to add --
			nrspells = 2;

			// --- Initialization ---
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}
			// ----------------------

			tountcooldown = 6;
			tountcont = 0;
			m_attackstart = false;

			// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not!
			spells[0].info = dbcSpell.LookupEntry(MARK_OF_KORTHAZZ);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = false;
			spells[0].perctrigger = 5.0f;
			spells[0].attackstoptimer = 1000; // 1sec

			spells[1].info = dbcSpell.LookupEntry(METEOR);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = false;
			spells[1].perctrigger = 15.0f;
			spells[1].attackstoptimer = 3000; // 1sec

		}

		void OnCombatStart(Unit* mTarget)
		{
			m_attackstart = true;
			_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Come out and fight ye wee ninny!");
			_unit->PlaySoundToSet(8899);

			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void OnCombatStop(Unit* mTarget)
		{
			m_attackstart = false;
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnTargetDied(Unit* mTarget)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Next time, bring more friends!");
			_unit->PlaySoundToSet(8901);
		}

		void OnDied(Unit* mKiller)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "What a bloody waste this is!");
			_unit->PlaySoundToSet(8900);
			_unit->CastSpell(_unit, spells[1].info, spells[1].instant);
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			if(!m_attackstart)
			{
				if(!tountcooldown)
				{
					tountcooldown = 6;

					switch(tountcont)
					{
						case 0:
							_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "To arms, ye roustabouts! We've got company!");
							_unit->PlaySoundToSet(8903);
						case 1:
							_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "I heard about enough of yer sniveling. Shut yer fly trap 'afore I shut it for ye!");
							_unit->PlaySoundToSet(8904);
						case 2:
							_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "I'm gonna enjoy killin' these slack-jawed daffodils!");
							_unit->PlaySoundToSet(8905);

					}
					tountcont++;
					if(tountcont >= 3)
						tountcont = 0;
				}
				tountcooldown--;
			}

			if(m_spellcheck[0])
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "I like my meat extra crispy!");
				_unit->PlaySoundToSet(8902);
			}

			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					if(!spells[i].perctrigger) continue;

					if(m_spellcheck[i])
					{
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}
						m_spellcheck[i] = false;
						return;
					}

					if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		bool m_attackstart;
		uint32 tountcooldown, tountcont;
		int nrspells;
};

/*
8899 - Come out and fight ye wee ninny!
8900 - what a bloody waste this is!
8901 - next time. bring more friend
8902 - i like my meat extra crispy
8903 - To arms, ye roustabouts! We've got company!
8904 - I heard about enough of yer sniveling. Shut yer fly trap 'afore I shut it for ye!
8905 - I'm gonna enjoy killin' these slack-jawed daffodils!

8899, 'A_KOR_NAXX_AGGRO'
8900, 'A_KOR_NAXX_DEATH'
8901, 'A_KOR_NAXX_SLAY'
8902, 'A_KOR_NAXX_SPECIAL'
8903, 'A_KOR_NAXX_TAUNT01'
8904, 'A_KOR_NAXX_TAUNT02'
8905, 'A_KOR_NAXX_TAUNT03'
*/

// Baron Rivendare AI
#define CN_Baron_Rivendare_4H 30549 //4H not to confuse with Strat UD Side.. 
//Gief new Strat boss blizz or you make Stab a sad panda


// Lady Blaumeux AI
#define CN_LADY_BLAUMEUX 16065

#define MARK_OF_BLAUMEUX 28833
#define VOID_ZONE 28863 // TODO: DUMMY PART

class BlaumeuxAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(BlaumeuxAI);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		BlaumeuxAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			// -- Number of spells to add --
			nrspells = 2;

			// --- Initialization ---
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}
			// ----------------------

			tountcooldown = 16;
			tountcont = 0;
			m_attackstart = false;

			// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not!
			spells[0].info = dbcSpell.LookupEntry(MARK_OF_BLAUMEUX);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = false;
			spells[0].perctrigger = 5.0f;
			spells[0].attackstoptimer = 1000; // 1sec

			spells[1].info = dbcSpell.LookupEntry(VOID_ZONE);
			spells[1].targettype = TARGET_VARIOUS;
			spells[1].instant = false;
			spells[1].perctrigger = 15.0f;
			spells[1].attackstoptimer = 3000; // 1sec

		}

		void OnCombatStart(Unit* mTarget)
		{
			m_attackstart = true;
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Defend yourself!");
			_unit->PlaySoundToSet(8892);

			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void OnCombatStop(Unit* mTarget)
		{
			m_attackstart = false;
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnTargetDied(Unit* mTarget)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Who's next?");
			_unit->PlaySoundToSet(8894);
		}

		void OnDied(Unit* mKiller)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Tou...che.");
			_unit->PlaySoundToSet(8893);
			_unit->CastSpell(_unit, spells[1].info, spells[1].instant);
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			if(!m_attackstart)
			{
				if(!tountcooldown)
				{
					tountcooldown = 16;

					switch(tountcont)
					{
						case 0:
							_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Come, Zeliek, do not drive them out. Not before we've had our fun.");
							_unit->PlaySoundToSet(8896);
						case 1:
							_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "I do hope they stay alive long enough for me to... introduce myself.");
							_unit->PlaySoundToSet(8897);
						case 2:
							_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "The first kill goes to me! Anyone care to wager?");
							_unit->PlaySoundToSet(8898);

					}
					tountcont++;
					if(tountcont >= 3)
						tountcont = 0;
				}
				tountcooldown--;
			}

			if(m_spellcheck[0])
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Your life is mine!");
				_unit->PlaySoundToSet(8895);
			}

			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					if(!spells[i].perctrigger) continue;

					if(m_spellcheck[i])
					{
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}
						m_spellcheck[i] = false;
						return;
					}

					if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		bool m_attackstart;
		uint32 tountcooldown, tountcont;
		int nrspells;
};

/*
8892 - Defend yourself
8893 - Tou...che
8894 - Who's next?
8895 - Your life is mine!
8896 - Come, Zeliek, do not drive them out. Not before we've had our fun
8897 - I do hope they stay alive long enough for me to... introduce myself,
8898 - The first kill goes to me! Anyone care to wager?

8892, 'A_BLA_NAXX_AGGRO'
8893, 'A_BLA_NAXX_DEATH'
8894, 'A_BLA_NAXX_SLAY'
8895, 'A_BLA_NAXX_SPECIAL'
8896, 'A_BLA_NAXX_TAUNT01'
8897, 'A_BLA_NAXX_TAUNT02'
8898, 'A_BLA_NAXX_TAUNT03'
*/

// Sir Zeliek AI
#define CN_SIR_ZELIEK 16063

#define MARK_OF_ZELIEK 28835
#define HOLY_WRATH 32445 // 1target

class ZeliekAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(ZeliekAI);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		ZeliekAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			// -- Number of spells to add --
			nrspells = 2;

			// --- Initialization ---
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}
			// ----------------------

			tountcooldown = 13;
			tountcont = 0;
			m_attackstart = false;

			// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not!
			spells[0].info = dbcSpell.LookupEntry(MARK_OF_ZELIEK);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = false;
			spells[0].perctrigger = 5.0f;
			spells[0].attackstoptimer = 1000; // 1sec

			spells[1].info = dbcSpell.LookupEntry(HOLY_WRATH);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = false;
			spells[1].perctrigger = 15.0f;
			spells[1].attackstoptimer = 1000; // 1sec

		}

		void OnCombatStart(Unit* mTarget)
		{
			m_attackstart = true;
			_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Flee before its too late!");
			_unit->PlaySoundToSet(8913);

			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void OnCombatStop(Unit* mTarget)
		{
			m_attackstart = false;
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnTargetDied(Unit* mTarget)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Forgive me!");
			_unit->PlaySoundToSet(8915);
		}

		void OnDied(Unit* mKiller)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "It is... as it should be.");
			_unit->PlaySoundToSet(8914);
			_unit->CastSpell(_unit, spells[1].info, spells[1].instant);
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			if(!m_attackstart)
			{
				if(!tountcooldown)
				{
					tountcooldown = 13;

					switch(tountcont)
					{
						case 0:
							_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Invaders, cease this foolish venture at once! Turn away while you still can!");
							_unit->PlaySoundToSet(8917);
						case 1:
							_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Perhaps they will come to their senses, and run away as fast as they can!");
							_unit->PlaySoundToSet(8918);
						case 2:
							_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Do not continue! Turn back while there's still time!");
							_unit->PlaySoundToSet(8919);

					}
					tountcont++;
					if(tountcont >= 3)
						tountcont = 0;
				}
				tountcooldown--;
			}

			if(m_spellcheck[0])
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "i have no choice but to obey!");
				_unit->PlaySoundToSet(8916);
			}

			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					if(!spells[i].perctrigger) continue;

					if(m_spellcheck[i])
					{
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}
						m_spellcheck[i] = false;
						return;
					}

					if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		bool m_attackstart;
		uint32 tountcooldown, tountcont;
		int nrspells;
};

// -- Sapphiron Encounter by M4ksiu -- //

// Settings

#define DISABLE_FROST_BREATH

// Frost Breath TrgiggerAI

#define CN_FROST_BREATH_TRIGGER		50010	// Flying "ball" of frost energy
#define CN_FROST_BREATH_TRIGGER2	50011	// Explosion
#define CN_FROST_BREATH_TRIGGER3	50012	// Wing Buffet
#define CN_CHILL_TRIGGER			50013	// Mass chill trigger (used, because we can't cast many aoe triggers on one character >_>)

// Air phase spells
#define FROST_BREATH		28524
#define FROST_BREATH_EFFECT	30101
#define FROST_BREATH_DAMAGE	29318

// Additional spells
#define SAPPHIRONS_WING_BUFFET	29328

struct Location PhaseTwoWP[] =
{
	{  },
	{ 3520.820068f, -5233.799805f, 137.626007f, 4.553010f }
};

struct Location IceBlocks[] =	// Those are not blizzlike pos, because those blocks are spawned randomly
{
	{  },
	{ 3580.986084f, -5241.330078f, 137.627304f, 3.006957f },
	{ 3562.967285f, -5257.952148f, 137.860916f, 2.468959f },
	{ 3569.620850f, -5276.108398f, 137.582733f, 2.480744f },
	{ 3551.420410f, -5283.535156f, 137.731903f, 2.009505f },
	{ 3535.933594f, -5294.710938f, 138.080002f, 1.823366f },
	{ 3522.235107f, -5286.610352f, 138.115601f, 1.532768f },
	{ 3503.184814f, -5296.418945f, 138.111252f, 1.222535f },
	{ 3489.055664f, -5278.863770f, 138.119934f, 0.884814f },
	{ 3473.002686f, -5277.641602f, 137.733414f, 0.680609f },
	{ 3472.302734f, -5255.734863f, 137.755569f, 0.331107f },
	{ 3458.193848f, -5241.013672f, 137.566147f, 0.111195f },
	{ 3463.324463f, -5221.530273f, 137.634888f, 6.084152f },
	{ 3467.574219f, -5200.617676f, 137.559662f, 5.860314f },
	{ 3479.394775f, -5178.301758f, 140.904312f, 5.405583f },
	{ 3507.219727f, -5180.725098f, 140.625473f, 4.431685f },
	{ 3518.371338f, -5172.666504f, 142.269135f, 4.694800f },
	{ 3542.516846f, -5184.699707f, 140.655182f, 4.470973f },
	{ 3559.013916f, -5183.916016f, 140.899689f, 4.644558f },
	{ 3559.006592f, -5183.923340f, 140.895554f, 3.952624f },
	{ 3571.978760f, -5209.633301f, 137.671906f, 3.514374f }
};

/*struct Spawns IceBlocks[]=	// Those are not blizzlike pos, because those blocks are spawned randomly
{
	{  },
	{ 3580.986084f, -5241.330078f, 137.627304f, 3.006957f },
	{ 3562.967285f, -5257.952148f, 137.860916f, 2.468959f },
	{ 3569.620850f, -5276.108398f, 137.582733f, 2.480744f },

	{ 3535.933594f, -5294.710938f, 138.080002f, 1.823366f },
	{ 3522.235107f, -5286.610352f, 138.115601f, 1.532768f },
	{ 3503.184814f, -5296.418945f, 138.111252f, 1.222535f },

	{ 3473.002686f, -5277.641602f, 137.733414f, 0.680609f },
	{ 3472.302734f, -5255.734863f, 137.755569f, 0.331107f },
	{ 3458.193848f, -5241.013672f, 137.566147f, 0.111195f },

	{ 3467.574219f, -5200.617676f, 137.559662f, 5.860314f },
	{ 3479.394775f, -5178.301758f, 140.904312f, 5.405583f },
	{ 3507.219727f, -5180.725098f, 140.625473f, 4.431685f },

	{ 3542.516846f, -5184.699707f, 140.655182f, 4.470973f },
	{ 3559.013916f, -5183.916016f, 140.899689f, 4.644558f },
	{ 3559.006592f, -5183.923340f, 140.895554f, 3.952624f },

	{ 3551.420410f, -5283.535156f, 137.731903f, 2.009505f },
	{ 3489.055664f, -5278.863770f, 138.119934f, 0.884814f },
	{ 3463.324463f, -5221.530273f, 137.634888f, 6.084152f },
	{ 3518.371338f, -5172.666504f, 142.269135f, 4.694800f },
	{ 3571.978760f, -5209.633301f, 137.671906f, 3.514374f }
};*/

class FrostBreathTriggerAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(FrostBreathTriggerAI);

		FrostBreathTriggerAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			_unit->GetAIInterface()->MoveTo(PhaseTwoWP[1].x, PhaseTwoWP[1].y, PhaseTwoWP[1].z + 10.5f, PhaseTwoWP[1].o);
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			_unit->GetAIInterface()->SetFly();
			_unit->m_noRespawn = true;
			_unit->Despawn(7000, 0);

			RegisterAIUpdateEvent(1000);

			AICounter = 7;
		}

		void OnCombatStart(Unit* mTarget) {}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			_unit->CastSpell(_unit, FROST_BREATH_EFFECT, true);

			AICounter--;
			if(AICounter == 6)
				_unit->GetAIInterface()->MoveTo(PhaseTwoWP[1].x, PhaseTwoWP[1].y, PhaseTwoWP[1].z + AICounter * 1.5f, PhaseTwoWP[1].o);
			else
				_unit->GetAIInterface()->MoveTo(PhaseTwoWP[1].x, PhaseTwoWP[1].y, PhaseTwoWP[1].z, PhaseTwoWP[1].o);
		}

	protected:

		int AICounter;
};

class FrostBreathTrigger2AI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(FrostBreathTrigger2AI);

		FrostBreathTrigger2AI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
#ifdef DISABLE_FROST_BREATH
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
#else
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
#endif
			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->m_canMove = false;
			_unit->m_noRespawn = true;
			_unit->Despawn(8000, 0);

			_unit->CastSpell(_unit, FROST_BREATH, false);
		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
		}
};

class FrostBreathTrigger3AI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(FrostBreathTrigger3AI);

		FrostBreathTrigger3AI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
			_unit->CastSpell(_unit, SAPPHIRONS_WING_BUFFET, true);
			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->m_canMove = false;
			_unit->m_noRespawn = true;

			RegisterAIUpdateEvent(1000);
		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			_unit->CastSpell(_unit, SAPPHIRONS_WING_BUFFET, true);
		}
};

class ChillTriggerAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(ChillTriggerAI);

		ChillTriggerAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			_unit->CastSpellAoF(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), dbcSpell.LookupEntry(28547), true);
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->m_canMove = false;
			_unit->m_noRespawn = true;
			_unit->Despawn(15000, 0);
		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		}
};

// SapphironAI
// Missing spawning effect with building skeleton of Sappiron
#define CN_SAPPHIRON	15989

// Land phase spells
#define LIFE_DRAIN					28542
#define CHILL						28547
#define FROST_AURA					28531

// Air phase spells
#define ICEBOLT						28522

// Additional spells
#define SAPPHIRON_DIES				29357
#define BERSERK						26662	// 28498 - casts frostbolt (would be cool for Sapphiron), but every 2 sec for 5 min (~16k dmg per hit);
// 27680 - 10 mins instead 5 mins
// Researches
#define SAPPHIRON_BIRTH				181356
#define FROSTWYRM_WATERFALL_DOOR	181225
#define ICE_BLOCK_GO				181247

// Move types
#define WALK	0
#define RUN		256
#define FLY		768

// Immunities
#define IMMUNITY_DISEASE			6681
#define IMMUNITY_SHADOW				7743
#define IMMUNITY_FROST				7940
#define IMMUNITY_NATURE				7941
#define IMMUNITY_FIRE				7942
#define IMMUNITY_HOLY				34182
#define IMMUNITY_ARCANE				34184
#define IMMUNITY_PHYSICAL			34310

class SapphironAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(SapphironAI);
		SP_AI_Spell spells[4];
		bool m_spellcheck[4];

		SapphironAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			_unit->GetAIInterface()->addWayPoint(CreateWaypoint(1, 3000, RUN));

			nrspells = 2;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
				spells[i].casttime = 0;
			}

			spells[0].info = dbcSpell.LookupEntry(LIFE_DRAIN);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = true;
			spells[0].perctrigger = 8.0f;
			spells[0].cooldown = 20;
			spells[0].attackstoptimer = 2000;

			spells[1].info = dbcSpell.LookupEntry(CHILL);
			spells[1].targettype = TARGET_RANDOM_DESTINATION;
			spells[1].instant = true;
			spells[1].perctrigger = 10.0f;
			spells[1].cooldown = 15;
			spells[1].attackstoptimer = 1000;
			spells[1].mindist2cast = 0.0f;
			spells[1].maxdist2cast = 40.0f;
			spells[1].minhp2cast = 0;
			spells[1].maxhp2cast = 100;

			spells[2].info = dbcSpell.LookupEntry(ICEBOLT);
			spells[2].targettype = TARGET_RANDOM_SINGLE;
			spells[2].instant = true;
			spells[2].perctrigger = 0.0f;
			spells[2].cooldown = 0;
			spells[2].attackstoptimer = 1000;
			spells[2].mindist2cast = 0.0f;
			spells[2].maxdist2cast = 70.0f;
			spells[2].minhp2cast = 0;
			spells[2].maxhp2cast = 100;

			spells[3].info = dbcSpell.LookupEntry(BERSERK);
			spells[3].targettype = TARGET_SELF;
			spells[3].instant = true;
			spells[3].perctrigger = 0.0f;
			spells[3].cooldown = 900;
			spells[3].attackstoptimer = 1000;

			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
			_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
			_unit->GetAIInterface()->StopFlying();
			_unit->GetAIInterface()->m_canMove = true;
			_unit->CastSpell(_unit, IMMUNITY_FROST, true);

			WorldPacket data(SMSG_MOVE_UNSET_HOVER, 13);
			data << _unit->GetNewGUID();
			data << uint32(0);
			_unit->SendMessageToSet(&data, false);

			ChillTarget = NULL;
			FlightActions = 0;
			ChillCounter = 0;
			PhaseTimer = 0;
			m_phase = 1;
		}

		void OnCombatStart(Unit* mTarget)
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = 0;

			spells[3].casttime = (uint32)time(NULL) + spells[3].cooldown;

			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
			_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
			_unit->GetAIInterface()->StopFlying();
			_unit->GetAIInterface()->m_canMove = true;

			WorldPacket data(SMSG_MOVE_UNSET_HOVER, 13);
			data << _unit->GetNewGUID();
			data << uint32(0);
			_unit->SendMessageToSet(&data, false);

			GameObject* Waterfall = NULL;
			Waterfall = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(3536.852783f, -5159.951172f, 143.636139f, FROSTWYRM_WATERFALL_DOOR);
			if(Waterfall != NULL)
			{
				Waterfall->SetState(1);
			}

			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));

			PhaseTimer = (uint32)time(NULL) + 35;
			ChillTarget = NULL;
			FlightActions = 0;
			ChillCounter = 0;
			m_phase = 1;
		}

		void OnCombatStop(Unit* mTarget)
		{
			Creature* BreathTrigger = NULL;
			BreathTrigger = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(PhaseTwoWP[1].x, PhaseTwoWP[1].y, PhaseTwoWP[1].z, CN_FROST_BREATH_TRIGGER3);
			if(BreathTrigger != NULL)
				BreathTrigger->Despawn(0, 0);

			for(int i = 1; i < 21; i++)
			{
				GameObject* IceBlock = NULL;
				IceBlock = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(IceBlocks[i].x, IceBlocks[i].y, IceBlocks[i].z, ICE_BLOCK_GO);
				if(IceBlock != NULL)
				{
					IceBlock->Delete();
				}
			}

			GameObject* Waterfall = NULL;
			Waterfall = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(3536.852783f, -5159.951172f, 143.636139f, FROSTWYRM_WATERFALL_DOOR);
			if(Waterfall != NULL)
			{
				Waterfall->SetState(0);
			}

			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
			_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
			_unit->GetAIInterface()->StopFlying();
			_unit->GetAIInterface()->m_canMove = true;

			WorldPacket data(SMSG_MOVE_UNSET_HOVER, 13);
			data << _unit->GetNewGUID();
			data << uint32(0);
			_unit->SendMessageToSet(&data, false);

			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);

			RemoveAIUpdateEvent();
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnDied(Unit* mKiller)
		{
			_unit->CastSpell(_unit, SAPPHIRON_DIES, true);

			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			uint32 t = (uint32)time(NULL);
			if(t > spells[3].casttime && _unit->GetCurrentSpell() == NULL)
			{
				_unit->CastSpell(_unit, spells[3].info, spells[3].instant);

				spells[3].casttime = t + spells[3].cooldown;
			}

			switch(m_phase)
			{
				case 1:
					PhaseOne();
					break;
				case 2:
					PhaseTwo();
					break;
				default:
					{
					}
			}
		}

		void PhaseOne()
		{
			_unit->CastSpell(_unit, FROST_AURA, true);

			if(_unit->GetAIInterface()->getMoveType() == MOVEMENTTYPE_WANTEDWP)
				return;

			if(_unit->GetHealthPct() > 10)
			{
				uint32 t = (uint32)time(NULL);
				if(t > PhaseTimer)
				{
					if(_unit->GetCurrentSpell() != NULL)
						_unit->GetCurrentSpell()->cancel();

					_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
					_unit->GetAIInterface()->StopMovement(0);
					_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
					_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
					_unit->GetAIInterface()->setWaypointToMove(1);

					return;
				}
			}

			if(ChillCounter > 0)
			{
				ChillCounter--;
				if(ChillTarget != NULL)
				{
					_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_CHILL_TRIGGER, ChillTarget->GetPositionX(), ChillTarget->GetPositionY(), ChillTarget->GetPositionZ(), ChillTarget->GetOrientation(), true, false, 0, 0);
				}

				if(ChillCounter == 0)
				{
					ChillTarget = NULL;
				}
			}

			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void PhaseTwo()
		{
			if(FlightActions == 0)
			{
				_unit->GetAIInterface()->m_canMove = false;
				_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
				_unit->GetAIInterface()->setCurrentAgent(AGENT_SPELL);
				_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
				_unit->GetAIInterface()->setWaypointToMove(0);
			}

			if(FlightActions < 5)
			{
				if(_unit->GetCurrentSpell() == NULL)
				{
					if(_unit->GetAIInterface()->getNextTarget() != NULL)
					{
						CastSpellOnRandomTarget(2, 0.0f, 40.0f, 0, 100);

						FlightActions++;
						if(FlightActions >= 5)
						{
							uint32 LastOne = 0;
							for(int i = 0; i < 2; i++)
							{
								uint32 Block = 0;
								while(LastOne == Block)
								{
									Block = RandomUInt(5) + 15;
								}

								LastOne = Block;

								GameObject* IceBlock = NULL;
								IceBlock = _unit->GetMapMgr()->GetInterface()->SpawnGameObject(ICE_BLOCK_GO, IceBlocks[Block].x, IceBlocks[Block].y, IceBlocks[Block].z, IceBlocks[Block].o, true, 0, 0);
								if(IceBlock != NULL)
								{
									IceBlock->SetUInt32Value(GAMEOBJECT_FLAGS, 1);
								}
							}

							RemoveAIUpdateEvent();
							RegisterAIUpdateEvent(3000);

							FlightActions = 5;
						}

						if(FlightActions == 2)
						{
							for(int i = 0; i < 2; i++)
							{
								uint32 Block = 0;
								if(i == 0)
									Block = RandomUInt(3) + 1;
								else
									Block = RandomUInt(3) + 10;

								GameObject* IceBlock = NULL;
								IceBlock = _unit->GetMapMgr()->GetInterface()->SpawnGameObject(ICE_BLOCK_GO, IceBlocks[Block].x, IceBlocks[Block].y, IceBlocks[Block].z, IceBlocks[Block].o, true, 0, 0);
								if(IceBlock != NULL)
								{
									IceBlock->SetUInt32Value(GAMEOBJECT_FLAGS, 1);
								}
							}
						}

						if(FlightActions == 4)
						{
							for(int i = 0; i < 2; i++)
							{
								uint32 Block = 0;
								if(i == 0)
									Block = RandomUInt(3) + 7;
								else
									Block = RandomUInt(9) + 13;

								GameObject* IceBlock = NULL;
								IceBlock = _unit->GetMapMgr()->GetInterface()->SpawnGameObject(ICE_BLOCK_GO, IceBlocks[Block].x, IceBlocks[Block].y, IceBlocks[Block].z, IceBlocks[Block].o, true, 0, 0);
								if(IceBlock != NULL)
								{
									IceBlock->SetUInt32Value(GAMEOBJECT_FLAGS, 1);
								}
							}
						}
					}
				}
			}

			else
			{
				if(FlightActions == 5)
				{
					Unit* FlyingFrostBreath = NULL;
					FlyingFrostBreath = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FROST_BREATH_TRIGGER, PhaseTwoWP[1].x, PhaseTwoWP[1].y, PhaseTwoWP[1].z + 18.0f, _unit->GetOrientation(), true, false, 0, 0);
					if(FlyingFrostBreath != NULL)
					{
						FlyingFrostBreath->GetAIInterface()->MoveTo(PhaseTwoWP[1].x, PhaseTwoWP[1].y, PhaseTwoWP[1].z, PhaseTwoWP[1].o);
					}

					_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FROST_BREATH_TRIGGER2, PhaseTwoWP[1].x, PhaseTwoWP[1].y, PhaseTwoWP[1].z, _unit->GetOrientation(), true, false, 0, 0);

					RemoveAIUpdateEvent();
					RegisterAIUpdateEvent(10000);

					Creature* BreathTrigger = NULL;
					BreathTrigger = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(PhaseTwoWP[1].x, PhaseTwoWP[1].y, PhaseTwoWP[1].z, CN_FROST_BREATH_TRIGGER3);
					if(BreathTrigger != NULL)
						BreathTrigger->Despawn(0, 0);
				}

				if(FlightActions == 6)
				{
					for(int i = 1; i < 21; i++)
					{
						GameObject* IceBlock = NULL;
						IceBlock = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(IceBlocks[i].x, IceBlocks[i].y, IceBlocks[i].z, ICE_BLOCK_GO);
						if(IceBlock != NULL)
						{
							IceBlock->Delete();
						}
					}

					_unit->GetAIInterface()->StopFlying();
					_unit->Emote(EMOTE_ONESHOT_LAND);

					WorldPacket data(SMSG_MOVE_UNSET_HOVER, 13);
					data << _unit->GetNewGUID();
					data << uint32(0);
					_unit->SendMessageToSet(&data, false);

					RemoveAIUpdateEvent();
					RegisterAIUpdateEvent(3000);
				}

				if(FlightActions == 7)
				{
					_unit->GetAIInterface()->m_canMove = true;
					_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
					_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
					_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
					_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
					_unit->GetAIInterface()->setWaypointToMove(0);

					RemoveAIUpdateEvent();
					RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));

					PhaseTimer = (uint32)time(NULL) + 67;
					ChillTarget = NULL;
					FlightActions = 0;
					ChillCounter = 0;
					m_phase = 1;
				}

				FlightActions++;
			}
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget() != NULL)
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					if(!spells[i].perctrigger) continue;

					if(m_spellcheck[i])
					{
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
							case TARGET_RANDOM_FRIEND:
							case TARGET_RANDOM_SINGLE:
							case TARGET_RANDOM_DESTINATION:
								CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast);
								break;
						}

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
						}

						m_spellcheck[i] = false;
						return;
					}

					uint32 t = (uint32)time(NULL);
					if((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) && t > spells[i].casttime)
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						spells[i].casttime = t + spells[i].cooldown;
						m_spellcheck[i] = true;
					}

					comulativeperc += spells[i].perctrigger;
				}
			}
		}

		void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
		{
			if(!maxdist2cast) maxdist2cast = 100.0f;
			if(!maxhp2cast) maxhp2cast = 100;

			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget() != NULL)
			{
				std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
				/* If anyone wants to use this function, then leave this note!										 */
				for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr)
				{
					if(isHostile(_unit, (*itr)) && (*itr) != _unit && (*itr)->IsUnit())
					{
						Unit* RandomTarget = NULL;
						RandomTarget = TO_UNIT(*itr);

						if(RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast * mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast * maxdist2cast && _unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))
						{
							TargetTable.push_back(RandomTarget);
						}
					}
				}

				if(!TargetTable.size())
					return;

				size_t RandTarget = rand() % TargetTable.size();

				Unit*  RTarget = TargetTable[RandTarget];

				if(RTarget == NULL)
					return;

				if(i == 1)
				{
					ChillCounter = RandomUInt(3) + 3;
					ChillTarget = RTarget;
				}

				else
				{
					switch(spells[i].targettype)
					{
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
							_unit->CastSpell(RTarget, spells[i].info, spells[i].instant);
							break;
						case TARGET_RANDOM_DESTINATION:
							_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant);
							break;
					}
				}

				TargetTable.clear();
			}
		}

		void OnReachWP(uint32 iWaypointId, bool bForwards)
		{
			if(iWaypointId == 1)
			{
				_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FROST_BREATH_TRIGGER3, PhaseTwoWP[1].x, PhaseTwoWP[1].y, PhaseTwoWP[1].z, _unit->GetOrientation(), true, false, 0, 0);
				_unit->GetAIInterface()->SetFly();
				_unit->Emote(EMOTE_ONESHOT_LIFTOFF);

				WorldPacket data(SMSG_MOVE_SET_HOVER, 13);
				data << _unit->GetNewGUID();
				data << uint32(0);
				_unit->SendMessageToSet(&data, false);

				RemoveAIUpdateEvent();
				RegisterAIUpdateEvent(3500);

				ChillTarget = NULL;
				FlightActions = 0;
				ChillCounter = 0;
				m_phase = 2;
			}
		}

		inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
		{
			WayPoint* wp = _unit->CreateWaypointStruct();
			wp->id = id;
			wp->x = PhaseTwoWP[id].x;
			wp->y = PhaseTwoWP[id].y;
			wp->z = PhaseTwoWP[id].z;
			wp->o = PhaseTwoWP[id].o;
			wp->waittime = waittime;
			wp->flags = flags;
			wp->forwardemoteoneshot = 0;
			wp->forwardemoteid = 0;
			wp->backwardemoteoneshot = 0;
			wp->backwardemoteid = 0;
			wp->forwardskinid = 0;
			wp->backwardskinid = 0;
			return wp;
		}

	protected:

		Unit* ChillTarget;	// I don't like it >_>

		uint32 FlightActions;
		uint32 ChillCounter;
		uint32 PhaseTimer;
		uint32 m_phase;
		int nrspells;
};

// -- Kel'thuzad Encounter by M4ksiu -- //

// Encounter mobs

#define CN_THE_LICH_KING 16980
#define CN_SOLDIER_OF_THE_FROZEN_WASTES 16427
#define CN_UNSTOPPABLE_ABOMINATION 16428
#define CN_SOUL_WEAVER 16429
#define CN_GUARDIAN_OF_ICECROWN 16441

/*
  _____
 /  K  \
/1     4\
|       |
|2     5|
|       |
\3     6/
 \_ 7 _/
   | |

*/

static Location SFrozenWastes[] =	// Soldier of the Frozen Wastes (no idea about those :| )
{
	{ 3759.149902f, -5074.879883f, 143.175003f, 1.203640f },	// 1
	{ 3762.959961f, -5067.399902f, 143.453003f, 0.893413f },
	{ 3772.419922f, -5076.379883f, 143.466995f, 3.606970f },
	{ 3779.699951f, -5078.180176f, 143.764008f, 4.038940f },
	{ 3770.219971f, -5065.740234f, 143.477005f, 0.630304f },
	{ 3765.709961f, -5060.799805f, 143.748001f, 1.608120f },
	{ 3776.909912f, -5066.100098f, 143.550003f, 5.130640f },
	{ 3782.659912f, -5069.529785f, 143.757004f, 5.150280f },
	{ 3773.909912f, -5059.589844f, 143.774002f, 6.257680f },
	{ 3780.260010f, -5061.580078f, 143.742996f, 5.169910f },
	{ 3721.429932f, -5052.759766f, 143.442993f, 1.457330f },	// 2
	{ 3732.149902f, -5051.589844f, 143.444000f, 1.017500f },
	{ 3741.889893f, -5047.439941f, 143.886002f, 3.075250f },
	{ 3726.229980f, -5043.410156f, 143.455994f, 6.150070f },
	{ 3718.679932f, -5042.520020f, 143.768005f, 1.614410f },
	{ 3733.060059f, -5040.979980f, 143.557007f, 1.669380f },
	{ 3741.860107f, -5038.410156f, 143.917999f, 4.410420f },
	{ 3736.189941f, -5032.810059f, 143.847000f, 5.026970f },
	{ 3723.219971f, -5035.770020f, 143.764999f, 4.701020f },
	{ 3728.760010f, -5031.759766f, 143.785995f, 3.723200f },
	{ 3683.189941f, -5062.419922f, 143.175995f, 0.559623f },	// 3
	{ 3687.739990f, -5057.779785f, 143.175995f, 1.345020f },
	{ 3674.040039f, -5067.899902f, 143.524994f, 0.005909f },
	{ 3688.340088f, -5052.009766f, 143.473007f, 2.299280f },
	{ 3681.659912f, -5052.549805f, 143.263000f, 3.343860f },
	{ 3678.840080f, -5055.529785f, 143.332993f, 3.512720f },
	{ 3685.800049f, -5045.779785f, 143.615997f, 6.253750f },
	{ 3673.479980f, -5053.350098f, 143.554993f, 1.945840f },
	{ 3670.649902f, -5061.149902f, 143.820007f, 4.266700f },
	{ 3688.018311f, -5061.541992f, 143.178223f, 0.512497f },
	{ 3742.659912f, -5157.009766f, 143.171005f, 0.773261f },	// 4
	{ 3750.310059f, -5153.779785f, 143.179993f, 5.603460f },
	{ 3739.770020f, -5163.000000f, 143.662994f, 1.095270f },
	{ 3746.649902f, -5164.560059f, 143.520004f, 1.970990f },
	{ 3748.479980f, -5169.109863f, 143.649002f, 2.591460f },
	{ 3744.560059f, -5173.520020f, 143.895996f, 1.370160f },
	{ 3758.260010f, -5156.299805f, 143.570999f, 6.192510f },
	{ 3762.620117f, -5159.149902f, 143.712997f, 4.433220f },
	{ 3760.840088f, -5162.959961f, 143.649002f, 4.280060f },
	{ 3756.580078f, -5170.060059f, 143.623001f, 3.031280f },
	{ 3705.229980f, -5163.189941f, 143.266998f, 5.692200f },	// 5
	{ 3696.949951f, -5163.370117f, 143.608002f, 0.901274f },
	{ 3710.080078f, -5168.709961f, 143.585999f, 4.675110f },
	{ 3697.030029f, -5170.220215f, 143.606003f, 0.343642f },
	{ 3692.030029f, -5170.020020f, 143.815994f, 0.932691f },
	{ 3710.320068f, -5175.319824f, 143.705002f, 4.639770f },
	{ 3694.550049f, -5177.540039f, 143.839005f, 1.549230f },
	{ 3705.830078f, -5179.120117f, 143.729996f, 3.956480f },
	{ 3712.800049f, -5179.109863f, 143.891006f, 3.855950f },
	{ 3701.669922f, -5181.859863f, 143.804001f, 1.380370f },
	{ 3676.590088f, -5144.899902f, 143.186005f, 3.575550f },	// 6
	{ 3670.649902f, -5142.629883f, 143.173996f, 4.313820f },
	{ 3665.689941f, -5136.279785f, 143.177994f, 1.176150f },
	{ 3664.870117f, -5132.330078f, 143.205002f, 3.249600f },
	{ 3662.120117f, -5144.299805f, 143.320007f, 2.256080f },
	{ 3658.979980f, -5139.490234f, 143.421997f, 6.077040f },
	{ 3651.850098f, -5137.140137f, 143.764999f, 0.799161f },
	{ 3654.689941f, -5144.009766f, 143.546997f, 2.519180f },
	{ 3666.020020f, -5149.500000f, 143.587006f, 3.359560f },
	{ 3667.959961f, -5153.120117f, 143.791000f, 4.015380f },
	{ 3659.979980f, -5100.450195f, 143.453003f, 5.168254f },	// 7
	{ 3662.800049f, -5088.189941f, 143.557999f, 4.409650f },
	{ 3658.439941f, -5090.649902f, 143.470001f, 5.528840f },
	{ 3652.810059f, -5090.450195f, 143.475998f, 1.362310f },
	{ 3651.780029f, -5095.339844f, 143.388000f, 0.981387f },
	{ 3652.629883f, -5098.970215f, 143.544998f, 2.799580f },
	{ 3652.679932f, -5083.479980f, 143.774994f, 0.482659f },
	{ 3647.189941f, -5085.490234f, 143.570999f, 1.919940f },
	{ 3645.120117f, -5097.240234f, 143.487000f, 0.765403f },
	{ 3646.360107f, -5101.200195f, 143.681000f, 2.909540f }
};

static Location Abomination[] =	// Unstoppable Abomination
{
	{ 3776.229980f, -5081.439941f, 143.779999f, 4.043730f },	// 1
	{ 3774.419922f, -5071.490234f, 143.423996f, 4.214940f },
	{ 3759.850098f, -5064.479980f, 143.636002f, 6.255410f },
	{ 3740.899902f, -5052.740234f, 143.785995f, 1.836760f },	// 2
	{ 3726.919922f, -5040.020020f, 143.535995f, 2.481570f },
	{ 3715.010010f, -5049.259766f, 143.632004f, 5.071810f },
	{ 3695.060059f, -5052.160156f, 143.548004f, 3.792400f },	// 3
	{ 3678.129883f, -5048.060059f, 143.436996f, 5.363980f },
	{ 3676.120117f, -5061.359863f, 143.492004f, 5.726840f },
	{ 3756.780029f, -5149.419922f, 143.460007f, 2.774530f },	// 4
	{ 3752.262695f, -5164.782227f, 143.409119f, 0.383768f },
	{ 3740.669922f, -5168.600098f, 143.873993f, 1.051360f },
	{ 3714.020020f, -5171.129883f, 143.776993f, 1.742510f },	// 5
	{ 3702.185303f, -5174.303711f, 143.532303f, 5.443298f },
	{ 3693.739990f, -5162.149902f, 143.748001f, 5.696990f },
	{ 3673.189941f, -5150.500000f, 143.751999f, 3.030570f },	// 6
	{ 3658.570068f, -5147.799805f, 143.494003f, 1.230440f },
	{ 3659.560059f, -5132.129883f, 143.677002f, 5.988380f },
	{ 3651.130859f, -5104.800293f, 143.798248f, 5.374194f },	// 7
	{ 3646.947021f, -5092.266113f, 143.305878f, 5.005841f },
	{ 3658.618408f, -5083.832031f, 143.778641f, 5.951464f }
};

static Location SoulWeaver[] =	// Soul Weaver
{
	{ 3768.540039f, -5075.140137f, 143.203995f, 5.096160f },
	{ 3728.030029f, -5047.359863f, 143.306000f, 5.230460f },
	{ 3682.929932f, -5055.819824f, 143.184006f, 5.368690f },
	{ 3749.429932f, -5160.419922f, 143.283997f, 4.723090f },
	{ 3706.120117f, -5169.250000f, 143.436996f, 2.682630f },
	{ 3665.310059f, -5142.339844f, 143.220001f, 1.147180f },
	{ 3656.365234f, -5094.724121f, 143.306641f, 6.203571f }
};

static Location Guardians[] =		// Guardians of Icecrown
{
	{ 3778.371582f, -5065.141113f, 143.614639f, 3.700061f },
	{ 3731.733398f, -5032.681152f, 143.775040f, 4.485459f },
	{ 3758.592285f, -5170.157715f, 143.667297f, 2.144972f },
	{ 3700.936279f, -5183.230469f, 143.858582f, 1.314648f }
};

static Location Waves[] =			// Spawn positions of units that attack circle
{
	{ 3756.380615f, -5080.560059f, 142.906921f, 3.762599f },
	{ 3726.448242f, -5058.546387f, 142.467331f, 4.262112f },
	{ 3690.084229f, -5066.993164f, 142.705917f, 5.245427f },
	{ 3742.711670f, -5146.786133f, 142.964890f, 2.178441f },
	{ 3706.024902f, -5155.362793f, 142.655304f, 1.294868f },
	{ 3676.363281f, -5133.007324f, 142.806168f, 0.615499f },
	{ 3668.310303f, -5096.927246f, 142.307312f, 6.128994f }
};

// Kel'thuzad AI
// each ~10-20 sec new mob

#define CN_KELTHUZAD 15990

#define SFROSTBOLT 28478
#define MFROSTBOLT 28479
#define CHAINS_OF_KELTHUZAD 28410
#define DETONATE_MANA 27819
#define SHADOW_FISSURE 27810
#define FROST_BLAST 27808

#define KELTHUZAD_CHANNEL 29423

class KelthuzadAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(KelthuzadAI);
		SP_AI_Spell spells[7];
		bool m_spellcheck[7];
		bool FrozenWastes[7];
		bool Abominations[7];
		bool SoulWeavers[7];

		KelthuzadAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 6;

			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
				spells[i].casttime = 0;
			}

			for(int i = 0; i < 7; i++)
			{
				FrozenWastes[i] = false;
				Abominations[i] = false;
				SoulWeavers[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(SFROSTBOLT);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = false;
			spells[0].perctrigger = 7.0f;
			spells[0].cooldown = 0;
			spells[0].attackstoptimer = 2000;

			spells[1].info = dbcSpell.LookupEntry(MFROSTBOLT);
			spells[1].targettype = TARGET_VARIOUS;
			spells[1].instant = true;
			spells[1].perctrigger = 0.1f;
			spells[1].cooldown = 15;			// it's casted after 15 sec anyway, so it does need additional perctrigger
			spells[1].attackstoptimer = 1000;

			spells[2].info = dbcSpell.LookupEntry(CHAINS_OF_KELTHUZAD);
			spells[2].targettype = TARGET_RANDOM_SINGLE;
			spells[2].instant = true;
			spells[2].perctrigger = 5.0f;
			spells[2].cooldown = 20;
			spells[2].attackstoptimer = 1000;
			spells[2].mindist2cast = 0.0f;
			spells[2].maxdist2cast = 40.0f;
			spells[2].minhp2cast = 0;
			spells[2].maxhp2cast = 100;

			spells[3].info = dbcSpell.LookupEntry(DETONATE_MANA);
			spells[3].targettype = TARGET_RANDOM_SINGLE;
			spells[3].instant = true;
			spells[3].perctrigger = 6.0f;
			spells[3].cooldown = 7;
			spells[3].attackstoptimer = 2000;
			spells[3].mindist2cast = 0.0f;
			spells[3].maxdist2cast = 40.0f;
			spells[3].minhp2cast = 0;
			spells[3].maxhp2cast = 100;

			spells[4].info = dbcSpell.LookupEntry(SHADOW_FISSURE);
			spells[4].targettype = TARGET_ATTACKING;
			spells[4].instant = true;
			spells[4].perctrigger = 5.0f;
			spells[4].cooldown = 10;
			spells[4].attackstoptimer = 2000;

			spells[5].info = dbcSpell.LookupEntry(FROST_BLAST);
			spells[5].targettype = TARGET_RANDOM_SINGLE;
			spells[5].instant = true;
			spells[5].perctrigger = 6.0f;
			spells[5].cooldown = 10;
			spells[5].attackstoptimer = 2000;
			spells[5].soundid = 8815;
			spells[5].speech = "I shall freeze the blood in your veins!";	// not sure if it's to this one or to one of bolt spells
			spells[5].mindist2cast = 0.0f;
			spells[5].maxdist2cast = 40.0f;
			spells[5].minhp2cast = 0;
			spells[5].maxhp2cast = 100;

			spells[6].info = dbcSpell.LookupEntry(KELTHUZAD_CHANNEL);
			spells[6].targettype = TARGET_SELF;
			spells[6].instant = false;
			spells[6].perctrigger = 0.0f;
			spells[6].cooldown = 0;
			spells[6].attackstoptimer = 1000;

			_unit->GetAIInterface()->disable_melee = false;
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
			_unit->GetAIInterface()->m_canMove = true;

			DespawnTrash = false;
			EventStart = false;
			SpawnCounter = 0;
			PhaseTimer = 310;
			SpawnTimer = 0;
			GCounter = 0;
			m_phase = 0;
		}

		void OnCombatStart(Unit* mTarget)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Minions, servants, soldiers of the cold dark, obey the call of Kel'Thuzad!");
			_unit->PlaySoundToSet(8819);

			Unit* TheLichKing = NULL;
			TheLichKing = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(3767.58f, -5117.15f, 174.49f, CN_THE_LICH_KING);
			if(TheLichKing != NULL)
			{
				_unit->SetChannelSpellTargetGUID(TheLichKing->GetGUID());
				_unit->SetChannelSpellId(29423);
			}

			GameObject*  KelGate  = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(3635.44f, -5090.33f, 143.205f, 181228);

			if(KelGate)
				KelGate->SetState(1);

			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->m_canMove = false;

			RegisterAIUpdateEvent(1000);
			CastTime();

			DespawnTrash = false;
			EventStart = true;
			SpawnCounter = 0;
			PhaseTimer = 310;
			SpawnTimer = 0;
			GCounter = 0;
			m_phase = 1;
		}

		void OnCombatStop(Unit* mTarget)
		{
			GameObject*  KelGate  = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(3635.44f, -5090.33f, 143.205f, 181228);
			if(KelGate != NULL)
				KelGate->SetState(0);

			for(int i = 0; i < 4; i++)
			{
				GameObject*  WindowGate  = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(Guardians[i].x, Guardians[i].y, Guardians[i].z, 200002);
				if(WindowGate != NULL)
					WindowGate->SetState(1);
			}

			_unit->SetChannelSpellTargetGUID(0);
			_unit->SetChannelSpellId(0);
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			_unit->GetAIInterface()->disable_melee = false;
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
			_unit->GetAIInterface()->m_canMove = true;
			RemoveAIUpdateEvent();

			DespawnTrash = true;
			EventStart = false;
			SpawnCounter = 0;
			PhaseTimer = 310;
			SpawnTimer = 0;
			GCounter = 0;
			m_phase = 0;
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = 0;
		}

		void OnTargetDied(Unit* mTarget)
		{
			if(_unit->GetHealthPct() == 0) return;

			uint32 RandomSpeach = rand() % 2;
			switch(RandomSpeach)
			{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The dark void awaits you!");
					_unit->PlaySoundToSet(8817);
					break;
				case 1:
					//_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "");	// no text?
					_unit->PlaySoundToSet(8818);
					break;
			}
		}

		void OnDied(Unit* mKiller)
		{
			GameObject*  KelGate  = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(3635.44f, -5090.33f, 143.205f, 181228);
			if(KelGate != NULL)
				KelGate->SetState(0);

			for(int i = 0; i < 4; i++)
			{
				GameObject*  WindowGate  = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(Guardians[i].x, Guardians[i].y, Guardians[i].z, 200002);
				if(WindowGate != NULL)
					WindowGate->SetState(1);
			}

			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Do not rejoice... your victory is a hollow one... for I shall return with powers beyond your imagining!");
			_unit->PlaySoundToSet(8814);

			_unit->GetAIInterface()->disable_melee = false;
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
			_unit->GetAIInterface()->m_canMove = true;
			RemoveAIUpdateEvent();

			EventStart = false;
			SpawnCounter = 0;
			PhaseTimer = 310;
			SpawnTimer = 0;
			GCounter = 0;
			m_phase = 0;
		}

		void AIUpdate()
		{
			switch(m_phase)
			{
				case 1:
					PhaseOne();
					break;
				case 2:
					PhaseTwo();
					break;
				case 3:
					PhaseThree();
					break;
				default:
					{
					}
			}
		}

		void PhaseOne()
		{
			if(EventStart == true)
			{
				SpawnTimer++;
				if(SpawnTimer == 5 || SpawnTimer == 10)
				{
					for(int i = 0; i < 7; i++)
					{
						int Counter = 0;

						while(Counter == 0)
						{
							if(FrozenWastes[i] == false && (rand() % 3 == 0 || SpawnCounter > 0))
							{
								for(int x = 0; x < 10; x++)
								{
									uint32 SpawnID = 10 * i + x;
									_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_SOLDIER_OF_THE_FROZEN_WASTES, SFrozenWastes[SpawnID].x, SFrozenWastes[SpawnID].y, SFrozenWastes[SpawnID].z, SFrozenWastes[SpawnID].o, true, false, 0, 0);
								}

								FrozenWastes[i] = true;
								Counter++;
							}

							if(Abominations[i] == false && (rand() % 3 == 0 || SpawnCounter > 0))
							{
								for(int x = 0; x < 3; x++)
								{
									uint32 SpawnID = 3 * i + x;
									_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_UNSTOPPABLE_ABOMINATION, Abomination[SpawnID].x, Abomination[SpawnID].y, Abomination[SpawnID].z, Abomination[SpawnID].o, true, false, 0, 0);
								}

								Abominations[i] = true;
								Counter++;
							}

							if(SoulWeavers[i] == false && ((rand() % 3 == 0 && Counter < 2) || Counter == 0 || SpawnCounter > 0))
							{
								uint32 SpawnID = i;
								_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_SOUL_WEAVER, SoulWeaver[SpawnID].x, SoulWeaver[SpawnID].y, SoulWeaver[SpawnID].z, SoulWeaver[SpawnID].o, true, false, 0, 0);

								SoulWeavers[i] = true;
								Counter++;
							}

							if(SoulWeavers[i] == true && Abominations[i] == true && FrozenWastes[i] == true)
								Counter = 1;
						}
					}

					SpawnCounter++;
				}

				if(SpawnCounter == 2)
				{
					for(int i = 0; i < 7; i++)
					{
						FrozenWastes[i] = false;
						Abominations[i] = false;
						SoulWeavers[i] = false;
					}

					WaveTimer = rand() % 6 + 10;
					EventStart = false;
					SpawnCounter = 0;
					PhaseTimer = 310;
					SpawnTimer = 0;
				}
			}

			else
			{
				PhaseTimer--;
				WaveTimer--;

				if(PhaseTimer == 5)
				{
					uint32 RandomSpeach = rand() % 3;
					switch(RandomSpeach)
					{
						case 0:
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Pray for mercy!");
							_unit->PlaySoundToSet(8809);
							break;
						case 1:
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Scream your dying breath!");
							_unit->PlaySoundToSet(8810);
							break;
						case 2:
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The end is upon you!");
							_unit->PlaySoundToSet(8811);
							break;
					}
				}

				if(PhaseTimer == 3)
					DespawnTrash = true;

				if(!PhaseTimer)
				{
					_unit->SetChannelSpellTargetGUID(0);
					_unit->SetChannelSpellId(0);
					_unit->GetAIInterface()->disable_melee = false;
					_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
					_unit->GetAIInterface()->m_canMove = true;

					DespawnTrash = false;
					HelpDialog = 0;
					GCounter = 0;
					m_phase = 2;
					return;
				}

				else if(!WaveTimer && PhaseTimer > 5)
				{
					uint32 SpawnPoint = RandomUInt(7);
					uint32 RandomSU = 0;
					if(PhaseTimer > 250)
						RandomSU = RandomUInt(4);
					if(PhaseTimer <= 250 && PhaseTimer >= 150)
						RandomSU = RandomUInt(5);
					if(PhaseTimer <= 150 && PhaseTimer > 100)
						RandomSU = RandomUInt(6);
					if(PhaseTimer <= 100)
						RandomSU = RandomUInt(7);

					uint32 UnitType;

					switch(RandomSU)
					{
						case 0:
						case 1:
							UnitType = CN_SOLDIER_OF_THE_FROZEN_WASTES;
							break;
						case 2:
						case 4:
							UnitType = CN_SOUL_WEAVER;
							break;
						case 3:
						case 5:
						case 6:
							UnitType = CN_UNSTOPPABLE_ABOMINATION;
							break;
						default:
							{
								UnitType = CN_UNSTOPPABLE_ABOMINATION;
							}
					}

					_unit->GetMapMgr()->GetInterface()->SpawnCreature(UnitType, Waves[SpawnPoint].x, Waves[SpawnPoint].y, Waves[SpawnPoint].z, Waves[SpawnPoint].o, true, false, 0, 0);
					WaveTimer = rand() % 11 + 10;
				}
			}
		}

		void PhaseTwo()
		{
			if(_unit->GetHealthPct() <= 40)
			{
				HelpDialog++;
				if(HelpDialog == 1)
				{
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Master! I require aid!");
					_unit->PlaySoundToSet(8816);
				}

				if(HelpDialog == 4)
				{
					Unit* TheLichKing = NULL;
					TheLichKing = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(3767.58f, -5117.15f, 174.49f, CN_THE_LICH_KING);
					if(TheLichKing != NULL)
					{
						TheLichKing->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Very well... warriors of the frozen wastes, rise up, I command you to fight, kill, and die for your master. Let none survive...");
						TheLichKing->PlaySoundToSet(8824);
					}

					else
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Very well... warriors of the frozen wastes, rise up, I command you to fight, kill, and die for your master. Let none survive...");
						_unit->PlaySoundToSet(8824);
					}

					for(int i = 0; i < 4; i++)
					{
						GameObject*  WindowGate  = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(Guardians[i].x, Guardians[i].y, Guardians[i].z, 200002);
						if(WindowGate)
							WindowGate->SetState(0);
					}
				}

				if(HelpDialog == 10 || HelpDialog == 12 || HelpDialog == 14 || HelpDialog == 16 || HelpDialog == 18)
				{
					Unit* Guardian = NULL;
					uint32 i = RandomUInt(4);
					Guardian = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_GUARDIAN_OF_ICECROWN, Guardians[i].x, Guardians[i].y, Guardians[i].z, Guardians[i].o, true, false, 0, 0);
					if(Guardian  != NULL)
					{
						if(Guardian->GetAIInterface()->getNextTarget() != NULL)
							Guardian->GetAIInterface()->AttackReaction(Guardian->GetAIInterface()->getNextTarget(), 1, 0);
					}

					GCounter++;
					if(GCounter == 5)
					{
						GCounter = 0;
						m_phase = 3;
					}
				}
			}

			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void PhaseThree()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					if(!spells[i].perctrigger) continue;

					if(m_spellcheck[i])
					{
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
							case TARGET_RANDOM_FRIEND:
							case TARGET_RANDOM_SINGLE:
							case TARGET_RANDOM_DESTINATION:
								CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast);
								break;
						}

						if(i == 2)
							ChainSound();

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
						}

						m_spellcheck[i] = false;
						return;
					}

					uint32 t = (uint32)time(NULL);
					if(((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || i == 1) && t > spells[i].casttime)
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						spells[i].casttime = t + spells[i].cooldown;
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

		void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
		{
			if(!maxdist2cast) maxdist2cast = 100.0f;
			if(!maxhp2cast) maxhp2cast = 100;

			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
				/* If anyone wants to use this function, then leave this note!										 */
				for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr)
				{
					if(((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && (*itr)->IsUnit())  // isAttackable(_unit, (*itr)) &&
					{
						Unit* RandomTarget = NULL;
						RandomTarget = TO_UNIT(*itr);

						if(RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast * mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast * maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
						{
							TargetTable.push_back(RandomTarget);
						}
					}
				}

				if(_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
					TargetTable.push_back(_unit);

				if(!TargetTable.size())
					return;

				size_t RandTarget = rand() % TargetTable.size();

				Unit*  RTarget = TargetTable[RandTarget];

				if(!RTarget)
					return;

				switch(spells[i].targettype)
				{
					case TARGET_RANDOM_FRIEND:
					case TARGET_RANDOM_SINGLE:
						_unit->CastSpell(RTarget, spells[i].info, spells[i].instant);
						break;
					case TARGET_RANDOM_DESTINATION:
						_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant);
						break;
				}

				TargetTable.clear();
			}
		}

		void ChainSound()
		{
			uint32 RandomSpeach = rand() % 2;
			switch(RandomSpeach)
			{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your soul is bound to me now!");
					_unit->PlaySoundToSet(8812);
					break;
				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "There will be no escape!");
					_unit->PlaySoundToSet(8813);
					break;
			}
		}

		bool GetDespawnTrash() { return DespawnTrash; }

	protected:

		bool DespawnTrash, EventStart;
		uint32 SpawnCounter;
		uint32 HelpDialog;
		uint32 SpawnTimer;
		uint32 PhaseTimer;
		uint32 WaveTimer;
		uint32 GCounter;
		uint32 m_phase;
		int nrspells;
};

// The Lich KingAI

class TheLichKingAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(TheLichKingAI);

		TheLichKingAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		}
};

// Soldier of the Frozen WastesAI

#define DARK_BLAST 28457 // 28458

class SoldierOfTheFrozenWastesAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(SoldierOfTheFrozenWastesAI);
		SoldierOfTheFrozenWastesAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			_unit->m_noRespawn = true;

			OnStart = false;

			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void OnCombatStart(Unit* mTarget)
		{
			LastPosX = _unit->GetPositionX();
			LastPosY = _unit->GetPositionY();
			LastPosZ = _unit->GetPositionZ();

			//RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);

			//RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			Creature* Kelthuzad = NULL;
			Kelthuzad = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(3749.950195f, -5113.451660f, 141.288635f, 15990);
			if(Kelthuzad == NULL || !Kelthuzad->isAlive())
			{
				_unit->Despawn(0, 0);
				return;
			}
			else if(Kelthuzad->GetScript())
			{
				CreatureAIScript* pScript = Kelthuzad->GetScript();
				if(TO< KelthuzadAI* >(pScript)->GetDespawnTrash())
				{
					_unit->Despawn(0, 0);
					return;
				}
			}
			if(_unit->GetPositionX() == LastPosX && _unit->GetPositionY() == LastPosY && _unit->GetPositionZ() == LastPosZ)
			{
				_unit->GetAIInterface()->MoveTo(newposx, newposy, 141.290451f, _unit->GetOrientation());
			}
			if(OnStart == false)
			{
				for(int i = 0; i < 7; i++)
				{
					if(_unit->GetPositionX() == Waves[i].x && _unit->GetPositionY() == Waves[i].y && _unit->GetPositionZ() == Waves[i].z)
					{
						float xchange  = RandomFloat(10.0f);
						float distance = 10.0f;

						float ychange = sqrt(distance * distance - xchange * xchange);

						if(rand() % 2 == 1)
							xchange *= -1;
						if(rand() % 2 == 1)
							ychange *= -1;

						newposx = 3715.845703f + xchange;
						newposy = -5106.928223f + ychange;

						_unit->GetAIInterface()->MoveTo(newposx, newposy, 141.290451f, _unit->GetOrientation());
					}
				}

				OnStart = true;
			}

			if(_unit->GetAIInterface()->getNextTarget() != NULL)
			{
				Unit* target = _unit->GetAIInterface()->getNextTarget();
				if(_unit->GetDistance2dSq(target) <= 49.0f)
					_unit->CastSpell(_unit, DARK_BLAST, true);
			}
		}

	protected:

		float LastPosX, LastPosY, LastPosZ;
		float newposx;
		float newposy;
		bool OnStart;
};

// Unstoppable Abomination AI

#define UA_MORTAL_WOUND 25646	// 36814

class UnstoppableAbominationAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(UnstoppableAbominationAI);
		SP_AI_Spell spells[1];
		bool m_spellcheck[1];

		UnstoppableAbominationAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 1;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
				spells[i].casttime = 0;
			}

			spells[0].info = dbcSpell.LookupEntry(UA_MORTAL_WOUND);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].cooldown = 10;
			spells[0].perctrigger = 15.0f;
			spells[0].attackstoptimer = 1000;

			_unit->m_noRespawn = true;

			OnStart = false;

			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void OnCombatStart(Unit* mTarget)
		{
			LastPosX = _unit->GetPositionX();
			LastPosY = _unit->GetPositionY();
			LastPosZ = _unit->GetPositionZ();

			//RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));

			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = 0;
		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);

			//RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			Creature* Kelthuzad = NULL;
			Kelthuzad = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(3749.950195f, -5113.451660f, 141.288635f, 15990);
			if(Kelthuzad == NULL || !Kelthuzad->isAlive())
			{
				_unit->Despawn(0, 0);
				return;
			}
			else if(Kelthuzad->GetScript())
			{
				CreatureAIScript* pScript = Kelthuzad->GetScript();
				if(TO< KelthuzadAI* >(pScript)->GetDespawnTrash())
				{
					_unit->Despawn(0, 0);
					return;
				}
			}
			if(_unit->GetPositionX() == LastPosX && _unit->GetPositionY() == LastPosY && _unit->GetPositionZ() == LastPosZ)
			{
				_unit->GetAIInterface()->MoveTo(newposx, newposy, 141.290451f, _unit->GetOrientation());
			}
			if(OnStart == false)
			{
				for(int i = 0; i < 7; i++)
				{
					if(_unit->GetPositionX() == Waves[i].x && _unit->GetPositionY() == Waves[i].y && _unit->GetPositionZ() == Waves[i].z)
					{
						float xchange  = RandomFloat(10.0f);
						float distance = 10.0f;

						float ychange = sqrt(distance * distance - xchange * xchange);

						if(rand() % 2 == 1)
							xchange *= -1;
						if(rand() % 2 == 1)
							ychange *= -1;

						newposx = 3715.845703f + xchange;
						newposy = -5106.928223f + ychange;

						_unit->GetAIInterface()->MoveTo(newposx, newposy, 141.290451f, _unit->GetOrientation());
					}
				}

				OnStart = true;
			}

			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					if(!spells[i].perctrigger) continue;

					if(m_spellcheck[i])
					{
						target = _unit->GetAIInterface()->getNextTarget();

						if(i == 0 && _unit->GetDistance2dSq(target) > 25.0f) return;

						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}
						m_spellcheck[i] = false;
						return;
					}

					uint32 t = (uint32)time(NULL);
					if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						spells[i].casttime = t + spells[i].cooldown;
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		float LastPosX, LastPosY, LastPosZ;
		float newposx;
		float newposy;
		bool OnStart;
		int nrspells;
};

// Soul Weaver AI

#define WAIL_OF_SOULS 28459

class SoulWeaverAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(SoulWeaverAI);
		SP_AI_Spell spells[1];
		bool m_spellcheck[1];

		SoulWeaverAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 1;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
				spells[i].casttime = 0;
			}

			spells[0].info = dbcSpell.LookupEntry(WAIL_OF_SOULS);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = true;
			spells[0].cooldown = 10;
			spells[0].perctrigger = 15.0f;
			spells[0].attackstoptimer = 1000;

			_unit->m_noRespawn = true;

			OnStart = false;

			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void OnCombatStart(Unit* mTarget)
		{
			LastPosX = _unit->GetPositionX();
			LastPosY = _unit->GetPositionY();
			LastPosZ = _unit->GetPositionZ();

			//RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));

			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = 0;
		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);

			//RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			Creature* Kelthuzad = NULL;
			Kelthuzad = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(3749.950195f, -5113.451660f, 141.288635f, 15990);
			if(Kelthuzad == NULL || !Kelthuzad->isAlive())
			{
				_unit->Despawn(0, 0);
				return;
			}
			else if(Kelthuzad->GetScript())
			{
				CreatureAIScript* pScript = Kelthuzad->GetScript();
				if(TO< KelthuzadAI* >(pScript)->GetDespawnTrash())
				{
					_unit->Despawn(0, 0);
					return;
				}
			}
			if(_unit->GetPositionX() == LastPosX && _unit->GetPositionY() == LastPosY && _unit->GetPositionZ() == LastPosZ)
			{
				_unit->GetAIInterface()->MoveTo(newposx, newposy, 141.290451f, _unit->GetOrientation());
			}
			if(OnStart == false)
			{
				for(int i = 0; i < 7; i++)
				{
					if(_unit->GetPositionX() == Waves[i].x && _unit->GetPositionY() == Waves[i].y && _unit->GetPositionZ() == Waves[i].z)
					{
						float xchange  = RandomFloat(10.0f);
						float distance = 10.0f;

						float ychange = sqrt(distance * distance - xchange * xchange);

						if(rand() % 2 == 1)
							xchange *= -1;
						if(rand() % 2 == 1)
							ychange *= -1;

						newposx = 3715.845703f + xchange;
						newposy = -5106.928223f + ychange;

						_unit->GetAIInterface()->MoveTo(newposx, newposy, 141.290451f, _unit->GetOrientation());
					}
				}

				OnStart = true;
			}

			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					if(!spells[i].perctrigger) continue;

					if(m_spellcheck[i])
					{
						target = _unit->GetAIInterface()->getNextTarget();

						if(i == 0 && _unit->GetDistance2dSq(target) > 64.0f) return;	// 8yards

						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}
						m_spellcheck[i] = false;
						return;
					}

					uint32 t = (uint32)time(NULL);
					if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						spells[i].casttime = t + spells[i].cooldown;
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		float LastPosX, LastPosY, LastPosZ;
		float newposx;
		float newposy;
		bool OnStart;
		int nrspells;
};

// Guardian of Icecrown AI

#define BLOOD_TAP 28459

class GuardianOfIcecrownAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(GuardianOfIcecrownAI);
		SP_AI_Spell spells[1];
		bool m_spellcheck[1];

		GuardianOfIcecrownAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 1;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(BLOOD_TAP);
			spells[0].targettype = TARGET_SELF;
			spells[0].instant = true;
			spells[0].cooldown = 0;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000;

			_unit->GetAIInterface()->SetRun();
			_unit->m_noRespawn = true;

			OnStart = false;

			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void OnCombatStart(Unit* mTarget)
		{
			if(_unit->GetAIInterface()->getNextTarget())
				LastTarget = _unit->GetAIInterface()->getNextTarget();

			LastPosX = _unit->GetPositionX();
			LastPosY = _unit->GetPositionY();
			LastPosZ = _unit->GetPositionZ();

			//RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);

			//RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			Unit* Kelthuzad = NULL;
			Kelthuzad = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(3715.950195f, -5106.451660f, 141.288635f, 15990);
			if(Kelthuzad && !Kelthuzad->isAlive())
			{
				_unit->Despawn(0, 0);
				return;
			}
			if(_unit->GetPositionX() == LastPosX && _unit->GetPositionY() == LastPosY && _unit->GetPositionZ() == LastPosZ)
			{
				_unit->GetAIInterface()->MoveTo(newposx, newposy, 141.290451f, _unit->GetOrientation());
			}
			if(OnStart == false)
			{
				for(int i = 0; i < 4; i++)
				{
					if(_unit->GetPositionX() == Guardians[i].x && _unit->GetPositionY() == Guardians[i].y && _unit->GetPositionZ() == Guardians[i].z)
					{
						float xchange  = RandomFloat(10.0f);
						float distance = 10.0f;

						float ychange = sqrt(distance * distance - xchange * xchange);

						if(rand() % 2 == 1)
							xchange *= -1;
						if(rand() % 2 == 1)
							ychange *= -1;

						newposx = 3715.845703f + xchange;
						newposy = -5106.928223f + ychange;

						_unit->GetAIInterface()->MoveTo(newposx, newposy, 141.290451f, _unit->GetOrientation());
					}
				}

				OnStart = true;
			}

			if(_unit->GetAIInterface()->getNextTarget())
			{
				Unit* target = NULL;
				target = _unit->GetAIInterface()->getNextTarget();

				if(!LastTarget) { LastTarget = target; return; }

				if(LastTarget != target)
					_unit->CastSpell(_unit, spells[0].info, spells[0].instant);

				LastTarget = target;
			}
		}

	protected:

		float LastPosX, LastPosY, LastPosZ;
		Unit* LastTarget;
		float newposx;
		float newposy;
		bool OnStart;
		int nrspells;
};

void SetupNaxxramas(ScriptMgr* pScriptMgr);
