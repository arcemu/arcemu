#include "StdAfx.h"

AIInterface * AIInterface_Mgr::CreateAIInterface(Unit * self, AIType type, Unit * owner)
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
AI_Spell * AIInterface_Mgr::ConstructAISpell(Field * field)
{
	SpellEntry * sE = dbcSpell.LookupEntryForced(field[AIAGENTFIELD_SPELLID].GetUInt32() );
	uint8 proc = field[AIAGENTFIELD_PROCHANCE].GetUInt8();
	//Fail conditions.
	if(sE == NULL || proc == 0 || proc > 100)
		return NULL;
	AI_Spell * nSpell = NULL;
	switch(field[AIAGENTFIELD_SPELLTYPE].GetUInt32() )
	{
	case STYPE_HEAL:
		nSpell = new AI_HealSpell;
		break;
	case STYPE_AOE:
		nSpell = new AI_AOESpell;
		break;
	case STYPE_AOE_HEAL:
		nSpell = new AI_AOEHealSpell;
		break;
	case STYPE_SUMMON:
		nSpell = new AI_SummonSpell;
		break;
	case STYPE_SUMMON_DEST:
		nSpell = new AI_SummonSpellDest;
		break;
	case STYPE_DISPEL:
		nSpell = new AI_DispelSpell;
		break;
	default:
		nSpell = new AI_Spell;
	}
	nSpell->Construct();
	nSpell->sEntry = sE;
	nSpell->minrange = GetMinRange(dbcSpellRange.LookupEntry(sE->rangeIndex));
	nSpell->maxrange = GetMaxRange(dbcSpellRange.LookupEntry(sE->rangeIndex));
	nSpell->difficulty = field[AIAGENTFIELD_DIFFICULTY].GetUInt8();
	nSpell->spell_agent = field[AIAGENTFIELD_SPELLAGENT].GetUInt8();
	nSpell->event_type = field[AIAGENTFIELD_EVENTYPE].GetUInt8();
	nSpell->procChance = proc;
	nSpell->maxcastcount = field[AIAGENTFIELD_CASTCOUNT].GetUInt8();
	nSpell->spellTargeType = field[AIAGENTFIELD_SPELLTTYPE].GetUInt8();
	nSpell->interval = field[AIAGENTFIELD_COOLDOWN].GetUInt32();
	if(nSpell->interval < 500) // 500ms seems pretty good 
	{
		//now this will not be exact cooldown but maybe a bigger one to not make him spam spells to often
		uint32 cooldown;
		SpellDuration *sd=dbcSpellDuration.LookupEntry(sE->DurationIndex);
		int Dur= 0;
		int Casttime= 0;//most of the time 0
		int RecoveryTime=sE->RecoveryTime;
		if(sE->DurationIndex)
			Dur =::GetDuration(sd);
		Casttime=GetCastTime(dbcSpellCastTime.LookupEntry(sE->CastingTimeIndex));
		cooldown=Dur+Casttime+RecoveryTime;
		if(cooldown<0)
			nSpell->interval=2000;//huge value that should not loop while adding some timestamp to it
		else nSpell->interval = cooldown;
	}
	const char * attributes = field[AIAGENTFIELD_ATTRIBUTES].GetString();
	if(attributes != NULL)
		AISpell_parseAttrStr(nSpell, attributes);
	return nSpell;
}
void AIInterface_Mgr::AISpell_parseAttrStr(AI_Spell* spell,const char * attr)
{
	string attributes(attr);
	int cpos, pos = 0;
	int length = attributes.length();
	string token = NULL;
	while(pos <= length)
	{
		//cpos stores the previous position to pos while pos advances.
		cpos = pos;
		pos = attributes.find(';',pos);
		if(pos == (int)string::npos) break;
		/*	Extract the token btwn semicolons. Initially we don't want to skip cpos = 0 position. 
			Otherwise we use cpos+1 to skip over previously found semicolon; */
		if(cpos == 0)
			token = attributes.substr(cpos, pos);
		else
			token = attributes.substr(++cpos, pos);
		int pos = 0;
		pos = token.find("initial_cooldown");
		if(pos != (int)string::npos)
		{
			//Dissect token "initial_cooldown(#cooldown)"
			pos = token.find('(');
			if(pos != (int)string::npos)
			{
				int cpos = pos;
				pos = token.find(')',pos);
				if(pos != (int)string::npos)
				{
					uint32 initial_cd = (uint32)atoi(token.substr(++cpos, pos).c_str() );
					spell->cooldown = getMSTime() + initial_cd;
					return;
				}
			}
		}
		//dissect "random_interval(#1,#2)"
		pos = token.find("random_interval");
		if(pos != (int)string::npos)
		{
			pos = token.find('(',pos);
			if(pos != (int)string::npos)
			{
				int cpos = pos;
				pos = token.find(',',pos);
				uint32 random_range1 = (uint32)atoi( token.substr(++cpos,pos).c_str() );
				cpos = pos;
				pos = token.find(')');
				uint32 random_range2 = (uint32)atoi(token.substr(++cpos, pos).c_str() );
				if(spell->rdata == NULL)
					spell->rdata = new AISpell_RandomData;
				spell->rdata->interval[0] = random_range1;
				spell->rdata->interval[1] = random_range2;
				return;
			}
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
				if(pos != (int)string::npos)
				{
					uint32 soundid = (uint32)atoi(token.substr(++cpos,pos).c_str() );
					if(spell->interact == NULL)
						spell->interact = new AISpell_InteractionData;
					spell->interact->soundId  = soundid;
					return;
				}
			}
		}
		//dissect "message(chat_type, lang_type, txt)"
		pos = token.find("message");
		if(pos != (int)string::npos)
		{
			uint16 chat_type = 0, lang_type = 0;
			const char * msg = NULL;
			pos = token.find('(',pos);
			if(pos != (int)string::npos)
			{
				int cpos = pos;
				pos = token.find(',',pos);
				if(pos != (int)string::npos)
				{
					chat_type = (uint16)atoi(token.substr(++cpos,pos).c_str() );
					cpos = pos;
					pos = token.find(',',pos);
					if(pos != (int)string::npos)
					{
						lang_type = (uint16)atoi(token.substr(++cpos, pos).c_str() );
						cpos = pos;
						pos = token.find(',',pos);
						if(pos != (int)string::npos)
						{
							msg = token.substr(++cpos, pos).c_str();
							size_t strLength = strlen(msg);
							if(spell->interact == NULL)
								spell->interact = new AISpell_InteractionData;
							spell->interact->chat_type = chat_type;
							spell->interact->lang_type = lang_type;
							//Allocate memory for our string + the null terminator.
							char * sendtxt = (char*)malloc(strLength+1);
							//Append null terminator at the very end.
							*(sendtxt+strLength) = 0;
							//Now we simply copy into the newly allocated memory.
							memcpy( sendtxt, msg, strLength);
							spell->interact->sendtxt = sendtxt;
							return;
						}
					}
				}
			}
		}
		if(spell->spell_agent == AGENT_SPELL)
			AIInterface_Mgr::AISpell_parseAttrToken(spell, token);
		else if(spell->spell_agent == AGENT_RANGED)
			AIInterface_Mgr::AIRangedSpell_parseAttrToken(spell, token);
		else if(spell->spell_agent == AGENT_MELEE)
			AIInterface_Mgr::AIMeleeSpell_parseAttrToken(spell, token);
	}
}
void AIInterface_Mgr::AISpell_parseAttrToken(AI_Spell * spell, string & token)
{
	int pos = 0;
	uint8 type = spell->type;
	if(type == AISPELLTYPE_DISPEL)
	{
		pos = token.find("dispel_type");
		if(pos != (int)string::npos)
		{
			pos = token.find('(',pos);
			if(pos != (int)string::npos)
			{
				int cpos = pos;
				pos = token.find(')',pos);
				if(pos != (int)string::npos)
				{
					uint32 dispel_type = (uint32)atoi( token.substr(++cpos, pos).c_str() );
					static_cast<AI_DispelSpell*>(spell)->dispel_Type = dispel_type;
				}
			}
		}
	}
	else if(type == AISPELLTYPE_AOE || type == AISPELLTYPE_AOE_HEAL || type == AISPELLTYPE_SUMMON || type == AISPELLTYPE_SUMMON_DEST)
	{
		//dissect "random_radius(#1,#2)"
		pos = token.find("random_radius");
		if(pos != (int)string::npos)
		{
			int cpos;
			pos = token.find('(',pos);
			if(pos != (int)string::npos)
			{
				cpos = pos;
				pos = token.find(',',pos);
				uint8 random_rad1 = (uint8)atoi( token.substr(++cpos,pos).c_str() );
				cpos = pos;
				pos = token.find(')',pos);
				uint8 random_rad2 = (uint8)atoi( token.substr(++cpos,pos).c_str() );
				if(spell->rdata == NULL)
					spell->rdata = new AISpell_RandomData;
				spell->rdata->radius[0] = random_rad1;
				spell->rdata->radius[1] = random_rad2;
				return;
			}
		}
		pos = token.find("random_facing");
		if(pos != (int)string::npos)
		{
			int cpos;
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
					if(pos != (int)string::npos)
						rand_theta2 = (uint16)atoi(token.substr(++cpos, pos).c_str() );
					if(rand_theta1 && rand_theta2)
					{
						if(spell->rdata == NULL)
							spell->rdata = new AISpell_RandomData;
						spell->rdata->facing[0] = rand_theta1;
						spell->rdata->facing[1] = rand_theta2;
					}
					return;
				}
			}
		}
		if(type == AISPELLTYPE_AOE)
		{
			pos = token.find("facing");
			if(pos != (int)string::npos)
			{
				pos = token.find('(',pos);
				if(pos != (int)string::npos)
				{
					int cpos = pos;
					pos = token.find(')',pos);
					if(pos != (int)string::npos)
					{
						uint16 facing = (uint16)atoi( token.substr(++cpos,pos).c_str() );
						static_cast<AI_AOESpell*>(spell)->facing = facing;
						return;
					}
				}
			}
			pos = token.find("radius");
			if(pos != (int)string::npos)
			{
				pos = token.find('(',pos);
				if(pos != (int)string::npos)
				{
					int cpos = pos;
					pos = token.find(')',pos);
					if(pos != (int)string::npos)
					{
						uint16 radius = (uint16)atoi( token.substr(++cpos,pos).c_str() );
						static_cast<AI_AOESpell*>(spell)->radius = radius;
						return;
					}
				}
			}
		}
		else if(type == AISPELLTYPE_HEAL || type == AISPELLTYPE_AOE_HEAL)
		{
			pos = token.find("heal_hp");
			if(pos != (int)string::npos)
			{
				int cpos;
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
			if(type == AISPELLTYPE_AOE_HEAL)
			{
				pos = token.find("heal_radius");
				if(pos != (int)string::npos)
				{
					pos = token.find('(',pos);
					if(pos != (int)string::npos)
					{
						int cpos = pos;
						pos = token.find(')',pos);
						uint16 healrads = (uint16)atoi(token.substr(++cpos,pos).c_str() );
						static_cast<AI_AOEHealSpell*>(spell)->heal_radius = healrads;
						return;
					}
				}
				pos = token.find("start_angle");
				if(pos != (int)string::npos)
				{
					pos = token.find('(',pos);
					if(pos != (int)string::npos)
					{
						int cpos = pos;
						pos = token.find(')',pos);
						if( (int)string::npos != pos)
						{
							uint16 startangle = (uint16)atoi(token.substr(++cpos, pos).c_str() );
							static_cast<AI_AOEHealSpell*>(spell)->facing = startangle;
							return;
						}
					}
				}
			}
		}
		else if(type == AISPELLTYPE_SUMMON || type == AISPELLTYPE_SUMMON_DEST)
		{
			//summon_flags(creature/gameobject/summon_spell);
			pos = token.find("summon_type");
			if(pos != (int)string::npos)
			{
				pos = token.find('(',pos);
				if(pos != (int)string::npos)
				{
					int cpos = pos;
					pos = token.find(')',pos);
					if(pos != (int)string::npos)
					{
						const char * summon_type = token.substr(++cpos,pos).c_str();
						if(!strcmp(summon_type, "creature") )
							static_cast<AI_SummonSpell*>(spell)->summon_type = AI_SUMMONFLAG_SUMMONTYPE_CREATURE;
						else if(!strcmp(summon_type, "gameobject") )
							static_cast<AI_SummonSpell*>(spell)->summon_type = AI_SUMMONFLAG_SUMMONTYPE_GAMEOBJECT;
						else if( !strcmp(summon_type, "summon_spell") )
							static_cast<AI_SummonSpell*>(spell)->summon_type = AI_SUMMONFLAG_SUMMONENTRY_IS_SUMMONSPELL;
						return;
					}
				}
			}
			// summon_entry(entry)
			pos = token.find("summon_entry");
			if(pos != (int)string::npos)
			{
				pos = token.find('(',pos);
				if(pos != (int)string::npos)
				{
					int cpos = pos;
					pos = token.find(')',pos);
					if(pos != (int)string::npos)
					{
						uint32 summon_entry = (uint32)atoi(token.substr(++cpos, pos).c_str() );
						static_cast<AI_SummonSpell*>(spell)->summon_entry = summon_entry;
						return;
					}
				}
			}
			// summon_count(count)
			pos = token.find("summon_count");
			if(pos != (int)string::npos)
			{
				pos = token.find('(',pos);
				if(pos != (int)string::npos)
				{
					int cpos = pos;
					pos = token.find(')',pos);
					if(pos != (int)string::npos)
					{
						uint8 summon_cnt = (uint8)atoi(token.substr(++cpos, pos).c_str() );
						static_cast<AI_SummonSpell*>(spell)->summon_count = summon_cnt;
						return;
					}
				}
			}
			// summon_faction(faction)
			pos = token.find("summon_faction");
			if(pos != (int)string::npos)
			{
				pos = token.find('(',pos);
				if(pos != (int)string::npos)
				{
					int cpos = pos;
					pos = token.find(')',pos);
					if(pos != (int)string::npos)
					{
						uint8 faction = (uint8)atoi(token.substr(++cpos,pos).c_str() );
						static_cast<AI_SummonSpell*>(spell)->summon_count = faction;
						return;
					}
				}
			}
			// summon_radius(radius)
			pos = token.find("summon_radius");
			if(pos != (int)string::npos)
			{
				pos = token.find('(',pos);
				if(pos != (int)string::npos)
				{
					int cpos = pos;
					pos = token.find(')',pos);
					if(pos != (int)string::npos)
					{
						uint8 summon_rads = (uint8)atoi(token.substr(++cpos,pos).c_str() );
						static_cast<AI_SummonSpell*>(spell)->summon_radius = summon_rads;
						return;
					}
				}
			}
			// summon_angle(angle)
			pos = token.find("summon_facing");
			if(pos != (int)string::npos)
			{
				pos = token.find('(',pos);
				if(pos != (int)string::npos)
				{
					int cpos = pos;
					pos = token.find(')',pos);
					if(pos != (int)string::npos)
					{
						uint16 summon_angle = (uint16)atoi(token.substr(++cpos, pos).c_str() );
						static_cast<AI_SummonSpell*>(spell)->facing = summon_angle;
						return;
					}
				}
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
					if(pos != (int)string::npos)
					{
						uint16 dTheta = (uint16)atoi(token.substr(++cpos, pos).c_str() );
						static_cast<AI_SummonSpell*>(spell)->delta_angle = dTheta;
						return;
					}
				}
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
						int cpos = pos;
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
										uint32 location_count = (uint32)atoi(token.substr(++ccpos,cpos).c_str() );
										LocationVector * locations = new LocationVector[location_count];
										memset(locations,0,sizeof(LocationVector)*location_count);
										cpos = token.find(',',cpos);
										if(cpos != (int)string::npos)
										{
											uint32 i = 0;
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