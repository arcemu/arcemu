/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
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

#ifdef USE_SPECIFIC_AIAGENTS

#ifndef MAX_INT
	#define MAX_INT ( 1 << 30 )
#endif 
#ifndef PI
	#define PI ( 3.14f )	//yeah i know this is low precision
#endif 

#define MAX(a,b) (a>b?a:b)

#define BOREDOM_TIMER_TO_START_TRIGGERING	10*60*1000
#define BOREDOM_TRIGGER_INTERVAL			2*60*1000

#define BOREDOM_TEXTCOUNT	49
char	bored_texts[BOREDOM_TEXTCOUNT][200]=
{
	"I can't heal boredom",
	"Even your natural healing is better then me right now",
	"Let's to kick some but ",
	"I could do this with my eyes closed",
	"You know anyone who wants to buy my full mana ?",
	"I could even tank this imaginary enemy",
	"Scared maybe ?",
	"Don't worry, be happy",
	"Stay a while and listen....but not this much",
	"Did that rock got smaller or my eyes aged since we are standing here ?",

	"Look an enemy ... and we are going to wrong way ",
	"I counted to 10000...10 times and nothing happened",
	"There is a good part in doing nothing...just can't remmeber of it",
	"If you do nothing, nothing can go wrong",
	"Look what i can do....",
	"If i change into a sexy nurse maybe we will ge more action",
	"If stare at the sun a whole day you will see your future",
	"Can i take a vacation now ?",
	"If i would be payed i would be happy to earn money for free",
	"BRB, playing solitare",

	"Wooot, a new boredom score record",
	"Did something happen cause i felt asleep for a second ?",
	"I'll go peal a banana tree, will be back some other age",
	"Book of boredom : how to kill time. We found someone that ressed it it seems ",
	"Robots are so lucky. They can shutdown to make time pass faster",
	"I don't like acting dumb i'm dumb naturaly",
	"People tell me I don't listen...or something like that",
	"My life is based on a true story",
	"Rehab is for quitters!",
	"Life is never boring, but some people choose to be bored.",

	"If you think nobody cares you're alive, try not filing your taxes",
	"Hard work never killed anybody, but why take a chance?",
	"If Tylenol, duct tape, or a band-aid can't fix it, you've got a serious problem",
	"Always forgive your enemies. Nothing annoys them more",
	"By the time you have finished reading this, you will have realized you just wasted 10 seconds of your life.",
	"Alpha, beta, gamma, delta....epsilon...damn it, what comes next?!",
	"I pretend to work, and You'll pretend to pay me",
	"f**k the fast food, f**k the pepsi, all what we need is milk and strudel.",
	"Love is like hair; it can be found in the strangest places.",
	"Moo. I'm a pig",

	"Actually no, I will not STFU",
	"When I'm feeling down, I like to whistle. It makes the neighbor's dog run to the end of his chain and gag himself.",
	"Follow your dream! Unless it's the one where you're at work in your underwear during a fire drill",
	"Do I have hippotomonstrosesquippedaliophobia?",
	"Bull**** makes the flowers grow and that's beautiful!",
	"LET DO SOMETHING DARING...LETS ALL EAT FROZEN YORGERT!!!",
	"Squirrels juggling knives in my head",
	"Only dead fish go with the flow.",
	"",
};

#define BOREDOM_EMOTECOUNT	21
int bored_emotes[BOREDOM_TEXTCOUNT]=
{
	EMOTE_ONESHOT_TALK,
	EMOTE_ONESHOT_WAVE,
	EMOTE_ONESHOT_CHEER,
	EMOTE_STATE_DANCE,
	EMOTE_STATE_DANCE,
	EMOTE_STATE_DANCE,
	EMOTE_STATE_DANCE,
	EMOTE_STATE_SLEEP,
	EMOTE_STATE_SLEEP,
	EMOTE_STATE_SLEEP,

	EMOTE_STATE_SLEEP,
	EMOTE_ONESHOT_CHICKEN,
	EMOTE_ONESHOT_FLEX,
	EMOTE_STATE_DEAD,
	EMOTE_STATE_TALK,
	EMOTE_STATE_WHIRLWIND,
	EMOTE_ONESHOT_STOMP,
	EMOTE_STATE_ROAR,
	EMOTE_STATE_DANCESPECIAL,
	EMOTE_ONESHOT_DANCESPECIAL,

	EMOTE_STATE_EXCLAIM,
};

