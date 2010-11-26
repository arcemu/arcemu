/*
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2010 <http://www.ArcEmu.org/>
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

/////////////////////////////////////////////////
//  Admin Chat Commands
//

#include "StdAfx.h"
#include "ObjectMgr.h"
#include "Master.h"

void ParseBanArgs(char* args, char** BanDuration, char** BanReason)
{
	// Usage: .ban character <char> [duration] [reason]
	//        .ban ip <ipaddr> [duration] [reason]
	//        .ban account <acct> [duration] [reason]
	//        .ban all <char> [duration] [reason]
	// Duration must be a number optionally followed by a character representing the calendar subdivision to use (h>hours, d>days, w>weeks, m>months, y>years, default minutes)
	// Lack of duration results in a permanent ban.
	char* pBanDuration = strchr(args, ' ');
	char* pReason = NULL;
	if(pBanDuration != NULL)
	{
		if(isdigit(*(pBanDuration+1))) // this is the duration of the ban
		{
			*pBanDuration = 0; // NULL-terminate the first string (character/account/ip)
			++pBanDuration; // point to next arg
			pReason = strchr(pBanDuration+1, ' ');
			if(pReason != NULL) // BanReason is OPTIONAL
			{
				*pReason = 0; // BanReason was given, so NULL-terminate the duration string
				++pReason; // and point to the ban reason
			}
		}
		else // no duration was given (didn't start with a digit) - so this arg must be ban reason and duration defaults to permanent
		{
			pReason = pBanDuration;
			pBanDuration = NULL;
			*pReason = 0;
			++pReason;
		}
	}
	*BanDuration = pBanDuration;
	*BanReason = pReason;
}

int32 GetSpellIDFromLink(const char* spelllink)
{
	if(spelllink== NULL)
		return 0;

	const char* ptr = strstr(spelllink, "|Hspell:");
	if(ptr == NULL)
	{
		return 0;
	}

	return atol(ptr+8); // spell id is just past "|Hspell:" (8 bytes)
}

bool ChatHandler::HandleWorldPortCommand(const char* args, WorldSession *m_session)
{
	float x, y, z, o = 0;
	uint32 mapid;
	if(sscanf(args, "%u %f %f %f %f", (unsigned int*)&mapid, &x, &y, &z, &o) < 4)
		return false;

	if(x >= _maxX || x <= _minX || y <= _minY || y >= _maxY)
		return false;

	LocationVector vec(x, y, z, o);
	m_session->GetPlayer()->SafeTeleport(mapid, 0, vec);
	return true;
}

bool ChatHandler::HandlePortToCreatureSpawnCommand(const char* args, WorldSession *m_session)
{
	if( !*args )
	{
		return false;
	}

	uint32 spawn_id, spawn_map;
	float spawn_x, spawn_y, spawn_z;

	if( sscanf(args, "%u", (unsigned int*)&spawn_id) != 1 )
	{
		return false;
	}
	QueryResult* QR = WorldDatabase.Query("SELECT * FROM creature_spawns WHERE id=%u",spawn_id);
	if( !QR )
	{
		RedSystemMessage(m_session, "No spawn found with id %u.", spawn_id);
		return true;
	}
	spawn_map = QR->Fetch()[2].GetUInt32();
	spawn_x = QR->Fetch()[3].GetFloat();
	spawn_y = QR->Fetch()[4].GetFloat();
	spawn_z = QR->Fetch()[5].GetFloat();
	LocationVector vec(spawn_x, spawn_y, spawn_z);
	m_session->GetPlayer()->SafeTeleport(spawn_map, 0, vec);
	delete QR;
	return true;
}

bool ChatHandler::HandlePortToGameObjectSpawnCommand(const char* args, WorldSession *m_session)
{
	if( !*args )
	{
		return false;
	}

	uint32 spawn_id, spawn_map;
	float spawn_x, spawn_y, spawn_z;

	if( sscanf(args, "%u", (unsigned int*)&spawn_id) != 1 )
	{
		return false;
	}
	QueryResult* QR = WorldDatabase.Query("SELECT * FROM gameobject_spawns WHERE id=%u",spawn_id);
	if( !QR )
	{
		RedSystemMessage(m_session, "No spawn found with id %u.", spawn_id);
		return true;
	}
	spawn_map = QR->Fetch()[2].GetUInt32();
	spawn_x = QR->Fetch()[3].GetFloat();
	spawn_y = QR->Fetch()[4].GetFloat();
	spawn_z = QR->Fetch()[5].GetFloat();
	LocationVector vec(spawn_x, spawn_y, spawn_z);
	m_session->GetPlayer()->SafeTeleport(spawn_map, 0, vec);
	delete QR;
	return true;
}

bool ChatHandler::HandleClearCooldownsCommand(const char *args, WorldSession *m_session)
{
	uint32 guid = (uint32)m_session->GetPlayer()->GetSelection();
	Player *plr = getSelectedChar(m_session, true);

	if(!plr)
	{
		plr = m_session->GetPlayer();
		SystemMessage(m_session, "Auto-targeting self.");
	}
	if(!plr) return false;

	if(plr != m_session->GetPlayer())
	{
		sGMLog.writefromsession(m_session, "Cleared all cooldowns for player %s", plr->GetName());
	}

	if(plr->getClass()==WARRIOR)
	{
		plr->ClearCooldownsOnLine(26, guid);
		plr->ClearCooldownsOnLine(256, guid);
		plr->ClearCooldownsOnLine(257 , guid);
		BlueSystemMessage(m_session, "Cleared all Warrior cooldowns.");
		return true;
	}
	if(plr->getClass()==PALADIN)
	{
		plr->ClearCooldownsOnLine(594, guid);
		plr->ClearCooldownsOnLine(267, guid);
		plr->ClearCooldownsOnLine(184, guid);
		BlueSystemMessage(m_session, "Cleared all Paladin cooldowns.");
		return true;
	}
	if(plr->getClass()==HUNTER)
	{
		plr->ClearCooldownsOnLine(50, guid);
		plr->ClearCooldownsOnLine(51, guid);
		plr->ClearCooldownsOnLine(163, guid);
		BlueSystemMessage(m_session, "Cleared all Hunter cooldowns.");
		return true;
	}
	if(plr->getClass()==ROGUE)
	{
		plr->ClearCooldownsOnLine(253, guid);
		plr->ClearCooldownsOnLine(38, guid);
		plr->ClearCooldownsOnLine(39, guid);
		BlueSystemMessage(m_session, "Cleared all Rogue cooldowns.");
		return true;
	}
	if(plr->getClass()==PRIEST)
	{
		plr->ClearCooldownsOnLine(56, guid);
		plr->ClearCooldownsOnLine(78, guid);
		plr->ClearCooldownsOnLine(613, guid);
		BlueSystemMessage(m_session, "Cleared all Priest cooldowns.");
		return true;
	}
	if(plr->getClass()==DEATHKNIGHT)
	{
		plr->ClearCooldownsOnLine(770, guid);
		plr->ClearCooldownsOnLine(771, guid);
		plr->ClearCooldownsOnLine(772, guid);
		BlueSystemMessage(m_session, "Cleared all Death Knight cooldowns.");
		return true;
	}
	if(plr->getClass()==SHAMAN)
	{
		plr->ClearCooldownsOnLine(373, guid);
		plr->ClearCooldownsOnLine(374, guid);
		plr->ClearCooldownsOnLine(375, guid);
		BlueSystemMessage(m_session, "Cleared all Shaman cooldowns.");
		return true;
	}
	if(plr->getClass()==MAGE)
	{
		plr->ClearCooldownsOnLine(6, guid);
		plr->ClearCooldownsOnLine(8, guid);
		plr->ClearCooldownsOnLine(237, guid);
		BlueSystemMessage(m_session, "Cleared all Mage cooldowns.");
		return true;
	}
	if(plr->getClass()==WARLOCK)
	{
		plr->ClearCooldownsOnLine(355, guid);
		plr->ClearCooldownsOnLine(354, guid);
		plr->ClearCooldownsOnLine(593, guid);
		BlueSystemMessage(m_session, "Cleared all Warlock cooldowns.");
		return true;
	}
	if(plr->getClass()==DRUID)
	{
		plr->ClearCooldownsOnLine(573, guid);
		plr->ClearCooldownsOnLine(574, guid);
		plr->ClearCooldownsOnLine(134, guid);
		BlueSystemMessage(m_session, "Cleared all Druid cooldowns.");
		return true;
	}
	return true;
}

bool ChatHandler::HandleLearnCommand(const char* args, WorldSession *m_session)
{
	if( !*args )
	{
		return false;
	}

	Player *plr = getSelectedChar( m_session, true );
	if( !plr )
	{
		plr = m_session->GetPlayer();
		SystemMessage(m_session, "Auto-targeting self.");
	}

	if( !plr )
	{
		return false;
	}

	if( stricmp( args, "all" ) == 0 )
	{
		sGMLog.writefromsession(m_session, "taught %s all spells.", plr->GetName());
		SystemMessage(m_session, "Taught %s all spells.", plr->GetName());
		static uint32 spellarray[DRUID+1][512] = {
			{ 0 }, // N/A
			{ 6673, 2457, 78, 100, 772, 6343, 1715, 284, 2687, 71, 6546, 7386, 355, 5242, 7384, 72, 1160, 6572, 285, 694, 2565, 676, 8198, 845, 6547, 20230, 12678, 6192, 5246, 7405, 6190, 5308, 1608, 6574, 1161, 6178, 871, 8204, 2458, 7369, 20252, 6548, 1464, 11549, 18499, 20658, 11564, 11554, 7379, 8380, 1680, 6552, 8820, 8205, 11608, 20660, 11565, 11572, 23922, 11550, 11555, 11600, 11578, 11604, 11596, 20661, 11566, 23923, 11580, 11609, 1719, 11573, 11551, 11556, 11601, 23924, 11605, 20662, 11567, 11597, 11581, 25289, 20569, 25286, 11574, 25288, 23925, 25241, 25202, 34428, 25269, 23920, 25234, 29707, 25258, 25225, 25264, 25231, 469, 25208, 2048, 25242, 25203, 25236, 30324, 3411, 30357, 30356, 46845, 47519, 47449, 47470, 47501, 47439, 47474, 55694, 47487, 47450, 47465, 47520, 47467, 47436, 47502, 47437, 47475, 47440, 47471, 57755, 57823, 47488, 0 }, // Warrior
			{ 465, 635, 21084, 19740, 20271, 498, 639, 853, 1152, 10290, 1022, 633, 19834, 53408, 7328, 19742, 647, 31789, 62124, 7294, 25780, 1044, 31785, 26573, 643, 879, 19750, 5502, 19835, 19746, 1026, 20164, 19850, 5588, 5599, 10322, 10326, 19939, 1038, 10298, 5614, 53407, 19876, 20116, 10291, 19752, 1042, 2800, 20165, 19836, 19888, 19852, 642, 19940, 5615, 19891, 10324, 10299, 10278, 3472, 20166, 20922, 1032, 5589, 19895, 19837, 4987, 19941, 19853, 10312, 19897, 24275, 6940, 10328, 10300, 19899, 20772, 20923, 10292, 19942, 2812, 10310, 19838, 10313, 25782, 24274, 19896, 19854, 25894, 10308, 10329, 19898, 10301, 19943, 25291, 25290, 20924, 10293, 10314, 19900, 25898, 25916, 25899, 25918, 24239, 25292, 10318, 53651, 20773, 32223, 27135, 27151, 27142, 27143, 27137, 27150, 33776, 27138, 27152, 27180, 27139, 27154, 31884, 27140, 27173, 27149, 27153, 27141, 27136, 48935, 54428, 48937, 48816, 48949, 48931, 48800, 48933, 48941, 48784, 48805, 48818, 48781, 53600, 54043, 48943, 48936, 48945, 48938, 48947, 48817, 48788, 48932, 48942, 48801, 48785, 48934, 48950, 48819, 48806, 48782, 53601, 61411, 0 }, // Paladin
			{ 75, 2973, 1494, 13163, 1978, 3044, 1130, 5116, 14260, 13165, 883, 2641, 6991, 982, 13549, 1515, 19883, 14281, 20736, 136, 2974, 6197, 1002, 1513, 13795, 1495, 14261, 14318, 2643, 13550, 19884, 14282, 5118, 34074, 781, 1499, 3111, 14323, 3043, 1462, 14262, 19885, 14302, 3045, 13551, 19880, 14283, 14319, 13809, 3661, 13161, 5384, 14269, 14288, 14326, 1543, 14263, 19878, 13813, 13552, 14284, 14303, 3662, 3034, 14320, 13159, 14310, 14324, 14264, 19882, 1510, 14289, 13553, 14285, 14316, 13542, 14270, 20043, 14304, 14327, 14321, 14265, 13554, 56641, 19879, 14294, 14286, 13543, 14317, 14290, 20190, 14305, 14266, 14322, 14325, 14271, 13555, 14295, 14287, 25296, 19263, 14311, 13544, 25294, 25295, 19801, 27025, 34120, 27014, 27023, 34026, 27021, 27016, 27022, 27044, 27045, 27046, 34600, 27019, 34477, 36916, 49066, 53351, 48995, 49051, 49055, 49044, 49000, 61846, 48989, 49047, 58431, 61005, 53271, 49071, 53338, 49067, 48996, 49052, 49056, 49045, 49001, 61847, 60192, 61006, 48990, 53339, 49048, 58434, 0 }, // Hunter
			{ 2098, 1752, 1784, 53, 921, 1776, 1757, 5277, 6760, 6770, 5171, 2983, 2589, 1766, 8647, 703, 1758, 6761, 1966, 1804, 8676, 2590, 51722, 1943, 1785, 1725, 8631, 1759, 1856, 2836, 6762, 8724, 1833, 8649, 2591, 6768, 8639, 2070, 1842, 8632, 408, 1760, 8623, 8725, 2094, 8696, 8721, 8650, 8640, 8633, 8621, 8624, 8637, 1860, 1786, 11267, 6774, 1857, 11279, 11273, 11197, 11289, 17347, 11293, 11299, 11297, 11268, 26669, 8643, 11280, 11303, 11274, 11290, 11294, 11300, 11198, 11269, 17348, 11305, 11281, 25300, 31016, 25302, 11275, 1787, 26839, 32645, 26861, 26889, 26679, 26865, 27448, 27441, 31224, 26866, 26863, 26867, 32684, 48689, 48673, 26884, 26864, 5938, 26862, 51724, 48658, 48667, 48656, 57992, 48671, 48690, 48675, 57934, 48674, 48637, 48669, 48659, 48668, 48672, 48691, 48657, 57993, 51723, 48676, 48660, 48638, 0 }, // Rogue
			{ 2050, 1243, 585, 2052, 589, 17, 591, 586, 139, 2053, 8092, 2006, 594, 588, 1244, 592, 528, 8122, 6074, 598, 2054, 8102, 527, 600, 970, 2944, 6346, 2061, 14914, 15237, 7128, 453, 6075, 9484, 2055, 8103, 2096, 2010, 984, 15262, 8129, 1245, 3747, 9472, 6076, 992, 19276, 6063, 15430, 8104, 8124, 15263, 602, 605, 6065, 596, 976, 1004, 552, 9473, 6077, 6064, 1706, 8105, 10880, 2767, 19277, 988, 15264, 15431, 2791, 6066, 9474, 6078, 6060, 2060, 1006, 8106, 996, 9485, 15265, 10898, 10888, 10957, 10892, 19278, 10915, 27799, 10909, 10927, 10963, 10945, 10881, 10933, 15266, 10937, 10899, 21562, 10916, 10951, 10960, 10928, 10893, 19279, 10964, 27800, 10946, 15267, 10900, 10934, 10917, 27683, 10890, 10929, 10958, 10965, 10947, 20770, 10894, 19280, 25314, 15261, 27801, 60931, 10952, 10938, 10901, 21564, 10961, 25316, 27681, 25315, 10955, 25233, 25363, 32379, 25210, 25372, 32546, 25217, 25221, 25367, 25384, 34433, 25235, 25467, 25213, 25331, 25308, 33076, 25435, 25433, 25431, 25375, 25364, 32375, 25389, 25218, 25392, 39374, 32999, 25222, 32996, 25368, 48040, 48119, 48134, 48299, 48070, 48062, 48126, 48112, 48122, 48075, 48077, 48045, 48065, 48067, 48157, 48124, 48072, 48169, 48168, 48170, 48120, 48063, 48135, 48171, 48300, 48071, 48127, 48113, 48123, 47951, 48078, 53023, 48161, 48066, 48162, 48074, 48068, 48158, 48125, 0 }, // Priest
			{ 53341, 53331, 53343, 54447, 53342, 54446, 53323, 53344, 62158, 48778, 48266, 45902, 52375, 50977, 49576, 49142, 45477, 45462, 53428, 49998, 50842, 46584, 48263, 47528, 48721, 45524, 49926, 47476, 51325, 43265, 49917, 49896, 49020, 3714, 49892, 48792, 51426, 49999, 49927, 45529, 56222, 57330, 49918, 49913, 49939, 48743, 49936, 49903, 51423, 56815, 55265, 48707, 49893, 51427, 49928, 49914, 51326, 45463, 49919, 48265, 49940, 61999, 49937, 49904, 51424, 55270, 49929, 51428, 49915, 51327, 49923, 47568, 57623, 49920, 49894, 49941, 49909, 51425, 51429, 55271, 49916, 42650, 49930, 51328, 49938, 49895, 49924, 49921, 0 }, // Death Knight
			{ 30669, 30670, 30671, 331, 403, 8017, 8042, 8071, 2484, 332, 8044, 529, 324, 8018, 5730, 8050, 8024, 3599, 8075, 2008, 1535, 547, 370, 8045, 548, 8154, 526, 325, 8019, 57994, 8052, 8027, 913, 6390, 8143, 8056, 8033, 2645, 5394, 8004, 915, 6363, 52127, 2870, 8498, 8166, 131, 20609, 8046, 8181, 939, 905, 10399, 8155, 8160, 6196, 8030, 943, 8190, 5675, 8184, 8053, 8227, 8038, 8008, 6391, 52129, 546, 556, 51730, 8177, 6375, 10595, 20608, 6364, 36936, 8232, 421, 8499, 959, 6041, 945, 8012, 8512, 8058, 6495, 10406, 52131, 20610, 10412, 16339, 8010, 10585, 10495, 8170, 8249, 10478, 10456, 10391, 6392, 8161, 1064, 930, 51988, 10447, 6377, 8005, 8134, 6365, 8235, 52134, 11314, 10537, 10466, 10392, 10600, 10407, 10622, 16341, 10472, 10586, 10496, 20776, 2860, 10413, 10526, 16355, 10395, 10431, 10427, 52136, 51991, 10462, 15207, 10437, 10486, 11315, 10448, 10467, 10442, 10623, 10479, 10408, 52138, 10605, 16342, 10396, 15208, 10432, 10587, 10497, 10538, 16387, 10473, 16356, 10428, 20777, 10414, 51992, 29228, 10463, 25357, 10468, 10601, 10438, 25361, 16362, 25422, 25546, 25448, 24398, 25439, 25391, 25469, 25508, 25489, 3738, 25552, 25570, 25528, 2062, 25500, 25420, 25557, 25560, 25449, 25525, 25423, 2894, 25563, 25464, 25505, 25590, 25454, 25567, 25574, 25533, 33736, 25442, 51993, 25547, 25457, 25396, 25472, 25509, 58649, 58785, 58794, 58755, 58771, 58699, 58580, 58801, 49275, 49235, 49237, 58731, 58751, 55458, 49270, 49230, 61649, 58737, 49232, 58652, 58741, 49272, 51505, 49280, 58746, 58703, 58581, 57622, 58789, 58795, 58756, 58773, 57960, 58803, 49276, 49236, 58734, 58582, 58753, 49231, 49238, 49277, 55459, 49271, 51994, 61657, 58739, 49233, 58656, 58790, 58745, 58796, 58757, 49273, 51514, 60043, 49281, 58774, 58749, 58704, 58643, 58804, 0 }, // Shaman
			{ 1459, 133, 168, 5504, 116, 587, 2136, 143, 5143, 205, 118, 5505, 7300, 122, 597, 604, 145, 130, 1449, 1460, 2137, 837, 5144, 2120, 1008, 3140, 475, 1953, 10, 5506, 12051, 543, 54648, 7301, 7322, 1463, 12824, 8437, 990, 2138, 6143, 2948, 5145, 2139, 8450, 8400, 2121, 120, 865, 8406, 1461, 6141, 759, 8494, 8444, 8455, 8438, 6127, 8412, 8457, 8401, 7302, 45438, 8416, 6129, 8422, 8461, 8407, 8492, 6117, 8445, 8427, 8451, 8402, 8495, 8439, 3552, 8413, 8408, 8417, 10138, 8458, 8423, 6131, 7320, 12825, 8446, 10169, 10156, 10159, 10144, 10148, 8462, 10185, 10179, 10191, 10201, 10197, 13031, 22782, 10205, 10211, 10053, 10173, 10149, 10215, 10160, 10139, 10223, 10180, 10219, 10186, 10145, 10177, 13032, 10192, 10206, 10170, 10202, 10199, 10150, 10230, 23028, 10157, 10212, 33041, 10216, 10181, 10161, 10054, 13033, 22783, 10207, 25345, 10187, 28612, 10140, 10174, 10225, 10151, 28609, 25304, 10220, 10193, 61305, 28272, 12826, 61025, 28271, 27078, 27080, 25306, 30482, 27130, 27075, 27071, 30451, 33042, 27086, 27134, 27087, 37420, 27073, 27070, 30455, 33944, 27088, 27085, 27101, 66, 27131, 33946, 38699, 27128, 27072, 27124, 27125, 27127, 27082, 27126, 38704, 33717, 27090, 33043, 27079, 38692, 32796, 38697, 33405, 43987, 27074, 30449, 42894, 43023, 43045, 53140, 42930, 42925, 42913, 43019, 42858, 42939, 42872, 42832, 53142, 42843, 42955, 42949, 42917, 42841, 44614, 43038, 42896, 42920, 43015, 43017, 42985, 43010, 42833, 42914, 42859, 42846, 42931, 42926, 43012, 42842, 43008, 43024, 43020, 43046, 42897, 43002, 42921, 42995, 42940, 42956, 61316, 61024, 42950, 42873, 47610, 43039, 55342, 58659, 53142, 0 }, // Mage
			{ 59671, 687, 348, 686, 688, 172, 702, 1454, 695, 980, 5782, 6201, 696, 1120, 707, 697, 1108, 755, 705, 6222, 704, 689, 1455, 5697, 693, 1014, 5676, 706, 3698, 1094, 5740, 698, 1088, 712, 6202, 6205, 699, 126, 6223, 5138, 8288, 5500, 1714, 132, 1456, 17919, 710, 6366, 6217, 7658, 3699, 1106, 20752, 1086, 709, 1098, 5784, 1949, 2941, 691, 1490, 7646, 6213, 6229, 7648, 5699, 6219, 17920, 17951, 2362, 3700, 11687, 7641, 11711, 7651, 8289, 20755, 11733, 5484, 11665, 7659, 11707, 6789, 11683, 17921, 11739, 11671, 11725, 11693, 11659, 17952, 11729, 11721, 11699, 11688, 11677, 18647, 17727, 11712, 6353, 20756, 11719, 17925, 11734, 11667, 1122, 17922, 11708, 11675, 11694, 11660, 11740, 11672, 11700, 11684, 17928, 17953, 11717, 6215, 11689, 17924, 11730, 11713, 17926, 11726, 11678, 17923, 25311, 20757, 17728, 603, 11722, 11735, 54785, 11695, 11668, 25309, 50589, 18540, 11661, 50581, 28610, 27224, 23161, 27219, 28176, 25307, 29722, 27211, 27216, 27210, 27250, 28172, 29858, 27218, 27217, 27259, 27230, 27223, 27213, 27222, 29893, 27226, 27228, 30909, 27220, 28189, 27215, 27212, 27209, 27238, 30910, 27260, 32231, 30459, 27243, 30545, 47812, 50511, 47886, 61191, 47819, 47890, 47871, 47863, 47859, 60219, 47892, 47837, 47814, 47808, 47810, 47835, 47897, 47824, 47884, 47793, 47856, 47813, 47855, 47888, 47865, 47860, 47857, 47823, 47891, 47878, 47864, 57595, 47893, 47820, 47815, 47809, 60220, 47867, 47889, 48018, 48020, 47811, 47838, 57946, 58887, 47836, 61290, 47825, 0 }, // Warlock
			{ 0 }, // N/A
			{ 5185, 1126, 5176, 8921, 774, 467, 5177, 339, 5186, 5487, 99, 6795, 5232, 6807, 8924, 16689, 1058, 18960, 5229, 8936, 50769, 5211, 8946, 5187, 782, 5178, 1066, 8925, 1430, 779, 1062, 770, 16857, 2637, 6808, 16810, 8938, 768, 1082, 1735, 5188, 6756, 5215, 20484, 1079, 2912, 8926, 2090, 5221, 2908, 5179, 1822, 8939, 2782, 50768, 780, 1075, 5217, 2893, 1850, 5189, 6809, 8949, 5209, 3029, 8998, 5195, 8927, 16811, 2091, 9492, 6798, 778, 17390, 5234, 20739, 8940, 6800, 740, 783, 5180, 9490, 22568, 6778, 6785, 5225, 8972, 8928, 1823, 3627, 8950, 769, 8914, 22842, 9005, 8941, 50767, 9493, 6793, 5201, 5196, 8903, 18657, 16812, 8992, 8955, 6780, 9000, 9634, 20719, 22827, 16914, 29166, 8907, 8929, 6783, 20742, 8910, 8918, 9747, 9749, 17391, 9745, 6787, 9750, 8951, 22812, 9758, 1824, 9752, 9754, 9756, 8983, 9821, 9833, 9823, 9839, 9829, 8905, 9849, 9852, 22828, 16813, 9856, 50766, 9845, 21849, 9888, 17401, 9884, 9880, 9866, 20747, 9875, 9862, 9892, 9898, 9834, 9840, 9894, 9907, 17392, 9904, 9857, 9830, 9901, 9908, 9910, 9912, 22829, 9889, 9827, 9850, 9853, 18658, 33986, 33982, 9881, 9835, 17329, 9867, 9841, 9876, 31709, 31018, 21850, 25297, 17402, 9885, 9913, 20748, 9858, 25299, 50765, 9896, 25298, 9846, 9863, 53223, 27001, 26984, 26998, 26978, 22570, 24248, 26987, 26981, 33763, 27003, 26997, 26992, 33357, 26980, 26993, 27011, 33745, 27006, 27005, 27000, 26996, 27008, 26986, 26989, 33943, 33987, 33983, 27009, 27004, 26979, 26994, 26982, 50764, 26985, 33786, 26991, 27012, 26990, 26988, 27002, 26995, 26983, 53225, 48559, 48442, 49799, 40120, 62078, 50212, 48576, 48450, 48573, 48464, 48561, 48569, 48567, 48479, 48578, 48377, 49802, 53307, 48459, 48563, 48565, 48462, 48440, 52610, 48571, 48446, 53226, 48575, 48476, 48475, 48560, 49803, 48443, 48562, 53308, 48577, 53312, 48574, 48465, 48570, 48378, 48480, 48579, 48477, 50213, 48461, 48470, 48467, 48568, 48451, 48564, 48566, 48469, 48463, 50464, 48441, 50763, 49800, 48572, 48447, 53227, 61384, 0 }, // Druid
 		};

		uint32 c = plr->getClass();
		for( uint32 i = 0; spellarray[c][i] != 0; ++i )
		{
			plr->addSpell(spellarray[c][i]);
		}

		static uint32 paladinspellarray[RACE_DRAENEI + 1][5] = {
			{ 0 },		// RACE 0
			{ 23214, 13819, 31801, 53720, 0 },		// HUMAN  Charger, Warhorse, Seal of Vengeance, Seal of Martyr
			{ 0 },		// ORC
			{ 23214, 13819, 31801, 53720, 0 },		// DWARF  Charger, Warhorse, Seal of Vengeance, Seal of Martyr
			{ 0 },		// NIGHTELF
			{ 0 },		// UNDEAD
			{ 0 },		// TAUREN
			{ 0 },		// GNOME
			{ 0 },		// TROLL
			{ 34767, 34769, 31892, 53736, 0 },		// BLOODELF  Summon Charger, Summon Warhorse, Seal of Blood, Seal of Corruption
			{ 23214, 13819, 31801, 53720, 0 },		// DRAENEI  Charger, Warhorse, Seal of Vengeance, Seal of Martyr
		};

		static uint32 shamanspellarray[RACE_DRAENEI+1][2] = {
			{ 0 },		// RACE 0
			{ 0 },		// HUMAN
			{ 2825, 0 },		// ORC Bloodlust
			{ 0 },		// DWARF
			{ 0 },		// NIGHTELF
			{ 0 },		// UNDEAD
			{ 2825, 0 },		// TAUREN Bloodlust
			{ 0 },		// GNOME
			{ 2825, 0 },		// TROLL Bloodlust
			{ 0 },		// BLOODELF
			{ 32182, 0 },		// DRAENEI Heroism
		};

		static uint32 magespellarray[RACE_DRAENEI+1][13] = {
			{ 0 },		// RACE 0
			{ 3561, 3562, 3565, 10059, 11416, 11419, 32266, 32271, 33690, 33691, 49359, 49360, 0 },		// HUMAN
			{ 3563, 3566, 3567, 11417, 11418, 11420, 32267, 32272, 35715, 35717, 49358, 49361, 0 },		// ORC
			{ 3561, 3562, 3565, 10059, 11416, 11419, 32266, 32271, 33690, 33691, 49359, 49360, 0 },		// DWARF
			{ 3561, 3562, 3565, 10059, 11416, 11419, 32266, 32271, 33690, 33691, 49359, 49360, 0 },		// NIGHTELF
			{ 3563, 3566, 3567, 11417, 11418, 11420, 32267, 32272, 35715, 35717, 49358, 49361, 0 },		// UNDEAD
			{ 3563, 3566, 3567, 11417, 11418, 11420, 32267, 32272, 35715, 35717, 49358, 49361, 0 },		// TAUREN
			{ 3561, 3562, 3565, 10059, 11416, 11419, 32266, 32271, 33690, 33691, 49359, 49360, 0 },		// GNOME
			{ 3563, 3566, 3567, 11417, 11418, 11420, 32267, 32272, 35715, 35717, 49358, 49361, 0 },		// TROLL
			{ 3563, 3566, 3567, 11417, 11418, 11420, 32267, 32272, 35715, 35717, 49358, 49361, 0 },		// BLOODELF
			{ 3561, 3562, 3565, 10059, 11416, 11419, 32266, 32271, 33690, 33691, 49359, 49360, 0 },		// DRAENEI
		};

		uint32 r = plr->getRace();
		switch (c)
		{
		case PALADIN:
			for( uint32 i = 0; paladinspellarray[r][i] != 0; ++i )
			{
				plr->addSpell( paladinspellarray[r][i] );
			}
			break;
		case MAGE:
			for( uint32 i = 0; magespellarray[r][i] != 0; ++i )
			{
				plr->addSpell( magespellarray[r][i] );
			}
			break;
		case SHAMAN:
			for( uint32 i = 0; shamanspellarray[r][i] != 0; ++i )
			{
				plr->addSpell( shamanspellarray[r][i] );
			}
			break;
		}
		return true;
	}

	uint32 spell = atol((char*)args);
	if( spell == 0 )
	{
		spell = GetSpellIDFromLink( args );
	}

	SpellEntry * sp = dbcSpell.LookupEntryForced( spell );
	if( !sp )
	{
		SystemMessage(m_session, "Invalid spell %u", spell);
		return true;
	}

	if( !plr->GetSession()->HasGMPermissions() && (	sp->Effect[0] == SPELL_EFFECT_INSTANT_KILL || sp->Effect[1] == SPELL_EFFECT_INSTANT_KILL || sp->Effect[2] == SPELL_EFFECT_INSTANT_KILL) )
	{
		SystemMessage(m_session, "don't be an idiot and teach players instakill spells. this action has been logged.");
		return true;
	}

	if( plr->HasSpell( spell ) ) // check to see if char already knows
	{
		std::string OutStr = plr->GetName();
		OutStr += " already knows that spell.";

		SystemMessage(m_session, OutStr.c_str());
		return true;
	}

	plr->addSpell(spell);
	sGMLog.writefromsession(m_session, "Taught %s spell %u", plr->GetName(), spell);
	BlueSystemMessageToPlr(plr, "%s taught you Spell %u", m_session->GetPlayer()->GetName(), spell);
	GreenSystemMessage(m_session, "Taught %s Spell %u", plr->GetName(), spell);

	return true;
}

bool ChatHandler::HandleReviveCommand(const char* args, WorldSession *m_session)
{
	Player* SelectedPlayer = getSelectedChar(m_session, true);
	if(!SelectedPlayer)
		return true;

	SelectedPlayer->SetMovement(MOVE_UNROOT, 1);
	SelectedPlayer->ResurrectPlayer();
	SelectedPlayer->SetHealth(SelectedPlayer->GetMaxHealth());
	SelectedPlayer->SetPower( POWER_TYPE_MANA, SelectedPlayer->GetMaxPower( POWER_TYPE_MANA ));
	SelectedPlayer->SetPower( POWER_TYPE_ENERGY, SelectedPlayer->GetMaxPower( POWER_TYPE_ENERGY ) );


	if ( SelectedPlayer != m_session->GetPlayer() )
		sGMLog.writefromsession( m_session, "revived player %s", SelectedPlayer->GetName() );

	return true;
}

bool ChatHandler::HandleExploreCheatCommand(const char* args, WorldSession *m_session)
{
	if (!*args)
		return false;

	Player *chr = getSelectedChar(m_session);
	if (chr == NULL)
	{
		chr = m_session->GetPlayer();
		SystemMessage(m_session, "Auto-targeting self.");
	}

	if (stricmp(args, "on") == 0)
	{
		SystemMessage(m_session, "%s has explored all zones now.", chr->GetName());
		SystemMessage(m_session, "%s has explored all zones for you.", m_session->GetPlayer()->GetName());
		sGMLog.writefromsession( m_session, "explored all zones for player %s", chr->GetName() );
	}
	else if(stricmp(args, "off") == 0)
	{
		SystemMessage(m_session, "%s has no more explored zones.", chr->GetName());
		SystemMessage(m_session, "%s has hidden all zones from you.", m_session->GetPlayer()->GetName());
		sGMLog.writefromsession( m_session, "hid all zones for player %s", chr->GetName() );
	}
	else
		return false;

	for (uint8 i= 0; i<PLAYER_EXPLORED_ZONES_LENGTH; ++i)
	{
		if (stricmp(args, "on") == 0)
		{
			chr->SetFlag(PLAYER_EXPLORED_ZONES_1+i,0xFFFFFFFF);
		}
		else if(stricmp(args, "off") == 0)
		{
			chr->RemoveFlag(PLAYER_EXPLORED_ZONES_1+i,0xFFFFFFFF);
		}
	}
#ifdef ENABLE_ACHIEVEMENTS
	chr->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA); // update
#endif
	return true;
}

bool ChatHandler::HandleBanCharacterCommand(const char* args, WorldSession *m_session)
{
	if(!*args)
		return false;

	char* pCharacter = (char*)args;
	PlayerInfo * pInfo = NULL;
	char* pReason;
	char* pDuration;
	int32 BanTime = 0;
	ParseBanArgs(pCharacter, &pDuration, &pReason);
	if(pDuration != NULL)
	{
		BanTime = GetTimePeriodFromString(pDuration);
		if(BanTime < 0) // if time is 0, ban is permanent
			return false;
	}

	Player * pPlayer = objmgr.GetPlayer(pCharacter, false);
	if(pPlayer == NULL)
	{
		pInfo = objmgr.GetPlayerInfoByName(pCharacter);
		if(pInfo == NULL)
		{
			SystemMessage(m_session, "Player not found.");
			return true;
		}
		SystemMessage(m_session, "Banning player '%s' in database for '%s'.", pCharacter, (pReason== NULL)?"No reason.":pReason);
		string escaped_reason = (pReason== NULL)?"No reason.":CharacterDatabase.EscapeString(string(pReason));
		CharacterDatabase.Execute("UPDATE characters SET banned = %u, banReason = '%s' WHERE guid = %u",
		BanTime ? BanTime+(uint32)UNIXTIME : 1, escaped_reason.c_str(), pInfo->guid);
	}
	else
	{
		SystemMessage(m_session, "Banning player '%s' ingame for '%s'.", pCharacter, (pReason== NULL)?"No reason.":pReason);
		string sReason = (pReason== NULL)?"No Reason.":string(pReason);
		uint32 uBanTime = BanTime ? BanTime+(uint32)UNIXTIME : 1;
		pPlayer->SetBanned(uBanTime, sReason);
		pInfo = pPlayer->getPlayerInfo();
	}
	SystemMessage(m_session, "This ban is due to expire %s%s.", BanTime ? "on " : "", BanTime ? ConvertTimeStampToDataTime(BanTime+(uint32)UNIXTIME).c_str() : "Never");

	sGMLog.writefromsession(m_session, "banned %s, reason %s, for %s", pCharacter, (pReason== NULL)?"No reason":pReason, BanTime ? ConvertTimeStampToString(BanTime).c_str() : "ever");
	char msg[200];
	snprintf( msg, 200, "%sGM: %s has been banned by %s for %s. Reason: %s", MSG_COLOR_RED, pCharacter, m_session->GetPlayer()->GetName(), BanTime ? ConvertTimeStampToString( BanTime ).c_str() : "ever", (pReason== NULL)?"No reason.":pReason );
	sWorld.SendWorldText( msg, NULL );
	if( sWorld.m_banTable && pInfo )
	{
		CharacterDatabase.Execute("INSERT INTO %s VALUES('%s', '%s', %u, %u, '%s')", sWorld.m_banTable, m_session->GetPlayer()->GetName(), pInfo->name, (uint32)UNIXTIME, (uint32)UNIXTIME + BanTime, (pReason== NULL)?"No reason.":CharacterDatabase.EscapeString(string(pReason)).c_str() );
	}
	
	if(pPlayer)
	{
		SystemMessage(m_session, "Kicking %s.", pPlayer->GetName());
		pPlayer->Kick();
	}
	return true;
}
bool ChatHandler::HandleBanAllCommand(const char* args, WorldSession *m_session)
{
	if (!*args)
		return false;

	//our vars

	Player *pBanned;
	string pAcc;
	string pIP;
	string pArgs = args;
	char* pCharacter = (char*)args;
	char* pReason;
	char* pDuration;
	ParseBanArgs(pCharacter, &pDuration, &pReason);
	int32 BanTime = 0;
	if(pDuration != NULL)
	{
		BanTime = GetTimePeriodFromString(pDuration);
		if(BanTime < 0)
			return false;
	}
	pBanned = objmgr.GetPlayer(pCharacter,false);
	if (!pBanned || !pBanned->IsInWorld())
	{
		RedSystemMessage(m_session,"Player \'%s\' is not online or does not exists!",pCharacter);
		return true;
	}
	if (pBanned == m_session->GetPlayer())
	{
		RedSystemMessage(m_session,"You cannot ban yourself!");
		return true;
	}
	if (pBanned->GetSession() == NULL)
	{
		RedSystemMessage(m_session,"Player does not have a session!");
		return true;
	}
	if (pBanned->GetSession()->GetSocket() == NULL)
	{
		RedSystemMessage(m_session,"Player does not have a socket!");
		return true;
	}
	pAcc = pBanned->GetSession()->GetAccountName();
	pIP = pBanned->GetSession()->GetSocket()->GetRemoteIP();
	//This check is there incase a gm tries to ban someone on their LAN etc.
	if (pIP == m_session->GetSocket()->GetRemoteIP())
	{
		RedSystemMessage(m_session,"That player has the same IP as you - ban failed");
		return true;
	}

	//Checks complete. time to fire it up?
	/*char Msg[512];
	snprintf(Msg,510,"%s[BAN] %sPlayer%s %s%s has been banned by%s %s %s- this is an account and ip ban. Reason:%s %s",MSG_COLOR_RED,
		MSG_COLOR_WHITE,MSG_COLOR_CYAN,pCharacter,MSG_COLOR_WHITE,MSG_COLOR_GREEN,m_session->GetPlayer()->GetName()
		,MSG_COLOR_WHITE,MSG_COLOR_RED,pReason);
	sWorld.SendWorldText(Msg,NULL);*/
	HandleBanCharacterCommand(pArgs.c_str(),m_session);
	char pIPCmd[256];
	snprintf(pIPCmd,254,"%s %s %s",pIP.c_str(),pDuration,pReason);
	HandleIPBanCommand(pIPCmd,m_session);
	char pAccCmd[256];
	snprintf(pAccCmd,254,"%s %s %s",pAcc.c_str(),pDuration,pReason);
	HandleAccountBannedCommand((const char*)pAccCmd,m_session);
	//GreenSystemMessage(m_session,"Successfully banned player %s with ip %s and account %s",pCharacter,pIP.c_str(),pAcc.c_str());
	return true;
}
bool ChatHandler::HandleUnBanCharacterCommand(const char* args, WorldSession *m_session)
{
	if(!*args)
		return false;

	char Character[255];
	if(sscanf(args, "%s", Character) != 1)
	{
		RedSystemMessage(m_session, "A character name and reason is required.");
		return true;
	}

	// Check if player is in world.
	Player * pPlayer = ObjectMgr::getSingleton( ).GetPlayer(Character, false);
	if(pPlayer != 0)
	{
		GreenSystemMessage(m_session, "Unbanned player %s ingame.",pPlayer->GetName());
		pPlayer->UnSetBanned();
	}
	else
	{
		GreenSystemMessage(m_session, "Player %s not found ingame.", Character);
	}

	// Ban in database
	CharacterDatabase.Execute("UPDATE characters SET banned = 0 WHERE name = '%s'", CharacterDatabase.EscapeString(string(Character)).c_str());

	SystemMessage(m_session, "Unbanned character %s in database.", Character);
	sGMLog.writefromsession(m_session, "unbanned %s", Character);
	return true;
}

