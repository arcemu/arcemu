/****************************************************************************
 *
 * SpellHandler Plugin
 * Copyright (c) 2007 Team Ascent
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter to Creative Commons,
 * 543 Howard Street, 5th Floor, San Francisco, California, 94105, USA.
 *
 * EXCEPT TO THE EXTENT REQUIRED BY APPLICABLE LAW, IN NO EVENT WILL LICENSOR BE LIABLE TO YOU
 * ON ANY LEGAL THEORY FOR ANY SPECIAL, INCIDENTAL, CONSEQUENTIAL, PUNITIVE OR EXEMPLARY DAMAGES
 * ARISING OUT OF THIS LICENSE OR THE USE OF THE WORK, EVEN IF LICENSOR HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 *
 */

#include "Setup.h"

/*
	How to add a new item spell to the dummy spell handler:

		1) Add a new function to handle the spell at the end of this file but before the
		   SetupItemSpells_1() function. SetupItemSpells_1() must always be the last function.

		2) Register the dummy spell by adding a new line to the end of the list in the
		   SetupItemSpells_1() function.

	Please look at how the other spells are handled and try to use the
	same variable names and formatting style in your new spell handler.
*/

// *****************************************************************************

bool GnomishTransporter(uint32 i, Spell * pSpell)
{
	if(!pSpell->p_caster) return true;

	pSpell->p_caster->EventAttackStop();
	pSpell->p_caster->SafeTeleport(1, 0, LocationVector(-7169.41f, -3838.63f, 8.72f));
	return true;
}

// -----------------------------------------------------------------------------

bool NoggenFoggerElixr(uint32 i, Spell * pSpell)
{
	if(!pSpell->p_caster) return true;

	uint32 chance = RandomUInt(2);

	switch(chance)
	{
	case 0:
		pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(16591), true);
		break;
	case 1:
		pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(16593), true);
		break;
	case 2:
		pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(16595), true);
		break;
	}
	return true;
}

// -----------------------------------------------------------------------------

bool HallowsEndCandy(uint32 i, Spell * pSpell)
{
	if(!pSpell->p_caster) return true;

	int newspell = 24924 + RandomUInt(3);

	SpellEntry *spInfo = dbcSpell.LookupEntryForced(newspell);
	if(!spInfo) return true;

	pSpell->p_caster->CastSpell(pSpell->p_caster, spInfo, true);
	return true;
}

// -----------------------------------------------------------------------------

bool DeviateFish(uint32 i, Spell * pSpell)
{
	if(!pSpell->p_caster) return true;

	int newspell = 8064 + RandomUInt(4);

	SpellEntry *spInfo = dbcSpell.LookupEntryForced(newspell);
	if(!spInfo) return true;

	pSpell->p_caster->CastSpell(pSpell->p_caster, spInfo, true);
	return true;
}

// -----------------------------------------------------------------------------

bool CookedDeviateFish(uint32 i, Spell * pSpell)
{
	if(!pSpell->p_caster) return true;

	int chance = 0;
	int newspell = 0;

	chance = RandomUInt(1);

	switch(chance)
	{
	case 0:
		newspell = 8219; // Flip Out (60 min) (turns you into a ninja)
		break;
	case 1:
		newspell = 8221; // Yaaarrrr (60 min) (turns you into a pirate)
		break;
	}

	if(newspell)
	{
		SpellEntry *spInfo = dbcSpell.LookupEntryForced(newspell);
		if(!spInfo) return true;

		pSpell->p_caster->CastSpell(pSpell->p_caster, spInfo, true);
	}
	return true;
}

// -----------------------------------------------------------------------------

bool HolidayCheer(uint32 i, Spell * pSpell)
{
	if(!pSpell->m_caster) return true;

	Unit * target;
	float dist = pSpell->GetRadius(i);

	for(ObjectSet::iterator itr = pSpell->m_caster->GetInRangeSetBegin(); itr != pSpell->m_caster->GetInRangeSetEnd(); ++itr)
	{
		if((*itr)->IsUnit())
			target = TO_UNIT(*itr);
		else
			continue;

		if(pSpell->m_caster->CalcDistance(target) > dist || isAttackable(pSpell->m_caster, target))
			continue;

		target->Emote(EMOTE_ONESHOT_LAUGH);
	}
	return true;
}

// -----------------------------------------------------------------------------

