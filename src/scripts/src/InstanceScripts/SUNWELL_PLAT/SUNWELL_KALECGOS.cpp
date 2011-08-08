/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// by Parox

#define CN_KALECGOS					24850
#define CN_SATHROVAR	24892
#define CN_DARK_ELF	24891
#define KALECGOS_FROST_BREATH		44799
#define KALECGOS_ARCANE_BUFFET		45018
#define KALECGOS_SPECTRAL_BLAST_DUMMY 44869
#define KALECGOS_SPECTRAL_BLAST_AOE 44866
#define KALECGOS_SPECTRAL_TELEPORT 46019
#define KALECGOS_TAIL_LASH 45122
#define KALECGOS_WILDMAGIC 0
#define KALECGOS_WILDMAGIC_COST 45010
#define KALECGOS_WILDMAGIC_HEALING 44978
#define KALECGOS_WILDMAGIC_CASTTIME 45001
#define KALECGOS_WILDMAGIC_PHYSICALHIT 45002
#define KALECGOS_WILDMAGIC_CRITICALDMG 45004
#define KALECGOS_WILDMAGIC_THREAT 45006
#define SPECTRAL_EXHAUSTION	44867
#define SPECTRAL_BOSSBUFF 44801
#define SPECTRAL_PLAYERBUFF 46021
#define SPECTRAL_FORCEREACTION 44852
#define SATHROVAR_CORRUPTING_STRIKE 45029
#define SATHROVAR_BOUNDLESS_CURSE 45032
#define SATHROVAR_SBOLT_VOLLEY 45031
#define KALECGOS_BERSERK 26662
#define KALECGOS_BANISHED_AURA 32567
#define DARKELF_HEROIC_STRIKE 45026
#define DARKELF_REVITALIZE 45027

#define EVILKALEC_INTRO 12422
#define EVILKALEC_INTROSPEECH "No longer will I be a slave to Malygos! Challenge me and you will be destroyed!"
#define EVILKALEC_SPELL1 12423
#define EVILKALEC_SPELL1SPEECH "I will purge you!"
#define EVILKALEC_SPELL2 12424
#define EVILKALEC_SPELL2SPEECH "Your pain has only begun!"
#define EVILKALEC_TARGETKILL1 12425
#define EVILKALEC_TARGETKILL1SPEECH "In the name of Kil'jaeden!"
#define EVILKALEC_TARGETKILL2 12426
#define EVILKALEC_TARGETKILL2SPEECH "You were warned!"
#define EVILKALEC_TURNS 12427
#define EVILKALEC_TURNSSPEECH "My awakening is complete! You shall all perish!"
#define GOODKALEC_INTRO 12428
#define GOODKALEC_INTROSPEECH "I need... your help... Cannot... resist him... much longer..."
#define GOODKALEC_LOSING 12429
#define GOODKALEC_LOSINGSPEECH "Aaahhh! Help me, before I lose my mind!"
#define GOODKALEC_WIN 12431
#define GOODKALEC_WINSPEECH "I am forever in your debt. Once we have triumphed over Kil'jaeden, this entire world will be in your debt as well."
#define GOODKALEC_NEARLYLOST 12430
#define GOODKALEC_NEARLYLOSTSPEECH  "Hurry! There is not much of me left!"
#define SATHROVAR_AGGRO 12451
#define SATHROVAR_AGGROSPEECH "Gyahaha... There will be no reprieve. My work here is nearly finished."
#define SATHROVAR_SPELL1 12453
#define SATHROVAR_SPELL1SPEECH "Your misery is my delight!"
#define SATHROVAR_SPELL2 12454
#define SATHROVAR_SPELL2SPEECH "I will watch you bleed!"
#define SATHROVAR_KILL1 12455
#define SATHROVAR_KILL1SPEECH "Pitious mortal!"
#define SATHROVAR_KILL2 12456
#define SATHROVAR_KILL2SPEECH "Haven't you heard? I always win!"
#define SATHROVAR_DEATH 12452
#define SATHROVAR_DEATHSPEECH "I'm... never on... the losing... side..."
#define SATHROVAR_BERSERK 12457
#define SATHROVAR_BERSERKSPEECH "I have toyed with you long enough!"

#define SCRIPTTARGETFLAG_NONE 0x00
#define SCRIPTTARGETFLAG_NO_TANK 0x01
#define SCRIPTTARGETFLAG_NO_EXHAUTION 0x02

#define FRIENDLY_FACTION 35

