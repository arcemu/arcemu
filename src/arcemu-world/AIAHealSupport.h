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

#ifndef WOWSERVER_AIAGENTHEALSUPPORT_H
#define WOWSERVER_AIAGENTHEALSUPPORT_H
#if 1 
class Object;
class Creature;
class Unit;
class Player;
class WorldSession;
class SpellCastTargets;

#define SPELL_EFF_PCT_SCALE_WITH_DIFFICULTY		1.2f	//given as a float value we influence the effectiveness of our spells
#define CREATURE_STATS_SCALE_WITH_DIFFICULTY	1
#define DIFFICULTY_UPDATE_SPEED					0.004f	//suppose we have 500 ms update interval -> 1 sec = 0.1

#define HELPER_MOUNT_A_DISPLAY 14584
#define HELPER_MOUNT_H_DISPLAY 5228

#define OWNER_SIDE_HORDE 0
#define OWNER_SIDE_ALIANCE 1

struct healagentspell
{
	healagentspell() { chance_to_cast=100; lastcastHP=100; }
	float		chance_to_cast;		//maybe there are spells that need to be casted rarely
	int32		max_scale;			//should scale with difficulty. No way to find a healbot that is good for everybody
	uint32		lastcastHP;			//this is a healbot we monitor owner HP to see if we should scale with diff
	uint32		cooldown;
	SpellEntry	*sp;
};

typedef std::map<uint32, uint32> CooldownMap;
typedef std::list<healagentspell*> SpellList;

class SERVER_DECL AiAgentHealSupport : public AIInterface
{

public:
	void				Init(Unit *un, AIType at, MovementType mt, Unit *owner);
	void				Update(uint32 p_time);

private:
	bool				Protect_self();
	bool				CheckCanCast(SpellEntry *sp,Unit *target);
	void				SetSpellDuration(healagentspell *sp);
	healagentspell		*PickSpellFromList(SpellList *spl,Unit *target);


	SpellList			m_healspells;			
	SpellList			m_AugmentSelf;			
	SpellList			m_AugmentTarget;			
	healagentspell		m_defend_self;
	healagentspell		revive_spell;
//	healagentspell		m_castingSpell;
	uint8				Owner_side; //can be A or H
	bool				last_time_full_health;
	float				DifficultyLevel; //spell values should scale up with the level of the support unit 
	float				CombatDifficultyLevel; //spell values should scale up with the level of the support unit 
	CooldownMap         spell_cooldown_map;
	uint32				First_noaction_stamp;
	uint32				Boredom_cooldown;
};

#endif
#endif