bool NetOMatic(uint32 i, Spell * pSpell)
{
	Unit * target = pSpell->GetUnitTarget();
	if(!pSpell->p_caster || !target) return true;

	SpellEntry *spInfo = dbcSpell.LookupEntryForced(13099);
	if(!spInfo) return true;

	int chance = RandomUInt(99)+1;

	if(chance < 51) // nets target: 50%
		pSpell->p_caster->CastSpell(target, spInfo, true);

	else if(chance < 76) // nets you instead: 25%
		pSpell->p_caster->CastSpell(pSpell->p_caster, spInfo, true);

	else // nets you and target: 25%
	{
		pSpell->p_caster->CastSpell(pSpell->p_caster, spInfo, true);
		pSpell->p_caster->CastSpell(target, spInfo, true);
	}
	return true;
}

// -----------------------------------------------------------------------------

bool BanishExile(uint32 i, Spell * pSpell)
{
	Unit *  target = pSpell->GetUnitTarget();
	if(!pSpell->p_caster || !target) return true;

	pSpell->p_caster->SpellNonMeleeDamageLog(target, pSpell->m_spellInfo->Id, target->GetUInt32Value(UNIT_FIELD_HEALTH), true);
	return true;
}

// -----------------------------------------------------------------------------

bool ForemansBlackjack(uint32 i, Spell *pSpell)
{
	Unit* target = pSpell->GetUnitTarget();
	if (!pSpell->p_caster || !target || !target->IsCreature() ) 
		return true;
	
	// check to see that we have the correct creature
	Creature* c_target = TO_CREATURE(target);
	if(c_target->GetEntry() != 10556 || !c_target->HasAura(17743)) 
		return true;

	// Start moving again
	if(target->GetAIInterface())
		target->GetAIInterface()->StopMovement(0);

	// Remove Zzz aura
	c_target->RemoveAllAuras();

	WorldPacket data(SMSG_PLAY_OBJECT_SOUND, 12);
	data << uint32( 6197 ) << c_target->GetGUID();
	pSpell->p_caster->SendMessageToSet(&data, true);

	// send chat message
	char msg[100];
	sprintf(msg, "Ow! Ok, I'll get back to work, %s", pSpell->p_caster->GetName());
	target->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, msg);

	// Increment the quest log
	sQuestMgr.OnPlayerKill( pSpell->p_caster, c_target, false );

	c_target->Emote( EMOTE_STATE_WORK_CHOPWOOD );

	// Add timed event to return lazy peon to Zzz after 5-10 minutes (spell 17743)
	SpellEntry * pSpellEntry = dbcSpell.LookupEntry(17743);
	sEventMgr.AddEvent( target ,&Unit::EventCastSpell , target , pSpellEntry , EVENT_UNK, 300000 + RandomUInt( 300000 ) , 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );

	return true;
}

// -----------------------------------------------------------------------------

bool NetherWraithBeacon(uint32 i, Spell *pSpell)
{
	if(!pSpell->p_caster) return true;

	float SSX = pSpell->p_caster->GetPositionX();
	float SSY = pSpell->p_caster->GetPositionY();
	float SSZ = pSpell->p_caster->GetPositionZ();
	float SSO = pSpell->p_caster->GetOrientation();

	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(22408, SSX, SSY, SSZ, SSO, true, false, 0, 0);
	return true;
}

// -----------------------------------------------------------------------------

bool NighInvulnBelt(uint32 i, Spell * pSpell)
{
	if(!pSpell->p_caster) return true;

	int chance = RandomUInt(99)+1;

	if(chance > 10) // Buff - Nigh-Invulnerability - 30456
		pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(30456), true);
	else // Malfunction - Complete Vulnerability - 30457
		pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(30457), true);

	return true;
}

// -----------------------------------------------------------------------------

bool ReindeerTransformation(uint32 i, Spell * pSpell)
{
	if(!pSpell->p_caster) return true;

	if(pSpell->p_caster->GetMount() != 0)
	{
		if(pSpell->p_caster->m_setflycheat)
			pSpell->p_caster->SetMount(22724);
		else
			pSpell->p_caster->SetMount(15902);
	}
	return true;
}

// -----------------------------------------------------------------------------

