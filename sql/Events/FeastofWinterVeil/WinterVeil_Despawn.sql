/*
======================================
Title: Feast of Winter Veil Despawn
Author: Krzysiek
======================================
*/

/* DESPAWN CREATURES */
DELETE FROM `creature_spawns` WHERE `entry` IN (13444, 15745, 13433, 13434, 15732, 19148);

/* GAMEOBJECT QUEST FINISHER REMOVE */
DELETE FROM `gameobject_quest_finisher` WHERE `id` IN (180743, 180746, 180747, 180748, 180793, 187236);

/* GAMEOBJECT QUEST STARTER REMOVE */
DELETE FROM `gameobject_quest_starter` WHERE `id` IN (180743, 180746, 180747, 180748, 180793, 187236);

/* DESPAWN GAMEOBJECTS */
DELETE FROM `gameobject_spawns` WHERE `Entry` IN (178425, 178667, 178671, 178438, 178645, 180844, 178554, 178764, 178765, 178746, 178437, 178649, 178428, 178429, 178430, 178431, 178432, 178433, 178557, 2722, 187235, 400057, 555558);

/* REMOVE LOOTS */
DELETE FROM `loot_creatures` WHERE `itemid` IN (21525, 21524);
