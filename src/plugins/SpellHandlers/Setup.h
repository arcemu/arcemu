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

#include "StdAfx.h"

#ifndef SPELL_SCRIPTS_SETUP_H
#define SPELL_SCRIPTS_SETUP_H

// Class spell handlers
void SetupShamanSpells(ScriptMgr* mgr);
void SetupWarlockSpells(ScriptMgr* mgr);
void SetupWarriorSpells(ScriptMgr* mgr);
void SetupHunterSpells(ScriptMgr* mgr);
void SetupMageSpells(ScriptMgr* mgr);
void SetupPaladinSpells(ScriptMgr* mgr);
void SetupRogueSpells(ScriptMgr* mgr);
void SetupPriestSpells(ScriptMgr* mgr);
void SetupDruidSpells(ScriptMgr* mgr);
void SetupDeathKnightSpells(ScriptMgr* mgr);

void SetupOldSpellCPPSuff(ScriptMgr* mgr);
void SetupPetAISpells(ScriptMgr* mgr);
void SetupQuestItems(ScriptMgr* mgr);
void SetupItemSpells_1(ScriptMgr* mgr);
void SetupMiscSpellhandlers(ScriptMgr* mgr);
void setupTransformSpellHandlers(ScriptMgr* mgr);

//Instance spells (mainly scripted targets)
void SetupRaidBlackTempleSpells(ScriptMgr* mgr);

#endif