//will get bigger if it is better to cast this spell then some other version
float GetSpellEficiencyFactor(SpellEntry *sp)
{
	SpellCastTime *sd = dbcSpellCastTime.LookupEntry( sp->CastingTimeIndex );
	return (float)(sp->EffectBasePoints[0]) / ( GetCastTime( sd ) * 2 + sp->manaCost + 1) ;
}

void AiAgentHealSupport::Init(Unit *un, AIType at, MovementType mt, Unit *owner)
{
	AIInterface::Init(un,at,mt,owner);//run the original init and we add our custom stuff too

	m_Unit->SetUInt32Value( UNIT_FIELD_BASE_HEALTH , m_Unit->GetUInt32Value( UNIT_FIELD_HEALTH ));
	m_Unit->SetUInt32Value( UNIT_FIELD_BASE_MANA , m_Unit->GetUInt32Value( UNIT_FIELD_POWER1 ));

	m_fallowAngle = PI - PI / 6;
	FollowDistance = 5 ; //we are support, we stand behind our master 

	DifficultyLevel = 1;

	//scale health and mana
	m_Unit->SetUInt32Value( UNIT_FIELD_MAXHEALTH , (uint32)(m_Unit->GetUInt32Value( UNIT_FIELD_BASE_HEALTH ) * DifficultyLevel * CREATURE_STATS_SCALE_WITH_DIFFICULTY) );
	m_Unit->SetUInt32Value( UNIT_FIELD_MAXPOWER1 , (uint32)(m_Unit->GetUInt32Value( UNIT_FIELD_BASE_MANA ) * DifficultyLevel * CREATURE_STATS_SCALE_WITH_DIFFICULTY) );
	m_Unit->SetUInt32Value( UNIT_FIELD_HEALTH , m_Unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
	m_Unit->SetUInt32Value( UNIT_FIELD_POWER1 , m_Unit->GetUInt32Value( UNIT_FIELD_MAXPOWER1 ) );

	uint8 race = m_PetOwner->getRace();

	if( race == RACE_HUMAN || race == RACE_DWARF || race == RACE_NIGHTELF || race == RACE_GNOME || race == RACE_DRAENEI )
		Owner_side = OWNER_SIDE_ALIANCE;
	else
		Owner_side = OWNER_SIDE_HORDE;

	last_time_full_health = true; //before we start healing we should try to quick augment our master

	SetNextTarget( m_Unit );

	m_defend_self.sp = dbcSpell.LookupEntry( 642 ); // Divine Shield 1
	m_defend_self.chance_to_cast = 75;
	revive_spell.sp = dbcSpell.LookupEntry( 2006 ); // resurrection 1
	revive_spell.chance_to_cast = 100;

	healagentspell *the;

	the = new healagentspell;
	the->chance_to_cast = 5;
	the->sp = dbcSpell.LookupEntry( 596 );// Prayer of Healing
	the->max_scale = 2000;
	m_healspells.push_back( the );

	the = new healagentspell;
	the->chance_to_cast = 5;
	the->sp = dbcSpell.LookupEntry( 1064 );// Chain Heal
	the->max_scale = 1500;
	m_healspells.push_back( the );

	the = new healagentspell;
	the->chance_to_cast = 2;
	the->sp = dbcSpell.LookupEntry( 5607 );// Healing Aura
	the->max_scale = 1000;
	m_healspells.push_back( the );

	the = new healagentspell;
	the->chance_to_cast = 100;
	the->sp = dbcSpell.LookupEntry( 139 );// Renew
	the->max_scale = 1700;
	m_healspells.push_back( the );

	the = new healagentspell;
	the->chance_to_cast = 30;
	the->sp = dbcSpell.LookupEntry( 27169 );// Blessing of Sanctuary
	the->max_scale = 200;
	m_healspells.push_back( the );

	the = new healagentspell;
	the->chance_to_cast = 100;
	the->sp = dbcSpell.LookupEntry( 2060 );// Greater Heal
	the->max_scale = 3500;
	m_healspells.push_back( the );

	the = new healagentspell;
	the->chance_to_cast = 5;
	the->sp = dbcSpell.LookupEntry( 26565 );// Heal Brethren
	the->max_scale = 0;
	m_healspells.push_back( the );

	the = new healagentspell;
	the->chance_to_cast = 5;
	the->sp = dbcSpell.LookupEntry( 36347 );// Healing Power
	the->max_scale = 0;
	m_AugmentSelf.push_back( the );

	the = new healagentspell;
	the->chance_to_cast = 5;
	the->sp = dbcSpell.LookupEntry( 32200 );// Increased Healing
	the->max_scale = 0;
	m_AugmentSelf.push_back( the );

	Boredom_cooldown = 0;
}

bool AiAgentHealSupport::CheckCanCast(SpellEntry *sp, Unit *target)
{
	if ( !sp )
		return false;

	if ( !target )
		return false;

	if( m_Unit->GetUInt32Value(UNIT_FIELD_POWER1) < sp->manaCost )
		return false; //we do not have enough juice

	if( spell_cooldown_map[ sp->Id ] >= getMSTime() )
		return false;

	return true;
}

void AiAgentHealSupport::_UpdateCombat(uint32 p_time)
{
	m_nextTarget = m_PetOwner;

	if( !m_nextTarget )
		return; //oh noez, our master has abandoned us ! Where is te luv ?

	if( m_Unit->isCasting() )
		return; // we are already supporting someone ...get back later

	//we should be at same level at owner so we profit of fighting formulas same as owner
	if(	m_Unit->GetUInt32Value( UNIT_FIELD_LEVEL ) != m_PetOwner->GetUInt32Value( UNIT_FIELD_LEVEL ) )
		m_Unit->SetUInt32Value( UNIT_FIELD_LEVEL, m_PetOwner->GetUInt32Value( UNIT_FIELD_LEVEL ) );

printf("we are not casting\n");
	float new_DifficultyLevel = DifficultyLevel;
	if( DifficultyPrevHealth > m_nextTarget->GetUInt32Value( UNIT_FIELD_HEALTH ) )
		new_DifficultyLevel += DIFFICULTY_UPDATE_SPEED;
	else if( DifficultyPrevHealth < m_nextTarget->GetUInt32Value( UNIT_FIELD_HEALTH ) )
		new_DifficultyLevel -= DIFFICULTY_UPDATE_SPEED;
	DifficultyPrevHealth = m_nextTarget->GetUInt32Value( UNIT_FIELD_HEALTH );

	if( new_DifficultyLevel != new_DifficultyLevel)
	{
		DifficultyLevel = new_DifficultyLevel;
		//scale health and mana.when we level we max out our stats
		m_Unit->SetUInt32Value( UNIT_FIELD_MAXHEALTH , (uint32)(m_Unit->GetUInt32Value( UNIT_FIELD_BASE_HEALTH ) * new_DifficultyLevel * CREATURE_STATS_SCALE_WITH_DIFFICULTY) );
		m_Unit->SetUInt32Value( UNIT_FIELD_MAXPOWER1 , (uint32)(m_Unit->GetUInt32Value( UNIT_FIELD_BASE_MANA ) * new_DifficultyLevel * CREATURE_STATS_SCALE_WITH_DIFFICULTY) );
		m_Unit->SetUInt32Value( UNIT_FIELD_HEALTH , (uint32)(m_Unit->GetUInt32Value( UNIT_FIELD_HEALTH ) * new_DifficultyLevel * CREATURE_STATS_SCALE_WITH_DIFFICULTY) );
		m_Unit->SetUInt32Value( UNIT_FIELD_POWER1 , (uint32)(m_Unit->GetUInt32Value( UNIT_FIELD_POWER1 ) * new_DifficultyLevel * CREATURE_STATS_SCALE_WITH_DIFFICULTY) );
	}

	uint32 Time_Now = getMSTime();

	std::list<healagentspell*>::iterator itr;
	SpellCastTargets targets;
	healagentspell *m_castingSpell = NULL;

	//poor thing died. Res him. Probably will never work
	if( !m_nextTarget->isAlive() && CheckCanCast( revive_spell.sp, m_nextTarget ) )
	{
		m_castingSpell = &revive_spell;
		m_Unit->CastSpell( m_PetOwner, m_castingSpell->sp, false);
printf("master died, we are going to resurect him\n");
	}

	//if we are injusred we should try to survive it
	if ( m_castingSpell== NULL && m_Unit->GetUInt32Value( UNIT_FIELD_HEALTH ) < m_Unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) )
	{
		if(	!Protect_self() ) //first we try to escape combat
		{
printf("we are injured, will try to heal us\n");
			for(itr=m_healspells.begin();itr!=m_healspells.end();itr++)
				if( CheckCanCast( (*itr)->sp, m_nextTarget ) && Rand( (*itr)->chance_to_cast ) )
				{
					m_castingSpell = *itr;
					m_Unit->CastSpell( m_PetOwner, m_castingSpell->sp, false);
					break;
				}
		}
		else
		{
			m_castingSpell = &m_defend_self;
			m_Unit->CastSpell( m_Unit, m_castingSpell->sp, false);
		}
	}

	//select an augment spell if we have nothing better to do
	if( m_castingSpell== NULL && m_nextTarget->GetUInt32Value( UNIT_FIELD_HEALTH ) == m_nextTarget->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) )
	{
		for(itr=m_AugmentSelf.begin();itr!=m_AugmentSelf.end();itr++)
			if( CheckCanCast( (*itr)->sp, m_nextTarget ) && Rand( (*itr)->chance_to_cast ) )
			{
				m_castingSpell = *itr;
				m_Unit->CastSpell( m_Unit, m_castingSpell->sp, false);
				break;
			}
		//try augment owner ?
		if( !m_castingSpell )
		{
			for(itr=m_AugmentTarget.begin();itr!=m_AugmentTarget.end();itr++)
				if( CheckCanCast( (*itr)->sp, m_nextTarget ) && Rand( (*itr)->chance_to_cast ) )
				{
					m_castingSpell = *itr;
					m_Unit->CastSpell( m_nextTarget, m_castingSpell->sp, false);
					break;
				}
		}
	}

	//master is injured, this should be most common case
	if( m_castingSpell==NULL && m_nextTarget->GetUInt32Value( UNIT_FIELD_HEALTH ) < m_nextTarget->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) )
	{
printf("master is injured, will try to heal him\n");
		for(itr=m_healspells.begin();itr!=m_healspells.end();itr++)
			if( CheckCanCast( (*itr)->sp, m_nextTarget ) && Rand( (*itr)->chance_to_cast ) )
			{
				m_castingSpell = *itr;
				m_Unit->CastSpell( m_nextTarget, m_castingSpell->sp, false);
				break;
			}

		//try to cast a group spell ?
		if ( m_castingSpell == NULL && m_PetOwner->IsPlayer() && static_cast< Player*>( m_PetOwner )->InGroup())
		{
			for(itr=m_healspells.begin();itr!=m_healspells.end();itr++)
				if( CheckCanCast( (*itr)->sp, m_nextTarget ) && Rand( (*itr)->chance_to_cast ) )
				{
					m_castingSpell = *itr;
					m_Unit->CastSpell( m_nextTarget, m_castingSpell->sp, false);
					break;
				}
		}
	}

	if( m_castingSpell )
	{
printf("we have a spell to cast\n");

		SpellCastTime *sd = dbcSpellCastTime.LookupEntry( m_castingSpell->sp->CastingTimeIndex );

		//do not stop for instant casts
		if(GetCastTime(sd) != 0)
		{
			StopMovement(0);
	printf("spell is not instant so we are going to stop movement \n");
		}

		float distance = m_Unit->GetDistanceSq(m_nextTarget);
		if(	distance <= m_castingSpell->sp->base_range_or_radius_sqr || m_castingSpell->sp->base_range_or_radius_sqr == 0 )
		{

	printf("we are in range and going to cast spell \n");
			m_AIState = STATE_CASTING;
			
			Spell *nspell = SpellPool.PooledNew();
			nspell->Init(m_Unit, m_castingSpell->sp, false, NULL);

#ifdef SPELL_EFF_PCT_SCALE_WITH_DIFFICULTY
			nspell->forced_basepoints[ 0 ] = (uint32)( m_castingSpell->sp->EffectBasePoints[0] * SPELL_EFF_PCT_SCALE_WITH_DIFFICULTY * DifficultyLevel );
			if( nspell->forced_basepoints[ 0 ] > m_castingSpell->max_scale )
				nspell->forced_basepoints[ 0 ] = m_castingSpell->max_scale;
			nspell->forced_basepoints[ 1 ] = (uint32)( m_castingSpell->sp->EffectBasePoints[1] * SPELL_EFF_PCT_SCALE_WITH_DIFFICULTY * DifficultyLevel );
			if( nspell->forced_basepoints[ 1 ] > m_castingSpell->max_scale )
				nspell->forced_basepoints[ 1 ] = m_castingSpell->max_scale;
			nspell->forced_basepoints[ 2 ] = (uint32)( m_castingSpell->sp->EffectBasePoints[2] * SPELL_EFF_PCT_SCALE_WITH_DIFFICULTY * DifficultyLevel );
			if( nspell->forced_basepoints[ 2 ] > m_castingSpell->max_scale )
				nspell->forced_basepoints[ 2 ] = m_castingSpell->max_scale;
#endif

			nspell->prepare( &targets );

			CastSpell( m_Unit, m_castingSpell->sp, targets );

			SetSpellDuration( m_castingSpell->sp );

		}
		else // Target out of Range -> Run to it
		{
	printf("we are going to move closer \n");
			m_moveRun = true;
			_CalcDestinationAndMove(m_nextTarget, sqrt( m_castingSpell->sp->base_range_or_radius_sqr ) );
		}
	}