bool WinterWondervolt(uint32 i, Spell * pSpell)
{
	Unit *  target = pSpell->GetUnitTarget();

	if( target == NULL || !target->IsPlayer() )
		return true;

	uint32 outfitspells[] = {
		26157, // green male
		26272, // green female
		26273, // red male
		26274  // red female
	};

	uint32 team = target->GetTeam();
	uint32 gender = target->getGender();
	uint32 spellid = 0;

	if( team == TEAM_HORDE )
		spellid = outfitspells[ 2 + gender ];
	else
		spellid = outfitspells[ gender ];


	target->CastSpell( target, spellid, true );

	return true;
}

// -----------------------------------------------------------------------------

bool ScryingCrystal(uint32 i, Spell * pSpell)
{
	QuestLogEntry *en = pSpell->p_caster->GetQuestLogForEntry(9824);
	if(pSpell->p_caster->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(pSpell->p_caster->GetPositionX(), 
		pSpell->p_caster->GetPositionY(), pSpell->p_caster->GetPositionZ(), 300078) && en)
	{
		
		if(en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
		{
			en->SetMobCount(0, 1);
			en->SendUpdateAddKill(0);
			en->UpdatePlayerFields();
			return false;
		}
	}
	else if(pSpell->p_caster->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(pSpell->p_caster->GetPositionX(), 
		pSpell->p_caster->GetPositionY(), pSpell->p_caster->GetPositionZ(), 300142) && en)
	{
		if(en->GetMobCount(1) < en->GetQuest()->required_mobcount[1])
		{
			en->SetMobCount(1, 1);
			en->SendUpdateAddKill(1);
			en->UpdatePlayerFields();
			return false;
		}
	}
	return true;
}

bool MinionsOfGurok(uint32 i, Spell * pSpell)
{
	Unit *  target = pSpell->GetUnitTarget();
	if(!pSpell->p_caster || !target || !target->IsCreature() || target->GetEntry() != 17157) return true;

	TO_CREATURE(target)->Despawn(500, 360000);

	float SSX = target->GetPositionX();
	float SSY = target->GetPositionY();
	float SSZ = target->GetPositionZ();
	float SSO = target->GetOrientation();

	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(18181, SSX+rand()%8-4, SSY+rand()%8-4, SSZ, SSO, true, false, 0, 0);
	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(18181, SSX+rand()%8-4, SSY+rand()%8-4, SSZ, SSO, true, false, 0, 0);
	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(18181, SSX+rand()%8-4, SSY+rand()%8-4, SSZ, SSO, true, false, 0, 0);

	return true;
}

bool PurifyBoarMeat(uint32 i, Spell * pSpell)
{
	uint32 bormeat = RandomUInt(2);
	switch(bormeat)
	{
	case 0:
		{
			pSpell->p_caster->CastSpell(pSpell->p_caster, 29277, true);
		}break;
	case 1:
		{
			pSpell->p_caster->CastSpell(pSpell->p_caster, 29278, true);
		}break;
	}
	
	return true;
}

bool WarpRiftGenerator(uint32 i, Spell * pSpell)
{
	if(!pSpell->p_caster) return true;

	float SSX = pSpell->p_caster->GetPositionX();
	float SSY = pSpell->p_caster->GetPositionY();
	float SSZ = pSpell->p_caster->GetPositionZ();
	float SSO = pSpell->p_caster->GetOrientation();

	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(16939,SSX,SSY,SSZ,SSO,true,false,0,0);

	return true;
}

bool OrbOfTheSindorei(uint32 i, Aura * pAura, bool apply)
{
	Unit* target = pAura->GetTarget();
	if( !target->IsPlayer() )
		return true;
	if( apply )
	{
		uint32 spellid = 0;

		if( target->getGender() == 0 )
			spellid = 46355;
		else
			spellid = 46356;

		target->CastSpell( target, spellid, true );
	}

	return true;
}

bool BigBlizzardBear(uint32 i, Aura * pAura, bool apply)
{
	if( !pAura->GetTarget()->IsPlayer() )
		return true;

	if( apply )
	{
		uint32 newspell = 0;
		Player* pPlayer = TO_PLAYER( pAura->GetTarget() );

		if( pPlayer->_GetSkillLineCurrent( SKILL_RIDING, true ) >= 150 )
			newspell = 58999;
		else
			newspell = 58997;

		pAura->GetTarget()->CastSpell(pAura->GetTarget(), newspell, true);
	}

	return true;
}

bool WingedSteed(uint32 i, Aura * pAura, bool apply)
{
	if( !pAura->GetTarget()->IsPlayer() )
		return true;

	if( apply )
	{
		uint32 newspell = 0;
		Player* pPlayer = TO_PLAYER( pAura->GetTarget() );

		if( pPlayer->_GetSkillLineCurrent( SKILL_RIDING, true ) == 300 )
			newspell = 54727;
		else
			newspell = 54726;

		pAura->GetTarget()->CastSpell(pAura->GetTarget(), newspell, true);
	}

	return true;
}

bool HeadlessHorsemanMount(uint32 i, Aura * pAura, bool apply)
{
	if( !pAura->GetTarget()->IsPlayer() )
		return true;

	if( apply )
	{
		uint32 newspell = 0;
		Player* pPlayer = TO_PLAYER(pAura->GetTarget());
		AreaTable *pArea = dbcArea.LookupEntry(pPlayer->GetAreaID());

		if( pPlayer->_GetSkillLineCurrent( SKILL_RIDING, true ) >= 225 && 
			( ( pArea->AreaFlags & 1024 && pPlayer->GetMapId() != 571 ) ||
				( pArea->AreaFlags & 1024 && pPlayer->GetMapId() == 571 && pPlayer->HasSpellwithNameHash( SPELL_HASH_COLD_WEATHER_FLYING ) ) ) )
		{
			if( pPlayer->_GetSkillLineCurrent( SKILL_RIDING, true) == 300 )
				newspell = 48023;
			else
				newspell = 51617;
		}
	else if( pPlayer->_GetSkillLineCurrent( SKILL_RIDING, true ) >= 150 )
		newspell = 48024;			
	else
		newspell = 51621;

	pAura->GetTarget()->CastSpell(pAura->GetTarget(), newspell, true);
	}

	return true;
}

bool MagicBroomMount(uint32 i, Aura * pAura, bool apply)
{
	if( !pAura->GetTarget()->IsPlayer() )
		return true;

	if( apply )
	{
		uint32 newspell = 0;
		Player* pPlayer = TO_PLAYER( pAura->GetTarget() );
		AreaTable *pArea = dbcArea.LookupEntry(pPlayer->GetAreaID());

		if( pPlayer->_GetSkillLineCurrent( SKILL_RIDING, true) >= 225 &&
			( ( pArea->AreaFlags & 1024 && pPlayer->GetMapId() != 571 ) ||
				( pArea->AreaFlags & 1024 && pPlayer->GetMapId() == 571 && pPlayer->HasSpellwithNameHash( SPELL_HASH_COLD_WEATHER_FLYING ) ) ) )
	{
		if( pPlayer->_GetSkillLineCurrent( SKILL_RIDING, true) == 300 )
			newspell = 42668;
		else
			newspell = 42667;
	}
	else if( pPlayer->_GetSkillLineCurrent( SKILL_RIDING, true ) >= 150 )
		newspell = 42683;			
	else
		newspell = 42680;

	pAura->GetTarget()->CastSpell(pAura->GetTarget(),newspell,true);
	}

	return true;
}

bool MagicRoosterMount(uint32 i, Aura * pAura, bool apply)
{
	if( !pAura->GetTarget()->IsPlayer() )
		return true;

	if( apply )
	{
		pAura->GetTarget()->CastSpell(pAura->GetTarget(), 66122, true);
	}

	return true;
}

bool Invincible(uint32 i, Aura * pAura, bool apply)
{
	if( !pAura->GetTarget()->IsPlayer() )
		return true;

	if( apply )
	{
		uint32 newspell = 0;
		Player* pPlayer = TO_PLAYER( pAura->GetTarget() );
		AreaTable *pArea = dbcArea.LookupEntry(pPlayer->GetAreaID());

		if( pPlayer->_GetSkillLineCurrent( SKILL_RIDING, true) >= 225 &&
			( ( pArea->AreaFlags & 1024 && pPlayer->GetMapId() != 571 ) ||
				( pArea->AreaFlags & 1024 && pPlayer->GetMapId() == 571 && pPlayer->HasSpellwithNameHash( SPELL_HASH_COLD_WEATHER_FLYING ) ) ) )
		{
		if( pPlayer->_GetSkillLineCurrent( SKILL_RIDING, true) == 300 )
			newspell = 72284;
		else
			newspell = 72283;
		}

		else if( pPlayer->_GetSkillLineCurrent( SKILL_RIDING, true ) >= 150 )
			{
				newspell = 72282;
		}
			else
				newspell = 72281;

		pAura->GetTarget()->CastSpell(pAura->GetTarget(), newspell, true);
	}


	return true;
}

bool Poultryizer( uint32 i, Spell *s ){
	Unit *unitTarget = s->GetUnitTarget();

	if( !unitTarget || !unitTarget->isAlive())
		return false;
	
	s->u_caster->CastSpell(unitTarget->GetGUID(),30501,true);

	return true;
}

bool SixDemonBag( uint32 i, Spell *s ){
	Unit *unitTarget = s->GetUnitTarget();

	if( !unitTarget || !unitTarget->isAlive())
		return false;
	
	uint32 ClearSpellId[6] = { 8401,8408,930,118,1680,10159 };
	uint32 randid = RandomUInt( 5 );
	uint32 spelltocast = ClearSpellId[ randid ];

	s->u_caster->CastSpell( unitTarget,  spelltocast, true );

	return true;
}

bool ExtractGas( uint32 i, Spell *s ){
	bool check = false;
	uint32 cloudtype = 0;
	Creature *creature = 0;
	
	if(!s->p_caster)
		return false;
	
	for(Object::InRangeSet::iterator itr = s->p_caster->GetInRangeSetBegin(); itr != s->p_caster->GetInRangeSetEnd(); ++itr)
	{
		if((*itr)->IsCreature())
		{
			creature=static_cast<Creature *>((*itr));
			cloudtype=creature->GetEntry();
			
			if(cloudtype == 24222 || cloudtype == 17408 || cloudtype == 17407 || cloudtype == 17378)
			{
				if(s->p_caster->GetDistance2dSq((*itr)) < 400)
				{
					s->p_caster->SetSelection(creature->GetGUID());
					check = true;
					break;
				}
			}
		}
	}
	
	if( !check )
		return false;

	uint32 item = 0;
	uint32 count = 0;
	
	count = 3+(rand()%3);
	
	if (cloudtype==24222) item=22572;//-air
	if (cloudtype==17408) item=22576;//-mana
	if (cloudtype==17407) item=22577;//-shadow
	if (cloudtype==17378) item=22578;//-water
	
	if( item == 0 )
		return false;
	
	s->p_caster->GetItemInterface()->AddItemById( item, count, 0 );
	creature->Despawn(3500,creature->GetProto()->RespawnTime);

	return true;
}

bool BrittleArmor( uint32 i, Spell *s ){
	if( s->u_caster == NULL )
		return false;

	for(int j= 0;j<20;j++)
		s->u_caster->CastSpell( s->u_caster, 24575, true);

	return true;
}

bool RequiresNoAmmo( uint32 i, Aura *a, bool apply ){
	
	a->SpellAuraConsumeNoAmmo( apply );
	
	return true;
}

/////////////////////////////////////////////////////////////////////////
//Nitro Boosts dummy effect handler
//
//Precondition(s)
//  Casted by Player.
//  Engineering skill of at least 400 points.
//
//Effect(s)
//  Casts another spell that increases run speed for 5 seconds
//
////////////////////////////////////////////////////////////////////////
bool NitroBoosts( uint32 i, Spell *s ){
	if( s->p_caster == NULL )
		return true;

	uint32 engineeringskill = s->p_caster->_GetSkillLineCurrent( SKILL_ENGINEERING );

	if( engineeringskill >= 400 )
		s->p_caster->CastSpell( s->p_caster, 54861, true );

	return true;
}


///////////////////////////////////////////////////////////////////////
//Gnomish Shrink Ray dummy effect handler
//
//Preconditions
//  Casted by Player
//
//Effect
//  Normally casts spell 13010 on the targeted unit. Shrinking it, and
//  making it weaker
//
//  On malfunction it either shrinks, or enlarges, the the caster,
//  the caster's party, all enemies, or enlarges the target
//
//
//////////////////////////////////////////////////////////////////////
bool ShrinkRay( uint32 i, Spell *s ){
	if( s->p_caster == NULL )
		return true;

	uint32 spellids[] = {
		13004, // grow
		13010  // shrink
	};

	uint32 chance =  RandomUInt( 5 );
	bool malfunction = false;

	if( chance == 5 )
		malfunction = true;

	if( !malfunction  ){
		
		s->p_caster->CastSpell( s->GetUnitTarget(), spellids[ 1 ], true );

	}else{
		uint32 spellindex = RandomUInt( 1 );
		uint32 who = RandomUInt( 3 );
	
		switch( who ){

		case 0:{ // us
			
			s->p_caster->CastSpell( s->p_caster, spellids[ spellindex ], true );
			   
			   }break;

		case 1:{ // them
			
			// if it's a malfunction it will only grow the target, since shrinking is normal
			s->p_caster->CastSpell( s->GetUnitTarget(), spellids[ 0 ], true );

			   }break;

		case 2:{ // our party

			for( std::set< Object* >::iterator itr = s->p_caster->GetInRangePlayerSetBegin(); itr != s->p_caster->GetInRangePlayerSetEnd(); ++itr ){
				Player *p = TO_PLAYER( *itr );

				if( ( p->GetPhase() & s->p_caster->GetPhase() ) == 0 )
					continue;

				if( p->GetGroup()->GetID() != s->p_caster->GetGroup()->GetID() )
					continue;

				s->p_caster->CastSpell( p, spellids[ spellindex ], true );
			}

			   }break;

		case 3:{ // every attacking enemy
			
			for( std::set< Object* >::iterator itr = s->p_caster->GetInRangeOppFactsSetBegin(); itr != s->p_caster->GetInRangeOppFactsSetEnd(); ++itr ){
				Object *o = *itr;

				if( ( o->GetPhase() & s->p_caster->GetPhase() ) == 0 )
					continue;

				if( !o->IsCreature() )
					continue;

				Unit *u = TO_UNIT( o );

				if( u->GetTargetGUID() != s->p_caster->GetGUID() )
					continue;

				if( !isAttackable( s->p_caster, u ) )
					continue;

				s->p_caster->CastSpell( u, spellids[ spellindex ], true );
			}

			   }break;
		}
	}

	return true;
}


////////////////////////////////////////////////////////////////////////
//Championing Tabards' dummy aura handler
//
//Precondition(s)
//  Casted by Player.
//
//Effect
//  We will get reputation for the faction the tabard represents,
//  in lvl80 dungeons, heroics and raids.
//
////////////////////////////////////////////////////////////////////////
bool ChampioningTabards( uint32 i, Aura *a, bool apply ){
	Player *p_caster = a->GetPlayerCaster();

	if( p_caster == NULL )
		return true;

	uint32 Faction = a->GetSpellProto()->EffectMiscValue[ 0 ];

	if( apply )
		p_caster->SetChampioningFaction( Faction );
	else
		p_caster->SetChampioningFaction( 0 );

	return true;
}


///////////////////////////////////////////////////////////////
//Spinning dummy effect handler
//
//Precondition(s)
//  Casted by Player
//
//Effect(s)
//  Spins the target around, setting a random orientation
//
//
///////////////////////////////////////////////////////////////
bool Spinning( uint32 i, Spell *s ){
	Player *p_caster = s->p_caster;

	if( p_caster == NULL )
		return true;

	uint32 orientations[] = { 0,1,2,3,4,5,6 };
	uint32 index = RandomUInt( 6 );
	float neworientation = orientations[ index ];

	float X = p_caster->GetPositionX();
	float Y = p_caster->GetPositionY();
	float Z = p_caster->GetPositionZ();
	uint32 mapid = p_caster->GetMapId();
	uint32 instanceid = p_caster->GetInstanceID();

	p_caster->SafeTeleport( mapid, instanceid, X, Y, Z, neworientation );
	
	return true;
}

//////////////////////////////////////////////////////////////////
//Listening to Music periodically triggered dummy aura
//( SpellId 50493 )
//
//Precondition(s)
//  Casted by Player
//
//Effect(s)
//  Makes the player dance
//
//
/////////////////////////////////////////////////////////////////
bool ListeningToMusic( uint32 i, Aura *a, bool apply ){
	Unit *m_target = a->GetTarget();
	Player *p_target = NULL;

	if( m_target->IsPlayer() )
		p_target = TO_PLAYER( m_target );
	else
		return true;

	if( apply ){
		p_target->SetEmoteState( EMOTE_STATE_DANCE );
	}else{
		p_target->SetEmoteState( EMOTE_STATE_NONE );
	}

	return true;
}


///////////////////////////////////////////////////////////////////
//Periodic Drink Dummy Aura handler
//
//
//Precondition(s)
//  None
//
//Effect(s)
//  Restores x Mana Points every second
//
//
//////////////////////////////////////////////////////////////////
bool DrinkDummyAura( uint32 i, Aura *a, bool apply ){
	if( !apply )
		return true;

	float famount = 2.2f * ( a->GetSpellProto()->EffectBasePoints[ 1 ] / 5 );
	int32 amount = static_cast< int32 >( Arcemu::round( famount ) );

	a->EventPeriodicDrink( amount );

	return true;
}

// ADD NEW FUNCTIONS ABOVE THIS LINE
// *****************************************************************************

void SetupItemSpells_1(ScriptMgr * mgr)
{
	mgr->register_dummy_spell(23453, &GnomishTransporter);      // Gnomish Transporter
	mgr->register_dummy_spell(16589, &NoggenFoggerElixr);       // Noggenfogger
	mgr->register_dummy_spell(24930, &HallowsEndCandy);         // Hallows End Candy
	mgr->register_dummy_spell( 8063, &DeviateFish);             // Deviate Fish
	mgr->register_dummy_spell( 8213, &CookedDeviateFish);       // Savory Deviate Delight
	mgr->register_dummy_spell(26074, &HolidayCheer);            // Holiday Cheer
	mgr->register_dummy_spell(13120, &NetOMatic);               // Net-o-Matic
	uint32 BanishExileIds[] = { 4130, 4131, 4132, 0 };
	mgr->register_dummy_spell( BanishExileIds, &BanishExile);   // Essence of the Exile Quest
	mgr->register_dummy_spell(19938, &ForemansBlackjack);       // Lazy Peons Quest
	mgr->register_dummy_spell(39105, &NetherWraithBeacon);      // Spellfire Tailor Quest
	mgr->register_dummy_spell(30458, &NighInvulnBelt);          // Nigh Invulnerability Belt
	
	mgr->register_dummy_spell(25860, &ReindeerTransformation);  // Fresh Holly & Preserved Holly
	
	mgr->register_script_effect(26275, &WinterWondervolt);        // PX-238 Winter Wondervolt Trap

	mgr->register_dummy_spell(32042, &ScryingCrystal);			// Violet Scrying Crystal (Quest)
	mgr->register_dummy_spell(32001, &MinionsOfGurok);			// Minions of gurok
	mgr->register_dummy_spell(29200, &PurifyBoarMeat);			// Purify Boar meat spell
	mgr->register_script_effect(35036, &WarpRiftGenerator);       // Summon a Warp Rift in Void Ridge
	mgr->register_dummy_aura( 46354, &OrbOfTheSindorei);        //Orb of the Sin'dorei
	mgr->register_dummy_aura( 58983, &BigBlizzardBear);			// Big Blizzard Bear mount
	mgr->register_dummy_aura( 54729, &WingedSteed);				// DK flying mount
	mgr->register_dummy_aura( 48025, &HeadlessHorsemanMount);	// Headless Horseman Mount
	mgr->register_dummy_aura( 47977, &MagicBroomMount);			// Magic Broom Mount
	mgr->register_dummy_aura( 65917, &MagicRoosterMount);		// Magic Rooster Mount
	mgr->register_dummy_aura( 72286, &Invincible);				// Invincible

	mgr->register_dummy_spell( 30507, &Poultryizer );
	mgr->register_dummy_spell( 14537, &SixDemonBag );
	mgr->register_dummy_spell( 30427, &ExtractGas );

	mgr->register_script_effect( 24590, &BrittleArmor );

	mgr->register_dummy_aura( 46699, &RequiresNoAmmo );

	mgr->register_dummy_spell( 55004, &NitroBoosts );

	mgr->register_dummy_spell( 13006, &ShrinkRay );

	uint32 championingspellids[] = {
		57819,
		57820,
		57821,
		57822,
		0
	};

	mgr->register_dummy_aura( championingspellids, &ChampioningTabards );

	mgr->register_dummy_spell( 64385, &Spinning );

	mgr->register_dummy_aura( 50493, &ListeningToMusic );

	uint32 DrinkDummySpellIDs[] = {
		430,
		431,
		432,
		1133,
		1135,
		1137,
		10250,
		22734,
		27089,
		34291,
		43182,
		43183,
		46755,
		57073,
		61830,
		72623,
		0
	};
	mgr->register_dummy_aura( DrinkDummySpellIDs, &::DrinkDummyAura );


// REGISTER NEW DUMMY SPELLS ABOVE THIS LINE
// *****************************************************************************
}