bool ChatHandler::HandleAddSkillCommand(const char* args, WorldSession *m_session)
{
	char buf[256];
	Player* target = objmgr.GetPlayer((uint32)m_session->GetPlayer()->GetSelection());

	if(!target) {
		SystemMessage(m_session, "Select A Player first.");
		return true;
	}

	uint32 skillline;
	uint16 cur, max;

	char* pSkillline = strtok((char*)args, " ");
	if (!pSkillline)
		return false;

	char* pCurrent = strtok(NULL, " ");
	if (!pCurrent)
		return false;

	char* pMax = strtok(NULL, " ");
	if (!pMax)
		return false;

	skillline = (uint32)atol(pSkillline);
	cur = (uint16)atol(pCurrent);
	max = (uint16)atol(pMax);

	target->_AddSkillLine(skillline,cur,max);

	snprintf(buf,256,"SkillLine: %u CurrentValue %u Max Value %u Added.",(unsigned int)skillline,(unsigned int)cur,(unsigned int)max);
	sGMLog.writefromsession(m_session, "added skill line %u (%u/%u) to %s", skillline, cur, max, target->GetName());
	SystemMessage(m_session, buf);

	return true;
}

bool ChatHandler::HandleNpcInfoCommand(const char *args, WorldSession *m_session)
{

	static const char *POWERTYPE[] = {
		"Mana",
		"Rage",
		"Focus",
		"Energy",
		"Happiness",
		"Runes",
		"Runic Power"
	};

    uint32 guid = Arcemu::Util::GUID_LOPART(m_session->GetPlayer()->GetSelection());
	Creature *crt = getSelectedCreature(m_session);
	if(!crt) return false;
	BlueSystemMessage(m_session, "Showing creature info for %s", crt->GetCreatureInfo()->Name);
	SystemMessage(m_session, "GUID: %d", guid);
	SystemMessage(m_session, "Faction: %d", crt->GetFaction());
	SystemMessage(m_session, "Phase: %u", crt->GetPhase());
	{
		string s = "";
		if (crt->isBattleMaster())
				s.append(" (Battlemaster)");
		if (crt->isTrainer())
				s.append(" (Trainer)");
		if (crt->isProf())
				s.append(" (Profession Trainer)");
		if (crt->isQuestGiver())
				s.append(" (Quests)");
		if (crt->isGossip())
				s.append(" (Gossip)");
		if (crt->isTaxi())
				s.append(" (Taxi)");
		if (crt->isCharterGiver())
				s.append(" (Charter)");
		if (crt->isGuildBank())
				s.append(" (Guild Bank)");
		if (crt->isSpiritHealer())
				s.append(" (Spirit Healer)");
		if (crt->isInnkeeper())
				s.append(" (Innkeeper)");
		if (crt->isTabardDesigner())
				s.append(" (Tabard Designer)");
		if (crt->isAuctioner())
				s.append(" (Auctioneer)");
		if (crt->isStableMaster())
				s.append(" (Stablemaster)");
		if (crt->isArmorer())
				s.append(" (Armorer)");

		SystemMessage(m_session, "NPCFlags: %d%s", crt->GetUInt32Value(UNIT_NPC_FLAGS), s.c_str());
	}
	SystemMessage(m_session, "DisplayID: %d", crt->GetDisplayId());
	if(crt->m_faction)
		SystemMessage(m_session, "Combat Support: 0x%.3X", crt->m_faction->FriendlyMask);
	SystemMessage(m_session, "Health (cur/max): %d/%d", crt->GetHealth(), crt->GetMaxHealth());
	SystemMessage(m_session, "Mana (cur/max): %d/%d", crt->GetPower( POWER_TYPE_MANA ), crt->GetMaxPower( POWER_TYPE_MANA ) );
	SystemMessage(m_session, "Happiness (cur/max): %d/%d", crt->GetPower( POWER_TYPE_HAPPINESS ), crt->GetMaxPower( POWER_TYPE_HAPPINESS ) );
	SystemMessage(m_session, "Focus (cur/max): %d/%d", crt->GetPower( POWER_TYPE_FOCUS ), crt->GetMaxPower( POWER_TYPE_FOCUS ) );

	uint32 powertype = crt->GetPowerType();
	if( powertype >= 0 && powertype <= 6 )
		SystemMessage(m_session, "Powertype: %s", POWERTYPE[ powertype ] );

	SystemMessage(m_session, "Armor/Holy/Fire/Nature/Frost/Shadow/Arcane");
	SystemMessage(m_session, "%d/%d/%d/%d/%d/%d/%d", crt->GetResistance(SCHOOL_NORMAL), crt->GetResistance(SCHOOL_HOLY), crt->GetResistance(SCHOOL_FIRE), crt->GetResistance(SCHOOL_NATURE), crt->GetResistance(SCHOOL_FROST), crt->GetResistance(SCHOOL_SHADOW), crt->GetResistance(SCHOOL_ARCANE));
	SystemMessage(m_session, "Damage (min/max): %f/%f", crt->GetMinDamage(),crt->GetMaxDamage());

	ColorSystemMessage(m_session, MSG_COLOR_RED, "Entry ID: %d", crt->GetEntry() );
	ColorSystemMessage(m_session, MSG_COLOR_RED, "SQL Entry ID: %d", crt->GetSQL_id());
	// show byte
	std::stringstream sstext;
	uint32 theBytes = crt->GetUInt32Value( UNIT_FIELD_BYTES_0 );
	sstext << "UNIT_FIELD_BYTES_0 are " << uint16((uint8)theBytes & 0xFF) << " " << uint16((uint8)(theBytes >> 8) & 0xFF) << " ";
	sstext << uint16((uint8)(theBytes >> 16) & 0xFF) << " " << uint16((uint8)(theBytes >> 24) & 0xFF) << '\n';

	theBytes = crt->GetUInt32Value( UNIT_FIELD_BYTES_1 );
	sstext << "UNIT_FIELD_BYTES_1 are " << uint16((uint8)theBytes & 0xFF) << " " << uint16((uint8)(theBytes >> 8) & 0xFF) << " ";
	sstext << uint16((uint8)(theBytes >> 16) & 0xFF) << " " << uint16((uint8)(theBytes >> 24) & 0xFF) << '\n';

	theBytes = crt->GetUInt32Value( UNIT_FIELD_BYTES_2 );
	sstext << "UNIT_FIELD_BYTES_2 are " << uint16((uint8)theBytes & 0xFF) << " " << uint16((uint8)(theBytes >> 8) & 0xFF) << " ";
	sstext << uint16((uint8)(theBytes >> 16) & 0xFF) << " " << uint16((uint8)(theBytes >> 24) & 0xFF) << '\0';
	SendMultilineMessage( m_session, sstext.str().c_str() );

    if( crt->GetOwner() && crt->GetOwner()->IsPlayer() )
        SystemMessage(m_session, "Owner: %s", "player" );
    
	return true;
}

