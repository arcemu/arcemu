/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008-2011 Arcemu Team
 * Copyright (C) 2007 Moon++ <http://www.moonplusplus.com/>
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

#ifndef LUA_AURA_H
#define LUA_AURA_H

namespace LuaAura
{
	int GetObjectType(lua_State* L, Aura* aura)
	{
		if(!aura)
		{
			RET_NIL();
		}
		lua_pushstring(L, "Aura");
		return 1;
	}

	int GetSpellId(lua_State* L, Aura* aura)
	{
		if(!aura) RET_NIL();
		lua_pushnumber(L, aura->GetSpellId());
		return 1;
	}

	int GetCaster(lua_State* L, Aura* aura)
	{
		if(!aura) RET_NIL();
		Object* caster = aura->GetCaster();
		if(caster->IsUnit())  //unit caster
		{
			PUSH_UNIT(L, caster);
			return 1;
		}
		else if(caster->IsGameObject())  //gameobject
		{
			PUSH_GO(L, caster);
			return 1;
		}
		else if(caster->GetTypeId() == TYPEID_ITEM)  //item
		{
			PUSH_ITEM(L, caster);
			return 1;
		}
		else
		{
			RET_NIL();
		}
	}

	int GetTarget(lua_State* L, Aura* aura)
	{
		if(!aura) RET_NIL();
		PUSH_UNIT(L, aura->GetTarget());
		return 1;
	}

	int GetDuration(lua_State* L, Aura* aura)
	{
		if(!aura) RET_NIL();
		RET_NUMBER(aura->GetDuration()); //in milliseconds
	}

	int SetDuration(lua_State* L, Aura* aura)
	{
		if(!aura) return 0;
		int32 duration = luaL_checkint(L, 1);
		aura->SetDuration(duration);
		sEventMgr.ModifyEventTimeLeft(aura, EVENT_AURA_REMOVE, duration);
		return 0;
	}

	int GetTimeLeft(lua_State* L, Aura* aura)
	{
		if(!aura) RET_NIL();
		RET_NUMBER(aura->GetTimeLeft()); //in milliseconds
	}

	int SetNegative(lua_State* L, Aura* aura)
	{
		if(!aura) RET_BOOL(false);
		uint32 negativery = luaL_optint(L, 1, 1);
		aura->SetNegative(negativery);
		RET_BOOL(true);
	}

	int SetPositive(lua_State* L, Aura* aura)
	{
		if(!aura) RET_BOOL(false);
		uint32 positivery = luaL_optint(L, 1, 1);
		aura->SetPositive(positivery);
		RET_BOOL(true);
	}

	int Remove(lua_State* L, Aura* aura)
	{
		if(!aura) return 0;
		aura->Remove();
		return 0;
	}

	int SetVar(lua_State* L, Aura* aura)
	{
		const char* var = luaL_checkstring(L, 1);
		int subindex = 0;
		if(lua_gettop(L) == 3)
		{
			subindex = luaL_optint(L, 2, 0);
		}
		if(!aura || !var || subindex < 0)
		{
			lua_pushboolean(L, 0);
			return 1;
		}
		int valindex = 2;
		if(subindex)
			valindex++;
		SpellEntry* proto = aura->m_spellProto;
		LuaSpellEntry l = GetLuaSpellEntryByName(var);
		if(!l.name)
			RET_BOOL(false);
		switch(l.typeId)  //0: int, 1: char*, 2: bool, 3: float
		{
			case 0:
				GET_SPELLVAR_INT(proto, l.offset, subindex) = luaL_checkinteger(L, valindex);
				lua_pushboolean(L, 1);
				break;
			case 1:
				strcpy(GET_SPELLVAR_CHAR(proto, l.offset, subindex), luaL_checkstring(L, valindex));
				lua_pushboolean(L, 1);
				break;
			case 2:
				GET_SPELLVAR_BOOL(proto, l.offset, subindex) = CHECK_BOOL(L, valindex);
				lua_pushboolean(L, 1);
				break;
			case 3:
				GET_SPELLVAR_FLOAT(proto, l.offset, subindex) = (float)luaL_checknumber(L, valindex);
				lua_pushboolean(L, 1);
				break;
		}
		return 1;
	}

	int GetVar(lua_State* L, Aura* aura)
	{
		const char* var = luaL_checkstring(L, 1);
		int subindex = luaL_optint(L, 2, 0);
		if(!aura || !var || subindex < 0)
		{
			lua_pushnil(L);
			return 1;
		}
		SpellEntry* proto = aura->m_spellProto;
		LuaSpellEntry l = GetLuaSpellEntryByName(var);
		if(!l.name)
			RET_NIL();
		switch(l.typeId)  //0: int, 1: char*, 2: bool, 3: float
		{
			case 0:
				lua_pushinteger(L, GET_SPELLVAR_INT(proto, l.offset, subindex));
				break;
			case 1:
				lua_pushstring(L, GET_SPELLVAR_CHAR(proto, l.offset, subindex));
				break;
			case 2:
				lua_pushboolean(L, (GET_SPELLVAR_BOOL(proto, l.offset, subindex)) ? 1 : 0);
				break;
			case 3:
				lua_pushnumber(L, GET_SPELLVAR_FLOAT(proto, l.offset, subindex));
				break;
		}
		return 1;
	}

	int GetAuraSlot(lua_State* L, Aura* aura)
	{
		if(!aura) RET_NIL();
		RET_INT(aura->GetAuraSlot());
	}

	int SetAuraSlot(lua_State* L, Aura* aura)
	{
		if(!aura) return 0;
		uint16 slot = CHECK_USHORT(L, 1);
		aura->SetAuraSlot(slot);
		return 0;
	}
};

#endif