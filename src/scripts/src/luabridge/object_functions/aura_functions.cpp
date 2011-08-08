
#include "LUAEngine.h"

namespace lua_engine
{

	void bindAuraMethods(luabridge::module & m)
	{
		m	.class_<Aura>("Aura")
		.constructor<void (*)(SpellEntry*, uint32, Unit*, Unit*, bool)>()
		.method(&Aura::GetSpellProto, "GetSpellProto", "getproto", "GetProto", "getProto", NULL)
		.method(&Aura::GetSpellId, "GetID", "GetSpellID", "getid", NULL)

		.method(&Aura::IsPassive, "IsPassive", "isPassive", "ispassive", NULL)
		.method(&Aura::IsPositive, "isPositive", "IsPositive", "ispositive", NULL)

		/*	Aura Duration */
		.method(&Aura::GetDuration, "GetDuration", "getDuration", "getduration", NULL)
		.method(&Aura::SetDuration, "setDuration", "SetDuration", "setduration", NULL)
		.method(&Aura::ResetDuration, "resetduration", "ResetDuration", "resetDuration", NULL)

		/*	Aura Slot */
		.method(&Aura::GetAuraSlot, "GetAuraSlot", "getAuraSlot", "getslot", "getSlot", NULL)
		.method(&Aura::SetAuraSlot, "setAuraSlot", "SetAuraSlot", "setSlot", "SetSlot", NULL)

		/*	Object Caster */
		.method(&Aura::GetCaster, "getcaster", "getCaster", "GetCaster", "getObject", NULL)
		.method(&Aura::GetCasterGUID, "getcasterguid", "getCasterGUID", "GetCasterGUID", NULL)

		/*	Unit Caster */
		.method(&Aura::GetUnitCaster, "getUnitCaster", "GetUnitCaster", "getunitcaster", "getunit", "getUnit", NULL)

		/*	Player caster */
		.method(&Aura::GetPlayerCaster, "GetPlayerCaster", "getplayercaster", "getplayer", "getPlayer", NULL)

		.method(&Aura::Remove, "Remove", "remove", NULL);
	}
}