bool ChatHandler::HandleCreaturePhaseCommand(const char *args, WorldSession *m_session)
{
	char* sPhase = strtok((char*)args, " ");
	if (!sPhase)
		return false;

	uint32 newphase = atoi(sPhase);

	bool Save = false;
	char* pSave = strtok(NULL, " ");
	if (pSave)
		Save = (atoi(pSave)>0?true:false);

	Creature *crt = getSelectedCreature(m_session);
	if(!crt) return false;

	crt->Phase(PHASE_SET, newphase);
	if (crt->m_spawn) 
		crt->m_spawn->phase = newphase; 
	//VLack: at this point we don't care if it has a spawn or not, as it gets one for sure in SaveToDB, that's why we don't return here from within an else statement.
	//I made this comment in case someone compares this code with the HandleGOPhaseCommand code where we have to have a spawn to be able to save it.

	// Save it to the database.
	if (Save)
	{
		crt->SaveToDB();
		crt->m_loadedFromDB = true;
	}

	sGMLog.writefromsession(m_session, "phased creature with entry %u to %u", crt->GetEntry(), newphase);

	return true;
}

bool ChatHandler::HandleIncreaseWeaponSkill(const char *args, WorldSession *m_session)
{
	char *pMin = strtok((char*)args, " ");
	uint32 cnt = 0;
	if(!pMin)
		cnt = 1;
	else
		cnt = atol(pMin);

	Player *pr = getSelectedChar(m_session, true);

	uint32 SubClassSkill = 0;
	if(!pr) pr = m_session->GetPlayer();
	if(!pr) return false;
	Item *it = pr->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
	ItemPrototype* proto = NULL;
	if (!it)
		it = pr->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED);
	if (it)
		proto = it->GetProto();
	if (proto)
	{
		switch(proto->SubClass)
		{
			// Weapons
		case 0:	// 1 handed axes
			SubClassSkill = SKILL_AXES;
			break;
		case 1:	// 2 handed axes
			SubClassSkill = SKILL_2H_AXES;
			break;
		case 2:	// bows
			SubClassSkill = SKILL_BOWS;
			break;
		case 3:	// guns
			SubClassSkill = SKILL_GUNS;
			break;
		case 4:	// 1 handed mace
			SubClassSkill = SKILL_MACES;
			break;
		case 5:	// 2 handed mace
			SubClassSkill = SKILL_2H_MACES;
			break;
		case 6:	// polearms
			SubClassSkill = SKILL_POLEARMS;
			break;
		case 7: // 1 handed sword
			SubClassSkill = SKILL_SWORDS;
			break;
		case 8: // 2 handed sword
			SubClassSkill = SKILL_2H_SWORDS;
			break;
		case 9: // obsolete
			SubClassSkill = 136;
			break;
		case 10: //1 handed exotic
			SubClassSkill = 136;
			break;
		case 11: // 2 handed exotic
			SubClassSkill = 0;
			break;
		case 12: // fist
			SubClassSkill = SKILL_FIST_WEAPONS;
			break;
		case 13: // misc
			SubClassSkill = 0;
			break;
		case 15: // daggers
			SubClassSkill = SKILL_DAGGERS;
			break;
		case 16: // thrown
			SubClassSkill = SKILL_THROWN;
			break;
		case 18: // crossbows
			SubClassSkill = SKILL_CROSSBOWS;
			break;
		case 19: // wands
			SubClassSkill = SKILL_WANDS;
			break;
		case 20: // fishing
			SubClassSkill = SKILL_FISHING;
			break;
		}
	}
	else
	{
		SubClassSkill = 162;
	}

	if(!SubClassSkill)
	{
		RedSystemMessage(m_session, "Can't find skill ID :-/");
		return false;
	}

	uint32 skill = SubClassSkill;

	BlueSystemMessage(m_session, "Modifying skill line %d. Advancing %d times.", skill, cnt);
	sGMLog.writefromsession(m_session, "increased weapon skill (%u) of %s by %u", skill, pr->GetName(), cnt);

	if(!pr->_HasSkillLine(skill))
	{
		SystemMessage(m_session, "Does not have skill line, adding.");
		pr->_AddSkillLine(skill, 1, 450);
	}
	else
	{
		pr->_AdvanceSkillLine(skill,cnt);
	}
	return true;
}


bool ChatHandler::HandleResetTalentsCommand(const char* args, WorldSession *m_session)
{
	Player *plr = this->getSelectedChar(m_session);
	if(!plr) return true;

	plr->Reset_Talents();

	SystemMessage(m_session, "Reset talents of %s.", plr->GetName());
	BlueSystemMessageToPlr(plr, "%s reset all your talents.", m_session->GetPlayer()->GetName());
	sGMLog.writefromsession(m_session, "reset talents of %s", plr->GetName());
	return true;
}

bool ChatHandler::HandleResetSpellsCommand(const char* args, WorldSession *m_session)
{
	Player *plr = this->getSelectedChar(m_session);
	if(!plr) return true;

	plr->Reset_Spells();

	SystemMessage(m_session, "Reset spells of %s to level 1.", plr->GetName());
	BlueSystemMessageToPlr(plr, "%s reset all your spells to starting values.", m_session->GetPlayer()->GetName());
	sGMLog.writefromsession(m_session, "reset spells of %s", plr->GetName());
	return true;
}

bool ChatHandler::HandleAccountLevelCommand(const char * args, WorldSession * m_session)
{
    if(!*args) return false;

	char account[100];
	char gmlevel[100];
	int argc = sscanf(args, "%s %s", account, gmlevel);
	if(argc != 2)
		return false;

	sLogonCommHandler.Account_SetGM( account, gmlevel );

	GreenSystemMessage(m_session, "Account '%s' level has been updated to '%s'. The change will be effective immediately.", account, gmlevel);
	sGMLog.writefromsession(m_session, "set account %s flags to %s", account, gmlevel);

	return true;
}

bool ChatHandler::HandleAccountUnbanCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;
	char * pAccount = (char*)args;

	sLogonCommHandler.Account_SetBanned( pAccount, 0, "" );
	GreenSystemMessage(m_session, "Account '%s' has been unbanned. This change will be effective immediately.", pAccount);

	sGMLog.writefromsession(m_session, "unbanned account %s", pAccount);
	return true;
}

bool ChatHandler::HandleAccountBannedCommand(const char * args, WorldSession * m_session)
{
	if(!*args)
		return false;
	char* pAccount = (char*)args;
	char* pReason;
	char* pDuration;
	ParseBanArgs(pAccount, &pDuration, &pReason);
	int32 timeperiod = 0;
	if(pDuration != NULL)
	{
		timeperiod = GetTimePeriodFromString(pDuration);
		if(timeperiod < 0)
			return false;
	}
	uint32 banned = (timeperiod ? (uint32)UNIXTIME+timeperiod : 1);

	char emptystring = 0;
	if( pReason == NULL )
		pReason = &emptystring;

	/*stringstream my_sql;
	my_sql << "UPDATE accounts SET banned = " << banned << " WHERE login = '" << CharacterDatabase.EscapeString(string(pAccount)) << "'";

	sLogonCommHandler.LogonDatabaseSQLExecute(my_sql.str().c_str());
	sLogonCommHandler.LogonDatabaseReloadAccounts();*/
	sLogonCommHandler.Account_SetBanned(pAccount, banned, pReason);

	GreenSystemMessage(m_session, "Account '%s' has been banned %s%s for reason : %s. The change will be effective immediately.", pAccount,
		timeperiod ? "until " : "forever", timeperiod ? ConvertTimeStampToDataTime(timeperiod+(uint32)UNIXTIME).c_str() : "", pReason);

	sWorld.DisconnectUsersWithAccount(pAccount, m_session);
	sGMLog.writefromsession(m_session, "banned account %s until %s", pAccount, timeperiod ? ConvertTimeStampToDataTime(timeperiod+(uint32)UNIXTIME).c_str() : "permanent");
	return true;
}

bool ChatHandler::HandleAccountMuteCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	char * pAccount = (char*)args;
	char * pDuration = strchr(pAccount, ' ');
	if(pDuration == NULL)
		return false;
	*pDuration = 0;
	++pDuration;

	int32 timeperiod = GetTimePeriodFromString(pDuration);
	if(timeperiod <= 0)
		return false;

	uint32 banned = (uint32)UNIXTIME+timeperiod;

	sLogonCommHandler.Account_SetMute( pAccount, banned );

	string tsstr = ConvertTimeStampToDataTime(timeperiod+(uint32)UNIXTIME);
	GreenSystemMessage(m_session, "Account '%s' has been muted until %s. The change will be effective immediately.", pAccount,
		tsstr.c_str());

	sGMLog.writefromsession(m_session, "mutex account %s until %s", pAccount, ConvertTimeStampToDataTime(timeperiod+(uint32)UNIXTIME).c_str());

	WorldSession * pSession = sWorld.FindSessionByName(pAccount);
	if( pSession != NULL )
	{
		pSession->m_muted = banned;
		pSession->SystemMessage("Your voice has been muted until %s by a GM. Until this time, you will not be able to speak in any form.", tsstr.c_str());
	}

	return true;
}

bool ChatHandler::HandleAccountUnmuteCommand(const char * args, WorldSession * m_session)
{
	sLogonCommHandler.Account_SetMute( args, 0 );

	GreenSystemMessage(m_session, "Account '%s' has been unmuted.", args);
	sGMLog.writefromsession( m_session, "unmuted account %s", args );
	WorldSession * pSession = sWorld.FindSessionByName(args);
	if( pSession != NULL )
	{
		pSession->m_muted = 0;
		pSession->SystemMessage("Your voice has restored. You may speak again.");
	}

	return true;
}

bool ChatHandler::HandleGetTransporterTime(const char* args, WorldSession* m_session)
{
	//Player *plyr = m_session->GetPlayer();
	Creature * crt = getSelectedCreature(m_session, false);
	if( crt == NULL )
		return false;

	WorldPacket data(SMSG_ATTACKERSTATEUPDATE, 1000);
	data << uint32(0x00000102);
	data << crt->GetNewGUID();
	data << m_session->GetPlayer()->GetNewGUID();

	data << uint32(6);
	data << uint8(1);
	data << uint32(1);
	data << uint32(0x40c00000);
	data << uint32(6);
	data << uint32(0);
	data << uint32(0);
	data << uint32(1);
	data << uint32(0x000003e8);
	data << uint32(0);
	data << uint32(0);
	m_session->SendPacket(&data);
	return true;
}

bool ChatHandler::HandleRemoveAurasCommand(const char *args, WorldSession *m_session)
{
	Player *plr = getSelectedChar(m_session, true);
	if(!plr) return false;

	BlueSystemMessage(m_session, "Removing all auras...");
	for(uint32 i = MAX_REMOVABLE_AURAS_START; i < MAX_REMOVABLE_AURAS_END; ++i)
	{
		if(plr->m_auras[i] != 0) plr->m_auras[i]->Remove();
	}
	if(plr != m_session->GetPlayer())
		sGMLog.writefromsession(m_session, "Removed all of %s's auras.", plr->GetName());
	return true;
}

bool ChatHandler::HandleRemoveRessurectionSickessAuraCommand(const char *args, WorldSession *m_session)
{
	Player *plr = getSelectedChar(m_session, true);
	if(!plr) return false;

	BlueSystemMessage(m_session, "Removing resurrection sickness...");
	plr->RemoveAura( 15007 );
	if(plr != m_session->GetPlayer())
		sGMLog.writefromsession(m_session, "Removed resurrection sickness from %s", plr->GetName());
	return true;
}

bool ChatHandler::HandleParalyzeCommand(const char* args, WorldSession *m_session)
{
	//Player *plr = getSelectedChar(m_session, true);
	//if(!plr) return false;
	Unit *plr = m_session->GetPlayer()->GetMapMgr()->GetUnit(m_session->GetPlayer()->GetSelection());
	if(!plr || plr->GetTypeId() != TYPEID_PLAYER)
	{
		RedSystemMessage(m_session, "Invalid target.");
		return true;
	}

	BlueSystemMessage(m_session, "Rooting target.");
	BlueSystemMessageToPlr( static_cast< Player* >( plr ), "You have been rooted by %s.", m_session->GetPlayer()->GetName() );
	sGMLog.writefromsession( m_session, "rooted player %s", static_cast< Player* >( plr )->GetName() );
	WorldPacket data;
	data.Initialize(SMSG_FORCE_MOVE_ROOT);
	data << plr->GetNewGUID();
	data << uint32(1);

	plr->SendMessageToSet(&data, true);
	return true;
}

bool ChatHandler::HandleUnParalyzeCommand(const char* args, WorldSession *m_session)
{
	//Player *plr = getSelectedChar(m_session, true);
	//if(!plr) return false;
	Unit *plr = m_session->GetPlayer()->GetMapMgr()->GetUnit(m_session->GetPlayer()->GetSelection());
	if(!plr || plr->GetTypeId() != TYPEID_PLAYER)
	{
		RedSystemMessage(m_session, "Invalid target.");
		return true;
	}

	BlueSystemMessage(m_session, "Unrooting target.");
	BlueSystemMessageToPlr( static_cast< Player* >( plr ), "You have been unrooted by %s.", m_session->GetPlayer()->GetName() );
	sGMLog.writefromsession( m_session, "unrooted player %s", static_cast< Player* >( plr )->GetName() );
	WorldPacket data;
	data.Initialize(SMSG_FORCE_MOVE_UNROOT);
	data << plr->GetNewGUID();
	data << uint32(5);

	plr->SendMessageToSet(&data, true);
	return true;
}