// check if pets regenrate mana, If not then we should implement that too
	//if owner is mounted then we mount too. Speed is not set though
	if( m_PetOwner->GetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID ) && m_Unit->GetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID ) == 0 )
	{
		if( Owner_side == OWNER_SIDE_ALIANCE )
			m_Unit->SetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID, HELPER_MOUNT_A_DISPLAY );
		else
			m_Unit->SetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID, OWNER_SIDE_HORDE );
		m_moveSprint =  true;
	}
	else if( m_PetOwner->GetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID ) == 0 && m_Unit->GetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID ) != 0 )
	{
		m_Unit->SetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID, 0 );
		m_moveSprint = false;
	}

	//for fun : mimic master standstate. Note that this might give strange results
	if( m_PetOwner->GetStandState() != m_Unit->GetStandState() )
		m_Unit->SetStandState( m_PetOwner->GetStandState() );

	if( m_castingSpell == NULL && First_noaction_stamp == 0 )
		First_noaction_stamp = Time_Now;
	else First_noaction_stamp = 0;

	if( 
		First_noaction_stamp
		&& First_noaction_stamp + BOREDOM_TIMER_TO_START_TRIGGERING < Time_Now 
		&& Boredom_cooldown < Time_Now
		)
	{
		//some chance to say something
		if( Rand( 50 ) )
			m_Unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, bored_texts[ RandomUInt( BOREDOM_TEXTCOUNT ) ]);
		else
			m_Unit->SetUInt32Value ( UNIT_NPC_EMOTESTATE, bored_emotes[ RandomUInt( BOREDOM_EMOTECOUNT ) ] );
		Boredom_cooldown = Time_Now + BOREDOM_TRIGGER_INTERVAL;
	}
}

