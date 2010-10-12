#include "StdAfx.h"

AIInterface * AIInterface_Mgr::CreateInterface(Unit * self, AIType type, Unit * owner)
{
	AIInterface * ai = NULL;
	switch(type)
	{
	case AITYPE_GUARDIAN:
		ai = new GuardianAI(self,owner);
		break;
	case AITYPE_CRITTER:
		ai = new CritterAI(self);
		break;
	case AITYPE_TOTEM:
		ai = new TotemAI(self,owner);
		break;
	case AITYPE_PET:
		ai = new PetAI(self,owner);
		break;
	default:
		ai = new MobAI(self);
	}
	return ai;
}
void AIInterface_Mgr::ConstructAISpell(Field * field)
{
	CreatureProto * cn = CreatureProtoStorage.LookupEntry( field[AIAGENTFIELD_OBJECTENTRY].GetUInt32() );
	uint8 proc = field[AIAGENTFIELD_PROCHANCE].GetUInt8();
	//Fail conditions.
	if(cn == NULL || proc == 0 || proc > 100)
		return;
	AI_Spell * nSpell = NULL;
	switch(field[AIAGENTFIELD_SPELLTYPE].GetUInt32() )
	{
	case AISPELLTYPE_HEAL:
		nSpell = new AI_HealSpell;
		break;
	case AISPELLTYPE_AOE:
		nSpell = new AI_AOESpell;
		break;
	case AISPELLTYPE_AOE_HEAL:
		nSpell = new AI_AOEHealSpell;
		break;
	case AISPELLTYPE_SUMMON:
		nSpell = new AI_SummonSpell;
		break;
	case AISPELLTYPE_SUMMON_DEST:
		nSpell = new AI_SummonSpellDest;
		break;
	case AISPELLTYPE_DISPEL:
		nSpell = new AI_DispelSpell;
		break;
	default:
		nSpell = new AI_Spell;
	}
	nSpell->Construct();
	//get the spell entry.
	nSpell->proto = dbcSpell.LookupEntryForced(field[AIAGENTFIELD_SPELLID].GetUInt32() );
	nSpell->interval = field[AIAGENTFIELD_COOLDOWN].GetUInt32();
	//some spells don't need a valid spell entry at all.
	if(nSpell->proto != NULL)
	{
		nSpell->minrange = GetMinRange(dbcSpellRange.LookupEntry(nSpell->proto->rangeIndex));
		nSpell->maxrange = GetMaxRange(dbcSpellRange.LookupEntry(nSpell->proto->rangeIndex));
		if(nSpell->interval < 500 )// 500ms seems pretty good 
		{
			//now this will not be exact cooldown but maybe a bigger one to not make him spam spells to often
			uint32 cooldown;
			SpellDuration *sd=dbcSpellDuration.LookupEntry(nSpell->proto->DurationIndex);
			int Dur= 0;
			int Casttime= 0;//most of the time 0
			int RecoveryTime=nSpell->proto->RecoveryTime;
			if(nSpell->proto->DurationIndex)
				Dur =::GetDuration(sd);
			Casttime=GetCastTime(dbcSpellCastTime.LookupEntry(nSpell->proto->CastingTimeIndex));
			cooldown=Dur+Casttime+RecoveryTime;
			if(cooldown<0)
				nSpell->interval=2000;//huge value that should not loop while adding some timestamp to it
			else nSpell->interval = cooldown;
		}
	}
	nSpell->difficulty = field[AIAGENTFIELD_DIFFICULTY].GetUInt8();
	nSpell->spell_agent = field[AIAGENTFIELD_SPELLAGENT].GetUInt8();
	//agent types are bit masks.
	nSpell->spell_agent = (1 << --nSpell->spell_agent);
	nSpell->event_type = field[AIAGENTFIELD_EVENTYPE].GetUInt8();
	nSpell->procChance = proc;
	nSpell->maxcastcount = field[AIAGENTFIELD_CASTCOUNT].GetUInt8();
	nSpell->spellTargeType = field[AIAGENTFIELD_SPELLTTYPE].GetUInt8();
	const char * attributes = field[AIAGENTFIELD_ATTRIBUTES].GetString();
	if(attributes != NULL)
		AISpell_parseAttrStr(nSpell, attributes);
	cn->spells.push_back(nSpell);
}
void AIInterface_Mgr::AISpell_parseAttrStr(AI_Spell* spell,const char * attr)
{
	string attributes(attr);
	int delimiter = 0, lastdelimiter = 0,cpos,pos;
	int length = attributes.length();
	string token;
	while(delimiter < length)
	{
		delimiter = attributes.find(';',lastdelimiter);
		if(delimiter == (int)string::npos) break;
		//Extract the token btwn semicolons.
		token = attributes.substr(lastdelimiter, delimiter);
		//increment past this semicolon to prepare to the next.
		++delimiter;
		lastdelimiter = delimiter;
		pos = token.find("initial_cooldown");
		if(pos != (int)string::npos)
		{
			//Dissect token "initial_cooldown(#cooldown)"
			pos = token.find('(',pos);
			if(pos != (int)string::npos)
			{
				cpos = pos;
				pos = token.find(')',pos);
				int initial_cd = atoi(token.substr(++cpos, (pos != string::npos)? pos : token.length() ).c_str() );
				spell->cooldown = getMSTime() + initial_cd;
			}
			break;
		}
		//dissect "random_interval(#1,#2)"
		pos = token.find("random_interval");
		if(pos != (int)string::npos)
		{
			pos = token.find('(',pos);
			if(pos != (int)string::npos)
			{
				cpos = pos;
				pos = token.find(',',pos);
				uint32 random_range1 = (uint32)atoi( token.substr(++cpos,pos).c_str() );
				cpos = pos;
				pos = token.find(')');
				uint32 random_range2 = (uint32)atoi(token.substr(++cpos, (pos != string::npos) ? pos : token.length() ).c_str() );
				if(spell->rdata == NULL)
					spell->rdata = new AISpell_RandomData;
				spell->rdata->interval[0] = random_range1;
				spell->rdata->interval[1] = random_range2;
			}break;
		}
		//dissect "sound_id(#id)"
		pos = token.find("sound_id");
		if(pos != (int)string::npos)
		{
			pos = token.find('(',pos);
			if(pos != (int)string::npos)
			{
				int cpos = pos;
				pos = token.find(')',pos);
				uint32 soundid = (uint32)atoi(token.substr(++cpos,(pos != string::npos) ? pos : token.length() ).c_str() );
				if(soundid)
				{
					if(spell->interact == NULL)
						spell->interact = new AISpell_InteractionData;
					spell->interact->soundId  = soundid;
				}
			}break;
		}
		//dissect "message(chat_type, lang_type, txt)"
		pos = token.find("message");
		if(pos != (int)string::npos)
		{
			int chat_type = 0, lang_type = 0;
			const char * msg = NULL;
			pos = token.find('(',pos);
			if(pos != (int)string::npos)
			{
				int cpos = pos;
				pos = token.find(',',pos);
				if(pos != (int)string::npos)
				{
					chat_type = atoi(token.substr(++cpos,pos).c_str() );
					cpos = pos;
					pos = token.find(',',pos);
					if(pos != (int)string::npos)
					{
						lang_type = atoi(token.substr(++cpos, pos).c_str() );
						cpos = pos;
						pos = token.find(')',pos);
						if(pos != (int)string::npos)
						{
							msg = token.substr(++cpos, pos).c_str();
							size_t strLength = strlen(msg);
							if(spell->interact == NULL)
								spell->interact = new AISpell_InteractionData;
							spell->interact->chat_type = (uint16)chat_type;
							spell->interact->lang_type = (uint16)lang_type;
							//Allocate memory for our string + the null terminator.
							char * sendtxt = (char*)malloc(strLength+1);
							//Append null terminator at the very end.
							*(sendtxt+strLength) = 0;
							//Now we simply copy into the newly allocated memory.
							memcpy( sendtxt, msg, strLength);
							spell->interact->sendtxt = sendtxt;
						}
					}
				}
			}break;
		}
		if(spell->spell_agent == AGENT_SPELL)
			AIInterface_Mgr::AICasterSpell_parseAttrToken(spell, token);
		else if(spell->spell_agent == AGENT_RANGED)
			AIInterface_Mgr::AIRangedSpell_parseAttrToken(spell, token);
		else if(spell->spell_agent == AGENT_MELEE)
			AIInterface_Mgr::AIMeleeSpell_parseAttrToken(spell, token);
	}
}
void AIInterface_Mgr::AICasterSpell_parseAttrToken(AI_Spell * spell, string & token)
{
	int pos = 0;
	int cpos = 0;
	uint8 type = spell->type;
	if(type == AISPELLTYPE_DISPEL)
	{
		pos = token.find("dispel_type");
		if(pos != (int)string::npos)
		{
			pos = token.find('(',pos);
			if(pos != (int)string::npos)
			{
				cpos = pos;
				pos = token.find(')',pos);
				int dispel_type = atoi( token.substr(++cpos,(pos != string::npos) ? pos : token.length() ).c_str() );
				static_cast<AI_DispelSpell*>(spell)->dispel_Type = dispel_type;
			}
		}
	}
	else if(type == AISPELLTYPE_AOE || type == AISPELLTYPE_AOE_HEAL || type == AISPELLTYPE_SUMMON || type == AISPELLTYPE_SUMMON_DEST)
	{
		//dissect "random_radius(#1,#2)"
		pos = token.find("random_radius");
		if(pos != (int)string::npos)
		{
			pos = token.find('(',pos);
			if(pos != (int)string::npos)
			{
				cpos = pos;
				pos = token.find(',',pos);
				if(pos != string::npos)
				{
					uint8 random_rad1 = (uint8)atoi( token.substr(++cpos,pos).c_str() );
					cpos = pos;
					pos = token.find(')',pos);
					uint8 random_rad2 = (uint8)atoi( token.substr(++cpos,(pos != string::npos) ? pos : token.length() ).c_str() );
					if(spell->rdata == NULL)
						spell->rdata = new AISpell_RandomData;
					spell->rdata->radius[0] = random_rad1;
					spell->rdata->radius[1] = random_rad2;
				}
			}
			return;
		}
		pos = token.find("random_facing");
		if(pos != (int)string::npos)
		{
			pos = token.find('(',pos);
			if(pos != (int)string::npos)
			{
				cpos = pos;
				pos = token.find(',',pos);
				if(pos != (int)string::npos)
				{
					uint16 rand_theta1 = 0, rand_theta2 = 0;
					rand_theta1 = (uint16)atoi(token.substr(++cpos, pos).c_str() );
					cpos = pos;
					pos = token.find(')');
					rand_theta2 = (uint16)atoi(token.substr(++cpos, ( pos != string::npos) ? pos : token.length() ).c_str() );
					if(rand_theta1 && rand_theta2)
					{
						if(spell->rdata == NULL)
							spell->rdata = new AISpell_RandomData;
						spell->rdata->facing[0] = rand_theta1;
						spell->rdata->facing[1] = rand_theta2;
					}
				}
			}
			return;
		}
		pos = token.find("facing");
		if(pos != (int)string::npos)
		{
			pos = token.find('(',pos);
			if(pos != (int)string::npos)
			{
				cpos = pos;
				pos = token.find(')',pos);
				uint16 facing = (uint16)atoi( token.substr(++cpos,(pos != string::npos) ? pos : token.length() ).c_str() );
				reinterpret_cast<AI_AOETemplate*>(spell)->facing = facing;
			}
			return;
		}
		pos = token.find("radius");
		if(pos != (int)string::npos)
		{
			pos = token.find('(',pos);
			if(pos != (int)string::npos)
			{
				cpos = pos;
				pos = token.find(')',pos);
				if(pos != (int)string::npos)
				{
					uint16 radius = (uint16)atoi( token.substr(++cpos,pos).c_str() );
					reinterpret_cast<AI_AOETemplate*>(spell)->radius = radius;
				}
			}
			return;
		}
		if(type == AISPELLTYPE_HEAL || type == AISPELLTYPE_AOE_HEAL)
		{
			pos = token.find("health");
			if(pos != (int)string::npos)
			{
				pos = token.find('(',pos);
				if(pos != (int)string::npos)
				{
					cpos = pos;
					pos = token.find(',',pos);
					if(pos != (int)string::npos)
					{
						uint32 heal_hp = (uint32)atoi(token.substr(++cpos, pos).c_str() );
						//make sure it's btwn 0-100
						if(heal_hp > 0 && heal_hp <= 100)
							static_cast<AI_HealSpell*>(spell)->heal_hp = heal_hp;
						return;
					}
				}
			}
		}
		else if(type == AISPELLTYPE_SUMMON || type == AISPELLTYPE_SUMMON_DEST)
		{
			//extract "summon_creature(#creature_entry)"
			pos = token.find("summon_creature");
			if(pos != (int)string::npos)
			{
				pos = token.find('(',pos);
				if(pos != (int)string::npos)
				{
					cpos = pos;
					pos = token.find(')',pos);
					if(pos != (int)string::npos)
					{
						uint32 creature_entry = (uint32)atoi( token.substr(++cpos,pos).c_str() );
						static_cast<AI_SummonSpell*>(spell)->summon_type = AI_SUMMONTYPE_CREATURE;
						static_cast<AI_SummonSpell*>(spell)->summon_entry = creature_entry;
					}
				}
				return;
			}
			//extract "summon_gameobject(#gameobject_entry)"
			pos = token.find("summon_gameobject");
			if(pos != string::npos)
			{
				pos = token.find('(',pos);
				if(pos != string::npos)
				{
					cpos = pos;
					pos = token.find(')',pos);
					if(pos != string::npos)
					{
						uint32 gameobject_entry = (uint32)atoi( token.substr(++cpos,pos).c_str() );
						static_cast<AI_SummonSpell*>(spell)->summon_type = AI_SUMMONFLAG_GAMEOBJECT;
						static_cast<AI_SummonSpell*>(spell)->summon_entry = gameobject_entry;
					}
				}
				return;
			}
			//extract "summon_spell(#spellid)"
			pos = token.find("summon_spell");
			if(pos != string::npos)
			{
				pos = token.find('(',pos);
				if(pos != string::npos)
				{
					uint32 summon_spell;
					cpos = pos;
					pos = token.find(')',pos);
					summon_spell = (uint32)atoi( token.substr(++cpos,(pos != string::npos) ?pos : token.length() ).c_str() );
					static_cast<AI_SummonSpell*>(spell)->summon_type = AI_SUMMONTYPE_ISSPELL;
					static_cast<AI_SummonSpell*>(spell)->summon_entry = summon_spell;
				}
				return;
			}
			// summon_count(count)
			pos = token.find("summon_count");
			if(pos != (int)string::npos)
			{
				pos = token.find('(',pos);
				if(pos != (int)string::npos)
				{
					cpos = pos;
					pos = token.find(')',pos);
					uint8 summon_cnt = (uint8)atoi(token.substr(++cpos, (pos != string::npos)? pos : token.length() ).c_str() );
					static_cast<AI_SummonSpell*>(spell)->summon_count = summon_cnt;
				}
				return;
			}
			// faction(faction)
			pos = token.find("faction");
			if(pos != (int)string::npos)
			{
				pos = token.find('(',pos);
				if(pos != (int)string::npos)
				{
					cpos = pos;
					pos = token.find(')',pos);
					uint8 faction = (uint8)atoi(token.substr(++cpos,(pos != string::npos)? pos : token.length() ).c_str() );
					static_cast<AI_SummonSpell*>(spell)->summon_count = faction;
				}
				return;
			}
			// delta_angle(angle)
			pos = token.find("delta_angle");
			if(pos != (int)string::npos)
			{
				pos = token.find('(',pos);
				if(pos != (int)string::npos)
				{
					int cpos = pos;
					pos = token.find(')',pos);
					int dTheta = atoi(token.substr(++cpos, (pos != string::npos) ? pos : token.length() ).c_str() );
					static_cast<AI_SummonSpell*>(spell)->delta_angle = (uint8)dTheta;
				}
				return;
			}
			//We parse entries, "summon_locations{count(#cnt),(x,y,z,o)(x,y,z,o)(x,y,z,o) }"
			if(type == AISPELLTYPE_SUMMON_DEST)
			{
				pos = token.find("summon_locations");
				if(pos != (int)string::npos)
				{
					pos = token.find('{',pos);
					if(pos != (int)string::npos)
					{
						cpos = pos;
						pos = token.find('}',pos);
						if(pos != (int)string::npos)
						{
							cpos = token.find("count",cpos);
							if(cpos != (int)string::npos)
							{
								cpos = token.find('(',cpos);
								if(cpos != (int)string::npos)
								{
									int ccpos = cpos;
									cpos = token.find(')',cpos);
									if(cpos != (int)string::npos)
									{
										int location_count = atoi(token.substr(++ccpos,cpos).c_str() );
										LocationVector * locations = new LocationVector[location_count];
										memset(locations,0,sizeof(LocationVector)*location_count);
										cpos = token.find(',',cpos);
										if(cpos != (int)string::npos)
										{
											int i = 0;
											while(cpos <= pos && i < location_count)
											{
												cpos = token.find('(',cpos);
												if(cpos == (int)string::npos)
													break;
												ccpos = cpos;
												cpos = token.find(',',cpos);
												//parse x
												locations[i].x = (float)atof(token.substr(++ccpos,cpos).c_str() );
												ccpos = cpos;
												cpos = token.find(',',cpos);
												locations[i].y = (float)atof(token.substr(++ccpos,cpos).c_str() );
												ccpos = cpos;
												cpos = token.find(',',cpos);
												locations[i].z = (float)atof(token.substr(++ccpos,cpos).c_str() );
												ccpos = cpos;
												cpos = token.find(')',cpos);
												locations[i].o = (float)atof(token.substr(++ccpos,cpos).c_str() );
												++i;
											}
										}
										static_cast<AI_SummonSpellDest*>(spell)->vectorCount = location_count;
										static_cast<AI_SummonSpellDest*>(spell)->dest = locations;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}
void AIInterface_Mgr::AIRangedSpell_parseAttrToken(AI_Spell * spell, string & token)
{
	int cpos, pos = 0;
	pos = token.find("isautoshot");
}
void AIInterface_Mgr::AIMeleeSpell_parseAttrToken(AI_Spell*spell, string& token)
{

}
AI_PetSpell * AIInterface_Mgr::ConstructPetSpell(uint32 spell_id)
{
	SpellEntry * proto = dbcSpell.LookupEntryForced(spell_id);
	AI_PetSpell * ret = NULL;
	//don't bother w/ passive spells.
	if(proto != NULL )
	{
		ret = new AI_PetSpell;
		switch(proto->NameHash)
		{
			//fire shield gets cast on owner attacked.
		case SPELL_HASH_FIRE_SHIELD:
			ret->event_type = EVENT_OWNERATTACKED;
			break;
		case SPELL_HASH_BLOOD_PACT:
			ret->event_type = EVENT_ENTERWORLD;
			break;
			//these get cast on leave combat?
		case SPELL_HASH_CONSUME_SHADOWS:
		case SPELL_HASH_PHASE_SHIFT:
		case SPELL_HASH_LESSER_INVISIBILITY:
			ret->event_type = EVENT_LEAVECOMBAT;
			break;
		default:
			//the rest get cast during combat.
			ret->event_type = EVENT_INCOMBAT;
		}
		SpellDuration *sd=dbcSpellDuration.LookupEntry(proto->DurationIndex);
		int Dur= 0;
		int Casttime= 0;//most of the time 0
		int RecoveryTime= proto->RecoveryTime;
		if(proto->DurationIndex)
			Dur =::GetDuration(sd);
		Casttime=GetCastTime(dbcSpellCastTime.LookupEntry(proto->CastingTimeIndex));
		ret->interval=Dur+Casttime+RecoveryTime; //assign the cooldown time.
		//greater than 1min cd, it's invalid.
		if(ret->interval > 60*1000)
			ret->interval = 0;
		ret->cooldown = 0;
		ret->proto = proto;	//assign the proto
	}
	return ret;
}