bool ChatHandler::HandleSetMotdCommand(const char* args, WorldSession* m_session)
{
	if(!args || strlen(args) < 2)
	{
		RedSystemMessage(m_session, "You must specify a message.");
		return true;
	}

	GreenSystemMessage(m_session, "Motd has been set to: %s", args);
	World::getSingleton().SetMotd(args);
	sGMLog.writefromsession(m_session, "Set MOTD to %s", args);
	return true;
}

bool ChatHandler::HandleAddItemSetCommand(const char* args, WorldSession* m_session)
{
	uint32 setid = (args ? atoi(args) : 0);
	if(!setid)
	{
		RedSystemMessage(m_session, "You must specify a setid.");
		return true;
	}

	Player *chr = getSelectedChar(m_session);
	if (chr == NULL) {
	RedSystemMessage(m_session, "Unable to select character.");
	return true;
	}

	ItemSetEntry *entry = dbcItemSet.LookupEntryForced(setid);
	std::list<ItemPrototype*>* l = objmgr.GetListForItemSet(setid);
	if(!entry || !l)
	{
		RedSystemMessage(m_session, "Invalid item set.");
		return true;
	}
	//const char* setname = sItemSetStore.LookupString(entry->name);
	BlueSystemMessage(m_session, "Searching item set %u...", setid);
	uint32 start = getMSTime();
	sGMLog.writefromsession(m_session, "used add item set command, set %u, target %s", setid, chr->GetName());
	for(std::list<ItemPrototype*>::iterator itr = l->begin(); itr != l->end(); ++itr)
	{
		Item *itm = objmgr.CreateItem((*itr)->ItemId, m_session->GetPlayer());
		if(!itm) continue;
		if(itm->GetProto()->Bonding == ITEM_BIND_ON_PICKUP)
		{
			if(itm->GetProto()->Flags & ITEM_FLAG_ACCOUNTBOUND) // don't "Soulbind" account-bound items
				itm->AccountBind();
			else
				itm->SoulBind();
		}

		if(!chr->GetItemInterface()->AddItemToFreeSlot(itm))
		{
			m_session->SendNotification("No free slots left!");
			itm->DeleteMe();
			return true;
		} else {
			//SystemMessage(m_session, "Added item: %s [%u]", (*itr)->Name1, (*itr)->ItemId);
			SlotResult * le = chr->GetItemInterface()->LastSearchResult();
            chr->SendItemPushResult( false,true,false,true,le->ContainerSlot,le->Slot,1 , itm->GetEntry(), itm->GetItemRandomSuffixFactor(), itm->GetItemRandomPropertyId(), itm->GetStackCount()  );
		}
	}
	GreenSystemMessage(m_session, "Added set to inventory complete. Time: %u ms", getMSTime() - start);
	return true;
}

bool ChatHandler::HandleCastTimeCheatCommand(const char* args, WorldSession* m_session)
{
	Player * plyr = getSelectedChar(m_session, true);
	if (!plyr)
		return true;

	if (!*args)
	{
		if (plyr->CastTimeCheat)
			args = "off";
		else
			args = "on";
	}

	if (stricmp(args, "on") == 0)
	{
		plyr->CastTimeCheat = true;
		BlueSystemMessage(m_session, "activated the cast time cheat on %s.", plyr->GetName());
		GreenSystemMessageToPlr(plyr, "activated the cast time cheat on you.", m_session->GetPlayer()->GetName());
	}
	else if (stricmp(args, "off") == 0)
	{
		plyr->CastTimeCheat = false;
		BlueSystemMessage(m_session, "deactivated the cast time cheat on %s.", plyr->GetName());
		GreenSystemMessageToPlr(plyr, "deactivated the cast time cheat on you.", m_session->GetPlayer()->GetName());

		if ( plyr != m_session->GetPlayer() )
			sGMLog.writefromsession(m_session, "god cast time on %s set to %s", plyr->GetName(), args);
	}
	else
		return false;
	return true;
}

bool ChatHandler::HandleCooldownCheatCommand(const char* args, WorldSession* m_session)
{
	Player * plyr = getSelectedChar(m_session, true);
	if(!plyr)
		return true;

	if(!*args)
	{
		if (plyr->CooldownCheat)
			args = "off";
		else
			args = "on";
	}

	if(stricmp(args, "on") == 0)
	{
		plyr->CooldownCheat = true;
		//best case we could simply iterate through cooldowns or create a special function ...
		SpellSet::const_iterator itr = plyr->mSpells.begin();
		for(; itr != plyr->mSpells.end(); ++itr)
			plyr->ClearCooldownForSpell( (*itr) );
		BlueSystemMessage(m_session, "activated the cooldown cheat on %s.", plyr->GetName());
		GreenSystemMessageToPlr(plyr, "activated the cooldown cheat on you.", m_session->GetPlayer()->GetName());
	}
	else if(stricmp(args, "off") == 0)
	{
		plyr->CooldownCheat = false;
		BlueSystemMessage(m_session, "deactivated the cooldown cheat on %s.", plyr->GetName());
		GreenSystemMessageToPlr(plyr, "deactivated the cooldown cheat on you.", m_session->GetPlayer()->GetName());

		if ( plyr != m_session->GetPlayer() )
			sGMLog.writefromsession(m_session, "cooldown cheat on %s set to %s", plyr->GetName(), args);
	}
	else
		return false;
	return true;
}

bool ChatHandler::HandleGodModeCommand(const char* args, WorldSession* m_session)
{
	Player * plyr = getSelectedChar(m_session, true);
	if(!plyr)
		return true;

	if(!*args)
	{
		if (plyr->GodModeCheat)
			args = "off";
		else
			args = "on";
	}

	if(stricmp(args, "on") == 0)
	{
		plyr->GodModeCheat = true;
		BlueSystemMessage(m_session, "Activating the god mode cheat on %s.", plyr->GetName());
		GreenSystemMessageToPlr(plyr, "%s activated the god mode cheat on you.", m_session->GetPlayer()->GetName());
	}
	else if(stricmp(args, "off") == 0)
	{
		plyr->GodModeCheat = false;
		BlueSystemMessage(m_session, "Deactivating the god mode cheat on %s.", plyr->GetName());
		GreenSystemMessageToPlr(plyr, "%s deactivated the god mode cheat on you.", m_session->GetPlayer()->GetName());

		if ( plyr != m_session->GetPlayer() )
			sGMLog.writefromsession(m_session, "god mode cheat on %s set to %s", plyr->GetName(), args);
	}
	else
		return false;
	return true;
}

bool ChatHandler::HandlePowerCheatCommand(const char* args, WorldSession* m_session)
{
	Player * plyr = getSelectedChar(m_session, true);
	if(!plyr)
		return true;

	if(!*args)
	{
		if (plyr->PowerCheat)
			args = "off";
		else
			args = "on";
	}

	if(stricmp(args, "on") == 0)
	{
		plyr->PowerCheat = true;
		BlueSystemMessage(m_session, "activated the power cheat on %s.", plyr->GetName());
		GreenSystemMessageToPlr(plyr, "activated the power cheat on you.", m_session->GetPlayer()->GetName());
	}
	else if(stricmp(args, "off") == 0)
	{
		plyr->PowerCheat = false;
		BlueSystemMessage(m_session, "deactivated the power cheat on %s.", plyr->GetName());
		GreenSystemMessageToPlr(plyr, "deactivated the power cheat on you.", m_session->GetPlayer()->GetName());

		if ( plyr != m_session->GetPlayer() )
			sGMLog.writefromsession(m_session, "power cheat on %s set to %s", plyr->GetName(), args);
	}
	else
		return false;
	return true;
}

bool ChatHandler::HandleShowCheatsCommand(const char* args, WorldSession* m_session)
{
	Player * plyr = getSelectedChar(m_session, true);
	if(!plyr) return true;

	uint32 active = 0, inactive = 0;
#define print_cheat_status(CheatName, CheatVariable) SystemMessage(m_session, "%s%s: %s%s", MSG_COLOR_LIGHTBLUE, CheatName, \
		CheatVariable ? MSG_COLOR_LIGHTRED : MSG_COLOR_GREEN, CheatVariable ? "Active" : "Inactive");  \
		if(CheatVariable) \
		active++; \
		else \
		inactive++;

	GreenSystemMessage(m_session, "Showing cheat status for: %s", plyr->GetName());
	print_cheat_status("Cooldown", plyr->CooldownCheat);
	print_cheat_status("CastTime", plyr->CastTimeCheat);
	print_cheat_status("GodMode", plyr->GodModeCheat);
	print_cheat_status("Power", plyr->PowerCheat);
	print_cheat_status("Fly", plyr->FlyCheat);
	print_cheat_status("AuraStack", plyr->AuraStackCheat);
	print_cheat_status("ItemStack", plyr->ItemStackCheat);
	print_cheat_status("TriggerPass", plyr->TriggerpassCheat);
	if( plyr->GetSession() && plyr->GetSession()->CanUseCommand('a') )
	{
		print_cheat_status("GM Invisibility", plyr->m_isGmInvisible);
		print_cheat_status("GM Invincibility", plyr->bInvincible);
	}
	SystemMessage(m_session, "%u cheats active, %u inactive.", active, inactive);

#undef print_cheat_status

	return true;
}

bool ChatHandler::HandleFlyCommand(const char* args, WorldSession* m_session)
{
	Player *chr = getSelectedChar(m_session);

	if(!chr)
		chr = m_session->GetPlayer();

	if(!*args)
	{
		if (chr->FlyCheat)
			args = "off";
		else
			args = "on";
	}

	if(stricmp(args, "on") == 0)
	{
		WorldPacket fly(835, 13);
		chr->m_setflycheat = true;
		fly << chr->GetNewGUID();
		fly << uint32(2);
		chr->SendMessageToSet(&fly, true);
		BlueSystemMessage(chr->GetSession(), "Flying mode enabled.");
		if(chr != m_session->GetPlayer())
			sGMLog.writefromsession(m_session, "enabled flying mode for %s", chr->GetName());
	}
	else if(stricmp(args, "off") == 0)
	{
		WorldPacket fly(836, 13);
		chr->m_setflycheat = false;
		fly << chr->GetNewGUID();
		fly << uint32(5);
		chr->SendMessageToSet(&fly, true);
		BlueSystemMessage(chr->GetSession(), "Flying mode disabled.");
		if( chr != m_session->GetPlayer() )
			sGMLog.writefromsession( m_session, "disabled flying mode for %s", chr->GetName() );
	}
	else
		return false;
	return true;
}

bool ChatHandler::HandleDBReloadCommand(const char* args, WorldSession* m_session)
{
	
	sWorld.SendWorldText("Support for reloading tables on the fly was disabled in Arcemu revision 3621. You are seeing this message because apparently reading SVN changelog or using forums search is way over the head of some of our users.", 0 );
	return true;

	/*

	char str[200];
	int ret = 0;
	
	if(!*args || strlen(args) < 3)
		return false;


	uint32 mstime = getMSTime();
	snprintf(str, 200, "%s%s initiated server-side reload of table `%s`. The server may experience some lag while this occurs.",
		MSG_COLOR_LIGHTRED, m_session->GetPlayer()->GetName(), args);
	sWorld.SendWorldText(str, 0);

	if (0 == stricmp(args, "spell_disable"))
	{
		objmgr.ReloadDisabledSpells();
		ret = 1;
	} else
	if (0 == stricmp(args, "vendors"))
	{
		objmgr.ReloadVendors();
		ret = 1;
	}
	else
	{
		ret = Storage_ReloadTable(args);
	}

	if (ret == 0)
		snprintf(str, 200, "%sDatabase reload failed.", MSG_COLOR_LIGHTRED);
	else
		snprintf(str, 200, "%sDatabase reload completed in %u ms.", MSG_COLOR_LIGHTBLUE, getMSTime() - mstime);
	sWorld.SendWorldText(str, 0);
	sGMLog.writefromsession(m_session, "reloaded table %s", args);
	return true;

	*/

}

bool ChatHandler::HandleModifyLevelCommand(const char* args, WorldSession* m_session)
{
	Player * plr = getSelectedChar(m_session, true);
	if(plr == 0) return true;

	uint32 Level = args ? atol(args) : 0;
	if(Level == 0 || Level > sWorld.m_levelCap)
	{
		RedSystemMessage(m_session, "A level (numeric) is required to be specified after this command.");
		return true;
	}

	// Set level message
	BlueSystemMessage(m_session, "Setting the level of %s to %u.", plr->GetName(), Level);
	GreenSystemMessageToPlr(plr, "%s set your level to %u.", m_session->GetPlayer()->GetName(), Level);

	sGMLog.writefromsession(m_session, "used modify level on %s, level %u", plr->GetName(), Level);

	// lookup level information
	LevelInfo * Info = objmgr.GetLevelInfo(plr->getRace(), plr->getClass(), Level);
	if(Info == 0)
	{
		RedSystemMessage(m_session, "Levelup information not found.");
		return true;
	}

	plr->ApplyLevelInfo(Info, Level);
	if( plr->getClass() == WARLOCK )
	{
		std::list<Pet*> summons = plr->GetSummons();
		for(std::list<Pet*>::iterator itr = summons.begin(); itr != summons.end(); ++itr)
		{
			Pet* summon = *itr;
			if( summon->IsInWorld() && summon->isAlive() )
			{
				summon->setLevel(Level);
				summon->ApplyStatsForLevel();
				summon->UpdateSpellList();
			}
		}
	}

	return true;
}

bool ChatHandler::HandleCreatePetCommand(const char* args, WorldSession* m_session)
{
/*	if(!args || strlen(args) < 2)
		return false;

	uint32 Entry = atol(args);
	if(!Entry)
		return false;
	CreatureProto * pTemplate = CreatureProtoStorage.LookupEntry(Entry);
	CreatureInfo * pCreatureInfo = CreatureNameStorage.LookupEntry(Entry);
	if(!pTemplate || !pCreatureInfo)
	{
		RedSystemMessage(m_session, "Invalid creature spawn template: %u", Entry);
		return true;
	}
	Player * plr = m_session->GetPlayer();

	// spawn a creature of this id to create from
	Creature * pCreature = new Creature(HIGHGUID_UNIT ,1);//no need in guid
	CreatureSpawn * sp = new CreatureSpawn;
	sp->id = 1;
	sp->bytes = 0;
	sp->bytes2 = 0;
	sp->displayid = pCreatureInfo->Male_DisplayID;
	sp->emote_state = 0;
	sp->entry = pCreatureInfo->Id;
	sp->factionid = pTemplate->Faction;
	sp->flags = 0;
	sp->form = 0;
	sp->movetype = 0;
	sp->o = plr->GetOrientation();
	sp->x = plr->GetPositionX();
	sp->y = plr->GetPositionY();
	//sp->respawnNpcLink = 0;
	sp->stand_state = 0;
	sp->channel_spell=sp->channel_target_creature=sp->channel_target_go= 0;
	pCreature->Load(sp, (uint32)NULL, NULL);

	Pet *old_tame = plr->GetSummon();
	if(old_tame != NULL)
	{
		old_tame->Dismiss(true);
	}

	// create a pet from this creature
	Pet * pPet = objmgr.CreatePet( Entry );
	pPet->SetInstanceID(plr->GetInstanceID());
	pPet->SetMapId(plr->GetMapId());
	pPet->CreateAsSummon(Entry, pCreatureInfo, pCreature, plr, NULL, 0x2, 0);

	// remove the temp creature
	delete sp;
	delete pCreature;

	sGMLog.writefromsession(m_session, "used create pet entry %u", Entry);
*/
	return true;
}


#ifdef USE_SPECIFIC_AIAGENTS
//this is custom stuff !
bool ChatHandler::HandlePetSpawnAIBot(const char* args, WorldSession *m_session)
{
	if (!*args)
		return false;

	if( !m_session->GetPlayer() )
		return false; //wtf ?

	uint32 botprice = m_session->GetPlayer()->getLevel() * 10000; //1 gold per level ?

	if( !m_session->GetPlayer()->HasGold(botprice) )
	{
		GreenSystemMessage(m_session, "You need a total of %u coins to afford a bot", botprice);
		return false;
	}

	uint8 botType = (uint8)atof((char*)args);

	if ( botType!= 0 )
	{
		RedSystemMessage(m_session, "Incorrect value. Accepting value 0 only = healbot :)");
		return true;
	}

	uint32 Entry;
	char name[50];
	uint8 race = m_session->GetPlayer()->getRace();

	if( race == RACE_HUMAN || race == RACE_DWARF || race == RACE_NIGHTELF || race == RACE_GNOME || race == RACE_DRAENEI )
	{
		Entry = 1826;
		strcpy( name, "|cffff6060A_HealBot" );
	}
	else
	{
		Entry = 5473;
		strcpy( name, "|cffff6060H_HealBot" );
	}

	CreatureProto * pTemplate = CreatureProtoStorage.LookupEntry(Entry);
	CreatureInfo * pCreatureInfo = CreatureNameStorage.LookupEntry(Entry);
	if(!pTemplate || !pCreatureInfo)
	{
		RedSystemMessage(m_session, "Invalid creature spawn template: %u", Entry);
		return true;
	}
	Player * plr = m_session->GetPlayer();
	Creature *newguard = plr->create_guardian( Entry, 2*60*1000, float(-M_PI*2), plr->getLevel() );
	AiAgentHealSupport *new_interface = new AiAgentHealSupport;
	new_interface->Init(newguard,AITYPE_PET,MOVEMENTTYPE_NONE,plr);
	newguard->ReplaceAIInterface( (AIInterface *) new_interface );

/*	Pet *old_tame = plr->GetSummon();
	if(old_tame != NULL)
	{
		old_tame->Dismiss(true);
	}

	// create a pet from this creature
	Pet * pPet = objmgr.CreatePet( Entry );
	pPet->SetInstanceID(plr->GetInstanceID());
	pPet->SetMapId(plr->GetMapId());

	pPet->SetFloatValue ( OBJECT_FIELD_SCALE_X, pTemplate->Scale / 2); //we do not wish to block visually other players
	AiAgentHealSupport *new_interface = new AiAgentHealSupport;
	pPet->ReplaceAIInterface( (AIInterface *) new_interface );
//	new_interface->Init(pPet,AITYPE_PET,MOVEMENTTYPE_NONE,plr); // i think this will get called automatically for pet

	pPet->CreateAsSummon(Entry, pCreatureInfo, pCreature, plr, NULL, 0x2, 0);

	pPet->Rename(name);

	//healer bot should not have any specific actions
	pPet->SetActionBarSlot(0,PET_SPELL_FOLLOW);
	pPet->SetActionBarSlot(1,PET_SPELL_STAY);
	pPet->SetActionBarSlot(2,0);
	pPet->SetActionBarSlot(3,0);
	pPet->SetActionBarSlot(4,0);
	pPet->SetActionBarSlot(5,0);
	pPet->SetActionBarSlot(6,0);
	pPet->SetActionBarSlot(7,0);
	pPet->SetActionBarSlot(8,0);
	pPet->SetActionBarSlot(9,0);
	pPet->SendSpellsToOwner();

	// remove the temp creature
	delete sp;
	delete pCreature;*/

	sGMLog.writefromsession(m_session, "used create an AI bot");
	return true;
}
#endif

bool ChatHandler::HandleAddPetSpellCommand(const char* args, WorldSession* m_session)
{
	Player * plr = getSelectedChar(m_session, false);
	if(!plr)
		return false;

	if( plr->GetSummon() == NULL)
	{
		RedSystemMessage(m_session, "%s has no pet.", plr->GetName());
		return true;
	}

	uint32 SpellId = atol(args);
	SpellEntry * spell = dbcSpell.LookupEntryForced(SpellId);
	if(!SpellId || !spell)
	{
		RedSystemMessage(m_session, "Invalid spell id requested.");
		return true;
	}

	std::list<Pet*> summons = plr->GetSummons();
	for(std::list<Pet*>::iterator itr = summons.begin(); itr != summons.end(); ++itr)
	{
		(*itr)->AddSpell(spell, true);
	}
	GreenSystemMessage( m_session, "Added spell %u to %s's pet.", SpellId, plr->GetName() );
	return true;
}

bool ChatHandler::HandleRemovePetSpellCommand(const char* args, WorldSession* m_session)
{
	Player * plr = getSelectedChar(m_session, false);
	if(!plr)
		return false;

	if( plr->GetSummon() == NULL )
	{
		RedSystemMessage(m_session, "%s has no pet.", plr->GetName());
		return true;
	}

	uint32 SpellId = atol(args);
	SpellEntry * spell = dbcSpell.LookupEntryForced(SpellId);
	if(!SpellId || !spell)
	{
		RedSystemMessage(m_session, "Invalid spell id requested.");
		return true;
	}

	std::list<Pet*> summons = plr->GetSummons();
	for(std::list<Pet*>::iterator itr = summons.begin(); itr != summons.end(); ++itr)
	{
		(*itr)->RemoveSpell(SpellId);
	}
	GreenSystemMessage(m_session, "Removed spell %u from %s's pet.", SpellId, plr->GetName());
	return true;
}

