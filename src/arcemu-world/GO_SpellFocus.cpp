/*
* ArcEmu MMORPG Server
* Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
* Copyright (C) 2008-2014 <http://www.ArcEmu.org/>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Affero General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Affero General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*/
#include "StdAfx.h"
namespace Arcemu{
	GO_SpellFocus::GO_SpellFocus() : GameObject(){
		spell = NULL;
	}
	GO_SpellFocus::GO_SpellFocus(uint64 GUID) : GameObject(GUID){
		spell = NULL;
	}
	GO_SpellFocus::~GO_SpellFocus(){
	}
	void GO_SpellFocus::InitAI(){
		GameObjectInfo *i = GameObjectNameStorage.LookupEntry(pInfo->spellFocus.linkedTrapId);;
		if (i == NULL){
			sLog.outError("Gamobject %u is of spellfocus type, has attachment GO data ( %u ), but attachment not found in database.", pInfo->ID, pInfo->raw.sound2);
			return;
		}
		spell = dbcSpell.LookupEntryForced(i->trap.spellId);
		range = static_cast< float >(i->trap.radius);
		if (myScript == NULL)
			myScript = sScriptMgr.CreateAIScriptClassForGameObject(GetEntry(), this);
	}
	void GO_SpellFocus::Update(unsigned long time_passed){
		if (m_event_Instanceid != m_instanceId){
			event_Relocate();
			return;
		}
		if (!IsInWorld())
			return;
		if (m_deleted)
			return;
		if (spell != NULL && (GetState() == 1)){
			for (std::set< Object* >::iterator itr = m_objectsInRange.begin(); itr != m_objectsInRange.end(); ++itr){
				float dist;
				Object *o = *itr;
				dist = GetDistanceSq(o);
				if (!o->IsUnit())
					continue;
				if ((m_summoner != NULL) && (o->GetGUID() == m_summoner->GetGUID()))
					continue;
				if (dist <= range){
					if (m_summonedGo){
						if (!m_summoner){
							ExpireAndDelete();
							return;
						}
						if (!isAttackable(m_summoner, o))
							continue;
					}
					CastSpell(o->GetGUID(), spell->Id);
					if (m_summonedGo){
						ExpireAndDelete();
						return;
					}
					if (spell->EffectImplicitTargetA[0] == 16 ||
						spell->EffectImplicitTargetB[0] == 16){
						return;	// on area don't continue.
					}
				}
			}
		}
	}
	void GO_SpellFocus::CastSpell(uint64 TargetGUID, uint32 SpellID){
		SpellEntry *sp = dbcSpell.LookupEntryForced(SpellID);
		if (sp == NULL){
			sLog.outError("GameObject %u tried to cast a non-existing Spell %u.", pInfo->ID, SpellID);
			return;
		}
		Spell *s = new Spell(this, sp, true, NULL);
		SpellCastTargets tgt(TargetGUID);
		tgt.m_destX = GetPositionX();
		tgt.m_destY = GetPositionY();
		tgt.m_destZ = GetPositionZ();
		s->prepare(&tgt);
	}
}