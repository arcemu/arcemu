/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
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

void CreateDummySpell(uint32 id)
{
	const char * name = "Dummy Trigger";
	SpellEntry * sp = new SpellEntry;
	memset(sp, 0, sizeof(SpellEntry));
	sp->Id = id;
	sp->Attributes = 384;
	sp->AttributesEx = 268435456;
	sp->AttributesExB = 4;
	sp->CastingTimeIndex=1;
	sp->procChance=75;
	sp->rangeIndex=13;
	sp->EquippedItemClass=uint32(-1);
	sp->Effect[0]=3;
	sp->EffectImplicitTargetA[0]=25;
	sp->NameHash=crc32((const unsigned char*)name, (unsigned int)strlen(name));
	sp->dmg_multiplier[0]=1.0f;
	sp->StanceBarOrder=-1;
	dbcSpell.SetRow(id,sp);
	sWorld.dummyspells.push_back(sp);
}

void ApplyExtraDataFixes()
{
	SpellEntry * sp;
	// Spell 53 (Backstab Rank 1) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(53);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 72 (Shield Bash Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(72);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 75 (Auto Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(75);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 78 (Heroic Strike Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(78);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 116 (Frostbolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(116);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 118 (Polymorph Rank 1) is infront from spell extra.
//	sp = dbcSpell.LookupEntryForced(118);
//	if( sp != NULL )
//		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 133 (Fireball Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(133);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 143 (Fireball Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(143);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 145 (Fireball Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(145);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 205 (Frostbolt Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(205);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 228 (Polymorph: Chicken Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(228);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 246 (Slow Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(246);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 284 (Heroic Strike Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(284);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 285 (Heroic Strike Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(285);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 348 (Immolate Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(348);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 370 (Purge Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(370);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 403 (Lightning Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(403);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 408 (Kidney Shot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(408);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 421 (Chain Lightning Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(421);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 529 (Lightning Bolt Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(529);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 548 (Lightning Bolt Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(548);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 585 (Smite Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(585);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 589 (Shadow Word: Pain Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(589);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_SKIPCHECK;

	// Spell 591 (Smite Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(591);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 594 (Shadow Word: Pain Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(594);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_SKIPCHECK;

	// Spell 598 (Smite Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(598);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 603 (Curse of Doom Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(603);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 686 (Shadow Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(686);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 689 (Drain Life Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(689);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 694 (Mocking Blow Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(694);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 695 (Shadow Bolt Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(695);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 699 (Drain Life Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(699);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 701 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(701);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 702 (Curse of Weakness Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(702);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 703 (Garrote Rank 1) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(703);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 704 (Curse of Recklessness Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(704);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 705 (Shadow Bolt Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(705);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 707 (Immolate Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(707);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 709 (Drain Life Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(709);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 710 (Banish Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(710);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 770 (Faerie Fire Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(770);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 772 (Rend Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(772);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 778 (Faerie Fire Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(778);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 830 (Fearless ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(830);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 837 (Frostbolt Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(837);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 845 (Cleave Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(845);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 851 (Polymorph: Sheep Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(851);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_SKIPCHECK;

	// Spell 853 (Hammer of Justice Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(853);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 879 (Exorcism Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(879);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 915 (Lightning Bolt Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(915);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 921 (Pick Pocket ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(921);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 930 (Chain Lightning Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(930);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 943 (Lightning Bolt Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(943);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 970 (Shadow Word: Pain Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(970);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_SKIPCHECK;

	// Spell 980 (Curse of Agony Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(980);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 984 (Smite Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(984);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 992 (Shadow Word: Pain Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(992);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_SKIPCHECK;

	// Spell 1004 (Smite Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1004);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1010 (Curse of Idiocy Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1010);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1014 (Curse of Agony Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1014);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1079 (Rip Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1079);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1082 (Claw Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1082);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1088 (Shadow Bolt Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1088);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1094 (Immolate Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1094);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1098 (Enslave Demon Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1098);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1106 (Shadow Bolt Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1106);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1108 (Curse of Weakness Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1108);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1120 (Drain Soul Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1120);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1130 (Hunter's Mark Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1130);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1329 (Mutilate Rank 1) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(1329);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 1464 (Slam Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1464);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1490 (Curse of the Elements Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1490);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1495 (Mongoose Bite Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1495);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1510 (Volley Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1510);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1608 (Heroic Strike Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1608);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1671 (Shield Bash Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1671);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1672 (Shield Bash Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1672);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1714 (Curse of Tongues Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1714);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1715 (Hamstring Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1715);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1752 (Sinister Strike Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1752);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1757 (Sinister Strike Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1757);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1758 (Sinister Strike Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1758);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1759 (Sinister Strike Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1759);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1760 (Sinister Strike Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1760);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1766 (Kick Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1766);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1767 (Kick Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1767);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1768 (Kick Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1768);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1769 (Kick Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1769);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1776 (Gouge Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1776);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1777 (Gouge Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1777);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1804 (Pick Lock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1804);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1822 (Rake Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1822);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1823 (Rake Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1823);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1824 (Rake Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1824);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1940 (Rocket Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1940);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1943 (Rupture Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1943);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 1949 (Hellfire Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1949);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 2070 (Sap Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2070);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 2098 (Eviscerate Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2098);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 2136 (Fire Blast Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2136);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 2137 (Fire Blast Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2137);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 2138 (Fire Blast Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2138);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 2589 (Backstab Rank 2) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(2589);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 2590 (Backstab Rank 3) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(2590);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 2591 (Backstab Rank 4) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(2591);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 2596 (Touch of the Black Claw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2596);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 2626 (Lightning Bolt Proc V ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2626);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 2637 (Hibernate Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2637);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 2643 (Multi-Shot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2643);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 2652 (Touch of Weakness Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2652);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 2691 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2691);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 2764 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2764);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 2767 (Shadow Word: Pain Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2767);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_SKIPCHECK;

	// Spell 2812 (Holy Wrath Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2812);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 2860 (Chain Lightning Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2860);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 2908 (Soothe Animal Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2908);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 2912 (Starfire Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2912);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 2941 (Immolate Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2941);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 2943 (Touch of Weakness Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2943);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 2944 (Devouring Plague Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2944);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 2948 (Scorch Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2948);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 2973 (Raptor Strike Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2973);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 2974 (Wing Clip Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2974);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 3009 (Claw Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3009);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 3010 (Claw Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3010);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 3018 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3018);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 3029 (Claw Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3029);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 3034 (Viper Sting Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3034);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 3044 (Arcane Shot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3044);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 3110 (Firebolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3110);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 3130 (Ice Claw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3130);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 3140 (Fireball Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3140);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 3232 (Gouge Stun Test ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3232);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 3242 (Ravage ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3242);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 3252 (Shred ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3252);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 3263 (Touch of Ravenclaw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3263);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 3446 (Ravage ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3446);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 3674 (Black Arrow Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3674);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 3720 (Throw Syndicate Bomb ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3720);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 3823 (Throw Proximity Bomb ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3823);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 4043 (Detonation ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(4043);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 4164 (Throw Rock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(4164);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 4165 (Throw Rock II ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(4165);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 5010 (Scorch Breath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5010);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 5019 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5019);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 5138 (Drain Mana Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5138);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 5143 (Arcane Missiles Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5143);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 5144 (Arcane Missiles Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5144);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 5145 (Arcane Missiles Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5145);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 5171 (Slice and Dice Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5171);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 5176 (Wrath Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5176);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 5177 (Wrath Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5177);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 5178 (Wrath Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5178);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 5179 (Wrath Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5179);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 5180 (Wrath Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5180);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 5201 (Claw Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5201);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 5211 (Bash Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5211);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 5221 (Shred Rank 1) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(5221);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 5256 (Claw Cover ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5256);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 5401 (Lizard Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5401);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 5422 (Lacerate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5422);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 5424 (Claw Grasp ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5424);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 5480 (Cleave Armor Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5480);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 5508 (Cleave Armor Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5508);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 5532 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5532);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 5570 (Insect Swarm Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5570);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 5588 (Hammer of Justice Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5588);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 5589 (Hammer of Justice Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5589);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 5614 (Exorcism Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5614);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 5615 (Exorcism Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5615);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 5648 (Stunning Blast Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5648);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 5649 (Stunning Blast Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5649);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 5676 (Searing Pain Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5676);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 5782 (Fear Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5782);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 5938 (Shiv Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5938);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 5940 (Shiv Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5940);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6041 (Lightning Bolt Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6041);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6060 (Smite Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6060);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6136 (Chilled Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6136);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6146 (Slow Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6146);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6205 (Curse of Weakness Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6205);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6213 (Fear Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6213);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6215 (Fear Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6215);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6217 (Curse of Agony Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6217);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6226 (Drain Mana Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6226);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6254 (Chained Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6254);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6255 (Lizard Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6255);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6345 (Polymorphic Explosion ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6345);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6346 (Fear Ward ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6346);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6353 (Soul Fire Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6353);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6434 (Slice and Dice Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6434);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6546 (Rend Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6546);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6547 (Rend Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6547);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6548 (Rend Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6548);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6552 (Pummel Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6552);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6554 (Pummel Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6554);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6572 (Revenge Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6572);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6574 (Revenge Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6574);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6660 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6660);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6760 (Eviscerate Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6760);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6761 (Eviscerate Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6761);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6762 (Eviscerate Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6762);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6770 (Sap Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6770);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6774 (Slice and Dice Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6774);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6780 (Wrath Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6780);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6785 (Ravage Rank 1) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(6785);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 6787 (Ravage Rank 2) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(6787);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 6789 (Death Coil Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6789);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6798 (Bash Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6798);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6800 (Shred Rank 2) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(6800);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 6949 (Weak Frostbolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6949);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6950 (Faerie Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6950);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 6965 (Polymorphic Ray ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6965);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 7101 (Flame Blast Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7101);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;
   
	// Spell 5308 (Execute Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5308);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20658 (Execute Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20658);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;
	   
	// Spell 20660 (Execute Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20660);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20661 (Execute Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20661);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20662 (Execute Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20662);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

		// Spell 7160 (Execute ) is infront from spell extra.
		sp = dbcSpell.LookupEntryForced(7160);
		if( sp != NULL )
			sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 7269 (Arcane Missiles Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7269);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 7270 (Arcane Missiles Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7270);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 7288 (Immolate Cumulative (TEST) Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7288);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 7321 (Chilled Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7321);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 7322 (Frostbolt Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7322);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 7369 (Cleave Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7369);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 7372 (Hamstring Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7372);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 7373 (Hamstring Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7373);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 7379 (Revenge Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7379);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 7384 (Overpower Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7384);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 7386 (Sunder Armor Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7386);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 7400 (Mocking Blow Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7400);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 7402 (Mocking Blow Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7402);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 7405 (Sunder Armor Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7405);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 7588 (Void Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7588);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 7641 (Shadow Bolt Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7641);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 7646 (Curse of Weakness Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7646);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 7651 (Drain Life Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7651);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 7656 (Hex of Ravenclaw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7656);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 7657 (Hex of Ravenclaw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7657);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 7658 (Curse of Recklessness Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7658);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 7659 (Curse of Recklessness Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7659);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 7799 (Firebolt Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7799);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 7800 (Firebolt Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7800);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 7801 (Firebolt Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7801);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 7802 (Firebolt Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7802);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 7887 (Overpower Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7887);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 7978 (Throw Dynamite ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7978);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8012 (Purge Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8012);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8042 (Earth Shock Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8042);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8044 (Earth Shock Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8044);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8045 (Earth Shock Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8045);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8046 (Earth Shock Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8046);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8050 (Flame Shock Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8050);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8052 (Flame Shock Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8052);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8053 (Flame Shock Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8053);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8056 (Frost Shock Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8056);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8058 (Frost Shock Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8058);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8092 (Mind Blast Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8092);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8102 (Mind Blast Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8102);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8103 (Mind Blast Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8103);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8104 (Mind Blast Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8104);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8105 (Mind Blast Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8105);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8106 (Mind Blast Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8106);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8129 (Mana Burn Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8129);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8131 (Mana Burn Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8131);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8242 (Shield Slam ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8242);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8246 (Lightning Bolt Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8246);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8288 (Drain Soul Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8288);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8289 (Drain Soul Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8289);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8292 (Chain Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8292);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8380 (Sunder Armor Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8380);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8391 (Ravage ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8391);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8398 (Frostbolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8398);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8400 (Fireball Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8400);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8401 (Fireball Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8401);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8402 (Fireball Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8402);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8406 (Frostbolt Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8406);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8407 (Frostbolt Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8407);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8408 (Frostbolt Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8408);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8412 (Fire Blast Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8412);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8413 (Fire Blast Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8413);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8416 (Arcane Missiles Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8416);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8417 (Arcane Missiles Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8417);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8418 (Arcane Missiles Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8418);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8419 (Arcane Missiles Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8419);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8444 (Scorch Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8444);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8445 (Scorch Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8445);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8446 (Scorch Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8446);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8552 (Curse of Weakness ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8552);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8598 (Lightning Blast Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8598);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8621 (Sinister Strike Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8621);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8623 (Eviscerate Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8623);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8624 (Eviscerate Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8624);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8627 (Backstab (TEST) Rank 1) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(8627);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 8629 (Gouge Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8629);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8631 (Garrote Rank 2) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(8631);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 8632 (Garrote Rank 3) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(8632);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 8633 (Garrote Rank 4) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(8633);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 8639 (Rupture Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8639);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8640 (Rupture Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8640);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8643 (Kidney Shot Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8643);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8647 (Expose Armor Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8647);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8649 (Expose Armor Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8649);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8650 (Expose Armor Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8650);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8676 (Ambush Rank 1) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(8676);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 8721 (Backstab Rank 5) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(8721);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 8724 (Ambush Rank 2) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(8724);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 8725 (Ambush Rank 3) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(8725);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 8818 (Garrote Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8818);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8820 (Slam Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8820);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8905 (Wrath Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8905);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8921 (Moonfire Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8921);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8924 (Moonfire Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8924);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8925 (Moonfire Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8925);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8926 (Moonfire Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8926);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8927 (Moonfire Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8927);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8928 (Moonfire Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8928);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8929 (Moonfire Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8929);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8949 (Starfire Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8949);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8950 (Starfire Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8950);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8951 (Starfire Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8951);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8955 (Soothe Animal Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8955);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8983 (Bash Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8983);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8992 (Shred Rank 3) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(8992);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 8994 (Banish ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8994);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8995 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8995);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8996 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8996);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 8997 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8997);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9007 (Pounce Bleed Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9007);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9008 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9008);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9034 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9034);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9035 (Hex of Weakness Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9035);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9053 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9053);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9057 (Firebolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9057);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9080 (Hamstring ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9080);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9081 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9081);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9275 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9275);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9347 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9347);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9435 (Detonation ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9435);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9484 (Shackle Undead Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9484);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9485 (Shackle Undead Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9485);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9487 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9487);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9488 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9488);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9492 (Rip Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9492);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9493 (Rip Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9493);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9532 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9532);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9575 (Self Detonation ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9575);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9613 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9613);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9672 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9672);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9739 (Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9739);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9749 (Faerie Fire Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9749);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9752 (Rip Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9752);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9771 (Radiation Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9771);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9824 (Pounce Bleed Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9824);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9826 (Pounce Bleed Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9826);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9829 (Shred Rank 4) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(9829);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 9830 (Shred Rank 5) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(9830);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 9833 (Moonfire Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9833);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9834 (Moonfire Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9834);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9835 (Moonfire Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9835);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9849 (Claw Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9849);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9850 (Claw Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9850);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9866 (Ravage Rank 3) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(9866);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 9867 (Ravage Rank 4) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(9867);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 9875 (Starfire Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9875);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9876 (Starfire Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9876);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9894 (Rip Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9894);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9896 (Rip Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9896);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9901 (Soothe Animal Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9901);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9904 (Rake Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9904);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9907 (Faerie Fire Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9907);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 9912 (Wrath Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9912);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10148 (Fireball Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10148);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10149 (Fireball Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10149);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10150 (Fireball Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10150);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10151 (Fireball Rank 11) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10151);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10179 (Frostbolt Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10179);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10180 (Frostbolt Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10180);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10181 (Frostbolt Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10181);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10197 (Fire Blast Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10197);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10199 (Fire Blast Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10199);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10205 (Scorch Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10205);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10206 (Scorch Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10206);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10207 (Scorch Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10207);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;
	    
	// Spell 27073 (Scorch Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27073);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;
		
	// Spell 27074 (Scorch Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27074);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10211 (Arcane Missiles Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10211);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10212 (Arcane Missiles Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10212);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10273 (Arcane Missiles Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10273);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10274 (Arcane Missiles Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10274);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25345 (Arcane Missiles Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25345);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;
	      
	// Spell 27075 (Arcane Missiles Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27075);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;
	   
	// Spell 38699 (Arcane Missiles Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38699);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;
	   
	// Spell 38704 (Arcane Missiles Rank 11) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38704);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10277 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10277);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10308 (Hammer of Justice Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10308);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10312 (Exorcism Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10312);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10313 (Exorcism Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10313);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10314 (Exorcism Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10314);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10318 (Holy Wrath Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10318);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10371 (Slow ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10371);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10391 (Lightning Bolt Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10391);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10392 (Lightning Bolt Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10392);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10412 (Earth Shock Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10412);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10413 (Earth Shock Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10413);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10414 (Earth Shock Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10414);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10447 (Flame Shock Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10447);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10448 (Flame Shock Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10448);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10472 (Frost Shock Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10472);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10473 (Frost Shock Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10473);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10578 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10578);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10605 (Chain Lightning Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10605);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10617 (Release Rageclaw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10617);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10797 (Starshards Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10797);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10833 (Arcane Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10833);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10874 (Mana Burn Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10874);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10875 (Mana Burn Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10875);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10876 (Mana Burn Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10876);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10887 (Crowd Pummel Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10887);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10892 (Shadow Word: Pain Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10892);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_SKIPCHECK;

	// Spell 10893 (Shadow Word: Pain Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10893);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_SKIPCHECK;

	// Spell 10894 (Shadow Word: Pain Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10894);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_SKIPCHECK;

	// Spell 10933 (Smite Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10933);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10934 (Smite Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10934);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10945 (Mind Blast Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10945);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10946 (Mind Blast Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10946);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10947 (Mind Blast Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10947);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 10955 (Shackle Undead Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10955);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11085 (Chain Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11085);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11113 (Blast Wave Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11113);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11197 (Expose Armor Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11197);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11198 (Expose Armor Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11198);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11264 (Ice Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11264);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11267 (Ambush Rank 4) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(11267);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 11268 (Ambush Rank 5) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(11268);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 11269 (Ambush Rank 6) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(11269);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 11273 (Rupture Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11273);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11274 (Rupture Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11274);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11275 (Rupture Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11275);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11279 (Backstab Rank 6) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(11279);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 11280 (Backstab Rank 7) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(11280);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 11281 (Backstab Rank 8) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(11281);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 11285 (Gouge Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11285);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11286 (Gouge Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11286);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11289 (Garrote Rank 5) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(11289);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 11290 (Garrote Rank 6) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(11290);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 11293 (Sinister Strike Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11293);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11294 (Sinister Strike Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11294);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11297 (Sap Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11297);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11299 (Eviscerate Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11299);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11300 (Eviscerate Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11300);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11366 (Pyroblast Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11366);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11427 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11427);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11430 (Slam ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11430);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11436 (Slow ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11436);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11444 (Shackle Undead ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11444);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11538 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11538);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11564 (Heroic Strike Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11564);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11565 (Heroic Strike Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11565);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11566 (Heroic Strike Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11566);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11567 (Heroic Strike Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11567);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11572 (Rend Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11572);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11573 (Rend Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11573);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11574 (Rend Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11574);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11584 (Overpower Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11584);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11585 (Overpower Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11585);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11596 (Sunder Armor Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11596);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11597 (Sunder Armor Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11597);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11600 (Revenge Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11600);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11601 (Revenge Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11601);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11604 (Slam Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11604);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11605 (Slam Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11605);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11608 (Cleave Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11608);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11609 (Cleave Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11609);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11639 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11639);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11659 (Shadow Bolt Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11659);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11660 (Shadow Bolt Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11660);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11661 (Shadow Bolt Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11661);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11665 (Immolate Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11665);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11667 (Immolate Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11667);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11668 (Immolate Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11668);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11675 (Drain Soul Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11675);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11683 (Hellfire Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11683);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11684 (Hellfire Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11684);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11699 (Drain Life Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11699);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11700 (Drain Life Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11700);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11703 (Drain Mana Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11703);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11704 (Drain Mana Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11704);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11707 (Curse of Weakness Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11707);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11708 (Curse of Weakness Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11708);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11711 (Curse of Agony Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11711);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11712 (Curse of Agony Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11712);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11713 (Curse of Agony Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11713);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11717 (Curse of Recklessness Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11717);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11719 (Curse of Tongues Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11719);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11721 (Curse of the Elements Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11721);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11722 (Curse of the Elements Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11722);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11725 (Enslave Demon Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11725);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11726 (Enslave Demon Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11726);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11762 (Firebolt Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11762);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11763 (Firebolt Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11763);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11815 (Detonation ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11815);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11839 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11839);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11891 (Antu'sul Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11891);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11921 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11921);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11962 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11962);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11971 (Sunder Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11971);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11972 (Shield Bash ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11972);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11977 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11977);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11978 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11978);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11980 (Curse of Weakness ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11980);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11981 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11981);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11985 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11985);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11988 (Fireball Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11988);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 11989 (Fireball Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11989);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12054 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12054);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12058 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12058);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12096 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12096);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12167 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12167);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12170 (Revenge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12170);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12294 (Mortal Strike Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12294);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12331 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12331);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12355 (Impact Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12355);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12466 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12466);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12471 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12471);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12484 (Chilled Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12484);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12485 (Chilled Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12485);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12486 (Chilled Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12486);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12493 (Curse of Weakness ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12493);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12494 (Frostbite ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12494);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12505 (Pyroblast Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12505);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12522 (Pyroblast Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12522);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12523 (Pyroblast Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12523);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12524 (Pyroblast Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12524);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12525 (Pyroblast Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12525);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12526 (Pyroblast Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12526);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12538 (Ravenous Claw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12538);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12540 (Gouge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12540);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12542 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12542);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12548 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12548);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12555 (Pummel ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12555);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12675 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12675);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12693 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12693);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12733 (Fearless ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12733);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12737 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12737);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12739 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12739);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12741 (Curse of Weakness ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12741);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12742 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12742);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12745 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12745);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12824 (Polymorph Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12824);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12825 (Polymorph Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12825);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12826 (Polymorph Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12826);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12889 (Curse of Tongues ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12889);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 12964 (Unbridled Wrath Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12964);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13005 (Hammer of Justice ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13005);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13009 (Amnennar's Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13009);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13018 (Blast Wave Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13018);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13019 (Blast Wave Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13019);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13020 (Blast Wave Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13020);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13021 (Blast Wave Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13021);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13140 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13140);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13281 (Earth Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13281);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13318 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13318);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13321 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13321);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13322 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13322);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13323 (Polymorph ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13323);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13338 (Curse of Tongues ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13338);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13339 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13339);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13340 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13340);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13341 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13341);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13342 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13342);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13374 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13374);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13375 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13375);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13398 (Throw Wrench ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13398);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13424 (Faerie Fire Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13424);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13438 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13438);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13439 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13439);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13440 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13440);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13441 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13441);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13442 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13442);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13443 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13443);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13444 (Sunder Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13444);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13445 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13445);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13480 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13480);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13482 (Lightning Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13482);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13491 (Pummel Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13491);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13527 (Lightning Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13527);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13530 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13530);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13553 (Serpent Sting Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13553);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13554 (Serpent Sting Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13554);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13555 (Serpent Sting Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13555);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13579 (Gouge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13579);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13728 (Earth Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13728);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13729 (Flame Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13729);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13737 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13737);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13738 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13738);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13747 (Slow ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13747);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13748 (Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13748);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13752 (Faerie Fire Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13752);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13860 (Mind Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13860);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13878 (Scorch ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13878);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 13901 (Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13901);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14032 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14032);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14033 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14033);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14034 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14034);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14087 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14087);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14103 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14103);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14105 (Frost Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14105);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14106 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14106);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14109 (Lightning Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14109);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14118 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14118);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14119 (Lightning Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14119);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14122 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14122);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14145 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14145);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14200 (Chained Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14200);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14260 (Raptor Strike Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14260);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14261 (Raptor Strike Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14261);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14262 (Raptor Strike Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14262);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14263 (Raptor Strike Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14263);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14264 (Raptor Strike Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14264);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14265 (Raptor Strike Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14265);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14266 (Raptor Strike Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14266);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14267 (Wing Clip Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14267);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14268 (Wing Clip Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14268);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14269 (Mongoose Bite Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14269);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14270 (Mongoose Bite Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14270);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14271 (Mongoose Bite Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14271);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14279 (Viper Sting Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14279);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14280 (Viper Sting Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14280);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14281 (Arcane Shot Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14281);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14282 (Arcane Shot Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14282);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14283 (Arcane Shot Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14283);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14284 (Arcane Shot Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14284);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14285 (Arcane Shot Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14285);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14286 (Arcane Shot Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14286);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14287 (Arcane Shot Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14287);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14288 (Multi-Shot Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14288);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14289 (Multi-Shot Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14289);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14290 (Multi-Shot Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14290);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14294 (Volley Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14294);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14295 (Volley Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14295);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14296 (Black Arrow Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14296);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14323 (Hunter's Mark Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14323);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14324 (Hunter's Mark Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14324);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14325 (Hunter's Mark Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14325);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14443 (Multi-Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14443);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14621 (Polymorph ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14621);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14868 (Curse of Agony ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14868);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14871 (Shadow Bolt Misfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14871);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14873 (Sinister Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14873);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14874 (Rupture ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14874);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14875 (Curse of Agony ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14875);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14887 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14887);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14895 (Overpower ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14895);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14903 (Rupture ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14903);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 14914 (Holy Fire Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14914);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15039 (Flame Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15039);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15040 (Molten Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15040);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15043 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15043);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15089 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15089);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15091 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15091);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15095 (Molten Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15095);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15096 (Flame Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15096);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15117 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15117);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15124 (Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15124);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15207 (Lightning Bolt Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15207);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15208 (Lightning Bolt Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15208);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25448 (Lightning Bolt Rank 11) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25448);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25449 (Lightning Bolt Rank 12) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25449);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15228 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15228);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15230 (Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15230);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15232 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15232);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15234 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15234);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15241 (Scorch ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15241);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15242 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15242);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15243 (Fireball Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15243);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15245 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15245);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15254 (Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15254);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15261 (Holy Fire Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15261);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15262 (Holy Fire Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15262);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15263 (Holy Fire Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15263);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15264 (Holy Fire Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15264);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15265 (Holy Fire Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15265);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15266 (Holy Fire Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15266);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15267 (Holy Fire Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15267);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15280 (Cleave Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15280);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15284 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15284);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15285 (Fireball Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15285);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15305 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15305);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15407 (Mind Flay Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15407);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15451 (Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15451);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15470 (Curse of Tongues ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15470);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15472 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15472);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15496 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15496);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15497 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15497);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15499 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15499);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15501 (Earth Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15501);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15502 (Sunder Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15502);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15505 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15505);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15530 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15530);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15534 (Polymorph ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15534);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15536 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15536);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15537 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15537);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15547 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15547);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15549 (Chained Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15549);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15570 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15570);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15572 (Sunder Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15572);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15574 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15574);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15579 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15579);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15581 (Sinister Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15581);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15583 (Rupture ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15583);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15584 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15584);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15587 (Mind Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15587);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15592 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15592);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15598 (Firebolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15598);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15607 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15607);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15608 (Ravenous Claw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15608);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15610 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15610);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15611 (Lizard Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15611);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15612 (Lizard Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15612);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15613 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15613);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15614 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15614);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15615 (Pummel ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15615);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15616 (Flame Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15616);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15619 (Throw Wrench ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15619);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15620 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15620);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15622 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15622);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15623 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15623);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15654 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15654);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15655 (Shield Slam ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15655);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15659 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15659);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15661 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15661);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15662 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15662);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15663 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15663);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15665 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15665);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15667 (Sinister Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15667);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15691 (Eviscerate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15691);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15692 (Eviscerate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15692);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15708 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15708);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15732 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15732);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15735 (Arcane Missiles ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15735);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15736 (Arcane Missiles ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15736);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15744 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15744);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15754 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15754);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15785 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15785);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15790 (Arcane Missiles ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15790);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15791 (Arcane Missiles ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15791);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15795 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15795);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15798 (Moonfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15798);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15800 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15800);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15801 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15801);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15850 (Chilled ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15850);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15878 (Ice Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15878);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15979 (Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15979);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 15980 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15980);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16000 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16000);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16006 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16006);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16033 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16033);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16044 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16044);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16046 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16046);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16067 (Arcane Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16067);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16075 (Throw Axe ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16075);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16100 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16100);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16101 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16101);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16144 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16144);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16145 (Sunder Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16145);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16231 (Curse of Recklessness ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16231);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16249 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16249);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16250 (Fireball Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16250);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16375 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16375);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16393 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16393);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16402 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16402);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16403 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16403);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16406 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16406);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16407 (Frost Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16407);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16408 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16408);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16409 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16409);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16410 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16410);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16412 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16412);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16413 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16413);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16414 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16414);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16415 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16415);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16454 (Searing Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16454);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16496 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16496);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16498 (Faerie Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16498);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16509 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16509);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16511 (Hemorrhage Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16511);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16553 (Ravenous Claw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16553);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16554 (Toxic Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16554);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16559 (Flame Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16559);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16560 (Flame Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16560);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16568 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16568);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16570 (Charged Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16570);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16572 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16572);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16608 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16608);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16767 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16767);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16768 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16768);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16772 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16772);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16775 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16775);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16776 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16776);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16777 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16777);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16778 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16778);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16779 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16779);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16780 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16780);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16782 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16782);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16783 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16783);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16784 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16784);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16788 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16788);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16799 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16799);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16804 (Flame Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16804);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16827 (Claw Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16827);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16828 (Claw Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16828);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16829 (Claw Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16829);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16830 (Claw Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16830);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16831 (Claw Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16831);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16832 (Claw Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16832);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16856 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16856);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16857 (Faerie Fire (Feral) Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16857);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16921 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16921);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16922 (Starfire Stun ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16922);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 16927 (Chilled ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16927);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17008 (Drain Mana ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17008);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17011 (Freezing Claw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17011);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17140 (Holy Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17140);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17141 (Holy Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17141);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17142 (Holy Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17142);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17144 (Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17144);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17145 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17145);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17146 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17146);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17147 (Exorcism ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17147);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17149 (Exorcism ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17149);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17153 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17153);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17165 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17165);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17173 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17173);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17194 (Mind Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17194);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17195 (Scorch ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17195);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17198 (Overpower ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17198);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17203 (Fireball Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17203);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17227 (Curse of Weakness ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17227);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17228 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17228);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17238 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17238);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17243 (Drain Mana ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17243);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17253 (Bite Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17253);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17255 (Bite Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17255);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17256 (Bite Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17256);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17257 (Bite Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17257);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17258 (Bite Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17258);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17259 (Bite Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17259);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17260 (Bite Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17260);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17261 (Bite Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17261);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17273 (Pyroblast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17273);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17274 (Pyroblast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17274);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17277 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17277);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17287 (Mind Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17287);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17290 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17290);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17311 (Mind Flay Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17311);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17312 (Mind Flay Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17312);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17313 (Mind Flay Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17313);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17314 (Mind Flay Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17314);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17347 (Hemorrhage Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17347);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17348 (Hemorrhage Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17348);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17353 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17353);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17368 (Egan's Blaster ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17368);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17390 (Faerie Fire (Feral) Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17390);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17391 (Faerie Fire (Feral) Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17391);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17392 (Faerie Fire (Feral) Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17392);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17393 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17393);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17434 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17434);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17435 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17435);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17470 (Ravenous Claw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17470);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17483 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17483);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17503 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17503);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17504 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17504);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17509 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17509);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17510 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17510);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17547 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17547);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17615 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17615);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17620 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17620);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17630 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17630);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17668 (Ramstein's Lightning Bolts ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17668);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17669 (Ramstein's Lightning Bolts ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17669);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17682 (Drain Mana ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17682);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17685 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17685);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17748 (Kick Peon ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17748);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17771 (Curse of Agony ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17771);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17862 (Curse of Shadow Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17862);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17883 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17883);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17919 (Searing Pain Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17919);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17920 (Searing Pain Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17920);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17921 (Searing Pain Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17921);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17922 (Searing Pain Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17922);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17923 (Searing Pain Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17923);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17924 (Soul Fire Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17924);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17925 (Death Coil Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17925);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17926 (Death Coil Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17926);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17937 (Curse of Shadow Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17937);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 17962 (Conflagrate Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17962);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18075 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18075);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18078 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18078);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18081 (Lightning Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18081);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18082 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18082);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18083 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18083);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18084 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18084);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18085 (Frost Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18085);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18086 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18086);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18088 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18088);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18089 (Lightning Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18089);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18091 (Arcane Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18091);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18092 (Frost Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18092);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18101 (Chilled Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18101);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18104 (Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18104);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18105 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18105);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18106 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18106);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18108 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18108);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18111 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18111);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18112 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18112);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18137 (Shadowguard Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18137);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18138 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18138);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18164 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18164);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18165 (Holy Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18165);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18187 (Firebolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18187);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18199 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18199);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18200 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18200);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18202 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18202);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18204 (Frost Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18204);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18205 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18205);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18211 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18211);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18214 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18214);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18217 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18217);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18265 (Siphon Life Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18265);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18266 (Curse of Agony ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18266);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18267 (Curse of Weakness ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18267);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18276 (Frost Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18276);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18371 (Drain Soul ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18371);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18376 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18376);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18392 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18392);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18396 (Dismounting Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18396);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18398 (Frost Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18398);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18425 (Kick - Silenced ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18425);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18498 (Shield Bash - Silenced ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18498);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18557 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18557);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18561 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18561);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18647 (Banish Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18647);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18651 (Multi-Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18651);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18656 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18656);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18657 (Hibernate Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18657);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18658 (Hibernate Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18658);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18662 (Curse of Doom Effect ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18662);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18671 (Curse of Agony ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18671);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18796 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18796);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18807 (Mind Flay Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18807);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18809 (Pyroblast Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18809);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18817 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18817);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18833 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18833);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18879 (Siphon Life Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18879);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18880 (Siphon Life Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18880);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18881 (Siphon Life Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18881);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18930 (Conflagrate Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18930);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18931 (Conflagrate Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18931);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18932 (Conflagrate Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18932);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 18972 (Slow ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18972);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19032 (Throw Human Skull ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19032);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19130 (Revenge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19130);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19133 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19133);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19136 (Stormbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19136);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19137 (Slow ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19137);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19229 (Improved Wing Clip ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19229);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19249 (Touch of Weakness Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19249);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19251 (Touch of Weakness Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19251);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19252 (Touch of Weakness Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19252);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19253 (Touch of Weakness Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19253);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19254 (Touch of Weakness Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19254);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19260 (Frost Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19260);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19261 (Touch of Weakness Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19261);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19262 (Touch of Weakness Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19262);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19264 (Touch of Weakness Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19264);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19265 (Touch of Weakness Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19265);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19266 (Touch of Weakness Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19266);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19276 (Devouring Plague Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19276);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19277 (Devouring Plague Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19277);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19278 (Devouring Plague Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19278);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19279 (Devouring Plague Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19279);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19280 (Devouring Plague Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19280);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19281 (Hex of Weakness Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19281);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19282 (Hex of Weakness Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19282);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19283 (Hex of Weakness Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19283);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19284 (Hex of Weakness Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19284);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19285 (Hex of Weakness Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19285);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19296 (Starshards Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19296);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19299 (Starshards Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19299);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19302 (Starshards Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19302);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19303 (Starshards Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19303);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19304 (Starshards Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19304);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19305 (Starshards Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19305);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19306 (Counterattack Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19306);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19308 (Shadowguard Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19308);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19309 (Shadowguard Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19309);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19310 (Shadowguard Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19310);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19311 (Shadowguard Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19311);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19312 (Shadowguard Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19312);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19386 (Wyvern Sting Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19386);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19391 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19391);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19397 (Incinerate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19397);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19472 (Sinister Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19472);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19574 (Bestial Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19574);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19597 (Tame Ice Claw Bear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19597);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19632 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19632);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19639 (Pummel ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19639);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19642 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19642);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19643 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19643);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19728 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19728);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19729 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19729);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19776 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19776);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19785 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19785);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19816 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19816);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19820 (Mangle ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19820);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19874 (Lightning Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19874);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19881 (Shoot (TEST) ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19881);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 19983 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19983);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20005 (Chilled ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20005);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20184 (Judgement of Justice Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20184);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20185 (Judgement of Light Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20185);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20186 (Judgement of Wisdom Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20186);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20187 (Judgement of Righteousness Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20187);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20188 (Judgement of the Crusader Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20188);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20228 (Pyroblast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20228);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20229 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20229);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20243 (Devastate Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20243);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20253 (Intercept Stun Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20253);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20267 (Judgement of Light Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20267);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20268 (Judgement of Wisdom Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20268);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20271 (Judgement ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20271);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20280 (Judgement of Righteousness Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20280);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20281 (Judgement of Righteousness Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20281);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20282 (Judgement of Righteousness Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20282);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20283 (Judgement of Righteousness Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20283);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20284 (Judgement of Righteousness Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20284);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20285 (Judgement of Righteousness Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20285);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20286 (Judgement of Righteousness Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20286);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20294 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20294);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20295 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20295);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20297 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20297);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20298 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20298);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20300 (Judgement of the Crusader Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20300);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20301 (Judgement of the Crusader Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20301);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20302 (Judgement of the Crusader Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20302);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20303 (Judgement of the Crusader Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20303);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20341 (Judgement of Light Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20341);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20342 (Judgement of Light Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20342);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20343 (Judgement of Light Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20343);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20344 (Judgement of Light Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20344);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20345 (Judgement of Light Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20345);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20346 (Judgement of Light Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20346);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20352 (Judgement of Wisdom Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20352);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20353 (Judgement of Wisdom Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20353);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20354 (Judgement of Wisdom Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20354);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20355 (Judgement of Wisdom Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20355);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20420 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20420);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20425 (Judgement of Command Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20425);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20463 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20463);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20467 (Judgement of Command Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20467);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20473 (Holy Shock Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20473);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20508 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20508);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20559 (Mocking Blow Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20559);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20560 (Mocking Blow Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20560);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20565 (Magma Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20565);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20566 (Wrath of Ragnaros ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20566);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20569 (Cleave Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20569);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20605 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20605);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20614 (Intercept Stun Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20614);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20615 (Intercept Stun Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20615);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20623 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20623);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20647 (Execute ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20647);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20656 (Faerie Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20656);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20666 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20666);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20677 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20677);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20678 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20678);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20679 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20679);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20684 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20684);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20685 (Storm Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20685);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20690 (Moonfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20690);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20691 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20691);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20692 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20692);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20698 (Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20698);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20714 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20714);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20720 (Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20720);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20733 (Black Arrow ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20733);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20735 (Multi-Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20735);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20741 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20741);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20743 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20743);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20787 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20787);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20791 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20791);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20792 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20792);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20793 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20793);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20795 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20795);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20797 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20797);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20800 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20800);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20801 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20801);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20802 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20802);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20805 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20805);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20806 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20806);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20807 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20807);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20808 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20808);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20811 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20811);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20815 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20815);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20816 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20816);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20817 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20817);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20819 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20819);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20822 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20822);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20823 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20823);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20824 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20824);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20825 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20825);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20826 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20826);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20829 (Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20829);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20830 (Mind Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20830);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20831 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20831);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20832 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20832);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20869 (Frost Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20869);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20882 (Enslave Demon ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20882);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20883 (Arcane Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20883);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20909 (Counterattack Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20909);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20910 (Counterattack Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20910);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20929 (Holy Shock Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20929);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20930 (Holy Shock Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20930);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20961 (Judgement of Command Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20961);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20962 (Judgement of Command Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20962);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20963 (Judgement of Command Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20963);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20964 (Judgement of Command Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20964);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20965 (Judgement of Command Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20965);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20966 (Judgement of Command Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20966);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20967 (Judgement of Command Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20967);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 20968 (Judgement of Command Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20968);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21007 (Curse of Weakness ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21007);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21008 (Mocking Blow ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21008);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21030 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21030);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21052 (Monty Bashes Rats (DND) ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21052);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21066 (Void Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21066);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21067 (Poison Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21067);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21068 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21068);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21072 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21072);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21077 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21077);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21081 (Sunder Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21081);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21141 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21141);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21159 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21159);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21162 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21162);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21170 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21170);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21179 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21179);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21183 (Judgement of the Crusader Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21183);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21230 (Fireball, Improved DoT ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21230);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21341 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21341);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21369 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21369);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21390 (Multi-Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21390);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21401 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21401);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21402 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21402);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21549 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21549);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21551 (Mortal Strike Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21551);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21552 (Mortal Strike Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21552);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21553 (Mortal Strike Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21553);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21667 (Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21667);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21668 (Starfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21668);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21669 (Moonfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21669);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21670 (Faerie Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21670);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21807 (Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21807);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21887 (Warrior's Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21887);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21889 (Judgement Smite ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21889);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21949 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21949);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21971 (Poison Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21971);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21977 (Warrior's Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21977);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 21987 (Lash of Pain ) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(21987);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 22088 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22088);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22120 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22120);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22121 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22121);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22189 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22189);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22206 (Moonfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22206);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22272 (Arcane Missiles ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22272);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22273 (Arcane Missiles ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22273);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22336 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22336);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22355 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22355);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22356 (Slow ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22356);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22357 (Icebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22357);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22411 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22411);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22414 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22414);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22423 (Flame Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22423);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22424 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22424);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22425 (Fireball Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22425);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22438 (Mark of Detonation ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22438);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22439 (Mark of Detonation ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22439);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22540 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22540);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22568 (Ferocious Bite Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22568);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22570 (Maim Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22570);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22582 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22582);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22639 (Eskhandar's Rake ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22639);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22643 (Frostbolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22643);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22665 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22665);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22677 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22677);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22678 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22678);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22689 (Mangle ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22689);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22709 (Void Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22709);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22822 (Starshards ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22822);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22823 (Starshards ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22823);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22827 (Ferocious Bite Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22827);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22828 (Ferocious Bite Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22828);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22829 (Ferocious Bite Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22829);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22851 (Ferocious Bite Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22851);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22853 (Ferocious Bite Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22853);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22861 (Ferocious Bite Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22861);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22862 (Ferocious Bite Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22862);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22878 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22878);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22885 (Earth Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22885);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22887 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22887);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22893 (Arcane Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22893);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22907 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22907);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22908 (Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22908);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22911 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22911);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22919 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22919);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22920 (Arcane Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22920);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22936 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22936);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22937 (Poison Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22937);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22940 (Arcane Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22940);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 22947 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22947);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23024 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23024);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23038 (Flame Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23038);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23039 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23039);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23073 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23073);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23102 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23102);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23106 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23106);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23113 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23113);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23114 (Earth Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23114);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23115 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23115);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23206 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23206);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23267 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23267);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23268 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23268);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23269 (Holy Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23269);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23308 (Incinerate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23308);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23309 (Incinerate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23309);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23331 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23331);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23337 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23337);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23380 (Moonfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23380);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23411 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23411);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23412 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23412);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23512 (Fireball Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23512);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23590 (Judgement ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23590);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23592 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23592);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23642 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23642);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23722 (Arcane Detonation ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23722);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23860 (Holy Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23860);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23880 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23880);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23881 (Bloodthirst Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23881);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23885 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23885);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23886 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23886);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23887 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23887);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23888 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23888);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23889 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23889);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23890 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23890);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23891 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23891);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23892 (Bloodthirst Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23892);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23893 (Bloodthirst Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23893);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23894 (Bloodthirst Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23894);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23922 (Shield Slam Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23922);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23923 (Shield Slam Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23923);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23924 (Shield Slam Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23924);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23925 (Shield Slam Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23925);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23952 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23952);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23953 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23953);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23959 (Test Stab R50 Rank 8) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(23959);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 23968 (Throw Liquid Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23968);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23969 (Throw Liquid Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23969);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23970 (Throw Liquid Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23970);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 23979 (Holy Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23979);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24099 (Poison Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24099);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24131 (Wyvern Sting Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24131);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24132 (Wyvern Sting Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24132);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24133 (Wyvern Sting Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24133);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24134 (Wyvern Sting Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24134);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24135 (Wyvern Sting Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24135);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24187 (Claw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24187);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24193 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24193);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24212 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24212);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24213 (Ravage ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24213);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24239 (Hammer of Wrath Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24239);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24274 (Hammer of Wrath Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24274);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24275 (Hammer of Wrath Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24275);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24300 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24300);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24317 (Sunder Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24317);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24331 (Rake ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24331);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24332 (Rake ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24332);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24333 (Ravage ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24333);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24335 (Wyvern Sting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24335);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24336 (Wyvern Sting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24336);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24374 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24374);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24395 (Bestial Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24395);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24396 (Bestial Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24396);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24397 (Bestial Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24397);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24407 (Overpower ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24407);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24408 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24408);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24415 (Slow ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24415);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24435 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24435);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24466 (Banish ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24466);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24530 (Felfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24530);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24573 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24573);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24585 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24585);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24611 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24611);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24618 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24618);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24668 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24668);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24680 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24680);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24685 (Earth Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24685);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24698 (Gouge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24698);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24816 (Test Mortal Strike Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24816);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24817 (Test Overpower Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24817);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24820 (Shadow Bolt Whirl ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24820);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24821 (Shadow Bolt Whirl ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24821);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24822 (Shadow Bolt Whirl ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24822);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24823 (Shadow Bolt Whirl ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24823);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24824 (Test Sinister Strike Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24824);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24825 (Test Backstab Rank 8) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(24825);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 24834 (Shadow Bolt Whirl ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24834);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24835 (Shadow Bolt Whirl ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24835);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24836 (Shadow Bolt Whirl ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24836);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24837 (Shadow Bolt Whirl ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24837);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24838 (Shadow Bolt Whirl ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24838);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24857 (Arcane Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24857);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24910 (Corruption of the Earth ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24910);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24942 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24942);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24957 (Moonfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24957);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24974 (Insect Swarm Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24974);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24975 (Insect Swarm Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24975);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24976 (Insect Swarm Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24976);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24977 (Insect Swarm Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24977);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24993 (Acid Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24993);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 24995 (Pyroblast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24995);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25004 (Throw Nightmare Object ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25004);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25021 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25021);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25025 (Earth Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25025);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25028 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25028);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25030 (Shoot Rocket ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25030);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25031 (Shoot Missile ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25031);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25049 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25049);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25051 (Sunder Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25051);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25055 (Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25055);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25099 (Detonation ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25099);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25195 (Curse of Tongues ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25195);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25208 (Rend Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25208);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25212 (Hamstring Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25212);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25225 (Sunder Armor Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25225);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25231 (Cleave Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25231);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25234 (Execute Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25234);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25236 (Execute Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25236);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25241 (Slam Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25241);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25242 (Slam Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25242);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25248 (Mortal Strike Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25248);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25251 (Bloodthirst Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25251);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25252 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25252);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25253 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25253);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25258 (Shield Slam Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25258);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25266 (Mocking Blow Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25266);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25269 (Revenge Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25269);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25273 (Intercept Stun Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25273);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25274 (Intercept Stun Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25274);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25286 (Heroic Strike Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25286);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25288 (Revenge Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25288);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25294 (Multi-Shot Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25294);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25295 (Serpent Sting Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25295);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25298 (Starfire Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25298);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26986 (Starfire Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26986);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25300 (Backstab Rank 9) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(25300);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 25304 (Frostbolt Rank 11) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25304);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27071 (Frostbolt Rank 12) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27071);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;
	      
	// Spell 27072 (Frostbolt Rank 13) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27072);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;
	      
	// Spell 38697 (Frostbolt Rank 14) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38697);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25306 (Fireball Rank 12) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25306);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27070 (Fireball Rank 13) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27070);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;
	   
	// Spell 38692 (Fireball Rank 14) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38692);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25307 (Shadow Bolt Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25307);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27209 (Shadow Bolt Rank 11) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27209);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25309 (Immolate Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25309);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25345 (Arcane Missiles Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25345);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25346 (Arcane Missiles Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25346);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25363 (Smite Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25363);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25364 (Smite Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25364);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25367 (Shadow Word: Pain Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25367);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_SKIPCHECK;

	// Spell 25368 (Shadow Word: Pain Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25368);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_SKIPCHECK;

	// Spell 25372 (Mind Blast Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25372);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25375 (Mind Blast Rank 11) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25375);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25379 (Mana Burn Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25379);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25380 (Mana Burn Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25380);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25384 (Holy Fire Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25384);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25387 (Mind Flay Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25387);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25424 (Poison Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25424);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25439 (Chain Lightning Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25439);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25442 (Chain Lightning Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25442);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25446 (Starshards Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25446);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25454 (Earth Shock Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25454);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25457 (Flame Shock Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25457);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25460 (Touch of Weakness Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25460);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25461 (Touch of Weakness Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25461);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25464 (Frost Shock Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25464);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25467 (Devouring Plague Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25467);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25470 (Hex of Weakness Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25470);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25477 (Shadowguard Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25477);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25515 (Bash ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25515);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25586 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25586);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25595 (Savage Pummel Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25595);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25671 (Drain Mana ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25671);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25710 (Heroic Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25710);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25712 (Heroic Strike Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25712);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25755 (Drain Mana ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25755);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25756 (Purge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25756);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25759 (Video Camera 1 ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25759);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25760 (Video Camera 2 ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25760);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25761 (Video Camera 3 ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25761);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25762 (Video Camera 4 ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25762);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25779 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25779);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25805 (Soul Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25805);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25811 (Poison Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25811);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25816 (Hex of Weakness Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25816);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25821 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25821);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25902 (Holy Shock Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25902);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25903 (Holy Shock Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25903);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25907 (Spell Blasting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25907);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25911 (Holy Shock Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25911);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25912 (Holy Shock Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25912);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25913 (Holy Shock Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25913);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25914 (Holy Shock Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25914);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 25991 (Poison Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25991);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26006 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26006);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26044 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26044);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26046 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26046);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26048 (Mind Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26048);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26049 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26049);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26052 (Poison Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26052);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26070 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26070);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26098 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26098);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26102 (Sand Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26102);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26121 (Stormcaller's Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26121);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26141 (Hamstring ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26141);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26143 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26143);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26180 (Wyvern Sting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26180);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26194 (Earth Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26194);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26211 (Hamstring ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26211);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26233 (Wyvern Sting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26233);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26282 (Shoot Tauren Rifleman ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26282);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26350 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26350);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26385 (Shadowburn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26385);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26458 (Shock Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26458);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26580 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26580);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26592 (Bestial Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26592);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26601 (Poison Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26601);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26639 (Drain Mana ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26639);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26661 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26661);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26679 (Deadly Throw Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26679);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26693 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26693);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26748 (Wyvern Sting Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26748);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26839 (Garrote Rank 7) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(26839);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 26861 (Sinister Strike Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26861);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26862 (Sinister Strike Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26862);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26863 (Backstab Rank 10) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(26863);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 26864 (Hemorrhage Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26864);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26865 (Eviscerate Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26865);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26866 (Expose Armor Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26866);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26867 (Rupture Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26867);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26884 (Garrote Rank 8) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(26884);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 27001 (Shred Rank 6) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(27001);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 27002 (Shred Rank 7) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(27002);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 27005 (Ravage Rank 5) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(27005);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 27014 (Raptor Strike Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27014);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27016 (Serpent Sting Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27016);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27018 (Viper Sting Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27018);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27019 (Arcane Shot Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27019);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27021 (Multi-Shot Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27021);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27022 (Volley Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27022);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27067 (Counterattack Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27067);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27068 (Wyvern Sting Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27068);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27069 (Wyvern Sting Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27069);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27138 (Exorcism Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27138);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27174 (Holy Shock Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27174);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27175 (Holy Shock Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27175);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27176 (Holy Shock Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27176);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27180 (Hammer of Wrath Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27180);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27210 (Searing Pain Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27210);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27211 (Soul Fire Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27211);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27213 (Hellfire Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27213);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27215 (Immolate Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27215);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27217 (Drain Soul Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27217);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27218 (Curse of Agony Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27218);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27219 (Drain Life Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27219);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27220 (Drain Life Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27220);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27221 (Drain Mana Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27221);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27223 (Death Coil Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27223);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27224 (Curse of Weakness Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27224);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27226 (Curse of Recklessness Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27226);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27228 (Curse of the Elements Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27228);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27229 (Curse of Shadow Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27229);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27243 (Seed of Corruption Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27243);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27264 (Siphon Life Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27264);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27285 (Seed of Corruption Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27285);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27286 (Shadow Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27286);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27382 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27382);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27383 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27383);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27441 (Ambush Rank 7) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(27441);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 27499 (Crusader's Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27499);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27555 (Shred ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27555);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27556 (Rake ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27556);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27557 (Ferocious Bite ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27557);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27565 (Banish ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27565);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27567 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27567);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27580 (Mortal Strike Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27580);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27584 (Hamstring Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27584);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27605 (Shadow Word: Pain Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27605);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_SKIPCHECK;

	// Spell 27611 (Eviscerate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27611);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27613 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27613);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27615 (Kidney Shot Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27615);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27626 (Purge Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27626);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27633 (Wing Clip Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27633);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27636 (Starshards ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27636);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27638 (Rake ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27638);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27641 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27641);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27646 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27646);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27662 (Throw Cupid's Dart ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27662);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27737 (Moonfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27737);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27760 (Polymorph ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27760);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27794 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27794);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27808 (Frost Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27808);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27812 (Void Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27812);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27814 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27814);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27820 (Mana Detonation ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27820);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27826 (Intercept ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27826);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27831 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27831);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27990 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27990);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27991 (Sunder Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27991);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27992 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27992);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 27994 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27994);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 28167 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28167);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 28271 (Polymorph Rank 1: Turtle) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28271);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 28272 (Polymorph Rank 1: Pig) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28272);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 28293 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28293);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 28301 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28301);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 28310 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28310);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 28311 (Slime Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28311);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 28377 (Shadowguard Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28377);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 28378 (Shadowguard Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28378);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 28379 (Shadowguard Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28379);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 28380 (Shadowguard Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28380);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 28381 (Shadowguard Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28381);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 28382 (Shadowguard Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28382);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 28385 (Shadowguard Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28385);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 28407 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28407);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 28412 (Death Coil ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28412);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 28448 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28448);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 28456 (Gouge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28456);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 28457 (Dark Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28457);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 28478 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28478);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 28479 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28479);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 28522 (Icebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28522);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 28526 (Icebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28526);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 28598 (Touch of Weakness ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28598);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 28599 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28599);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 28608 (Test Curse of Agony Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28608);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 28750 (Blessing of the Claw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28750);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 28796 (Poison Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28796);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 28883 (Holy Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28883);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 28900 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28900);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29058 (Drain Mana ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29058);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29124 (Polymorph ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29124);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29155 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29155);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29168 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29168);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29183 (Polymorph Immunity ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29183);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29214 (Wrath of the Plaguebringer ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29214);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29228 (Flame Shock Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29228);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29310 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29310);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29317 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29317);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29320 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29320);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29321 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29321);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29341 (Shadowburn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29341);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29405 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29405);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29407 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29407);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29425 (Gouge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29425);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29426 (Heroic Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29426);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29492 (Searing Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29492);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29515 (TEST Scorch ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29515);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29522 (Holy Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29522);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29560 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29560);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29561 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29561);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29563 (Holy Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29563);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29567 (Heroic Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29567);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29570 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29570);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29572 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29572);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29574 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29574);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29576 (Multi-Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29576);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29578 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29578);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29579 (Throw Dynamite ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29579);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29582 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29582);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29586 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29586);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29640 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29640);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29644 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29644);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29665 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29665);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29666 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29666);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29667 (Hamstring ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29667);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29684 (Shield Slam ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29684);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29707 (Heroic Strike Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29707);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29722 (Incinerate Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29722);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29848 (Polymorph ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29848);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29881 (Drain Mana ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29881);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29928 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29928);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 29930 (Curse of Agony ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29930);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30002 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30002);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30014 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30014);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30016 (Devastate Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30016);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30017 (Devastate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30017);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30022 (Devastate Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30022);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30108 (Unstable Affliction Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30108);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30131 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30131);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30153 (Intercept Stun Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30153);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30195 (Intercept Stun Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30195);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30197 (Intercept Stun Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30197);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30213 (Cleave Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30213);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30219 (Cleave Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30219);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30223 (Cleave Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30223);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30231 (Banish ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30231);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30324 (Heroic Strike Rank 11) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30324);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30330 (Mortal Strike Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30330);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30335 (Bloodthirst Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30335);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30339 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30339);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30340 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30340);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30356 (Shield Slam Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30356);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30357 (Revenge Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30357);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30358 (Searing Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30358);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30404 (Unstable Affliction Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30404);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30405 (Unstable Affliction Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30405);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30412 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30412);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30459 (Searing Pain Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30459);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30460 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30460);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30470 (Slice and Dice ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30470);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30475 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30475);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30476 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30476);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30478 (Hemorrhage ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30478);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30482 (Molten Armor Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30482);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30493 (Shoot Bow ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30493);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30500 (Death Coil ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30500);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30530 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30530);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30545 (Soul Fire Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30545);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30584 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30584);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30615 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30615);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30619 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30619);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30621 (Kidney Shot Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30621);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30688 (Shield Slam ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30688);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30741 (Death Coil ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30741);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30832 (Kidney Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30832);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30846 (Throw Proximity Bomb ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30846);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30854 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30854);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30859 (Hellfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30859);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30898 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30898);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30901 (Sunder Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30901);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30908 (Drain Mana Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30908);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30909 (Curse of Weakness Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30909);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30910 (Curse of Doom Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30910);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30911 (Siphon Life Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30911);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30933 (Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30933);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30938 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30938);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30980 (Sap ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30980);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30989 (Hamstring ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30989);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 30990 (Multi-Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30990);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 31016 (Eviscerate Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31016);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 31018 (Ferocious Bite Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31018);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 31043 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31043);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 31117 (Unstable Affliction ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31117);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 31312 (Drunken Avenger's Shield ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31312);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 31330 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31330);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 31345 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31345);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 31358 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31358);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 31378 (Worm Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31378);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 31402 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31402);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 31405 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31405);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 31407 (Viper Sting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31407);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 31516 (Mind Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31516);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 31553 (Hamstring ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31553);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 31566 (Raptor Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31566);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 31615 (Hunter's Mark ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31615);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 31717 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31717);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 31729 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31729);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 31733 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31733);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 31751 (Arcane Missiles ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31751);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 31779 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31779);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 31827 (Heroic Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31827);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 31911 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31911);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 31935 (Avenger's Shield Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31935);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 31942 (Multi-Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31942);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 31946 (Throw Freezing Trap ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31946);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 31970 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31970);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 31975 (Serpent Sting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31975);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 31997 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31997);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 31998 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31998);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32001 (Throw Gordawg's Boulder ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32001);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32063 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32063);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32105 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32105);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32154 (Overpower ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32154);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32197 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32197);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32231 (Incinerate Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32231);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32241 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32241);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32323 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32323);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32337 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32337);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32379 (Shadow Word: Death Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32379);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32409 (Shadow Word: Death Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32409);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32416 (Hammer of Justice ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32416);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32417 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32417);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32418 (Curse of Agony ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32418);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32554 (Drain Mana ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32554);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32645 (Envenom Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32645);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32674 (Avenger's Shield ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32674);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32684 (Envenom Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32684);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32699 (Avenger's Shield Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32699);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32700 (Avenger's Shield Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32700);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32707 (Incinerate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32707);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32709 (Death Coil ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32709);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32736 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32736);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32771 (Holy Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32771);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32772 (Hammer of Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32772);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32774 (Avenger's Shield ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32774);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32784 (Throw Proximity Bomb ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32784);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32861 (Shadowguard ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32861);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32862 (Drain Soul ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32862);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32863 (Seed of Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32863);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32864 (Kidney Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32864);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32865 (Seed of Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32865);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32908 (Wing Clip ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32908);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32915 (Raptor Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32915);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32967 (Flame Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32967);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 32996 (Shadow Word: Death Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32996);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 33070 (Cloud of Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33070);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 33072 (Holy Shock Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33072);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 33073 (Holy Shock Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33073);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 33074 (Holy Shock Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33074);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 33130 (Death Coil ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33130);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 33385 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33385);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 33424 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33424);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 33547 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33547);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 33625 (Purge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33625);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 33632 (Exorcism ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33632);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 33643 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33643);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 33670 (Throw Explosives ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33670);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 33709 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33709);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 33745 (Lacerate Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33745);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 33805 (Throw Hammer Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33805);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 33806 (Throw Hammer Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33806);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 33808 (Shoot Gun Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33808);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 33822 (Throw Hammer Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33822);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 33871 (Shield Bash ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33871);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 33924 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33924);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34076 (Fear the Worgs ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34076);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34095 (Throw Proximity Bomb ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34095);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34100 (Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34100);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34107 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34107);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34259 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34259);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34353 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34353);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34354 (Flame Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34354);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34389 (Throw Doomhammer ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34389);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34411 (Mutilate Rank 2) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(34411);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 34412 (Mutilate Rank 3) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(34412);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 34413 (Mutilate Rank 4) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(34413);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;

	// Spell 34428 (Victory Rush Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34428);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34437 (Death Coil ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34437);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34438 (Unstable Affliction ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34438);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34439 (Unstable Affliction ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34439);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34441 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34441);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34620 (Slam ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34620);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34659 (Hellfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34659);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34660 (Hellfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34660);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34696 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34696);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34802 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34802);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34806 (Drain Mana Visual ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34806);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34829 (Arcane Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34829);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34832 (Hunter's Mark Visual Only Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34832);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34846 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34846);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34879 (Multi-Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34879);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34913 (Molten Armor Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34913);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34914 (Vampiric Touch Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34914);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34916 (Vampiric Touch Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34916);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34917 (Vampiric Touch Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34917);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34919 (Vampiric Touch ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34919);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34930 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34930);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34931 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34931);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34940 (Gouge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34940);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34941 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34941);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34942 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34942);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34974 (Multi-Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34974);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 34995 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34995);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 35054 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35054);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 35123 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35123);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 35125 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35125);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 35139 (Throw Boom's Doom ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35139);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 35155 (Smite ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35155);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 35160 (Holy Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35160);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 35178 (Shield Bash ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35178);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 35182 (Banish ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35182);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 35183 (Unstable Affliction ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35183);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 35195 (Siphon Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35195);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 35276 (Throw Dynamite ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35276);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 35332 (Drain Mana ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35332);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 35401 (Arcane Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35401);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 35412 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35412);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 35507 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35507);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 35511 (Serpent Sting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35511);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 35570 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35570);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 35724 (Throw Torch ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35724);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 35748 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35748);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 35754 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35754);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 35839 (Drain Soul ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35839);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 35851 (Test Sunder Armor Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35851);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 35915 (Molten Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35915);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 35916 (Molten Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35916);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 35947 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35947);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 35948 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35948);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 35950 (Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35950);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 35954 (Death Coil ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35954);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 35963 (Improved Wing Clip ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35963);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36033 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36033);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36058 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36058);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36088 (Drain Mana ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36088);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36095 (Drain Mana ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36095);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36123 (Seed of Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36123);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36140 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36140);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36224 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36224);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36293 (Arcane Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36293);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36340 (Holy Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36340);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36470 (Pummel ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36470);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36484 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36484);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36509 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36509);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36608 (Charged Arcane Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36608);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36609 (Arcane Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36609);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36623 (Arcane Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36623);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36638 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36638);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36645 (Throw Rock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36645);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36651 (Throw Rock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36651);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36655 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36655);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36825 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36825);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36831 (Curse of the Elements ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36831);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36832 (Incinerate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36832);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36862 (Gouge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36862);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36891 (Devastate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36891);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36894 (Devastate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36894);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36896 (Chain Lightning 02 ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36896);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36916 (Mongoose Bite Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36916);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36947 (Holy Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36947);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36965 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36965);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36979 (Multi-Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36979);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36984 (Serpent Sting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36984);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36988 (Shield Bash ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36988);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 36991 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36991);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37066 (Garrote ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37066);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37074 (Deadly Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37074);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37113 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37113);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37159 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37159);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37176 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37176);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37251 (Hammer of Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37251);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37255 (Hammer of Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37255);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37259 (Hammer of Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37259);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37275 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37275);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37276 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37276);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37321 (Overpower ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37321);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37330 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37330);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37331 (Hemorrhage ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37331);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37332 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37332);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37334 (Curse of Agony ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37334);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37335 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37335);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37369 (Hammer of Justice ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37369);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37428 (Hellfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37428);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37448 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37448);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37476 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37476);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37511 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37511);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37517 (Revenge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37517);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37527 (Banish ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37527);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37529 (Overpower ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37529);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37531 (Mind Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37531);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37546 (Banish ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37546);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37551 (Viper Sting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37551);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37554 (Avenger's Shield ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37554);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37598 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37598);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37621 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37621);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37628 (Fel Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37628);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37662 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37662);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37668 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37668);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37790 (Spread Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37790);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37826 (Seed of Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37826);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37833 (Banish ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37833);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37865 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37865);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37961 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37961);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 37992 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37992);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38009 (Banish ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38009);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38065 (Death Coil ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38065);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38154 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38154);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38219 (Mark of Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38219);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38220 (Mark of Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38220);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38221 (Mark of Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38221);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38222 (Mark of Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38222);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38230 (Mark of Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38230);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38233 (Shield Bash ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38233);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38234 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38234);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38243 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38243);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38252 (Seed of Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38252);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38259 (Mind Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38259);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38260 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38260);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38262 (Hamstring ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38262);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38310 (Multi-Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38310);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38313 (Pummel ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38313);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38376 (Banish ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38376);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38379 (Shadowguard ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38379);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38383 (Multi-Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38383);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38401 (Incinerate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38401);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38461 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38461);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38474 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38474);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38556 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38556);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38557 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38557);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38558 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38558);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38559 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38559);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38560 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38560);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38561 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38561);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38562 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38562);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38563 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38563);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38564 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38564);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38565 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38565);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38566 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38566);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38567 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38567);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38568 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38568);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38569 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38569);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38570 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38570);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38585 (Holy Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38585);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38595 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38595);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38625 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38625);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38631 (Avenger's Shield ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38631);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38660 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38660);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38764 (Gouge Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38764);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38768 (Kick Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38768);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38791 (Banish ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38791);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38806 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38806);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38807 (Arcane Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38807);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38808 (Charged Arcane Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38808);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38817 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38817);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38849 (Devastate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38849);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38859 (Serpent Sting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38859);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38863 (Gouge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38863);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38883 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38883);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38884 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38884);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38904 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38904);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38914 (Serpent Sting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38914);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38918 (Incinerate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38918);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38921 (Holy Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38921);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38959 (Execute ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38959);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38967 (Devastate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38967);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 38995 (Hamstring ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38995);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39020 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39020);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39043 (Kick Akuno ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39043);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39047 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39047);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39060 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39060);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39062 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39062);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39066 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39066);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39071 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39071);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39072 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39072);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39077 (Hammer of Justice ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39077);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39083 (Incinerate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39083);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39119 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39119);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39131 (Hellfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39131);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39132 (Hellfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39132);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39136 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39136);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39146 (Throw Orange Juice Bottle ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39146);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39147 (Throw Tar Bottle ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39147);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39148 (Throw Blood Bottle ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39148);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39149 (Throw Frosty Bottle ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39149);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39150 (Throw Random Bottle ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39150);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39151 (Throw 15 Bottles ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39151);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39154 (Throw Acid Bottle ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39154);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39155 (Throw Grape Juice Bottle ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39155);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39156 (Throw Random Bottle ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39156);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39171 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39171);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39174 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39174);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39176 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39176);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39182 (Serpent Sting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39182);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39210 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39210);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39212 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39212);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39262 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39262);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39323 (Holy Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39323);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39367 (Seed of Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39367);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39413 (Viper Sting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39413);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39415 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39415);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 39416 (Blizzard ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39416);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26987 (Moonfire Rank 11) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26987);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;

	// Spell 26988 (Moonfire Rank 12) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26988);
	if( sp != NULL )
		sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INFRONT;
}

void Apply112SpellFixes()
{
	SpellEntry * sp;
	// Spell 1455 Proc Chance (Life Tap Rank 2)
	sp = dbcSpell.LookupEntryForced(1455);
	if(sp != NULL)
		sp->procChance = 100;

	// Spell 1456 Proc Chance (Life Tap Rank 3)
	sp = dbcSpell.LookupEntryForced(1456);
	if(sp != NULL)
		sp->procChance = 100;

	// Spell 3391 Proc Chance (Thrash )
	sp = dbcSpell.LookupEntryForced(3391);
	if(sp != NULL)
		sp->procChance = 10;

	// Spell 8033 Proc Chance (Frostbrand Weapon Rank 1)
	sp = dbcSpell.LookupEntryForced(8033);
	if(sp != NULL)
		sp->procChance = 10;

	// Spell 8034 Proc Chance (Frostbrand Attack Rank 1)
	sp = dbcSpell.LookupEntryForced(8034);
	if(sp != NULL)
		sp->procChance = 10;

	// Spell 8037 Proc Chance (Frostbrand Attack Rank 2)
	sp = dbcSpell.LookupEntryForced(8037);
	if(sp != NULL)
		sp->procChance = 10;

	// Spell 8182 Proc Chance (Frost Resistance Rank 1)
	sp = dbcSpell.LookupEntryForced(8182);
	if(sp != NULL)
		sp->procChance = 100;

	// Spell 8185 Proc Chance (Fire Resistance Rank 1)
	sp = dbcSpell.LookupEntryForced(8185);
	if(sp != NULL)
		sp->procChance = 100;

	// Spell 8516 Proc Chance (Windfury Totem Rank 1)
	sp = dbcSpell.LookupEntryForced(8516);
	if(sp != NULL)
		sp->procChance = 10;

	// Spell 10456 Proc Chance (Frostbrand Weapon Rank 3)
	sp = dbcSpell.LookupEntryForced(10456);
	if(sp != NULL)
		sp->procChance = 10;

	// Spell 10458 Proc Chance (Frostbrand Attack Rank 3)
	sp = dbcSpell.LookupEntryForced(10458);
	if(sp != NULL)
		sp->procChance = 10;

	// Spell 10476 Proc Chance (Frost Resistance Rank 2)
	sp = dbcSpell.LookupEntryForced(10476);
	if(sp != NULL)
		sp->procChance = 100;

	// Spell 10477 Proc Chance (Frost Resistance Rank 3)
	sp = dbcSpell.LookupEntryForced(10477);
	if(sp != NULL)
		sp->procChance = 100;

	// Spell 10534 Proc Chance (Fire Resistance Rank 2)
	sp = dbcSpell.LookupEntryForced(10534);
	if(sp != NULL)
		sp->procChance = 100;

	// Spell 10535 Proc Chance (Fire Resistance Rank 3)
	sp = dbcSpell.LookupEntryForced(10535);
	if(sp != NULL)
		sp->procChance = 100;

	// Spell 10608 Proc Chance (Windfury Totem Rank 2)
	sp = dbcSpell.LookupEntryForced(10608);
	if(sp != NULL)
		sp->procChance = 10;

	// Spell 10610 Proc Chance (Windfury Totem Rank 3)
	sp = dbcSpell.LookupEntryForced(10610);
	if(sp != NULL)
		sp->procChance = 10;

	// Spell 11069 Group Relation (Improved Fireball Rank 1)
	sp = dbcSpell.LookupEntryForced(11069);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1;
	}

	// Spell 11070 Group Relation (Improved Frostbolt Rank 1)
	sp = dbcSpell.LookupEntryForced(11070);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 32;
	}

	// Spell 11071 Group Relation (Frostbite Rank 1)
	sp = dbcSpell.LookupEntryForced(11071);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1048576;
	}

	// Spell 11151 Group Relation (Piercing Ice Rank 1)
	sp = dbcSpell.LookupEntryForced(11151);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 736;
	}

	// Spell 11160 Group Relation (Frost Channeling Rank 1)
	sp = dbcSpell.LookupEntryForced(11160);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 524288;
	}

	// Spell 11175 Group Relation (Permafrost Rank 1)
	sp = dbcSpell.LookupEntryForced(11175);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 1048576;
	}

	// Spell 11207 Group Relation (Ice Shards Rank 1)
	sp = dbcSpell.LookupEntryForced(11207);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 736;
	}

	// Spell 11213 Group Relation (Arcane Concentration Rank 1)
	sp = dbcSpell.LookupEntryForced(11213);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 128;
	}

	// Spell 11222 Group Relation (Arcane Focus Rank 1)
	sp = dbcSpell.LookupEntryForced(11222);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 2359296;
	}

	// Spell 11687 Proc Chance (Life Tap Rank 4)
	sp = dbcSpell.LookupEntryForced(11687);
	if(sp != NULL)
		sp->procChance = 100;

	// Spell 11688 Proc Chance (Life Tap Rank 5)
	sp = dbcSpell.LookupEntryForced(11688);
	if(sp != NULL)
		sp->procChance = 100;

	// Spell 11689 Proc Chance (Life Tap Rank 6)
	sp = dbcSpell.LookupEntryForced(11689);
	if(sp != NULL)
		sp->procChance = 100;

	// Spell 12043 Group Relation (Presence of Mind )
	sp = dbcSpell.LookupEntryForced(12043);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 4294967295UL;
	}

	// Spell 12284 Proc Chance (Mace Specialization Rank 1)
	sp = dbcSpell.LookupEntryForced(12284);
	if(sp != NULL)
		sp->procChance = 1;

	// Spell 12286 Group Relation (Improved Rend Rank 1)
	sp = dbcSpell.LookupEntryForced(12286);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 32;
	}

	// Spell 12292 Proc Chance (Death Wish )
	sp = dbcSpell.LookupEntryForced(12292);
	if(sp != NULL)
		sp->procChance = 100;

	// Spell 12295 Group Relation (Tactical Mastery Rank 1)
	sp = dbcSpell.LookupEntryForced(12295);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 8388608;
	}

	// Spell 12308 Group Relation (Improved Sunder Armor Rank 1)
	sp = dbcSpell.LookupEntryForced(12308);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 16384;
	}

	// Spell 12318 Group Relation (Commanding Presence Rank 1)
	sp = dbcSpell.LookupEntryForced(12318);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 65536;
	}

	// Spell 12321 Group Relation (Booming Voice Rank 1)
	sp = dbcSpell.LookupEntryForced(12321);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 196608;
		sp->EffectSpellGroupRelation[1] = 196608;
	}

	// Spell 12322 Proc Chance (Unbridled Wrath Rank 1)
	sp = dbcSpell.LookupEntryForced(12322);
	if(sp != NULL)
		sp->procChance = 8;

	// Spell 12330 Group Relation (Improved Slam Rank 2)
	sp = dbcSpell.LookupEntryForced(12330);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 2097152;
	}

	// Spell 12338 Group Relation (Improved Fireball Rank 2)
	sp = dbcSpell.LookupEntryForced(12338);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1;
	}

	// Spell 12339 Group Relation (Improved Fireball Rank 3)
	sp = dbcSpell.LookupEntryForced(12339);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1;
	}

	// Spell 12340 Group Relation (Improved Fireball Rank 4)
	sp = dbcSpell.LookupEntryForced(12340);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1;
	}

	// Spell 12341 Group Relation (Improved Fireball Rank 5)
	sp = dbcSpell.LookupEntryForced(12341);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1;
	}

	// Spell 12473 Group Relation (Improved Frostbolt Rank 2)
	sp = dbcSpell.LookupEntryForced(12473);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 32;
	}

	// Spell 12496 Group Relation (Frostbite Rank 2)
	sp = dbcSpell.LookupEntryForced(12496);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1048576;
	}

	// Spell 12497 Group Relation (Frostbite Rank 3)
	sp = dbcSpell.LookupEntryForced(12497);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1048576;
	}

	// Spell 12518 Group Relation (Frost Channeling Rank 2)
	sp = dbcSpell.LookupEntryForced(12518);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 524288;
	}

	// Spell 12519 Group Relation (Frost Channeling Rank 3)
	sp = dbcSpell.LookupEntryForced(12519);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 524288;
	}

	// Spell 12569 Group Relation (Permafrost Rank 2)
	sp = dbcSpell.LookupEntryForced(12569);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 1048576;
	}

	// Spell 12571 Group Relation (Permafrost Rank 3)
	sp = dbcSpell.LookupEntryForced(12571);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 1048576;
	}

	// Spell 12574 Group Relation (Arcane Concentration Rank 2)
	sp = dbcSpell.LookupEntryForced(12574);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 128;
	}

	// Spell 12575 Group Relation (Arcane Concentration Rank 3)
	sp = dbcSpell.LookupEntryForced(12575);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 128;
	}

	// Spell 12576 Group Relation (Arcane Concentration Rank 4)
	sp = dbcSpell.LookupEntryForced(12576);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 128;
	}

	// Spell 12577 Group Relation (Arcane Concentration Rank 5)
	sp = dbcSpell.LookupEntryForced(12577);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 128;
	}

	// Spell 12658 Group Relation (Improved Rend Rank 2)
	sp = dbcSpell.LookupEntryForced(12658);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 32;
	}

	// Spell 12659 Group Relation (Improved Rend Rank 3)
	sp = dbcSpell.LookupEntryForced(12659);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 32;
	}

	// Spell 12672 Group Relation (Ice Shards Rank 2)
	sp = dbcSpell.LookupEntryForced(12672);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 736;
	}

	// Spell 12676 Group Relation (Tactical Mastery Rank 2)
	sp = dbcSpell.LookupEntryForced(12676);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 8388608;
	}

	// Spell 12677 Group Relation (Tactical Mastery Rank 3)
	sp = dbcSpell.LookupEntryForced(12677);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 8388608;
	}

	// Spell 12678 Group Relation (Stance Mastery Passive)
	sp = dbcSpell.LookupEntryForced(12678);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 8388608;
	}

	// Spell 23689 Proc Chance (Darkmoon Card: Heroism)
	sp = dbcSpell.LookupEntryForced(23689);
	if(sp != NULL)
		sp->procChance = 10;

	// Spell 12701 Proc Chance (Mace Specialization Rank 2)
	sp = dbcSpell.LookupEntryForced(12701);
	if(sp != NULL)
		sp->procChance = 2;

	// Spell 12702 Proc Chance (Mace Specialization Rank 3)
	sp = dbcSpell.LookupEntryForced(12702);
	if(sp != NULL)
		sp->procChance = 3;

	// Spell 12703 Proc Chance (Mace Specialization Rank 4)
	sp = dbcSpell.LookupEntryForced(12703);
	if(sp != NULL)
		sp->procChance = 4;

	// Spell 12704 Proc Chance (Mace Specialization Rank 5)
	sp = dbcSpell.LookupEntryForced(12704);
	if(sp != NULL)
		sp->procChance = 6;

	// Spell 12810 Group Relation (Improved Sunder Armor Rank 2)
	sp = dbcSpell.LookupEntryForced(12810);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 16384;
	}

	// Spell 12811 Group Relation (Improved Sunder Armor Rank 3)
	sp = dbcSpell.LookupEntryForced(12811);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 16384;
	}

	// Spell 12835 Group Relation (Booming Voice Rank 2)
	sp = dbcSpell.LookupEntryForced(12835);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 196608;
		sp->EffectSpellGroupRelation[1] = 196608;
	}

	// Spell 12836 Group Relation (Booming Voice Rank 3)
	sp = dbcSpell.LookupEntryForced(12836);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 196608;
		sp->EffectSpellGroupRelation[1] = 196608;
	}

	// Spell 12837 Group Relation (Booming Voice Rank 4)
	sp = dbcSpell.LookupEntryForced(12837);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 196608;
		sp->EffectSpellGroupRelation[1] = 196608;
	}

	// Spell 12838 Group Relation (Booming Voice Rank 5)
	sp = dbcSpell.LookupEntryForced(12838);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 196608;
		sp->EffectSpellGroupRelation[1] = 196608;
	}

	// Spell 12839 Group Relation (Arcane Focus Rank 2)
	sp = dbcSpell.LookupEntryForced(12839);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 2359296;
	}

	// Spell 12840 Group Relation (Arcane Focus Rank 3)
	sp = dbcSpell.LookupEntryForced(12840);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 2359296;
	}

	// Spell 12841 Group Relation (Arcane Focus Rank 4)
	sp = dbcSpell.LookupEntryForced(12841);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 2359296;
	}

	// Spell 12857 Group Relation (Commanding Presence Rank 2)
	sp = dbcSpell.LookupEntryForced(12857);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 65536;
	}

	// Spell 12858 Group Relation (Commanding Presence Rank 3)
	sp = dbcSpell.LookupEntryForced(12858);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 65536;
	}

	// Spell 12860 Group Relation (Commanding Presence Rank 4)
	sp = dbcSpell.LookupEntryForced(12860);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 65536;
	}

	// Spell 12861 Group Relation (Commanding Presence Rank 5)
	sp = dbcSpell.LookupEntryForced(12861);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 65536;
	}

	// Spell 12862 Group Relation (Improved Slam Rank 1)
	sp = dbcSpell.LookupEntryForced(12862);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 2097152;
	}

	// Spell 12945 Group Relation (Improved Shield Block Rank 1)
	sp = dbcSpell.LookupEntryForced(12945);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 4096;
	}

	// Spell 12952 Group Relation (Piercing Ice Rank 2)
	sp = dbcSpell.LookupEntryForced(12952);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 736;
	}

	// Spell 12953 Group Relation (Piercing Ice Rank 3)
	sp = dbcSpell.LookupEntryForced(12953);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 736;
	}

	// Spell 12954 Group Relation (Piercing Ice Rank 4)
	sp = dbcSpell.LookupEntryForced(12954);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 736;
	}

	// Spell 12957 Group Relation (Piercing Ice Rank 5)
	sp = dbcSpell.LookupEntryForced(12957);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 736;
	}

	// Spell 12999 Proc Chance (Unbridled Wrath Rank 2)
	sp = dbcSpell.LookupEntryForced(12999);
	if(sp != NULL)
		sp->procChance = 16;

	// Spell 13000 Proc Chance (Unbridled Wrath Rank 3)
	sp = dbcSpell.LookupEntryForced(13000);
	if(sp != NULL)
		sp->procChance = 24;

	// Spell 13001 Proc Chance (Unbridled Wrath Rank 4)
	sp = dbcSpell.LookupEntryForced(13001);
	if(sp != NULL)
		sp->procChance = 32;

	// Spell 13002 Proc Chance (Unbridled Wrath Rank 5)
	sp = dbcSpell.LookupEntryForced(13002);
	if(sp != NULL)
		sp->procChance = 40;

	// Spell 13733 Group Relation (Puncturing Wounds Rank 1)
	sp = dbcSpell.LookupEntryForced(13733);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 4;
	}

	// Spell 13865 Group Relation (Puncturing Wounds Rank 2)
	sp = dbcSpell.LookupEntryForced(13865);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 4;
	}

	// Spell 13866 Group Relation (Puncturing Wounds Rank 3)
	sp = dbcSpell.LookupEntryForced(13866);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 4;
	}

	// Spell 13975 Group Relation (Camouflage Rank 1)
	sp = dbcSpell.LookupEntryForced(13975);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 4194304;
	}

	// Spell 14057 Group Relation (Opportunity Rank 1)
	sp = dbcSpell.LookupEntryForced(14057);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 516;
	}

	// Spell 14062 Group Relation (Camouflage Rank 2)
	sp = dbcSpell.LookupEntryForced(14062);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 4194304;
	}

	// Spell 14063 Group Relation (Camouflage Rank 3)
	sp = dbcSpell.LookupEntryForced(14063);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 4194304;
	}

	// Spell 14064 Group Relation (Camouflage Rank 4)
	sp = dbcSpell.LookupEntryForced(14064);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 4194304;
	}

	// Spell 14065 Group Relation (Camouflage Rank 5)
	sp = dbcSpell.LookupEntryForced(14065);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 4194304;
	}

	// Spell 14072 Group Relation (Opportunity Rank 2)
	sp = dbcSpell.LookupEntryForced(14072);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 516;
	}

	// Spell 14073 Group Relation (Opportunity Rank 3)
	sp = dbcSpell.LookupEntryForced(14073);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 516;
	}

	// Spell 14074 Group Relation (Opportunity Rank 4)
	sp = dbcSpell.LookupEntryForced(14074);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 516;
	}

	// Spell 14075 Group Relation (Opportunity Rank 5)
	sp = dbcSpell.LookupEntryForced(14075);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 516;
	}

	// Spell 14076 Proc Chance (Dirty Tricks Rank 1)
	sp = dbcSpell.LookupEntryForced(14076);
	if(sp != NULL)
		sp->procChance = 30;

	// Spell 14094 Proc Chance (Dirty Tricks Rank 2)
	sp = dbcSpell.LookupEntryForced(14094);
	if(sp != NULL)
		sp->procChance = 60;

	// Spell 14128 Group Relation (Lethality Rank 1)
	sp = dbcSpell.LookupEntryForced(14128);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 100663310;
	}

	// Spell 14132 Group Relation (Lethality Rank 2)
	sp = dbcSpell.LookupEntryForced(14132);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 100663310;
	}

	// Spell 14135 Group Relation (Lethality Rank 3)
	sp = dbcSpell.LookupEntryForced(14135);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 100663310;
	}

	// Spell 14136 Group Relation (Lethality Rank 4)
	sp = dbcSpell.LookupEntryForced(14136);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 100663310;
	}

	// Spell 14137 Group Relation (Lethality Rank 5)
	sp = dbcSpell.LookupEntryForced(14137);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 100663310;
	}

	// Spell 14162 Group Relation (Improved Eviscerate Rank 1)
	sp = dbcSpell.LookupEntryForced(14162);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 131072;
	}

	// Spell 14163 Group Relation (Improved Eviscerate Rank 2)
	sp = dbcSpell.LookupEntryForced(14163);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 131072;
	}

	// Spell 14164 Group Relation (Improved Eviscerate Rank 3)
	sp = dbcSpell.LookupEntryForced(14164);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 131072;
	}

	// Spell 14177 Group Relation (Cold Blood )
	sp = dbcSpell.LookupEntryForced(14177);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 131590;
	}

	// Spell 14179 Group Relation (Relentless Strikes )
	sp = dbcSpell.LookupEntryForced(14179);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 4063232;
	}

	// Spell 14520 Group Relation (Mental Agility Rank 1)
	sp = dbcSpell.LookupEntryForced(14520);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 2185347403UL;
	}

	// Spell 14743 Group Relation (Focused Casting Rank 1)
	sp = dbcSpell.LookupEntryForced(14743);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1152794256UL;
	}

	// Spell 14747 Group Relation (Improved Inner Fire Rank 1)
	sp = dbcSpell.LookupEntryForced(14747);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 2;
	}

	// Spell 14750 Group Relation (Improved Mana Burn Rank 1)
	sp = dbcSpell.LookupEntryForced(14750);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 16;
	}

	// Spell 14751 Group Relation (Inner Focus )
	sp = dbcSpell.LookupEntryForced(14751);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 3338141659UL;
		sp->EffectSpellGroupRelation[1] = 3377741456UL;
	}

	// Spell 14770 Group Relation (Improved Inner Fire Rank 2)
	sp = dbcSpell.LookupEntryForced(14770);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 2;
	}

	// Spell 14771 Group Relation (Improved Inner Fire Rank 3)
	sp = dbcSpell.LookupEntryForced(14771);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 2;
	}

	// Spell 14772 Group Relation (Improved Mana Burn Rank 2)
	sp = dbcSpell.LookupEntryForced(14772);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 16;
	}

	// Spell 14780 Group Relation (Mental Agility Rank 2)
	sp = dbcSpell.LookupEntryForced(14780);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 2185347403UL;
	}

	// Spell 14781 Group Relation (Mental Agility Rank 3)
	sp = dbcSpell.LookupEntryForced(14781);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 2185347403UL;
	}

	// Spell 14782 Group Relation (Mental Agility Rank 4)
	sp = dbcSpell.LookupEntryForced(14782);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 2185347403UL;
	}

	// Spell 14783 Group Relation (Mental Agility Rank 5)
	sp = dbcSpell.LookupEntryForced(14783);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 2185347403UL;
	}

	// Spell 14889 Group Relation (Holy Specialization Rank 1)
	sp = dbcSpell.LookupEntryForced(14889);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 156507776UL;
	}

	// Spell 14898 Group Relation (Spiritual Healing Rank 1)
	sp = dbcSpell.LookupEntryForced(14898);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 151264832UL;
	}

	// Spell 14908 Group Relation (Improved Renew Rank 1)
	sp = dbcSpell.LookupEntryForced(14908);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 64;
	}

	// Spell 14910 Group Relation (Shadow Resilience Rank 1)
	sp = dbcSpell.LookupEntryForced(14910);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 256;
	}

	// Spell 14911 Group Relation (Healing Prayers Rank 1)
	sp = dbcSpell.LookupEntryForced(14911);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 512;
	}

	// Spell 14912 Group Relation (Improved Healing Rank 1)
	sp = dbcSpell.LookupEntryForced(14912);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 267264;
	}

	// Spell 14913 Group Relation (Healing Focus Rank 1)
	sp = dbcSpell.LookupEntryForced(14913);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 269824;
	}

	// Spell 15008 Group Relation (Holy Specialization Rank 2)
	sp = dbcSpell.LookupEntryForced(15008);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 156507776;
	}

	// Spell 15009 Group Relation (Holy Specialization Rank 3)
	sp = dbcSpell.LookupEntryForced(15009);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 156507776;
	}

	// Spell 15010 Group Relation (Holy Specialization Rank 4)
	sp = dbcSpell.LookupEntryForced(15010);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 156507776;
	}

	// Spell 15011 Group Relation (Holy Specialization Rank 5)
	sp = dbcSpell.LookupEntryForced(15011);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 156507776;
	}

	// Spell 15012 Group Relation (Healing Focus Rank 2)
	sp = dbcSpell.LookupEntryForced(15012);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 269824;
	}

	// Spell 15013 Group Relation (Improved Healing Rank 2)
	sp = dbcSpell.LookupEntryForced(15013);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 267264;
	}

	// Spell 15014 Group Relation (Improved Healing Rank 3)
	sp = dbcSpell.LookupEntryForced(15014);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 267264;
	}

	// Spell 15018 Group Relation (Healing Prayers Rank 2)
	sp = dbcSpell.LookupEntryForced(15018);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 512;
	}

	// Spell 15020 Group Relation (Improved Renew Rank 2)
	sp = dbcSpell.LookupEntryForced(15020);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 64;
	}

	// Spell 15047 Group Relation (Ice Shards Rank 3)
	sp = dbcSpell.LookupEntryForced(15047);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 736;
	}

	// Spell 15052 Group Relation (Ice Shards Rank 4)
	sp = dbcSpell.LookupEntryForced(15052);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 736;
	}

	// Spell 15053 Group Relation (Ice Shards Rank 5)
	sp = dbcSpell.LookupEntryForced(15053);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 736;
	}

	// Spell 15058 Group Relation (Arcane Instability Rank 1)
	sp = dbcSpell.LookupEntryForced(15058);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 4194437;
		sp->EffectSpellGroupRelation[2] = 14686967;
	}

	// Spell 15059 Group Relation (Arcane Instability Rank 2)
	sp = dbcSpell.LookupEntryForced(15059);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 4194437;
		sp->EffectSpellGroupRelation[2] = 14686967;
	}

	// Spell 15060 Group Relation (Arcane Instability Rank 3)
	sp = dbcSpell.LookupEntryForced(15060);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 4194437;
		sp->EffectSpellGroupRelation[2] = 14686967;
	}

	// Spell 15257 Group Relation (Shadow Weaving Rank 1)
	sp = dbcSpell.LookupEntryForced(15257);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 42508288;
	}

	// Spell 15259 Group Relation (Darkness Rank 1)
	sp = dbcSpell.LookupEntryForced(15259);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 33562624;
		sp->EffectSpellGroupRelation[1] = 8421376;
	}

	// Spell 15260 Group Relation (Shadow Focus Rank 1)
	sp = dbcSpell.LookupEntryForced(15260);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 109813760;
	}

	// Spell 15272 Group Relation (Shadow Affinity Rank 2)
	sp = dbcSpell.LookupEntryForced(15272);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 109830400;
	}

	// Spell 15307 Group Relation (Darkness Rank 2)
	sp = dbcSpell.LookupEntryForced(15307);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 33562624;
		sp->EffectSpellGroupRelation[1] = 8421376;
	}

	// Spell 15308 Group Relation (Darkness Rank 3)
	sp = dbcSpell.LookupEntryForced(15308);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 33562624;
		sp->EffectSpellGroupRelation[1] = 8421376;
	}

	// Spell 15309 Group Relation (Darkness Rank 4)
	sp = dbcSpell.LookupEntryForced(15309);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 33562624;
		sp->EffectSpellGroupRelation[1] = 8421376;
	}

	// Spell 15310 Group Relation (Darkness Rank 5)
	sp = dbcSpell.LookupEntryForced(15310);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 33562624;
		sp->EffectSpellGroupRelation[1] = 8421376;
	}

	// Spell 15318 Group Relation (Shadow Affinity Rank 1)
	sp = dbcSpell.LookupEntryForced(15318);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 109830400;
	}

	// Spell 15320 Group Relation (Shadow Affinity Rank 3)
	sp = dbcSpell.LookupEntryForced(15320);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 109830400;
	}

	// Spell 15327 Group Relation (Shadow Focus Rank 2)
	sp = dbcSpell.LookupEntryForced(15327);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 109813760;
	}

	// Spell 15328 Group Relation (Shadow Focus Rank 3)
	sp = dbcSpell.LookupEntryForced(15328);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 109813760;
	}

	// Spell 15329 Group Relation (Shadow Focus Rank 4)
	sp = dbcSpell.LookupEntryForced(15329);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 109813760;
	}

	// Spell 15330 Group Relation (Shadow Focus Rank 5)
	sp = dbcSpell.LookupEntryForced(15330);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 109813760;
	}

	// Spell 15331 Group Relation (Shadow Weaving Rank 2)
	sp = dbcSpell.LookupEntryForced(15331);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 42508288;
	}

	// Spell 15332 Group Relation (Shadow Weaving Rank 3)
	sp = dbcSpell.LookupEntryForced(15332);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 42508288;
	}

	// Spell 15333 Group Relation (Shadow Weaving Rank 4)
	sp = dbcSpell.LookupEntryForced(15333);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 42508288;
	}

	// Spell 15334 Group Relation (Shadow Weaving Rank 5)
	sp = dbcSpell.LookupEntryForced(15334);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 42508288;
	}

	// Spell 15349 Group Relation (Spiritual Healing Rank 2)
	sp = dbcSpell.LookupEntryForced(15349);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 151264832;
	}

	// Spell 15354 Group Relation (Spiritual Healing Rank 3)
	sp = dbcSpell.LookupEntryForced(15354);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 151264832;
	}

	// Spell 15355 Group Relation (Spiritual Healing Rank 4)
	sp = dbcSpell.LookupEntryForced(15355);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 151264832;
	}

	// Spell 15356 Group Relation (Spiritual Healing Rank 5)
	sp = dbcSpell.LookupEntryForced(15356);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 151264832;
	}

	// Spell 15494 Proc Chance (Fury of Forgewright )
	sp = dbcSpell.LookupEntryForced(15494);
	if(sp != NULL)
		sp->procChance = 10;

	// Spell 15600 Proc Chance (Hand of Justice )
	sp = dbcSpell.LookupEntryForced(15600);
	if(sp != NULL)
		sp->procChance = 2;

	// Spell 15601 Proc Chance (Hand of Justice )
	sp = dbcSpell.LookupEntryForced(15601);
	if(sp != NULL)
		sp->procChance = 10;

	// Spell 15642 Proc Chance (Ironfoe )
	sp = dbcSpell.LookupEntryForced(15642);
	if(sp != NULL)
		sp->procChance = 10;

	// Spell 16178 Group Relation (Purification Rank 1)
	sp = dbcSpell.LookupEntryForced(16178);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 448;
	}

	// Spell 16179 Group Relation (Tidal Focus Rank 1)
	sp = dbcSpell.LookupEntryForced(16179);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 448;
	}

	// Spell 16188 Group Relation (Nature's Swiftness )
	sp = dbcSpell.LookupEntryForced(16188);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 2499;
	}

	// Spell 16210 Group Relation (Purification Rank 2)
	sp = dbcSpell.LookupEntryForced(16210);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 448;
	}

	// Spell 16211 Group Relation (Purification Rank 3)
	sp = dbcSpell.LookupEntryForced(16211);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 448;
	}

	// Spell 16212 Group Relation (Purification Rank 4)
	sp = dbcSpell.LookupEntryForced(16212);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 448;
	}

	// Spell 16213 Group Relation (Purification Rank 5)
	sp = dbcSpell.LookupEntryForced(16213);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 448;
	}

	// Spell 16214 Group Relation (Tidal Focus Rank 2)
	sp = dbcSpell.LookupEntryForced(16214);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 448;
	}

	// Spell 16215 Group Relation (Tidal Focus Rank 3)
	sp = dbcSpell.LookupEntryForced(16215);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 448;
	}

	// Spell 16216 Group Relation (Tidal Focus Rank 4)
	sp = dbcSpell.LookupEntryForced(16216);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 448;
	}

	// Spell 16217 Group Relation (Tidal Focus Rank 5)
	sp = dbcSpell.LookupEntryForced(16217);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 448;
	}

	// Spell 16246 Group Relation (Clearcasting )
	sp = dbcSpell.LookupEntryForced(16246);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 2416967683UL;
	}

	// Spell 16261 Group Relation (Improved Lightning Shield Rank 1)
	sp = dbcSpell.LookupEntryForced(16261);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1024;
	}

	// Spell 16290 Group Relation (Improved Lightning Shield Rank 2)
	sp = dbcSpell.LookupEntryForced(16290);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1024;
	}

	// Spell 16291 Group Relation (Improved Lightning Shield Rank 3)
	sp = dbcSpell.LookupEntryForced(16291);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1024;
	}

	// Spell 16352 Proc Chance (Frostbrand Attack Rank 4)
	sp = dbcSpell.LookupEntryForced(16352);
	if(sp != NULL)
		sp->procChance = 10;

	// Spell 16353 Proc Chance (Frostbrand Attack Rank 5)
	sp = dbcSpell.LookupEntryForced(16353);
	if(sp != NULL)
		sp->procChance = 10;

	// Spell 16355 Proc Chance (Frostbrand Weapon Rank 4)
	sp = dbcSpell.LookupEntryForced(16355);
	if(sp != NULL)
		sp->procChance = 10;

	// Spell 16356 Proc Chance (Frostbrand Weapon Rank 5)
	sp = dbcSpell.LookupEntryForced(16356);
	if(sp != NULL)
		sp->procChance = 10;

	// Spell 16459 Proc Chance (Sword Specialization )
	sp = dbcSpell.LookupEntryForced(16459);
	if(sp != NULL)
		sp->procChance = 10;

	// Spell 16493 Group Relation (Impale Rank 1)
	sp = dbcSpell.LookupEntryForced(16493);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 3999288558UL;
	}

	// Spell 16494 Group Relation (Impale Rank 2)
	sp = dbcSpell.LookupEntryForced(16494);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 3999288558UL;
	}

	// Spell 16578 Group Relation (Lightning Mastery Rank 1)
	sp = dbcSpell.LookupEntryForced(16578);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 3;
	}

	// Spell 16579 Group Relation (Lightning Mastery Rank 2)
	sp = dbcSpell.LookupEntryForced(16579);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 3;
	}

	// Spell 16580 Group Relation (Lightning Mastery Rank 3)
	sp = dbcSpell.LookupEntryForced(16580);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 3;
	}

	// Spell 16581 Group Relation (Lightning Mastery Rank 4)
	sp = dbcSpell.LookupEntryForced(16581);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 3;
	}

	// Spell 16582 Group Relation (Lightning Mastery Rank 5)
	sp = dbcSpell.LookupEntryForced(16582);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 3;
	}

	// Spell 16757 Group Relation (Arctic Reach Rank 1)
	sp = dbcSpell.LookupEntryForced(16757);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 160;
		sp->EffectSpellGroupRelation[1] = 576;
	}

	// Spell 16758 Group Relation (Arctic Reach Rank 2)
	sp = dbcSpell.LookupEntryForced(16758);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 160;
		sp->EffectSpellGroupRelation[1] = 576;
	}

	// Spell 16763 Group Relation (Improved Frostbolt Rank 3)
	sp = dbcSpell.LookupEntryForced(16763);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 32;
	}

	// Spell 16765 Group Relation (Improved Frostbolt Rank 4)
	sp = dbcSpell.LookupEntryForced(16765);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 32;
	}

	// Spell 16766 Group Relation (Improved Frostbolt Rank 5)
	sp = dbcSpell.LookupEntryForced(16766);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 32;
	}

	// Spell 16814 Group Relation (Starlight Wrath Rank 1)
	sp = dbcSpell.LookupEntryForced(16814);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1;
	}

	// Spell 16815 Group Relation (Starlight Wrath Rank 2)
	sp = dbcSpell.LookupEntryForced(16815);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1;
	}

	// Spell 16816 Group Relation (Starlight Wrath Rank 3)
	sp = dbcSpell.LookupEntryForced(16816);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1;
	}

	// Spell 16817 Group Relation (Starlight Wrath Rank 4)
	sp = dbcSpell.LookupEntryForced(16817);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1;
	}

	// Spell 16818 Group Relation (Starlight Wrath Rank 5)
	sp = dbcSpell.LookupEntryForced(16818);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1;
	}

	// Spell 16819 Group Relation (Nature's Reach Rank 1)
	sp = dbcSpell.LookupEntryForced(16819);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 4195847;
	}

	// Spell 16820 Group Relation (Nature's Reach Rank 2)
	sp = dbcSpell.LookupEntryForced(16820);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 4195847;
	}

	// Spell 16821 Group Relation (Improved Moonfire Rank 1)
	sp = dbcSpell.LookupEntryForced(16821);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 2;
		sp->EffectSpellGroupRelation[1] = 2;
	}

	// Spell 16822 Group Relation (Improved Moonfire Rank 2)
	sp = dbcSpell.LookupEntryForced(16822);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 2;
		sp->EffectSpellGroupRelation[1] = 2;
	}

	// Spell 16824 Group Relation (Improved Moonfire Rank 4)
	sp = dbcSpell.LookupEntryForced(16824);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 2;
	}

	// Spell 16833 Group Relation (Natural Shapeshifter Rank 1)
	sp = dbcSpell.LookupEntryForced(16833);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 3758096384UL;
	}

	// Spell 16834 Group Relation (Natural Shapeshifter Rank 2)
	sp = dbcSpell.LookupEntryForced(16834);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 3758096384UL;
	}

	// Spell 16835 Group Relation (Natural Shapeshifter Rank 3)
	sp = dbcSpell.LookupEntryForced(16835);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 3758096384UL;
	}

	// Spell 16843 Proc Chance (Crimson Fury )
	sp = dbcSpell.LookupEntryForced(16843);
	if(sp != NULL)
		sp->procChance = 10;

	// Spell 16850 Group Relation (Celestial Focus Rank 1)
	sp = dbcSpell.LookupEntryForced(16850);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 4;
	}

	// Spell 16850 Proc Chance (Celestial Focus Rank 1)
	sp = dbcSpell.LookupEntryForced(16850);
	if(sp != NULL)
		sp->procChance = 3;

	// Spell 16886 Group Relation (Nature's Grace )
	sp = dbcSpell.LookupEntryForced(16886);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 16777831;
	}

	// Spell 16896 Group Relation (Moonfury Rank 1)
	sp = dbcSpell.LookupEntryForced(16896);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 7;
	}

	// Spell 16897 Group Relation (Moonfury Rank 2)
	sp = dbcSpell.LookupEntryForced(16897);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 7;
	}

	// Spell 16899 Group Relation (Moonfury Rank 3)
	sp = dbcSpell.LookupEntryForced(16899);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 7;
	}

	// Spell 16900 Group Relation (Moonfury Rank 4)
	sp = dbcSpell.LookupEntryForced(16900);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 7;
	}

	// Spell 16901 Group Relation (Moonfury Rank 5)
	sp = dbcSpell.LookupEntryForced(16901);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 7;
	}

	// Spell 16918 Group Relation (Control of Nature Rank 1)
	sp = dbcSpell.LookupEntryForced(16918);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 512;
	}

	// Spell 16919 Group Relation (Control of Nature Rank 2)
	sp = dbcSpell.LookupEntryForced(16919);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 512;
	}

	// Spell 16920 Group Relation (Control of Nature Rank 3)
	sp = dbcSpell.LookupEntryForced(16920);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 512;
	}

	// Spell 16923 Group Relation (Celestial Focus Rank 2)
	sp = dbcSpell.LookupEntryForced(16923);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 4;
	}

	// Spell 16923 Proc Chance (Celestial Focus Rank 2)
	sp = dbcSpell.LookupEntryForced(16923);
	if(sp != NULL)
		sp->procChance = 6;

	// Spell 16924 Group Relation (Celestial Focus Rank 3)
	sp = dbcSpell.LookupEntryForced(16924);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 4;
	}

	// Spell 16924 Proc Chance (Celestial Focus Rank 3)
	sp = dbcSpell.LookupEntryForced(16924);
	if(sp != NULL)
		sp->procChance = 9;

	// Spell 16934 Group Relation (Ferocity Rank 1)
	sp = dbcSpell.LookupEntryForced(16934);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 2048;
		sp->EffectSpellGroupRelation[1] = 4096;
	}

	// Spell 16935 Group Relation (Ferocity Rank 2)
	sp = dbcSpell.LookupEntryForced(16935);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 2048;
		sp->EffectSpellGroupRelation[1] = 4096;
	}

	// Spell 16936 Group Relation (Ferocity Rank 3)
	sp = dbcSpell.LookupEntryForced(16936);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 2048;
		sp->EffectSpellGroupRelation[1] = 4096;
	}

	// Spell 16937 Group Relation (Ferocity Rank 4)
	sp = dbcSpell.LookupEntryForced(16937);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 2048;
		sp->EffectSpellGroupRelation[1] = 4096;
	}

	// Spell 16938 Group Relation (Ferocity Rank 5)
	sp = dbcSpell.LookupEntryForced(16938);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 2048;
		sp->EffectSpellGroupRelation[1] = 4096;
	}

	// Spell 16947 Group Relation (Feral Instinct Rank 1)
	sp = dbcSpell.LookupEntryForced(16947);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 33554432;
	}

	// Spell 16948 Group Relation (Feral Instinct Rank 2)
	sp = dbcSpell.LookupEntryForced(16948);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 33554432;
	}

	// Spell 16949 Group Relation (Feral Instinct Rank 3)
	sp = dbcSpell.LookupEntryForced(16949);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 33554432;
	}

	// Spell 16966 Group Relation (Shredding Attacks Rank 1)
	sp = dbcSpell.LookupEntryForced(16966);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 32768;
	}

	// Spell 16968 Group Relation (Shredding Attacks Rank 2)
	sp = dbcSpell.LookupEntryForced(16968);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 32768;
	}

	// Spell 16998 Group Relation (Savage Fury Rank 1)
	sp = dbcSpell.LookupEntryForced(16998);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 6144;
	}

	// Spell 16999 Group Relation (Savage Fury Rank 2)
	sp = dbcSpell.LookupEntryForced(16999);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 6144;
	}

	// Spell 17104 Group Relation (Gift of Nature Rank 1)
	sp = dbcSpell.LookupEntryForced(17104);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 240;
	}

	// Spell 17111 Group Relation (Improved Rejuvenation Rank 1)
	sp = dbcSpell.LookupEntryForced(17111);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 16;
	}

	// Spell 17112 Group Relation (Improved Rejuvenation Rank 2)
	sp = dbcSpell.LookupEntryForced(17112);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 16;
	}

	// Spell 17113 Group Relation (Improved Rejuvenation Rank 3)
	sp = dbcSpell.LookupEntryForced(17113);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 16;
	}

	// Spell 17114 Group Relation (Improved Rejuvenation Rank 4)
	sp = dbcSpell.LookupEntryForced(17114);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 16;
	}

	// Spell 17115 Group Relation (Improved Rejuvenation Rank 5)
	sp = dbcSpell.LookupEntryForced(17115);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 16;
	}

	// Spell 17116 Group Relation (Nature's Swiftness )
	sp = dbcSpell.LookupEntryForced(17116);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 16777825;
	}

	// Spell 17118 Group Relation (Subtlety Rank 1)
	sp = dbcSpell.LookupEntryForced(17118);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 240;
	}

	// Spell 17119 Group Relation (Subtlety Rank 2)
	sp = dbcSpell.LookupEntryForced(17119);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 240;
	}

	// Spell 17120 Group Relation (Subtlety Rank 3)
	sp = dbcSpell.LookupEntryForced(17120);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 240;
	}

	// Spell 17121 Group Relation (Subtlety Rank 4)
	sp = dbcSpell.LookupEntryForced(17121);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 240;
	}

	// Spell 17122 Group Relation (Subtlety Rank 5)
	sp = dbcSpell.LookupEntryForced(17122);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 240;
	}

	// Spell 17191 Group Relation (Improved Renew Rank 3)
	sp = dbcSpell.LookupEntryForced(17191);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 64;
	}

	// Spell 17322 Group Relation (Shadow Reach Rank 1)
	sp = dbcSpell.LookupEntryForced(17322);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 41984000;
	}

	// Spell 17323 Group Relation (Shadow Reach Rank 2)
	sp = dbcSpell.LookupEntryForced(17323);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 41984000;
	}

	// Spell 17768 Group Relation (Wolfshead Helm )
	sp = dbcSpell.LookupEntryForced(17768);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1073741824UL;
		sp->EffectSpellGroupRelation[1] = 2147483648UL;
	}

	// Spell 17778 Group Relation (Cataclysm Rank 1)
	sp = dbcSpell.LookupEntryForced(17778);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 997;
	}

	// Spell 17779 Group Relation (Cataclysm Rank 2)
	sp = dbcSpell.LookupEntryForced(17779);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 997;
	}

	// Spell 17780 Group Relation (Cataclysm Rank 3)
	sp = dbcSpell.LookupEntryForced(17780);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 997;
	}

	// Spell 17781 Group Relation (Cataclysm Rank 4)
	sp = dbcSpell.LookupEntryForced(17781);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 997;
	}

	// Spell 17782 Group Relation (Cataclysm Rank 5)
	sp = dbcSpell.LookupEntryForced(17782);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 997;
	}

	// Spell 17788 Group Relation (Bane Rank 1)
	sp = dbcSpell.LookupEntryForced(17788);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 64;
	}

	// Spell 17789 Group Relation (Bane Rank 2)
	sp = dbcSpell.LookupEntryForced(17789);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 64;
	}

	// Spell 17790 Group Relation (Bane Rank 3)
	sp = dbcSpell.LookupEntryForced(17790);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 64;
	}

	// Spell 17791 Group Relation (Bane Rank 4)
	sp = dbcSpell.LookupEntryForced(17791);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 64;
	}

	// Spell 17792 Group Relation (Bane Rank 5)
	sp = dbcSpell.LookupEntryForced(17792);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 64;
	}

	// Spell 17804 Group Relation (Soul Siphon Rank 1)
	sp = dbcSpell.LookupEntryForced(17804);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 8;
	}

	// Spell 17805 Group Relation (Soul Siphon Rank 2)
	sp = dbcSpell.LookupEntryForced(17805);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 8;
	}

	// Spell 17877 Group Relation (Shadowburn Rank 1)
	sp = dbcSpell.LookupEntryForced(17877);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 6265;
	}

	// Spell 17917 Group Relation (Destructive Reach Rank 1)
	sp = dbcSpell.LookupEntryForced(17917);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 997;
	}

	// Spell 17918 Group Relation (Destructive Reach Rank 2)
	sp = dbcSpell.LookupEntryForced(17918);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 997;
	}

	// Spell 17927 Group Relation (Improved Searing Pain Rank 1)
	sp = dbcSpell.LookupEntryForced(17927);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 256;
	}

	// Spell 17929 Group Relation (Improved Searing Pain Rank 2)
	sp = dbcSpell.LookupEntryForced(17929);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 256;
	}

	// Spell 17930 Group Relation (Improved Searing Pain Rank 3)
	sp = dbcSpell.LookupEntryForced(17930);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 256;
	}

	// Spell 17941 Group Relation (Shadow Trance )
	sp = dbcSpell.LookupEntryForced(17941);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1;
	}

	// Spell 17954 Group Relation (Emberstorm Rank 1)
	sp = dbcSpell.LookupEntryForced(17954);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 868;
	}

	// Spell 17955 Group Relation (Emberstorm Rank 2)
	sp = dbcSpell.LookupEntryForced(17955);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 868;
	}

	// Spell 17956 Group Relation (Emberstorm Rank 3)
	sp = dbcSpell.LookupEntryForced(17956);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 868;
	}

	// Spell 17957 Group Relation (Emberstorm Rank 4)
	sp = dbcSpell.LookupEntryForced(17957);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 868;
	}

	// Spell 17958 Group Relation (Emberstorm Rank 5)
	sp = dbcSpell.LookupEntryForced(17958);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 868;
	}

	// Spell 17959 Group Relation (Ruin Rank 1)
	sp = dbcSpell.LookupEntryForced(17959);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 997;
	}

	// Spell 18073 Group Relation (Pyroclasm Rank 2)
	sp = dbcSpell.LookupEntryForced(18073);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 64;
	}

	// Spell 18094 Group Relation (Nightfall Rank 1)
	sp = dbcSpell.LookupEntryForced(18094);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1;
	}

	// Spell 18095 Group Relation (Nightfall Rank 2)
	sp = dbcSpell.LookupEntryForced(18095);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1;
	}

	// Spell 18096 Group Relation (Pyroclasm Rank 1)
	sp = dbcSpell.LookupEntryForced(18096);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 64;
	}

	// Spell 18126 Group Relation (Improved Firebolt Rank 1)
	sp = dbcSpell.LookupEntryForced(18126);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 4096;
	}

	// Spell 18127 Group Relation (Improved Firebolt Rank 2)
	sp = dbcSpell.LookupEntryForced(18127);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 4096;
	}

	// Spell 18130 Group Relation (Devastation Rank 1)
	sp = dbcSpell.LookupEntryForced(18130);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 997;
	}

	// Spell 18131 Group Relation (Devastation Rank 2)
	sp = dbcSpell.LookupEntryForced(18131);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 997;
	}

	// Spell 18132 Group Relation (Devastation Rank 3)
	sp = dbcSpell.LookupEntryForced(18132);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 997;
	}

	// Spell 18133 Group Relation (Devastation Rank 4)
	sp = dbcSpell.LookupEntryForced(18133);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 997;
	}

	// Spell 18134 Group Relation (Devastation Rank 5)
	sp = dbcSpell.LookupEntryForced(18134);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 997;
	}

	// Spell 18135 Group Relation (Intensity Rank 1)
	sp = dbcSpell.LookupEntryForced(18135);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 96;
	}

	// Spell 18136 Group Relation (Intensity Rank 2)
	sp = dbcSpell.LookupEntryForced(18136);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 96;
	}

	// Spell 18179 Group Relation (Improved Curse of Weakness Rank 1)
	sp = dbcSpell.LookupEntryForced(18179);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 32768;
	}

	// Spell 18180 Group Relation (Improved Curse of Weakness Rank 2)
	sp = dbcSpell.LookupEntryForced(18180);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 32768;
	}

	// Spell 18213 Group Relation (Improved Drain Soul Rank 1)
	sp = dbcSpell.LookupEntryForced(18213);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 16384;
	}

	// Spell 18271 Group Relation (Shadow Mastery Rank 1)
	sp = dbcSpell.LookupEntryForced(18271);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 2;
	}

	// Spell 18272 Group Relation (Shadow Mastery Rank 2)
	sp = dbcSpell.LookupEntryForced(18272);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 2;
	}

	// Spell 18273 Group Relation (Shadow Mastery Rank 3)
	sp = dbcSpell.LookupEntryForced(18273);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 2;
	}

	// Spell 18274 Group Relation (Shadow Mastery Rank 4)
	sp = dbcSpell.LookupEntryForced(18274);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 2;
	}

	// Spell 18275 Group Relation (Shadow Mastery Rank 5)
	sp = dbcSpell.LookupEntryForced(18275);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 2;
	}

	// Spell 18288 Group Relation (Amplify Curse )
	sp = dbcSpell.LookupEntryForced(18288);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1024;
		sp->EffectSpellGroupRelation_high[0] = 2;
		sp->EffectSpellGroupRelation[1] = 4194304;
	}

	// Spell 18427 Group Relation (Aggression Rank 1)
	sp = dbcSpell.LookupEntryForced(18427);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 131074;
	}

	// Spell 18428 Group Relation (Aggression Rank 2)
	sp = dbcSpell.LookupEntryForced(18428);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 131074;
	}

	// Spell 18429 Group Relation (Aggression Rank 3)
	sp = dbcSpell.LookupEntryForced(18429);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 131074;
	}

	// Spell 18544 Group Relation (Force of Will Rank 1)
	sp = dbcSpell.LookupEntryForced(18544);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 139993232;
		sp->EffectSpellGroupRelation[1] = 5775504;
		sp->EffectSpellGroupRelation[2] = 10518528;
	}

	// Spell 18547 Group Relation (Force of Will Rank 2)
	sp = dbcSpell.LookupEntryForced(18547);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 139993232;
		sp->EffectSpellGroupRelation[1] = 5775504;
		sp->EffectSpellGroupRelation[2] = 10518528;
	}

	// Spell 18548 Group Relation (Force of Will Rank 3)
	sp = dbcSpell.LookupEntryForced(18548);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 139993232;
		sp->EffectSpellGroupRelation[1] = 5775504;
		sp->EffectSpellGroupRelation[2] = 10518528;
	}

	// Spell 18549 Group Relation (Force of Will Rank 4)
	sp = dbcSpell.LookupEntryForced(18549);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 139993232;
		sp->EffectSpellGroupRelation[1] = 5775504;
		sp->EffectSpellGroupRelation[2] = 10518528;
	}

	// Spell 18550 Group Relation (Force of Will Rank 5)
	sp = dbcSpell.LookupEntryForced(18550);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 139993232;
		sp->EffectSpellGroupRelation[1] = 5775504;
		sp->EffectSpellGroupRelation[2] = 10518528;
	}

	// Spell 18731 Group Relation (Fel Intellect Rank 1)
	sp = dbcSpell.LookupEntryForced(18731);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 268435456;
	}

	// Spell 18743 Group Relation (Fel Intellect Rank 2)
	sp = dbcSpell.LookupEntryForced(18743);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 268435456;
	}

	// Spell 18744 Group Relation (Fel Intellect Rank 3)
	sp = dbcSpell.LookupEntryForced(18744);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 268435456;
	}

	// Spell 18748 Group Relation (Fel Stamina Rank 1)
	sp = dbcSpell.LookupEntryForced(18748);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 134217728;
	}

	// Spell 18749 Group Relation (Fel Stamina Rank 2)
	sp = dbcSpell.LookupEntryForced(18749);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 134217728;
	}

	// Spell 18750 Group Relation (Fel Stamina Rank 3)
	sp = dbcSpell.LookupEntryForced(18750);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 134217728;
	}

	// Spell 18790 Group Relation (Fel Stamina )
	sp = dbcSpell.LookupEntryForced(18790);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 536870912;
	}

	// Spell 18797 Proc Chance (Flurry Axe )
	sp = dbcSpell.LookupEntryForced(18797);
	if(sp != NULL)
		sp->procChance = 10;

	// Spell 18803 Proc Chance (Focus )
	sp = dbcSpell.LookupEntryForced(18803);
	if(sp != NULL)
		sp->procChance = 100;

	// Spell 18821 Group Relation (Improved Enslave Demon Rank 1)
	sp = dbcSpell.LookupEntryForced(18821);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 2048;
		sp->EffectSpellGroupRelation[1] = 2048;
		sp->EffectSpellGroupRelation[2] = 2048;
	}

	// Spell 18822 Group Relation (Improved Enslave Demon Rank 2)
	sp = dbcSpell.LookupEntryForced(18822);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 2048;
		sp->EffectSpellGroupRelation[1] = 2048;
		sp->EffectSpellGroupRelation[2] = 2048;
	}

	// Spell 18827 Group Relation (Improved Curse of Agony Rank 1)
	sp = dbcSpell.LookupEntryForced(18827);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1024;
	}

	// Spell 18829 Group Relation (Improved Curse of Agony Rank 2)
	sp = dbcSpell.LookupEntryForced(18829);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1024;
	}

	// Spell 18867 Group Relation (Shadowburn Rank 2)
	sp = dbcSpell.LookupEntryForced(18867);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 6265;
	}

	// Spell 18868 Group Relation (Shadowburn Rank 3)
	sp = dbcSpell.LookupEntryForced(18868);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 6265;
	}

	// Spell 18869 Group Relation (Shadowburn Rank 4)
	sp = dbcSpell.LookupEntryForced(18869);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 6265;
	}

	// Spell 18870 Group Relation (Shadowburn Rank 5)
	sp = dbcSpell.LookupEntryForced(18870);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 6265;
	}

	// Spell 18871 Group Relation (Shadowburn Rank 6)
	sp = dbcSpell.LookupEntryForced(18871);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 6265;
	}

	// Spell 19105 Proc Chance (MHTest01 Effect )
	sp = dbcSpell.LookupEntryForced(19105);
	if(sp != NULL)
		sp->procChance = 10;

	// Spell 19109 Proc Chance (MHTest02 Effect )
	sp = dbcSpell.LookupEntryForced(19109);
	if(sp != NULL)
		sp->procChance = 10;

	// Spell 19184 Proc Chance (Entrapment Rank 1)
	sp = dbcSpell.LookupEntryForced(19184);
	if(sp != NULL)
		sp->procChance = 5;

	// Spell 19228 Proc Chance (Improved Wing Clip Rank 1)
	sp = dbcSpell.LookupEntryForced(19228);
	if(sp != NULL)
		sp->procChance = 4;

	// Spell 19232 Proc Chance (Improved Wing Clip Rank 2)
	sp = dbcSpell.LookupEntryForced(19232);
	if(sp != NULL)
		sp->procChance = 8;

	// Spell 19233 Proc Chance (Improved Wing Clip Rank 3)
	sp = dbcSpell.LookupEntryForced(19233);
	if(sp != NULL)
		sp->procChance = 12;

	// Spell 19387 Proc Chance (Entrapment Rank 2)
	sp = dbcSpell.LookupEntryForced(19387);
	if(sp != NULL)
		sp->procChance = 10;

	// Spell 19388 Proc Chance (Entrapment Rank 3)
	sp = dbcSpell.LookupEntryForced(19388);
	if(sp != NULL)
		sp->procChance = 15;

	// Spell 19416 Group Relation (Efficiency Rank 1)
	sp = dbcSpell.LookupEntryForced(19416);
	if(sp != NULL)
	{
		sp->EffectSpellGroupRelation[0] = 522752;
		sp->EffectSpellGroupRelation_high[0] = 1 | 128;
	}

	// Spell 19417 Group Relation (Efficiency Rank 2)
	sp = dbcSpell.LookupEntryForced(19417);
	if(sp != NULL)
	{
		sp->EffectSpellGroupRelation[0] = 522752;
		sp->EffectSpellGroupRelation_high[0] = 1 | 128;
	}

	// Spell 19418 Group Relation (Efficiency Rank 3)
	sp = dbcSpell.LookupEntryForced(19418);
	if(sp != NULL)
	{
		sp->EffectSpellGroupRelation[0] = 522752;
		sp->EffectSpellGroupRelation_high[0] = 1 | 128;
	}

	// Spell 19419 Group Relation (Efficiency Rank 4)
	sp = dbcSpell.LookupEntryForced(19419);
	if(sp != NULL)
	{
		sp->EffectSpellGroupRelation[0] = 522752;
		sp->EffectSpellGroupRelation_high[0] = 1 | 128;
	}

	// Spell 19420 Group Relation (Efficiency Rank 5)
	sp = dbcSpell.LookupEntryForced(19420);
	if(sp != NULL)
	{
		sp->EffectSpellGroupRelation[0] = 522752;
		sp->EffectSpellGroupRelation_high[0] = 1 | 128;
	}

	// Spell 19421 Group Relation (Improved Hunter's Mark Rank 1)
	sp = dbcSpell.LookupEntryForced(19421);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1024;
	}

	// Spell 19422 Group Relation (Improved Hunter's Mark Rank 2)
	sp = dbcSpell.LookupEntryForced(19422);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1024;
	}

	// Spell 19423 Group Relation (Improved Hunter's Mark Rank 3)
	sp = dbcSpell.LookupEntryForced(19423);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1024;
	}

	// Spell 19424 Group Relation (Improved Hunter's Mark Rank 4)
	sp = dbcSpell.LookupEntryForced(19424);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1024;
	}

	// Spell 19425 Group Relation (Improved Hunter's Mark Rank 5)
	sp = dbcSpell.LookupEntryForced(19425);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1024;
	}

	// Spell 19461 Group Relation (Barrage Rank 1)
	sp = dbcSpell.LookupEntryForced(19461);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 4096;
		sp->EffectSpellGroupRelation[1] = 8192;
	}

	// Spell 19462 Group Relation (Barrage Rank 2)
	sp = dbcSpell.LookupEntryForced(19462);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 4096;
		sp->EffectSpellGroupRelation[1] = 8192;
	}

	// Spell 19549 Group Relation (Improved Aspect of the Monkey Rank 1)
	sp = dbcSpell.LookupEntryForced(19549);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 524288;
	}

	// Spell 19550 Group Relation (Improved Aspect of the Monkey Rank 2)
	sp = dbcSpell.LookupEntryForced(19550);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 524288;
	}

	// Spell 19551 Group Relation (Improved Aspect of the Monkey Rank 3)
	sp = dbcSpell.LookupEntryForced(19551);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 524288;
	}

	// Spell 19552 Group Relation (Improved Aspect of the Hawk Rank 1)
	sp = dbcSpell.LookupEntryForced(19552);
	if(sp != NULL) 
	{
		sp->EffectSpellGroupRelation[0] = 1048576;
		sp->EffectSpellGroupRelation_high[1] = 512;
	}

	// Spell 19553 Group Relation (Improved Aspect of the Hawk Rank 2)
	sp = dbcSpell.LookupEntryForced(19553);
	if(sp != NULL) 
	{
		sp->EffectSpellGroupRelation[0] = 1048576;
		sp->EffectSpellGroupRelation_high[1] = 512;
	}

	// Spell 19554 Group Relation (Improved Aspect of the Hawk Rank 3)
	sp = dbcSpell.LookupEntryForced(19554);
	if(sp != NULL) 
	{
		sp->EffectSpellGroupRelation[0] = 1048576;
		sp->EffectSpellGroupRelation_high[1] = 512;
	}

	// Spell 19555 Group Relation (Improved Aspect of the Hawk Rank 4)
	sp = dbcSpell.LookupEntryForced(19555);
	if(sp != NULL)
	{
		sp->EffectSpellGroupRelation[0] = 1048576;
		sp->EffectSpellGroupRelation_high[1] = 512;
	}

	// Spell 19556 Group Relation (Improved Aspect of the Hawk Rank 5)
	sp = dbcSpell.LookupEntryForced(19556);
	if(sp != NULL)
	{
		sp->EffectSpellGroupRelation[0] = 1048576;
		sp->EffectSpellGroupRelation_high[1] = 512;
	}

	// Spell 19559 Group Relation (Pathfinding Rank 1)
	sp = dbcSpell.LookupEntryForced(19559);
	if(sp != NULL) 
	{
		sp->EffectSpellGroupRelation[0] = 2097152;
	}

	// Spell 19560 Group Relation (Pathfinding Rank 2)
	sp = dbcSpell.LookupEntryForced(19560);
	if(sp != NULL)
	{
		sp->EffectSpellGroupRelation[0] = 2097152;
	}

	// Spell 19572 Group Relation (Improved Mend Pet Rank 1)
	sp = dbcSpell.LookupEntryForced(19572);
	if(sp != NULL)
	{
		sp->EffectSpellGroupRelation[0] = 8388608;
	}

	// Spell 19573 Group Relation (Improved Mend Pet Rank 2)
	sp = dbcSpell.LookupEntryForced(19573);
	if(sp != NULL) 
	{
		sp->EffectSpellGroupRelation[0] = 8388608;
	}

	// Spell 19583 Group Relation (Endurance Training Rank 1)
	sp = dbcSpell.LookupEntryForced(19583);
	if(sp != NULL)
	{
		sp->EffectSpellGroupRelation[0] = 134217728;
	}

	// Spell 19584 Group Relation (Endurance Training Rank 2)
	sp = dbcSpell.LookupEntryForced(19584);
	if(sp != NULL) 
	{
		sp->EffectSpellGroupRelation[0] = 134217728;
	}

	// Spell 19585 Group Relation (Endurance Training Rank 3)
	sp = dbcSpell.LookupEntryForced(19585);
	if(sp != NULL) 
	{
		sp->EffectSpellGroupRelation[0] = 134217728;
	}

	// Spell 19586 Group Relation (Endurance Training Rank 4)
	sp = dbcSpell.LookupEntryForced(19586);
	if(sp != NULL)
	{
		sp->EffectSpellGroupRelation[0] = 134217728;
	}

	// Spell 19587 Group Relation (Endurance Training Rank 5)
	sp = dbcSpell.LookupEntryForced(19587);
	if(sp != NULL)
	{
		sp->EffectSpellGroupRelation[0] = 134217728;
	}

	// Spell 19590 Group Relation (Bestial Discipline Rank 1)
	sp = dbcSpell.LookupEntryForced(19590);
	if(sp != NULL)
	{
		sp->EffectSpellGroupRelation[0] = 536870912;
	}

	// Spell 19592 Group Relation (Bestial Discipline Rank 2)
	sp = dbcSpell.LookupEntryForced(19592);
	if(sp != NULL) 
	{
		sp->EffectSpellGroupRelation[0] = 536870912;
	}

	// Spell 19598 Group Relation (Ferocity Rank 1)
	sp = dbcSpell.LookupEntryForced(19598);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1073741824;
	}

	// Spell 19599 Group Relation (Ferocity Rank 2)
	sp = dbcSpell.LookupEntryForced(19599);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1073741824;
	}

	// Spell 19600 Group Relation (Ferocity Rank 3)
	sp = dbcSpell.LookupEntryForced(19600);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1073741824;
	}

	// Spell 19601 Group Relation (Ferocity Rank 4)
	sp = dbcSpell.LookupEntryForced(19601);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1073741824;
	}

	// Spell 19602 Group Relation (Ferocity Rank 5)
	sp = dbcSpell.LookupEntryForced(19602);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1073741824;
	}

	// Spell 19609 Group Relation (Thick Hide Rank 1)
	sp = dbcSpell.LookupEntryForced(19609);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 67108864;
	}

	// Spell 19610 Group Relation (Thick Hide Rank 2)
	sp = dbcSpell.LookupEntryForced(19610);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 67108864;
	}

	// Spell 19612 Group Relation (Thick Hide Rank 3)
	sp = dbcSpell.LookupEntryForced(19612);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 67108864;
	}

	// Spell 20138 Group Relation (Improved Devotion Aura Rank 1)
	sp = dbcSpell.LookupEntryForced(20138);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 64;
	}

	// Spell 20139 Group Relation (Improved Devotion Aura Rank 2)
	sp = dbcSpell.LookupEntryForced(20139);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 64;
	}

	// Spell 20140 Group Relation (Improved Devotion Aura Rank 3)
	sp = dbcSpell.LookupEntryForced(20140);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 64;
	}

	// Spell 20141 Group Relation (Improved Devotion Aura Rank 4)
	sp = dbcSpell.LookupEntryForced(20141);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 64;
	}

	// Spell 20142 Group Relation (Improved Devotion Aura Rank 5)
	sp = dbcSpell.LookupEntryForced(20142);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 64;
	}

	// Spell 20174 Group Relation (Guardian's Favor Rank 1)
	sp = dbcSpell.LookupEntryForced(20174);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 128;
		sp->EffectSpellGroupRelation[1] = 16;
	}

	// Spell 20175 Group Relation (Guardian's Favor Rank 2)
	sp = dbcSpell.LookupEntryForced(20175);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 128;
		sp->EffectSpellGroupRelation[1] = 16;
	}

	// Spell 20178 Proc Chance (Reckoning )
	sp = dbcSpell.LookupEntryForced(20178);
	if(sp != NULL)
		sp->procChance = 10;

	// Spell 20216 Group Relation (Divine Favor )
	sp = dbcSpell.LookupEntryForced(20216);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 2121728;
	}

	// Spell 20224 Group Relation (Improved Seal of Righteousness Rank 1)
	sp = dbcSpell.LookupEntryForced(20224);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1024;
	}

	// Spell 20225 Group Relation (Improved Seal of Righteousness Rank 2)
	sp = dbcSpell.LookupEntryForced(20225);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1024;
	}

	// Spell 20237 Group Relation (Healing Light Rank 1)
	sp = dbcSpell.LookupEntryForced(20237);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 3221225472UL;
	}

	// Spell 20238 Group Relation (Healing Light Rank 2)
	sp = dbcSpell.LookupEntryForced(20238);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 3221225472UL;
	}

	// Spell 20239 Group Relation (Healing Light Rank 3)
	sp = dbcSpell.LookupEntryForced(20239);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 3221225472UL;
	}

	// Spell 20254 Group Relation (Improved Concentration Aura Rank 1)
	sp = dbcSpell.LookupEntryForced(20254);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 131072;
	}

	// Spell 20255 Group Relation (Improved Concentration Aura Rank 2)
	sp = dbcSpell.LookupEntryForced(20255);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 131072;
	}

	// Spell 20256 Group Relation (Improved Concentration Aura Rank 3)
	sp = dbcSpell.LookupEntryForced(20256);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 131072;
	}

	// Spell 20330 Group Relation (Improved Seal of Righteousness Rank 3)
	sp = dbcSpell.LookupEntryForced(20330);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1024;
	}

	// Spell 20331 Group Relation (Improved Seal of Righteousness Rank 4)
	sp = dbcSpell.LookupEntryForced(20331);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1024;
	}

	// Spell 20332 Group Relation (Improved Seal of Righteousness Rank 5)
	sp = dbcSpell.LookupEntryForced(20332);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1024;
	}

	// Spell 20335 Group Relation (Improved Seal of the Crusader Rank 1)
	sp = dbcSpell.LookupEntryForced(20335);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 512;
		sp->EffectSpellGroupRelation[1] = 536870912;
	}

	// Spell 20336 Group Relation (Improved Seal of the Crusader Rank 2)
	sp = dbcSpell.LookupEntryForced(20336);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 512;
		sp->EffectSpellGroupRelation[1] = 536870912;
	}

	// Spell 20337 Group Relation (Improved Seal of the Crusader Rank 3)
	sp = dbcSpell.LookupEntryForced(20337);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 512;
		sp->EffectSpellGroupRelation[1] = 536870912;
	}

	// Spell 20359 Group Relation (Sanctified Light Rank 1)
	sp = dbcSpell.LookupEntryForced(20359);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 524288;
	}

	// Spell 20360 Group Relation (Sanctified Light Rank 2)
	sp = dbcSpell.LookupEntryForced(20360);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 524288;
	}

	// Spell 20361 Group Relation (Sanctified Light Rank 3)
	sp = dbcSpell.LookupEntryForced(20361);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 524288;
	}

	// Spell 20504 Group Relation (Weapon Mastery Rank 1)
	sp = dbcSpell.LookupEntryForced(20504);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1073741824;
	}

	// Spell 20505 Group Relation (Weapon Mastery Rank 2)
	sp = dbcSpell.LookupEntryForced(20505);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1073741824;
	}

	// Spell 20575 Group Relation (Command Racial Passive)
	sp = dbcSpell.LookupEntryForced(20575);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 67108864;
	}

	// Spell 20576 Group Relation (Command Racial Passive)
	sp = dbcSpell.LookupEntryForced(20576);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 33554432;
	}

	// Spell 21873 Group Relation (Cheaper Druid Shapeshifting )
	sp = dbcSpell.LookupEntryForced(21873);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 4026531840UL;
	}

	// Spell 21881 Group Relation (Improved Poisons )
	sp = dbcSpell.LookupEntryForced(21881);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 268558336UL;
	}

	// Spell 21887 Group Relation (Warrior's Wrath )
	sp = dbcSpell.LookupEntryForced(21887);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 3999289070UL;
	}

	// Spell 21895 Group Relation (Increased Totem Radius )
	sp = dbcSpell.LookupEntryForced(21895);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 67362816;
	}

	// Spell 21919 Proc Chance (Thrash )
	sp = dbcSpell.LookupEntryForced(21919);
	if(sp != NULL)
		sp->procChance = 10;

	// Spell 22008 Group Relation (Netherwind Focus )
	sp = dbcSpell.LookupEntryForced(22008);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1073741824;
		sp->EffectSpellGroupRelation[1] = 1073741824;
	}

	// Spell 22801 Group Relation (Ghost Wolf Speed Rank 1)
	sp = dbcSpell.LookupEntryForced(22801);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 2048;
	}

	// Spell 22812 Group Relation (Barkskin )
	sp = dbcSpell.LookupEntryForced(22812);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[2] = 16777829;
	}

	// Spell 23025 Group Relation (Blink Cooldown Reduction Rank 1)
	sp = dbcSpell.LookupEntryForced(23025);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 65536;
	}

	// Spell 23047 Group Relation (Fear Cast Time Reduction )
	sp = dbcSpell.LookupEntryForced(23047);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 4;
		sp->EffectSpellGroupRelation[1] = 4;
	}

	// Spell 23158 Group Relation (Concussive Shot Cooldown Reduction )
	sp = dbcSpell.LookupEntryForced(23158);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 512;
	}

	// Spell 23158 Proc Chance (Concussive Shot Cooldown Reduction )
	sp = dbcSpell.LookupEntryForced(23158);
	if(sp != NULL)
		sp->procChance = 4;

	// Spell 23398 Group Relation (Involuntary Transformation )
	sp = dbcSpell.LookupEntryForced(23398);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 4026531840UL;
	}

	// Spell 23555 Group Relation (Decreased Destruction Threat )
	sp = dbcSpell.LookupEntryForced(23555);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 997;
	}

	// Spell 23561 Group Relation (Enhanced Sunder Armor )
	sp = dbcSpell.LookupEntryForced(23561);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 16384;
	}

	// Spell 23566 Group Relation (Improved Volley and Multishot )
	sp = dbcSpell.LookupEntryForced(23566);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 4096;
	}

	// Spell 23724 Group Relation (Metamorphosis Rune )
	sp = dbcSpell.LookupEntryForced(23724);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 4026531840UL;
	}

	// Spell 23726 Group Relation (Venomous Totem )
	sp = dbcSpell.LookupEntryForced(23726);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 268558336;
	}

	// Spell 24348 Group Relation (Cheetah Sprint )
	sp = dbcSpell.LookupEntryForced(24348);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 2097152;
	}

	// Spell 24431 Group Relation (Improved Whirlwind )
	sp = dbcSpell.LookupEntryForced(24431);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 2147483648UL;
	}

	// Spell 24433 Group Relation (Improved Wrath/Starfire )
	sp = dbcSpell.LookupEntryForced(24433);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 5;
	}

	// Spell 24460 Group Relation (Improved Blessings )
	sp = dbcSpell.LookupEntryForced(24460);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 268435456;
	}

	// Spell 24469 Group Relation (Improved Blind )
	sp = dbcSpell.LookupEntryForced(24469);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 16777216;
	}

	// Spell 24499 Group Relation (Energized Shield )
	sp = dbcSpell.LookupEntryForced(24499);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1024;
	}

	// Spell 24542 Group Relation (Nimble Healing Touch )
	sp = dbcSpell.LookupEntryForced(24542);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 32;
	}

	// Spell 24543 Group Relation (Massive Destruction )
	sp = dbcSpell.LookupEntryForced(24543);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 997;
	}

	// Spell 24544 Group Relation (Arcane Potency )
	sp = dbcSpell.LookupEntryForced(24544);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 2359296;
		sp->EffectSpellGroupRelation[1] = 2359296;
	}

	// Spell 24546 Group Relation (Rapid Healing )
	sp = dbcSpell.LookupEntryForced(24546);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 4096;
	}

	// Spell 24691 Group Relation (Barrage Rank 3)
	sp = dbcSpell.LookupEntryForced(24691);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 4096;
		sp->EffectSpellGroupRelation[1] = 8192;
	}

	// Spell 24818 Group Relation (Noxious Breath )
	sp = dbcSpell.LookupEntryForced(24818);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 4294967295UL;
	}

	// Spell 24943 Group Relation (Gift of Nature Rank 2)
	sp = dbcSpell.LookupEntryForced(24943);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 240;
	}

	// Spell 24944 Group Relation (Gift of Nature Rank 3)
	sp = dbcSpell.LookupEntryForced(24944);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 240;
	}

	// Spell 24945 Group Relation (Gift of Nature Rank 4)
	sp = dbcSpell.LookupEntryForced(24945);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 240;
	}

	// Spell 24946 Group Relation (Gift of Nature Rank 5)
	sp = dbcSpell.LookupEntryForced(24946);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 240;
	}

	// Spell 26022 Proc Chance (Pursuit of Justice Rank 1)
	sp = dbcSpell.LookupEntryForced(26022);
	if(sp != NULL)
		sp->procChance = 100;

	// Spell 26023 Proc Chance (Pursuit of Justice Rank 2)
	sp = dbcSpell.LookupEntryForced(26023);
	if(sp != NULL)
		sp->procChance = 100;

	// Spell 26106 Group Relation (Genesis Rebirth Bonus )
	sp = dbcSpell.LookupEntryForced(26106);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 268435456;
	}

	// Spell 26109 Group Relation (Conqueror Shout Bonus )
	sp = dbcSpell.LookupEntryForced(26109);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 983040;
	}

	// Spell 26112 Group Relation (Deathdealer Evasion Bonus )
	sp = dbcSpell.LookupEntryForced(26112);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 32;
	}

	// Spell 26174 Group Relation (Striker's Rapid Fire Bonus )
	sp = dbcSpell.LookupEntryForced(26174);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 32;
	}

	// Spell 27035 Proc Chance (Sword Specialization (OLD) )
	sp = dbcSpell.LookupEntryForced(27035);
	if(sp != NULL)
		sp->procChance = 10;

	// Spell 27521 Proc Chance (Mana Restore )
	sp = dbcSpell.LookupEntryForced(27521);
	if(sp != NULL)
		sp->procChance = 2;

	// Spell 27789 Group Relation (Holy Reach Rank 1)
	sp = dbcSpell.LookupEntryForced(27789);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 138412544;
	}

	// Spell 27790 Group Relation (Holy Reach Rank 2)
	sp = dbcSpell.LookupEntryForced(27790);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[1] = 138412544;
	}

	// Spell 27828 Group Relation (Focused Casting Rank 2)
	sp = dbcSpell.LookupEntryForced(27828);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 1152794256;
	}

	// Spell 27846 Group Relation (Improved Healing Touch )
	sp = dbcSpell.LookupEntryForced(27846);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 32;
	}

	// Spell 27850 Group Relation (Devotion Armor Increased )
	sp = dbcSpell.LookupEntryForced(27850);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 64;
	}

	// Spell 27851 Group Relation (Claw and Rake Damage Bonus )
	sp = dbcSpell.LookupEntryForced(27851);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 4096;
	}

	// Spell 27867 Proc Chance (Freeze )
	sp = dbcSpell.LookupEntryForced(27867);
	if(sp != NULL)
		sp->procChance = 2;

	// Spell 28088 Group Relation (Bonescythe Bracer )
	sp = dbcSpell.LookupEntryForced(28088);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 268558336;
	}

	// Spell 28107 Group Relation (Bonescythe Breastplate )
	sp = dbcSpell.LookupEntryForced(28107);
	if(sp != NULL) {
		sp->EffectSpellGroupRelation[0] = 262144;
	}

	// Spell 25500 Proc Chance (Frostbrand Weapon Rank 6)
	sp = dbcSpell.LookupEntryForced(25500);
	if(sp != NULL)
		sp->procChance = 10;

	// Spell 25501 Proc Chance (Frostbrand Attack Rank 6)
	sp = dbcSpell.LookupEntryForced(25501);
	if(sp != NULL)
		sp->procChance = 10;

	// Spell 38617 Proc Chance (Frostbrand Attack )
	sp = dbcSpell.LookupEntryForced(38617);
	if(sp != NULL)
		sp->procChance = 10;
}

void ApplyNormalFixes()
{
	//Updating spell.dbc--this is slow like hell due to we cant read string fields
	//dbc method will be changed in future
	DBCFile dbc;

	if( !dbc.open( "DBC/Spell.dbc" ) )
	{
		Log.Error("World", "Cannot find file ./DBC/Spell.dbc" );
		return;
	}

	Log.Notice("World", "Processing %u spells...", dbc.getRecordCount());
	Apply112SpellFixes();

	uint32 cnt = (uint32)dbc.getRecordCount();
	uint32 effect;
	uint32 All_Seal_Groups_Combined=0;
	// Relation Groups

	//uint32 group_relation_paladin_healing_light = 0;
	uint32 group_relation_paladin_sanctified_light = 0;
	uint32 group_relation_shaman_enhancing_totems = 0;
	uint32 group_relation_shaman_restorative_totems = 0;
	uint32 group_relation_shaman_totems = 0;
	uint32 group_relation_shaman_lightning = 0;
	uint32 group_relation_shaman_shock = 0;
	uint32 group_relation_shaman_mental_quickness = 0;
	uint32 group_relation_rogue_elusiveness = 0;
	uint32 group_relation_rogue_poisons = 0;
	uint32 group_relation_rogue_find_weakness = 0;
	uint32 group_relation_rogue_shadow_step = 0;
	uint32 group_relation_rogue_lethality = 0;

	map<uint32, uint32> talentSpells;
	map<uint32,uint32>::iterator talentSpellIterator;
	unsigned int i,j;
	for(i = 0; i < dbcTalent.GetNumRows(); ++i)
	{
		TalentEntry * tal = dbcTalent.LookupRow(i);
		for(j = 0; j < 5; ++j)
			if(tal->RankID[j] != 0)
				talentSpells.insert(make_pair(tal->RankID[j], tal->TalentTree));
	}


	for(uint32 x=0; x < cnt; x++)
	{
		uint32 result = 0;
		// SpellID
		uint32 spellid = dbc.getRecord(x).getUInt(0);
		// Description field
		char* desc = (char*)dbc.getRecord(x).getString(158); 
		const char* ranktext = dbc.getRecord(x).getString(141);
		const char* nametext = dbc.getRecord(x).getString(124);

		uint32 rank = 0;
		uint32 type = 0;
		uint32 namehash = 0;

		// get spellentry
		SpellEntry * sp = dbcSpell.LookupEntry(spellid);		
		sp->self_cast_only = false;
		sp->apply_on_shapeshift_change = false;
		sp->always_apply = false;

		// hash the name
		//!!!!!!! representing all strings on 32 bits is dangerous. There is a chance to get same hash for a lot of strings ;)
        namehash = crc32((const unsigned char*)nametext, (unsigned int)strlen(nametext));
		sp->NameHash   = namehash; //need these set before we start processing spells

		float radius=std::max(::GetRadius(dbcSpellRadius.LookupEntry(sp->EffectRadiusIndex[0])),::GetRadius(dbcSpellRadius.LookupEntry(sp->EffectRadiusIndex[1])));
		radius=std::max(::GetRadius(dbcSpellRadius.LookupEntry(sp->EffectRadiusIndex[2])),radius);
		radius=std::max(GetMaxRange(dbcSpellRange.LookupEntry(sp->rangeIndex)),radius);
		sp->base_range_or_radius_sqr = radius*radius;

		sp->ai_target_type = GetAiTargetType( sp );
		// NEW SCHOOLS AS OF 2.4.0:
		/* (bitwise)
		SCHOOL_NORMAL = 1,
		SCHOOL_HOLY   = 2,
		SCHOOL_FIRE   = 4,
		SCHOOL_NATURE = 8,
		SCHOOL_FROST  = 16,
		SCHOOL_SHADOW = 32,
		SCHOOL_ARCANE = 64
		*/

		/*
				uint32 school = 0;
#define SET_SCHOOL(x) if( school != 0 ) { printf("Spell %u HAS MULTIPLE SCHOOLS!!!! %u\n", sp->Id, sp->School); } else { school = x; }

		if(sp->School & 1)
		{
			SET_SCHOOL(SCHOOL_NORMAL);
		}
		else if(sp->School & 2)
		{
			SET_SCHOOL(SCHOOL_HOLY);
		}
		else if(sp->School & 4)
		{
			SET_SCHOOL(SCHOOL_FIRE);
		}
		else if(sp->School & 8)
		{
			SET_SCHOOL(SCHOOL_NATURE);
		}
		else if(sp->School & 16)
		{
			SET_SCHOOL(SCHOOL_FROST);
		}
		else if(sp->School & 32)
		{
			SET_SCHOOL(SCHOOL_SHADOW);
		}
		else if(sp->School & 64)
		{
			SET_SCHOOL(SCHOOL_ARCANE);
		}
		else
		{
			printf("UNKNOWN SCHOOL %u\n", sp->School);
		}
		*/

#define SET_SCHOOL(x) sp->School = x
		if(sp->School & 1)
			SET_SCHOOL(SCHOOL_NORMAL);
		else if(sp->School & 2)
			SET_SCHOOL(SCHOOL_HOLY);
		else if(sp->School & 4)
			SET_SCHOOL(SCHOOL_FIRE);
		else if(sp->School & 8)
			SET_SCHOOL(SCHOOL_NATURE);
		else if(sp->School & 16)
			SET_SCHOOL(SCHOOL_FROST);
		else if(sp->School & 32)
			SET_SCHOOL(SCHOOL_SHADOW);
		else if(sp->School & 64)
			SET_SCHOOL(SCHOOL_ARCANE);
		else
			printf("UNKNOWN SCHOOL %u\n", sp->School);
#undef SET_SCHOOL

		/*
		AURASTATE_FLAG_DODGE_BLOCK			= 1,        //1
		AURASTATE_FLAG_HEALTH20             = 2,        //2
		AURASTATE_FLAG_BERSERK              = 4,        //3
		AURASTATE_FLAG_JUDGEMENT            = 16,       //5
		AURASTATE_FLAG_PARRY                = 64,       //7
		AURASTATE_FLAG_LASTKILLWITHHONOR    = 512,      //10
		AURASTATE_FLAG_CRITICAL             = 1024,     //11
		AURASTATE_FLAG_HEALTH35             = 4096,     //13
		AURASTATE_FLAG_IMMOLATE             = 8192,     //14
		AURASTATE_FLAG_REJUVENATE           = 16384,    //15 //where do i use this ?
		AURASTATE_FLAG_POISON               = 32768,    //16
		*/

		// correct caster aura state
		if( sp->CasterAuraState != 0 )
		{
			switch( sp->CasterAuraState )
			{
			case 1:
				sp->CasterAuraState = AURASTATE_FLAG_DODGE_BLOCK;
				break;

			case 2:
				sp->CasterAuraState = AURASTATE_FLAG_HEALTH20;
				break;

			case 3:
				sp->CasterAuraState = AURASTATE_FLAG_BERSERK;
				break;

			case 5:
				sp->CasterAuraState = AURASTATE_FLAG_JUDGEMENT;
				break;

			case 7:
				sp->CasterAuraState = AURASTATE_FLAG_PARRY;
				break;

			case 10:
				sp->CasterAuraState = AURASTATE_FLAG_LASTKILLWITHHONOR;
				break;

			case 11:
				sp->CasterAuraState = AURASTATE_FLAG_CRITICAL;
				break;

			case 13:
				sp->CasterAuraState = AURASTATE_FLAG_HEALTH35;
				break;

			case 14:
				sp->CasterAuraState = AURASTATE_FLAG_IMMOLATE;
				break;

			case 15:
				sp->CasterAuraState = AURASTATE_FLAG_REJUVENATE;
				break;

			case 16:
				sp->CasterAuraState = AURASTATE_FLAG_POISON;
				break;

			default:
				Log.Error("AuraState", "Spell %u (%s) has unknown caster aura state %u\n", sp->Id, sp->Name, sp->CasterAuraState);
				break;
			}
		}

		if( sp->TargetAuraState != 0 )
		{
			switch( sp->TargetAuraState )
			{
			case 1:
				sp->TargetAuraState = AURASTATE_FLAG_DODGE_BLOCK;
				break;

			case 2:
				sp->TargetAuraState = AURASTATE_FLAG_HEALTH20;
				break;

			case 3:
				sp->TargetAuraState = AURASTATE_FLAG_BERSERK;
				break;

			case 5:
				sp->TargetAuraState = AURASTATE_FLAG_JUDGEMENT;
				break;

			case 7:
				sp->TargetAuraState = AURASTATE_FLAG_PARRY;
				break;

			case 10:
				sp->TargetAuraState = AURASTATE_FLAG_LASTKILLWITHHONOR;
				break;

			case 11:
				sp->TargetAuraState = AURASTATE_FLAG_CRITICAL;
				break;

			case 13:
				sp->TargetAuraState = AURASTATE_FLAG_HEALTH35;
				break;

			case 14:
				sp->TargetAuraState = AURASTATE_FLAG_IMMOLATE;
				break;

			case 15:
				sp->TargetAuraState = AURASTATE_FLAG_REJUVENATE;
				break;

			case 16:
				sp->TargetAuraState = AURASTATE_FLAG_POISON;
				break;

			default:
				Log.Error("AuraState", "Spell %u (%s) has unknown target aura state %u\n", sp->Id, sp->Name, sp->TargetAuraState);
				break;
			}
		}

		// apply on shapeshift change
		if( sp->NameHash == SPELL_HASH_TRACK_HUMANOIDS )
			sp->apply_on_shapeshift_change = true;

		if( sp->NameHash == SPELL_HASH_BLOOD_FURY || sp->NameHash == SPELL_HASH_SHADOWSTEP )
			sp->always_apply = true;

		//there are some spells that change the "damage" value of 1 effect to another : devastate = bonus first then damage
		//this is a total bullshit so remove it when spell system supports effect overwriting
		for( uint32 col1_swap = 0; col1_swap < 2 ; col1_swap++ )
			for( uint32 col2_swap = col1_swap ; col2_swap < 3 ; col2_swap++ )
				if( sp->Effect[col1_swap] == SPELL_EFFECT_WEAPON_PERCENT_DAMAGE && sp->Effect[col2_swap] == SPELL_EFFECT_DUMMYMELEE )
				{
					uint32 temp;
					float ftemp;
					temp = sp->Effect[col1_swap];			sp->Effect[col1_swap] = sp->Effect[col2_swap] ;						sp->Effect[col2_swap] = temp;
					temp = sp->EffectDieSides[col1_swap];	sp->EffectDieSides[col1_swap] = sp->EffectDieSides[col2_swap] ;		sp->EffectDieSides[col2_swap] = temp;
					temp = sp->EffectBaseDice[col1_swap];	sp->EffectBaseDice[col1_swap] = sp->EffectBaseDice[col2_swap] ;		sp->EffectBaseDice[col2_swap] = temp;
					ftemp = sp->EffectDicePerLevel[col1_swap];			sp->EffectDicePerLevel[col1_swap] = sp->EffectDicePerLevel[col2_swap] ;				sp->EffectDicePerLevel[col2_swap] = ftemp;
					ftemp = sp->EffectRealPointsPerLevel[col1_swap];	sp->EffectRealPointsPerLevel[col1_swap] = sp->EffectRealPointsPerLevel[col2_swap] ;	sp->EffectRealPointsPerLevel[col2_swap] = ftemp;
					temp = sp->EffectBasePoints[col1_swap];	sp->EffectBasePoints[col1_swap] = sp->EffectBasePoints[col2_swap] ;	sp->EffectBasePoints[col2_swap] = temp;
					temp = sp->EffectMechanic[col1_swap];	sp->EffectMechanic[col1_swap] = sp->EffectMechanic[col2_swap] ;	sp->EffectMechanic[col2_swap] = temp;
					temp = sp->EffectImplicitTargetA[col1_swap];	sp->EffectImplicitTargetA[col1_swap] = sp->EffectImplicitTargetA[col2_swap] ;	sp->EffectImplicitTargetA[col2_swap] = temp;
					temp = sp->EffectImplicitTargetB[col1_swap];	sp->EffectImplicitTargetB[col1_swap] = sp->EffectImplicitTargetB[col2_swap] ;	sp->EffectImplicitTargetB[col2_swap] = temp;
					temp = sp->EffectRadiusIndex[col1_swap];	sp->EffectRadiusIndex[col1_swap] = sp->EffectRadiusIndex[col2_swap] ;	sp->EffectRadiusIndex[col2_swap] = temp;
					temp = sp->EffectApplyAuraName[col1_swap];	sp->EffectApplyAuraName[col1_swap] = sp->EffectApplyAuraName[col2_swap] ;	sp->EffectApplyAuraName[col2_swap] = temp;
					temp = sp->EffectAmplitude[col1_swap];		sp->EffectAmplitude[col1_swap] = sp->EffectAmplitude[col2_swap] ;	sp->EffectAmplitude[col2_swap] = temp;
					ftemp = sp->Effectunknown[col1_swap];		sp->Effectunknown[col1_swap] = sp->Effectunknown[col2_swap] ;	sp->Effectunknown[col2_swap] = ftemp;
					temp = sp->EffectChainTarget[col1_swap];	sp->EffectChainTarget[col1_swap] = sp->EffectChainTarget[col2_swap] ;	sp->EffectChainTarget[col2_swap] = temp;
					temp = sp->EffectSpellGroupRelation[col1_swap];	sp->EffectSpellGroupRelation[col1_swap] = sp->EffectSpellGroupRelation[col2_swap] ;	sp->EffectSpellGroupRelation[col2_swap] = temp;
					temp = sp->EffectMiscValue[col1_swap];		sp->EffectMiscValue[col1_swap] = sp->EffectMiscValue[col2_swap] ;	sp->EffectMiscValue[col2_swap] = temp;
					temp = sp->EffectTriggerSpell[col1_swap];	sp->EffectTriggerSpell[col1_swap] = sp->EffectTriggerSpell[col2_swap] ;	sp->EffectTriggerSpell[col2_swap] = temp;
					ftemp = sp->EffectPointsPerComboPoint[col1_swap];	sp->EffectPointsPerComboPoint[col1_swap] = sp->EffectPointsPerComboPoint[col2_swap] ;	sp->EffectPointsPerComboPoint[col2_swap] = ftemp;
				}

		for(uint32 b=0;b<3;++b)
		{
			if(sp->EffectTriggerSpell[b] != 0 && dbcSpell.LookupEntryForced(sp->EffectTriggerSpell[b]) == NULL)
			{
				/* proc spell referencing non-existant spell. create a dummy spell for use w/ it. */
				CreateDummySpell(sp->EffectTriggerSpell[b]);
			}
			/** Load teaching spells (used for hunters when learning pets wild abilities) */
			if(sp->Effect[b]==SPELL_EFFECT_LEARN_SPELL && sp->EffectImplicitTargetA[b]==EFF_TARGET_PET)
			{
				map<uint32,uint32>::iterator itr = sWorld.TeachingSpellMap.find(sp->EffectTriggerSpell[b]);
				if(itr == sWorld.TeachingSpellMap.end())
					sWorld.TeachingSpellMap.insert(make_pair(sp->EffectTriggerSpell[b],sp->Id));
			}

			if( sp->Attributes & ATTRIBUTES_ONLY_OUTDOORS && sp->EffectApplyAuraName[b] == SPELL_AURA_MOUNTED )
			{
				sp->Attributes &= ~ATTRIBUTES_ONLY_OUTDOORS;
			}

			// fill in more here
			/*switch( sp->EffectImplicitTargetA[b] )
			{
			case 1:
			case 9:
				sp->self_cast_only = true;
				break;
			}

			// fill in more here too
			switch( sp->EffectImplicitTargetB[b] )
			{
			case 1:
			case 9:
				sp->self_cast_only = true;
				break;
			}*/
		}

		/*if(sp->self_cast_only && !(sp->Attributes&64))
			printf("SPELL SELF CAST ONLY: %s %u\n", sp->Name, sp->Id);*/

		if(!strcmp(sp->Name, "Hearthstone") || !strcmp(sp->Name, "Stuck") || !strcmp(sp->Name, "Astral Recall"))
			sp->self_cast_only = true;

		sp->proc_interval = 0;//trigger at each event
		sp->c_is_flags = 0;
		sp->spell_coef_flags = 0;
		sp->Dspell_coef_override = -1;
		sp->OTspell_coef_override = -1;
		sp->casttime_coef = 0;
		sp->fixed_dddhcoef = -1;
		sp->fixed_hotdotcoef = -1;

		talentSpellIterator = talentSpells.find(sp->Id);
		if(talentSpellIterator == talentSpells.end())
			sp->talent_tree = 0;
		else
			sp->talent_tree = talentSpellIterator->second;

		// parse rank text
		if( !sscanf( ranktext, "Rank %d", (unsigned int*)&rank) )
			rank = 0;

		//seal of light 
		if( namehash == SPELL_HASH_SEAL_OF_LIGHT )			
			sp->procChance = 45;	/* this will do */

		//seal of command
		else if( namehash == SPELL_HASH_SEAL_OF_COMMAND )		
			sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC;

		//judgement of command
		else if( namehash == SPELL_HASH_JUDGEMENT_OF_COMMAND )		
			sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC;

		else if( namehash == SPELL_HASH_ARCANE_SHOT )		
			sp->c_is_flags |= SPELL_FLAG_IS_NOT_USING_DMG_BONUS;

		else if( namehash == SPELL_HASH_SERPENT_STING )		
			sp->c_is_flags |= SPELL_FLAG_IS_NOT_USING_DMG_BONUS;

		//Rogue: Posion time fix for 2.3
		if( strstr( nametext, "Crippling Poison") && sp->Effect[0] == 54 ) //I, II
			sp->EffectBasePoints[0] = 3599;
		if( strstr( nametext, "Mind-numbing Poison") && sp->Effect[0] == 54 ) //I,II,III
			sp->EffectBasePoints[0] = 3599;
		if( strstr( nametext, "Instant Poison") && sp->Effect[0] == 54 ) //I,II,III,IV,V,VI,VII    
			sp->EffectBasePoints[0] = 3599;
		if( strstr( nametext, "Deadly Poison") && sp->Effect[0] == 54 ) //I,II,III,IV,V,VI,VII
			sp->EffectBasePoints[0] = 3599;
		if( strstr( nametext, "Wound Poison") && sp->Effect[0] == 54 ) //I,II,III,IV,V
			sp->EffectBasePoints[0] = 3599;
		if( strstr( nametext, "Anesthetic Poison") && sp->Effect[0] == 54 ) //I
			sp->EffectBasePoints[0] = 3599;

        if( strstr( nametext, "Sharpen Blade") && sp->Effect[0] == 54 ) //All BS stones
            sp->EffectBasePoints[0] = 3599;

		//these mostly do not mix so we can use else 
        // look for seal, etc in name
        if( strstr( nametext, "Seal"))
		{
            type |= SPELL_TYPE_SEAL;
			All_Seal_Groups_Combined |= sp->SpellGroupType;
		}
        else if( strstr( nametext, "Blessing"))
            type |= SPELL_TYPE_BLESSING;
        else if( strstr( nametext, "Curse"))
		{
            type |= SPELL_TYPE_CURSE;
			sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_SKIPCHECK;
		}
        else if( strstr( nametext, "Aspect"))
            type |= SPELL_TYPE_ASPECT;
        else if( strstr( nametext, "Sting") || strstr( nametext, "sting"))
            type |= SPELL_TYPE_STING;
        // don't break armor items!
        else if(strcmp(nametext, "Armor") && strstr( nametext, "Armor") || strstr( nametext, "Demon Skin"))
            type |= SPELL_TYPE_ARMOR;
        else if( strstr( nametext, "Aura"))
            type |= SPELL_TYPE_AURA;
		else if( strstr( nametext, "Track")==nametext)
            type |= SPELL_TYPE_TRACK;
		else if( namehash == SPELL_HASH_GIFT_OF_THE_WILD || namehash == SPELL_HASH_MARK_OF_THE_WILD )
            type |= SPELL_TYPE_MARK_GIFT;
		else if( namehash == SPELL_HASH_IMMOLATION_TRAP || namehash == SPELL_HASH_FREEZING_TRAP || namehash == SPELL_HASH_FROST_TRAP || namehash == SPELL_HASH_EXPLOSIVE_TRAP || namehash == SPELL_HASH_SNAKE_TRAP )
            type |= SPELL_TYPE_HUNTER_TRAP;
		else if( namehash == SPELL_HASH_ARCANE_INTELLECT || namehash == SPELL_HASH_ARCANE_BRILLIANCE )
            type |= SPELL_TYPE_MAGE_INTEL;
		else if( namehash == SPELL_HASH_AMPLIFY_MAGIC || namehash == SPELL_HASH_DAMPEN_MAGIC )
            type |= SPELL_TYPE_MAGE_MAGI;
		else if( namehash == SPELL_HASH_FIRE_WARD || namehash == SPELL_HASH_FROST_WARD )
            type |= SPELL_TYPE_MAGE_WARDS;
		else if( namehash == SPELL_HASH_SHADOW_PROTECTION || namehash == SPELL_HASH_PRAYER_OF_SHADOW_PROTECTION )
            type |= SPELL_TYPE_PRIEST_SH_PPROT;
		else if( namehash == SPELL_HASH_WATER_SHIELD || namehash == SPELL_HASH_EARTH_SHIELD || namehash == SPELL_HASH_LIGHTNING_SHIELD )
            type |= SPELL_TYPE_SHIELD;
		else if( namehash == SPELL_HASH_POWER_WORD__FORTITUDE || namehash == SPELL_HASH_PRAYER_OF_FORTITUDE )
            type |= SPELL_TYPE_FORTITUDE;
		else if( namehash == SPELL_HASH_DIVINE_SPIRIT || namehash == SPELL_HASH_PRAYER_OF_SPIRIT )
            type |= SPELL_TYPE_SPIRIT;
//		else if( strstr( nametext, "Curse of Weakness") || strstr( nametext, "Curse of Agony") || strstr( nametext, "Curse of Recklessness") || strstr( nametext, "Curse of Tongues") || strstr( nametext, "Curse of the Elements") || strstr( nametext, "Curse of Idiocy") || strstr( nametext, "Curse of Shadow") || strstr( nametext, "Curse of Doom"))
//		else if(namehash==4129426293 || namehash==885131426 || namehash==626036062 || namehash==3551228837 || namehash==2784647472 || namehash==776142553 || namehash==3407058720 || namehash==202747424)
//		else if( strstr( nametext, "Curse of "))
//            type |= SPELL_TYPE_WARLOCK_CURSES;
		else if( strstr( nametext, "Immolate") || strstr( nametext, "Conflagrate"))
			type |= SPELL_TYPE_WARLOCK_IMMOLATE;
		else if( strstr( nametext, "Amplify Magic") || strstr( nametext, "Dampen Magic"))
			type |= SPELL_TYPE_MAGE_AMPL_DUMP;
        else if( strstr( desc, "Battle Elixir"))
            type |= SPELL_TYPE_ELIXIR_BATTLE;
        else if( strstr( desc, "Guardian Elixir"))
            type |= SPELL_TYPE_ELIXIR_GUARDIAN;
        else if( strstr( desc, "Battle and Guardian elixir"))
            type |= SPELL_TYPE_ELIXIR_FLASK;
		else if( namehash == SPELL_HASH_HUNTER_S_MARK )		// hunter's mark
			type |= SPELL_TYPE_HUNTER_MARK;
        else if( namehash == SPELL_HASH_COMMANDING_SHOUT || namehash == SPELL_HASH_BATTLE_SHOUT )
            type |= SPELL_TYPE_WARRIOR_SHOUT;
		else if( strstr( desc, "Finishing move")==desc)
			sp->c_is_flags |= SPELL_FLAG_IS_FINISHING_MOVE;
		if( IsDamagingSpell( sp ) )
			sp->c_is_flags |= SPELL_FLAG_IS_DAMAGING;
		if( IsHealingSpell( sp ) )
			sp->c_is_flags |= SPELL_FLAG_IS_HEALING;
		if( IsTargetingStealthed( sp ) )
			sp->c_is_flags |= SPELL_FLAG_IS_TARGETINGSTEALTHED;
		

		//stupid spell ranking problem
		if(sp->spellLevel==0)
		{
			uint32 new_level=0;
			if( strstr( nametext, "Apprentice "))
				new_level = 1;
			else if( strstr( nametext, "Journeyman "))
				new_level = 2;
			else if( strstr( nametext, "Expert "))
				new_level = 3;
			else if( strstr( nametext, "Artisan "))
				new_level = 4;
			else if( strstr( nametext, "Master "))
				new_level = 5;
			if(new_level!=0)
			{
				uint32 teachspell=0;
				if(sp->Effect[0]==SPELL_EFFECT_LEARN_SPELL)
					teachspell = sp->EffectTriggerSpell[0];
				else if(sp->Effect[1]==SPELL_EFFECT_LEARN_SPELL)
					teachspell = sp->EffectTriggerSpell[1];
				else if(sp->Effect[2]==SPELL_EFFECT_LEARN_SPELL)
					teachspell = sp->EffectTriggerSpell[2];
				if(teachspell)
				{
					SpellEntry *spellInfo;
					spellInfo = dbcSpell.LookupEntryForced(teachspell);
					spellInfo->spellLevel = new_level;
					sp->spellLevel = new_level;
				}
			}
		}

		/*FILE * f = fopen("C:\\spells.txt", "a");
		fprintf(f, "case 0x%08X:		// %s\n", namehash, nametext);
		fclose(f);*/

		// find diminishing status
		sp->DiminishStatus = GetDiminishingGroup(namehash);
		sp->buffIndexType=0;
		switch(namehash)
		{
		case SPELL_HASH_HUNTER_S_MARK:		// Hunter's mark
			sp->buffIndexType = SPELL_TYPE_INDEX_MARK;
			break;

		case SPELL_HASH_POLYMORPH:			// Polymorph
		case SPELL_HASH_POLYMORPH__CHICKEN:	// Polymorph: Chicken
		case SPELL_HASH_POLYMORPH__PIG:		// Polymorph: Pig
		case SPELL_HASH_POLYMORPH__SHEEP:	// Polymorph: Sheep
		case SPELL_HASH_POLYMORPH__TURTLE:	// Polymorph: Turtle
			sp->buffIndexType = SPELL_TYPE_INDEX_POLYMORPH;
			break;

		case SPELL_HASH_FEAR:				// Fear
			sp->buffIndexType = SPELL_TYPE_INDEX_FEAR;
			break;

		case SPELL_HASH_SAP:				// Sap
			sp->buffIndexType = SPELL_TYPE_INDEX_SAP;
			break;

		case SPELL_HASH_SCARE_BEAST:		// Scare Beast
			sp->buffIndexType = SPELL_TYPE_INDEX_SCARE_BEAST;
			break;

		case SPELL_HASH_HIBERNATE:			// Hibernate
			sp->buffIndexType = SPELL_TYPE_INDEX_HIBERNATE;
			break;

//		removed by Zack Earth shield stacks 10 times. Current code does not support it
//		case SPELL_HASH_EARTH_SHIELD:		// Earth Shield
//			sp->buffIndexType = SPELL_TYPE_INDEX_EARTH_SHIELD;
//			break;

		case SPELL_HASH_CYCLONE:			// Cyclone
			sp->buffIndexType = SPELL_TYPE_INDEX_CYCLONE;
			break;

		case SPELL_HASH_BANISH:				// Banish
			sp->buffIndexType = SPELL_TYPE_INDEX_BANISH;
			break;

		//case SPELL_HASH_JUDGEMENT_OF_VENGEANCE:
		case SPELL_HASH_JUDGEMENT_OF_THE_CRUSADER:
		case SPELL_HASH_JUDGEMENT_OF_LIGHT:
		case SPELL_HASH_JUDGEMENT_OF_WISDOM:
		case SPELL_HASH_JUDGEMENT_OF_JUSTICE:
			sp->buffIndexType = SPELL_TYPE_INDEX_JUDGEMENT;
			break;
		}

		// HACK FIX: Break roots/fear on damage.. this needs to be fixed properly!
		if(!(sp->AuraInterruptFlags & AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN))
		{
			for(uint32 z = 0; z < 3; ++z) {
				if(sp->EffectApplyAuraName[z] == SPELL_AURA_MOD_FEAR ||
					sp->EffectApplyAuraName[z] == SPELL_AURA_MOD_ROOT)
				{
					sp->AuraInterruptFlags |= AURA_INTERRUPT_ON_UNUSED2;
					break;
				}

				if( ( sp->Effect[z] == SPELL_EFFECT_SCHOOL_DAMAGE && sp->Spell_Dmg_Type == SPELL_DMG_TYPE_MELEE ) || sp->Effect[z] == SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL || sp->Effect[z] == SPELL_EFFECT_WEAPON_DAMAGE || sp->Effect[z] == SPELL_EFFECT_WEAPON_PERCENT_DAMAGE || sp->Effect[z] == SPELL_EFFECT_DUMMYMELEE )
					sp->is_melee_spell = true;
				if( ( sp->Effect[z] == SPELL_EFFECT_SCHOOL_DAMAGE && sp->Spell_Dmg_Type == SPELL_DMG_TYPE_RANGED ) )
				{
					//Log.Notice( "SpellFixes" , "Ranged Spell: %u [%s]" , sp->Id , sp->Name );
					sp->is_ranged_spell = true;
				}
			}
		}

		// set extra properties
		sp->buffType   = type;
		sp->RankNumber = rank;

		uint32 pr=sp->procFlags;
		for(uint32 y=0;y < 3; y++)
		{
			// get the effect number from the spell
			effect = dbc.getRecord(x).getUInt(64 + y); // spelleffect[0] = 64 // 2.0.1

			//spell group
			/*if(effect==SPELL_EFFECT_SUMMON_TOTEM_SLOT1||effect==SPELL_EFFECT_SUMMON_TOTEM_SLOT2||
				effect==SPELL_EFFECT_SUMMON_TOTEM_SLOT3||effect==SPELL_EFFECT_SUMMON_TOTEM_SLOT4)
			{
			
					const char *p=desc;
					while(p=strstr(p,"$"))
					{
						p++;
						//got $  -> check if spell
						if(*p>='0' && *p <='9')
						{//woot this is spell id
							uint32 tmp=atoi(p);
							SpellEntry*s=sSpellStore.LookupEntryForced(tmp);
							bool ch=false;
							for(uint32 i=0;i<3;i++)
								if(s->EffectTriggerSpell[i])
								{
									ch=true;
									result=tmp;
									break;
								}
							if(ch)break;
							result=tmp;
							
						}
					
					}
				
			}else*/
			/*if(effect==SPELL_EFFECT_ENCHANT_ITEM)//add inventory type check
			{
				result=0;
				//136--desc field
				//dirty code
				if( strstr( desc,"head"))
					result|=(1<<INVTYPE_HEAD);
				if( strstr( desc,"leg"))
					result|=(1<<INVTYPE_LEGS);
				if( strstr( desc,"neck"))
					result|=(1<<INVTYPE_NECK);
				if( strstr( desc,"shoulder"))
					result|=(1<<INVTYPE_SHOULDERS);
				if( strstr( desc,"body"))
					result|=(1<<INVTYPE_BODY);
				if( strstr( desc,"chest"))
					result|=((1<<INVTYPE_CHEST)|(1<<INVTYPE_ROBE));
				if( strstr( desc,"waist"))
					result|=(1<<INVTYPE_WAIST);
				if( strstr( desc,"foot")||strstr(desc,"feet")||strstr(desc,"boot"))
					result|=(1<<INVTYPE_FEET);
				if( strstr( desc,"wrist")||strstr(desc,"bracer"))
					result|=(1<<INVTYPE_WRISTS);
				if( strstr( desc,"hand")||strstr(desc,"glove"))
					result|=(1<<INVTYPE_HANDS);
				if( strstr( desc,"finger")||strstr(desc,"ring"))
					result|=(1<<INVTYPE_FINGER);
				if( strstr( desc,"trinket"))
					result|=(1<<INVTYPE_TRINKET);
				if( strstr( desc,"shield"))
					result|=(1<<INVTYPE_SHIELD);
				if( strstr( desc,"cloak"))
					result|=(1<<INVTYPE_CLOAK);
				if( strstr( desc,"robe"))
					result|=(1<<INVTYPE_ROBE);
				//if( strstr( desc,"two")||strstr(desc,"Two"))
				//	result|=(1<<INVTYPE_2HWEAPON);<-handled in subclass
			}
			else*/
			if(effect==SPELL_EFFECT_APPLY_AURA)
			{
				uint32 aura = dbc.getRecord(x).getUInt(94+y); // 58+30+3 = 91
				if( aura == SPELL_AURA_PROC_TRIGGER_SPELL ||
					aura == SPELL_AURA_PROC_TRIGGER_DAMAGE
					)//search for spellid in description
				{
					const char *p=desc;
					while((p=strstr(p,"$")))
					{
						p++;
						//got $  -> check if spell
						if(*p>='0' && *p <='9')
						{//woot this is spell id
						
							result=atoi(p);
						}					
					}
					pr=0;

					uint32 len = (uint32)strlen(desc);
					for(i = 0; i < len; ++i)
						desc[i] = tolower(desc[i]);
					//dirty code for procs, if any1 got any better idea-> u are welcome
					//139944 --- some magic number, it will trigger on all hits etc
						//for seems to be smth like custom check
					if( strstr( desc,"your ranged criticals"))
						pr|=PROC_ON_RANGED_CRIT_ATTACK;
					if( strstr( desc,"chance on hit"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"takes damage"))
						pr|=PROC_ON_ANY_DAMAGE_VICTIM;
					if( strstr( desc,"attackers when hit"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( desc,"character strikes an enemy"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"strike you with a melee attack"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( desc,"target casts a spell"))
						pr|=PROC_ON_CAST_SPELL;
                    if( strstr( desc,"your harmful spells land"))
                        pr|=PROC_ON_CAST_SPELL;
                    if( strstr( desc,"on spell critical hit"))
                        pr|=PROC_ON_SPELL_CRIT_HIT;
                    if( strstr( desc,"spell critical strikes"))
                        pr|=PROC_ON_SPELL_CRIT_HIT;
                    if( strstr( desc,"being able to resurrect"))
                        pr|=PROC_ON_DIE;
					if( strstr( desc,"any damage caused"))
						pr|=PROC_ON_ANY_DAMAGE_VICTIM;
					if( strstr( desc,"the next melee attack against the caster"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( desc,"when successfully hit"))
						pr|=PROC_ON_MELEE_ATTACK ;
					if( strstr( desc,"an enemy on hit"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"when it hits"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"when successfully hit"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"on a successful hit"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"damage to attacker on hit"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( desc,"on a hit"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"strikes you with a melee attack"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( desc,"when caster takes damage"))
						pr|=PROC_ON_ANY_DAMAGE_VICTIM;
					if( strstr( desc,"when the caster is using melee attacks"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"when struck in combat") || strstr(desc,"When struck in combat"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( desc,"successful melee attack"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"chance per attack"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"chance per hit"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"that strikes a party member"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( desc,"when hit by a melee attack"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( desc,"landing a melee critical strike"))
						pr|=PROC_ON_CRIT_ATTACK;
					if( strstr( desc,"your critical strikes"))
						pr|=PROC_ON_CRIT_ATTACK;
					if( strstr( desc,"whenever you deal ranged damage"))
						pr|=PROC_ON_RANGED_ATTACK;
//					if( strstr( desc,"whenever you deal melee damage"))
					if( strstr( desc,"you deal melee damage"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"your melee attacks"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"damage with your Sword"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"when struck in melee combat"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( desc,"any successful spell cast against the priest"))
						pr|=PROC_ON_SPELL_HIT_VICTIM;
					if( strstr( desc,"the next melee attack on the caster"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( desc,"striking melee or ranged attackers"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM|PROC_ON_RANGED_ATTACK_VICTIM;
					if( strstr( desc,"when damaging an enemy in melee"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"victim of a critical strike"))
						pr|=PROC_ON_CRIT_HIT_VICTIM;
					if( strstr( desc,"on successful melee or ranged attack"))
						pr|=PROC_ON_MELEE_ATTACK|PROC_ON_RANGED_ATTACK;
					if( strstr( desc,"enemy that strikes you in melee"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( desc,"after getting a critical strike"))
						pr|=PROC_ON_CRIT_ATTACK;
					if( strstr( desc,"whenever damage is dealt to you"))
						pr|=PROC_ON_ANY_DAMAGE_VICTIM;
					if( strstr( desc,"when ranged or melee damage is dealt"))
						pr|=PROC_ON_MELEE_ATTACK|PROC_ON_RANGED_ATTACK;
					if( strstr( desc,"damaging melee attacks"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"on melee or ranged attack"))
						pr|=PROC_ON_MELEE_ATTACK|PROC_ON_RANGED_ATTACK;
					if( strstr( desc,"on a melee swing"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"Chance on melee"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"spell criticals against you"))
						pr|=PROC_ON_SPELL_CRIT_HIT_VICTIM;
					if( strstr( desc,"after being struck by a melee or ranged critical hit"))
						pr|=PROC_ON_CRIT_HIT_VICTIM;
//					if( strstr( desc,"on a critical hit"))
					if( strstr( desc,"critical hit"))
						pr|=PROC_ON_CRIT_ATTACK;
					if( strstr( desc,"strikes the caster"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( desc,"a spell, melee or ranged attack hits the caster"))
						pr|=PROC_ON_ANY_DAMAGE_VICTIM;
					if( strstr( desc,"after dealing a critical strike"))
						pr|=PROC_ON_CRIT_ATTACK;
					if( strstr( desc,"each melee or ranged damage hit against the priest"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM|PROC_ON_RANGED_ATTACK_VICTIM;				
					if( strstr( desc, "a chance to deal additional"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc, "chance to get an extra attack"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc, "melee attacks has"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc, "any damage spell hits a target"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc, "giving each melee attack a chance"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc, "damage when hit"))
						pr|=PROC_ON_ANY_DAMAGE_VICTIM; //myabe melee damage ?
					if( strstr( desc, "gives your"))
					{
						if( strstr( desc, "melee"))
							pr|=PROC_ON_MELEE_ATTACK;
						else if( strstr( desc,"sinister strike, backstab, gouge and shiv"))
							pr|=PROC_ON_CAST_SPELL;
						else if( strstr( desc,"chance to daze the target"))
							pr|=PROC_ON_CAST_SPELL;
						else if( strstr( desc,"finishing moves"))
							pr|=PROC_ON_CAST_SPELL;
//						else if( strstr( desc,"shadow bolt, shadowburn, soul fire, incinerate, searing pain and conflagrate"))
//							pr|=PROC_ON_CAST_SPELL|PROC_TARGET_SELF;
						//we should find that specific spell (or group) on what we will trigger
						else pr|=PROC_ON_CAST_SPECIFIC_SPELL;
					}
					if( strstr( desc, "chance to add an additional combo") && strstr(desc, "critical") )
						pr|=PROC_ON_CRIT_ATTACK;
					else if( strstr( desc, "chance to add an additional combo"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc, "victim of a melee or ranged critical strike"))
						pr|=PROC_ON_CRIT_HIT_VICTIM;
					if( strstr( desc, "getting a critical effect from"))
						pr|=PROC_ON_SPELL_CRIT_HIT_VICTIM;
					if( strstr( desc, "damaging attack is taken"))
						pr|=PROC_ON_ANY_DAMAGE_VICTIM;
					if( strstr( desc, "struck by a Stun or Immobilize"))
						pr|=PROC_ON_SPELL_HIT_VICTIM;
					if( strstr( desc, "melee critical strike"))
						pr|=PROC_ON_CRIT_ATTACK;
					if( strstr( nametext, "Bloodthirst"))
						pr|=PROC_ON_MELEE_ATTACK | PROC_TARGET_SELF;
					if( strstr( desc, "experience or honor"))
						pr|=PROC_ON_GAIN_EXPIERIENCE;
					if( strstr( desc,"your next offensive ability"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc,"hit by a melee or ranged attack"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM;
					if( strstr( desc,"enemy strikes the caster"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( desc,"melee and ranged attacks against you"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM;
					if( strstr( desc,"when a block occurs"))
						pr|=PROC_ON_BLOCK_VICTIM;
					if( strstr( desc,"dealing a critical strike from a weapon swing, spell, or ability"))
						pr|=PROC_ON_CRIT_ATTACK|PROC_ON_SPELL_CRIT_HIT;
					if( strstr( desc,"dealing a critical strike from a weapon swing, spell, or ability"))
						pr|=PROC_ON_CRIT_ATTACK|PROC_ON_SPELL_CRIT_HIT;
					if( strstr( desc,"shadow bolt critical strikes increase shadow damage"))
						pr|=PROC_ON_SPELL_CRIT_HIT;
					if( strstr( desc,"next offensive ability"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc,"after being hit with a shadow or fire spell"))
						pr|=PROC_ON_SPELL_LAND_VICTIM;
					if( strstr( desc,"giving each melee attack"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"each strike has"))
						pr|=PROC_ON_MELEE_ATTACK;		
					if( strstr( desc,"your Fire damage spell hits"))
						pr|=PROC_ON_CAST_SPELL;		//this happens only on hit ;)
					if( strstr( desc,"corruption, curse of agony, siphon life and seed of corruption spells also cause"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc,"pain, mind flay and vampiric touch spells also cause"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc,"shadow damage spells have"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc,"on successful spellcast"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc,"your spell criticals have"))
						pr|=PROC_ON_SPELL_CRIT_HIT | PROC_ON_SPELL_CRIT_HIT_VICTIM;
					if( strstr( desc,"after dodging their attack"))
					{
						pr|=PROC_ON_DODGE_VICTIM;
						if( strstr( desc,"add a combo point"))
							pr|=PROC_TARGET_SELF;
					}
					if( strstr( desc,"fully resisting"))
						pr|=PROC_ON_RESIST_VICTIM;
					if( strstr( desc,"Your Shadow Word: Pain, Mind Flay and Vampiric Touch spells also cause the target"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc,"chance on spell hit"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc,"your melee and ranged attacks"))
						pr|=PROC_ON_MELEE_ATTACK|PROC_ON_RANGED_ATTACK;
//					if( strstr( desc,"chill effect to your Blizzard"))
//						pr|=PROC_ON_CAST_SPELL;	
					//////////////////////////////////////////////////
					//proc dmg flags
					//////////////////////////////////////////////////
					if( strstr( desc,"each attack blocked"))
						pr|=PROC_ON_BLOCK_VICTIM;
					if( strstr( desc,"into flame, causing an additional"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"victim of a critical melee strike"))
						pr|=PROC_ON_CRIT_HIT_VICTIM;
					if( strstr( desc,"damage to melee attackers"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"target blocks a melee attack"))
						pr|=PROC_ON_BLOCK_VICTIM;
					if( strstr( desc,"ranged and melee attacks to deal"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM;
					if( strstr( desc,"damage on hit"))
						pr|=PROC_ON_ANY_DAMAGE_VICTIM;
					if( strstr( desc,"chance on hit"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"after being hit by any damaging attack"))
						pr|=PROC_ON_ANY_DAMAGE_VICTIM;
					if( strstr( desc,"striking melee or ranged attackers"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM;
					if( strstr( desc,"damage to attackers when hit"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( desc,"striking melee attackers"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( desc,"whenever the caster takes damage"))
						pr|=PROC_ON_ANY_DAMAGE_VICTIM;
					if( strstr( desc,"damage on every attack"))
						pr|=PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK;
					if( strstr( desc,"chance to reflect Fire spells"))
						pr|=PROC_ON_SPELL_HIT_VICTIM;
					if( strstr( desc,"hunter takes on the aspects of a hawk"))
						pr|=PROC_TARGET_SELF | PROC_ON_RANGED_ATTACK;
					if( strstr( desc,"successful auto shot attacks"))
						pr|=PROC_ON_AUTO_SHOT_HIT;
					if( strstr( desc,"after getting a critical effect from your"))
						pr=PROC_ON_SPELL_CRIT_HIT;
//					if( strstr( desc,"Your critical strikes from Fire damage"))
//						pr|=PROC_ON_SPELL_CRIT_HIT;
				}//end "if procspellaura"
				//dirty fix to remove auras that should expire on event and they are not
//				else if(sp->procCharges>0)
//				{
					//there are at least 185 spells that should loose charge uppon some event.Be prepared to add more here !
					// ! watch it cause this might conflict with our custom modified spells like : lighning shield !

					//spells like : Presence of Mind,Nature's Swiftness, Inner Focus,Amplify Curse,Coup de Grace
					//SELECT * FROM dbc_spell where proc_charges!=0 and (effect_aura_1=108 or effect_aura_2=108 and effect_aura_3=108) and description!=""
//					if(aura == SPELL_AURA_ADD_PCT_MODIFIER)
//						sp->AuraInterruptFlags |= AURA_INTERRUPT_ON_CAST_SPELL;
					//most of them probably already have these flags...not sure if we should add to all of them without checking
/*					if( strstr( desc, "melee"))
						sp->AuraInterruptFlags |= AURA_INTERRUPT_ON_START_ATTACK;
					if( strstr( desc, "ranged"))
						sp->AuraInterruptFlags |= AURA_INTERRUPT_ON_START_ATTACK;*/
//				}
			}//end "if aura"
		}//end "for each effect"
		sp->procFlags = pr;

		if( strstr( desc, "Must remain seated"))
		{
			sp->RecoveryTime = 1000;
			sp->CategoryRecoveryTime = 1000;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// procintervals
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//omg lighning shield trigger spell id's are all wrong ?
		//if you are bored you could make thiese by hand but i guess we might find other spells with this problem..and this way it's safe
		if( strstr( nametext, "Lightning Shield" ) && sp->EffectTriggerSpell[0] )
		{
			//check if we can find in the desription
			char *startofid = strstr(desc, "for $");
			if( startofid )
			{
				startofid += strlen("for $");
				sp->EffectTriggerSpell[0] = atoi( startofid ); //get new lightning shield trigger id
			}
			sp->proc_interval = 3000; //few seconds
		}
		//mage ignite talent should proc only on some chances
		else if( strstr( nametext, "Ignite") && sp->Id>=11119 && sp->Id<=12848 && sp->EffectApplyAuraName[0] == 4 )
		{
			//check if we can find in the desription
			char *startofid=strstr(desc, "an additional ");
			if(startofid)
			{
				startofid += strlen("an additional ");
				sp->EffectBasePoints[0]=atoi(startofid); //get new value. This is actually level*8 ;)
			}
			sp->Effect[0] = 6; //aura
			sp->EffectApplyAuraName[0] = 42; //force him to use procspell effect
			sp->EffectTriggerSpell[0] = 12654; //evil , but this is good for us :D
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT; //add procflag here since this was not processed with the others !
		}
		// Winter's Chill handled by frost school
		else if( strstr( nametext, "Winter's Chill"))
		{
			sp->School = 4;
		}
		// Blackout handled by Shadow school
		else if( strstr( nametext, "Blackout"))
		{
			sp->School = 5;
		}
#ifndef NEW_PROCFLAGS
		// Shadow Weaving
		else if( strstr( nametext, "Shadow Weaving"))
		{
			sp->School = 5;
			sp->EffectApplyAuraName[0] = 42;
			sp->procChance = sp->EffectBasePoints[0] + 1;
			sp->procFlags = PROC_ON_CAST_SPECIFIC_SPELL;
		}
#endif
		//Improved Aspect of the Hawk
		else if( strstr( nametext, "Improved Aspect of the Hawk"))
			sp->EffectSpellGroupRelation[1] = 0x00100000;
		//more triggered spell ids are wrong. I think blizz is trying to outsmart us :S
		else if( strstr( nametext, "Nature's Guardian"))
		{
			sp->EffectTriggerSpell[0] = 31616;
			sp->proc_interval = 5000;
		}
		//Chain Heal all ranks %50 heal value (49 + 1)
		else if( strstr( nametext, "Chain Heal"))
		{
			sp->EffectDieSides[0] = 49;
		}
		//this starts to be an issue for trigger spell id : Deep Wounds
		else if( strstr( nametext, "Deep Wounds") && sp->EffectTriggerSpell[0])
		{
			//check if we can find in the desription
			char *startofid=strstr(desc, "over $");
			if(startofid)
			{
				startofid += strlen("over $");
				sp->EffectTriggerSpell[0] = atoi(startofid);
			}
		}
		else if( strstr( nametext, "Holy Shock"))
		{
			//check if we can find in the desription
			char *startofid=strstr(desc, "causing $");
			if(startofid)
			{
				startofid += strlen("causing $");
				sp->EffectTriggerSpell[0] = atoi(startofid);
			}
			//check if we can find in the desription
			startofid=strstr(desc, " or $");
			if(startofid)
			{
				startofid += strlen(" or $");
				sp->EffectTriggerSpell[1]=atoi(startofid);
			}
		}
		else if( strstr( nametext, "Touch of Weakness"))
		{
			//check if we can find in the desription
			char *startofid=strstr(desc, "cause $");
			if(startofid)
			{
				startofid += strlen("cause $");
				sp->EffectTriggerSpell[0] = atoi(startofid);
				sp->EffectTriggerSpell[1]=sp->EffectTriggerSpell[0]; //later versions of this spell changed to eff[1] the aura
				sp->procFlags = uint32(PROC_ON_MELEE_ATTACK_VICTIM);
			}
		}
		else if( strstr( nametext, "Firestone Passive"))
		{
			//Enchants the main hand weapon with fire, granting each attack a chance to deal $17809s1 additional fire damage.
			//check if we can find in the desription
			char * startofid=strstr(desc, "to deal $");
			if(startofid)
			{
				startofid += strlen("to deal $");
				sp->EffectTriggerSpell[0] = atoi(startofid);
				sp->EffectApplyAuraName[0] = 42;
				sp->procFlags = PROC_ON_MELEE_ATTACK;
				sp->procChance = 50;
			}
		}
		//some procs trigger at intervals
		else if( strstr( nametext, "Water Shield"))
		{
			sp->proc_interval = 3000; //few seconds
			sp->procFlags |= PROC_TARGET_SELF;
		}
		else if( strstr( nametext, "Earth Shield"))
			sp->proc_interval = 3000; //few seconds
		else if( strstr( nametext, "Shadowguard"))
			sp->proc_interval = 3000; //few seconds
		else if( strstr( nametext, "Poison Shield"))
			sp->proc_interval = 3000; //few seconds
		else if( strstr( nametext, "Infused Mushroom"))
			sp->proc_interval = 10000; //10 seconds
		else if( strstr( nametext, "Aviana's Purpose"))
			sp->proc_interval = 10000; //10 seconds
		//don't change to namehash since we are searching only a protion of the name
 		else if( strstr( nametext, "Crippling Poison"))
		{
			sp->SpellGroupType |= 16384; //some of them do have the flags but i's hard to write down those some from 130 spells
			sp->c_is_flags |= SPELL_FLAG_IS_POISON;
		}
		else if( strstr( nametext, "Mind-Numbing Poison"))
		{
			sp->SpellGroupType |= 32768; //some of them do have the flags but i's hard to write down those some from 130 spells
			sp->c_is_flags |= SPELL_FLAG_IS_POISON;
		}
		else if( strstr( nametext, "Instant Poison"))
		{
			sp->SpellGroupType |= 8192; //some of them do have the flags but i's hard to write down those some from 130 spells
			sp->c_is_flags |= SPELL_FLAG_IS_POISON;
		}
		else if( strstr( nametext, "Deadly Poison"))
		{
			sp->SpellGroupType |= 65536; //some of them do have the flags but i's hard to write down those some from 130 spells
			sp->c_is_flags |= SPELL_FLAG_IS_POISON;
		}
		else if( strstr( nametext, "Wound Poison"))
		{
			sp->SpellGroupType |= 268435456; //some of them do have the flags but i's hard to write down those some from 130 spells
			sp->c_is_flags |= SPELL_FLAG_IS_POISON;
		}
		else if( strstr( nametext, "Scorpid Poison") )
		{
			// groups?
			sp->c_is_flags |= SPELL_FLAG_IS_POISON;
		}

		//warlock - shadow bolt
		if( sp->NameHash == SPELL_HASH_SHADOW_BOLT )
			sp->SpellGroupType |= 1; //some of them do have the flags but i's hard to write down those some from 130 spells

		//mage Ice Floes affects these spells : Cone of Cold,Cold Snap,Ice Barrier,Ice Block
		if( sp->NameHash == SPELL_HASH_CONE_OF_COLD || sp->NameHash == SPELL_HASH_COLD_SNAP || sp->NameHash == SPELL_HASH_ICE_BARRIER || sp->NameHash == SPELL_HASH_ICE_BLOCK )
			sp->EffectSpellGroupRelation[0] = 0x00200000;

/*		else if( strstr( nametext, "Anesthetic Poison"))
			sp->SpellGroupType |= 0; //not yet known ? 
		else if( strstr( nametext, "Blinding Powder"))
			sp->SpellGroupType |= 0; //not yet known ?
		else 
		if( sp->NameHash == SPELL_HASH_ILLUMINATION )
			sp->EffectTriggerSpell[0] = 20272;*/  // broken trigger spell, do not use
		//sp->dummy=result;
/*		//if there is a proc spell and has 0 as charges then it's probably going to triger infinite times. Better not save these
		if(sp->procCharges==0)
			sp->procCharges=-1;*/
		if( sp->NameHash == SPELL_HASH_ILLUMINATION )
			sp->procFlags |= PROC_TARGET_SELF;

		// Set default mechanics if we don't already have one
		if( !sp->MechanicsType )
		{
			//Set Silencing spells mechanic.
			if( sp->EffectApplyAuraName[0] == 27 || 
				sp->EffectApplyAuraName[1] == 27 ||
				sp->EffectApplyAuraName[2] == 27 )
				sp->MechanicsType = MECHANIC_SILENCED;
	
			//Set Stunning spells mechanic.
			if( sp->EffectApplyAuraName[0] == 12 || 
				sp->EffectApplyAuraName[1] == 12 ||
				sp->EffectApplyAuraName[2] == 12 )
				sp->MechanicsType = MECHANIC_STUNNED;
	
			//Set Fearing spells mechanic
			if( sp->EffectApplyAuraName[0] == 7 || 
				sp->EffectApplyAuraName[1] == 7 ||
				sp->EffectApplyAuraName[2] == 7 )
				sp->MechanicsType = MECHANIC_FLEEING;
		}

		if( sp->proc_interval != 0 )
			sp->procFlags |= PROC_REMOVEONUSE;

		// Seal of Command - Proc Chance
		if( sp->NameHash == SPELL_HASH_SEAL_OF_COMMAND )
		{
			sp->procChance = 25;
			sp->School = SCHOOL_HOLY; //the procspells of the original seal of command have fizical school instead of holy
			sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC; //heh, crazy spell uses melee/ranged/magic dmg type for 1 spell. Now which one is correct ?
		}

		//Seal of Jusice - Proc Chance
		if( sp->NameHash == SPELL_HASH_SEAL_OF_JUSTICE )
			sp->procChance = 25;

		/* Decapitate */
		if( sp->NameHash == SPELL_HASH_DECAPITATE )
			sp->procChance = 30;

		//shaman - shock, has no spellgroup.very dangerous move !
		if( sp->NameHash == SPELL_HASH_SHOCK )
			sp->SpellGroupType = 4;

		//mage - fireball. Only some of the spell has the flags 
		if( sp->NameHash == SPELL_HASH_FIREBALL )
			sp->SpellGroupType |= 1;

		if( sp->NameHash == SPELL_HASH_DIVINE_SHIELD || sp->NameHash == SPELL_HASH_DIVINE_PROTECTION || sp->NameHash == SPELL_HASH_BLESSING_OF_PROTECTION )
			sp->MechanicsType = 25;

		/* hackfix for this - FIX ME LATER - Burlex */
		if( namehash == SPELL_HASH_SEAL_FATE )
			sp->procFlags = 0;

		if(
			((sp->Attributes & ATTRIBUTES_TRIGGER_COOLDOWN) && (sp->AttributesEx & ATTRIBUTESEX_NOT_BREAK_STEALTH)) //rogue cold blood
			|| ((sp->Attributes & ATTRIBUTES_TRIGGER_COOLDOWN) && (!sp->AttributesEx || sp->AttributesEx & ATTRIBUTESEX_REMAIN_OOC))
			)
		{
			sp->c_is_flags |= SPELL_FLAG_IS_REQUIRECOOLDOWNUPDATE;
		}

		if( namehash == SPELL_HASH_SHRED || namehash == SPELL_HASH_BACKSTAB || namehash == SPELL_HASH_AMBUSH )
		{
			// Shred, Backstab, Ambush
			sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_INBACK;
		}

		if( namehash == SPELL_HASH_FEAR || namehash == SPELL_HASH_POLYMORPH )
			sp->in_front_status = SPELL_INFRONT_STATUS_REQUIRE_SKIPCHECK;

//junk code to get me has :P 
//if(sp->Id==11267 || sp->Id==11289 || sp->Id==6409)
//	printf("!!!!!!! name %s , id %u , hash %u \n",nametext,sp->Id, namehash);
	}


	/////////////////////////////////////////////////////////////////
	//SPELL COEFFICIENT SETTINGS START
	//////////////////////////////////////////////////////////////////

	for(uint32 x=0; x < cnt; x++)
	{
		// SpellID
		uint32 spellid = dbc.getRecord(x).getUInt(0);
		// get spellentry
		SpellEntry * sp = dbcSpell.LookupEntry(spellid);

		//Setting Cast Time Coefficient
		SpellCastTime *sd = dbcSpellCastTime.LookupEntry(sp->CastingTimeIndex);
		float castaff = float(GetCastTime(sd));
		if(castaff < 1500) castaff = 1500;
		else
			if(castaff > 7000) castaff = 7000;

		sp->casttime_coef = castaff / 3500;		 

		SpellEntry * spz;
		bool spcheck = false;

		//Flag for DoT and HoT
		for( uint8 i = 0 ; i < 3 ; i++ )
		{
			if (sp->EffectApplyAuraName[i] == SPELL_AURA_PERIODIC_DAMAGE ||
				sp->EffectApplyAuraName[i] == SPELL_AURA_PERIODIC_HEAL ||
				sp->EffectApplyAuraName[i] == SPELL_AURA_PERIODIC_LEECH )
			{
				sp->spell_coef_flags |= SPELL_FLAG_IS_DOT_OR_HOT_SPELL;
				break;
			}
		}

		//Flag for DD or DH
		for( uint8 i = 0 ; i < 3 ; i++ )
		{
			if ( sp->EffectApplyAuraName[i] == SPELL_AURA_PERIODIC_TRIGGER_SPELL && sp->EffectTriggerSpell[i] )
			{
				spz = dbcSpell.LookupEntry( sp->EffectTriggerSpell[i] );
				if( spz &&
					spz->Effect[i] == SPELL_EFFECT_SCHOOL_DAMAGE ||
					spz->Effect[i] == SPELL_EFFECT_HEAL
					) 
					spcheck = true;
			}
			if (sp->Effect[i] == SPELL_EFFECT_SCHOOL_DAMAGE ||
				sp->Effect[i] == SPELL_EFFECT_HEAL ||
				spcheck
				)
			{
				sp->spell_coef_flags |= SPELL_FLAG_IS_DD_OR_DH_SPELL;
				break;
			}
		}

		for(uint8 i = 0 ; i < 3; i++)
		{
			switch (sp->EffectImplicitTargetA[i])
			{
				//AoE
			case EFF_TARGET_ALL_TARGETABLE_AROUND_LOCATION_IN_RADIUS:
			case EFF_TARGET_ALL_ENEMY_IN_AREA:
			case EFF_TARGET_ALL_ENEMY_IN_AREA_INSTANT:
			case EFF_TARGET_ALL_PARTY_AROUND_CASTER:
			case EFF_TARGET_ALL_ENEMIES_AROUND_CASTER:
			case EFF_TARGET_IN_FRONT_OF_CASTER:
			case EFF_TARGET_ALL_ENEMY_IN_AREA_CHANNELED:
			case EFF_TARGET_ALL_PARTY_IN_AREA_CHANNELED:
			case EFF_TARGET_ALL_FRIENDLY_IN_AREA:
			case EFF_TARGET_ALL_TARGETABLE_AROUND_LOCATION_IN_RADIUS_OVER_TIME:
			case EFF_TARGET_ALL_PARTY:
			case EFF_TARGET_LOCATION_INFRONT_CASTER:
			case EFF_TARGET_BEHIND_TARGET_LOCATION:
			case EFF_TARGET_LOCATION_INFRONT_CASTER_AT_RANGE:
				{
					sp->spell_coef_flags |= SPELL_FLAG_AOE_SPELL;
					break;
				}
			}	
		}

		for(uint8 i = 0 ; i < 3 ; i++)
		{
			switch (sp->EffectImplicitTargetB[i])
			{
				//AoE
			case EFF_TARGET_ALL_TARGETABLE_AROUND_LOCATION_IN_RADIUS:
			case EFF_TARGET_ALL_ENEMY_IN_AREA:
			case EFF_TARGET_ALL_ENEMY_IN_AREA_INSTANT:
			case EFF_TARGET_ALL_PARTY_AROUND_CASTER:
			case EFF_TARGET_ALL_ENEMIES_AROUND_CASTER:
			case EFF_TARGET_IN_FRONT_OF_CASTER:
			case EFF_TARGET_ALL_ENEMY_IN_AREA_CHANNELED:
			case EFF_TARGET_ALL_PARTY_IN_AREA_CHANNELED:
			case EFF_TARGET_ALL_FRIENDLY_IN_AREA:
			case EFF_TARGET_ALL_TARGETABLE_AROUND_LOCATION_IN_RADIUS_OVER_TIME:
			case EFF_TARGET_ALL_PARTY:
			case EFF_TARGET_LOCATION_INFRONT_CASTER:
			case EFF_TARGET_BEHIND_TARGET_LOCATION:
			case EFF_TARGET_LOCATION_INFRONT_CASTER_AT_RANGE:
				{
					sp->spell_coef_flags |= SPELL_FLAG_AOE_SPELL;
					break;
				}
			}	
		}

		//Special Cases
		//Holy Light & Flash of Light
		if(sp->NameHash == SPELL_HASH_HOLY_LIGHT ||
			sp->NameHash == SPELL_HASH_FLASH_OF_LIGHT)
			sp->spell_coef_flags |= SPELL_FLAG_IS_DD_OR_DH_SPELL;

		//Additional Effect (not healing or damaging)
		for( uint8 i = 0 ; i < 3 ; i++ )
		{
			if(sp->Effect[i] == 0)
				continue;

			switch (sp->Effect[i])
			{
			case SPELL_EFFECT_SCHOOL_DAMAGE:
			case SPELL_EFFECT_ENVIRONMENTAL_DAMAGE:
			case SPELL_EFFECT_HEALTH_LEECH:
			case SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL:
			case SPELL_EFFECT_ADD_EXTRA_ATTACKS:
			case SPELL_EFFECT_WEAPON_PERCENT_DAMAGE:
			case SPELL_EFFECT_POWER_BURN:
			case SPELL_EFFECT_ATTACK:
			case SPELL_EFFECT_HEAL:
			case SPELL_EFFECT_HEALTH_FUNNEL:
			case SPELL_EFFECT_HEAL_MAX_HEALTH:
			case SPELL_EFFECT_DUMMY:
				continue;
			}

			switch (sp->EffectApplyAuraName[i])
			{
			case SPELL_AURA_PERIODIC_DAMAGE:
			case SPELL_AURA_PROC_TRIGGER_DAMAGE:
			case SPELL_AURA_PERIODIC_DAMAGE_PERCENT:
			case SPELL_AURA_POWER_BURN:
			case SPELL_AURA_PERIODIC_HEAL:
			case SPELL_AURA_MOD_INCREASE_HEALTH:
			case SPELL_AURA_PERIODIC_HEALTH_FUNNEL:
			case SPELL_AURA_DUMMY:
				continue;
			}

			sp->spell_coef_flags |= SPELL_FLAG_ADITIONAL_EFFECT;
			break;

		}

		//Calculating fixed coeficients
		//Channeled spells
		if( sp->ChannelInterruptFlags != 0 )
		{
			float Duration = float( GetDuration( dbcSpellDuration.LookupEntry( sp->DurationIndex ) ));
			if(Duration < 1500) Duration = 1500;
			else if(Duration > 7000) Duration = 7000;
			sp->fixed_hotdotcoef = (Duration / 3500.0f);

			if( sp->spell_coef_flags & SPELL_FLAG_ADITIONAL_EFFECT )
				sp->fixed_hotdotcoef *= 0.95f;
			if( sp->spell_coef_flags & SPELL_FLAG_AOE_SPELL )
				sp->fixed_hotdotcoef *= 0.5f;
		}

		//Standard spells
		else if( (sp->spell_coef_flags & SPELL_FLAG_IS_DD_OR_DH_SPELL) && !(sp->spell_coef_flags & SPELL_FLAG_IS_DOT_OR_HOT_SPELL) )
		{
			sp->fixed_dddhcoef = sp->casttime_coef;
			if( sp->spell_coef_flags & SPELL_FLAG_ADITIONAL_EFFECT )
				sp->fixed_dddhcoef *= 0.95f;
			if( sp->spell_coef_flags & SPELL_FLAG_AOE_SPELL )
				sp->fixed_dddhcoef *= 0.5f;
		}

		//Over-time spells
		else if( !(sp->spell_coef_flags & SPELL_FLAG_IS_DD_OR_DH_SPELL) && (sp->spell_coef_flags & SPELL_FLAG_IS_DOT_OR_HOT_SPELL) )
		{
			float Duration = float( GetDuration( dbcSpellDuration.LookupEntry( sp->DurationIndex ) ));
			sp->fixed_hotdotcoef = (Duration / 15000.0f);

			if( sp->spell_coef_flags & SPELL_FLAG_ADITIONAL_EFFECT )
				sp->fixed_hotdotcoef *= 0.95f;
			if( sp->spell_coef_flags & SPELL_FLAG_AOE_SPELL )
				sp->fixed_hotdotcoef *= 0.5f;

		}

		//Combined standard and over-time spells
		else if( sp->spell_coef_flags & SPELL_FLAG_IS_DD_DH_DOT_SPELL )
		{
			float Duration = float( GetDuration( dbcSpellDuration.LookupEntry( sp->DurationIndex ) ));
			float Portion_to_Over_Time = (Duration / 15000.0f) / ((Duration / 15000.0f) + sp->casttime_coef );
			float Portion_to_Standard = 1.0f - Portion_to_Over_Time;

			sp->fixed_dddhcoef = sp->casttime_coef * Portion_to_Standard;
			sp->fixed_hotdotcoef = (Duration / 15000.0f) * Portion_to_Over_Time;

			if( sp->spell_coef_flags & SPELL_FLAG_ADITIONAL_EFFECT )
			{
				sp->fixed_dddhcoef *= 0.95f;
				sp->fixed_hotdotcoef *= 0.95f;
			}
			if( sp->spell_coef_flags & SPELL_FLAG_AOE_SPELL )
			{
				sp->fixed_dddhcoef *= 0.5f;
				sp->fixed_hotdotcoef *= 0.5f;
			}		
		}

		///	SPELLS CAN CRIT ///
		sp->spell_can_crit = true; // - except in special cases noted in this section


		//////////////////////////////////////////////////////
		// CLASS-SPECIFIC SPELL FIXES						//
		//////////////////////////////////////////////////////

		/* Note: when applying spell hackfixes, please follow a template */

		//////////////////////////////////////////
		// WARRIOR								//
		//////////////////////////////////////////

		// Insert warrior spell fixes here
		//Warrior - Overpower Rank 1
		sp = dbcSpell.LookupEntryForced(7384);
		if(sp != NULL)
			sp->Attributes |= ATTRIBUTES_CANT_BE_DPB;
		//Warrior - Overpower Rank 2
		sp = dbcSpell.LookupEntryForced(7887);
		if(sp != NULL)
			sp->Attributes |= ATTRIBUTES_CANT_BE_DPB;
		//Warrior - Overpower Rank 3
		sp = dbcSpell.LookupEntryForced(11584);
		if(sp != NULL)
			sp->Attributes |= ATTRIBUTES_CANT_BE_DPB;
		//Warrior - Overpower Rank 4
		sp = dbcSpell.LookupEntryForced(11585);
		if(sp != NULL)
			sp->Attributes |= ATTRIBUTES_CANT_BE_DPB;
		//Warrior - Tactical Mastery Rank 1
		sp = dbcSpell.LookupEntry(0x00003007);
		if(sp != NULL)
			sp->RequiredShapeShift = 0x00070000;
		//Warrior - Tactical Mastery Rank 2
		sp = dbcSpell.LookupEntry(0x00003184);
		if(sp != NULL)
			sp->RequiredShapeShift = 0x00070000;
		//Warrior - Tactical Mastery Rank 3
		sp = dbcSpell.LookupEntry(0x00003185);
		if(sp != NULL)
			sp->RequiredShapeShift = 0x00070000;

		//////////////////////////////////////////
		// PALADIN								//
		//////////////////////////////////////////

		// Insert paladin spell fixes here
			
			// Seal of Righteousness - cannot crit
			if( sp->NameHash == SPELL_HASH_SEAL_OF_RIGHTEOUSNESS )
				sp->spell_can_crit = false;

		/**********************************************************
		* Holy Shield
		**********************************************************/
		sp = dbcSpell.LookupEntryForced( 20925 ); // -- rank 1
		if( sp != NULL )
			sp->procFlags = PROC_ON_BLOCK_VICTIM;
		sp = dbcSpell.LookupEntryForced( 20927 ); // -- rank 2
		if( sp != NULL )
			sp->procFlags = PROC_ON_BLOCK_VICTIM;
		sp = dbcSpell.LookupEntryForced( 20928 ); // -- rank 3
		if( sp != NULL )
			sp->procFlags = PROC_ON_BLOCK_VICTIM;
		sp = dbcSpell.LookupEntryForced( 27179 ); // -- rank 4
		if( sp != NULL )
			sp->procFlags = PROC_ON_BLOCK_VICTIM;

		//////////////////////////////////////////
		// HUNTER								//
		//////////////////////////////////////////

		// Insert hunter spell fixes here
		
			// THESE FIXES ARE GROUPED FOR CODE CLEANLINESS.
			/*
			// Concussive Shot, Distracting Shot, Silencing Shot - ranged spells
			if( sp->NameHash == SPELL_HASH_CONCUSSIVE_SHOT || sp->NameHash == SPELL_HASH_DISTRACTING_SHOT || sp->NameHash == SPELL_HASH_SILENCING_SHOT || sp->NameHash == SPELL_HASH_SCATTER_SHOT || sp->NameHash == SPELL_HASH_TRANQUILIZING_SHOT )
				sp->is_ranged_spell = true;

			// All stings - ranged spells
			if( sp->NameHash == SPELL_HASH_SERPENT_STING || sp->NameHash == SPELL_HASH_SCORPID_STING || sp->NameHash == SPELL_HASH_VIPER_STING || sp->NameHash == SPELL_HASH_WYVERN STING )
				sp->is_ranged_spell = true;
			*/
			// come to think of it... anything *castable* requiring a ranged weapon is a ranged spell -.-
			// Note that talents etc also come under this, however it does not matter
			// if they get flagged as ranged spells because is_ranged_spell is only used for
			// differentiating between resistable and physically avoidable spells.
			if( sp->EquippedItemClass == 2 && sp->EquippedItemSubClass & 262156 ) // 4 + 8 + 262144 ( becomes item classes 2, 3 and 18 which correspond to bow, gun and crossbow respectively)
			{
				sp->is_ranged_spell = true;
			}

		//////////////////////////////////////////
		// ROGUE								//
		//////////////////////////////////////////

		// Insert rogue spell fixes here

		//////////////////////////////////////////
		// PRIEST								//
		//////////////////////////////////////////

		// Insert priest spell fixes here

		//////////////////////////////////////////
		// SHAMAN								//
		//////////////////////////////////////////

		// Insert shaman spell fixes here

			// Lightning Shield - cannot crit
			if( sp->NameHash == SPELL_HASH_LIGHTNING_SHIELD ) // not a mistake, the correct proc spell for lightning shield is also called lightning shield
				sp->spell_can_crit = false;

		//////////////////////////////////////////
		// MAGE								//
		//////////////////////////////////////////

		// Insert mage spell fixes here
		// Invisibility: changed to "Dummy" effect for invisibility to trigger.
		if( sp->NameHash == SPELL_HASH_INVISIBILITY )
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
		}

		sp = dbcSpell.LookupEntryForced( 66 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[2] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
			sp->Effect[2] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectAmplitude[2] = 6000;
			sp->EffectImplicitTargetA[2] = EFF_TARGET_SELF;
			sp->EffectBaseDice[2] = 1;
			sp->EffectDieSides[2] = 1;
			sp->EffectTriggerSpell[2] = 32612;
			sp->EffectBasePoints[2] = -1;
		}
			// Hypothermia: undispellable
			if( sp->NameHash == SPELL_HASH_HYPOTHERMIA )
				sp->c_is_flags |= SPELL_FLAG_IS_FORCEDDEBUFF;

		//////////////////////////////////////////
		// WARLOCK								//
		//////////////////////////////////////////

		// Insert warlock spell fixes here

		//////////////////////////////////////////
		// DRUID								//
		//////////////////////////////////////////

		// Insert druid spell fixes here


	}

	//Settings for special cases
	QueryResult * resultx = WorldDatabase.Query("SELECT * FROM spell_coef_override");
	if( resultx != NULL )
	{
		do 
		{
			Field * f;
			f = resultx->Fetch();
			SpellEntry * sp = dbcSpell.LookupEntryForced( f[0].GetUInt32() );
			if( sp != NULL )
			{
				sp->Dspell_coef_override = f[2].GetFloat();
				sp->OTspell_coef_override = f[3].GetFloat();
			}
			else
				Log.Warning("SpellCoefOverride", "Has nonexistant spell %u.", f[0].GetUInt32());
		} while( resultx->NextRow() );
		delete resultx;
	}

	//Fully loaded coefficients, we must share channeled coefficient to its triggered spells
	for(uint32 x=0; x < cnt; x++)
	{
		// SpellID
		uint32 spellid = dbc.getRecord(x).getUInt(0);
		// get spellentry
		SpellEntry * sp = dbcSpell.LookupEntry(spellid);
		SpellEntry * spz;

		//Case SPELL_AURA_PERIODIC_TRIGGER_SPELL
		for( uint8 i = 0 ; i < 3 ; i++ )
		{
			if ( sp->EffectApplyAuraName[i] == SPELL_AURA_PERIODIC_TRIGGER_SPELL )
			{
				spz = dbcSpell.LookupEntry( sp->EffectTriggerSpell[i] );
				if( spz != NULL ) 
				{
					if( sp->Dspell_coef_override >= 0 )
						spz->Dspell_coef_override = sp->Dspell_coef_override;
					else
					{
						//we must set bonus per tick on triggered spells now (i.e. Arcane Missiles)
						if( sp->ChannelInterruptFlags != 0 )
						{
							float Duration = float( GetDuration( dbcSpellDuration.LookupEntry( sp->DurationIndex ) ));
							float amp = float(sp->EffectAmplitude[i]);
							sp->fixed_dddhcoef = sp->fixed_hotdotcoef * amp / Duration;
						}			
						spz->fixed_dddhcoef = sp->fixed_dddhcoef;
					}

					if( sp->OTspell_coef_override >= 0 )
						spz->OTspell_coef_override = sp->OTspell_coef_override;
					else
					{
						//we must set bonus per tick on triggered spells now (i.e. Arcane Missiles)
						if( sp->ChannelInterruptFlags != 0 )
						{
							float Duration = float( GetDuration( dbcSpellDuration.LookupEntry( sp->DurationIndex ) ));
							float amp = float(sp->EffectAmplitude[i]);
							sp->fixed_hotdotcoef *= amp / Duration;
						}
						spz->fixed_hotdotcoef = sp->fixed_hotdotcoef;
					}
					break;
				}
			}
		}
	}	

	/////////////////////////////////////////////////////////////////
	//SPELL COEFFICIENT SETTINGS END
	/////////////////////////////////////////////////////////////////
	SpellEntry* sp;

	/********************************************************
	 * Windfury Enchantment
	 ********************************************************/
	EnchantEntry* Enchantment;
		
	Enchantment = dbcEnchant.LookupEntryForced( 283 );
	if( Enchantment != NULL )
	{
		Enchantment->spell[0] = 33757; //this is actually good
		sp = dbcSpell.LookupEntryForced( 33757 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = PROC_ON_MELEE_ATTACK; //we do not need proc on spell ;)
			sp->EffectTriggerSpell[0] = 8232; //for the logs and rest
			sp->procChance = 20;
			sp->proc_interval = 3000;//http://www.wowwiki.com/Windfury_Weapon
			sp->maxstack = 1;
		}
	}

	Enchantment = dbcEnchant.LookupEntryForced( 284 );
	if( Enchantment != NULL )
	{
		Enchantment->spell[0] = 33756; 
		sp = dbcSpell.LookupEntryForced( 33756 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = PROC_ON_MELEE_ATTACK; //we do not need proc on spell ;)
			sp->EffectTriggerSpell[0] = 8235; //for the logs and rest
			sp->procChance = 20;
			sp->proc_interval = 3000; //http://www.wowwiki.com/Windfury_Weapon
			sp->maxstack = 1;
		}
	}

	Enchantment = dbcEnchant.LookupEntryForced( 525 );
	if( Enchantment != NULL )
	{
		Enchantment->spell[0] = 33755; 
		sp = dbcSpell.LookupEntryForced( 33755 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = PROC_ON_MELEE_ATTACK; //we do not need proc on spell ;)
			sp->EffectTriggerSpell[0] = 10486; //for the logs and rest
			sp->procChance = 20;
			sp->proc_interval = 3000;//http://www.wowwiki.com/Windfury_Weapon
			sp->maxstack = 1;
		}
	}

	Enchantment = dbcEnchant.LookupEntryForced( 1669 );
	if( Enchantment != NULL )
	{
		Enchantment->spell[0] = 33754; 
		sp = dbcSpell.LookupEntryForced( 33754 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = PROC_ON_MELEE_ATTACK; //we do not need proc on spell ;)
			sp->EffectTriggerSpell[0] = 16362; //for the logs and rest
			sp->procChance = 20;
			sp->proc_interval = 3000;//http://www.wowwiki.com/Windfury_Weapon
			sp->maxstack = 1;
		}
	}

	Enchantment = dbcEnchant.LookupEntryForced( 2636 );
	if( Enchantment != NULL )
	{
		Enchantment->spell[0] = 33727; 
		sp = dbcSpell.LookupEntryForced( 33727 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = PROC_ON_MELEE_ATTACK; //we do not need proc on spell ;)
			sp->EffectTriggerSpell[0] = 25505; //for the logs and rest
			sp->procChance = 20;
			sp->proc_interval = 3000;//http://www.wowwiki.com/Windfury_Weapon
			sp->maxstack = 1;
		}
	}

	/**********************************************************
	 *	PROFFESION - Enchant Cloak - Major Resistance
	 **********************************************************/
	sp = dbcSpell.LookupEntryForced( 27962 );
	if( sp != NULL )
		sp->EffectMiscValue[0] = 2998;
	sp = dbcSpell.LookupEntryForced( 36285 );
	if( sp != NULL )
		sp->EffectMiscValue[0] = 2998;

	/**********************************************************
	 *	Wand Shoot
	 **********************************************************/
	sp = dbcSpell.LookupEntryForced( 5019 );
	if( sp != NULL )
		sp->SpellGroupType = 134217728;

	/**********************************************************
	 * Clearcasting
	 **********************************************************/
	sp = dbcSpell.LookupEntryForced( 12536 );
	if( sp != NULL )
	{
		sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
		sp->EffectMiscValue[0] = SMT_COST;
		sp->EffectSpellGroupRelation[0] = 0xFFFFFFFF; //all possible spells we can affect
	}
	sp = dbcSpell.LookupEntryForced( 16246 );
	if( sp != NULL )
	{
		sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
		sp->EffectMiscValue[0] = SMT_COST;
		sp->EffectSpellGroupRelation[0] = 0xFFFFFFFF; //all possible spells we can affect
	}
	sp = dbcSpell.LookupEntryForced( 16870 );
	if( sp != NULL )
	{
		sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
		sp->EffectMiscValue[0] = SMT_COST;
		sp->EffectSpellGroupRelation[0] = 0xFFFFFFFF; //all possible spells we can affect
	}
	sp = dbcSpell.LookupEntryForced( 34754 );
	if( sp != NULL )
	{
		sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
		sp->EffectMiscValue[0] = SMT_COST;
		sp->EffectSpellGroupRelation[0] = 0xFFFFFFFF; //all possible spells we can affect
	}

	/**********************************************************
	 * Berserking - TROLL'S RACIAL SPELL
	 **********************************************************/
	sp = dbcSpell.LookupEntryForced( 20554 );
	if( sp != NULL )
	{
		sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
		sp->EffectTriggerSpell[0] = 26635;
	}
	sp = dbcSpell.LookupEntryForced( 26296 );
	if( sp != NULL )
	{
		sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
		sp->EffectTriggerSpell[0] = 26635;
	}
	sp = dbcSpell.LookupEntryForced( 26297 );
	if( sp != NULL )
	{
		sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
		sp->EffectTriggerSpell[0] = 26635;
	}

	sp = dbcSpell.LookupEntryForced( 20134 ); // << --- WTF?
	if( sp != NULL )
		sp->procChance = 50;

	/**********************************************************
	* Mana Tap - BLOOD ELF RACIAL
	**********************************************************/
	sp = dbcSpell.LookupEntryForced( 28734 );
	if( sp != NULL )
		sp->Effect[0] = SPELL_EFFECT_POWER_BURN; // should be Power Burn, not Power Drain. Power Drain leeches mana which is incorrect.

	/**********************************************************
	 * thrown - add a 1.6 second cooldown
	 **********************************************************/
	const static uint32 thrown_spells[] = {SPELL_RANGED_GENERAL,SPELL_RANGED_THROW,SPELL_RANGED_WAND, 26679, 27084, 29436, 37074, 41182, 41346, 0};
	for(i = 0; thrown_spells[i] != 0; ++i)
	{
		sp = dbcSpell.LookupEntryForced( thrown_spells[i] );
		if( sp != NULL && sp->RecoveryTime==0 && sp->StartRecoveryTime == 0 )
			sp->RecoveryTime = 1600;
	}

	/**********************************************************
	 * Wands
	 **********************************************************/
	sp = dbcSpell.LookupEntryForced( SPELL_RANGED_WAND );
	if( sp != NULL )
		sp->Spell_Dmg_Type = SPELL_DMG_TYPE_RANGED;

	/**********************************************************
	 * Misc stuff (questfixes etc)
	 **********************************************************/

	sp = dbcSpell.LookupEntryForced( 30877 );
	if( sp != NULL )
	{
		sp->EffectImplicitTargetB[0]=0;
	}

	sp = dbcSpell.LookupEntryForced(23179);
	if( sp != NULL )
		sp->EffectMiscValue[0] = 1434;

	// list of guardians that should inherit casters level
	//fire elemental
	sp = dbcSpell.LookupEntryForced(32982); 
	if( sp != NULL )
		sp->c_is_flags |= SPELL_FLAG_IS_INHERITING_LEVEL;
	//Earth elemental
	sp = dbcSpell.LookupEntryForced(33663); 
	if( sp != NULL )
		sp->c_is_flags |= SPELL_FLAG_IS_INHERITING_LEVEL;
	//Force of Nature
	sp = dbcSpell.LookupEntryForced(33831); 
	if( sp != NULL )
		sp->c_is_flags |= SPELL_FLAG_IS_INHERITING_LEVEL;

	//////////////////////////////////////////////////////
	// CLASS-SPECIFIC SPELL FIXES						//
	//////////////////////////////////////////////////////

	/* Note: when applying spell hackfixes, please follow a template */

	//////////////////////////////////////////
	// WARRIOR								//
	//////////////////////////////////////////

	// Insert warrior spell fixes here
		//Warrior - Charge Rank 1
		sp = dbcSpell.LookupEntryForced(100);
		if(sp != NULL)
		{
			sp->Effect[1] = SPELL_EFFECT_DUMMY;
			sp->EffectMiscValue[1] = 90;
		}
		//Warrior - Charge Rank 2
		sp = dbcSpell.LookupEntryForced(6178);
		if(sp != NULL)
		{
			sp->Effect[1] = SPELL_EFFECT_DUMMY;
			sp->EffectMiscValue[1] = 120;
		}
		//Warrior - Charge Rank 3
		sp = dbcSpell.LookupEntryForced(11578);
		if(sp != NULL)
		{
			sp->Effect[1] = SPELL_EFFECT_DUMMY;		
			sp->EffectMiscValue[1] = 150;
		}
		//Warrior - Improved Hamstring Rank 1
		sp = dbcSpell.LookupEntryForced(12289);
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[1]=23694;// Improved Hamstring : Immobilized. 5 seconds remaining.
			sp->procFlags = PROC_ON_CAST_SPECIFIC_SPELL;
			sp->procChance = 5;
		}
		//Warrior - Improved Hamstring Rank 2
		sp = dbcSpell.LookupEntryForced(12668);
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[1]=23694;// Improved Hamstring : Immobilized. 5 seconds remaining.
			sp->procFlags = PROC_ON_CAST_SPECIFIC_SPELL;
			sp->procChance = 10;
		}
		//Warrior - Improved Hamstring Rank 3
		sp = dbcSpell.LookupEntryForced(23695);
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[1]=23694;// Improved Hamstring : Immobilized. 5 seconds remaining.
			sp->procFlags = PROC_ON_CAST_SPECIFIC_SPELL;
			sp->procChance = 15;
		}
		//Warrior - Retaliation
		sp = dbcSpell.LookupEntryForced( 20230 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6; //aura
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 22858; //evil , but this is good for us :D
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM; //add procflag here since this was not processed with the others !
		}

		//"bloodthirst" new version is ok but old version is wrong from now on :(
		sp = dbcSpell.LookupEntryForced( 23881 );
		if( sp != NULL )
		{
			sp->Effect[1] = 64; //cast on us, it is good
			sp->EffectTriggerSpell[1] = 23885; //evil , but this is good for us :D
		}
		sp = dbcSpell.LookupEntryForced( 23892 );
		if( sp != NULL )
		{
			sp->Effect[1] = 64;
			sp->EffectTriggerSpell[1] = 23886; //evil , but this is good for us :D
		}
		sp = dbcSpell.LookupEntryForced( 23893 );
		if( sp != NULL )
		{
			sp->Effect[1] = 64; //
			sp->EffectTriggerSpell[1] = 23887; //evil , but this is good for us :D
		}
		sp = dbcSpell.LookupEntryForced( 23894 );
		if( sp != NULL )
		{
			sp->Effect[1] = 64; //
			sp->EffectTriggerSpell[1] = 23888; //evil , but this is good for us :D
		}
		sp = dbcSpell.LookupEntryForced( 25251 );
		if( sp != NULL )
		{
			sp->Effect[1] = 64; //aura
			sp->EffectTriggerSpell[1] = 25252; //evil , but this is good for us :D
		}
		sp = dbcSpell.LookupEntryForced( 30335 );
		if( sp != NULL )
		{
			sp->Effect[1] = 64; //aura
			sp->EffectTriggerSpell[1] = 30339; //evil , but this is good for us :D
		}
		//rend
		sp = dbcSpell.LookupEntryForced( 772 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 6546 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 6547 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 6548 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 11572 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 11573 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 11574 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 25208 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;

		//warrior - Rampage
		sp = dbcSpell.LookupEntryForced( 29801 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_TARGET_SELF;
			sp->EffectTriggerSpell[0] = sp->EffectTriggerSpell[1];
		}
		sp = dbcSpell.LookupEntryForced( 30030 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_TARGET_SELF;
			sp->EffectTriggerSpell[0] = sp->EffectTriggerSpell[1];
		}
		sp = dbcSpell.LookupEntryForced( 30033 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_TARGET_SELF;
			sp->EffectTriggerSpell[0] = sp->EffectTriggerSpell[1];
		}
	#ifdef NEW_PROCFLAGS
		//warrior - deep wounds
		sp = dbcSpell.LookupEntry( 12162);
		if ( sp!=NULL )
			sp->SpellGroupType = 32;
		sp = dbcSpell.LookupEntry( 12850);
		if ( sp!=NULL )
			sp->SpellGroupType = 32;
		sp = dbcSpell.LookupEntry( 12868);
		if ( sp!=NULL )
			sp->SpellGroupType = 32;
	#endif

		//warrior - second wind should trigger on self
		sp = dbcSpell.LookupEntryForced( 29841 );
		if( sp != NULL )
			sp->procFlags |= PROC_TARGET_SELF;
		sp = dbcSpell.LookupEntryForced( 29842 );
		if( sp != NULL )
			sp->procFlags |= PROC_TARGET_SELF;

		//warrior - Improved Disciplines
		sp = dbcSpell.LookupEntryForced( 29725 );
		if (sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 16 | 8192;
			sp->EffectSpellGroupRelation_high[0] = 8;
			sp->EffectSpellGroupRelation[1] = 16 | 8192;
			sp->EffectSpellGroupRelation_high[1] = 8;
		}
		sp = dbcSpell.LookupEntryForced( 29724 );
		if (sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 16 | 8192;
			sp->EffectSpellGroupRelation_high[0] = 8;
			sp->EffectSpellGroupRelation[1] = 16 | 8192;
			sp->EffectSpellGroupRelation_high[1] = 8;
		}
		sp = dbcSpell.LookupEntryForced( 29723 );
		if (sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 16 | 8192;
			sp->EffectSpellGroupRelation_high[0] = 8;
			sp->EffectSpellGroupRelation[1] = 16 | 8192;
			sp->EffectSpellGroupRelation_high[1] = 8;
		}

		//warrior - improved berserker rage
		sp = dbcSpell.LookupEntryForced( 20500 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL | PROC_TARGET_SELF;
		sp = dbcSpell.LookupEntryForced( 20501 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL | PROC_TARGET_SELF;

		//warrior - Blood Frenzy
		sp = dbcSpell.LookupEntryForced( 29836 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 29859 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
		}

		//warrior - Unbridled Wrath
		sp = dbcSpell.LookupEntryForced( 12322 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_TARGET_SELF;
		sp = dbcSpell.LookupEntryForced( 12999 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_TARGET_SELF;
		sp = dbcSpell.LookupEntryForced( 13000 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_TARGET_SELF;
		sp = dbcSpell.LookupEntryForced( 13001 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_TARGET_SELF;
		sp = dbcSpell.LookupEntryForced( 13002 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_TARGET_SELF;

		//warrior - Commanding Presence
		sp = dbcSpell.LookupEntryForced( 12318 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 65536 ;
			sp->EffectSpellGroupRelation_high[0] = 128;
		}
		sp = dbcSpell.LookupEntryForced( 12857 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 65536 ;
			sp->EffectSpellGroupRelation_high[0] = 128;
		}
		sp = dbcSpell.LookupEntryForced( 12858 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 65536 ;
			sp->EffectSpellGroupRelation_high[0] = 128;
		}
		sp = dbcSpell.LookupEntryForced( 12860 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 65536 ;
			sp->EffectSpellGroupRelation_high[0] = 128;
		}
		sp = dbcSpell.LookupEntryForced( 12861 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 65536 ;
			sp->EffectSpellGroupRelation_high[0] = 128;
		}

		//warrior - Booming Voice
		sp = dbcSpell.LookupEntryForced( 12321 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 65536 | 131072;
			sp->EffectSpellGroupRelation_high[0] = 128;
			sp->EffectSpellGroupRelation[1] = 65536 | 131072;
			sp->EffectSpellGroupRelation_high[1] = 128;
		}
		sp = dbcSpell.LookupEntryForced( 12835 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 65536 | 131072;
			sp->EffectSpellGroupRelation_high[0] = 128;
			sp->EffectSpellGroupRelation[1] = 65536 | 131072;
			sp->EffectSpellGroupRelation_high[1] = 128;
		}
		sp = dbcSpell.LookupEntryForced( 12836 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 65536 | 131072;
			sp->EffectSpellGroupRelation_high[0] = 128;
			sp->EffectSpellGroupRelation[1] = 65536 | 131072;
			sp->EffectSpellGroupRelation_high[1] = 128;
		}
		sp = dbcSpell.LookupEntryForced( 12837 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 65536 | 131072;
			sp->EffectSpellGroupRelation_high[0] = 128;
			sp->EffectSpellGroupRelation[1] = 65536 | 131072;
			sp->EffectSpellGroupRelation_high[1] = 128;
		}
		sp = dbcSpell.LookupEntryForced( 12838 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 65536 | 131072;
			sp->EffectSpellGroupRelation_high[0] = 128;
			sp->EffectSpellGroupRelation[1] = 65536 | 131072;
			sp->EffectSpellGroupRelation_high[1] = 128;
		}

		//warrior - Improved Intercept
		sp = dbcSpell.LookupEntryForced( 29888 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1073741824;
		sp = dbcSpell.LookupEntryForced( 29889 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1073741824;

		//warrior - Improved Mortal Strike

		sp = dbcSpell.LookupEntryForced( 35446 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[1] = 33554432;
		sp->EffectSpellGroupRelation[0] = 33554432;
		}
		sp = dbcSpell.LookupEntryForced( 35448 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[1] = 33554432;
		sp->EffectSpellGroupRelation[0] = 33554432;
		}
		sp = dbcSpell.LookupEntryForced( 35449 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[1] = 33554432;
		sp->EffectSpellGroupRelation[0] = 33554432;
		}
		sp = dbcSpell.LookupEntryForced( 35450 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[1] = 33554432;
		sp->EffectSpellGroupRelation[0] = 33554432;
		}
		sp = dbcSpell.LookupEntryForced( 35451 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[1] = 33554432;
		sp->EffectSpellGroupRelation[0] = 33554432;
		}
		//watrior - Improved Thudner Clap
		sp = dbcSpell.LookupEntryForced( 12287 );
		if ( sp != NULL )
			sp->EffectSpellGroupRelation[1] = 128;
		sp = dbcSpell.LookupEntryForced( 12665 );
		if ( sp != NULL )
			sp->EffectSpellGroupRelation[1] = 128;	
		sp = dbcSpell.LookupEntryForced( 12666 );
		if ( sp != NULL )
			sp->EffectSpellGroupRelation[1] = 128;
		//warrior - Tactical Mastery
		sp = dbcSpell.LookupEntryForced( 12677 );
		if ( sp != NULL )
		{
			sp->EffectSpellGroupRelation_high[1] = 1024;
			sp->EffectSpellGroupRelation[2] =33554432;
		}
		sp = dbcSpell.LookupEntryForced( 12676 );
		if ( sp != NULL )
		{
			sp->EffectSpellGroupRelation_high[1] = 1024;
			sp->EffectSpellGroupRelation[2] =33554432;
		}
		sp = dbcSpell.LookupEntryForced( 12295 );
		if ( sp != NULL )
		{
			sp->EffectSpellGroupRelation_high[1] = 1024;
			sp->EffectSpellGroupRelation[2] =33554432;
		}

		//warrior - Focused Rage
		sp = dbcSpell.LookupEntryForced( 29787 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 8 | 1 | 4 | 32 | 2 | 4194304 | 536870912 | 2097152 | 128 | 134217728 | 1073741824UL | 2048 | 64 | 1024 | 33554432;
			sp->EffectSpellGroupRelation_high[0] = 8 | 256 | 64;
		}
		sp = dbcSpell.LookupEntryForced( 29790 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 8 | 1 | 4 | 32 | 2 | 4194304 | 536870912 | 2097152 | 128 | 134217728 | 1073741824UL | 2048 | 64 | 1024 | 33554432;
			sp->EffectSpellGroupRelation_high[0] = 8 | 256 | 64;
		}
		sp = dbcSpell.LookupEntryForced( 29792 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 8 | 1 | 4 | 32 | 2 | 4194304 | 536870912 | 2097152 | 128 | 134217728 | 1073741824UL | 2048 | 64 | 1024 | 33554432;
			sp->EffectSpellGroupRelation_high[0] = 8 | 256 | 64;
		}

		//warrior - impale
		sp = dbcSpell.LookupEntryForced( 16493 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 8 | 1 | 4 | 32 | 2 | 4194304 | 536870912 | 2097152 | 128 | 134217728 | 1073741824 | 2048 | 64 | 1024;
		sp = dbcSpell.LookupEntryForced( 16494 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 8 | 1 | 4 | 32 | 2 | 4194304 | 536870912 | 2097152 | 128 | 134217728 | 1073741824 | 2048 | 64 | 1024;

		//warrior - Improved Whirlwind
		sp = dbcSpell.LookupEntryForced( 29721 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation_high[0] = 4;
		sp = dbcSpell.LookupEntryForced( 29776 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation_high[0] = 4;

		//warrior - Improved Commanding Shout
		sp = dbcSpell.LookupEntryForced( 38408 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation_high[0] = 128;

		//////////////////////////////////////////
		// DRUID								//
		//////////////////////////////////////////

		// Spell 22570 (Maim Rank 1)
		sp = dbcSpell.LookupEntryForced(22570);
		if( sp != NULL )
			sp->AuraInterruptFlags |= AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN;

		//////////////////////////////////////////
		// PALADIN								//
		//////////////////////////////////////////

		// Insert paladin spell fixes here

		// Seal of Command - Holy damage, but melee mechanics (crit damage, chance, etc)
		sp = dbcSpell.LookupEntryForced( 20424 );
		if( sp != NULL )
			sp->is_melee_spell = true;

		/**********************************************************
		 *	Blessing of Light
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 19977 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
		}
		sp = dbcSpell.LookupEntryForced( 19978 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
		}
		sp = dbcSpell.LookupEntryForced( 19979 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
		}
		sp = dbcSpell.LookupEntryForced( 27144 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
		}
		sp = dbcSpell.LookupEntryForced( 32770 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
		}
		/**********************************************************
		 * Improved Righteous Fury
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 20468 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1;
			sp->EffectSpellGroupRelation[1] = 1;
		}
		sp = dbcSpell.LookupEntryForced( 20469 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1;
			sp->EffectSpellGroupRelation[1] = 1;
		}
		sp = dbcSpell.LookupEntryForced( 20470 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1;
			sp->EffectSpellGroupRelation[1] = 1;
		}

		/**********************************************************
		 * Healing Light
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 20237 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 0x40000000 | 0x80000000;
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 0x40000000 | 0x80000000;
		sp = dbcSpell.LookupEntryForced( 20239 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 0x40000000 | 0x80000000;

		/**********************************************************
		 * Improved Devotion Aura
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 20142 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 64;
		sp = dbcSpell.LookupEntryForced( 20141 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 64;
		sp = dbcSpell.LookupEntryForced( 20140 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 64;
		sp = dbcSpell.LookupEntryForced( 20139 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 64;
		sp = dbcSpell.LookupEntryForced( 20138 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 64;

		/**********************************************************
		 * Guardian's Favor
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 20175 );
		if (sp != NULL)
		{
			sp->EffectSpellGroupRelation[0]=128;
			sp->EffectSpellGroupRelation[1]=268435472;
		}
		sp = dbcSpell.LookupEntryForced( 20174 );
		if (sp != NULL)
		{
			sp->EffectSpellGroupRelation[0]=128;
			sp->EffectSpellGroupRelation[1]=268435472;
		}

		/**********************************************************
		 * Improved Concentration Aura
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 20256 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]=131072;
		sp = dbcSpell.LookupEntryForced( 20255 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]=131072;
		sp = dbcSpell.LookupEntryForced( 20254 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]=131072;

		/**********************************************************
		 * Sacred Duty
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 31849 );
		if (sp != NULL)
		{
			sp->EffectSpellGroupRelation[0]=4194304;
			sp->EffectSpellGroupRelation[1]=4194304;
		}
		sp = dbcSpell.LookupEntryForced( 31848 );
		if (sp != NULL)
		{
			sp->EffectSpellGroupRelation[0]=4194304;
			sp->EffectSpellGroupRelation[1]=4194304;
		}

		/**********************************************************
		 * Aura Mastery
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 31821 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 131072 | 67108864 | 8 | 64;
		
		/**********************************************************
		 * Sanctified Light
		 **********************************************************/
		group_relation_paladin_sanctified_light = 0x80000000; // <-- Grouping
		sp = dbcSpell.LookupEntryForced( 20359 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_paladin_sanctified_light;
		sp = dbcSpell.LookupEntryForced( 20360 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_paladin_sanctified_light;
		sp = dbcSpell.LookupEntryForced( 20361 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_paladin_sanctified_light;

		/**********************************************************
		 * Improved Seal of the Crusader
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 20335 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 536870912;
		sp = dbcSpell.LookupEntryForced( 20336 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 536870912;
		sp = dbcSpell.LookupEntryForced( 20337 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 536870912;
		sp = dbcSpell.LookupEntryForced( 28852 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 536870912;
		sp = dbcSpell.LookupEntryForced( 33557 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 536870912;
		
		/**********************************************************
		 * Light's Grace
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 31834 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 0x80000000;

		/**********************************************************
		 * Stoicism
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 31844 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[1] = 0xFFFFFFFF;
		sp = dbcSpell.LookupEntryForced( 31845 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[1] = 0xFFFFFFFF;

		/**********************************************************
		 * Fanaticism
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 31879 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1048576 | 524288 | 1024 | 536870912 | 524288;
		sp = dbcSpell.LookupEntryForced( 31880 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1048576 | 524288 | 1024 | 536870912 | 524288;
		sp = dbcSpell.LookupEntryForced( 31881 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1048576 | 524288 | 1024 | 536870912 | 524288;
		sp = dbcSpell.LookupEntryForced( 31882 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1048576 | 524288 | 1024 | 536870912 | 524288;
		sp = dbcSpell.LookupEntryForced( 31883 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1048576 | 524288 | 1024 | 536870912 | 524288;

		/**********************************************************
		 * Seal of Vengeance
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 31801 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->EffectTriggerSpell[0] = 31803;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
		}

		/**********************************************************
		 * Reckoning
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 20177 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM | PROC_TARGET_SELF;
		sp = dbcSpell.LookupEntryForced( 20179 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM | PROC_TARGET_SELF;
		sp = dbcSpell.LookupEntryForced( 20180 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM | PROC_TARGET_SELF;
		sp = dbcSpell.LookupEntryForced( 20181 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM | PROC_TARGET_SELF;
		sp = dbcSpell.LookupEntryForced( 20182 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM | PROC_TARGET_SELF;

		/**********************************************************
		 * Reckoning Effect
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 20178 );
		if( sp != NULL )
		{
			sp->procChance = 100;
			sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_TARGET_SELF;
		}

		/**********************************************************
		 * Judgement of Wisdom
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 20186 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;
			sp->EffectTriggerSpell[0] = 20268;
		}
		sp = dbcSpell.LookupEntryForced( 20354 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;
			sp->EffectTriggerSpell[0] = 20352;
		}
		sp = dbcSpell.LookupEntryForced( 20355 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;
			sp->EffectTriggerSpell[0] = 20353;
		}
		sp = dbcSpell.LookupEntryForced( 27164 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;
			sp->EffectTriggerSpell[0] = 27165;
		}

		sp = dbcSpell.LookupEntryForced( 20268 );
		if( sp != NULL )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_ENEMY;
		sp = dbcSpell.LookupEntryForced( 20352 );
		if( sp != NULL )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_ENEMY;
		sp = dbcSpell.LookupEntryForced( 20353 );
		if( sp != NULL )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_ENEMY;
		sp = dbcSpell.LookupEntryForced( 27165 );
		if( sp != NULL )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_ENEMY;

		/**********************************************************
		 * Judgement of Light
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 20185 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM;
			sp->EffectTriggerSpell[0] = 20267;
		}
		sp = dbcSpell.LookupEntryForced( 20344 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM;
			sp->EffectTriggerSpell[0] = 20341;
		}
		sp = dbcSpell.LookupEntryForced( 20345 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM;
			sp->EffectTriggerSpell[0] = 20342;
		}
		sp = dbcSpell.LookupEntryForced( 20346 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM;
			sp->EffectTriggerSpell[0] = 20343;
		}
		sp = dbcSpell.LookupEntryForced( 27162 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM;
			sp->EffectTriggerSpell[0] = 27163;
		}
		sp = dbcSpell.LookupEntryForced( 20267 );
		if( sp != NULL )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_ENEMY;
		sp = dbcSpell.LookupEntryForced( 20341 );
		if( sp != NULL )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_ENEMY;
		sp = dbcSpell.LookupEntryForced( 20342 );
		if( sp != NULL )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_ENEMY;
		sp = dbcSpell.LookupEntryForced( 20343 );
		if( sp != NULL )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_ENEMY;
		sp = dbcSpell.LookupEntryForced( 27163 );
		if( sp != NULL )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_ENEMY;

		/**********************************************************
		 * Eye for an Eye
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 9799 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT_VICTIM;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 25997;
		}
		sp = dbcSpell.LookupEntryForced( 25988 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT_VICTIM;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 25997;
		}

		/**********************************************************
		 * sanctified judgement
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 31876 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 31930;
		}
		sp = dbcSpell.LookupEntryForced( 31877 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 31930;
		}
		sp = dbcSpell.LookupEntryForced( 31878 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 31930;
			//sp->procChance=100;
		}

		/**********************************************************
		 * Blessed Life
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 31828 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;
			sp->EffectTriggerSpell[0] = 31828;
		}
		sp = dbcSpell.LookupEntryForced( 31829 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;
			sp->EffectTriggerSpell[0] = 31828;
		}
		sp = dbcSpell.LookupEntryForced( 31830 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;
			sp->EffectTriggerSpell[0] = 31828;
		}

		/**********************************************************
		 * Light's Grace
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 31833 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;
		sp = dbcSpell.LookupEntryForced( 31835 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;
		sp = dbcSpell.LookupEntryForced( 31836 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;

		//Paladin: Seal of Wisdom
		uint32 procchance = 0;
		sp = dbcSpell.LookupEntryForced( 27116 );
		if( sp != NULL )
			procchance = sp->procChance;
		sp = dbcSpell.LookupEntryForced( 20166 );
		if( sp != NULL )
			sp->procChance = procchance;
		sp = dbcSpell.LookupEntryForced( 20356 );
		if( sp != NULL )
			sp->procChance = procchance;
		sp = dbcSpell.LookupEntryForced( 20357 );
		if( sp != NULL )
			sp->procChance = procchance;
		sp = dbcSpell.LookupEntryForced( 27166 );
		if( sp != NULL )
			sp->procChance = procchance;
		//paladin - seal of blood
		sp = dbcSpell.LookupEntryForced( 31892 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 31893;
		}
		sp = dbcSpell.LookupEntryForced( 38008 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 31893;
		}

		//paladin - Spiritual Attunement 
		sp = dbcSpell.LookupEntryForced( 31785 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_SPELL_HIT_VICTIM | PROC_TARGET_SELF ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 31786;
		}
		sp = dbcSpell.LookupEntryForced( 33776 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_SPELL_HIT_VICTIM | PROC_TARGET_SELF;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 31786;
		}

		//Seal of Justice -lowered proc chance (experimental values !)
		sp = dbcSpell.LookupEntryForced( 20164 );
		if( sp != NULL )
			sp->procChance = 20;
		sp = dbcSpell.LookupEntryForced( 31895 );
		if( sp != NULL )
			sp->procChance = 20;

		// paladin - Improved Sanctity Aura
		sp = dbcSpell.LookupEntryForced( 31869 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 67108864;
			sp->EffectMiscValue[0] = SMT_SPELL_VALUE;
		}
		sp = dbcSpell.LookupEntryForced( 31870 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 67108864;
			sp->EffectMiscValue[0] = SMT_SPELL_VALUE;
		}
		//Paladin - Improved Lay on Hands
		sp = dbcSpell.LookupEntryForced( 20234 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;
		sp = dbcSpell.LookupEntryForced( 20235 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;

	//////////////////////////////////////////
	// HUNTER								//
	//////////////////////////////////////////

	// Insert hunter spell fixes here

		//Hunter - Go for the Throat
		sp = dbcSpell.LookupEntryForced( 34950 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_RANGED_CRIT_ATTACK;
		sp = dbcSpell.LookupEntryForced( 34954 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_RANGED_CRIT_ATTACK;
		sp = dbcSpell.LookupEntryForced( 34952 );
		if( sp != NULL )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		sp = dbcSpell.LookupEntryForced( 34953 );
		if( sp != NULL )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;

		// Hunter - Improved Wing Clip
		sp = dbcSpell.LookupEntryForced( 19228 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPECIFIC_SPELL;
			sp->ProcOnNameHash[0] = SPELL_HASH_WING_CLIP;
		}
		sp = dbcSpell.LookupEntryForced( 19232 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPECIFIC_SPELL;
			sp->ProcOnNameHash[0] = SPELL_HASH_WING_CLIP;
		}
		sp = dbcSpell.LookupEntryForced( 19233 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPECIFIC_SPELL;
			sp->ProcOnNameHash[0] = SPELL_HASH_WING_CLIP;
		}

		// Hunter - Master Tactician
		sp = dbcSpell.LookupEntryForced( 34506 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_RANGED_ATTACK | PROC_TARGET_SELF;
		sp = dbcSpell.LookupEntryForced( 34507 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_RANGED_ATTACK | PROC_TARGET_SELF;
		sp = dbcSpell.LookupEntryForced( 34508 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_RANGED_ATTACK | PROC_TARGET_SELF;
		sp = dbcSpell.LookupEntryForced( 34838 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_RANGED_ATTACK | PROC_TARGET_SELF;
		sp = dbcSpell.LookupEntryForced( 34839 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_RANGED_ATTACK | PROC_TARGET_SELF;

		// Hunter - Spirit Bond
		sp = dbcSpell.LookupEntryForced( 19578 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER | SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 19579;
		}
		sp = dbcSpell.LookupEntryForced( 20895 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER | SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 24529;
		}
		sp = dbcSpell.LookupEntryForced( 19579 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA; //we should do the same for player too as we did for pet
			sp->EffectApplyAuraName[1] = sp->EffectApplyAuraName[0];
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			sp->EffectBasePoints[1] = sp->EffectBasePoints[0];
			sp->EffectAmplitude[1] = sp->EffectAmplitude[0];
			sp->EffectDieSides[1] = sp->EffectDieSides[0];
		}
		sp = dbcSpell.LookupEntryForced( 24529 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA; //we should do the same for player too as we did for pet
			sp->EffectApplyAuraName[1] = sp->EffectApplyAuraName[0];
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			sp->EffectBasePoints[1] = sp->EffectBasePoints[0];
			sp->EffectAmplitude[1] = sp->EffectAmplitude[0];
			sp->EffectDieSides[1] = sp->EffectDieSides[0];
		}

		//Hunter Silencing Shot
		//http://www.naxxramas.net/bug_list/showreport.php?bugid=234 NTY
		sp = dbcSpell.LookupEntryForced(34490);
		if(sp != NULL)
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_SILENCE ;
		}

		// Hunter - Animal Handler
		sp = dbcSpell.LookupEntryForced( 34453 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_HIT_CHANCE;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
		}
		sp = dbcSpell.LookupEntryForced( 34454 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_HIT_CHANCE;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
		}

		// Hunter - Catlike Reflexes
		sp = dbcSpell.LookupEntryForced( 34462 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
			sp->EffectApplyAuraName[1] = sp->EffectApplyAuraName[0];
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
		}
		sp = dbcSpell.LookupEntryForced( 34464 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
			sp->EffectApplyAuraName[1] = sp->EffectApplyAuraName[0];
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
		}
		sp = dbcSpell.LookupEntryForced( 34465 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
			sp->EffectApplyAuraName[1] = sp->EffectApplyAuraName[0];
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
		}

		// Hunter - Serpent's Swiftness
		sp = dbcSpell.LookupEntryForced( 34466 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_HASTE;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
		}
		sp = dbcSpell.LookupEntryForced( 34467 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_HASTE;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
		}
		sp = dbcSpell.LookupEntryForced( 34468 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_HASTE;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
		}
		sp = dbcSpell.LookupEntryForced( 34469 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_HASTE;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
		}
		sp = dbcSpell.LookupEntryForced( 34470 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_HASTE;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
		}

		// Hunter - Ferocious Inspiration
		sp = dbcSpell.LookupEntryForced( 34455 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectTriggerSpell[0] = 34456;
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT | PROC_TARGET_SELF; //maybe target master ?
			sp->Effect[1] = 0; //remove this
		}
		sp = dbcSpell.LookupEntryForced( 34459 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectTriggerSpell[0] = 34456;
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT | PROC_TARGET_SELF; 
			sp->Effect[1] = 0; //remove this
		}
		sp = dbcSpell.LookupEntryForced( 34460 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectTriggerSpell[0] = 34456;
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT | PROC_TARGET_SELF;
			sp->Effect[1] = 0; //remove this
		}

		// Hunter - Focused Fire
		sp = dbcSpell.LookupEntryForced( 35029 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER | SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 35060;
			sp->EffectSpellGroupRelation_high[1] = 2048;
		}
		sp = dbcSpell.LookupEntryForced( 35030 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER | SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 35061;
			sp->EffectSpellGroupRelation_high[1] = 2048;
		}
		sp = dbcSpell.LookupEntryForced( 35060 );
		if( sp != NULL )
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
		sp = dbcSpell.LookupEntryForced( 35061 );
		if( sp != NULL )
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;

		// Hunter - Thick Hide
		sp = dbcSpell.LookupEntryForced( 19609 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_RESISTANCE; //we do not support armor rating for pets yet !
			sp->EffectBasePoints[0] *= 10; //just give it a little juice :P
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}
		sp = dbcSpell.LookupEntryForced( 19610 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_RESISTANCE; //we do not support armor rating for pets yet !
			sp->EffectBasePoints[0] *= 10; //just give it a little juice :P
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}
		sp = dbcSpell.LookupEntryForced( 19612 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_RESISTANCE; //we do not support armor rating for pets yet !
			sp->EffectBasePoints[0] *= 10; //just give it a little juice :P
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}

		// Hunter - Ferocity
		sp = dbcSpell.LookupEntryForced( 19612 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_CRIT_PERCENT;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}
		sp = dbcSpell.LookupEntryForced( 19599 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_CRIT_PERCENT;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}
		sp = dbcSpell.LookupEntryForced( 19600 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_CRIT_PERCENT;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}
		sp = dbcSpell.LookupEntryForced( 19601 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_CRIT_PERCENT;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}
		sp = dbcSpell.LookupEntryForced( 19602 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_CRIT_PERCENT;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}

		// Hunter - Bestial Swiftness
		sp = dbcSpell.LookupEntryForced( 19596 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_INCREASE_SPEED; 
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}

		// Hunter - Endurance Training
		sp = dbcSpell.LookupEntryForced( 19583 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}
		sp = dbcSpell.LookupEntryForced( 19584 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}
		sp = dbcSpell.LookupEntryForced( 19585 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}
		sp = dbcSpell.LookupEntryForced( 19586 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}
		sp = dbcSpell.LookupEntryForced( 19587 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}

		// Hunter - Thrill of the Hunt
		sp = dbcSpell.LookupEntryForced( 34497 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT | PROC_TARGET_SELF;
			sp->procChance = sp->EffectBasePoints[0]+1;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 34720;
		}
		sp = dbcSpell.LookupEntryForced( 34498 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT | PROC_TARGET_SELF;
			sp->procChance = sp->EffectBasePoints[0]+1;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 34720;
		}
		sp = dbcSpell.LookupEntryForced( 34499 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT | PROC_TARGET_SELF;
			sp->procChance = sp->EffectBasePoints[0]+1;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 34720;
		}

		// Hunter - Expose Weakness
		sp = dbcSpell.LookupEntryForced( 34500 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_RANGED_CRIT_ATTACK;
		sp = dbcSpell.LookupEntryForced( 34502 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_RANGED_CRIT_ATTACK;
		sp = dbcSpell.LookupEntryForced( 34503 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_RANGED_CRIT_ATTACK;

		// Hunter - Hawk Eye
		sp = dbcSpell.LookupEntryForced( 19498 );
		if( sp != NULL )
		{	
			sp->EffectSpellGroupRelation[0] = 1 | 512 | 2048 | 4096 | 16384 | 262144 | 32768 | 65536 | 131072 ;
			sp->EffectSpellGroupRelation_high[0] = 1 | 128 | 4096 ;
		}
		sp = dbcSpell.LookupEntryForced( 19499 );
		if( sp != NULL )
		{	
			sp->EffectSpellGroupRelation[0] = 1 | 512 | 2048 | 4096 | 16384 | 262144 | 32768 | 65536 | 131072 ;
			sp->EffectSpellGroupRelation_high[0] = 1 | 128 | 4096 ;
		}
		sp = dbcSpell.LookupEntryForced( 19500 );
		if( sp != NULL )
		{	
			sp->EffectSpellGroupRelation[0] = 1 | 512 | 2048 | 4096 | 16384 | 262144 | 32768 | 65536 | 131072 ;
			sp->EffectSpellGroupRelation_high[0] = 1 | 128 | 4096 ;
		}

		//Hunter - Frenzy
		sp = dbcSpell.LookupEntryForced( 19621 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 19615;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->procChance = sp->EffectBasePoints[0];
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET | SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET | PROC_TARGET_SELF;
		}
		sp = dbcSpell.LookupEntryForced( 19622 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 19615;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->procChance = sp->EffectBasePoints[0];
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET | SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET | PROC_TARGET_SELF;
		}
		sp = dbcSpell.LookupEntryForced( 19623 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 19615;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->procChance = sp->EffectBasePoints[0];
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET | SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET | PROC_TARGET_SELF;
		}
		sp = dbcSpell.LookupEntryForced( 19624 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 19615;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->procChance = sp->EffectBasePoints[0];
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET | SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET | PROC_TARGET_SELF;
		}
		sp = dbcSpell.LookupEntryForced( 19625 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 19615;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->procChance = sp->EffectBasePoints[0];
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET | SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET | PROC_TARGET_SELF;
		}

		//Hunter - Unleashed Fury
		sp = dbcSpell.LookupEntryForced( 19616 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectMiscValue[0] = 1; //tweekign melee dmg
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET | SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
		}
		sp = dbcSpell.LookupEntryForced( 19617 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectMiscValue[0] = 1; //tweekign melee dmg
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET | SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
		}
		sp = dbcSpell.LookupEntryForced( 19618 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectMiscValue[0] = 1; //tweekign melee dmg
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET | SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
		}
		sp = dbcSpell.LookupEntryForced( 19619 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectMiscValue[0] = 1; //tweekign melee dmg
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET | SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
		}
		sp = dbcSpell.LookupEntryForced( 19620 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectMiscValue[0] = 1; //tweekign melee dmg
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET | SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
		}

		//Hunter : Pathfinding
		sp = dbcSpell.LookupEntryForced( 19559 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 2097152;
			sp->EffectMiscValue[0] = SMT_SPELL_VALUE;
		}
		sp = dbcSpell.LookupEntryForced( 19560 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 2097152;
			sp->EffectMiscValue[0] = SMT_SPELL_VALUE;
		}
		sp = dbcSpell.LookupEntryForced( 5131 );
		if( sp != NULL )
			sp->SpellGroupType = 2097152;
		sp = dbcSpell.LookupEntryForced( 13160 );
		if( sp != NULL )
			sp->SpellGroupType = 2097152;

		//Hunter : Rapid Killing - might need to add honor trigger too here. I'm guessing you receive Xp too so i'm avoiding double proc
		sp = dbcSpell.LookupEntryForced( 34948 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_GAIN_EXPIERIENCE | PROC_TARGET_SELF;
			sp->EffectSpellGroupRelation[1] = 32;
		}
		sp = dbcSpell.LookupEntryForced( 34949 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_GAIN_EXPIERIENCE | PROC_TARGET_SELF;
			sp->EffectSpellGroupRelation[1] = 32;
		}
		//Hunter : Rapid Killing - PROC
		sp = dbcSpell.LookupEntryForced( 35098 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 131072 | 2048 | 1;
		sp = dbcSpell.LookupEntryForced( 35099 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 131072 | 2048 | 1;

		//Hunter : Improved Stings
		sp = dbcSpell.LookupEntryForced( 19464 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 16384;
			sp->EffectSpellGroupRelation_high[0] = 256;
			sp->EffectSpellGroupRelation_high[1] = 128;
			sp->EffectSpellGroupRelation[2] = 16384 | 32768;
			sp->EffectSpellGroupRelation_high[2] = 128 | 4096;
		}
		sp = dbcSpell.LookupEntryForced( 19465 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 16384;
			sp->EffectSpellGroupRelation_high[0] = 256;
			sp->EffectSpellGroupRelation_high[1] = 128;
			sp->EffectSpellGroupRelation[2] = 16384 | 32768;
			sp->EffectSpellGroupRelation_high[2] = 128 | 4096;
		}
		sp = dbcSpell.LookupEntryForced( 19466 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 16384;
			sp->EffectSpellGroupRelation_high[0] = 256;
			sp->EffectSpellGroupRelation_high[1] = 128;
			sp->EffectSpellGroupRelation[2] = 16384 | 32768;
			sp->EffectSpellGroupRelation_high[2] = 128 | 4096;
		}
		sp = dbcSpell.LookupEntryForced( 19467 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 16384;
			sp->EffectSpellGroupRelation_high[0] = 256;
			sp->EffectSpellGroupRelation_high[1] = 128;
			sp->EffectSpellGroupRelation[2] = 16384 | 32768;
			sp->EffectSpellGroupRelation_high[2] = 128 | 4096;
		}
		sp = dbcSpell.LookupEntryForced( 19468 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 16384;
			sp->EffectSpellGroupRelation_high[0] = 256;
			sp->EffectSpellGroupRelation_high[1] = 128;
			sp->EffectSpellGroupRelation[2] = 16384 | 32768;
			sp->EffectSpellGroupRelation_high[2] = 128 | 4096;
		}

		//we need to adress this somehow : shot
		sp = dbcSpell.LookupEntryForced( 3018 );
		if( sp != NULL )
			sp->SpellGroupType = 4;

		//Hunter : Mortal Shots
		sp = dbcSpell.LookupEntryForced( 19485 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1 | 4 | 2048 | 4096 | 131072 | 262144 ;
			sp->EffectSpellGroupRelation_high[0] = 1 ;
		}
		sp = dbcSpell.LookupEntryForced( 19487 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1 | 4 | 2048 | 4096 | 131072 | 262144 ;
			sp->EffectSpellGroupRelation_high[0] = 1 ;
		}
		sp = dbcSpell.LookupEntryForced( 19488 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1 | 4 | 2048 | 4096 | 131072 | 262144 ;
			sp->EffectSpellGroupRelation_high[0] = 1 ;
		}
		sp = dbcSpell.LookupEntryForced( 19489 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1 | 4 | 2048 | 4096 | 131072 | 262144 ;
			sp->EffectSpellGroupRelation_high[0] = 1 ;
		}
		sp = dbcSpell.LookupEntryForced( 19490 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1 | 4 | 2048 | 4096 | 131072 | 262144 ;
			sp->EffectSpellGroupRelation_high[0] = 1 ;
		}

		//Hunter : Improved Barrage
		sp = dbcSpell.LookupEntryForced( 35104 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 4096;
			sp->EffectSpellGroupRelation[1] = 8192;
		}
		sp = dbcSpell.LookupEntryForced( 35110 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 4096;
			sp->EffectSpellGroupRelation[1] = 8192;
		}
		sp = dbcSpell.LookupEntryForced( 35111 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 4096;
			sp->EffectSpellGroupRelation[1] = 8192;
		}

		//Hunter : Clever Traps
		sp = dbcSpell.LookupEntryForced( 19239 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 8 | 16;
			sp->EffectSpellGroupRelation[1] = 4;
			sp->EffectSpellGroupRelation_high[1] = 8192;
		}
		sp = dbcSpell.LookupEntryForced( 19245 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 8 | 16;
			sp->EffectSpellGroupRelation[1] = 4;
			sp->EffectSpellGroupRelation_high[1] = 8192;
		}

		//Hunter : Resourcefulness
		sp = dbcSpell.LookupEntryForced( 34491 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 128 | 2 | 64; 
			sp->EffectSpellGroupRelation_high[0] = 524288; //was missing Counterattack =P
			sp->EffectSpellGroupRelation[1] = 128;
		}
		sp = dbcSpell.LookupEntryForced( 34492 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 128 | 2 | 64; 
			sp->EffectSpellGroupRelation_high[0] = 524288; //was missing Counterattack =P
			sp->EffectSpellGroupRelation[1] = 128;
		}
		sp = dbcSpell.LookupEntryForced( 34493 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 128 | 2 | 64; 
			sp->EffectSpellGroupRelation_high[0] = 524288; //was missing Counterattack =P
			sp->EffectSpellGroupRelation[1] = 128;
		}

		
		//Hunter : Entrapment
		sp = dbcSpell.LookupEntryForced( 19184 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_TRAP_TRIGGER;
		sp = dbcSpell.LookupEntryForced( 19387 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_TRAP_TRIGGER;
		sp = dbcSpell.LookupEntryForced( 19388 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_TRAP_TRIGGER;
		
		/* aspect of the pack - change to AA */
		sp = dbcSpell.LookupEntryForced( 13159 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;

		/* aspect of the cheetah - add proc flags */
		sp = dbcSpell.LookupEntryForced( 5118 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;

		sp = dbcSpell.LookupEntryForced(34471);
		if(sp!=NULL)
		{
					sp->EffectApplyAuraName[0] = 72 ;
					sp->EffectApplyAuraName[1] = 79 ;
					sp->EffectApplyAuraName[2] = 77 ;
		}

	//////////////////////////////////////////
	// ROGUE								//
	//////////////////////////////////////////

	// Insert rogue spell fixes here

		/**********************************************************
		 *	Garrote - this is used? 
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 37066 );
		if( sp != NULL )
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_ENEMY;
		}

		//rogue ( grouping ) Elusiveness = blind + vanish
		group_relation_rogue_elusiveness = 0x00000800 | 0x01000000;

		//rogue - Elusiveness
		sp = dbcSpell.LookupEntryForced( 13981 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_rogue_elusiveness;
		sp = dbcSpell.LookupEntryForced( 14066 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_rogue_elusiveness;

		//rogue ( grouping ) Poisons
		group_relation_rogue_poisons = 0x00002000 | 0x00004000 | 0x00008000 | 0x00010000 | 0x10000000;

		//rogue - Vile Poisons
		sp = dbcSpell.LookupEntryForced( 14168 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = group_relation_rogue_poisons;
			sp->EffectSpellGroupRelation[1] = 0x00800000; //maybe this is mixed up with 0 grouprelation ?
			sp->EffectSpellGroupRelation[2] = group_relation_rogue_poisons;
		}
		sp = dbcSpell.LookupEntryForced( 16514 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = group_relation_rogue_poisons;
			sp->EffectSpellGroupRelation[1] = 0x00800000; //maybe this is mixed up with 0 grouprelation ?
			sp->EffectSpellGroupRelation[2] = group_relation_rogue_poisons;
		}
		sp = dbcSpell.LookupEntryForced( 16515 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = group_relation_rogue_poisons;
			sp->EffectSpellGroupRelation[1] = 0x00800000; //maybe this is mixed up with 0 grouprelation ?
			sp->EffectSpellGroupRelation[2] = group_relation_rogue_poisons;
		}
		sp = dbcSpell.LookupEntryForced( 16719 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = group_relation_rogue_poisons;
			sp->EffectSpellGroupRelation[1] = 0x00800000; //maybe this is mixed up with 0 grouprelation ?
			sp->EffectSpellGroupRelation[2] = group_relation_rogue_poisons;
		}
		sp = dbcSpell.LookupEntryForced( 16720 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = group_relation_rogue_poisons;
			sp->EffectSpellGroupRelation[1] = 0x00800000; //maybe this is mixed up with 0 grouprelation ?
			sp->EffectSpellGroupRelation[2] = group_relation_rogue_poisons;
		}

		//rogue - Improved Poisons
		sp = dbcSpell.LookupEntryForced( 14113 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_rogue_poisons;
		sp = dbcSpell.LookupEntryForced( 14114 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_rogue_poisons;
		sp = dbcSpell.LookupEntryForced( 14115 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_rogue_poisons;
		sp = dbcSpell.LookupEntryForced( 14116 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_rogue_poisons;
		sp = dbcSpell.LookupEntryForced( 14117 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_rogue_poisons;
		sp = dbcSpell.LookupEntryForced( 21881 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_rogue_poisons;

		//rogue - Improved Expose Armor
		sp = dbcSpell.LookupEntryForced( 14168 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 0x00080000;
		sp = dbcSpell.LookupEntryForced( 14169 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 0x00080000;

		//rogue - Master Poisoner.
		sp = dbcSpell.LookupEntryForced( 31226 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_rogue_poisons;
		sp = dbcSpell.LookupEntryForced( 31227 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_rogue_poisons;

		//rogue - Find Weakness.
		sp = dbcSpell.LookupEntryForced( 31233 ); 
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;
		sp = dbcSpell.LookupEntryForced( 31239 ); 
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;
		sp = dbcSpell.LookupEntryForced( 31240 ); 
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;
		sp = dbcSpell.LookupEntryForced( 31241 ); 
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;
		sp = dbcSpell.LookupEntryForced( 31242 ); 
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;

		//rogue ( grouping ) Find Weakness
		group_relation_rogue_find_weakness = 0x00000008 | 0x00000010 | 0x00000100 | 0x00100000 | 0x00800000 | 0x04000000 | 0x20000000;

		//rogue - Find Weakness. The effect
		sp = dbcSpell.LookupEntryForced( 31234 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_rogue_find_weakness;
		sp = dbcSpell.LookupEntryForced( 31235 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_rogue_find_weakness;
		sp = dbcSpell.LookupEntryForced( 31236 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_rogue_find_weakness;
		sp = dbcSpell.LookupEntryForced( 31237 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_rogue_find_weakness;
		sp = dbcSpell.LookupEntryForced( 31238 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_rogue_find_weakness;
		
		//rogue - Camouflage.
		sp = dbcSpell.LookupEntryForced( 13975 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 4194304;
			sp->EffectMiscValue[0] = SMT_SPELL_VALUE;
			sp->EffectSpellGroupRelation[1] = 4194304;
		}
		sp = dbcSpell.LookupEntryForced( 14062 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 4194304;
			sp->EffectMiscValue[0] = SMT_SPELL_VALUE;
			sp->EffectSpellGroupRelation[1] = 4194304;
		}
		sp = dbcSpell.LookupEntryForced( 14063 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 4194304;
			sp->EffectMiscValue[0] = SMT_SPELL_VALUE;
			sp->EffectSpellGroupRelation[1] = 4194304;
		}
		sp = dbcSpell.LookupEntryForced( 14064 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 4194304;
			sp->EffectMiscValue[0] = SMT_SPELL_VALUE;
			sp->EffectSpellGroupRelation[1] = 4194304;
		}
		sp = dbcSpell.LookupEntryForced( 14065 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 4194304;
			sp->EffectMiscValue[0] = SMT_SPELL_VALUE;
			sp->EffectSpellGroupRelation[1] = 4194304;
		}

		//rogue - Vanish : Second Trigger Spell
		sp = dbcSpell.LookupEntryForced( 18461 );
		if( sp != NULL )
			sp->AttributesEx |= ATTRIBUTESEX_NOT_BREAK_STEALTH;

		// rogue - shadowstep
//		sp = dbcSpell.LookupEntryForced( 36554 );
//		if( sp != NULL )
//			sp->AttributesEx |= ATTRIBUTESEX_NOT_BREAK_STEALTH;

		// rogue - Blind (Make it able to miss!)
		sp = dbcSpell.LookupEntryForced( 2094 );
		if( sp != NULL )
		{
			sp->Spell_Dmg_Type = SPELL_DMG_TYPE_RANGED;
			sp->is_ranged_spell = true; 
		}
		//rogue - Mace Specialization.
		sp = dbcSpell.LookupEntryForced( 13709 ); 
		if( sp != NULL )
			sp->procFlags = PROC_ON_MELEE_ATTACK;
		sp = dbcSpell.LookupEntryForced( 13800 ); 
		if( sp != NULL )
			sp->procFlags = PROC_ON_MELEE_ATTACK;
		sp = dbcSpell.LookupEntryForced( 13801 ); 
		if( sp != NULL )
			sp->procFlags = PROC_ON_MELEE_ATTACK;
		sp = dbcSpell.LookupEntryForced( 13802 ); 
		if( sp != NULL )
			sp->procFlags = PROC_ON_MELEE_ATTACK;
		sp = dbcSpell.LookupEntryForced( 13803 ); 
		if( sp != NULL )
			sp->procFlags = PROC_ON_MELEE_ATTACK;

		//rogue - Dirty Tricks 
		sp = dbcSpell.LookupEntryForced( 14076 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 16777216 | 128; // blind + sap
			sp->EffectSpellGroupRelation[1] = 16777216 | 128; // blind + sap
		}
		sp = dbcSpell.LookupEntryForced( 14094 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 16777216 | 128; // blind + sap
			sp->EffectSpellGroupRelation[1] = 16777216 | 128; // blind + sap
		}

		//rogue - Dirty Deeds
		sp = dbcSpell.LookupEntryForced( 14082 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1024 | 256; // Cheap Shot + Garrote
		sp = dbcSpell.LookupEntryForced( 14083 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1024 | 256; // Cheap Shot + Garrote


		//rogue ( grouping ) Shadowstep
		group_relation_rogue_shadow_step |= 512;//rogue - ambush (only a part of the whole group since it would affect other spells too)
		group_relation_rogue_shadow_step |= 4;//rogue - Backstab (only a part of the whole group since it would affect other spells too)
		group_relation_rogue_shadow_step |= 256;//Garrote
		group_relation_rogue_shadow_step |= 536870912 | 16 | 8 | 8389120 | 41943040 | 33554432 | 32 | 67108864 | 64 | 128 ;

		//rogue - Shadowstep
		sp = dbcSpell.LookupEntryForced( 36563 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[2] = group_relation_rogue_shadow_step;
			sp->EffectSpellGroupRelation_high[2] = 256 | 128 ;
			sp->EffectMiscValue[2] = SMT_SPELL_VALUE;
//			sp->EffectTriggerSpell[1] = 36554;
//			sp->EffectTriggerSpell[2] = 44373;
//			sp->procFlags = PROC_ON_CAST_SPELL;	
		}
		// Still related to shadowstep - prevent the trigger spells from breaking stealth.
		sp = dbcSpell.LookupEntryForced( 44373 );
		if( sp )
			sp->AttributesEx |= ATTRIBUTESEX_NOT_BREAK_STEALTH;
		sp = dbcSpell.LookupEntryForced( 36563 );
		if( sp )
			sp->AttributesEx |= ATTRIBUTESEX_NOT_BREAK_STEALTH;
		sp = dbcSpell.LookupEntryForced( 36554 );
		if( sp != NULL )
			sp->AttributesEx |= ATTRIBUTESEX_NOT_BREAK_STEALTH;

		//rogue ( grouping ) Lethality
		group_relation_rogue_lethality |= 2;//rogue - Sinister Strike (only a part of the whole group since it would affect other spells too)
		group_relation_rogue_lethality |= 4;//rogue - backstab (only a part of the whole group since it would affect other spells too)
		group_relation_rogue_lethality |= 8;//rogue - Gouge (only a part of the whole group since it would affect other spells too)
		group_relation_rogue_lethality |= 33554432UL;//rogue - Hemorrhage (only a part of the whole group since it would affect other spells too)
		group_relation_rogue_lethality |= 536870912UL;//rogue - Shiv (only a part of the whole group since it would affect other spells too)
		group_relation_rogue_lethality |= 1073741824UL;//rogue - Ghostly Strike (only a part of the whole group since it would affect other spells too)

		//rogue Lethality
		sp = dbcSpell.LookupEntryForced( 14128 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_rogue_lethality;
		sp = dbcSpell.LookupEntryForced( 14132 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_rogue_lethality;
		sp = dbcSpell.LookupEntryForced( 14135 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_rogue_lethality;
		sp = dbcSpell.LookupEntryForced( 14136 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_rogue_lethality;
		sp = dbcSpell.LookupEntryForced( 14137 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_rogue_lethality;

		//rogue - Endurance 
		sp = dbcSpell.LookupEntryForced( 13742 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 64 | 32;	//Sprint + Evasion
		sp = dbcSpell.LookupEntryForced( 13872 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 64 | 32;	//Sprint + Evasion
		//rogue - Seal Fate
		sp = dbcSpell.LookupEntryForced( 14186 );
		if( sp != NULL ) 
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 14189;
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->procChance = 20;
		}
		sp = dbcSpell.LookupEntryForced( 14190 );
		if( sp != NULL ) 
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 14189;
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->procChance = 40;
		}
		sp = dbcSpell.LookupEntryForced( 14193 );
		if( sp != NULL ) 
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 14189;
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->procChance = 60;
		}
		sp = dbcSpell.LookupEntryForced( 14194 );
		if( sp != NULL ) 
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 14189;
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->procChance = 80;
		}
		sp = dbcSpell.LookupEntryForced( 14195 );
		if( sp != NULL ) 
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 14189;
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->procChance = 100;
		}
	#ifndef NEW_PROCFLAGS
		//Improved Sprint
		sp = dbcSpell.LookupEntryForced( 13743 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 50;
		}
		sp = dbcSpell.LookupEntryForced( 13875 );
		if( sp != NULL )
		{
			sp->procChance = 100;
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
	#else
		//Improved Sprint
		sp = dbcSpell.LookupEntryForced( 13743 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0]=64;
		sp = dbcSpell.LookupEntryForced( 13875 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0]=64;
	#endif
		//rogue-shiv -> add 1 combo point
		sp = dbcSpell.LookupEntryForced( 5938 );
		if( sp != NULL )
			sp->Effect[1]=80;
		//garrot
		sp = dbcSpell.LookupEntryForced( 703 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 8631 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 8632 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 8633 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 11289 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 11290 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 26839 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 26884 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;

		//rupture
		sp = dbcSpell.LookupEntryForced( 1943 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 8639 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 8640 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 11273 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 11274 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 11275 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 26867 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
	#ifndef NEW_PROCFLAGS
		//Relentless Strikes
		sp = dbcSpell.LookupEntryForced( 14179 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;//proc spell
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectBasePoints[1] = 20; //client showes 20% chance but whe do not have it ? :O
		}
	#else
		//Relentless Strikes
		sp = dbcSpell.LookupEntryForced( 14179 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0]= 262144 | 2097152 | 8388608 | 8519680 | 524288 | 1048576 | 8388608;
	#endif
	#ifndef NEW_PROCFLAGS
		//rogue - intiative
		sp = dbcSpell.LookupEntryForced( 13976 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = uint32(PROC_ON_CAST_SPELL|PROC_TARGET_SELF);
		}
		sp = dbcSpell.LookupEntryForced( 13979 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = uint32(PROC_ON_CAST_SPELL|PROC_TARGET_SELF);
		}
		sp = dbcSpell.LookupEntryForced( 13980 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = uint32(PROC_ON_CAST_SPELL|PROC_TARGET_SELF);
		}
	#else
		//rogue - intiative
		sp = dbcSpell.LookupEntryForced( 13976 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 8389120 | 256 | 1024;
		sp = dbcSpell.LookupEntryForced( 13979 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 8389120 | 256 | 1024;
		sp = dbcSpell.LookupEntryForced( 13980 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 8389120 | 256 | 1024;
	#endif

	//////////////////////////////////////////
	// PRIEST								//
	//////////////////////////////////////////

	// Insert priest spell fixes here

		//shadowfiend
		sp = dbcSpell.LookupEntryForced( 41913 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_MELEE_ATTACK;
		//shadowfiend proc -> kinda rewriting this
		sp = dbcSpell.LookupEntryForced( 41914 );
		if( sp != NULL )
		{
			sp->Effect[0] = SPELL_EFFECT_ENERGIZE;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET_MASTER;
			sp->EffectBasePoints[0] = 100; // represent pct value to convert dmg to mana
			sp->Effect[1] = sp->Effect[2] = 0; //disable these
		}
		/**********************************************************
		 *	Holy Nova
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 15237 );
		if( sp != NULL )
		{
			sp->Effect[1] = 64;
			sp->EffectTriggerSpell[1] = 23455;
		}
		sp = dbcSpell.LookupEntryForced( 15430 );
		if( sp != NULL )
		{
			sp->Effect[1] = 64;
			sp->EffectTriggerSpell[1] = 23458;
		}
		sp = dbcSpell.LookupEntryForced( 15431 );
		if( sp != NULL )
		{
			sp->Effect[1] = 64;
			sp->EffectTriggerSpell[1] = 23459;
		}
		sp = dbcSpell.LookupEntryForced( 27799 );
		if( sp != NULL )
		{
			sp->Effect[1] = 64;
			sp->EffectTriggerSpell[1] = 27803;
		}
		sp = dbcSpell.LookupEntryForced( 27800 );
		if( sp != NULL )
		{
			sp->Effect[1] = 64;
			sp->EffectTriggerSpell[1] = 27804;
		}
		sp = dbcSpell.LookupEntryForced( 27801 );
		if( sp != NULL )
		{
			sp->Effect[1] = 64;
			sp->EffectTriggerSpell[1] = 27805;
		}
		sp = dbcSpell.LookupEntryForced( 25331 );
		if( sp != NULL )
		{
			sp->Effect[1] = 64;
			sp->EffectTriggerSpell[1] = 25329;
		}
		 //priest - Holy Concentration
		 sp = dbcSpell.LookupEntryForced( 34753 );
		 if (sp != NULL)
			  sp->procFlags = PROC_ON_CAST_SPELL;
		 sp = dbcSpell.LookupEntryForced( 34859 );
		 if (sp != NULL)
			 sp->procFlags = PROC_ON_CAST_SPELL;
		 sp = dbcSpell.LookupEntryForced( 34860 );
		 if (sp != NULL)
			  sp->procFlags = PROC_ON_CAST_SPELL;
		 sp = dbcSpell.LookupEntryForced( 34754 );
		 if (sp != NULL)
		 {
			  //sp->EffectSpellGroupRelation[0] = 2048 | 4096;
			  sp->EffectSpellGroupRelation_high[0] = 4;
		 }

		//priest - Focused Mind 
		sp = dbcSpell.LookupEntryForced( 33213 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 8192 | 131072 | 8388608; // Mind Blast + Mind Control + Mind Flay
		sp = dbcSpell.LookupEntryForced( 33214 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 8192 | 131072 | 8388608; // Mind Blast + Mind Control + Mind Flay
		sp = dbcSpell.LookupEntryForced( 33215 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 8192 | 131072 | 8388608; // Mind Blast + Mind Control + Mind Flay

		//Priest: Blessed Recovery
		sp = dbcSpell.LookupEntryForced(27811);
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[0] = 27813;
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM;
		}
		sp = dbcSpell.LookupEntryForced(27815);
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[0] = 27817;
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM;
		}
		sp = dbcSpell.LookupEntryForced(27816);
		if(sp != NULL)
		{
			sp->EffectTriggerSpell[0] = 27818;
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM;
		}
		//priest- Blessed Resilience
		sp = dbcSpell.LookupEntryForced( 33142 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM;
			sp->procChance = 20;
		}
		sp = dbcSpell.LookupEntryForced( 33145 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM;
			sp->procChance = 40;
		}
		sp = dbcSpell.LookupEntryForced( 33146 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM;
			sp->procChance = 60;
		}

		//priest- Focused Will
		sp = dbcSpell.LookupEntryForced( 45234 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM;
		sp = dbcSpell.LookupEntryForced( 45243 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM;
		sp = dbcSpell.LookupEntryForced( 45244 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM;

		//priest - Improved Divine Spirit 
		sp = dbcSpell.LookupEntryForced( 33174 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 32 ;
			sp->EffectSpellGroupRelation[1] = 32 ;
		}
		sp = dbcSpell.LookupEntryForced( 33182 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 32 ;
			sp->EffectSpellGroupRelation[1] = 32 ;
		}

		//priest - Empowered Healing 
		sp = dbcSpell.LookupEntryForced( 33158 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 4096 ;
			sp->EffectSpellGroupRelation[1] = 2048 ;
			sp->EffectSpellGroupRelation_high[1] = 4 ;
		}
		sp = dbcSpell.LookupEntryForced( 33159 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 4096 ;
			sp->EffectSpellGroupRelation[1] = 2048 ;
			sp->EffectSpellGroupRelation_high[1] = 4 ;
		}
		sp = dbcSpell.LookupEntryForced( 33160 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 4096 ;
			sp->EffectSpellGroupRelation[1] = 2048 ;
			sp->EffectSpellGroupRelation_high[1] = 4 ;
		}
		sp = dbcSpell.LookupEntryForced( 33161 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 4096 ;
			sp->EffectSpellGroupRelation[1] = 2048 ;
			sp->EffectSpellGroupRelation_high[1] = 4 ;
		}
		sp = dbcSpell.LookupEntryForced( 33162 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 4096 ;
			sp->EffectSpellGroupRelation[1] = 2048 ;
			sp->EffectSpellGroupRelation_high[1] = 4 ;
		}

		//priest - Force of Will 
		sp = dbcSpell.LookupEntryForced( 18544 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 4194304 | 128 | 32768 | 8192 | 16 | 1048576 | 8388608 | 2097152 | 67108864;
			sp->EffectSpellGroupRelation[1] = 4194304 | 128 | 32768 | 8192 | 16 | 1048576 | 8388608 | 2097152 | 67108864;
			sp->EffectSpellGroupRelation[2] = 0 ;	//1-2 mod the same ?
			sp->EffectSpellGroupRelation_high[0] = 2 | 1024 ;
			sp->EffectSpellGroupRelation_high[1] = 2 | 1024 ;
		}
		sp = dbcSpell.LookupEntryForced( 18547 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 4194304 | 128 | 32768 | 8192 | 16 | 1048576 | 8388608 | 2097152 | 67108864;
			sp->EffectSpellGroupRelation[1] = 4194304 | 128 | 32768 | 8192 | 16 | 1048576 | 8388608 | 2097152 | 67108864;
			sp->EffectSpellGroupRelation[2] = 0 ;	//1-2 mod the same ?
			sp->EffectSpellGroupRelation_high[0] = 2 | 1024 ;
			sp->EffectSpellGroupRelation_high[1] = 2 | 1024 ;
		}
		sp = dbcSpell.LookupEntryForced( 18548 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 4194304 | 128 | 32768 | 8192 | 16 | 1048576 | 8388608 | 2097152 | 67108864;
			sp->EffectSpellGroupRelation[1] = 4194304 | 128 | 32768 | 8192 | 16 | 1048576 | 8388608 | 2097152 | 67108864 ;
			sp->EffectSpellGroupRelation[2] = 0 ;	//1-2 mod the same ?
			sp->EffectSpellGroupRelation_high[0] = 2 | 1024 ;
			sp->EffectSpellGroupRelation_high[1] = 2 | 1024 ;
		}
		sp = dbcSpell.LookupEntryForced( 18549 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 4194304 | 128 | 32768 | 8192 | 16 | 1048576 | 8388608 | 2097152 | 67108864;
			sp->EffectSpellGroupRelation[1] = 4194304 | 128 | 32768 | 8192 | 16 | 1048576 | 8388608 | 2097152 | 67108864;
			sp->EffectSpellGroupRelation[2] = 0;	//1-2 mod the same ?
			sp->EffectSpellGroupRelation_high[0] = 2 | 1024 ;
			sp->EffectSpellGroupRelation_high[1] = 2 | 1024 ;
		}
		sp = dbcSpell.LookupEntryForced( 18550 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 4194304 | 128 | 32768 | 8192 | 16 | 1048576 | 8388608 | 2097152 | 67108864;
			sp->EffectSpellGroupRelation[1] = 4194304 | 128 | 32768 | 8192 | 16 | 1048576 | 8388608 | 2097152 | 67108864;
			sp->EffectSpellGroupRelation[2] = 0;	//1-2 mod the same ?
			sp->EffectSpellGroupRelation_high[0] = 2 | 1024;
			sp->EffectSpellGroupRelation_high[1] = 2 | 1024;
		}

		//Priest: Shadowguard
		sp = dbcSpell.LookupEntryForced( 18137 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_SPELL_HIT_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM | PROC_ON_MELEE_ATTACK_VICTIM;
			sp->proc_interval = 3000; //every 3 seconds
			sp->EffectTriggerSpell[0] = 28377;
		}
		sp = dbcSpell.LookupEntryForced( 19308 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_SPELL_HIT_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM | PROC_ON_MELEE_ATTACK_VICTIM;
			sp->proc_interval = 3000; //every 3 seconds
			sp->EffectTriggerSpell[0] = 28378;
		}
		sp = dbcSpell.LookupEntryForced( 19309 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_SPELL_HIT_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM | PROC_ON_MELEE_ATTACK_VICTIM;
			sp->proc_interval = 3000; //every 3 seconds
			sp->EffectTriggerSpell[0] = 28379;
		}
		sp = dbcSpell.LookupEntryForced( 19310 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_SPELL_HIT_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM | PROC_ON_MELEE_ATTACK_VICTIM;
			sp->proc_interval = 3000; //every 3 seconds
			sp->EffectTriggerSpell[0] = 28380;
		}
		sp = dbcSpell.LookupEntryForced( 19311 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_SPELL_HIT_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM | PROC_ON_MELEE_ATTACK_VICTIM;
			sp->proc_interval = 3000; //every 3 seconds
			sp->EffectTriggerSpell[0] = 28381;
		}
		sp = dbcSpell.LookupEntryForced( 19312 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_SPELL_HIT_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM | PROC_ON_MELEE_ATTACK_VICTIM;
			sp->proc_interval = 3000; //every 3 seconds
			sp->EffectTriggerSpell[0] = 28382;
		}
		sp = dbcSpell.LookupEntryForced( 25477 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_SPELL_HIT_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM | PROC_ON_MELEE_ATTACK_VICTIM;
			sp->proc_interval = 3000; //every 3 seconds
			sp->EffectTriggerSpell[0] = 28385;
		}

		//priest - Absolution 
		sp = dbcSpell.LookupEntryForced( 33167 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 2147483648UL;
			sp->EffectSpellGroupRelation_high[0] = 1 | 128;
		}
		sp = dbcSpell.LookupEntryForced( 33171 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 2147483648UL;
			sp->EffectSpellGroupRelation_high[0] = 1 | 128;
		}
		sp = dbcSpell.LookupEntryForced( 33172 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 2147483648UL;
			sp->EffectSpellGroupRelation_high[0] = 1 | 128;
		}

		//priest - Mental Agility - all instant spells. I wonder if it conflicts with any other spells 
		sp = dbcSpell.LookupEntryForced( 14520 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 2147483648UL | 65536 | 67108864UL | 4 | 1 | 64 | 32 | 4194304UL | 32768 | 8388608UL | 8 | 16384 | 2 | 256 | 16777216UL | 2097152UL | 33554432UL;
			sp->EffectSpellGroupRelation_high[0] = 1 | 64 | 2;
		}
		sp = dbcSpell.LookupEntryForced( 14780 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 2147483648UL | 65536 | 67108864UL | 4 | 1 | 64 | 32 | 4194304UL | 32768 | 8388608UL | 8 | 16384 | 2 | 256 | 16777216UL | 2097152UL | 33554432UL;
			sp->EffectSpellGroupRelation_high[0] = 1 | 64 | 2;
		}
		sp = dbcSpell.LookupEntryForced( 14781 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 2147483648UL | 65536 | 67108864UL | 4 | 1 | 64 | 32 | 4194304UL | 32768 | 8388608UL | 8 | 16384 | 2 | 256 | 16777216UL | 2097152UL | 33554432UL;
			sp->EffectSpellGroupRelation_high[0] = 1 | 64 | 2;
		}
		sp = dbcSpell.LookupEntryForced( 14782 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 2147483648UL | 65536 | 67108864UL | 4 | 1 | 64 | 32 | 4194304UL | 32768 | 8388608UL | 8 | 16384 | 2 | 256 | 16777216UL | 2097152UL | 33554432UL;
			sp->EffectSpellGroupRelation_high[0] = 1 | 64 | 2;
		}
		sp = dbcSpell.LookupEntryForced( 14783 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 2147483648UL | 65536 | 67108864UL | 4 | 1 | 64 | 32 | 4194304UL | 32768 | 8388608UL | 8 | 16384 | 2 | 256 | 16777216UL | 2097152UL | 33554432UL;
			sp->EffectSpellGroupRelation_high[0] = 1 | 64 | 2;
		}

		//priest - Focused Power
		sp = dbcSpell.LookupEntryForced( 33186 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 2147483648UL;
			sp->EffectSpellGroupRelation[1] = 128 | 8192 | 2147483648UL;
		}
		sp = dbcSpell.LookupEntryForced( 33190 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 2147483648UL;
			sp->EffectSpellGroupRelation[1] = 128 | 8192 | 2147483648UL;
		}

		//priest - Shadow Reach 
		sp = dbcSpell.LookupEntryForced( 17322 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 32768 | 65536 | 4 | 8192 | 16 | 8388608 | 2147483648UL;
		sp = dbcSpell.LookupEntryForced( 17323 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 32768 | 65536 | 4 | 8192 | 16 | 8388608 | 2147483648UL;

		//priest - Shadow Focus 
		sp = dbcSpell.LookupEntryForced( 15260 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 65536 | 67108864 | 131072 | 2147483648UL | 32768 | 8192 | 16 | 4 | 8388608 | 16384 | 256 | 33554432;
			sp->EffectSpellGroupRelation_high[0] = 64 | 2 | 1024;
		}
		sp = dbcSpell.LookupEntryForced( 15327 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 65536 | 67108864 | 131072 | 2147483648UL | 32768 | 8192 | 16 | 4 | 8388608 | 16384 | 256 | 33554432;
			sp->EffectSpellGroupRelation_high[0] = 64 | 2 | 1024;
		}
		sp = dbcSpell.LookupEntryForced( 15328 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 65536 | 67108864 | 131072 | 2147483648UL | 32768 | 8192 | 16 | 4 | 8388608 | 16384 | 256 | 33554432;
			sp->EffectSpellGroupRelation_high[0] = 64 | 2 | 1024;
		}
		sp = dbcSpell.LookupEntryForced( 15329 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 65536 | 67108864 | 131072 | 2147483648UL | 32768 | 8192 | 16 | 4 | 8388608 | 16384 | 256 | 33554432;
			sp->EffectSpellGroupRelation_high[0] = 64 | 2 | 1024;
		}
		sp = dbcSpell.LookupEntryForced( 15330 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 65536 | 67108864 | 131072 | 2147483648UL | 32768 | 8192 | 16 | 4 | 8388608 | 16384 | 256 | 33554432;
			sp->EffectSpellGroupRelation_high[0] = 64 | 2 | 1024;
		}
		//Priest - Wand Specialization
		sp = dbcSpell.LookupEntryForced( 14524 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectMiscValue[0] = SMT_SPELL_VALUE;
			sp->EffectSpellGroupRelation[0] = 134217728;
		}
		sp = dbcSpell.LookupEntryForced( 14525 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectMiscValue[0] = SMT_SPELL_VALUE;
			sp->EffectSpellGroupRelation[0] = 134217728;
		}
		sp = dbcSpell.LookupEntryForced( 14526 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectMiscValue[0] = SMT_SPELL_VALUE;
			sp->EffectSpellGroupRelation[0] = 134217728;
		}
		sp = dbcSpell.LookupEntryForced( 14527 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectMiscValue[0] = SMT_SPELL_VALUE;
			sp->EffectSpellGroupRelation[0] = 134217728;
		}
		sp = dbcSpell.LookupEntryForced( 14528 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectMiscValue[0] = SMT_SPELL_VALUE;
			sp->EffectSpellGroupRelation[0] = 134217728;
		}
		//Priest: Shadow Power
		sp = dbcSpell.LookupEntryForced( 15310 );
		if( sp != NULL )
		{
			uint32 group = sp->EffectSpellGroupRelation[0];
			sp = dbcSpell.LookupEntryForced( 33221 );
			if( sp != NULL )
				sp->EffectSpellGroupRelation[0] = group;
			sp = dbcSpell.LookupEntryForced( 33222 );
			if( sp != NULL )
				sp->EffectSpellGroupRelation[0] = group;
			sp = dbcSpell.LookupEntryForced( 33223 );
			if( sp != NULL )
				sp->EffectSpellGroupRelation[0] = group;
			sp = dbcSpell.LookupEntryForced( 33224 );
			if( sp != NULL )
				sp->EffectSpellGroupRelation[0] = group;
			sp = dbcSpell.LookupEntryForced( 33225 );
			if( sp != NULL )
				sp->EffectSpellGroupRelation[0] = group;
		}

	#ifdef NEW_PROCFLAGS
		//priest -  Shadow Weaving
		if (sp != NULL)
		{
			uint32 group = sp->EffectSpellGroupRelation[0];
			sp = dbcSpell.LookupEntryForced(15334);
			if (sp !=NULL)
				sp->EffectSpellGroupRelation[0] = group;
			sp = dbcSpell.LookupEntryForced(15333);
			if (sp !=NULL)
				sp->EffectSpellGroupRelation[0] = group;
			sp = dbcSpell.LookupEntryForced(15332);
			if (sp !=NULL)
				sp->EffectSpellGroupRelation[0] = group;
			sp = dbcSpell.LookupEntryForced(15331);
			if (sp !=NULL)
				sp->EffectSpellGroupRelation[0] = group;
			sp = dbcSpell.LookupEntryForced(15257);
			if (sp !=NULL)
				sp->EffectSpellGroupRelation[0] = group;
		}
	#endif

		//Priest - Inspiration proc spell
		sp = dbcSpell.LookupEntryForced( 14893 );
		if( sp != NULL )
			sp->rangeIndex = 4;
		sp = dbcSpell.LookupEntryForced( 15357 );
		if( sp != NULL )
			sp->rangeIndex = 4;
		sp = dbcSpell.LookupEntryForced( 15359 );
		if( sp != NULL )
			sp->rangeIndex = 4;

		//priest - surge of light
		sp = dbcSpell.LookupEntryForced( 33150 );
		if( sp != NULL )
			sp->procFlags = uint32(PROC_ON_SPELL_CRIT_HIT_VICTIM | PROC_TARGET_SELF);
		sp = dbcSpell.LookupEntryForced( 33154 );
		if( sp != NULL )
			sp->procFlags = uint32(PROC_ON_SPELL_CRIT_HIT_VICTIM | PROC_TARGET_SELF);
		sp = dbcSpell.LookupEntryForced( 33151 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 128;
			sp->EffectSpellGroupRelation[1] = 128;
			sp->EffectSpellGroupRelation[2] = 128;
			sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL;
		}
		// priest - Reflective Shield
		sp = dbcSpell.LookupEntryForced( 33201 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_ABSORB;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 33619; //!! WRONG spell, we will make direct dmg here
		}
		sp = dbcSpell.LookupEntryForced( 33202 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_ABSORB;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 33619; //!! WRONG spell, we will make direct dmg here
		}
		sp = dbcSpell.LookupEntryForced( 33203 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_ABSORB;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 33619; //!! WRONG spell, we will make direct dmg here
		}
		sp = dbcSpell.LookupEntryForced( 33204 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_ABSORB;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 33619; //!! WRONG spell, we will make direct dmg here
		}
		sp = dbcSpell.LookupEntryForced( 33205 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_ABSORB;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 33619; //!! WRONG spell, we will make direct dmg here
		}

	//////////////////////////////////////////
	// SHAMAN								//
	//////////////////////////////////////////

	// Insert shaman spell fixes here

		/**********************************************************
		 *	Shamanistic Rage
		 **********************************************************/
		SpellEntry*  parentsp = dbcSpell.LookupEntryForced( 30823 );
		SpellEntry* triggersp = dbcSpell.LookupEntryForced( 30824 );
		if( parentsp != NULL && triggersp != NULL )
			triggersp->EffectBasePoints[0] = parentsp->EffectBasePoints[0];

		//summon only 1 elemental totem
		sp = dbcSpell.LookupEntryForced( 2894 );
		if( sp != NULL && sp->Id == 2894 )
			sp->EffectImplicitTargetA[0] = 0; //remove this targeting. it is enough to get 1 target

		//summon only 1 elemental totem
		sp = dbcSpell.LookupEntryForced( 2062 );
		if( sp != NULL && sp->Id == 2062 )
			sp->EffectImplicitTargetA[0] = 0; //remove this targeting. it is enough to get 1 target

		/**********************************************************
		 *	Elemental Focus
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16164 );
		if( sp != NULL && sp->Id == 16164 )
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;

		/**********************************************************
		 *	Stormstrike
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 17364 );
		if( sp != NULL && sp->Id == 17364 )
			sp->Effect[0] = 0;

		/**********************************************************
		 *	Bloodlust
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 2825 );
		if( sp != NULL )
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_ALL_PARTY;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_ALL_PARTY;
			sp->EffectImplicitTargetA[2] = 0;
			sp->EffectImplicitTargetB[0] = 0;
			sp->EffectImplicitTargetB[1] = 0;
			sp->EffectImplicitTargetB[2] = 0;
		}

		/**********************************************************
		 *	Heroism
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 32182 );
		if( sp != NULL )
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_ALL_PARTY;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_ALL_PARTY;
			sp->EffectImplicitTargetA[2] = 0;
			sp->EffectImplicitTargetB[0] = 0;
			sp->EffectImplicitTargetB[1] = 0;
			sp->EffectImplicitTargetB[2] = 0;
		}
		/**********************************************************
		 *	Lightning Mastery
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16578 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0]=1|2;
		sp = dbcSpell.LookupEntryForced( 16579 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0]=1|2;
		sp = dbcSpell.LookupEntryForced( 16580 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0]=1|2;
		sp = dbcSpell.LookupEntryForced( 16581 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0]=1|2;
		sp = dbcSpell.LookupEntryForced( 16582 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0]=1|2;
		/**********************************************************
		 *	Lightning Overload 
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 30675 ); 
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 39805;//proc something (we will owerride this)
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 30678 ); 
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 39805;//proc something (we will owerride this)
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 30679 ); 
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 39805;//proc something (we will owerride this)
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 30680 ); 
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 39805;//proc something (we will owerride this)
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 30681 ); 
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 39805;//proc something (we will owerride this)
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		/**********************************************************
		 *	Purge 
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 370 ); 
		if( sp != NULL )
			sp->DispelType = DISPEL_MAGIC;
		sp = dbcSpell.LookupEntryForced( 8012 ); 
		if( sp != NULL )
			sp->DispelType = DISPEL_MAGIC;
		sp = dbcSpell.LookupEntryForced( 27626 ); 
		if( sp != NULL )
			sp->DispelType = DISPEL_MAGIC;
		sp = dbcSpell.LookupEntryForced( 33625 ); 
		if( sp != NULL )
			sp->DispelType = DISPEL_MAGIC;

		/**********************************************************
		 *	Elemental mastery
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16166 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 0xFFFFFFFF;//nature+fire+frost is all that shaman can do
			sp->EffectSpellGroupRelation_high[0] = 0xFFFFFFFF;//nature+fire+frost is all that shaman can do
			sp->EffectSpellGroupRelation[1] = 0xFFFFFFFF;//nature+fire+frost is all that shaman can do
			sp->EffectSpellGroupRelation_high[1] = 0xFFFFFFFF;//nature+fire+frost is all that shaman can do
		}
		/**********************************************************
		 *	Eye of the Storm
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 29062 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM;
		sp = dbcSpell.LookupEntryForced( 29064 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM;
		sp = dbcSpell.LookupEntryForced( 29065 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM;

		/**********************************************************
		 *	Shamanistic Focus
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 43338 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->EffectTriggerSpell[0] = 43339;
			sp->procChance = 100;
			sp->maxstack = 1;
		}

		/**********************************************************
		 *	focused
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 43339 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectMiscValue[0] = SMT_COST;
			sp->EffectSpellGroupRelation[0] = 1048576UL | 268435456UL | 2147483648UL;
		}

		/**********************************************************
		 *	Focused Casting
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 29063 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] =  0xFFFFFFFF; // shaman spells. Guess that wraps them all 
			sp->EffectSpellGroupRelation_high[0] =  0xFFFFFFFF; // shaman spells. Guess that wraps them all 
		}

		/**********************************************************
		 *	Healing Focus
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16181 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] =  64 | 128 | 256;
		sp = dbcSpell.LookupEntryForced( 16230 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] =  64 | 128 | 256;
		sp = dbcSpell.LookupEntryForced( 16232 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] =  64 | 128 | 256;
		sp = dbcSpell.LookupEntryForced( 16233 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] =  64 | 128 | 256;
		sp = dbcSpell.LookupEntryForced( 16234 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] =  64 | 128 | 256;

		/**********************************************************
		 *	Improved Lightning shield 
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16261 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1024;
		sp = dbcSpell.LookupEntryForced( 16290 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1024;
		sp = dbcSpell.LookupEntryForced( 16291 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1024;
		/**********************************************************
		 *	Tidal focus
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16179 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 64 | 128 | 256;
		sp = dbcSpell.LookupEntryForced( 16214 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 64 | 128 | 256;
		sp = dbcSpell.LookupEntryForced( 16215 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 64 | 128 | 256;
		sp = dbcSpell.LookupEntryForced( 16216 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 64 | 128 | 256;
		sp = dbcSpell.LookupEntryForced( 16217 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 64 | 128 | 256;

		/**********************************************************
		 *	Enhancing Totems
		 **********************************************************/
		group_relation_shaman_enhancing_totems = 0x00010000 | 0x00020000; // <--- GROUPING
		sp = dbcSpell.LookupEntryForced( 16259 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_enhancing_totems;
		sp = dbcSpell.LookupEntryForced( 16295 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_enhancing_totems;

		/**********************************************************
		 *	Elemental Fury - ! Not finished !
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16089 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 0xFFFFFFFF ; //damn, what other spells do there remain after that list ? Maybe later :P
			//sp->EffectSpellGroupRelation[0] = 1073741824 | 32 | 1048576 | 1 | ... ; //Searing/Magma/Fire Nova Totem effects and Fire,Frost,Nature spells
			sp->EffectSpellGroupRelation_high[0] = 0xFFFFFFFF ; //damn, what other spells do there remain after that list ? Maybe later :P
		}

		/**********************************************************
		 *	Restorative Totems
		 **********************************************************/
		group_relation_shaman_restorative_totems = 0x00004000 | 0x00002000; // <--- GROUPING
		sp = dbcSpell.LookupEntryForced( 16259 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_restorative_totems;
		sp = dbcSpell.LookupEntryForced( 16205 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_restorative_totems;
		sp = dbcSpell.LookupEntryForced( 16206 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_restorative_totems;
		sp = dbcSpell.LookupEntryForced( 16207 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_restorative_totems;
		sp = dbcSpell.LookupEntryForced( 16208 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_restorative_totems;

		/**********************************************************
		 *	Healing Way
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 29202 ); 
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 29205 ); 
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 29206 ); 
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}

		/**********************************************************
		 *	Elemental Devastation
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 29179 ); 
		if( sp != NULL )
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
		sp = dbcSpell.LookupEntryForced( 29180 ); 
		if( sp != NULL )
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
		sp = dbcSpell.LookupEntryForced( 30160 ); 
		if( sp != NULL )
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;

		/**********************************************************
		 *	Ancestral healing
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16176 ); 
		if( sp != NULL )
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
		sp = dbcSpell.LookupEntryForced( 16235 ); 
		if( sp != NULL )
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
		sp = dbcSpell.LookupEntryForced( 16240 ); 
		if( sp != NULL )
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;

		/**********************************************************
		 *	Ancestral healing proc spell
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16177 );
		if( sp != NULL )
			sp->rangeIndex = 4;
		sp = dbcSpell.LookupEntryForced( 16236 );
		if( sp != NULL )
			sp->rangeIndex = 4;
		sp = dbcSpell.LookupEntryForced( 16237 );
		if( sp != NULL )
			sp->rangeIndex = 4;

		/**********************************************************
		 *	Mental Quickness
		 **********************************************************/
		group_relation_shaman_mental_quickness = 0x00000008 | 0x00000010 | 0x00000200 | 0x00000400 | 0x00080000 | 0x00100000 | 0x00400000 | 0x20000000 | 0x10000000 | 0x80000000;
		sp = dbcSpell.LookupEntryForced( 30812 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_mental_quickness;
		sp = dbcSpell.LookupEntryForced( 30813 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_mental_quickness;
		sp = dbcSpell.LookupEntryForced( 30814 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_mental_quickness;

		/**********************************************************
		 *	Totems grouping
		 **********************************************************/
		group_relation_shaman_totems |= 0x00000008 | 0x00000010 | 0x00001000 | 0x00080000 | 0x20000000;
		
		/**********************************************************
		 *	Totemic focus
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16173 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_totems;
		sp = dbcSpell.LookupEntryForced( 16222 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_totems;
		sp = dbcSpell.LookupEntryForced( 16223 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_totems;
		sp = dbcSpell.LookupEntryForced( 16224 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_totems;
		sp = dbcSpell.LookupEntryForced( 16225 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_totems;
		
		/********************************************************
		Call of Flame
		*******************************/
		sp = dbcSpell.LookupEntryForced( 16038 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1073741824;    
		sp = dbcSpell.LookupEntryForced( 16160 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1073741824;
		sp = dbcSpell.LookupEntryForced( 16161 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1073741824;

		/**********************************************************
		 *	Lightning - Grouping
		 **********************************************************/
		group_relation_shaman_lightning = 0x00000001 | 0x00000002;
		
		/**********************************************************
		 *	Call of Thunder
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16041 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16117 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16118 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16119 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16120 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_lightning;
		
		/**********************************************************
		 *	Shock Grouping
		 **********************************************************/
		group_relation_shaman_shock = 0x00100000 | 0x10000000 | 0x80000000;

		/**********************************************************
		 *	Convection
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16039 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16109 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16110 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16111 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16112 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;

		/**********************************************************
		 *	Concussion
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16035 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16105 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16106 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16107 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16108 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
		
		// Shaman - Storm Reach
		sp = dbcSpell.LookupEntryForced( 28999 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 2 | 1;
		sp = dbcSpell.LookupEntryForced( 29000 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 2 | 1;

		//wrath of air totem targets sorounding creatures instead of us
		sp = dbcSpell.LookupEntryForced( 2895 );
		if( sp != NULL )
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_SELF;
			sp->EffectImplicitTargetA[2] = 0;
			sp->EffectImplicitTargetB[0] = 0;
			sp->EffectImplicitTargetB[1] = 0;
			sp->EffectImplicitTargetB[2] = 0;
		}

		sp = dbcSpell.LookupEntryForced( 20608 ); //Reincarnation
		if( sp != NULL )
		{
			for(uint32 i=0;i<8;i++)
			{
				if(sp->Reagent[i])
				{
					sp->Reagent[i] = 0;
					sp->ReagentCount[i] = 0;
				}
			}
		}

	//////////////////////////////////////////
	// MAGE								//
	//////////////////////////////////////////

	// Insert mage spell fixes here

		/**********************************************************
		 *	Arcane Power
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 12042 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1 | 8192 | 4194304 | 8388608 | 262144 | 131072 | 536870912 | 524352 | 4 | 4096 | 2 | 2048 | 16;
			sp->EffectSpellGroupRelation[1] = 1 | 8192 | 4194304 | 8388608 | 262144 | 131072 | 536870912 | 524352 | 4 | 4096 | 2 | 2048 | 16;
			sp->EffectSpellGroupRelation[2] = 1 | 8192 | 4194304 | 8388608 | 262144 | 131072 | 536870912 | 524352 | 4 | 4096 | 2 | 2048 | 16;
		}

		/**********************************************************
		 *	Arcane Concentration
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 11213 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_SPELL_HIT | PROC_TARGET_SELF;
		sp = dbcSpell.LookupEntryForced( 12574 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_SPELL_HIT | PROC_TARGET_SELF;
		sp = dbcSpell.LookupEntryForced( 12575 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_SPELL_HIT | PROC_TARGET_SELF;
		sp = dbcSpell.LookupEntryForced( 12576 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_SPELL_HIT | PROC_TARGET_SELF;
		sp = dbcSpell.LookupEntryForced( 12577 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_SPELL_HIT | PROC_TARGET_SELF;

		//Mage - Icy Veins
		sp = dbcSpell.LookupEntryForced( 12472 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation_high[1] = 512;
			sp->EffectMiscValue[1] = SMT_TRIGGER;
		}
		//Mage - Wand Specialization. Not the forst thing we messed up. Blizz uses attack as magic and wandds as weapons :S
		sp = dbcSpell.LookupEntryForced( 6057 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectMiscValue[0] = SMT_SPELL_VALUE;
			sp->EffectSpellGroupRelation[0] = 134217728;
		}
		sp = dbcSpell.LookupEntryForced( 6085 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectMiscValue[0] = SMT_SPELL_VALUE;
			sp->EffectSpellGroupRelation[0] = 134217728;
		}
		//Mage - Spell Power
		sp = dbcSpell.LookupEntryForced( 35578 );
		if( sp != NULL )
		{
			sp->EffectMiscValue[0] = SMT_CRITICAL_DAMAGE;
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectSpellGroupRelation[0] = 524288 | 131072 | 0 | 1 | 2 | 4 | 22 | 12 | 29 | 11;
			//sp->EffectSpellGroupRelation_high[0] = 0xFFFFFFFF;
		}
		sp = dbcSpell.LookupEntryForced( 35581 );
		if( sp != NULL )
		{
			sp->EffectMiscValue[0] = SMT_CRITICAL_DAMAGE;
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectSpellGroupRelation[0] = 524288 | 131072 | 0 | 1 | 2 | 4 | 22 | 12 | 29 | 11;
			//sp->EffectSpellGroupRelation_high[0] = 0xFFFFFFFF;
		}

		//Mage - Frost Channeling
		sp = dbcSpell.LookupEntryForced( 11160 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 524288 | 131072  ;
			sp->EffectSpellGroupRelation_high[0] = 1;
		}
		sp = dbcSpell.LookupEntryForced( 12518 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 524288 | 131072  ;
			sp->EffectSpellGroupRelation_high[0] = 1;
		}
		sp = dbcSpell.LookupEntryForced( 12519 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 524288 | 131072  ;
			sp->EffectSpellGroupRelation_high[0] = 1;
		}

		//Mage - Elemental Precision
		sp = dbcSpell.LookupEntryForced( 29438 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectSpellGroupRelation[0] = 8388608 | 2 | 16 | 4 | 1573376 | 524288 | 8 | 131072 | 262144 | 4194304 | 1 ;
			sp->EffectSpellGroupRelation_high[0] = 64 | 128 | 1;
			sp->EffectMiscValue[0] = SMT_COST;
		}
		sp = dbcSpell.LookupEntryForced( 29439 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectSpellGroupRelation[0] = 8388608 | 2 | 16 | 4 | 1573376 | 524288 | 8 | 131072 | 262144 | 4194304 | 1 ;
			sp->EffectSpellGroupRelation_high[0] = 64 | 128 | 1;
			sp->EffectMiscValue[0] = SMT_COST;
		}
		sp = dbcSpell.LookupEntryForced( 29440 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectSpellGroupRelation[0] = 8388608 | 2 | 16 | 4 | 1573376 | 524288 | 8 | 131072 | 262144 | 4194304 | 1 ;
			sp->EffectSpellGroupRelation_high[0] = 64 | 128 | 1;
			sp->EffectMiscValue[0] = SMT_COST;
		}

		//Mage - Arcane Blast
		sp = dbcSpell.LookupEntryForced( 30451 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1] = 36032;
			sp->procFlags = PROC_ON_CAST_SPECIFIC_SPELL;
		}

		//Mage - Magic Attunement
		sp = dbcSpell.LookupEntryForced( 11247 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 8192;
		sp = dbcSpell.LookupEntryForced( 12606 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 8192; //strange lvl 2 was working 

		//Mage - Arcane Blast proc spell
		sp = dbcSpell.LookupEntryForced( 36032 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 536870912;
			sp->EffectSpellGroupRelation[1] = 536870912;
		}

		//mage : Improved Blink
		sp = dbcSpell.LookupEntryForced( 31569 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 65536;
		sp = dbcSpell.LookupEntryForced( 31570 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 65536;

		//mage : Empowered Arcane Missiles
		sp = dbcSpell.LookupEntryForced( 31579 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 2097152;
			sp->EffectBasePoints[0] *= 5; //heh B thinks he is smart by adding this to description ? If it doesn;t work std then it still needs to made by hand
			sp->EffectSpellGroupRelation[1] = 2048;
		}
		sp = dbcSpell.LookupEntryForced( 31582 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 2097152;
			sp->EffectBasePoints[0] *= 5; //heh B thinks he is smart by adding this to description ? If it doesn;t work std then it still needs to made by hand
			sp->EffectSpellGroupRelation[1] = 2048;
		}
		sp = dbcSpell.LookupEntryForced( 31583 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 2097152; //damage
			sp->EffectBasePoints[0] *= 5; //heh B thinks he is smart by adding this to description ? If it doesn;t work std then it still needs to made by hand
			sp->EffectSpellGroupRelation[1] = 2048; //cost
		}

		//mage : Empowered Fireball
		sp = dbcSpell.LookupEntryForced( 31656 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1;
		sp = dbcSpell.LookupEntryForced( 31657 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1;
		sp = dbcSpell.LookupEntryForced( 31658 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1;
		sp = dbcSpell.LookupEntryForced( 31659 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1;
		sp = dbcSpell.LookupEntryForced( 31660 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1;

		//mage : Ice Floes
		sp = dbcSpell.LookupEntryForced( 31670 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 512;
			sp->EffectSpellGroupRelation_high[0] = 4 | 1;
		}
		sp = dbcSpell.LookupEntryForced( 31672 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 512;
			sp->EffectSpellGroupRelation_high[0] = 4 | 1;
		}

		//mage : Empowered Frostbolt
		sp = dbcSpell.LookupEntryForced( 31682 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 32;
			sp->EffectSpellGroupRelation[1] = 32;
		}
		sp = dbcSpell.LookupEntryForced( 31683 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 32;
			sp->EffectSpellGroupRelation[1] = 32;
		}
		sp = dbcSpell.LookupEntryForced( 31684 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 32;
			sp->EffectSpellGroupRelation[1] = 32;
		}
		sp = dbcSpell.LookupEntryForced( 31685 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 32;
			sp->EffectSpellGroupRelation[1] = 32;
		}
		sp = dbcSpell.LookupEntryForced( 31686 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 32;
			sp->EffectSpellGroupRelation[1] = 32;
		}

		//Mage - Ice Shards
		sp = dbcSpell.LookupEntryForced( 11207 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] =  524288 | 131072;
		sp = dbcSpell.LookupEntryForced( 12672 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] =  524288 | 131072;
		sp = dbcSpell.LookupEntryForced( 15047 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] =  524288 | 131072;
		sp = dbcSpell.LookupEntryForced( 15052 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] =  524288 | 131072;
		sp = dbcSpell.LookupEntryForced( 15053 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] =  524288 | 131072;

		//Mage - Improved Blizzard
		sp = dbcSpell.LookupEntryForced( 11185 );
		if( sp != NULL )
		{    
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 12484;
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 12487 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 12485;
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 12488 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 12486;
			sp->procFlags = PROC_ON_CAST_SPELL;
		}

		//mage: Fire Power
		sp = dbcSpell.LookupEntryForced( 11124 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 8388608 | 16 | 2 | 4 | 8 | 262144 | 4194304 | 1;
			sp->EffectSpellGroupRelation[1] = 8388608 | 16 | 2 | 4 | 8 | 262144 | 4194304 | 1;
		}
		sp = dbcSpell.LookupEntryForced( 12398 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 8388608 | 16 | 2 | 4 | 8 | 262144 | 4194304 | 1;
			sp->EffectSpellGroupRelation[1] = 8388608 | 16 | 2 | 4 | 8 | 262144 | 4194304 | 1;
		}
		sp = dbcSpell.LookupEntryForced( 12399 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 8388608 | 16 | 2 | 4 | 8 | 262144 | 4194304 | 1;
			sp->EffectSpellGroupRelation[1] = 8388608 | 16 | 2 | 4 | 8 | 262144 | 4194304 | 1;
		}
		sp = dbcSpell.LookupEntryForced( 12400 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 8388608 | 16 | 2 | 4 | 8 | 262144 | 4194304 | 1;
			sp->EffectSpellGroupRelation[1] = 8388608 | 16 | 2 | 4 | 8 | 262144 | 4194304 | 1;
		}
		sp = dbcSpell.LookupEntryForced( 12378 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 8388608 | 16 | 2 | 4 | 8 | 262144 | 4194304 | 1;
			sp->EffectSpellGroupRelation[1] = 8388608 | 16 | 2 | 4 | 8 | 262144 | 4194304 | 1;
		}
		
		//mage - Burning Soul
		sp = dbcSpell.LookupEntryForced( 11083 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 8388608 | 16 | 2 | 4 | 8 | 262144 | 4194304 | 1;
		sp = dbcSpell.LookupEntryForced( 12351 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 8388608 | 16 | 2 | 4 | 8 | 262144 | 4194304 | 1;

		//mage - Combustion
		sp = dbcSpell.LookupEntryForced( 11129 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL | PROC_ON_SPELL_CRIT_HIT | PROC_TARGET_SELF;
			sp->procCharges = 0;
			sp->c_is_flags |= SPELL_FLAG_IS_REQUIRECOOLDOWNUPDATE;
		}
		sp = dbcSpell.LookupEntryForced( 28682 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 8388608 | 16 | 2 | 4 | 4194304 | 1;

		//mage - Empowered Fireball
		sp = dbcSpell.LookupEntryForced( 31656 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1;
		sp = dbcSpell.LookupEntryForced( 31657 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1;
		sp = dbcSpell.LookupEntryForced( 31658 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1;
		sp = dbcSpell.LookupEntryForced( 31659 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1;
		sp = dbcSpell.LookupEntryForced( 31660 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1;

		//mage - Empowered Frostbolt
		sp = dbcSpell.LookupEntryForced( 31682 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 32;
			sp->EffectSpellGroupRelation[1] = 32;
		}
		sp = dbcSpell.LookupEntryForced( 31683 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 32;
			sp->EffectSpellGroupRelation[1] = 32;
		}
		sp = dbcSpell.LookupEntryForced( 31684 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 32;
			sp->EffectSpellGroupRelation[1] = 32;
		}
		sp = dbcSpell.LookupEntryForced( 31685 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 32;
			sp->EffectSpellGroupRelation[1] = 32;
		}
		sp = dbcSpell.LookupEntryForced( 31686 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 32;
			sp->EffectSpellGroupRelation[1] = 32;
		}

		//mage - Master of Elements
		sp = dbcSpell.LookupEntryForced( 29074 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 29077;
			sp->procFlags = uint32(PROC_ON_SPELL_CRIT_HIT|PROC_TARGET_SELF);
			sp->procChance = 100;
		}
		sp = dbcSpell.LookupEntryForced( 29075 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 29077;
			sp->procFlags = uint32(PROC_ON_SPELL_CRIT_HIT|PROC_TARGET_SELF);
			sp->procChance = 100;
		}
		sp = dbcSpell.LookupEntryForced( 29076 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 29077;
			sp->procFlags = uint32(PROC_ON_SPELL_CRIT_HIT|PROC_TARGET_SELF);
			sp->procChance = 100;
		}

		//mage: Blazing Speed
		sp = dbcSpell.LookupEntryForced( 31641 ); 
		if( sp != NULL )
			sp->EffectTriggerSpell[0] = 31643;
		sp = dbcSpell.LookupEntryForced( 31642 );
		if( sp != NULL )
			sp->EffectTriggerSpell[0] = 31643;

		// Mage: Cold Snap ( set spell group relation for second effect, "gives you 100% chance to avoid interruption caused by damage while casting."
		sp = dbcSpell.LookupEntryForced( 12472 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[1] = 0xFFFFFFFF; // we can be sure this won't fuck up like Spell Power did, since it's 100%, not additive :P

		//mage talent "frostbyte". we make it to be dummy
		sp = dbcSpell.LookupEntryForced( 11071 );
		if( sp != NULL )
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
		sp = dbcSpell.LookupEntryForced( 12496 );
		if( sp != NULL )
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
		sp = dbcSpell.LookupEntryForced( 12497 );
		if( sp != NULL )
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
		//Mage - Improved Scorch
		sp = dbcSpell.LookupEntryForced( 11095 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance =33;
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 12872 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL; 
			sp->procChance =66;
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 12873 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance =100;
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		//mage - Presence of Mind
		sp = dbcSpell.LookupEntryForced(12043);
		if (sp!= NULL)
		{
			sp->EffectSpellGroupRelation[0]= 0xFFFFFFFF;
			sp->EffectSpellGroupRelation_high[0]= 0xFFFFFFFF;
		}

		// mage - Frost Warding
		sp = dbcSpell.LookupEntryForced( 11189 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 33554432;
		sp = dbcSpell.LookupEntryForced( 28332 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 33554432;

		// mage - Conjure Refreshment Table
		sp = dbcSpell.LookupEntryForced( 43985 );
		if ( sp != NULL )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_DYNAMIC_OBJECT;

	//////////////////////////////////////////
	// WARLOCK								//
	//////////////////////////////////////////

	// Insert warlock spell fixes here

		/**********************************************************
		 *	Nether Protection
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 30299 );
		if (sp != NULL)
		{
			sp->procChance = 10;
			sp->proc_interval = 13000;
		}
		sp = dbcSpell.LookupEntryForced( 30301 );
		if (sp != NULL)
		{
			sp->procChance = 20;
			sp->proc_interval = 13000;
		}
		sp = dbcSpell.LookupEntryForced( 30302 );
		if (sp != NULL)
		{
			sp->procChance = 30;
			sp->proc_interval = 13000;
		}
		/**********************************************************
		 *	Backlash
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 34935 );
		if (sp != NULL)
		{
			sp->proc_interval = 8000;
			sp->procFlags |= PROC_ON_MELEE_ATTACK_VICTIM | PROC_TARGET_SELF;
		}
		sp = dbcSpell.LookupEntryForced( 34938 );
		if (sp != NULL)
		{
			sp->proc_interval = 8000;
			sp->procFlags |= PROC_ON_MELEE_ATTACK_VICTIM | PROC_TARGET_SELF;
		}
		sp = dbcSpell.LookupEntryForced( 34939 );
		if (sp != NULL)
		{
			sp->proc_interval = 8000;
			sp->procFlags |= PROC_ON_MELEE_ATTACK_VICTIM | PROC_TARGET_SELF;
		}
		sp = dbcSpell.LookupEntryForced( 34936 );
		if (sp != NULL)
		{
			sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL;
			sp->EffectSpellGroupRelation[0] = 1 ;
			sp->EffectSpellGroupRelation_high[0] = 64;
		}

		/**********************************************************
		 *	Demonic Knowledge
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 35691 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_DAMAGE_DONE;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
			sp->Effect[1] = 6;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_DONE;
			sp->EffectBasePoints[1] = sp->EffectBasePoints[0];
			sp->EffectImplicitTargetA[1]= EFF_TARGET_PET;
			sp->Effect[2] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[2] = 35696;
			sp->EffectImplicitTargetA[2]=EFF_TARGET_PET;
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER | SPELL_FLAG_IS_EXPIREING_WITH_PET;
		}
		sp = dbcSpell.LookupEntryForced( 35692 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_DAMAGE_DONE;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
			sp->Effect[1] = 6;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_DONE;
			sp->EffectBasePoints[1] = sp->EffectBasePoints[0];
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			sp->Effect[2] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[2] = 35696;
			sp->EffectImplicitTargetA[2] = EFF_TARGET_PET;
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER | SPELL_FLAG_IS_EXPIREING_WITH_PET;
		}
		sp = dbcSpell.LookupEntryForced( 35693 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_DAMAGE_DONE;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
			sp->Effect[1] = 6;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_DONE;
			sp->EffectBasePoints[1] = sp->EffectBasePoints[0];
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			sp->Effect[2] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[2] = 35696;
			sp->EffectImplicitTargetA[2] = EFF_TARGET_PET;
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER | SPELL_FLAG_IS_EXPIREING_WITH_PET;
		}
		sp = dbcSpell.LookupEntryForced( 35696 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6; //making this only for the visible effect
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY; //no effect here
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}
		//warlock -  Seed of Corruption
		sp = dbcSpell.LookupEntryForced( 27243 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1] = 27285;
			sp->procFlags = PROC_ON_SPELL_HIT_VICTIM | PROC_ON_DIE;
			sp->procChance = 100;
		}

		//warlock -  soul link
		sp = dbcSpell.LookupEntryForced( 19028 );
		if( sp != NULL )
		{
			//this is for the trigger effect
			sp->Effect[0]=SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_SPLIT_DAMAGE;
			sp->EffectMiscValue[0]=20;
			//sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			//this is for the extra 5% dmg for caster and pet
			sp->Effect[1] = 6;
			sp->EffectApplyAuraName[1] = 79;
			sp->EffectBasePoints[1] = 4; //4+1=5
			sp->EffectImplicitTargetA[1] = EFF_TARGET_SELF;
			sp->EffectImplicitTargetB[1] = EFF_TARGET_PET;
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
		}

		//warlock: Demonic Aegis
		sp = dbcSpell.LookupEntryForced( 30143 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation_high[0] = 32;
		sp = dbcSpell.LookupEntryForced( 30144 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation_high[0] = 32;
		sp = dbcSpell.LookupEntryForced( 30145 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation_high[0] = 32;

		//warlock: Nightfall
		sp = dbcSpell.LookupEntryForced( 18094 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 17941;
			sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION | PROC_TARGET_SELF;
			sp->procChance = 2;
		}
		sp = dbcSpell.LookupEntryForced( 18095 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 17941;
			sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION | PROC_TARGET_SELF;
			sp->procChance = 4;
		}
		//Shadow Trance should be removed on the first SB
		sp = dbcSpell.LookupEntryForced( 17941 );
		if( sp != NULL )
		{
			sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL;
			sp->EffectSpellGroupRelation[0] = 1;
		}
		//Fel Concentration
		sp = dbcSpell.LookupEntryForced( 17783 );
		if( sp != NULL)
		{
			sp->EffectSpellGroupRelation[0] = 8 | 16 | 16384;
		}
		sp = dbcSpell.LookupEntryForced( 17784 );
		if( sp != NULL)
		{
			sp->EffectSpellGroupRelation[0] = 8 | 16 | 16384;
		}
		sp = dbcSpell.LookupEntryForced( 17785 );
		if( sp != NULL)
		{
			sp->EffectSpellGroupRelation[0] = 8 | 16 | 16384;
		}
		sp = dbcSpell.LookupEntryForced( 17786 );
		if( sp != NULL)
		{
			sp->EffectSpellGroupRelation[0] = 8 | 16 | 16384;
		}
		sp = dbcSpell.LookupEntryForced( 17787 );
		if( sp != NULL)
		{
			sp->EffectSpellGroupRelation[0] = 8 | 16 | 16384;
		}
		//warlock: Contagion
		sp = dbcSpell.LookupEntryForced( 30060 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1024 | 2;
			sp->EffectSpellGroupRelation_high[0] = 32784;
			sp->EffectSpellGroupRelation[1] = 1024 | 2;
			sp->EffectSpellGroupRelation_high[1] = 32784;
			sp->EffectSpellGroupRelation[2] = 2 | 8 | 32768 | 2147483648UL | 1024 | 16384 | 262144UL | 16 | 524288UL | 4194304UL;
		}
		sp = dbcSpell.LookupEntryForced( 30061 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1024 | 2;
			sp->EffectSpellGroupRelation_high[0] = 32784;
			sp->EffectSpellGroupRelation[1] = 1024 | 2;
			sp->EffectSpellGroupRelation_high[1] = 32784;
			sp->EffectSpellGroupRelation[2] = 2 | 8 | 32768 | 2147483648UL | 1024 | 16384 | 262144UL | 16 | 524288UL | 4194304UL;
		}
		sp = dbcSpell.LookupEntryForced( 30062 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1024 | 2;
			sp->EffectSpellGroupRelation_high[0] = 32784;
			sp->EffectSpellGroupRelation[1] = 1024 | 2;
			sp->EffectSpellGroupRelation_high[1] = 32784;
			sp->EffectSpellGroupRelation[2] = 2 | 8 | 32768 | 2147483648UL | 1024 | 16384 | 262144UL | 16 | 524288UL | 4194304UL;
		}
		sp = dbcSpell.LookupEntryForced( 30063 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1024 | 2;
			sp->EffectSpellGroupRelation_high[0] = 32784;
			sp->EffectSpellGroupRelation[1] = 1024 | 2;
			sp->EffectSpellGroupRelation_high[1] = 32784;
			sp->EffectSpellGroupRelation[2] = 2 | 8 | 32768 | 2147483648UL | 1024 | 16384 | 262144UL | 16 | 524288UL | 4194304UL;
		}
		sp = dbcSpell.LookupEntryForced( 30064 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1024 | 2;
			sp->EffectSpellGroupRelation_high[0] = 32784;
			sp->EffectSpellGroupRelation[1] = 1024 | 2;
			sp->EffectSpellGroupRelation_high[1] = 32784;
			sp->EffectSpellGroupRelation[2] = 2 | 8 | 32768 | 2147483648UL | 1024 | 16384 | 262144UL | 16 | 524288UL | 4194304UL;
		}

		//warlock: Malediction
		sp = dbcSpell.LookupEntryForced( 32477 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 2097152;
			sp->EffectSpellGroupRelation_high[0] = 512;
		}
		sp = dbcSpell.LookupEntryForced( 32483 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 2097152;
			sp->EffectSpellGroupRelation_high[0] = 512;
		}
		sp = dbcSpell.LookupEntryForced( 32484 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 2097152;
			sp->EffectSpellGroupRelation_high[0] = 512;
		}

		//warlock: Improved Searing Pain
		sp = dbcSpell.LookupEntryForced( 17927 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 256;
		sp = dbcSpell.LookupEntryForced( 17929 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 256;
		sp = dbcSpell.LookupEntryForced( 17930 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 256;

		//warlock: Empowered Corruption
		sp = dbcSpell.LookupEntryForced( 32381 );
		if( sp != NULL )
		{
			sp->EffectBasePoints[0] *= 6;
			sp->EffectSpellGroupRelation[0] = 2;
		}
		sp = dbcSpell.LookupEntryForced( 32382 );
		if( sp != NULL )
		{
			sp->EffectBasePoints[0] *= 6;
			sp->EffectSpellGroupRelation[0] = 2;
		}
		sp = dbcSpell.LookupEntryForced( 32383 );
		if( sp != NULL )
		{
			sp->EffectBasePoints[0] *= 6;
			sp->EffectSpellGroupRelation[0] = 2;
		}

		//warlock: Improved Enslave Demon
		sp = dbcSpell.LookupEntryForced( 18821 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 2048;
			sp->EffectMiscValue[0]=SMT_SPELL_VALUE_PCT;
			sp->EffectBasePoints[0] = -(sp->EffectBasePoints[0]+2);
			//sp->EffectSpellGroupRelation[1] = 2048; //we do not handle this misc type yet anyway. Removed it just as a reminder
			sp->EffectSpellGroupRelation[2] = 2048;
		}
		sp = dbcSpell.LookupEntryForced( 18822 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 2048;
			sp->EffectSpellGroupRelation[1] = 2048;
			sp->EffectSpellGroupRelation[2] = 2048;
		}
		//warlock: Devastation
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 4 | 1 | 64 | 256 | 32 | 128 | 512; //destruction spells
			sp->EffectSpellGroupRelation_high[0] = 64 | 128 |4096;
		}
		sp = dbcSpell.LookupEntryForced( 18131 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 4 | 1 | 64 | 256 | 32 | 128 | 512;
			sp->EffectSpellGroupRelation_high[0] = 64 | 128 |4096;
		}
		sp = dbcSpell.LookupEntryForced( 18132 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 4 | 1 | 64 | 256 | 32 | 128 | 512;
			sp->EffectSpellGroupRelation_high[0] = 64 | 128 |4096;
		}
		sp = dbcSpell.LookupEntryForced( 18133 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 4 | 1 | 64 | 256 | 32 | 128 | 512;
			sp->EffectSpellGroupRelation_high[0] = 64 | 128 |4096;
		}
		sp = dbcSpell.LookupEntryForced( 18134 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 4 | 1 | 64 | 256 | 32 | 128 | 512;
			sp->EffectSpellGroupRelation_high[0] = 64 | 128 |4096;
		}

		//warlock - Shadow Mastery
		sp = dbcSpell.LookupEntryForced( 18271 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] =  2147483648UL | 4194304 | 1 | 2 | 16384 | 1024 | 8 | 262144 | 524288 | 2147483648UL | 16777216UL | 128 | 16 | 32768;
			sp->EffectSpellGroupRelation[1] =  2147483648UL | 4194304 | 1 | 2 | 16384 | 1024 | 8 | 262144 | 524288 | 2147483648UL | 16777216UL | 128 | 16 | 32768;
		}
		sp = dbcSpell.LookupEntryForced( 18272 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] =  2147483648UL | 4194304 | 1 | 2 | 16384 | 1024 | 8 | 262144 | 524288 | 2147483648UL | 16777216UL | 128 | 16 | 32768;
			sp->EffectSpellGroupRelation[1] =  2147483648UL | 4194304 | 1 | 2 | 16384 | 1024 | 8 | 262144 | 524288 | 2147483648UL | 16777216UL | 128 | 16 | 32768;
		}
		sp = dbcSpell.LookupEntryForced( 18273 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] =  2147483648UL | 4194304 | 1 | 2 | 16384 | 1024 | 8 | 262144 | 524288 | 2147483648UL | 16777216UL | 128 | 16 | 32768;
			sp->EffectSpellGroupRelation[1] =  2147483648UL | 4194304 | 1 | 2 | 16384 | 1024 | 8 | 262144 | 524288 | 2147483648UL | 16777216UL | 128 | 16 | 32768;
		}
		sp = dbcSpell.LookupEntryForced( 18274 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] =  2147483648UL | 4194304 | 1 | 2 | 16384 | 1024 | 8 | 262144 | 524288 | 2147483648UL | 16777216UL | 128 | 16 | 32768;
			sp->EffectSpellGroupRelation[1] =  2147483648UL | 4194304 | 1 | 2 | 16384 | 1024 | 8 | 262144 | 524288 | 2147483648UL | 16777216UL | 128 | 16 | 32768;
		}
		sp = dbcSpell.LookupEntryForced( 18275 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] =  2147483648UL | 4194304 | 1 | 2 | 16384 | 1024 | 8 | 262144 | 524288 | 2147483648UL | 16777216UL | 128 | 16 | 32768;
			sp->EffectSpellGroupRelation[1] =  2147483648UL | 4194304 | 1 | 2 | 16384 | 1024 | 8 | 262144 | 524288 | 2147483648UL | 16777216UL | 128 | 16 | 32768;
		}
		//warlock - Improved Curse of Weakness
		sp = dbcSpell.LookupEntryForced( 18179 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 32768;
		sp = dbcSpell.LookupEntryForced( 18180 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 32768;

		//warlock - Improved Howl of Terror
		sp = dbcSpell.LookupEntryForced( 30054 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation_high[0] = 8;
		sp = dbcSpell.LookupEntryForced( 30057 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation_high[0] = 8;

		//warlock - Emberstorm
		sp = dbcSpell.LookupEntryForced( 17954 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[1] = 32 | 64 | 4 | 256 | 512;
			sp->EffectSpellGroupRelation_high[0] = 64 | 128;
			sp->EffectSpellGroupRelation_high[2] = 64;
			
		}
		sp = dbcSpell.LookupEntryForced( 17955 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[1] = 32 | 64 | 4 | 256 | 512;
			sp->EffectSpellGroupRelation_high[0] = 64 | 128;
			sp->EffectSpellGroupRelation_high[2] = 64;
		}
		sp = dbcSpell.LookupEntryForced( 17956 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[1] = 32 | 64 | 4 | 256 | 512;
			sp->EffectSpellGroupRelation_high[0] = 64 | 128;
			sp->EffectSpellGroupRelation_high[2] = 64;
		}
		sp = dbcSpell.LookupEntryForced( 17957 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[1] = 32 | 64 | 4 | 256 | 512;
			sp->EffectSpellGroupRelation_high[0] = 64 | 128;
			sp->EffectSpellGroupRelation_high[2] = 64;
		}
		sp = dbcSpell.LookupEntryForced( 17958 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[1] = 32 | 64 | 4 | 256 | 512;
			sp->EffectSpellGroupRelation_high[0] = 64 | 128;
			sp->EffectSpellGroupRelation_high[2] = 64;
		}

		//warlock - Shadow and Flame
		sp = dbcSpell.LookupEntryForced( 30288 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1;
			sp->EffectSpellGroupRelation_high[0] = 64;
		}
		sp = dbcSpell.LookupEntryForced( 30289 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1;
			sp->EffectSpellGroupRelation_high[0] = 64;
		}
		sp = dbcSpell.LookupEntryForced( 30290 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1;
			sp->EffectSpellGroupRelation_high[0] = 64;
		}
		sp = dbcSpell.LookupEntryForced( 30291 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1;
			sp->EffectSpellGroupRelation_high[0] = 64;
		}
		sp = dbcSpell.LookupEntryForced( 30292 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1;
			sp->EffectSpellGroupRelation_high[0] = 64;
		}

		//warlock - Ruin
		sp = dbcSpell.LookupEntryForced( 17959 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1 | 4 | 32 | 64 | 128 | 256 | 512 ;
			sp->EffectSpellGroupRelation_high[0] = 64 | 128 | 4096 ;
		}

		//warlock - destructive reach 
		sp = dbcSpell.LookupEntryForced( 17917 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1 | 4 | 32 | 64 | 128 | 256 | 512 ;
			sp->EffectSpellGroupRelation_high[0] = 64 | 128 | 4096;
			sp->EffectSpellGroupRelation[1] = 1 | 4 | 32 | 64 | 128 | 256 | 512 ;
			sp->EffectSpellGroupRelation_high[1] = 64 | 128 | 4096 ;
		}
		sp = dbcSpell.LookupEntryForced( 17918 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1 | 4 | 32 | 64 | 128 | 256 | 512 ;
			sp->EffectSpellGroupRelation_high[0] = 64 | 128 | 4096;
			sp->EffectSpellGroupRelation[1] = 1 | 4 | 32 | 64 | 128 | 256 | 512 ;
			sp->EffectSpellGroupRelation_high[1] = 64 | 128 | 4096 ;
		}

		//warlock - Cataclysm 
		sp = dbcSpell.LookupEntryForced( 17778 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1 | 4 | 32 | 64 | 128 | 256 | 512 ;
			sp->EffectSpellGroupRelation_high[0] = 64 | 128 | 4096 ;
			sp->EffectSpellGroupRelation[1] = 1 | 4 | 32 | 64 | 128 | 256 | 512 ;
			sp->EffectSpellGroupRelation_high[1] = 64 | 128 | 4096 ;
		}
		sp = dbcSpell.LookupEntryForced( 17779 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1 | 4 | 32 | 64 | 128 | 256 | 512 ;
			sp->EffectSpellGroupRelation_high[0] = 64 | 128 | 4096;
			sp->EffectSpellGroupRelation[1] = 1 | 4 | 32 | 64 | 128 | 256 | 512 ;
			sp->EffectSpellGroupRelation_high[1] = 64 | 128 | 4096 ;
		}
		sp = dbcSpell.LookupEntryForced( 17780 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1 | 4 | 32 | 64 | 128 | 256 | 512 ;
			sp->EffectSpellGroupRelation_high[0] = 64 | 128 | 4096;
			sp->EffectSpellGroupRelation[1] = 1 | 4 | 32 | 64 | 128 | 256 | 512 ;
			sp->EffectSpellGroupRelation_high[1] = 64 | 128 | 4096 ;
		}
		sp = dbcSpell.LookupEntryForced( 17781 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1 | 4 | 32 | 64 | 128 | 256 | 512 ;
			sp->EffectSpellGroupRelation_high[0] = 64 | 128 | 4096;
			sp->EffectSpellGroupRelation[1] = 1 | 4 | 32 | 64 | 128 | 256 | 512 ;
			sp->EffectSpellGroupRelation_high[1] = 64 | 128 | 4096 ;
		}
		sp = dbcSpell.LookupEntryForced( 17782 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1 | 4 | 32 | 64 | 128 | 256 | 512 ;
			sp->EffectSpellGroupRelation_high[0] = 64 | 128 | 4096;
			sp->EffectSpellGroupRelation[1] = 1 | 4 | 32 | 64 | 128 | 256 | 512 ;
			sp->EffectSpellGroupRelation_high[1] = 64 | 128 | 4096 ;
		}

		//warlock - Intensity 
		sp = dbcSpell.LookupEntryForced( 18135 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1 | 4 | 32 | 64 | 128 | 256 | 512 ;
			sp->EffectSpellGroupRelation_high[0] = 64 | 128 | 4096 ;
		}
		sp = dbcSpell.LookupEntryForced( 18136 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1 | 4 | 32 | 64 | 128 | 256 | 512 ;
			sp->EffectSpellGroupRelation_high[0] = 64 | 128 | 4096 ;
		}

		//warlock - Suppression
		sp = dbcSpell.LookupEntryForced( 18174 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1 | 2 | 8 | 32768 | 2147483648UL | 1024 | 16384 | 262144 | 16 | 524288 | 4194304;
			sp->EffectSpellGroupRelation_high[0] = 1 | 2 | 8 | 32768 | 2147483648UL | 1024 | 16384 | 262144 | 16 | 524288 | 4194304;
		}
		sp = dbcSpell.LookupEntryForced( 18175 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1 | 2 | 8 | 32768 | 2147483648UL | 1024 | 16384 | 262144 | 16 | 524288 | 4194304;
			sp->EffectSpellGroupRelation_high[0] = 1 | 2 | 8 | 32768 | 2147483648UL | 1024 | 16384 | 262144 | 16 | 524288 | 4194304;
		}
		sp = dbcSpell.LookupEntryForced( 18176 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1 | 2 | 8 | 32768 | 2147483648UL | 1024 | 16384 | 262144 | 16 | 524288 | 4194304;
			sp->EffectSpellGroupRelation_high[0] = 1 | 2 | 8 | 32768 | 2147483648UL | 1024 | 16384 | 262144 | 16 | 524288 | 4194304;
		}
		sp = dbcSpell.LookupEntryForced( 18177 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1 | 2 | 8 | 32768 | 2147483648UL | 1024 | 16384 | 262144 | 16 | 524288 | 4194304;
			sp->EffectSpellGroupRelation_high[0] = 1 | 2 | 8 | 32768 | 2147483648UL | 1024 | 16384 | 262144 | 16 | 524288 | 4194304;
		}
		sp = dbcSpell.LookupEntryForced( 18178 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1 | 2 | 8 | 32768 | 2147483648UL | 1024 | 16384 | 262144 | 16 | 524288 | 4194304;
			sp->EffectSpellGroupRelation_high[0] = 1 | 2 | 8 | 32768 | 2147483648UL | 1024 | 16384 | 262144 | 16 | 524288 | 4194304;
		}
		//warlock - Improved Curse of Agony
		sp = dbcSpell.LookupEntryForced( 18827 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1024;
		sp = dbcSpell.LookupEntryForced( 18829 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1024;

		//warlock - Grim Reach
		sp = dbcSpell.LookupEntryForced( 18218 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 2 | 8 | 32768 | 2147483648UL | 1024 | 16384 | 262144 | 16 | 524288 | 4194304;
			sp->EffectSpellGroupRelation_high[0] = 1 | 2 | 1024 ;
		sp = dbcSpell.LookupEntryForced( 18219 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 2 | 8 | 32768 | 2147483648UL | 1024 | 16384 | 262144 | 16 | 524288 | 4194304;
			sp->EffectSpellGroupRelation_high[0] = 1 | 2 | 1024 ;
		//warlock - Demonic Sacrifice
		sp = dbcSpell.LookupEntryForced( 18789 );
		if( sp != NULL )
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_ON_PET;
		sp = dbcSpell.LookupEntryForced( 18790 );
		if( sp != NULL )
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_ON_PET;
		sp = dbcSpell.LookupEntryForced( 18791 );
		if( sp != NULL )
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_ON_PET;
		sp = dbcSpell.LookupEntryForced( 18792 );
		if( sp != NULL )
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_ON_PET;
		sp = dbcSpell.LookupEntryForced( 35701 );
		if( sp != NULL )
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_ON_PET;

		//warlock - Demonic Tactics
		sp = dbcSpell.LookupEntryForced( 30242 );
		if( sp != NULL )
		{
			sp->Effect[0] = 0; //disble this. This is just blizz crap. Pure proove that they suck :P
			sp->EffectImplicitTargetB[1] = EFF_TARGET_PET;
			sp->EffectApplyAuraName[2] = SPELL_AURA_MOD_SPELL_CRIT_CHANCE; //lvl 1 has it fucked up :O
			sp->EffectImplicitTargetB[2] = EFF_TARGET_PET;
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
		}
		sp = dbcSpell.LookupEntryForced( 30245 );
		if( sp != NULL )
		{
			sp->Effect[0] = 0; //disble this. This is just blizz crap. Pure proove that they suck :P
			sp->EffectImplicitTargetB[1] = EFF_TARGET_PET;
			sp->EffectImplicitTargetB[2] = EFF_TARGET_PET;
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
		}
		sp = dbcSpell.LookupEntryForced( 30246 );
		if( sp != NULL )
		{
			sp->Effect[0] = 0; //disble this. This is just blizz crap. Pure proove that they suck :P
			sp->EffectImplicitTargetB[1] = EFF_TARGET_PET;
			sp->EffectImplicitTargetB[2] = EFF_TARGET_PET;
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
		}
		sp = dbcSpell.LookupEntryForced( 30247 );
		if( sp != NULL )
		{
			sp->Effect[0] = 0; //disble this. This is just blizz crap. Pure proove that they suck :P
			sp->EffectImplicitTargetB[1] = EFF_TARGET_PET;
			sp->EffectImplicitTargetB[2] = EFF_TARGET_PET;
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
		}
		sp = dbcSpell.LookupEntryForced( 30248 );
		if( sp != NULL )
		{
			sp->Effect[0] = 0; //disble this. This is just blizz crap. Pure proove that they suck :P
			sp->EffectImplicitTargetB[1] = EFF_TARGET_PET;
			sp->EffectImplicitTargetB[2] = EFF_TARGET_PET;
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
		}

		//warlock - Demonic Resilience
		sp = dbcSpell.LookupEntryForced( 30319 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
		}
		sp = dbcSpell.LookupEntryForced( 30320 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
		}
		sp = dbcSpell.LookupEntryForced( 30321 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
		}

		//warlock - Improved Imp
		sp = dbcSpell.LookupEntryForced( 18694 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectSpellGroupRelation[0] = 4096 | 8388608;
		}
		sp = dbcSpell.LookupEntryForced( 18695 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectSpellGroupRelation[0] = 4096 | 8388608;
		}
		sp = dbcSpell.LookupEntryForced( 18696 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectSpellGroupRelation[0] = 4096 | 8388608;
		}

		//warlock - Improved Voidwalker
		sp = dbcSpell.LookupEntryForced( 18705 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectSpellGroupRelation[0] = 33554432;
		}
		sp = dbcSpell.LookupEntryForced( 18706 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectSpellGroupRelation[0] = 33554432;
		}
		sp = dbcSpell.LookupEntryForced( 18707 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectSpellGroupRelation[0] = 33554432;
		}

		//warlock - Improved Succubus
		sp = dbcSpell.LookupEntryForced( 18754 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			sp->EffectSpellGroupRelation[0] = 8192 | 1073741824;
			sp->EffectSpellGroupRelation[1] = 1073741824;
		}
		sp = dbcSpell.LookupEntryForced( 18755 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			sp->EffectSpellGroupRelation[0] = 8192 | 1073741824;
			sp->EffectSpellGroupRelation[1] = 1073741824;
		}
		sp = dbcSpell.LookupEntryForced( 18756 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			sp->EffectSpellGroupRelation[0] = 8192 | 1073741824;
			sp->EffectSpellGroupRelation[1] = 1073741824;
		}

		//warlock - Fel Intellect
		sp = dbcSpell.LookupEntryForced( 18731 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_PERCENT_STAT;
			sp->EffectMiscValue[0] = 3;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}
		sp = dbcSpell.LookupEntryForced( 18743 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_PERCENT_STAT;
			sp->EffectMiscValue[0] = 3;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}
		sp = dbcSpell.LookupEntryForced( 18744 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_PERCENT_STAT;
			sp->EffectMiscValue[0] = 3;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}

		//warlock - Fel Stamina
		sp = dbcSpell.LookupEntryForced( 18748 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_PERCENT_STAT;
			sp->EffectMiscValue[0] = 2;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}
		sp = dbcSpell.LookupEntryForced( 18749 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_PERCENT_STAT;
			sp->EffectMiscValue[0] = 2;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}
		sp = dbcSpell.LookupEntryForced( 18750 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_PERCENT_STAT;
			sp->EffectMiscValue[0] = 2;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}

		//warlock - Demonic Tactics
		sp = dbcSpell.LookupEntryForced( 30242 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			//this is required since blizz uses spells for melee attacks while we use fixed functions
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			sp->EffectMiscValue[1] = SCHOOL_NORMAL;
			sp->EffectBasePoints[1] = sp->EffectBasePoints[0] ;
		}

		//warlock - Unholy Power
		sp = dbcSpell.LookupEntryForced( 18769 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectSpellGroupRelation[0] = 4096;
			//this is required since blizz uses spells for melee attacks while we use fixed functions
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			sp->EffectMiscValue[1] = SCHOOL_NORMAL;
			sp->EffectBasePoints[1] = sp->EffectBasePoints[0] ;
		}
		sp = dbcSpell.LookupEntryForced( 18770 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectSpellGroupRelation[0] = 4096;
			//this is required since blizz uses spells for melee attacks while we use fixed functions
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			sp->EffectMiscValue[1] = SCHOOL_NORMAL;
			sp->EffectBasePoints[1] = sp->EffectBasePoints[0] ;
		}
		sp = dbcSpell.LookupEntryForced( 18771 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectSpellGroupRelation[0] = 4096;
			//this is required since blizz uses spells for melee attacks while we use fixed functions
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			sp->EffectMiscValue[1] = SCHOOL_NORMAL;
			sp->EffectBasePoints[1] = sp->EffectBasePoints[0] ;
		}
		sp = dbcSpell.LookupEntryForced( 18772 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectSpellGroupRelation[0] = 4096;
			//this is required since blizz uses spells for melee attacks while we use fixed functions
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			sp->EffectMiscValue[1] = SCHOOL_NORMAL;
			sp->EffectBasePoints[1] = sp->EffectBasePoints[0] ;
		}
		sp = dbcSpell.LookupEntryForced( 18773 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectSpellGroupRelation[0] = 4096;
			//this is required since blizz uses spells for melee attacks while we use fixed functions
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			sp->EffectMiscValue[1] = SCHOOL_NORMAL;
			sp->EffectBasePoints[1] = sp->EffectBasePoints[0] ;
		}

		//warlock - Master Demonologist - 25 spells here
		sp = dbcSpell.LookupEntryForced( 23785 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET | SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 23784;
		}
		sp = dbcSpell.LookupEntryForced( 23822 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET | SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 23830;
		}
		sp = dbcSpell.LookupEntryForced( 23823 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET | SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 23831;
		}
		sp = dbcSpell.LookupEntryForced( 23824 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET | SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 23832;
		}
		sp = dbcSpell.LookupEntryForced( 23825 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET | SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 35708;
		}
		//and the rest
		sp = dbcSpell.LookupEntryForced( 23784 );
		if( sp != NULL )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		sp = dbcSpell.LookupEntryForced( 23830 );
		if( sp != NULL )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		sp = dbcSpell.LookupEntryForced( 23831 );
		if( sp != NULL )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		sp = dbcSpell.LookupEntryForced( 23832 );
		if( sp != NULL )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		sp = dbcSpell.LookupEntryForced( 35708 );
		if( sp != NULL )
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		sp = dbcSpell.LookupEntryForced( 23759 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
		}
		sp = dbcSpell.LookupEntryForced( 23760 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
		}
		sp = dbcSpell.LookupEntryForced( 23761 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
		}
		sp = dbcSpell.LookupEntryForced( 23762 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
		}
		sp = dbcSpell.LookupEntryForced( 23826 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
		}
		sp = dbcSpell.LookupEntryForced( 23827 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
		}
		sp = dbcSpell.LookupEntryForced( 23828 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
		}
		sp = dbcSpell.LookupEntryForced( 23829 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
		}
		for(uint32 i=23833;i<=23844;i++)
		{
			sp = dbcSpell.LookupEntryForced( i );
			if( sp != NULL )
			{
				sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
				sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			}
		}
		sp = dbcSpell.LookupEntryForced( 35702 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->Effect[1] = 0; //hacks, we are handling this in another way
		}
		sp = dbcSpell.LookupEntryForced( 35703 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->Effect[1] = 0; //hacks, we are handling this in another way
		}
		sp = dbcSpell.LookupEntryForced( 35704 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->Effect[1] = 0; //hacks, we are handling this in another way
		}
		sp = dbcSpell.LookupEntryForced( 35705 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->Effect[1] = 0; //hacks, we are handling this in another way
		}
		sp = dbcSpell.LookupEntryForced( 35706 );
		if( sp != NULL )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->Effect[1] = 0; //hacks, we are handling this in another way
		}
		//warlock - Improved Healthstone
		sp = dbcSpell.LookupEntryForced( 18692 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectMiscValue[0] = SMT_SPELL_VALUE;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
			sp->EffectSpellGroupRelation[0] = 65536;
		}
		sp = dbcSpell.LookupEntryForced( 18693 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectMiscValue[0] = SMT_SPELL_VALUE;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
			sp->EffectSpellGroupRelation[0] = 65536;
		}

		//warlock - Improved Drain Soul
		sp = dbcSpell.LookupEntryForced( 18213 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_TARGET_DIE | PROC_TARGET_SELF;
			sp->procChance = 100;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 18371;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
			sp->EffectSpellGroupRelation[1] = 2 | 8 | 32768 | 2147483648UL | 1024 | 16384 | 262144 | 16 | 524288 | 4194304;
			sp->Effect[2] = 0 ; //remove this effect
		}
		sp = dbcSpell.LookupEntryForced( 18372 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_TARGET_DIE | PROC_TARGET_SELF;
			sp->procChance = 100;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 18371;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
			sp->EffectSpellGroupRelation[1] = 2 | 8 | 32768 | 2147483648UL | 1024 | 16384 | 262144 | 16 | 524288 | 4194304;
			sp->Effect[2] = 0 ; //remove this effect
		}

		//warlock - Shadow Embrace
		sp = dbcSpell.LookupEntryForced( 32385 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->Effect[1] = 0 ; //remove this effect ? Maybe remove the other one :P xD
		}
		sp = dbcSpell.LookupEntryForced( 32387 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->Effect[1] = 0 ; //remove this effect ? Maybe remove the other one :P xD
		}
		sp = dbcSpell.LookupEntryForced( 32392 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->Effect[1] = 0 ; //remove this effect ? Maybe remove the other one :P xD
		}
		sp = dbcSpell.LookupEntryForced( 32393 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->Effect[1] = 0 ; //remove this effect ? Maybe remove the other one :P xD
		}
		sp = dbcSpell.LookupEntryForced( 32394 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->Effect[1] = 0 ; //remove this effect ? Maybe remove the other one :P xD
		}

		//warlock - Bane
		sp = dbcSpell.LookupEntryForced( 17788 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1 | 4;
			sp->EffectSpellGroupRelation_high[1] = 128;
		}
		sp = dbcSpell.LookupEntryForced( 17789 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1 | 4;
			sp->EffectSpellGroupRelation_high[1] = 128;
		}
		sp = dbcSpell.LookupEntryForced( 17790 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1 | 4;
			sp->EffectSpellGroupRelation_high[1] = 128;
		}
		sp = dbcSpell.LookupEntryForced( 17791 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1 | 4;
			sp->EffectSpellGroupRelation_high[1] = 128;
		}
		sp = dbcSpell.LookupEntryForced( 17792 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1 | 4;
			sp->EffectSpellGroupRelation_high[1] = 128;
		}

		//warlock - soul leech
		sp = dbcSpell.LookupEntryForced( 30293 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6; //aura
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 30294;
			sp->procFlags = uint32(PROC_ON_CAST_SPELL|PROC_TARGET_SELF);
		}
		sp = dbcSpell.LookupEntryForced( 30295 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6; //aura
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 30294;
			sp->procFlags = uint32(PROC_ON_CAST_SPELL|PROC_TARGET_SELF);
		}
		sp = dbcSpell.LookupEntryForced( 30296 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6; //aura
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 30294;
			sp->procFlags = uint32(PROC_ON_CAST_SPELL|PROC_TARGET_SELF);
		}

		//warlock - Pyroclasm
		sp = dbcSpell.LookupEntryForced( 18073 );
		if( sp != NULL )
		{
			sp->Effect[0] = 0; //delete this owerride effect :P
			sp->EffectTriggerSpell[1] = 18093; //trigger spell was wrong :P
			sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION;
			sp->procChance = 13; //god, save us from fixed values !
		}
		sp = dbcSpell.LookupEntryForced( 18096 );
		if( sp != NULL )
		{
			sp->Effect[0] = 0; //delete this owerride effect :P
			sp->EffectTriggerSpell[1] = 18093; //trigger spell was wrong :P
			sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION;
			sp->procChance = 26; //god, save us from fixed values !
		}

	//////////////////////////////////////////
	// DRUID								//
	//////////////////////////////////////////

	// Insert druid spell fixes here

		//Druid: Feral Swiftness
		sp = dbcSpell.LookupEntryForced( 17002 );
		if ( sp != NULL )
		{
			sp->Effect[1] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1] = 24864;
		}
		sp = dbcSpell.LookupEntryForced( 24866 );
		if ( sp != NULL )
		{
			sp->Effect[1] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1] = 24867;
		}

		//Druid: Natural Perfection
		sp = dbcSpell.LookupEntryForced( 33881 );
		if ( sp != NULL )
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM;
		sp = dbcSpell.LookupEntryForced( 33882 );
		if ( sp != NULL )
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM;
		sp = dbcSpell.LookupEntryForced( 33883 );
		if ( sp != NULL )
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM;

		//Druid: Frenzied Regeneration
		sp = dbcSpell.LookupEntryForced( 22842 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 22845;
		}
		sp = dbcSpell.LookupEntryForced( 22895 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 22845;
		}
		sp = dbcSpell.LookupEntryForced( 22896 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 22845;
		}
		sp = dbcSpell.LookupEntryForced( 26999 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 22845;
		}

		//Druid - Ferocity.
		sp = dbcSpell.LookupEntryForced( 16934 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 2048;
			sp->EffectSpellGroupRelation_high[0] = 1048576 | 64;
			sp->EffectSpellGroupRelation[1] = 4096;
			sp->EffectSpellGroupRelation_high[1] = 1024;
		}
		sp = dbcSpell.LookupEntryForced( 16935 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 2048;
			sp->EffectSpellGroupRelation_high[0] = 1048576 | 64;
			sp->EffectSpellGroupRelation[1] = 4096;
			sp->EffectSpellGroupRelation_high[1] = 1024;
		}
		sp = dbcSpell.LookupEntryForced( 16936 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 2048;
			sp->EffectSpellGroupRelation_high[0] = 1048576 | 64;
			sp->EffectSpellGroupRelation[1] = 4096;
			sp->EffectSpellGroupRelation_high[1] = 1024;
		}
		sp = dbcSpell.LookupEntryForced( 16937 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 2048;
			sp->EffectSpellGroupRelation_high[0] = 1048576 | 64;
			sp->EffectSpellGroupRelation[1] = 4096;
			sp->EffectSpellGroupRelation_high[1] = 1024;
		}
		sp = dbcSpell.LookupEntryForced( 16938 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 2048;
			sp->EffectSpellGroupRelation_high[0] = 1048576 | 64;
			sp->EffectSpellGroupRelation[1] = 4096;
			sp->EffectSpellGroupRelation_high[1] = 1024;
		}

		//Druid - Focused Starlight
		sp = dbcSpell.LookupEntryForced( 35363 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1 | 4;
		sp = dbcSpell.LookupEntryForced( 35364 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1 | 4;

		//Druid - Celestial Focus
		sp = dbcSpell.LookupEntryForced( 16850 ); 
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectSpellGroupRelation[1] = 1;
		}
		sp = dbcSpell.LookupEntryForced( 16923 ); 
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectSpellGroupRelation[1] = 1;
		}
		sp = dbcSpell.LookupEntryForced( 16924 ); 
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectSpellGroupRelation[1] = 1;
		}

		//Druid - Feral Aggression. Blizz made a mistake here ?
		sp = dbcSpell.LookupEntryForced( 16858 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 8;
			sp->EffectSpellGroupRelation[1] = 8388608;
		}
		sp = dbcSpell.LookupEntryForced( 16859 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 8;
			sp->EffectSpellGroupRelation[1] = 8388608;
		}
		sp = dbcSpell.LookupEntryForced( 16860 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 8;
			sp->EffectSpellGroupRelation[1] = 8388608;
		}
		sp = dbcSpell.LookupEntryForced( 16861 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 8;
			sp->EffectSpellGroupRelation[1] = 8388608;
		}
		sp = dbcSpell.LookupEntryForced( 16862 ); 
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 8;
			sp->EffectSpellGroupRelation[1] = 8388608;
		}
		//Druid: Leader of the Pack
		sp = dbcSpell.LookupEntryForced( 24932 );
		if( sp != NULL )
		{
			sp->Effect[1] = 0;
			sp->Effect[2] = 0; //removing strange effects.
		}

		// Druid: Control of Nature
		sp = dbcSpell.LookupEntryForced( 16918 );			// Rank 1
		sp->EffectSpellGroupRelation_high[0] |= 0x20;

		sp = dbcSpell.LookupEntryForced( 16919 );			// Rank 2
		sp->EffectSpellGroupRelation_high[0] |= 0x20;

		sp = dbcSpell.LookupEntryForced( 16920 );			// Rank 3
		sp->EffectSpellGroupRelation_high[0] |= 0x20;

		// Druid: Brambles
		sp = dbcSpell.LookupEntryForced( 16836 );			// Rank 1
		sp->EffectSpellGroupRelation[0] |= 0x200;

		sp = dbcSpell.LookupEntryForced( 16839 );			// Rank 2
		sp->EffectSpellGroupRelation[0] |= 0x200;

		sp = dbcSpell.LookupEntryForced( 16840 );			// Rank 3
		sp->EffectSpellGroupRelation[0] |= 0x200;

		//Druid: Improved Leader of the Pack
		sp = dbcSpell.LookupEntryForced( 34299 );
		if( sp != NULL )
			sp->proc_interval = 6000;//6 secs
		//druid Savage Fury
		sp = dbcSpell.LookupEntryForced( 16998 );
		if( sp != NULL ) 
		{
			sp->EffectSpellGroupRelation_high[0] |= 1024;
			//sp->EffectSpellGroupRelation_high[1] |= 1024;
			//sp->EffectSpellGroupRelation_high[2] |= 1024;
		}
		sp = dbcSpell.LookupEntryForced( 16999 );
		if( sp != NULL ) 
		{
			sp->EffectSpellGroupRelation_high[0] |= 1024;
			//sp->EffectSpellGroupRelation_high[1] |= 1024;
			//sp->EffectSpellGroupRelation_high[2] |= 1024;
		}

		//druid - Blood Frenzy
		sp = dbcSpell.LookupEntryForced( 16954 );
		if( sp != NULL ) 
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 16953;
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->procChance = 100;
		}
		sp = dbcSpell.LookupEntryForced( 16952 );
		if( sp != NULL ) 
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 16953;
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->procChance = 50;
		}

		//druid - Primal Fury
		sp = dbcSpell.LookupEntryForced( 16961 );
		if( sp != NULL ) 
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 16959;
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->procChance = 100;
		}
		sp = dbcSpell.LookupEntryForced( 16958 );
		if( sp != NULL ) 
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 16959;
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->procChance = 50;
		}

		//druid - Intensity
		sp = dbcSpell.LookupEntryForced( 17106 );
		if( sp != NULL )
		{
		   sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
		   sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 17107 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			 sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 17108 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		//Nature's Grasp
		sp = dbcSpell.LookupEntryForced( 16689 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6; 
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 339; 
			sp->Effect[1] = 0; 
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM | PROC_REMOVEONUSE;
			sp->AuraInterruptFlags = 0; //we remove it on proc or timeout
		}
		sp = dbcSpell.LookupEntryForced( 16810 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6; 
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 1062; 
			sp->Effect[1] = 0; 
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM | PROC_REMOVEONUSE;
			sp->AuraInterruptFlags = 0; //we remove it on proc or timeout
		}
		sp = dbcSpell.LookupEntryForced( 16811 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6; 
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 5195; 
			sp->Effect[1] = 0; 
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM | PROC_REMOVEONUSE;
			sp->AuraInterruptFlags = 0; //we remove it on proc or timeout
		}
		sp = dbcSpell.LookupEntryForced( 16812 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6; 
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 5196; 
			sp->Effect[1] = 0; 
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM | PROC_REMOVEONUSE;
			sp->AuraInterruptFlags = 0; //we remove it on proc or timeout
		}
		sp = dbcSpell.LookupEntryForced( 16813 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6; 
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 9852; 
			sp->Effect[1] = 0; 
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM | PROC_REMOVEONUSE;
			sp->AuraInterruptFlags = 0; //we remove it on proc or timeout
		}
		sp = dbcSpell.LookupEntryForced( 17329 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6; 
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 9853; 
			sp->Effect[1] = 0; 
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM | PROC_REMOVEONUSE;
			sp->AuraInterruptFlags = 0; //we remove it on proc or timeout
		}
		sp = dbcSpell.LookupEntryForced( 27009 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6; 
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 26989; 
			sp->Effect[1] = 0; 
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM | PROC_REMOVEONUSE;
			sp->AuraInterruptFlags = 0; //we remove it on proc or timeout
		}
		//pounce
		sp = dbcSpell.LookupEntryForced( 9007 );
		if( sp != NULL )
		{
			sp->MechanicsType = MECHANIC_BLEEDING;
		}
		sp = dbcSpell.LookupEntryForced( 9824 );
		if( sp != NULL )
		{
			sp->MechanicsType = MECHANIC_BLEEDING;
		}
		sp = dbcSpell.LookupEntryForced( 9826 );
		if( sp != NULL )
		{
			sp->MechanicsType = MECHANIC_BLEEDING;
		}
		sp = dbcSpell.LookupEntryForced( 27007 );
		if( sp != NULL )
		{
			sp->MechanicsType = MECHANIC_BLEEDING;
		}
		//rip
		sp = dbcSpell.LookupEntryForced( 1079 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 9492 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 9493 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 9752 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 9894 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 9896 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 27008 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		//rake
		sp = dbcSpell.LookupEntryForced( 1822 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 1823 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 1824 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 9904 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		sp = dbcSpell.LookupEntryForced( 27003 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;
		//lacerate
		sp = dbcSpell.LookupEntryForced( 33745 );
		if( sp != NULL )
			sp->MechanicsType = MECHANIC_BLEEDING;

		//Pounce Bleed
		sp = dbcSpell.LookupEntryForced( 9007 );
		if( sp != NULL )
			sp->DurationIndex = 18000;
		sp = dbcSpell.LookupEntryForced( 9824 );
		if( sp != NULL )
			sp->DurationIndex = 18000;
		sp = dbcSpell.LookupEntryForced( 9826 );
		if( sp != NULL )
			sp->DurationIndex = 18000;
		sp = dbcSpell.LookupEntryForced( 27007 );
		if( sp != NULL )
			sp->DurationIndex = 18000;

		//Druid: Natural Shapeshifter
		sp = dbcSpell.LookupEntryForced( 16833 );
		if( sp != NULL )
			sp->DurationIndex = 0;
		sp = dbcSpell.LookupEntryForced( 16834 );
		if( sp != NULL )
			sp->DurationIndex = 0;
		sp = dbcSpell.LookupEntryForced( 16835 );
		if( sp != NULL )
			sp->DurationIndex = 0;
		// druid - Tree of Life
		sp = dbcSpell.LookupEntryForced( 5420 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[2] = 32 | 64 | 16; //for the mana cost tweak
			sp->EffectSpellGroupRelation_high[2] = 2 | 4096 | 128 | 524288 ;
		}

		// druid - Shredding Attacks
		sp = dbcSpell.LookupEntryForced( 16966 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 32768;
			sp->EffectSpellGroupRelation_high[1] = 256;
		}
		sp = dbcSpell.LookupEntryForced( 16968 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 32768;
			sp->EffectSpellGroupRelation_high[1] = 256;
		}

		// druid - Naturalist
		sp = dbcSpell.LookupEntryForced( 17069 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
			sp->EffectMiscValue[1] = 1;  
		}
		sp = dbcSpell.LookupEntryForced( 17070 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
			sp->EffectMiscValue[1] = 1; 
		}
		sp = dbcSpell.LookupEntryForced( 17071 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
			sp->EffectMiscValue[1] = 1; 
		}
		sp = dbcSpell.LookupEntryForced( 17072 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
			sp->EffectMiscValue[1] = 1;
		}
		sp = dbcSpell.LookupEntryForced( 17073 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
			sp->EffectMiscValue[1] = 1; 
		}

		// druid - Gift of Nature
		sp = dbcSpell.LookupEntryForced( 17104 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 268435456 | 32 | 64 | 16 | 128 | 268435456;
			sp->EffectSpellGroupRelation_high[0] |= 16;
			sp->EffectSpellGroupRelation[1] = 268435456 | 32 | 64 | 16 | 128 | 268435456;
			sp->EffectSpellGroupRelation_high[1] |= 16;
		}
		sp = dbcSpell.LookupEntryForced( 24943 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 268435456 | 32 | 64 | 16 | 128 | 268435456;
			sp->EffectSpellGroupRelation_high[0] |= 16;
			sp->EffectSpellGroupRelation[1] = 268435456 | 32 | 64 | 16 | 128 | 268435456;
			sp->EffectSpellGroupRelation_high[1] |= 16;
		}
		sp = dbcSpell.LookupEntryForced( 24944 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 268435456 | 32 | 64 | 16 | 128 | 268435456;
			sp->EffectSpellGroupRelation_high[0] |= 16;
			sp->EffectSpellGroupRelation[1] = 268435456 | 32 | 64 | 16 | 128 | 268435456;
			sp->EffectSpellGroupRelation_high[1] |= 16;
		}
		sp = dbcSpell.LookupEntryForced( 24945 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 268435456 | 32 | 64 | 16 | 128 | 268435456;
			sp->EffectSpellGroupRelation_high[0] |= 16;
			sp->EffectSpellGroupRelation[1] = 268435456 | 32 | 64 | 16 | 128 | 268435456;
			sp->EffectSpellGroupRelation_high[1] |= 16;
		}
		sp = dbcSpell.LookupEntryForced( 24946 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 268435456 | 32 | 64 | 16 | 128 | 268435456;
			sp->EffectSpellGroupRelation_high[0] |= 16;
			sp->EffectSpellGroupRelation[1] = 268435456 | 32 | 64 | 16 | 128 | 268435456;
			sp->EffectSpellGroupRelation_high[1] |= 16;
		}

		// druid - Empowered Touch
		sp = dbcSpell.LookupEntryForced( 33879 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 32;
		sp = dbcSpell.LookupEntryForced( 33880 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 32;

		// druid - Empowered Rejuvenation
		sp = dbcSpell.LookupEntryForced( 33886 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 64 | 16 | 128;
		sp = dbcSpell.LookupEntryForced( 33887 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 64 | 16 | 128;
		sp = dbcSpell.LookupEntryForced( 33888 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 64 | 16 | 128;
		sp = dbcSpell.LookupEntryForced( 33889 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 64 | 16 | 128;
		sp = dbcSpell.LookupEntryForced( 33890 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 64 | 16 | 128;

		// druid - Wrath of cenarius
		sp = dbcSpell.LookupEntryForced( 33603 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 4;
			sp->EffectSpellGroupRelation[1] = 1;
		}
		sp = dbcSpell.LookupEntryForced( 33604 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 4;
			sp->EffectSpellGroupRelation[1] = 1;
		}
		sp = dbcSpell.LookupEntryForced( 33605 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 4;
			sp->EffectSpellGroupRelation[1] = 1;
		}
		sp = dbcSpell.LookupEntryForced( 33606 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 4;
			sp->EffectSpellGroupRelation[1] = 1;
		}
		sp = dbcSpell.LookupEntryForced( 33607 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 4;
			sp->EffectSpellGroupRelation[1] = 1;
		}

		// druid - Nature's Grace
		sp = dbcSpell.LookupEntryForced( 16880 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;

		sp = dbcSpell.LookupEntryForced( 16886 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 0xFFFFFFFF; //all spells, too bad not all spells have grouping flags :S
			sp->procCharges = 1;
		}

		// druid - Starlight Wrath
		sp = dbcSpell.LookupEntryForced( 16814 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1 | 4;
		sp = dbcSpell.LookupEntryForced( 16815 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1 | 4;
		sp = dbcSpell.LookupEntryForced( 16816 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1 | 4;
		sp = dbcSpell.LookupEntryForced( 16817 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1 | 4;
		sp = dbcSpell.LookupEntryForced( 16818 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1 | 4;

		// Druid: Omen of Clarity
		sp = dbcSpell.LookupEntryForced( 16864 );
		if( sp != NULL )
		{
			sp->procChance=100; //procchance dynamic. 3ppm
			sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_ON_CRIT_ATTACK;
		}
		uint32 mm = (1<<(FORM_BEAR-1))|(1<<(FORM_DIREBEAR-1))|(1<<(FORM_MOONKIN-1))|(1<<(FORM_CAT-1));

		sp = dbcSpell.LookupEntryForced( 16972 );
		if( sp != NULL )
			sp->RequiredShapeShift = mm;
		sp = dbcSpell.LookupEntryForced( 16974 );
		if( sp != NULL )
			sp->RequiredShapeShift = mm;
		sp = dbcSpell.LookupEntryForced( 16975 );
		if( sp != NULL )
			sp->RequiredShapeShift = mm;

	//////////////////////////////////////////
	// ITEMS								//
	//////////////////////////////////////////

	// Insert items spell fixes here

	#ifdef NEW_PROCFLAGS
		//Bonescythe Armor
		sp = dbcSpell.LookupEntryForced( 28814 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]=8519680;

		//Tome of the Lightbringer
		sp = dbcSpell.LookupEntryForced( 41042 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]=8388608;
		//Gladiator's Libram of Fortitude
		sp = dbcSpell.LookupEntryForced( 43850 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]=8388608;
		//Vengeful Gladiator's Libram of Fortitude
		sp = dbcSpell.LookupEntryForced( 43852 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]=8388608;
		//Merciless Gladiator's Libram of Fortitude
		sp = dbcSpell.LookupEntryForced( 43851 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]=8388608;
		//Gladiator's Libram of Vengeance
		sp = dbcSpell.LookupEntryForced( 43854 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation_high[0]=64;
		//Merciless Gladiator's Libram of Vengeance
		sp = dbcSpell.LookupEntryForced( 43855 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation_high[0]=64;
		//Vengeful Gladiator's Libram of Vengeance
		sp = dbcSpell.LookupEntryForced( 43856 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation_high[0]=64;
		//The Earthshatterer
		sp = dbcSpell.LookupEntryForced( 28821 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]=1024;
		//Idol of the White Stag
		sp = dbcSpell.LookupEntryForced( 41037 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation_high[0]=64 | 1024;
		//Merciless Gladiator's Idol of Resolve
		sp = dbcSpell.LookupEntryForced( 43842 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation_high[0]=64 | 1024;
		//Vengeful Gladiator's Idol of Resolve
		sp = dbcSpell.LookupEntryForced( 43843 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation_high[0]=64 | 1024;
		//Merciless Gladiator's Idol of Steadfastness
		sp = dbcSpell.LookupEntryForced( 43844 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]=2;
		//Vengeful Gladiator's Idol of Steadfastness
		sp = dbcSpell.LookupEntryForced( 43845 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]=2;
		//Merciless Gladiator's Totem of Indomitability
		sp = dbcSpell.LookupEntryForced( 43858 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation_high[0]=16;
		//Vengeful Gladiator's Totem of Indomitability
		sp = dbcSpell.LookupEntryForced( 43859 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation_high[0]=16;
		//Gladiator's Totem of Indomitability
		sp = dbcSpell.LookupEntryForced( 43857 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation_high[0]=16;
		//Merciless Gladiator's Totem of Survival
		sp = dbcSpell.LookupEntryForced( 43861 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]= 1048576 |268435456 | 2147483648;
		//Vengeful Gladiator's Totem of Survival
		sp = dbcSpell.LookupEntryForced( 43862 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]= 1048576 |268435456 | 2147483648;
		//Gladiator's Totem of Survival
		sp = dbcSpell.LookupEntryForced( 43861 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]= 1048576 |268435456 | 2147483648;
		//Wolfshead Helm
		sp = dbcSpell.LookupEntryForced( 17768 );
		if (sp != NULL)
		{
			sp->EffectSpellGroupRelation[0]= 1073741824;
			sp->EffectSpellGroupRelation[1]= 2147483648;
		}
		//Set: Plagueheart Raiment
		sp = dbcSpell.LookupEntryForced( 28831 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]= 1;
		//Set: Gladiator's Idol of Resolve
		sp = dbcSpell.LookupEntryForced( 37191 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation_high[0]=64 | 1024;
		//Set: Gladiator's Idol of Steadfastness
		sp = dbcSpell.LookupEntryForced( 43841 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]=2;
		//Set: Incarnate Raiment
		sp = dbcSpell.LookupEntryForced( 37564 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]=512;
		//Talon of Al'ar
		sp = dbcSpell.LookupEntryForced( 37507 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]=2048;
		//Set: Crystalforge Armor
		sp = dbcSpell.LookupEntryForced( 37191 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation_high[0]=64;
		//Set: Redemption Armor
		sp = dbcSpell.LookupEntryForced( 28787 );
		if (sp != NULL)
			sp->EffectSpellGroupRelation[0]=4096;
		//Idol of the Claw
		sp = dbcSpell.LookupEntryForced( 34323 );
		if( sp != NULL )
		{
			sp->Flags5 = FLAGS5_PROCCHANCE_COMBOBASED;
			sp->EffectSpellGroupRelation[0]=8388608;
			sp->EffectSpellGroupRelation_high[0]=128;
		}
	#endif
		//Compact Harvest Reaper
		sp = dbcSpell.LookupEntryForced( 4078 );
		if( sp != NULL )
		{
			sp->DurationIndex = 6;
		}

		//Graccu's Mince Meat Fruitcake
		sp = dbcSpell.LookupEntryForced(25990);
		if( sp != NULL )
		{
			sp->EffectAmplitude[1] = 1000;
		}

		//Extract Gas 
		sp = dbcSpell.LookupEntryForced( 30427 );
		if( sp != NULL )
		{
			sp->Effect[0] = SPELL_EFFECT_DUMMY;
		}

		//Relic - Idol of the Unseen Moon
		sp = dbcSpell.LookupEntryForced( 43739 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 43740;
		}

		//Tome of Fiery Redemption
		sp = dbcSpell.LookupEntryForced( 37197 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 37198;
			sp->procChance = 15;
		}

		//Thunderfury
		sp = dbcSpell.LookupEntryForced( 21992 );
		if( sp != NULL )
		{
			sp->Effect[2] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[2] = 27648;
			sp->EffectImplicitTargetA[2] = EFF_TARGET_SELF;
		}

		//Solarian's Sapphire
		sp = dbcSpell.LookupEntryForced( 37536 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 65536;

		//Totem of the Pulsing Earth
		sp = dbcSpell.LookupEntryForced( 37740 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1;

		//Totem of the Maelstrom
		sp = dbcSpell.LookupEntryForced( 37738 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 64;

		//Totem of Living Water
		sp = dbcSpell.LookupEntryForced( 43752 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 256;

		//Totem of Healing Rains
		sp = dbcSpell.LookupEntryForced( 38322 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 256;

		//Totem of Lightning
		sp = dbcSpell.LookupEntryForced( 33696 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1;

		//Everbloom Idol
		sp = dbcSpell.LookupEntryForced( 33693 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 32768;

		//Idol of the Avian Heart
		sp = dbcSpell.LookupEntryForced( 38321 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 32;

		//Idol of the Crescent Goddess
		sp = dbcSpell.LookupEntryForced( 37737 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 64;

		//Idol of the Avenger
		sp = dbcSpell.LookupEntryForced( 37760 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1;

		//Energized 
		sp = dbcSpell.LookupEntryForced( 43750 ); 
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;

		//Spell Haste Trinket
		sp = dbcSpell.LookupEntryForced( 33297 ); 
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL | PROC_TARGET_SELF;

		// Band of the Eternal Sage
		sp = dbcSpell.LookupEntryForced( 35083 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;

		// Band of the Eternal Restorer 
		sp = dbcSpell.LookupEntryForced( 35086 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;

		// Ashtongue Talisman of Shadows 
		sp = dbcSpell.LookupEntryForced( 40478 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;

		// Ashtongue Talisman of Swiftness
		sp = dbcSpell.LookupEntryForced( 40485 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;

		// Ashtongue Talisman of Valor
		sp = dbcSpell.LookupEntryForced( 40458 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;

		// Memento of Tyrande
		sp = dbcSpell.LookupEntryForced( 37655 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_CAST_SPELL;

		// Ashtongue Talisman of Insight
		sp = dbcSpell.LookupEntryForced( 40482 );
		if( sp != NULL )
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;

		//Ashtongue Talisman of Equilibrium
		sp = dbcSpell.LookupEntryForced( 40442 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 40;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectTriggerSpell[0] = 40452;
			sp->maxstack = 1;
			sp->Effect[1] = 6;
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 25;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectTriggerSpell[1] = 40445;
			sp->maxstack = 1;
			sp->Effect[2] = 6;
			sp->EffectApplyAuraName[2] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 25;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectTriggerSpell[2] = 40446;
			sp->maxstack = 1;
		}

		//Ashtongue Talisman of Acumen
		sp = dbcSpell.LookupEntryForced( 40438 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 10;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectTriggerSpell[0] = 40441;
			sp->maxstack = 1;
			sp->Effect[1] = 6;
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 10;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectTriggerSpell[1] = 40440;
			sp->maxstack = 1;
		}
		// Drums of war targets sorounding party members instead of us
		sp = dbcSpell.LookupEntryForced( 35475 );
		if( sp != NULL )
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_ALL_PARTY;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_ALL_PARTY;
			sp->EffectImplicitTargetA[2] = 0;
			sp->EffectImplicitTargetB[0] = 0;
			sp->EffectImplicitTargetB[1] = 0;
			sp->EffectImplicitTargetB[2] = 0;
		}
		// Symbol of Hope targets sorounding party members instead of us
		sp = dbcSpell.LookupEntryForced( 32548 );
		if( sp != NULL )
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_ALL_PARTY;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_ALL_PARTY;
			sp->EffectImplicitTargetA[2] = 0;
			sp->EffectImplicitTargetB[0] = 0;
			sp->EffectImplicitTargetB[1] = 0;
			sp->EffectImplicitTargetB[2] = 0;
		}

		// Drums of Battle targets sorounding party members instead of us
		sp = dbcSpell.LookupEntryForced( 35476 );
		if( sp != NULL )
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_ALL_PARTY;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_ALL_PARTY;
			sp->EffectImplicitTargetA[2] = 0;
			sp->EffectImplicitTargetB[0] = 0;
			sp->EffectImplicitTargetB[1] = 0;
			sp->EffectImplicitTargetB[2] = 0;
		}

		// Drums of Panic targets sorounding creatures instead of us
		sp = dbcSpell.LookupEntryForced( 35474 );
		if( sp != NULL )
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
			sp->EffectImplicitTargetA[2] = 0;
			sp->EffectImplicitTargetB[0] = 0;
			sp->EffectImplicitTargetB[1] = 0;
			sp->EffectImplicitTargetB[2] = 0;
		}

		// Drums of Restoration targets sorounding party members instead of us
		sp = dbcSpell.LookupEntryForced( 35478 );
		if( sp != NULL )
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_ALL_PARTY;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_ALL_PARTY;
			sp->EffectImplicitTargetA[2] = 0;
			sp->EffectImplicitTargetB[0] = 0;
			sp->EffectImplicitTargetB[1] = 0;
			sp->EffectImplicitTargetB[2] = 0;
		}
		// Drums of Speed targets sorounding party members instead of us
		sp = dbcSpell.LookupEntryForced( 35477 );
		if( sp != NULL )
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_ALL_PARTY;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_ALL_PARTY;
			sp->EffectImplicitTargetA[2] = 0;
			sp->EffectImplicitTargetB[0] = 0;
			sp->EffectImplicitTargetB[1] = 0;
			sp->EffectImplicitTargetB[2] = 0;
		}

		sp = dbcSpell.LookupEntryForced( 34774 );
		if( sp != NULL ) //dragonspine trophy proc
		{
			sp->procChance = 6;
		}
	#ifndef NEW_PROCFLAGS
		//Ashtongue Talisman of Lethality
		sp = dbcSpell.LookupEntryForced( 40460 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 20;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectTriggerSpell[0] = 40461;
			sp->maxstack = 1;
		}
	#else
		sp = dbcSpell.LookupEntryForced( 40460 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 262144 | 2097152 | 8388608 | 8519680 | 524288 | 1048576 | 8388608;
	#endif

		//Serpent-Coil Braid
		sp = dbcSpell.LookupEntryForced( 37447 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 100;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectTriggerSpell[0] = 37445;
			sp->maxstack = 1;
		}

		//Item: Assassination Armor
		sp = dbcSpell.LookupEntryForced(37166);
		if (sp != NULL)
		{
			sp->EffectSpellGroupRelation[0]=8519680;
			sp->EffectSpellGroupRelation_high[0]=8;
		}

		//Item Set: Thunderheart Harness
		sp = dbcSpell.LookupEntryForced( 38447 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation_high[0] |=1024;
			sp->EffectSpellGroupRelation_high[1] |=64;
		}
		sp = dbcSpell.LookupEntryForced( 38416 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] |=8388608;
			sp->EffectSpellGroupRelation_high[0] |=1048576;
			sp->EffectSpellGroupRelation[1] |=8388608;
		}

		//Item Set: Thunderheart Regalia
		sp = dbcSpell.LookupEntryForced( 38414 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] |=2;
		}
		sp = dbcSpell.LookupEntryForced( 38415 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] |=4;
		}

		//Item Set: Thunderheart Raiment
		sp = dbcSpell.LookupEntryForced( 38417 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation_high[0] |=2;
		}
		sp = dbcSpell.LookupEntryForced( 38420 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation_high[0] |=32;
		}

		//Item Set: Nordrassil Harness
		sp = dbcSpell.LookupEntryForced( 37333 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation_high[1] |=256;
			sp->EffectSpellGroupRelation[0] |=32768;
		}
/*
		//Item Set: Nordrassil Raiment
		sp = dbcSpell.LookupEntryForced( 37313 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] |=64;
		}
		sp = dbcSpell.LookupEntryForced( 37314 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation_high[0] |=16;
		}
*/
		//Item Set: Malorne Raiment
		sp = dbcSpell.LookupEntryForced( 37292 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation_high[0] |=524288;
		}

		//Item Set: Malorne Regalia
		sp = dbcSpell.LookupEntryForced( 37297 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation_high[0] |=4096;
		}

		//Item Set: Malorne Harness
		sp = dbcSpell.LookupEntryForced( 37306 );
		if( sp != NULL )
		{
			sp->procChance = 4;
			sp->procFlags = PROC_ON_MELEE_ATTACK;
		}
		sp = dbcSpell.LookupEntryForced( 37311 );
		if( sp != NULL )
		{
			sp->procChance = 4;
			sp->procFlags = PROC_ON_MELEE_ATTACK;
		}

		//Item Set: Slayer's Armor
		sp = dbcSpell.LookupEntryForced( 38388 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] |=262144;
		}
		sp = dbcSpell.LookupEntryForced( 38389 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation_high[0] |= 2 | 4;
			sp->EffectSpellGroupRelation[0] |= 8388612 |8388610 |41943040;
		}

		//Item Set: Deathmantle
		sp = dbcSpell.LookupEntryForced( 37170 );
		if( sp != NULL )
		{
			sp->procChance = 4;
			sp->procFlags = PROC_ON_MELEE_ATTACK;
		}

		//Item Set: Netherblade
		sp = dbcSpell.LookupEntryForced( 37167 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] |= 262144;
		}
		sp = dbcSpell.LookupEntryForced( 37168 );
		if( sp != NULL )
		{
			sp->procChance = 15;
			//sp->procFlags = PROC_ON_CAST_SPELL; Need new flag - PROC_ON_FINISH_MOVE;
		}

		//Item Set: Tempest Regalia
		sp = dbcSpell.LookupEntryForced( 38396 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 67108864;
		}
		sp = dbcSpell.LookupEntryForced( 38397 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1572896 | 1 | 2048;
		}

		//Item Set: Tirisfal Regalia
		sp = dbcSpell.LookupEntryForced( 37441 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 536870912;
			sp->EffectSpellGroupRelation[1] = 536870912;
		}
		sp = dbcSpell.LookupEntryForced( 37443 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
		}

		//Item Set: Aldor Regalia
		sp = dbcSpell.LookupEntryForced( 37438 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1572896 | 1;
		}
		sp = dbcSpell.LookupEntryForced( 37439 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation_high[0] = 32;
			sp->EffectSpellGroupRelation_high[1] = 64;
			//sp->EffectSpellGroupRelation_high[2] = 64; NEED DBC 2.3.2 :P
		}

		//Item Set: Absolution Regalia
		sp = dbcSpell.LookupEntryForced( 38413 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 32768;
		}
		sp = dbcSpell.LookupEntryForced( 38412 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 8192;
		}

		//Item Set: Vestments of Absolution
		sp = dbcSpell.LookupEntryForced( 38410 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 512;
		}
		sp = dbcSpell.LookupEntryForced( 38411 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 4096 ;
		}

		//Item Set: Avatar Raiment
		sp = dbcSpell.LookupEntryForced( 26171 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 64;
		}

		//Item Set: Avatar Regalia
		sp = dbcSpell.LookupEntryForced( 37600 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 6;
		}

		//Item Set: Incarnate Raiment
		sp = dbcSpell.LookupEntryForced( 37568 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 37565 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 4096;
		}

		//Item Set: Incarnate Regalia
		sp = dbcSpell.LookupEntryForced( 37570 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation_high[1] = 256;
		}
		sp = dbcSpell.LookupEntryForced( 37571 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 128;
			sp->EffectSpellGroupRelation[1] = 8388608;
		}

		//Item Set: Malefic Raiment
		sp = dbcSpell.LookupEntryForced( 38393 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1;
			sp->EffectSpellGroupRelation_high[0] = 64;
		}

		//Item Set: Voidheart Raiment
		sp = dbcSpell.LookupEntryForced( 37377 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 5;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->proc_interval = 20;
			sp->EffectTriggerSpell[0] = 37379;
		}
		sp = dbcSpell.LookupEntryForced( 39437 );
		if( sp != NULL )
		{
			sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 5;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->proc_interval = 20;
			sp->EffectTriggerSpell[0] = 37378;
		}
		sp = dbcSpell.LookupEntryForced( 37380 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 2|4;
		}

		//Item Set: Gronnstalker's Armor
		sp = dbcSpell.LookupEntryForced( 38392 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation_high[0] = 1;
		}

		//Item Set: Rift Stalker Armor
		sp = dbcSpell.LookupEntryForced( 37505 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation_high[0] = 1;
		}

		//Item Set: Demon Stalker Armor
		sp = dbcSpell.LookupEntryForced( 37484 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 256;
		}
		sp = dbcSpell.LookupEntryForced( 37485 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 4096;
		}

		//Item Set: Skyshatter Harness
		sp = dbcSpell.LookupEntryForced( 38429 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1048576 | 268435456 | 2147483648UL;
		}

		//Item Set: Skyshatter Raiment
		sp = dbcSpell.LookupEntryForced( 38434 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 256;
		}
		sp = dbcSpell.LookupEntryForced( 38435 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 256;
		}

		//Item Set: Skyshatter Regalia
		sp = dbcSpell.LookupEntryForced( 38436 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1;
		}

		//Item Set: Cataclysm Raiment
		sp = dbcSpell.LookupEntryForced( 37225 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 128;
		}
		sp = dbcSpell.LookupEntryForced( 37227 );
		if( sp != NULL )
		{
			sp->proc_interval = 60000;
			sp->procChance = 100;
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
		}
		sp = dbcSpell.LookupEntryForced( 39950 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 64;
		}

		//Item Set: Cataclysm Regalia
		sp = dbcSpell.LookupEntryForced( 37228 );
		if( sp != NULL )
		{
			sp->procChance = 7;
			sp->procFlags = PROC_ON_CAST_SPELL;
		}
		sp = dbcSpell.LookupEntryForced( 37234 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 128;
		}
		sp = dbcSpell.LookupEntryForced( 37237 );
		if( sp != NULL )
		{
			sp->procChance = 25;
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
		}

		//Item Set: Cataclysm Harness
		sp = dbcSpell.LookupEntryForced( 37239 );
		if( sp != NULL )
		{
			sp->procChance = 2;
			sp->procFlags = PROC_ON_MELEE_ATTACK;
		}
		sp = dbcSpell.LookupEntryForced( 37240 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 128;
		}
		sp = dbcSpell.LookupEntryForced( 37241 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation_high[0] = 512;
		}

		//Item Set: Cyclone Raiment
		sp = dbcSpell.LookupEntryForced( 37210 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 524288;
		}
		sp = dbcSpell.LookupEntryForced( 37211 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation_high[0] = 524288;
		}

		//Item Set: Cyclone Harness
		sp = dbcSpell.LookupEntryForced( 37224 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation_high[0] = 16;
		}
		sp = dbcSpell.LookupEntryForced( 37223 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 536870912UL;
		}

		//Item Set: Cyclone Regalia
		sp = dbcSpell.LookupEntryForced( 37212 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation_high[0] = 256;
		}
		sp = dbcSpell.LookupEntryForced( 37213 );
		if( sp != NULL )
		{
			sp->procChance = 11;
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
		}

		//Item Set: Lightbringer Armor
		sp = dbcSpell.LookupEntryForced( 38421 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation_high[0] = 4096;
		}
		sp = dbcSpell.LookupEntryForced( 38422 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 32;
		}

		//Item Set: Lightbringer Battlegear
		sp = dbcSpell.LookupEntryForced( 38427 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->procChance = 20;
		}
		sp = dbcSpell.LookupEntryForced( 38424 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 128;
		}

		//Item Set: Lightbringer Raiment
		sp = dbcSpell.LookupEntryForced( 38426 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 2147483648UL;
		}
		sp = dbcSpell.LookupEntryForced( 38425 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1073741824UL;
		}

		//Item Set: Crystalforge Armor
		sp = dbcSpell.LookupEntryForced( 37190 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 8;
		}
		sp = dbcSpell.LookupEntryForced( 37191 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}

		//Item Set: Crystalforge Battlegear
		sp = dbcSpell.LookupEntryForced( 37190 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 1024 | 524288 | 1048576 | 536870912UL;
			sp->EffectSpellGroupRelation_high[0] = 1|520;
		}
		sp = dbcSpell.LookupEntryForced( 37195 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 6;
		}

		//Item Set: Crystalforge Raiment
		sp = dbcSpell.LookupEntryForced( 37189 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
			sp->proc_interval = 60000;
		}
		sp = dbcSpell.LookupEntryForced( 43837 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 2147483648UL;
		}
		sp = dbcSpell.LookupEntryForced( 37188 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}

		//Item Set: Justicar Raiment
		sp = dbcSpell.LookupEntryForced( 37182 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation_high[0] = 1;
		}
		sp = dbcSpell.LookupEntryForced( 37183 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation_high[0] = 256;
		}

		//Item Set: Justicar Armor
		sp = dbcSpell.LookupEntryForced( 37184 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation_high[0] = 1024;
			sp->EffectSpellGroupRelation[0] = 134217728;
			sp->EffectSpellGroupRelation_high[1] = 8;
		}
		sp = dbcSpell.LookupEntryForced( 37185 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation_high[0] = 64;
		}

		//Item Set: Justicar Battlegear
		sp = dbcSpell.LookupEntryForced( 37186 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 536870912UL;
		}
		sp = dbcSpell.LookupEntryForced( 37187 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation_high[0] = 520;
		}

		//Item Set: Onslaught Battlegear
		sp = dbcSpell.LookupEntryForced( 38399 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 33554432UL;
			sp->EffectSpellGroupRelation_high[0] = 1024;
		}
		sp = dbcSpell.LookupEntryForced( 38398 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 536870912UL;
		}

		//Item Set: Onslaught Armor
		sp = dbcSpell.LookupEntryForced( 38408 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation_high[0] = 128;
		}
		sp = dbcSpell.LookupEntryForced( 38407 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation_high[0] = 513;
		}

		//Item Set: Destroyer Armor
		sp = dbcSpell.LookupEntryForced( 37525 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM;
			sp->procChance = 7;
		}

		//Item Set: Destroyer Battlegear
		sp = dbcSpell.LookupEntryForced( 37528 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 100;
		}
		sp = dbcSpell.LookupEntryForced( 37535 );
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] = 33554432;
			sp->EffectSpellGroupRelation_high[0] = 1024;
		}

		//Item Set: Warbringer Armor
		sp = dbcSpell.LookupEntryForced( 37516 );
		if( sp != NULL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 100;
		}

		//Item Set: Warbringer Battlegear
		sp = dbcSpell.LookupEntryForced( 37518 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation_high[0] = 4;

		//all Drums 
		sp = dbcSpell.LookupEntryForced( 35474 );
		if( sp != NULL )
			sp->RequiredShapeShift = 0;
		sp = dbcSpell.LookupEntryForced( 35475 );
		if( sp != NULL )
			sp->RequiredShapeShift = 0;
		sp = dbcSpell.LookupEntryForced( 35476 );
		if( sp != NULL )
			sp->RequiredShapeShift = 0;
		sp = dbcSpell.LookupEntryForced( 35477 );
		if( sp != NULL )
			sp->RequiredShapeShift = 0;
		sp = dbcSpell.LookupEntryForced( 35478 );
		if( sp != NULL )
			sp->RequiredShapeShift = 0;

		//this an on equip item spell(2824) :  ice arrow(29501)
		sp = dbcSpell.LookupEntryForced( 29501 );
		if( sp != NULL )
		{
			sp->procChance = 30;//some say it is triggered every now and then
			sp->procFlags = PROC_ON_RANGED_ATTACK;
		}

		//Purify helboar meat
		sp = dbcSpell.LookupEntryForced( 29200 );
		if( sp != NULL )
		{
			sp->Reagent[1] = 0;
			sp->ReagentCount[1] = 0;
			sp->EffectSpellGroupRelation[0] = 23248;
			sp->Effect[0] = 24;
		}

		// - Warrior - Warbringer Armor
		// 2 pieces: You have a chance each time you parry to gain Blade Turning, absorbing 200 damage for 15 sec.
		// SPELL ID = 37514 (http://www.wowhead.com/?spell=37514)

		sp = dbcSpell.LookupEntryForced( 37514 );
		if( sp != NULL )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 37515;
			sp->procChance = 25;
		}
		sp = dbcSpell.LookupEntryForced( 40475 );		// Black temple melee trinket
		if( sp != NULL )
			sp->procChance = 50;

		// Band of the Eternal Champion: reduced proc rate
		sp = dbcSpell.LookupEntryForced( 35080 );
		if( sp != NULL )
			sp->procChance = 5;

		// Band of the Eternal Sage: reduced proc rate
		sp = dbcSpell.LookupEntryForced( 35083 );
		if( sp != NULL )
			sp->procChance = 5;

		// Band of the Eternal Defender: reduced proc rate
		sp = dbcSpell.LookupEntryForced( 35077 );
		if( sp != NULL )
			sp->procChance = 5;

		// Band of the Eternal Restorer: reduced proc rate
		sp = dbcSpell.LookupEntryForced( 35086 );
		if( sp != NULL )
			sp->procChance = 5;




		//////////////////////////////////////////
		// BOSSES								//
		//////////////////////////////////////////

		// Insert boss spell fixes here
		
			// Dark Glare
			sp = dbcSpell.LookupEntryForced( 26029 );
			if( sp != NULL )
				sp->cone_width = 15.0f; // 15 degree cone
}