bool ChatHandler::HandleRenamePetCommand(const char* args, WorldSession* m_session)
{
	Player * plr = m_session->GetPlayer();
	Pet * pPet = plr->GetSummon();
	if(pPet == NULL)
	{
		RedSystemMessage(m_session, "You have no pet.");
		return true;
	}

	if(strlen(args) < 1)
	{
		RedSystemMessage(m_session, "You must specify a name.");
		return true;
	}

	GreenSystemMessage(m_session, "Renamed your pet to %s.", args);
	pPet->Rename(args);//support for only 1st pet
	return true;
}

bool ChatHandler::HandleDismissPetCommand(const char* args, WorldSession* m_session)
{
	Player* plr = getSelectedChar(m_session, false);
	Pet* pPet = NULL;
	if(plr)
	{
		if(plr->GetSummon() == NULL)
		{
			RedSystemMessage(m_session, "Player has no pet.");
			return true;
		}
		else
		{
			plr->DismissActivePets();
		}
	}
	else
	{
		// no player selected, see if it is a pet
		Creature* pCrt = getSelectedCreature(m_session, false);
		if(!pCrt)
		{
			// show usage string
			return false;
		}
		if(pCrt->IsPet())
		{
			pPet = (Pet*)pCrt;
		}
		if(!pPet)
		{
			RedSystemMessage(m_session, "No player or pet selected.");
			return true;
		}
		plr = pPet->GetPetOwner();
		pPet->Dismiss();
	}

	GreenSystemMessage(m_session, "Dismissed %s's pet.", plr->GetName());
	plr->GetSession()->SystemMessage("%s dismissed your pet.", m_session->GetPlayer()->GetName());
	return true;
}

bool ChatHandler::HandlePetLevelCommand(const char* args, WorldSession* m_session)
{
	if( !args )
	{
		return false;
	}

	int32 newLevel = atol(args);
	if( newLevel < 1 )
	{
		return false;
	}

	Player* plr = getSelectedChar(m_session, false);
	Pet* pPet = NULL;
	if(plr)
	{
		pPet = plr->GetSummon();
		if(!pPet)
		{
			RedSystemMessage(m_session, "Player has no pet.");
			return true;
		}
	}
	else
	{
		// no player selected, see if it is a pet
		Creature* pCrt = getSelectedCreature(m_session, false);
		if(!pCrt)
		{
			// show usage string
			return false;
		}
		if(pCrt->IsPet())
		{
			pPet = (Pet*)pCrt;
		}
		if(!pPet)
		{
			RedSystemMessage(m_session, "No player or pet selected.");
			return true;
		}
		plr = pPet->GetPetOwner();
	}

	// Should GMs be allowed to set a pet higher than its owner?  I don't think so
	if( (uint32)newLevel > plr->getLevel() )
	{
		newLevel = plr->getLevel();
	}

	//support for only 1 pet
	pPet->setLevel(newLevel );
	pPet->SetUInt32Value( UNIT_FIELD_PETEXPERIENCE, 0 );
	pPet->SetUInt32Value( UNIT_FIELD_PETNEXTLEVELEXP, pPet->GetNextLevelXP(newLevel) );
	pPet->ApplyStatsForLevel();
	pPet->UpdateSpellList();

	GreenSystemMessage(m_session, "Set %s's pet to level %lu.", plr->GetName(), newLevel);
	plr->GetSession()->SystemMessage("%s set your pet to level %lu.", m_session->GetPlayer()->GetName(), newLevel);
	return true;
}

bool ChatHandler::HandleShutdownCommand(const char* args, WorldSession* m_session)
{
	uint32 shutdowntime;
	if( !args )
		shutdowntime = 5;
	else
		shutdowntime = atol( args );

	char msg[500];
	snprintf(msg, 500, "%sServer shutdown initiated by %s, shutting down in %u seconds.", MSG_COLOR_LIGHTBLUE,
		m_session->GetPlayer()->GetName(), (unsigned int)shutdowntime);

	sWorld.SendWorldText(msg);
	sGMLog.writefromsession(m_session, "initiated server shutdown timer %u sec", shutdowntime);
	shutdowntime *= 1000;
	sMaster.m_ShutdownTimer = shutdowntime;
	sMaster.m_ShutdownEvent = true;
	sMaster.m_restartEvent = false;
	return true;
}

bool ChatHandler::HandleShutdownRestartCommand(const char* args, WorldSession* m_session)
{
	uint32 shutdowntime;
	if( !args )
		shutdowntime = 5;
	else
		shutdowntime = atol( args );

	char msg[500];
	snprintf(msg, 500, "%sServer restart initiated by %s, shutting down in %u seconds.", MSG_COLOR_LIGHTBLUE,
		m_session->GetPlayer()->GetName(), (unsigned int)shutdowntime);

	sGMLog.writefromsession(m_session, "initiated server restart timer %u sec", shutdowntime);
	sWorld.SendWorldText(msg);
		shutdowntime *= 1000;
	sMaster.m_ShutdownTimer = shutdowntime;
	sMaster.m_ShutdownEvent = true;
	sMaster.m_restartEvent = true;
	return true;
}

bool ChatHandler::HandleCancelShutdownCommand(const char* args, WorldSession* m_session)
{
	if(sMaster.m_ShutdownEvent == false)
		return false;
	char msg[500];
	snprintf(msg, 500, "%sServer %s cancelled by %s.", MSG_COLOR_LIGHTBLUE, (sMaster.m_restartEvent ? "Restart" : "Shutdown"), m_session->GetPlayer()->GetName());
	sWorld.SendWorldText(msg);

	sMaster.m_ShutdownTimer = 5000;
	sMaster.m_ShutdownEvent = false;
	sMaster.m_restartEvent = false;
	return true;

}

bool ChatHandler::HandleAllowWhispersCommand(const char* args, WorldSession* m_session)
{
	if(args == 0 || strlen(args) < 2) return false;
	PlayerCache* playercache = objmgr.GetPlayerCache(args, false);
	if(playercache == NULL)
	{
		RedSystemMessage(m_session, "Player not found.");
		return true;
	}

	m_session->GetPlayer()->m_cache->InsertValue64(CACHE_GM_TARGETS, playercache->GetUInt32Value(CACHE_PLAYER_LOWGUID));
	std::string name;
	playercache->GetStringValue(CACHE_PLAYER_NAME, name);
	BlueSystemMessage(m_session, "Now accepting whispers from %s.", name.c_str());
	playercache->DecRef();
	return true;
}

bool ChatHandler::HandleBlockWhispersCommand(const char* args, WorldSession* m_session)
{
	if(args == 0 || strlen(args) < 2) return false;
	PlayerCache* playercache = objmgr.GetPlayerCache(args, false);
	if(playercache == NULL)
	{
		RedSystemMessage(m_session, "Player not found.");
		return true;
	}

	m_session->GetPlayer()->m_cache->RemoveValue64(CACHE_GM_TARGETS, playercache->GetUInt32Value(CACHE_PLAYER_LOWGUID));
	std::string name;
	playercache->GetStringValue(CACHE_PLAYER_NAME, name);
	BlueSystemMessage(m_session, "Now blocking whispers from %s.", name.c_str());
	playercache->DecRef();
	return true;
}

bool ChatHandler::HandleAdvanceAllSkillsCommand(const char* args, WorldSession* m_session)
{
	uint32 amt = args ? atol(args) : 0;
	if(!amt)
	{
		RedSystemMessage(m_session, "An amount to increment is required.");
		return true;
	}

	Player * plr = getSelectedChar(m_session);
	if(!plr)
		return true;


	plr->_AdvanceAllSkills(amt);
	GreenSystemMessageToPlr(plr, "Advanced all your skill lines by %u points.", amt);
	sGMLog.writefromsession(m_session, "advanced all skills by %u on %s", amt, plr->GetName());
	return true;
}

bool ChatHandler::HandleKillByPlayerCommand(const char* args, WorldSession* m_session)
{
	if(!args || strlen(args) < 2)
	{
		RedSystemMessage(m_session, "A player's name is required.");
		return true;
	}

	sWorld.DisconnectUsersWithPlayerName(args,m_session);
	sGMLog.writefromsession( m_session, "disconnected player %s", args );
	return true;
}

bool ChatHandler::HandleKillBySessionCommand(const char* args, WorldSession* m_session)
{
	if(!args || strlen(args) < 2)
	{
		RedSystemMessage(m_session, "A player's name is required.");
		return true;
	}

	sWorld.DisconnectUsersWithAccount(args,m_session);
	sGMLog.writefromsession( m_session, "disconnected player with account %s", args );
	return true;
}
bool ChatHandler::HandleKillByIPCommand(const char* args, WorldSession* m_session)
{
	if(!args || strlen(args) < 2)
	{
		RedSystemMessage(m_session, "An IP is required.");
		return true;
	}

	sWorld.DisconnectUsersWithIP(args,m_session);
	sGMLog.writefromsession( m_session, "disconnected players with IP %s", args );
	return true;
}

bool ChatHandler::HandleMassSummonCommand(const char* args, WorldSession* m_session)
{
	PlayerStorageMap::const_iterator itr;
	objmgr._playerslock.AcquireReadLock();
	Player * summoner = m_session->GetPlayer();
	Player * plr;
	int faction=-1;
	char Buffer[170];
        if(*args == 'a' || *args == 'A')
        {
            faction= 0;
           	snprintf(Buffer,170,"%s%s Has requested a mass summon of all Alliance players. Do not feel obliged to accept the summon, as it is most likely for an event or a test of sorts",MSG_COLOR_GOLD,m_session->GetPlayer()->GetName());

        }
        else if(*args == 'h' || *args == 'H')
        {
            faction=1;
            snprintf(Buffer,170,"%s%s Has requested a mass summon of all Horde players. Do not feel obliged to accept the summon, as it is most likely for an event or a test of sorts",MSG_COLOR_GOLD,m_session->GetPlayer()->GetName());
        }
        else  snprintf(Buffer,170,"%s%s Has requested a mass summon of all players. Do not feel obliged to accept the summon, as it is most likely for an event or a test of sorts",MSG_COLOR_GOLD,m_session->GetPlayer()->GetName());

	uint32 c= 0;

	for (itr = objmgr._players.begin(); itr != objmgr._players.end(); itr++)
	{
		plr = itr->second;
		if(plr->GetSession() && plr->IsInWorld())
		{
			//plr->SafeTeleport(summoner->GetMapId(), summoner->GetInstanceID(), summoner->GetPosition());
			/* let's do this the blizz way */
			if(faction>-1 && plr->GetTeam() == static_cast<uint32>( faction ))
			{
                plr->SummonRequest(summoner->GetLowGUID(), summoner->GetZoneId(), summoner->GetMapId(), summoner->GetInstanceID(), summoner->GetPosition());
                ++c;
			}
			else if (faction==-1)
			{
                plr->SummonRequest(summoner->GetLowGUID(), summoner->GetZoneId(), summoner->GetMapId(), summoner->GetInstanceID(), summoner->GetPosition());
                ++c;
            }

		}
	}
	sGMLog.writefromsession(m_session, "requested a mass summon of %u players.", c);
	objmgr._playerslock.ReleaseReadLock();
	return true;
}