void AiAgentHealSupport::SetSpellDuration(SpellEntry *sp)
{
	if ( !sp )
		return ;

	uint32 Time_Now = getMSTime();
	uint32 cooldowntime;

#ifdef SPELL_COOLD_PCT_SCALE_WITH_DIFFICULTY
	cooldowntime = (uint32) (sp->RecoveryTime / ( SPELL_COOLD_PCT_SCALE_WITH_DIFFICULTY * DifficultyLevel ) );
#else
	cooldowntime = sp->RecoveryTime;
#endif

    if(sp->DurationIndex)
    {
		SpellDuration *sd=dbcSpellDuration.LookupEntry(sp->DurationIndex);
		int Dur=0;
		int Casttime=0;//most of the time 0
		int RecoveryTime=sp->RecoveryTime;
		if(sp->DurationIndex)
			Dur =::GetDuration(sd);
		Casttime=GetCastTime(dbcSpellCastTime.LookupEntry(sp->CastingTimeIndex));
		cooldowntime=Dur+Casttime+RecoveryTime;
		if(cooldowntime<0)
			cooldowntime=2000;//huge value that should not loop while adding some timestamp to it
	}

	if( cooldowntime )
		spell_cooldown_map[ sp->Id ] = Time_Now + cooldowntime;
}

bool AiAgentHealSupport::Protect_self()
{
	if ( CheckCanCast ( m_defend_self.sp , m_Unit ) && !m_Unit->HasAura( m_defend_self.sp->Id ) )
		return true;
	return false;
}

#endif