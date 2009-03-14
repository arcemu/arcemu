REPLACE INTO creature_names
   (entry, name, subname, info_str, Flags1, type, family, rank, unk4, spelldataid, male_displayid, female_displayid, male_displayid2, female_displayid2, unknown_float1, unknown_float2, civilian, leader)
VALUES
   (123457, 'Mariya', 'The wedding dealer', ' ', 0, 7, 0, 0, 0, 0, 1287, 0, 0, 0, 1, 1, 1, 0),
   (123456, 'Pafnutiy', 'The wedding priest', ' ', 0, 7, 0, 0, 0, 0, 3401, 0, 0, 0, 1, 1, 1, 0);

REPLACE INTO creature_proto
   (entry, minlevel, maxlevel, faction, minhealth, maxhealth, mana, scale, npcflags, attacktime, attacktype, mindamage, maxdamage, respawntime, armor, resistance1, resistance2, resistance3, resistance4, resistance5, resistance6, combat_reach, bounding_radius, auras, boss, money, invisibility_type, death_state, walk_speed, run_speed, fly_speed, extra_a9_flags, spell1, spell2, spell3, spell4, spell_flags)
VALUES
   (123457, 70, 70, 35, 10000000, 10000000, 5000, 1, 4224, 1910, 0, 9000, 15000, 2101, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2.5, 8, 14, 0, 0, 0, 0, 0, 0),
   (123456, 70, 70, 35, 10000000, 10000000, 5000, 1, 1, 1910, 0, 9000, 15000, 2101, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2.5, 8, 14, 0, 0, 0, 0, 0, 0);

REPLACE INTO vendors
   (entry, item, amount, max_amount, inctime, extended_cost)
VALUES
   (123457, 6835, 1, 0, 0, 0),
   (123457, 6834, 1, 0, 0, 0),
   (123457, 6833, 1, 0, 0, 0),
   (123457, 10040, 1, 0, 0, 0);

REPLACE INTO items
   (entry, class, subclass, field4, name1, name2, name3, name4, displayid, quality, flags, buyprice, sellprice, inventorytype, allowableclass, allowablerace, itemlevel, requiredlevel, RequiredSkill, RequiredSkillRank, RequiredSkillSubRank, RequiredPlayerRank1, RequiredPlayerRank2, RequiredFaction, RequiredFactionStanding, `Unique`, maxcount, ContainerSlots, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3, stat_type4, stat_value4, stat_type5, stat_value5, stat_type6, stat_value6, stat_type7, stat_value7, stat_type8, stat_value8, stat_type9, stat_value9, stat_type10, stat_value10, dmg_min1, dmg_max1, dmg_type1, dmg_min2, dmg_max2, dmg_type2, dmg_min3, dmg_max3, dmg_type3, dmg_min4, dmg_max4, dmg_type4, dmg_min5, dmg_max5, dmg_type5, armor, holy_res, fire_res, nature_res, frost_res, shadow_res, arcane_res, delay, ammo_type, range, spellid_1, spelltrigger_1, spellcharges_1, spellcooldown_1, spellcategory_1, spellcategorycooldown_1, spellid_2, spelltrigger_2, spellcharges_2, spellcooldown_2, spellcategory_2, spellcategorycooldown_2, spellid_3, spelltrigger_3, spellcharges_3, spellcooldown_3, spellcategory_3, spellcategorycooldown_3, spellid_4, spelltrigger_4, spellcharges_4, spellcooldown_4, spellcategory_4, spellcategorycooldown_4, spellid_5, spelltrigger_5, spellcharges_5, spellcooldown_5, spellcategory_5, spellcategorycooldown_5, bonding, description, page_id, page_language, page_material, quest_id, lock_id, lock_material, sheathID, randomprop, unk203_1, block, itemset, MaxDurability, ZoneNameID, mapid, bagfamily, TotemCategory, socket_color_1, unk201_3, socket_color_2, unk201_5, socket_color_3, unk201_7, socket_bonus, GemProperties, ReqDisenchantSkill, unk2)
VALUES
   (313131, 4, 0, -1, 'The wedding Ring', '', '', '', 9823, 2, 524288, 0, 0, 11, -1, -1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, '', 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0);