bool ChatHandler::HandleCastAllCommand(const char* args, WorldSession* m_session)
{
	if(!args || strlen(args) < 2)
	{
		RedSystemMessage(m_session, "No spellid specified.");
		return true;
	}
	Player * plr;
	uint32 spellid = atol(args);
	SpellEntry * info = dbcSpell.LookupEntryForced(spellid);
	if(!info)
	{
		RedSystemMessage(m_session, "Invalid spell specified.");
		return true;
	}

	// this makes sure no moron casts a learn spell on everybody and wrecks the server
	for (int i = 0; i < 3; i++)
	{
		if (info->Effect[i] == SPELL_EFFECT_LEARN_SPELL) //SPELL_EFFECT_LEARN_SPELL - 36
		{
			sGMLog.writefromsession(m_session, "used wrong / learnall castall command, spellid %u", spellid);
			RedSystemMessage(m_session, "Learn spell specified.");
			return true;
		}
	}

	sGMLog.writefromsession(m_session, "used castall command, spellid %u", spellid);

	PlayerStorageMap::const_iterator itr;
	objmgr._playerslock.AcquireReadLock();
	for (itr = objmgr._players.begin(); itr != objmgr._players.end(); itr++)
	{
		plr = itr->second;
		if(plr->GetSession() && plr->IsInWorld())
		{
			if(plr->GetMapMgr() != m_session->GetPlayer()->GetMapMgr())
			{
				sEventMgr.AddEvent( static_cast< Unit* >( plr ), &Unit::EventCastSpell, static_cast< Unit* >( plr ), info, EVENT_PLAYER_CHECKFORCHEATS, 100, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
			}
			else
			{
				Spell * sp = new Spell(plr, info, true, 0);
				SpellCastTargets targets(plr->GetGUID());
				sp->prepare(&targets);
			}
		}
	}
	objmgr._playerslock.ReleaseReadLock();

	BlueSystemMessage(m_session, "Casted spell %u on all players!", spellid);
	return true;
}

bool ChatHandler::HandleNpcReturnCommand(const char* args, WorldSession* m_session)
{
	Creature * creature = getSelectedCreature(m_session);
	if(!creature || !creature->m_spawn) return true;

	// return to respawn coords
	float x = creature->m_spawn->x;
	float y = creature->m_spawn->y;
	float z = creature->m_spawn->z;
	float o = creature->m_spawn->o;

	// restart movement
	creature->GetAIInterface()->SetAIState(STATE_IDLE);
	creature->GetAIInterface()->WipeHateList();
	creature->GetAIInterface()->WipeTargetList();
	creature->GetAIInterface()->MoveTo(x, y, z, o);

	sGMLog.writefromsession( m_session, "returned NPC %s, sqlid %u", creature->GetCreatureInfo()->Name, creature->GetSQL_id() );

	return true;
}

bool ChatHandler::HandleModPeriodCommand(const char* args, WorldSession * m_session)
{
	Transporter * trans = m_session->GetPlayer()->m_CurrentTransporter;
	if(trans == 0)
	{
		RedSystemMessage(m_session, "You must be on a transporter.");
		return true;
	}

	uint32 np = args ? atol(args) : 0;
	if(np == 0)
	{
		RedSystemMessage(m_session, "A time in ms must be specified.");
		return true;
	}

	trans->SetPeriod(np);
	BlueSystemMessage(m_session, "Period of %s set to %u.", trans->GetInfo()->Name, np);
	return true;
}

bool ChatHandler::HandleFormationLink1Command(const char* args, WorldSession * m_session)
{
	// set formation "master"
	Creature * pCreature = getSelectedCreature(m_session, true);
	if(pCreature == 0) return true;

	m_session->GetPlayer()->linkTarget = pCreature;
	BlueSystemMessage(m_session, "Linkup \"master\" set to %s.", pCreature->GetCreatureInfo()->Name);
	return true;
}

bool ChatHandler::HandleFormationLink2Command(const char* args, WorldSession * m_session)
{
	// set formation "slave" with distance and angle
	float ang, dist;
	if(*args == 0 || sscanf(args, "%f %f", &dist, &ang) != 2)
	{
		RedSystemMessage(m_session, "You must specify a distance and angle.");
		return true;
	}

	if(m_session->GetPlayer()->linkTarget == NULL || m_session->GetPlayer()->linkTarget->IsPet())
	{
		RedSystemMessage(m_session, "Master not selected. select the master, and use formationlink1.");
		return true;
	}

	Creature * slave = getSelectedCreature(m_session, true);
	if(slave == 0) return true;

	slave->GetAIInterface()->m_formationFollowDistance = dist;
	slave->GetAIInterface()->m_formationFollowAngle = ang;
	slave->GetAIInterface()->m_formationLinkTarget = m_session->GetPlayer()->linkTarget->GetGUID();
	slave->GetAIInterface()->m_formationLinkSqlId = m_session->GetPlayer()->linkTarget->GetSQL_id();
	slave->GetAIInterface()->SetUnitToFollowAngle(ang);

	// add to db
	WorldDatabase.Execute("INSERT INTO creature_formations VALUES(%u, %u, '%f', '%f')",
		slave->GetSQL_id(), slave->GetAIInterface()->m_formationLinkSqlId, ang, dist);

	BlueSystemMessage(m_session, "%s linked up to %s with a distance of %f at %f radians.", slave->GetCreatureInfo()->Name,
		m_session->GetPlayer()->linkTarget->GetCreatureInfo()->Name, dist, ang );

	return true;
}

bool ChatHandler::HandleNpcFollowCommand(const char* args, WorldSession * m_session)
{
	Creature * creature = getSelectedCreature(m_session, true);
	if(!creature) return true;

	creature->GetAIInterface()->SetUnitToFollow(m_session->GetPlayer());

	sGMLog.writefromsession( m_session, "used npc follow command on %s, sqlid %u", creature->GetCreatureInfo()->Name, creature->GetSQL_id() );
	return true;
}

bool ChatHandler::HandleFormationClearCommand(const char* args, WorldSession * m_session)
{
	Creature * c = getSelectedCreature(m_session, true);
	if(!c) return true;

	c->GetAIInterface()->m_formationLinkSqlId = 0;
	c->GetAIInterface()->m_formationLinkTarget = 0;
	c->GetAIInterface()->m_formationFollowAngle = 0.0f;
	c->GetAIInterface()->m_formationFollowDistance = 0.0f;
	c->GetAIInterface()->ResetUnitToFollow();

	WorldDatabase.Execute("DELETE FROM creature_formations WHERE spawn_id=%u", c->GetSQL_id());
	return true;
}

bool ChatHandler::HandleNullFollowCommand(const char* args, WorldSession * m_session)
{
	Creature * c = getSelectedCreature(m_session, true);
	if(!c) return true;

	// restart movement
	c->GetAIInterface()->SetAIState(STATE_IDLE);
	c->GetAIInterface()->ResetUnitToFollow();

	sGMLog.writefromsession( m_session, "cancelled npc follow command on %s, sqlid %u", c->GetCreatureInfo()->Name, c->GetSQL_id() );
	return true;
}

bool ChatHandler::HandleItemStackCheatCommand(const char* args, WorldSession* m_session)
{
	Player* p = getSelectedChar(m_session, true);
	if(!p)
		return true;

	bool turnCheatOn;
	if(!*args)
		turnCheatOn = (p->ItemStackCheat) ? false : true;
	else if(stricmp(args, "on") == 0)
		turnCheatOn = true;
	else if(stricmp(args, "off") == 0)
		turnCheatOn = false;
	else
		return false;
	
	p->ItemStackCheat = turnCheatOn;
	BlueSystemMessage(m_session, "%s the item stack cheat on %s.", (turnCheatOn) ? "activated" : "deactivated", p->GetName());
	GreenSystemMessageToPlr(p, "%s %s the item stack cheat on you.%s", m_session->GetPlayer()->GetName(), (turnCheatOn) ? "activated" : "deactivated", (turnCheatOn) ? "" : "  WARNING!!! Make sure all your item stacks are normal (if possible) before logging off, or else you may lose some items!");
	if(p != m_session->GetPlayer() )
	{
		sGMLog.writefromsession(m_session, "item stack cheat on %s set to %s", p->GetName(), (turnCheatOn) ? "on" : "off");
	}
	return true;
}


bool ChatHandler::HandleAuraStackCheatCommand(const char* args, WorldSession * m_session)
{
	Player * plyr = getSelectedChar(m_session, true);
	if(!plyr)
		return true;

	if(!*args)
	{
		if (plyr->AuraStackCheat)
			args = "off";
		else
			args = "on";
	}

	if(stricmp(args, "on") == 0)
	{
		plyr->AuraStackCheat = true;
		BlueSystemMessage(m_session, "activated the aura stack cheat on %s.", plyr->GetName());
		GreenSystemMessageToPlr(plyr, "activated the aura stack cheat on you.", m_session->GetPlayer()->GetName());
	}
	else if(stricmp(args, "off") == 0)
	{
		plyr->AuraStackCheat = false;
		BlueSystemMessage(m_session, "deactivated the aura stack cheat on %s.", plyr->GetName());
		GreenSystemMessageToPlr(plyr, "deactivated the aura stack cheat on you.", m_session->GetPlayer()->GetName());

		if ( plyr != m_session->GetPlayer() )
			sGMLog.writefromsession(m_session, "aura stack cheat on %s set to %s", plyr->GetName(), args);
	}
	else
		return false;
	return true;
}

bool ChatHandler::HandleTriggerpassCheatCommand(const char* args, WorldSession * m_session)
{
	Player * plyr = getSelectedChar(m_session, true);
	if(!plyr)
		return true;

	if(!*args)
	{
		if (plyr->TriggerpassCheat)
			args = "off";
		else
			args = "on";
	}

	if(stricmp(args, "on") == 0)
	{
		plyr->TriggerpassCheat = true;
		BlueSystemMessage(m_session, "activated the triggerpass cheat on %s.", plyr->GetName());
		GreenSystemMessageToPlr(plyr, "activated the triggerpass cheat on you.", m_session->GetPlayer()->GetName());
	}
	else if(stricmp(args, "off") == 0)
	{
		plyr->TriggerpassCheat = false;
		BlueSystemMessage(m_session, "deactivated the triggerpass cheat on %s.", plyr->GetName());
		GreenSystemMessageToPlr(plyr, "deactivated the triggerpass cheat on you.", m_session->GetPlayer()->GetName());

		if ( plyr != m_session->GetPlayer() )
			sGMLog.writefromsession(m_session, "triggerpass cheat on %s set to %s", plyr->GetName(), args);
	}
	else
		return false;
	return true;
}

bool ChatHandler::HandleResetSkillsCommand(const char* args, WorldSession * m_session)
{
	skilllineentry * se;
	Player * plr = getSelectedChar(m_session, true);
	if(!plr) return true;

	plr->_RemoveAllSkills();

	// Load skills from create info.
	PlayerCreateInfo * info = objmgr.GetPlayerCreateInfo(plr->getRace(), plr->getClass());
	if(!info) return true;

	for(std::list<CreateInfo_SkillStruct>::iterator ss = info->skills.begin(); ss!=info->skills.end(); ss++)
	{
		se = dbcSkillLine.LookupEntry(ss->skillid);
		if(se->type != SKILL_TYPE_LANGUAGE && ss->skillid && ss->currentval && ss->maxval)
			plr->_AddSkillLine(ss->skillid, ss->currentval, ss->maxval);
	}
	//Chances depend on stats must be in this order!
	plr->UpdateStats();
	plr->UpdateChances();
	plr->_UpdateMaxSkillCounts();
	plr->_AddLanguages(false);
	BlueSystemMessage(m_session, "Reset skills to default.");
	sGMLog.writefromsession(m_session, "reset skills of %s to default", plr->GetName());
	return true;
}

bool ChatHandler::HandlePlayerInfo(const char* args, WorldSession * m_session)
{
	Player * plr;
	if(strlen(args) >= 2) // char name can be 2 letters
	{
		plr = objmgr.GetPlayer(args, false);
		if(!plr)
		{
			RedSystemMessage(m_session, "Unable to locate player %s.", args);
			return true;
		}
	}
	else
		plr = getSelectedChar(m_session, true);

	if(!plr) return true;
	if(!plr->GetSession())
	{
		RedSystemMessage(m_session, "ERROR: this player hasn't got any session !");
		return true;
	}
	if(!plr->GetSession()->GetSocket())
	{
		RedSystemMessage(m_session, "ERROR: this player hasn't got any socket !");
		return true;
	}
	WorldSession* sess = plr->GetSession();

//	char* infos = new char[128];
	static const char* classes[12] =
	{"None","Warrior", "Paladin", "Hunter", "Rogue", "Priest", "Death Knight", "Shaman", "Mage", "Warlock", "None", "Druid"};
	static const char* races[12] =
	{"None","Human","Orc","Dwarf","Night Elf","Undead","Tauren","Gnome","Troll","None","Blood Elf","Draenei"};

	char playedLevel[64];
	char playedTotal[64];

	int seconds = (plr->GetPlayedtime())[0];
	int mins= 0;
	int hours= 0;
	int days= 0;
	if(seconds >= 60)
	{
		mins = seconds / 60;
		if(mins)
		{
			seconds -= mins*60;
			if(mins >= 60)
			{
				hours = mins / 60;
				if(hours)
				{
					mins -= hours*60;
					if(hours >= 24)
					{
						days = hours/24;
						if(days)
							hours -= days*24;
					}
				}
			}
		}
	}
	snprintf(playedLevel, 64, "[%d days, %d hours, %d minutes, %d seconds]", days, hours, mins, seconds);

	seconds = (plr->GetPlayedtime())[1];
	mins= 0;
	hours= 0;
	days= 0;
	if(seconds >= 60)
	{
		mins = seconds / 60;
		if(mins)
		{
			seconds -= mins*60;
			if(mins >= 60)
			{
				hours = mins / 60;
				if(hours)
				{
					mins -= hours*60;
					if(hours >= 24)
					{
						days = hours/24;
						if(days)
							hours -= days*24;
					}
				}
			}
		}
	}
	snprintf(playedTotal, 64, "[%d days, %d hours, %d minutes, %d seconds]", days, hours, mins, seconds);

	GreenSystemMessage(m_session, "%s is a %s %s %s", plr->GetName(),
		(plr->getGender()?"Female":"Male"), races[plr->getRace()], classes[plr->getClass()]);

	BlueSystemMessage(m_session, "%s has played %s at this level",(plr->getGender()?"She":"He"), playedLevel);
	BlueSystemMessage(m_session, "and %s overall", playedTotal);

	BlueSystemMessage(m_session, "%s is connecting from account '%s'[%u] with permissions '%s'",
		(plr->getGender()?"She":"He"), sess->GetAccountName().c_str(), sess->GetAccountId(), sess->GetPermissions());

	const char *client;

	// Clean code says you need to work from highest combined bit to lowest. Second, you need to check if both flags exists.
        if(sess->HasFlag(ACCOUNT_FLAG_XPACK_02) && sess->HasFlag(ACCOUNT_FLAG_XPACK_01))
	    client = "TBC and WotLK";
        else if(sess->HasFlag(ACCOUNT_FLAG_XPACK_02))
	    client = "Wrath of the Lich King";
        else if(sess->HasFlag(ACCOUNT_FLAG_XPACK_01))
	    client = "WoW Burning Crusade";
        else
	    client = "WoW";

	BlueSystemMessage(m_session, "%s uses %s (build %u)", (plr->getGender()?"She":"He"),
		client, sess->GetClientBuild());

	BlueSystemMessage(m_session, "%s IP is '%s', and has a latency of %ums", (plr->getGender()?"Her":"His"),
		sess->GetSocket()->GetRemoteIP().c_str(), sess->GetLatency());

	return true;
}

bool ChatHandler::HandleGlobalPlaySoundCommand(const char* args, WorldSession * m_session)
{
	if(!*args) return false;
	uint32 sound = atoi(args);
	if(!sound) return false;

	WorldPacket data(SMSG_PLAY_SOUND, 4);
	data << sound;
	sWorld.SendGlobalMessage(&data, 0);
	BlueSystemMessage(m_session, "Broadcasted sound %u to server.", sound);
	sGMLog.writefromsession(m_session, "used play all command soundid %u", sound);
	return true;
}

bool ChatHandler::HandleIPBanCommand(const char * args, WorldSession * m_session)
{
	char* pIp = (char*)args;
	char* pReason;
	char* pDuration;
	ParseBanArgs(pIp, &pDuration, &pReason);
	int32 timeperiod = 0;
	if(pDuration != NULL)
	{
		timeperiod = GetTimePeriodFromString(pDuration);
		if(timeperiod < 0)
			return false;
	}

	uint32 o1, o2, o3, o4;
	if ( sscanf(pIp, "%3u.%3u.%3u.%3u", (unsigned int*)&o1, (unsigned int*)&o2, (unsigned int*)&o3, (unsigned int*)&o4) != 4
			|| o1 > 255 || o2 > 255 || o3 > 255 || o4 > 255)
	{
		RedSystemMessage(m_session, "Invalid IPv4 address [%s]", pIp);
		return true;	// error in syntax, but we wont remind client of command usage
	}

	time_t expire_time;
	if ( timeperiod == 0)		// permanent ban
		expire_time = 0;
	else
		expire_time = UNIXTIME + (time_t)timeperiod;
	string IP = pIp;
	string::size_type i = IP.find("/");
	if (i == string::npos)
	{
		RedSystemMessage(m_session, "Lack of CIDR address assumes a 32bit match (if you don't understand, don't worry, it worked)");
		IP.append("/32");
	}

	//temporal IP or real pro flooder who will change it tomorrow ?
	char emptystring = 0;
	if( pReason == NULL )
		pReason = &emptystring;

	SystemMessage(m_session, "Adding [%s] to IP ban table, expires %s.Reason is :%s", pIp, (expire_time == 0)? "Never" : ctime( &expire_time ),pReason);
	sLogonCommHandler.IPBan_Add( IP.c_str(), (uint32)expire_time, pReason );
	sWorld.DisconnectUsersWithIP(IP.substr(0,IP.find("/")).c_str(), m_session);
	sGMLog.writefromsession(m_session, "banned ip address %s, expires %s", pIp, (expire_time == 0)? "Never" : ctime( &expire_time ));
	return true;
}

bool ChatHandler::HandleIPUnBanCommand(const char * args, WorldSession * m_session)
{
	string pIp = args;
	if (pIp.length() == 0)
		return false;

	if (pIp.find("/") == string::npos)
	{
		RedSystemMessage(m_session, "Lack of CIDR address assumes a 32bit match (if you don't understand, don't worry, it worked)");
		pIp.append("/32");
	}
	/**
	* We can afford to be less fussy with the validity of the IP address given since
	 * we are only attempting to remove it.
	 * Sadly, we can only blindly execute SQL statements on the logonserver so we have
	 * no idea if the address existed and so the account/IPBanner cache requires reloading.
	 */

	SystemMessage(m_session,"Deleting [%s] from ip ban table if it exists",pIp.c_str());
	sLogonCommHandler.IPBan_Remove( pIp.c_str() );
	sGMLog.writefromsession(m_session, "unbanned ip address %s", pIp.c_str());
	return true;
}
bool ChatHandler::HandleCreatureSpawnCommand(const char *args, WorldSession *m_session)
{
	uint32 entry = atol(args);
	uint8 gender = 0;

	if(entry == 0)
		return false;

	CreatureProto * proto = CreatureProtoStorage.LookupEntry(entry);
	CreatureInfo * info = CreatureNameStorage.LookupEntry(entry);
	if(proto == 0 || info == 0)
	{
		RedSystemMessage(m_session, "Invalid entry id.");
		return true;
	}

	CreatureSpawn * sp = new CreatureSpawn;
	//sp->displayid = info->DisplayID;
	gender = info->GenerateModelId(&sp->displayid);
 	sp->entry = entry;
	sp->entry = entry;
	sp->form = 0;
	sp->id = objmgr.GenerateCreatureSpawnID();
	sp->movetype = 0;
	sp->x = m_session->GetPlayer()->GetPositionX();
	sp->y = m_session->GetPlayer()->GetPositionY();
	sp->z = m_session->GetPlayer()->GetPositionZ();
	sp->o = m_session->GetPlayer()->GetOrientation();
	sp->emote_state = 0;
	sp->flags = 0;
	sp->factionid = proto->Faction;
	sp->bytes0 = sp->setbyte(0,2,gender);
	sp->bytes1 = 0;
	sp->bytes2 = 0;
	//sp->respawnNpcLink = 0;
	sp->stand_state = 0;
	sp->death_state = 0;
	sp->channel_target_creature = sp->channel_target_go = sp->channel_spell = 0;
	sp->MountedDisplayID = 0;
	sp->Item1SlotDisplay = 0;
	sp->Item2SlotDisplay = 0;
	sp->Item3SlotDisplay = 0;
	sp->CanFly = 0;
	sp->phase = m_session->GetPlayer()->GetPhase();


	Creature * p = m_session->GetPlayer()->GetMapMgr()->CreateCreature(entry);
	Arcemu::Util::ARCEMU_ASSERT(   p != NULL );
	p->Load(sp, (uint32)NULL, NULL);
	p->m_loadedFromDB = true;
	p->PushToWorld(m_session->GetPlayer()->GetMapMgr());

	uint32 x = m_session->GetPlayer()->GetMapMgr()->GetPosX(m_session->GetPlayer()->GetPositionX());
	uint32 y = m_session->GetPlayer()->GetMapMgr()->GetPosY(m_session->GetPlayer()->GetPositionY());

	// Add spawn to map
	m_session->GetPlayer()->GetMapMgr()->GetBaseMap()->GetSpawnsListAndCreate(
		x,
		y)->CreatureSpawns.push_back(sp);

	MapCell * mCell = m_session->GetPlayer()->GetMapMgr()->GetCell( x, y );

	if( mCell != NULL )
		mCell->SetLoaded();

	BlueSystemMessage(m_session, "Spawned a creature `%s` with entry %u at %f %f %f on map %u", info->Name,
		entry, sp->x, sp->y, sp->z, m_session->GetPlayer()->GetMapId());

	// Save it to the database.
	p->SaveToDB();

	sGMLog.writefromsession(m_session, "spawned a %s at %u %f %f %f", info->Name, m_session->GetPlayer()->GetMapId(),sp->x,sp->y,sp->z);

	return true;
}

bool ChatHandler::HandleCreatureRespawnCommand(const char *args, WorldSession *m_session)
{
	Creature * cCorpse = getSelectedCreature( m_session, false );

	if( cCorpse != NULL && cCorpse->IsCreature() && cCorpse->getDeathState() == CORPSE && cCorpse->GetSQL_id() != 0 )
	{
		sEventMgr.RemoveEvents( cCorpse, EVENT_CREATURE_RESPAWN );

		BlueSystemMessage( m_session, "Respawning a Creature: `%s` with entry: %u on map: %u sqlid: %u", cCorpse->GetCreatureInfo()->Name,
			cCorpse->GetEntry(), cCorpse->GetMapMgr()->GetMapId(), cCorpse->GetSQL_id() );

		sGMLog.writefromsession(m_session, "Respawned a Creature: `%s` with entry: %u on map: %u sqlid: %u", cCorpse->GetCreatureInfo()->Name,
			cCorpse->GetEntry(), cCorpse->GetMapMgr()->GetMapId(), cCorpse->GetSQL_id() );

		cCorpse->Despawn( 0, 1000 );
		return true;
	}

	RedSystemMessage( m_session, "You must select a creature's corpse with a valid CreatureSpawn point." );
	return false;
}

bool ChatHandler::HandleNPCCanFlyCommand(const char * args, WorldSession * m_session)
{
	Creature * pCreature = getSelectedCreature(m_session, true);
	if(pCreature == NULL)
		return true;
	pCreature->GetAIInterface()->m_moveFly = !pCreature->GetAIInterface()->m_moveFly;
	pCreature->GetAIInterface()->onGameobject = false;
	char* sSave = strtok((char*)args, " ");
	if (sSave)
	{
		bool save = (atoi(sSave)>0?true:false);
		if (save)
		{
			pCreature->SaveToDB();
			pCreature->m_loadedFromDB = true;
		}
	}
	GreenSystemMessage(m_session, "You may have to leave and re-enter this zone for changes to take effect.");
	return true;
}

bool ChatHandler::HandleNPCOnGOCommand(const char * args, WorldSession * m_session)
{
	Creature * pCreature = getSelectedCreature(m_session, true);
	if(pCreature == NULL)
		return true;
	pCreature->GetAIInterface()->m_moveFly = false;
	pCreature->GetAIInterface()->onGameobject = !pCreature->GetAIInterface()->onGameobject;
	char* sSave = strtok((char*)args, " ");
	if (sSave)
	{
		bool save = (atoi(sSave)>0?true:false);
		if (save)
		{
			pCreature->SaveToDB();
			pCreature->m_loadedFromDB = true;
		}
	}
	GreenSystemMessage(m_session, "You may have to leave and re-enter this zone for changes to take effect.");
	return true;
}

bool ChatHandler::HandleRemoveItemCommand(const char * args, WorldSession * m_session)
{
	uint32 item_id;
	int32 count, ocount;
	int argc = sscanf(args, "%u %u", (unsigned int*)&item_id, (unsigned int*)&count);
	if(argc == 1)
		count = 1;
	else if(argc != 2 || !count)
		return false;

	ocount = count;
	Player * plr = getSelectedChar(m_session, true);
	if(!plr) return true;

	// loop until they're all gone.
	int32 loop_count = 0;
	int32 start_count = plr->GetItemInterface()->GetItemCount(item_id, true);
	int32 start_count2 = start_count;
	if(count > start_count)
		count = start_count;

	while(start_count >= count && (count > 0) && loop_count < 20)	 // Prevent a loop here.
	{
		plr->GetItemInterface()->RemoveItemAmt(item_id, count);
		start_count2 = plr->GetItemInterface()->GetItemCount(item_id, true);
		count -= (start_count - start_count2);
		start_count = start_count2;
		++loop_count;
	}

	ItemPrototype * iProto	= ItemPrototypeStorage.LookupEntry(item_id);

	if( iProto )
	{
		sGMLog.writefromsession(m_session, "used remove item %s (id: %u) count %u from %s", iProto->Name1, item_id, ocount, plr->GetName());
		BlueSystemMessage(m_session, "Removing %u copies of item %s (id: %u) from %s's inventory.", ocount, GetItemLinkByProto(iProto, m_session->language).c_str(), item_id, plr->GetName());
		BlueSystemMessage(plr->GetSession(), "%s removed %u copies of item %s from your inventory.", m_session->GetPlayer()->GetName(), ocount, GetItemLinkByProto(iProto, plr->GetSession()->language).c_str());
	}
	else RedSystemMessage(m_session, "Cannot remove non valid item id: %u .",item_id);

	return true;
}

bool ChatHandler::HandleForceRenameCommand(const char * args, WorldSession * m_session)
{
	// prevent buffer overflow
	if(strlen(args) > 100)
		return false;

	string tmp = string(args);
	PlayerInfo * pi = objmgr.GetPlayerInfoByName(tmp.c_str());
	if(pi == 0)
	{
		RedSystemMessage(m_session, "Player with that name not found.");
		return true;
	}

	Player * plr = objmgr.GetPlayer((uint32)pi->guid);
	if(plr == 0)
	{
		CharacterDatabase.Execute("UPDATE characters SET forced_rename_pending = 1 WHERE guid = %u", (uint32)pi->guid);
	}
	else
	{
		plr->rename_pending = true;
		plr->SaveToDB(false);
		BlueSystemMessageToPlr(plr, "%s forced your character to be renamed next logon.", m_session->GetPlayer()->GetName());
	}

	CharacterDatabase.Execute("INSERT INTO banned_names VALUES('%s')", CharacterDatabase.EscapeString(string(pi->name)).c_str());
	GreenSystemMessage(m_session, "Forcing %s to rename his character next logon.", args);
	sGMLog.writefromsession(m_session, "forced %s to rename his charater (%u)", pi->name, pi->guid);
	return true;
}

bool ChatHandler::HandleGetStandingCommand(const char * args, WorldSession * m_session)
{
	uint32 faction = atoi(args);
	Player * plr = getSelectedChar(m_session, true);
	if(!plr) return true;

	int32 standing = plr->GetStanding(faction);
	int32 bstanding = plr->GetBaseStanding(faction);

	GreenSystemMessage(m_session, "Reputation for faction %u:", faction);
	SystemMessage(m_session, "Base Standing: %d", bstanding);
	BlueSystemMessage(m_session, "Standing: %d", standing);
	return true;
}

bool ChatHandler::HandleSetStandingCommand(const char * args, WorldSession * m_session)
{
	uint32 faction;
	int32 standing;
	if(sscanf(args, "%u %d", (unsigned int*)&faction, (unsigned int*)&standing) != 2)
		return false;
	Player * plr = getSelectedChar(m_session, true);
	if(!plr) return true;

	BlueSystemMessage(m_session, "Setting standing of %u to %d on %s.", faction, standing, plr->GetName());
	plr->SetStanding(faction, standing);
	GreenSystemMessageToPlr(plr, "%s set your standing of faction %u to %d.", m_session->GetPlayer()->GetName(), faction, standing);
	sGMLog.writefromsession(m_session, "set standing of faction %u to %u for %s", faction,standing,plr->GetName());
	return true;
}

void ChatHandler::SendHighlightedName(WorldSession * m_session, const char* prefix, const char* full_name, string& lowercase_name, string& highlight, uint32 id)
{
	char message[1024];
	char start[50];
	start[0] = message[0] = 0;

	snprintf(start, 50, "%s %u: %s", prefix, (unsigned int)id, MSG_COLOR_WHITE);

	string::size_type hlen = highlight.length();
	string fullname = string(full_name);
	string::size_type offset = lowercase_name.find(highlight);
	string::size_type remaining = fullname.size() - offset - hlen;
	strcat(message, start);
	strncat(message, fullname.c_str(), offset);
	strcat(message, MSG_COLOR_LIGHTRED);
	strncat(message, (fullname.c_str() + offset), hlen);
	strcat(message, MSG_COLOR_WHITE);
	if(remaining > 0) strncat(message, (fullname.c_str() + offset + hlen), remaining);

	SystemMessage(m_session, message);
}

void ChatHandler::SendItemLinkToPlayer(ItemPrototype * iProto, WorldSession * pSession, bool ItemCount, Player * owner, uint32 language )
{
	if(!iProto || !pSession)
		return;
	if(ItemCount && owner == NULL)
		return;

 	if(ItemCount)
	{
		int8 count = static_cast<int8>(owner->GetItemInterface()->GetItemCount(iProto->ItemId, true));
		//int8 slot = owner->GetItemInterface()->GetInventorySlotById(iProto->ItemId); //DISABLED due to being a retarded concept
		if( iProto->ContainerSlots > 0 )
		{
			SystemMessage(pSession,"Item %u %s Count %u ContainerSlots %u", iProto->ItemId, GetItemLinkByProto(iProto, language).c_str(), count, iProto->ContainerSlots);
		}
		else
		{
			SystemMessage(pSession,"Item %u %s Count %u", iProto->ItemId, GetItemLinkByProto(iProto, language).c_str(), count);
		}
	}
 	else
	{
		if( iProto->ContainerSlots > 0 )
		{
			SystemMessage(pSession,"Item %u %s ContainerSlots %u", iProto->ItemId, GetItemLinkByProto(iProto, language).c_str(), iProto->ContainerSlots);
		}
		else
		{
			SystemMessage(pSession,"Item %u %s", iProto->ItemId, GetItemLinkByProto(iProto, language).c_str());
		}
	}
}


bool ChatHandler::HandleLookupItemCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	string x = string(args);
	arcemu_TOLOWER(x);
	if(x.length() < 4)
	{
		RedSystemMessage(m_session, "Your search string must be at least 4 characters long.");
		return true;
	}

	BlueSystemMessage(m_session, "Starting search of item `%s`...", x.c_str());
	uint32 t = getMSTime();
	ItemPrototype * it;
	uint32 count = 0;

	StorageContainerIterator<ItemPrototype> * itr = ItemPrototypeStorage.MakeIterator();

	while(!itr->AtEnd())
	{
		it = itr->Get();
		LocalizedItem *lit	= (m_session->language>0) ? sLocalizationMgr.GetLocalizedItem(it->ItemId, m_session->language) : NULL;

		std::string litName	= std::string(lit ? lit->Name : "");

		arcemu_TOLOWER(litName);

		bool localizedFound	= false;
		if(FindXinYString(x, litName))
			localizedFound	= true;

		if(FindXinYString(x, it->lowercase_name) || localizedFound)
		{
			// Print out the name in a cool highlighted fashion
			//SendHighlightedName(m_session, it->Name1, it->lowercase_name, x, it->ItemId, true);
			SendItemLinkToPlayer(it, m_session, false, 0, localizedFound ? m_session->language : 0);
			++count;
			if(count == 25)
			{
				RedSystemMessage(m_session, "More than 25 results returned. aborting.");
				break;
			}
		}

		if(!itr->Inc())
			break;
	}
	itr->Destruct();

	BlueSystemMessage(m_session, "Search completed in %u ms.", getMSTime() - t);
	return true;
}