namespace SUNWELL_KALECGOS
{
	struct coords
	{
		float x, y, z;
		coords(float x, float y, float z)
		{
			coords::x = x;
			coords::y = y;
			coords::z = z;
		}
	};

	struct CallBackManager
	{
		uint32 evtype;
		CallbackBase* cb;
		time_t interval;
		uint8 repeats;
		bool random;
		uint16 randinterval[2];
		bool deleted;
		CallBackManager(uint32 type, CallbackBase* cb, time_t interval, uint8 repeats, bool random, uint16 randinterval[2])
		{
			evtype = type;
			CallBackManager::cb = cb;
			CallBackManager::interval = interval;
			CallBackManager::repeats = repeats;
			CallBackManager::random = random;
			CallBackManager::randinterval[0] = randinterval[0];
			CallBackManager::randinterval[1] = randinterval[1];
			deleted = false;
		}
		~CallBackManager()
		{
			delete cb;
		}
	};
	struct SoundEntryManager
	{
		bool repeatable;
		bool oneshotsay;
		SoundEntryManager(bool reps, bool oneshot)
		{
			repeatable = reps;
			oneshotsay = oneshot;
		}
	};

	coords spectralCoords(1724.94f, 909.265f, -74.5588f);
	coords wp1(1703.69653f, 869.224854f, 65.416687f);
	coords wp2(1703.131348f, 842.142822f, 91.754280f);
	coords wp3(1752.924194f, 754.499634f, 181.749878f);
	class DRAGON_KALECGOS : public CreatureAIScript
	{
			CreaturePointer sathrovar;
			CreaturePointer darkelf;
			bool failed;
			bool  success;
			typedef map<uint32, SoundEntryManager*> SAYMAP;
			vector<CallBackManager*> cbMap;
			SAYMAP saymap;
		public:
			ADD_CREATURE_FACTORY_FUNCTION(DRAGON_KALECGOS);
			DRAGON_KALECGOS(CreaturePointer crc) : CreatureAIScript(crc)
			{
				success = false;
				_unit->GetAIInterface()->addWayPoint(CreateWaypoint(1, wp1.x, wp1.y, wp1.z, _unit->GetOrientation(), 1000));
				_unit->GetAIInterface()->addWayPoint(CreateWaypoint(2, wp2.x, wp2.y, wp2.z, _unit->GetOrientation(), 1000));
				_unit->GetAIInterface()->addWayPoint(CreateWaypoint(3, wp3.x, wp3.y, wp3.z, _unit->GetOrientation(), 1000));
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
			}
			WayPoint* const CreateWaypoint(uint32 id, float x, float y, float z, float o, uint32 waitime)
			{
				WayPoint* tempWp = new WayPoint();
				tempWp->x = x;
				tempWp->y = y;
				tempWp->z = z;
				tempWp->o = o;
				tempWp->id = id;
				tempWp->waittime = waitime;
				tempWp->flags = 768; // fly
				return tempWp;
			}
			UnitPointer const GetRandomTarget(uint8 targetFlags)
			{
				vector<UnitPointer> targetMap;
				PlayerSet::iterator itr = _unit->GetInRangePlayerSetBegin();
				for(; itr != _unit->GetInRangePlayerSetEnd(); ++itr)
				{
					if((*itr) && (*itr)->isAlive())
					{
						if((targetFlags & SCRIPTTARGETFLAG_NO_TANK) && _unit->GetAIInterface()->GetMostHated()->GetGUID() == (*itr)->GetGUID())
							continue;
						if((*itr)->FindAura(SPECTRAL_EXHAUSTION) && (targetFlags & SCRIPTTARGETFLAG_NO_EXHAUTION))
							continue;
						targetMap.push_back(static_cast<UnitPointer>((*itr)));
					}
				}
				if(targetMap.size() > 0)
					return targetMap[(uint32)(rand() % targetMap.size())];
				else
					return NULLUNIT;
			}
			void const SayYell(CreaturePointer obj, uint32 id, const char* text)
			{
				SAYMAP::iterator itr = saymap.find(id);
				if(itr == saymap.end()) return;
				if(!itr->second->oneshotsay && !itr->second->repeatable) return;
				obj->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, text);
				obj->PlaySoundToSet(id);
				itr->second->oneshotsay = false;
				if(!itr->second->repeatable)
				{
					delete itr->second;
					saymap.erase(itr);
				}
			}
			void OnCombatStart(UnitPointer mTarget)
			{
				failed = false;
				_unit->SetStandState(STANDSTATE_STAND);
				FillSayMap();
				InitializeSpells();
				SayYell(_unit, EVILKALEC_INTRO, EVILKALEC_INTROSPEECH);
				RegisterAIUpdateEvent(1000);
				sathrovar = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1704.109985f, 927.835022f, -74.558296f, CN_SATHROVAR);
				darkelf = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1729.9559961f, 935.401001f, -74.558098f, CN_DARK_ELF);
				ASSERT(sathrovar != NULL && darkelf != NULL)
				darkelf->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, FRIENDLY_FACTION);
				darkelf->_setFaction();
				darkelf->GetAIInterface()->AttackReaction(sathrovar, 1);

			}
			void OnTargetDied(UnitPointer mTarget)
			{

				if(rand() % 2)
					SayYell(_unit, EVILKALEC_TARGETKILL1, EVILKALEC_TARGETKILL1SPEECH);
				else
					SayYell(_unit, EVILKALEC_TARGETKILL2, EVILKALEC_TARGETKILL2SPEECH);
			}
			void OnCombatStop(UnitPointer mTarget)
			{
				if(failed)
				{
					cbMap.clear();
					saymap.clear();
				}
			}
			void OnDied(UnitPointer mKiller)
			{
				_unit->SetHealthPct(3); // prevent him from dieing lololol
				_unit->CastSpell(_unit, KALECGOS_BANISHED_AURA, false);
				_unit->CallScriptUpdate(); // force a  callscriptupdate.
			}
			void const HandleCallBacks()
			{
				vector<CallBackManager*>::iterator itr, itr2 = cbMap.begin();
				while(itr2 != cbMap.end())
				{
					itr = itr2++;
					if((*itr)->deleted)
					{
						delete(*itr);
						cbMap.erase(itr); // all elements after this are invalidated :(
						itr2 = cbMap.begin();
						continue;
					}
					if((*itr)->interval)
					{
						time_t currTime = (*itr)->interval;
						if((currTime - 1000) <= 0)
						{
							(*itr)->cb->execute();
							if((*itr)->repeats && --(*itr)->repeats <= 0)
								(*itr)->deleted = true;
							else
							{
								if((*itr)->random)
								{
									uint16 first = (*itr)->randinterval[0];
									uint16 second = (*itr)->randinterval[1];
									(*itr)->interval = (time_t)((rand() % first) + second);
								}
								else
									(*itr)->interval = (*itr)->randinterval[0];
							}
						}
						else
							(*itr)->interval -= 1000;
					}
				}
			}
			void const PauseEvent(uint32 evt, time_t extendedtime)
			{
				vector<CallBackManager*>::iterator itr = cbMap.begin();
				for(; itr != cbMap.end(); ++itr)
				{
					if((*itr)->evtype == evt)
						(*itr)->interval += extendedtime;
					break;
				}
			}
			void const StopAllEvents()
			{
				vector<CallBackManager*>::iterator itr = cbMap.begin();
				for(; itr != cbMap.end(); ++itr)
					(*itr)->deleted = true;
			}
			void const SetFly(bool fly)
			{
				if(fly)
				{
					WorldPacket data(SMSG_MOVE_SET_HOVER, 13);
					data << _unit->GetNewGUID();
					data << uint32(0);
					_unit->SendMessageToSet(&data, false);
					_unit->GetAIInterface()->m_moveFly = true;
				}
				else
				{
					WorldPacket data(SMSG_MOVE_UNSET_HOVER, 13);
					data << _unit->GetNewGUID();
					data << uint32(0);
					_unit->SendMessageToSet(&data, false);
					_unit->GetAIInterface()->m_moveFly = false;
				}
			}
			void MoveToWps()
			{
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
				_unit->GetAIInterface()->setWaypointToMove(1);
			}
			void OnReachWP(uint32 iWaypointId, bool bForwards)
			{
				switch(iWaypointId)
				{
					case 3: // Finished encounter ^ ^
						{
							cbMap.clear();
							saymap.clear();
							_unit->Despawn(100, 0);
							if(sathrovar != NULL)
								sathrovar->Despawn(100, 0);
							if(darkelf != NULL)
								darkelf->Despawn(100, 0);
						}
				}
			}
			void AIUpdate()
			{
				HandleCallBacks();
				if(success) return;
				int32 hp = _unit->GetHealthPct();
				if(sathrovar != NULL && sathrovar->FindAura(KALECGOS_BERSERK) && !_unit->FindAura(KALECGOS_BERSERK))
				{
					_unit->CastSpell(_unit, KALECGOS_BERSERK, false);
				}
				if(darkelf != NULL && darkelf->IsDead())
				{
					failed = true;
					SayYell(_unit, EVILKALEC_TURNS, EVILKALEC_TURNSSPEECH);
					_unit->Despawn(20000, 20000);
				}
				if(hp <= 10 && !_unit->FindAura(KALECGOS_BERSERK) && !sathrovar->FindAura(KALECGOS_BERSERK) && !success)
				{
					_unit->CastSpell(_unit, KALECGOS_BERSERK, false);
					sathrovar->CastSpell(sathrovar, KALECGOS_BERSERK, false);
				}
				if(hp <= 3 && !_unit->FindAura(KALECGOS_BANISHED_AURA) && !failed)
				{
					_unit->GetAIInterface()->disable_melee = true;
					_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
					StopAllEvents();
					_unit->CastSpell(_unit, KALECGOS_BANISHED_AURA, false);
					_unit->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
				}
				if(sathrovar->FindAura(KALECGOS_BANISHED_AURA) && darkelf->isAlive())
				{
					if(darkelf->GetAIInterface()->GetNextTarget()->GetGUID() == sathrovar->GetGUID())
					{
						darkelf->GetAIInterface()->SetAllowedToEnterCombat(false);
						darkelf->GetAIInterface()->disable_melee = true;
					}
				}
				if(!failed && _unit->FindAura(KALECGOS_BANISHED_AURA) && sathrovar->FindAura(KALECGOS_BANISHED_AURA) && darkelf->isAlive())
				{
					StopAllEvents();
					_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, FRIENDLY_FACTION);
					_unit->_setFaction();
					_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
					_unit->GetAIInterface()->disable_melee = true;
					_unit->RemoveAura(KALECGOS_BANISHED_AURA);
					_unit->RemoveAura(KALECGOS_BERSERK);
					sathrovar->RemoveAura(KALECGOS_BERSERK);
					SayYell(_unit, GOODKALEC_WIN, GOODKALEC_WINSPEECH);
					success = true;
					SetFly(true);
					uint16 range[] = {0x00, 0x00}; // yeahh.....
					cbMap.push_back(new CallBackManager(0x00, new CallbackP0<DRAGON_KALECGOS>(this, &DRAGON_KALECGOS::MoveToWps), 5000, 1, false, range));
				}
			}
			void const FillSayMap()
			{
				saymap.insert(pair<uint32, SoundEntryManager*>(EVILKALEC_INTRO, new SoundEntryManager(false, true)));
				saymap.insert(pair<uint32, SoundEntryManager*>(EVILKALEC_SPELL1, new SoundEntryManager(true, true)));
				saymap.insert(pair<uint32, SoundEntryManager*>(EVILKALEC_SPELL2, new SoundEntryManager(true, true)));
				saymap.insert(pair<uint32, SoundEntryManager*>(EVILKALEC_TURNS, new SoundEntryManager(false, true)));
				saymap.insert(pair<uint32, SoundEntryManager*>(EVILKALEC_TARGETKILL1, new SoundEntryManager(true, true)));
				saymap.insert(pair<uint32, SoundEntryManager*>(EVILKALEC_TARGETKILL2, new SoundEntryManager(true, true)));
				saymap.insert(pair<uint32, SoundEntryManager*>(GOODKALEC_WIN, new SoundEntryManager(false, true)));
			}
			void const InitializeSpells()
			{
				set<CallBackManager*> buffer;
				uint16 range[] = {10000, 12000};
				CallBackManager* cbm = new CallBackManager(KALECGOS_FROST_BREATH, new CallbackP1<DRAGON_KALECGOS, uint32>(this, &DRAGON_KALECGOS::CastScriptSpell, KALECGOS_FROST_BREATH), 10000, 0, true, range);
				buffer.insert(cbm);
				cbm = new CallBackManager(KALECGOS_TAIL_LASH, new CallbackP1<DRAGON_KALECGOS, uint32>(this, &DRAGON_KALECGOS::CastScriptSpell, KALECGOS_TAIL_LASH), 10000, 0, true, range);
				buffer.insert(cbm);
				cbm = new CallBackManager(KALECGOS_WILDMAGIC, new CallbackP1<DRAGON_KALECGOS, uint32>(this, &DRAGON_KALECGOS::CastScriptSpell, KALECGOS_WILDMAGIC), 10000, 0, true, range);
				buffer.insert(cbm);
				range[0] = 15000;
				range[1] = 25000;
				cbm = new CallBackManager(KALECGOS_SPECTRAL_BLAST_DUMMY, new CallbackP1<DRAGON_KALECGOS, uint32>(this, &DRAGON_KALECGOS::CastScriptSpell, KALECGOS_SPECTRAL_BLAST_DUMMY), 15000, 0, true, range);
				buffer.insert(cbm);
				range[0] = 9000;
				range[1] = 0x00;
				cbm = new CallBackManager(KALECGOS_ARCANE_BUFFET, new CallbackP1<DRAGON_KALECGOS, uint32>(this, &DRAGON_KALECGOS::CastScriptSpell, KALECGOS_ARCANE_BUFFET), 9000, 0, false, range);
				buffer.insert(cbm);
				for(set<CallBackManager*>::iterator itr = buffer.begin(); itr != buffer.end(); ++itr)
				{
					cbMap.push_back((*itr));
				}
			}
			void CastScriptSpell(uint32 spellid)
			{
				if(_unit->GetCurrentSpell() != NULL || _unit->GetUInt32Value(UNIT_CHANNEL_SPELL))
				{
					PauseEvent(spellid, 1000);
				}
				UnitPointer target = NULLUNIT;
				switch(spellid)
				{
					case KALECGOS_WILDMAGIC:
						{
							uint32 const sarray[] =
							{
								KALECGOS_WILDMAGIC_CRITICALDMG,
								KALECGOS_WILDMAGIC_COST,
								KALECGOS_WILDMAGIC_PHYSICALHIT,
								KALECGOS_WILDMAGIC_THREAT,
								KALECGOS_WILDMAGIC_HEALING,
								KALECGOS_WILDMAGIC_CASTTIME
							};
							uint32 spelltocast = sarray[(uint32)(rand() % 6)];
							target = GetRandomTarget(SCRIPTTARGETFLAG_NONE);
							if(target != NULL)
								_unit->CastSpell(_unit, spelltocast, false);
						}
						break;
					case KALECGOS_SPECTRAL_BLAST_DUMMY:
						{
							target = GetRandomTarget(SCRIPTTARGETFLAG_NO_EXHAUTION);
							if(target != NULL)
							{
								_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, target->GetGUID());
								_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, spellid);
								uint16 range[] = {0x00, 0x00};
								CallBackManager* cb = new CallBackManager(spellid, new CallbackP1<DRAGON_KALECGOS, uint32>(this, &DRAGON_KALECGOS::CastScriptSpell, KALECGOS_SPECTRAL_BLAST_AOE), 2000, 1, false, range);
								cbMap.push_back(cb);
							}
						}
						break;
					case KALECGOS_SPECTRAL_BLAST_AOE:
						{
							target = _unit->GetMapMgr()->GetUnit(_unit->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT));
							if(target != NULL)
							{
								target->CastSpell(target, spellid, false);
								target->CastSpell(target, SPECTRAL_PLAYERBUFF, true);
								target->CastSpell(target, KALECGOS_SPECTRAL_TELEPORT, false);
							}
							_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
							_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
						}
						break;
					case KALECGOS_FROST_BREATH:
						_unit->GetAIInterface()->StopMovement(1000);
						_unit->CastSpell(_unit, spellid, false);
						break;
					default:
						{
							target = GetRandomTarget(SCRIPTTARGETFLAG_NONE);
							if(target != NULL)
							{
								_unit->CastSpell(target, spellid, false);
							}
							if(Rand(40))
							{
								if(!(rand() % 2))
									SayYell(_unit, EVILKALEC_SPELL1, EVILKALEC_SPELL1SPEECH);
								else
									SayYell(_unit, EVILKALEC_SPELL2, EVILKALEC_SPELL2SPEECH);
							}
						}
						break;
				}
			}

	};
	class SATHROVAR : public CreatureAIScript
	{
			vector<CallBackManager*> cbMap;
			map<uint32, SoundEntryManager*> saymap;
		public:
			ADD_CREATURE_FACTORY_FUNCTION(SATHROVAR);
			SATHROVAR(CreaturePointer crc) : CreatureAIScript(crc) {}
			UnitPointer const GetRandomTarget()
			{
				vector<UnitPointer> targetMap;
				PlayerSet::iterator itr = _unit->GetInRangePlayerSetBegin();
				for(; itr != _unit->GetInRangePlayerSetEnd(); ++itr)
				{
					if((*itr) && (*itr)->isAlive())
					{
						if((*itr)->FindAura(SPECTRAL_PLAYERBUFF) == NULL)
							continue;
						targetMap.push_back(static_cast<UnitPointer>((*itr)));
					}
				}
				if(targetMap.size() > 0)
					return targetMap[(uint32)(rand() % targetMap.size())];
				else
					return NULL;
			}
			void const InitializeSpells()
			{
				uint16 range[] = {10000, 12000};
				CallBackManager* cbm = new CallBackManager(SATHROVAR_SBOLT_VOLLEY, new CallbackP1<SATHROVAR, uint32>(this, &SATHROVAR::CastScriptSpell, SATHROVAR_SBOLT_VOLLEY), 10000, 0, true, range);
				cbMap.push_back(cbm);
				cbm = new CallBackManager(SATHROVAR_CORRUPTING_STRIKE, new CallbackP1<SATHROVAR, uint32>(this, &SATHROVAR::CastScriptSpell, SATHROVAR_CORRUPTING_STRIKE), 10000, 0, true, range);
				cbMap.push_back(cbm);
				range[0] = 60000;
				range[1] = 0;
				cbm = new CallBackManager(SATHROVAR_BOUNDLESS_CURSE, new CallbackP1<SATHROVAR, uint32>(this, &SATHROVAR::CastScriptSpell, SATHROVAR_BOUNDLESS_CURSE), 60000, 0, false, range);
				cbMap.push_back(cbm);
			}
			void const FillSayMap()
			{
				saymap.insert(pair<uint32, SoundEntryManager*>(SATHROVAR_AGGRO, new SoundEntryManager(false, true)));
				saymap.insert(pair<uint32, SoundEntryManager*>(SATHROVAR_KILL1, new SoundEntryManager(true, true)));
				saymap.insert(pair<uint32, SoundEntryManager*>(SATHROVAR_KILL2, new SoundEntryManager(true, true)));
				saymap.insert(pair<uint32, SoundEntryManager*>(SATHROVAR_SPELL1, new SoundEntryManager(true, true)));
				saymap.insert(pair<uint32, SoundEntryManager*>(SATHROVAR_SPELL2, new SoundEntryManager(true, true)));
				saymap.insert(pair<uint32, SoundEntryManager*>(SATHROVAR_DEATH, new SoundEntryManager(false, true)));
				saymap.insert(pair<uint32, SoundEntryManager*>(SATHROVAR_BERSERK, new SoundEntryManager(false, true)));
			}
			void const SayYell(uint32 id, const char* text)
			{
				map<uint32, SoundEntryManager*>::iterator itr = saymap.find(id);
				if(itr == saymap.end()) return;
				if(!itr->second->oneshotsay && !itr->second->repeatable) return;
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, text);
				_unit->PlaySoundToSet(id);
				itr->second->oneshotsay = false;
				if(!itr->second->repeatable)
				{
					delete itr->second;
					saymap.erase(itr);
				}
			}
			void OnLoad()
			{
				_unit->CastSpell(_unit, SPECTRAL_BOSSBUFF, false);
			}
			void OnCombatStart(UnitPointer mTarget)
			{
				FillSayMap();
				InitializeSpells();
				RegisterAIUpdateEvent(1000);
			}
			void AIUpdate()
			{
				HandleCallBacks();
				int32 hp = _unit->GetHealthPct();
				if(hp <= 98)
					SayYell(SATHROVAR_AGGRO, SATHROVAR_AGGROSPEECH);
				if(hp <= 10 && !_unit->FindAura(KALECGOS_BERSERK) && !_unit->FindAura(KALECGOS_BANISHED_AURA))
				{
					SayYell(SATHROVAR_BERSERK, SATHROVAR_BERSERKSPEECH);
					_unit->CastSpell(_unit, KALECGOS_BERSERK, false);
				}
				else if(hp <= 3 && !_unit->FindAura(KALECGOS_BANISHED_AURA))
				{
					StopAllEvents();
					_unit->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
					_unit->GetAIInterface()->disable_melee = true;
					_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
					_unit->CastSpell(_unit, KALECGOS_BANISHED_AURA, false);
				}
				else if(hp <= 4 && _unit->FindAura(KALECGOS_BANISHED_AURA) && !_unit->FindAura(KALECGOS_BERSERK))
				{
					_unit->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
					_unit->DealDamage(_unit, _unit->GetUInt32Value(UNIT_FIELD_HEALTH), 0, 0, 0);
				}

			}
			void OnTargetDied(UnitPointer mTarget)
			{
				if(mTarget->IsCreature() && TO_CREATURE(mTarget)->GetEntry() == CN_DARK_ELF)
				{
					StopAllEvents();
					PlayerSet::iterator itr = _unit->GetInRangePlayerSetBegin();
					for(; itr != _unit->GetInRangePlayerSetEnd(); ++itr)
					{
						if((*itr)->HasAura(SPECTRAL_PLAYERBUFF))
							(*itr)->RemoveAura(SPECTRAL_PLAYERBUFF);
					}
					_unit->Despawn(1000, 20000);
				}
			}
			void OnDied(UnitPointer mKiller)
			{
				SayYell(SATHROVAR_DEATH, SATHROVAR_DEATHSPEECH);
			}
			void CastScriptSpell(uint32 spellid)
			{
				if(_unit->GetCurrentSpell() != NULL)
				{
					PauseEvent(spellid, 1000);
					return;
				}
				UnitPointer target = NULLUNIT;
				switch(spellid)
				{
					case SATHROVAR_CORRUPTING_STRIKE:
						{
							target = _unit->GetAIInterface()->GetMostHated();
							if(target != NULL)
							{
								_unit->CastSpell(target, SATHROVAR_CORRUPTING_STRIKE, false);
							}
						}
						break;
					default:
						target = GetRandomTarget();
						if(target != NULL)
						{
							_unit->CastSpell(target, spellid, false);
							if(Rand(40))
							{
								if((rand() % 2))
									SayYell(SATHROVAR_KILL1, SATHROVAR_KILL1SPEECH);
								else
									SayYell(SATHROVAR_KILL2, SATHROVAR_KILL2SPEECH);
							}
						}
						break;
				}
			}
			void const PauseEvent(uint32 evt, time_t extendedtime)
			{
				vector<CallBackManager*>::iterator itr = cbMap.begin();
				for(; itr != cbMap.end(); ++itr)
				{
					if((*itr)->evtype == evt)
						(*itr)->interval += extendedtime;
					break;
				}
			}
			void const StopAllEvents()
			{
				vector<CallBackManager*>::iterator itr = cbMap.begin();
				for(; itr != cbMap.end(); ++itr)
					(*itr)->deleted = true;
			}
			void const HandleCallBacks()
			{
				vector<CallBackManager*>::iterator itr, itr2 = cbMap.begin();
				while(itr2 != cbMap.end())
				{
					itr = itr2++;
					if((*itr)->deleted)
					{
						delete(*itr);
						cbMap.erase(itr); // all elements after this are invalidated :(
						itr2 = cbMap.begin();
						continue;
					}
					if((*itr)->interval)
					{
						time_t currTime = (*itr)->interval;
						if((currTime - 1000) <= 0)
						{
							(*itr)->cb->execute();
							if((*itr)->repeats && --(*itr)->repeats <= 0)
								(*itr)->deleted = true;
							else
							{
								if((*itr)->random)
								{
									uint16 first = (*itr)->randinterval[0];
									uint16 second = (*itr)->randinterval[1];
									(*itr)->interval = (time_t)((rand() % first) + second);
								}
								else
									(*itr)->interval = (*itr)->randinterval[0];
							}
						}
						else
							(*itr)->interval -= 1000;
					}
				}
			}
	};
	class DARK_ELF : public CreatureAIScript
	{
			map<uint32, SoundEntryManager*> saymap;
			vector<CallBackManager*> cbMap;
			CreaturePointer sathrovar;
		public:
			ADD_CREATURE_FACTORY_FUNCTION(DARK_ELF);
			DARK_ELF(CreaturePointer crc) : CreatureAIScript(crc) {}
			void OnCombatStart(UnitPointer mTarget)
			{
				FillSayMap();
				InitializeSpells();
				RegisterAIUpdateEvent(1000);
				if(!mTarget->IsCreature())
				{
					_unit->GetAIInterface()->WipeCurrentTarget();
				}
				else
				{
					sathrovar = TO_CREATURE(mTarget);
				}
			}
			void AIUpdate()
			{
				HandleCallBacks();
				int32 hp = _unit->GetHealthPct();
				if(hp <= 98)
					SayYell(GOODKALEC_INTRO, GOODKALEC_INTROSPEECH);
				else if(hp <= 30)
					SayYell(GOODKALEC_LOSING, GOODKALEC_LOSINGSPEECH);
				else if(hp <= 10)
					SayYell(GOODKALEC_NEARLYLOST, GOODKALEC_NEARLYLOSTSPEECH);
				if(sathrovar != NULL && sathrovar->FindAura(KALECGOS_BANISHED_AURA))
				{
					if(_unit->GetAIInterface()->GetNextTarget() == sathrovar)
					{
						_unit->GetAIInterface()->disable_melee = true;
						_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
						StopAllEvents();
					}
				}
				if(_unit->GetAIInterface()->GetNextTarget() != sathrovar)
					_unit->GetAIInterface()->WipeCurrentTarget();
			}
			void OnCombatStop(UnitPointer mTarget)
			{
				saymap.clear();
				cbMap.clear();
			}
			void const InitializeSpells()
			{
				uint16 range[] = {5000, 8000};
				CallBackManager* cbm = new CallBackManager(DARKELF_HEROIC_STRIKE, new CallbackP1<DARK_ELF, uint32>(this, &DARK_ELF::CastScriptSpell, DARKELF_HEROIC_STRIKE), 5000, 0, true, range);
				cbMap.push_back(cbm);
				range[1] = 10000;
				cbm = new CallBackManager(DARKELF_REVITALIZE, new CallbackP1<DARK_ELF, uint32>(this, &DARK_ELF::CastScriptSpell, DARKELF_REVITALIZE), 5000, 0, true, range);
				cbMap.push_back(cbm);
			}
			void const FillSayMap()
			{
				saymap.insert(pair<uint32, SoundEntryManager*>(GOODKALEC_INTRO, new SoundEntryManager(false, true)));
				saymap.insert(pair<uint32, SoundEntryManager*>(GOODKALEC_LOSING, new SoundEntryManager(false, true)));
				saymap.insert(pair<uint32, SoundEntryManager*>(GOODKALEC_NEARLYLOST, new SoundEntryManager(false, true)));
			}
			void const SayYell(uint32 id, const char* text)
			{
				map<uint32, SoundEntryManager*>::iterator itr = saymap.find(id);
				if(itr == saymap.end()) return;
				if(!itr->second->oneshotsay && !itr->second->repeatable) return;
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, text);
				_unit->PlaySoundToSet(id);
				itr->second->oneshotsay = false;
				if(!itr->second->repeatable)
				{
					delete itr->second;
					saymap.erase(itr);
				}
			}
			void CastScriptSpell(uint32 spellid)
			{
				if(_unit->GetCurrentSpell() != NULL)
				{
					PauseEvent(spellid, 1000);
					return;
				}
				_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), spellid, false);
			}

			void const PauseEvent(uint32 evt, time_t extendedtime)
			{
				vector<CallBackManager*>::iterator itr = cbMap.begin();
				for(; itr != cbMap.end(); ++itr)
				{
					if((*itr)->evtype == evt)
						(*itr)->interval += extendedtime;
					break;
				}
			}
			void const StopAllEvents()
			{
				vector<CallBackManager*>::iterator itr = cbMap.begin();
				for(; itr != cbMap.end(); ++itr)
					(*itr)->deleted = true;
			}
			void const HandleCallBacks()
			{
				vector<CallBackManager*>::iterator itr, itr2 = cbMap.begin();
				while(itr2 != cbMap.end())
				{
					itr = itr2++;
					if((*itr)->deleted)
					{
						delete(*itr);
						cbMap.erase(itr); // all elements after this are invalidated :(
						itr2 = cbMap.begin();
						continue;
					}
					if((*itr)->interval)
					{
						time_t currTime = (*itr)->interval;
						if((currTime - 1000) <= 0)
						{
							(*itr)->cb->execute();
							if((*itr)->repeats && --(*itr)->repeats <= 0)
								(*itr)->deleted = true;
							else
							{
								if((*itr)->random)
								{
									uint16 first = (*itr)->randinterval[0];
									uint16 second = (*itr)->randinterval[1];
									(*itr)->interval = (time_t)((rand() % first) + second);
								}
								else
									(*itr)->interval = (*itr)->randinterval[0];
							}
						}
						else
							(*itr)->interval -= 1000;
					}
				}
			}

	};
	bool HandleSpectralTeleport(uint32 i, SpellPointer pSpell)
	{
		if(i) return false;
		if(pSpell->p_caster)
		{
			pSpell->p_caster->CastSpell(pSpell->p_caster, SPECTRAL_PLAYERBUFF, true);
		}
		return true;
	}
}