bool ChatHandler::HandleLookupObjectCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	string x = string(args);
	arcemu_TOLOWER(x);

	StorageContainerIterator<GameObjectInfo> * itr = GameObjectNameStorage.MakeIterator();

	GreenSystemMessage(m_session, "Starting search of object `%s`...", x.c_str());
	uint32 t = getMSTime();
	GameObjectInfo * i;
	uint32 count = 0;
	string y;
	string recout;
	while(!itr->AtEnd())
	{
		i = itr->Get();
		y = string(i->Name);
		arcemu_TOLOWER(y);
		if(FindXinYString(x,y))
		{
			//string objectID=MyConvertIntToString(i->ID);
			string Name;
			std::stringstream strm;
			strm<<i->ID;
			strm << ", Display ";
			strm << i->DisplayID;
			//string ObjectID = i.c_str();
			const char*objectName=i->Name;
			recout="|cfffff000Object ";
			recout+=strm.str();
			recout+="|cffFFFFFF: ";
			recout+=objectName;
			recout = recout + Name;
			SendMultilineMessage(m_session,recout.c_str());
			
			++count;
			if(count==25 || count > 25)
			{
				RedSystemMessage(m_session,"More than 25 results returned. aborting.");
				break;
			}
		}
		if(!itr->Inc()) break;
	}
	itr->Destruct();
	if (count== 0)
	{
		recout="|cff00ccffNo matches found.";
		SendMultilineMessage(m_session,recout.c_str());
	}
	BlueSystemMessage(m_session,"Search completed in %u ms.",getMSTime()-t);
	return true;
}
bool ChatHandler::HandleLookupCreatureCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	string x = string(args);
	arcemu_TOLOWER(x);
	if(x.length() < 4)
	{
		RedSystemMessage(m_session, "Your search string must be at least 4 characters long.");
		return true;
	}

	StorageContainerIterator<CreatureInfo> * itr = CreatureNameStorage.MakeIterator();

	GreenSystemMessage(m_session, "Starting search of creature `%s`...", x.c_str());
	uint32 t = getMSTime();
	CreatureInfo * i;
	uint32 count = 0;
	while(!itr->AtEnd())
	{
		i = itr->Get();
		LocalizedCreatureName *li	= (m_session->language>0) ? sLocalizationMgr.GetLocalizedCreatureName(i->Id, m_session->language) : NULL;

		std::string liName	= std::string(li ? li->Name : "");

		arcemu_TOLOWER(liName);

		bool localizedFound	= false;

		if(FindXinYString(x, liName))
			localizedFound	= true;

		if(FindXinYString(x, i->lowercase_name) || localizedFound)
 		{
 			// Print out the name in a cool highlighted fashion
			SendHighlightedName(m_session, "Creature", localizedFound ? li->Name : i->Name, localizedFound ? liName : i->lowercase_name, x, i->Id);
			++count;
			if(count == 25)
			{
				RedSystemMessage(m_session, "More than 25 results returned. aborting.");
				break;
			}
		}
		if(!itr->Inc())
			break;
	}
	itr->Destruct();

	GreenSystemMessage(m_session, "Search completed in %u ms.", getMSTime() - t);
	return true;
}

bool ChatHandler::HandleLookupSpellCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	string x = string(args);
	arcemu_TOLOWER(x);
	if(x.length() < 4)
	{
		RedSystemMessage(m_session, "Your search string must be at least 4 characters long.");
		return true;
	}

	GreenSystemMessage(m_session, "Starting search of spell `%s`...", x.c_str());
	uint32 t = getMSTime();
	uint32 count = 0;
	string recout;
	char itoabuf[12];
	for (uint32 index = 0; index < dbcSpell.GetNumRows(); ++index)
	{
		SpellEntry* spell = dbcSpell.LookupRow(index);
		string y = string(spell->Name);
		arcemu_TOLOWER(y);
		if(FindXinYString(x, y))
 		{
 			// Print out the name in a cool highlighted fashion
			// SendHighlightedName(m_session, "Spell", spell->Name, y, x, spell->Id);
			// Send spell link instead
			sprintf((char*)itoabuf,"%u",spell->Id);
			recout = (const char*)itoabuf;
			recout += ": |cff71d5ff|Hspell:";
			recout += (const char*)itoabuf;
			recout += "|h[";
			recout += spell->Name;
			recout += "]|h|r";

			std::string::size_type pos = recout.find('%');
			if( pos != std::string::npos ){
				recout.insert( pos + 1, "%");
			}

			SendMultilineMessage(m_session, recout.c_str());

			++count;
			if(count == 25)
			{
				RedSystemMessage(m_session, "More than 25 results returned. aborting.");
				break;
			}
		}
	}

	GreenSystemMessage(m_session, "Search completed in %u ms.", getMSTime() - t);
	return true;
}

bool ChatHandler::HandleLookupSkillCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	string x = string(args);
	arcemu_TOLOWER(x);
	if(x.length() < 4)
	{
		RedSystemMessage(m_session, "Your search string must be at least 4 characters long.");
		return true;
	}

	GreenSystemMessage(m_session, "Starting search of skill `%s`...", x.c_str());
	uint32 t = getMSTime();
	uint32 count = 0;
	for (uint32 index = 0; index < dbcSkillLine.GetNumRows(); ++index)
	{
		skilllineentry* skill = dbcSkillLine.LookupRow(index);
		string y = string(skill->Name);
		arcemu_TOLOWER(y);
		if(FindXinYString(x, y))
 		{
 			// Print out the name in a cool highlighted fashion
			SendHighlightedName(m_session, "Skill", skill->Name, y, x, skill->id);
			++count;
			if(count == 25)
			{
				RedSystemMessage(m_session, "More than 25 results returned. aborting.");
				break;
			}
		}
	}

	GreenSystemMessage(m_session, "Search completed in %u ms.", getMSTime() - t);
	return true;
}

bool ChatHandler::HandleLookupFactionCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	string x = string(args);
	arcemu_TOLOWER(x);
	if(x.length() < 4)
	{
		RedSystemMessage(m_session, "Your search string must be at least 4 characters long.");
		return true;
	}

	GreenSystemMessage(m_session, "Starting search of faction `%s`...", x.c_str());
	uint32 t = getMSTime();
	uint32 count = 0;
	for (uint32 index = 0; index < dbcFaction.GetNumRows(); ++index)
	{
		FactionDBC* faction = dbcFaction.LookupRow(index);
		string y = string(faction->Name);
		arcemu_TOLOWER(y);
		if(FindXinYString(x, y))
 		{
 			// Print out the name in a cool highlighted fashion
			SendHighlightedName(m_session, "Faction", faction->Name, y, x, faction->ID);
			++count;
			if(count == 25)
			{
				RedSystemMessage(m_session, "More than 25 results returned. aborting.");
				break;
			}
		}
	}

	GreenSystemMessage(m_session, "Search completed in %u ms.", getMSTime() - t);
	return true;
}

bool ChatHandler::HandleGORotate(const char * args, WorldSession * m_session)
{
	char Axis;
	float deg;
	if(sscanf(args, "%c %f", &Axis, &deg) < 1) return false;
	GameObject *go = m_session->GetPlayer()->GetSelectedGo();
	if( !go )
	{
		RedSystemMessage(m_session, "No selected GameObject...");
		return true;
	}

	// float rad = deg * (float(M_PI) / 180.0f);

	switch(tolower(Axis))
	{
	case 'x':
//		go->ModFloatValue(GAMEOBJECT_ROTATION, rad);
		break;
	case 'y':
//		go->ModFloatValue(GAMEOBJECT_ROTATION_01, rad);
		break;
	case 'o':
		if(m_session->GetPlayer()){
			float ori = m_session->GetPlayer()->GetOrientation();
			go->SetParentRotation(2, sinf(ori / 2));
			go->SetParentRotation(3, cosf(ori / 2));
			go->SetOrientation(ori);
			go->UpdateRotation();
		}
		break;
	default:
		RedSystemMessage(m_session, "Invalid Axis, Please use x, y, or o.");
		return true;
	}

	uint32 NewGuid = m_session->GetPlayer()->GetMapMgr()->GenerateGameobjectGuid();
	go->RemoveFromWorld(true);
	go->SetNewGuid(NewGuid);
	go->PushToWorld(m_session->GetPlayer()->GetMapMgr());
	go->SaveToDB();
	//lets reselect the object that can be really annoying...
	m_session->GetPlayer()->m_GM_SelectedGO = NewGuid;
	return true;
}

bool ChatHandler::HandleGOMove(const char * args, WorldSession * m_session)
{
	// move the go to player's coordinates
	GameObject *go = m_session->GetPlayer()->GetSelectedGo();
	if( !go )
	{
		RedSystemMessage(m_session, "No selected GameObject...");
		return true;
	}

	// new GO position (player location)
	// orientation doesn't change
	float x = m_session->GetPlayer()->GetPositionX();
	float y = m_session->GetPlayer()->GetPositionY();
	float z = m_session->GetPlayer()->GetPositionZ();
	float o = go->GetOrientation();

	go->RemoveFromWorld(true);
	go->SetPosition(x, y, z, o);
//	go->SetFloatValue(GAMEOBJECT_POS_X, x);
//	go->SetFloatValue(GAMEOBJECT_POS_Y, y);
//	go->SetFloatValue(GAMEOBJECT_POS_Z, z);
	uint32 NewGuid = m_session->GetPlayer()->GetMapMgr()->GenerateGameobjectGuid();
	go->SetNewGuid(NewGuid);
	go->PushToWorld(m_session->GetPlayer()->GetMapMgr());
	go->SaveToDB();
	m_session->GetPlayer()->m_GM_SelectedGO = NewGuid;
	return true;
}

bool ChatHandler::HandleNpcPossessCommand(const char * args, WorldSession * m_session)
{
	Unit * pTarget = getSelectedChar(m_session, false);
	if(!pTarget)
	{
		pTarget = getSelectedCreature(m_session, false);
		if(pTarget && (pTarget->IsPet() || pTarget->GetCreatedByGUID() != 0))
			return false;
	}

	if(!pTarget)
	{
		RedSystemMessage(m_session, "You must select a player/creature.");
		return true;
	}

	m_session->GetPlayer()->Possess(pTarget);
	BlueSystemMessage(m_session, "Possessed "I64FMT, pTarget->GetGUID());
	switch( pTarget->GetTypeId() )
	{
		case TYPEID_PLAYER:
			sGMLog.writefromsession( m_session, "used possess command on PLAYER %s", static_cast< Player* >( pTarget )->GetName() );
			break;
		case TYPEID_UNIT:
			sGMLog.writefromsession( m_session, "used possess command on CREATURE %s, sqlid %u", static_cast< Creature* >( pTarget )->GetCreatureInfo()->Name, static_cast< Creature* >( pTarget )->GetSQL_id() );
			break;
	}
	return true;
}

bool ChatHandler::HandleNpcUnPossessCommand(const char * args, WorldSession * m_session)
{
	Creature * creature = getSelectedCreature(m_session);
 	m_session->GetPlayer()->UnPossess();

	if(creature)
	{
			// restart movement
			creature->GetAIInterface()->SetAIState(STATE_IDLE);
			creature->GetAIInterface()->WipeHateList();
			creature->GetAIInterface()->WipeTargetList();

			if(creature->m_spawn)
			{
				// return to respawn coords
				float x = creature->m_spawn->x;
				float y = creature->m_spawn->y;
				float z = creature->m_spawn->z;
				float o = creature->m_spawn->o;
				creature->GetAIInterface()->MoveTo(x, y, z, o);
			}
	}
	GreenSystemMessage(m_session, "Removed any possessed targets.");
	sGMLog.writefromsession(m_session, "used unpossess command");
	return true;
}

bool ChatHandler::HandleRehashCommand(const char * args, WorldSession * m_session)
{
	/*
	rehashes
	*/
	char msg[250];
	snprintf(msg, 250, "%s is rehashing config file.", m_session->GetPlayer()->GetName());
	sWorld.SendWorldWideScreenText(msg, 0);
	sWorld.SendWorldText(msg, 0);
	sWorld.Rehash(true);
	return true;
}

struct spell_thingo
{
	uint32 type;
	uint32 target;
};

list<SpellEntry*> aiagent_spells;
map<uint32, spell_thingo> aiagent_extra;

bool ChatHandler::HandleAIAgentDebugBegin(const char * args, WorldSession * m_session)
{
	QueryResult * result = WorldDatabase.Query("SELECT DISTINCT spell FROM ai_agents");
	if(!result) return false;

	do
	{
		SpellEntry * se = dbcSpell.LookupEntryForced(result->Fetch()[0].GetUInt32());
		if(se)
			aiagent_spells.push_back(se);
	} while(result->NextRow());
	delete result;

	for(list<SpellEntry*>::iterator itr = aiagent_spells.begin(); itr != aiagent_spells.end(); ++itr)
	{
		result = WorldDatabase.Query("SELECT * FROM ai_agents WHERE spell = %u", (*itr)->Id);
		Arcemu::Util::ARCEMU_ASSERT(   result != NULL );
		spell_thingo t;
		t.type = result->Fetch()[6].GetUInt32();
		t.target = result->Fetch()[7].GetUInt32();
		delete result;
		aiagent_extra[(*itr)->Id] = t;
	}

	GreenSystemMessage(m_session, "Loaded %u spells for testing.", aiagent_spells.size());
	return true;
}

SpellCastTargets SetTargets(SpellEntry * sp, uint32 type, uint32 targettype, Unit * dst, Creature * src)
{
	SpellCastTargets targets;
	targets.m_unitTarget = 0;
	targets.m_itemTarget = 0;
	targets.m_srcX = 0;
	targets.m_srcY = 0;
	targets.m_srcZ = 0;
	targets.m_destX = 0;
	targets.m_destY = 0;
	targets.m_destZ = 0;

	if(targettype == TTYPE_SINGLETARGET)
	{
		targets.m_targetMask = TARGET_FLAG_UNIT;
		targets.m_unitTarget = dst->GetGUID();
	}
	else if(targettype == TTYPE_SOURCE)
	{
		targets.m_targetMask = TARGET_FLAG_SOURCE_LOCATION;
		targets.m_srcX = src->GetPositionX();
		targets.m_srcY = src->GetPositionY();
		targets.m_srcZ = src->GetPositionZ();
	}
	else if(targettype == TTYPE_DESTINATION)
	{
		targets.m_targetMask = TARGET_FLAG_DEST_LOCATION;
		targets.m_destX = dst->GetPositionX();
		targets.m_destY = dst->GetPositionY();
		targets.m_destZ = dst->GetPositionZ();
	}

	return targets;
};

bool ChatHandler::HandleAIAgentDebugContinue(const char * args, WorldSession * m_session)
{
	uint32 count = atoi(args);
	if(!count) return false;

	Creature * pCreature = getSelectedCreature(m_session, true);
	if(!pCreature) return true;

	Player * pPlayer = m_session->GetPlayer();

	for(uint32 i = 0; i < count; ++i)
	{
		if(!aiagent_spells.size())
			break;

		SpellEntry * sp = *aiagent_spells.begin();
		aiagent_spells.erase(aiagent_spells.begin());
		BlueSystemMessage(m_session, "Casting %u, "MSG_COLOR_SUBWHITE"%u remaining.", sp->Id, aiagent_spells.size());

		map<uint32, spell_thingo>::iterator it = aiagent_extra.find(sp->Id);
		Arcemu::Util::ARCEMU_ASSERT(   it != aiagent_extra.end());

		SpellCastTargets targets;
		if(it->second.type == STYPE_BUFF)
			targets = SetTargets(sp, it->second.type, it->second.type, pCreature, pCreature );
		else
			targets = SetTargets(sp, it->second.type, it->second.type, pPlayer, pCreature );

		pCreature->GetAIInterface()->CastSpell(pCreature, sp, targets);
	}

	if(!aiagent_spells.size())
		RedSystemMessage(m_session, "Finished.");
	/*else
		BlueSystemMessage(m_session, "Got %u remaining.", aiagent_spells.size());*/
	return true;
}

bool ChatHandler::HandleAIAgentDebugSkip(const char * args, WorldSession * m_session)
{
	uint32 count = atoi(args);
	if(!count) return false;

	for(uint32 i = 0; i < count; ++i)
	{
		if(!aiagent_spells.size())
			break;

		aiagent_spells.erase(aiagent_spells.begin());
	}
	BlueSystemMessage(m_session, "Erased %u spells.", count);
	return true;
}

bool ChatHandler::HandleRenameGuildCommand(const char* args, WorldSession *m_session)
{
	Player * plr = getSelectedChar(m_session);
	if(!plr || !plr->GetGuildId() || !plr->GetGuild() || !args || !strlen(args))
		return false;

	Guild * pGuild = objmgr.GetGuildByGuildName(string(args));

	if(pGuild)
	{
		RedSystemMessage(m_session, "Guild name %s is already taken.", args);
		return false;
	}

	GreenSystemMessage(m_session, "Changed guild name of %s to %s. This will take effect next restart.", plr->GetGuild()->GetGuildName(), args);
	CharacterDatabase.Execute("UPDATE guilds SET `guildName` = \'%s\' WHERE `guildId` = '%u'", CharacterDatabase.EscapeString(string(args)).c_str(), plr->GetGuild()->GetGuildId());
	sGMLog.writefromsession(m_session, "Changed guild name of '%s' to '%s'", plr->GetGuild()->GetGuildName(), args);
	return true;
}

//People seem to get stuck in guilds from time to time. This should be helpful. -DGM
bool ChatHandler::HandleGuildRemovePlayerCommand(const char* args, WorldSession *m_session)
{
	Player * plr = getSelectedChar(m_session);
	if(!plr || !plr->GetGuildId() || !plr->GetGuild() || plr->GetGuild()->GetGuildLeader() == plr->GetLowGUID())
		return false;

	GreenSystemMessage(m_session, "Kicked %s from Guild: %s", plr->GetName(), plr->GetGuild()->GetGuildName());

	if(plr->GetLowGUID() != m_session->GetPlayer()->GetLowGUID())
		sGMLog.writefromsession(m_session, "Kicked %s from Guild %s", plr->GetName(), plr->GetGuild()->GetGuildName());

	plr->GetGuild()->RemoveGuildMember(plr->getPlayerInfo(), plr->GetSession());
	return true;
}

//-DGM
bool ChatHandler::HandleGuildDisbandCommand(const char* args, WorldSession *m_session)
{
	Player * plr = getSelectedChar(m_session);
	if(!plr || !plr->GetGuildId() || !plr->GetGuild())
		return false;

	GreenSystemMessage(m_session, "Disbanded Guild: %s", plr->GetGuild()->GetGuildName());
	sGMLog.writefromsession(m_session, "Disbanded Guild %s", plr->GetGuild()->GetGuildName());
	plr->GetGuild()->Disband();
	return true;
}

bool ChatHandler::HandleGuildJoinCommand(const char * args,WorldSession *m_session)
{
	if(!*args)
		return false;

	Player * ptarget = getSelectedChar(m_session);
	if(!ptarget) return false;

	if(ptarget->IsInGuild())
	{
		RedSystemMessage(m_session, "%s is already in a guild.", ptarget->GetName());
		return true;
	}

	Guild * pGuild = NULL;
	pGuild = objmgr.GetGuildByGuildName(string(args));

	if(pGuild)
	{
		pGuild->AddGuildMember(ptarget->getPlayerInfo(),m_session,-2);
		GreenSystemMessage(m_session, "You have joined the guild '%s'",pGuild->GetGuildName());
		sGMLog.writefromsession(m_session, "Force joined guild '%s'", pGuild->GetGuildName());
		return true;
	}

	return false;
}

//-DGM
bool ChatHandler::HandleGuildMembersCommand(const char* args, WorldSession *m_session)
{
	Player * plr = getSelectedChar(m_session);
	if(!plr || !plr->GetGuildId() || !plr->GetGuild())
		return false;
	GreenSystemMessage(m_session, "Now showing guild members for %s", plr->GetGuild()->GetGuildName());
	plr->GetGuild()->Lock();
	for(GuildMemberMap::iterator itr = plr->GetGuild()->GetGuildMembersBegin(); itr != plr->GetGuild()->GetGuildMembersEnd(); ++itr)
	{
		GuildMember *member = itr->second;
		if(!member || !member->pPlayer)
			continue;

		BlueSystemMessage(m_session, "%s (Rank: %s)", member->pPlayer->name, member->pRank->szRankName);
	}
	plr->GetGuild()->Unlock();
	return true;
}

bool ChatHandler::HandleArenaCreateTeamCommand(const char * args, WorldSession * m_session)
{
	uint32 arena_team_type;
	char name[1000];
	uint32 real_type;
	Player * plr = getSelectedChar(m_session, true);
	if(sscanf(args, "%u %[^\n]", &arena_team_type, name) != 2)
	{
		SystemMessage(m_session, "Invalid syntax.");
		return true;
	}

	switch(arena_team_type)
	{
	case 2:
		real_type= 0;
		break;
	case 3:
		real_type=1;
		break;
	case 5:
		real_type=2;
		break;
	default:
		SystemMessage(m_session, "Invalid arena team type specified.");
		return true;
	}

	if(!plr)
		return true;

	if(plr->m_arenaTeams[real_type] != NULL)
	{
		SystemMessage(m_session, "Already in an arena team of that type.");
		return true;
	}

	ArenaTeam * t = new ArenaTeam(real_type,objmgr.GenerateArenaTeamId());
	t->m_emblemStyle=22;
	t->m_emblemColour=4292133532UL;
	t->m_borderColour=4294931722UL;
	t->m_borderStyle=1;
	t->m_backgroundColour=4284906803UL;
	t->m_leader=plr->GetLowGUID();
	t->m_name = string(name);
	t->AddMember(plr->getPlayerInfo());
	objmgr.AddArenaTeam(t);
	SystemMessage(m_session, "created arena team.");
	return true;
}

bool ChatHandler::HandleArenaSetTeamLeaderCommand(const char * args, WorldSession * m_session)
{
	uint32 arena_team_type;
	uint32 real_type;
	Player * plr = getSelectedChar(m_session, true);
	if(sscanf(args, "%u", &arena_team_type) != 1)
	{
		SystemMessage(m_session, "Invalid syntax.");
		return true;
	}

	switch(arena_team_type)
	{
	case 2:
		real_type= 0;
		break;
	case 3:
		real_type=1;
		break;
	case 5:
		real_type=2;
		break;
	default:
		SystemMessage(m_session, "Invalid arena team type specified.");
		return true;
	}

	if(!plr)
		return true;

	if(plr->m_arenaTeams[real_type] == NULL)
	{
		SystemMessage(m_session, "Not in an arena team of that type.");
		return true;
	}

	ArenaTeam * t = plr->m_arenaTeams[real_type];
	t->SetLeader(plr->getPlayerInfo());
	SystemMessage(m_session, "player is now arena team leader.");
	return true;
}

bool ChatHandler::HandleArenaResetAllRatingsCommand(const char * args, WorldSession * m_session)
{
	objmgr.ResetArenaTeamRatings();
	return true;
}

bool ChatHandler::HandleWhisperBlockCommand(const char * args, WorldSession * m_session)
{
	if(m_session->GetPlayer()->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_GM))
		return false;

	m_session->GetPlayer()->SetFlag(PLAYER_FLAGS, PLAYER_FLAG_GM);
	return true;
}

bool ChatHandler::HandleGenderChanger(const char* args, WorldSession *m_session)
{
	uint8 gender;
	Player* target = objmgr.GetPlayer( (uint32)m_session->GetPlayer()->GetSelection() );
	if( target == NULL )
	{
		SystemMessage(m_session, "Select A Player first.");
		return true;
	}
	uint32 displayId = target->GetNativeDisplayId();
	if( *args == 0 )
		gender = target->getGender()== 1 ? 0 : 1;
	else
	{
		gender = (uint8)atoi( (char*)args );
		if( gender > 1 )
			gender = 1;
	}
	
	if( gender == target->getGender() )
	{
		SystemMessage( m_session, "%s's gender is already set to %s(%u).", target->GetName(), gender? "Female" : "Male", gender );
		return true;
	}

	target->setGender( gender );

	if( target->getGender() == 0 )
	{
		target->SetDisplayId(( target->getRace() == RACE_BLOODELF ) ? ++displayId : --displayId );
		target->SetNativeDisplayId(displayId );
	}
	else
	{
		target->SetDisplayId(( target->getRace() == RACE_BLOODELF )? --displayId : ++displayId );
		target->SetNativeDisplayId(displayId );
	}
	target->EventModelChange();

	SystemMessage( m_session, "Set %s's gender to %s(%u).", target->GetName(), gender ? "Female" : "Male", gender );
	return true;
}

bool ChatHandler::HandleDispelAllCommand(const char * args, WorldSession * m_session)
{
	uint32 pos= 0;
	if(*args)
		pos=atoi(args);

	Player * plr;

	sGMLog.writefromsession(m_session, "used dispelall command, pos %u", pos);

	PlayerStorageMap::const_iterator itr;
	objmgr._playerslock.AcquireReadLock();
	for (itr = objmgr._players.begin(); itr != objmgr._players.end(); itr++)
	{
		plr = itr->second;
		if(plr->GetSession() && plr->IsInWorld())
		{
			if(plr->GetMapMgr() != m_session->GetPlayer()->GetMapMgr())
			{
				sEventMgr.AddEvent( static_cast< Unit* >( plr ), &Unit::DispelAll, pos ? true : false, EVENT_PLAYER_CHECKFORCHEATS, 100, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
			}
			else
			{
				plr->DispelAll(pos?true:false);
			}
		}
	}
	sGMLog.writefromsession(m_session, "used mass dispel");
	objmgr._playerslock.ReleaseReadLock();

	BlueSystemMessage(m_session, "Dispel action done.");
	return true;
}

bool ChatHandler::HandleShowItems(const char * args, WorldSession * m_session)
{
	string q;
	Player * plr = getSelectedChar(m_session, true);
	if(!plr) return true;
	BlueSystemMessage(m_session, "Listing items for player %s",plr->GetName());
	int itemcount= 0;
	ItemIterator itr(plr->GetItemInterface());
	itr.BeginSearch();
	for(; !itr.End(); itr.Increment())
	{
		if(!(*itr))
			return false;
		itemcount++;
		SendItemLinkToPlayer((*itr)->GetProto(), m_session, true, plr, m_session->language);
	}
	itr.EndSearch();
	BlueSystemMessage(m_session, "Listed %d items for player %s",itemcount,plr->GetName());

	sGMLog.writefromsession(m_session, "used show items command on %s,", plr->GetName());

   return true;
}

bool ChatHandler::HandleShowSkills(const char * args, WorldSession * m_session)
{
	Player * plr = getSelectedChar(m_session, true);
	if(!plr)
		return true;

	BlueSystemMessage(m_session, "Listing items for player %s",plr->GetName());
	int itemcount= 0;
	SkillIterator itr2(plr);
	itr2.BeginSearch();
	for(; !itr2.End(); itr2.Increment())
	{
		itemcount++;
		SystemMessage(m_session, "Skill %u %s %u/%u", itr2->Skill->id, itr2->Skill->Name ,itr2->CurrentValue, itr2->MaximumValue);
	}
	itr2.EndSearch();
	BlueSystemMessage(m_session, "Listed %d skills for player %s",itemcount,plr->GetName());
	sGMLog.writefromsession(m_session, "used show skills command on %s,", plr->GetName());

	return true;
}

bool ChatHandler::HandleCollisionTestIndoor(const char * args, WorldSession * m_session)
{
	if (sWorld.Collision) {
		Player * plr = m_session->GetPlayer();
		const LocationVector & loc = plr->GetPosition();
		bool res = CollideInterface.IsIndoor(plr->GetMapId(), loc.x, loc.y, loc.z + 2.0f);
		SystemMessage(m_session, "Result was: %s.", res ? "indoors" : "outside");
		return true;
	} else {
		SystemMessage(m_session, "Collision is not enabled.");
		return true;
	}
}

bool ChatHandler::HandleCollisionTestLOS(const char * args, WorldSession * m_session)
{
	if (sWorld.Collision) {
		Object * pObj = NULL;
		Creature * pCreature = getSelectedCreature(m_session, false);
		Player * pPlayer = getSelectedChar(m_session, false);
		if(pCreature)
			pObj = pCreature;
		else if(pPlayer)
			pObj = pPlayer;

		if(pObj == NULL)
		{
			SystemMessage(m_session, "Invalid target.");
			return true;
		}

		const LocationVector & loc2 = pObj->GetPosition();
		const LocationVector & loc1 = m_session->GetPlayer()->GetPosition();
		bool res = CollideInterface.CheckLOS(pObj->GetMapId(), loc1.x, loc1.y, loc1.z, loc2.x, loc2.y, loc2.z);
		bool res2 = CollideInterface.CheckLOS(pObj->GetMapId(), loc1.x, loc1.y, loc1.z+2.0f, loc2.x, loc2.y, loc2.z+2.0f);
		bool res3 = CollideInterface.CheckLOS(pObj->GetMapId(), loc1.x, loc1.y, loc1.z+5.0f, loc2.x, loc2.y, loc2.z+5.0f);
		SystemMessage(m_session, "Result was: %s %s %s.", res ? "in LOS" : "not in LOS", res2 ? "in LOS" : "not in LOS", res3 ? "in LOS" : "not in LOS");
		return true;
	} else {
		SystemMessage(m_session, "Collision is not enabled.");
		return true;
	}
}

bool ChatHandler::HandleGetDeathState(const char * args, WorldSession * m_session)
{
    Player* SelectedPlayer = getSelectedChar(m_session, true);
    if(!SelectedPlayer)
        return true;

    SystemMessage(m_session, "Death State: %d",SelectedPlayer->getDeathState());
        return true;
}


bool ChatHandler::HandleCollisionGetHeight(const char * args, WorldSession * m_session)
{
	if (sWorld.Collision) {
		Player * plr = m_session->GetPlayer();
		float radius = 5.0f;

		float posX = plr->GetPositionX();
		float posY = plr->GetPositionY();
		float posZ = plr->GetPositionZ();
		float ori  = plr->GetOrientation();

		LocationVector src(posX,posY,posZ);

		LocationVector dest(posX+(radius*(cosf(ori))),posY+(radius*(sinf(ori))),posZ);
		//LocationVector destest(posX+(radius*(cosf(ori))),posY+(radius*(sinf(ori))),posZ);


		float z = CollideInterface.GetHeight(plr->GetMapId(), posX, posY, posZ + 2.0f);
		float z2 = CollideInterface.GetHeight(plr->GetMapId(), posX, posY, posZ + 5.0f);
		float z3 = CollideInterface.GetHeight(plr->GetMapId(), posX, posY, posZ);
		float z4 = plr->GetMapMgr()->GetLandHeight(plr->GetPositionX(),plr->GetPositionY());
		bool fp = CollideInterface.GetFirstPoint( plr->GetMapId(), src, dest, dest, -1.5f );

		SystemMessage(m_session, "Results were: %f(offset2.0f) | %f(offset5.0f) | %f(org) | landheight:%f | target radius5 FP:%d", z, z2, z3,z4,fp);
		return true;
	} else {
		SystemMessage(m_session, "Collision is not enabled.");
		return true;
	}
}
bool ChatHandler::HandleLevelUpCommand(const char* args, WorldSession *m_session)
{
	int levels = 0;

	if (!*args)
		levels = 1;
	else
		levels = atoi(args);

	if(levels <= 0)
		return false;

	Player *plr = getSelectedChar(m_session, true);

	if(!plr) plr = m_session->GetPlayer();

	if(!plr) return false;

	sGMLog.writefromsession(m_session, "used level up command on %s, with %u levels", plr->GetName(), levels);

	levels += plr->getLevel();

	if(levels>PLAYER_LEVEL_CAP)
		levels=PLAYER_LEVEL_CAP;

	LevelInfo * inf = objmgr.GetLevelInfo(plr->getRace(),plr->getClass(),levels);
	if(!inf)
		return false;
	plr->ApplyLevelInfo(inf,levels);
	if(plr->getClass() == WARLOCK)
	{
		std::list<Pet*> summons = plr->GetSummons();
		for(std::list<Pet*>::iterator itr = summons.begin(); itr != summons.end(); ++itr)
		{
			if( (*itr)->IsInWorld() && (*itr)->isAlive() )
			{
				(*itr)->setLevel(levels);
				(*itr)->ApplyStatsForLevel();
				(*itr)->UpdateSpellList();
			}
		}
	}

	WorldPacket data;
	std::stringstream sstext;
	sstext << "You have been leveled up to Level " << levels << '\0';
	SystemMessage(plr->GetSession(), sstext.str().c_str());

	plr->Social_TellFriendsOnline();

	return true;
}

bool ChatHandler::HandleFixScaleCommand(const char * args, WorldSession * m_session)
{
	Creature * pCreature = getSelectedCreature(m_session, true);
	if( pCreature == NULL )
		return true;

	float sc = (float)atof(args);
	if(sc < 0.1f)
	{
		return false;
	}

	pCreature->SetScale(  sc);
	pCreature->GetProto()->Scale = sc;
	WorldDatabase.Execute("UPDATE creature_proto SET scale = '%f' WHERE entry = %u", sc, pCreature->GetEntry());
	return true;
}

bool ChatHandler::HandleAddTrainerSpellCommand( const char * args, WorldSession * m_session )
{
	Creature * pCreature = getSelectedCreature(m_session, true);
	if( pCreature == NULL )
		return true;

	uint32 spellid, cost, reqspell, reqlevel, delspell;
	if( sscanf(args, "%u %u %u %u %u", &spellid, &cost, &reqspell, &reqlevel, &delspell) != 5 )
		return false;

	Trainer * pTrainer =  pCreature->GetTrainer();
	if( pTrainer == NULL )
	{
		RedSystemMessage(m_session, "Target is not a trainer.");
		return true;
	}

	SpellEntry* pSpell = dbcSpell.LookupEntryForced(spellid);
	if(pSpell== NULL)
	{
		RedSystemMessage(m_session, "Invalid spell.");
		return true;
	}

	if( pSpell->Effect[0] == SPELL_EFFECT_INSTANT_KILL || pSpell->Effect[1] == SPELL_EFFECT_INSTANT_KILL || pSpell->Effect[2] == SPELL_EFFECT_INSTANT_KILL )
	{
		RedSystemMessage(m_session, "No. You're not doing that.");
		return true;
	}

	TrainerSpell sp;
	sp.Cost = cost;
	sp.IsProfession = false;
	sp.pLearnSpell = pSpell;
	sp.pCastRealSpell = NULL;
	sp.pCastSpell = NULL;
	sp.RequiredLevel = reqlevel;
	sp.RequiredSpell = reqspell;
	sp.DeleteSpell = delspell;

	pTrainer->Spells.push_back(sp);
	pTrainer->SpellCount++;

	SystemMessage(m_session, "Added spell %u (%s) to trainer.", pSpell->Id, pSpell->Name);
	sGMLog.writefromsession(m_session, "added spell %u to trainer %u", spellid, pCreature->GetEntry());
	WorldDatabase.Execute("INSERT INTO trainer_spells VALUES(%u, %u, %u, %u, %u, %u, %u, %u, %u, %u)",
		pCreature->GetEntry(), (int)0, pSpell->Id, cost, reqspell, (int)0, (int)0, reqlevel, delspell, (int)0);

	return true;
}

bool ChatHandler::HandleSetTitle( const char *args, WorldSession *m_session )
{
	Player* plr = getSelectedChar( m_session, true );
	if( plr == NULL )
		return false;

	int32 title = atol( args );
	if( title > int32( PVPTITLE_END ) || title < - int32( PVPTITLE_END ) )
	{
		RedSystemMessage( m_session, "Argument %i is out of range!", title );
		return false;
	}
	if( title == 0 )
	{
		plr->SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES, 0 );
		plr->SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES1, 0 );
	}
	else if( title > 0 )
		plr->SetKnownTitle( static_cast< RankTitles >( title ), true );
	else
		plr->SetKnownTitle( static_cast< RankTitles >( -title ), false );

	plr->SetChosenTitle(0 ); // better remove chosen one
	SystemMessage( m_session, "Title has been %s.", title > 0 ? "set" : "reset" );

    std::stringstream logtext;
    logtext << "has ";
    if( title > 0 )
        logtext << "set the title field of " << plr->GetName() << "to " << title << ".";
    else
        logtext << "reset the title field of " << plr->GetName();

    sGMLog.writefromsession(m_session, logtext.str().c_str());
	return true;
}

bool ChatHandler::HandleNPCLootCommand(const char* args, WorldSession* m_session)
{
	Creature * pCreature = getSelectedCreature(m_session, true);
	if( pCreature == NULL )
	{
		return false;
	}

	QueryResult* _result = WorldDatabase.Query("SELECT itemid, normal10percentchance, heroic10percentchance, normal25percentchance, heroic25percentchance, mincount, maxcount FROM loot_creatures WHERE entryid=%u;", pCreature->GetEntry());
	if( _result != NULL )
	{
		Field* _field;
		std::stringstream ss;
		ItemPrototype* proto;
		string color;
		int32 minQuality = 0;
		uint8 numFound = 0;

		if( *args )
		{
			minQuality = atol(args);
		}

		do
		{
			_field = _result->Fetch();
			ss.str("");
			proto = ItemPrototypeStorage.LookupEntry( _field[0].GetUInt32() );
			if( proto == NULL || (int32)proto->Quality < minQuality )
			{
				continue;
			}
			++numFound;
			ss << "(N10 " << _field[1].GetFloat() << "%%) (N25 " << _field[3].GetFloat() << "%%) (H10 " << _field[2].GetFloat() << "%%) (H25 " << _field[4].GetFloat() << "%%): ";

			switch( proto->Quality )
			{
				case 0: //Poor,gray
					color = "cff9d9d9d";
					break;
				case 1: //Common,white
					color = "cffffffff";
					break;
				case 2: //Uncommon,green
					color = "cff1eff00";
					break;
				case 3: //Rare,blue
					color = "cff0070dd";
					break;
				case 4: //Epic,purple
					color = "cffa335ee";
					break;
				case 5: //Legendary,orange
					color = "cffff8000";
					break;
				case 6: //Artifact,pale gold
					color = "c00fce080";
					break;
				case 7: //Heirloom,pale gold
					color = "c00fce080";
					break;
				default:
					color = "cff9d9d9d";
					break;
			}

			ss << "|" << color.c_str() << "|Hitem:" << proto->ItemId << ":0:0:0:0:0:0:0|h[" << proto->Name1;
			ss << "]|h|r (" << _field[5].GetUInt32() << "-" << _field[6].GetUInt32() << ")";
			SystemMessage( m_session, ss.str().c_str(), '%', '%' );
		} while( _result->NextRow() && (numFound <= 25) );
		delete _result;
		if( numFound > 25 )
		{
			SystemMessage( m_session, "More than 25 results found." );
		}
		else
		{
			SystemMessage( m_session, "%lu results found.", numFound );
		}
	}
	else
	{
		SystemMessage( m_session, "0 results found." );
	}
	return true;
}
