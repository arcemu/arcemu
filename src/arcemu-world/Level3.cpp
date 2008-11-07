
/*
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
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

/////////////////////////////////////////////////
//  Admin Chat Commands
//

#include "StdAfx.h"
#include "ObjectMgr.h"
#include "Master.h"

bool ChatHandler::HandleWorldPortCommand(const char* args, WorldSession *m_session)
{
	float x, y, z;
	uint32 mapid;
	if(sscanf(args, "%u %f %f %f", (unsigned int*)&mapid, &x, &y, &z) != 4)
		return false;

	if(x >= _maxX || x <= _minX || y <= _minY || y >= _maxY)
		return false;

	LocationVector vec(x, y, z);
	m_session->GetPlayer()->SafeTeleport(mapid, 0, vec);
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
	if (!*args)
		return false;

	Player *plr = getSelectedChar(m_session, true);
	if(!plr)
	{
		plr = m_session->GetPlayer();
		SystemMessage(m_session, "Auto-targeting self.");
	}
	if(!plr) return false;


	if(stricmp(args, "all")==0)
	{
		sGMLog.writefromsession(m_session, "taught %s all spells.", plr->GetName());
		static uint32 spellarray[DRUID+1][9999] = {
			{ 0 },		// CLASS 0
			{ 71, 2458, 355, 469, 674, 676, 750, 871, 1161, 1680, 1719, 2048, 2565, 2687, 3411, 5246, 6554, 7620, 11578, 11585, 12678, 18499, 20230, 23920, 25203, 25208, 25212, 25225, 25231, 25236, 25242, 25264, 25266, 25275, 29704, 29707, 30357, 34428, 0 },		// CLASS 1
			{ 750, 1020, 1038, 1044, 1152, 3127, 4987, 5502, 5573, 7620, 10278, 10308, 10322, 10324, 10326, 19746, 19752, 20271, 20347, 20348, 20356, 20772, 20773, 25780, 25895, 25898, 27136, 27137, 27138, 27139, 27140, 27141, 27142, 27143, 27144, 27145, 27148, 27149, 27150, 27151, 27152, 27153, 27154, 27155, 27158, 27169, 27173, 27180, 31789, 31801, 31884, 31895, 32223, 33388, 33391, 33776, 34090, 34091, 0 },		// CLASS 2
			{ 136, 674, 781, 883, 982, 1002, 1130, 1462, 1494, 1495, 1499, 1510, 1513, 1515, 1543, 1978, 2641, 2643, 2974, 3018, 3034, 3043, 3044, 3045, 3111, 3127, 3661, 3662, 5116, 5118, 5384, 6197, 6991, 13159, 13161, 13163, 13165, 13542, 13543, 13544, 13549, 13550, 13551, 13552, 13553, 13554, 13555, 13795, 13809, 13813, 14260, 14261, 14262, 14263, 14264, 14265, 14266, 14267, 14268, 14269, 14270, 14271, 14272, 14273, 14274, 14279, 14280, 14281, 14282, 14283, 14284, 14285, 14286, 14287, 14288, 14289, 14290, 14294, 14295, 14302, 14303, 14304, 14305, 14310, 14311, 14316, 14317, 14318, 14319, 14320, 14321, 14322, 14323, 14324, 14325, 14326, 14327, 15629, 15630, 15631, 15632, 19801, 19878, 19879, 19880, 19882, 19883, 19884, 19885, 20043, 20190, 20736, 25294, 25295, 25296, 27014, 27015, 27016, 27018, 27019, 27020, 27021, 27022, 27023, 27025, 27044, 27045, 27046, 34026, 34074, 34120, 34477, 34600, 36916, 0 },		// CLASS 3
			{ 674, 921, 1725, 1787, 1804, 1833, 1842, 1860, 2094, 2836, 2842, 3127, 5938, 6774, 8643, 8681, 11297, 11305, 26669, 26679, 26862, 26863, 26865, 26866, 26867, 26884, 26889, 27441, 27448, 31224, 32684, 38764, 38768, 0 },		// CLASS 4
			{ 17, 139, 453, 527, 528, 552, 586, 588, 589, 591, 592, 594, 596, 598, 600, 602, 605, 970, 976, 984, 988, 992, 996, 1004, 1006, 1243, 1244, 1245, 1706, 2006, 2010, 2052, 2053, 2054, 2055, 2060, 2061, 2096, 2767, 2791, 3747, 6060, 6063, 6064, 6065, 6066, 6074, 6075, 6076, 6077, 6078, 7128, 8092, 8102, 8103, 8104, 8105, 8106, 8122, 8124, 8129, 8131, 8192, 9472, 9473, 9474, 9484, 9485, 9578, 9579, 9592, 10874, 10875, 10876, 10880, 10881, 10888, 10890, 10892, 10893, 10894, 10898, 10899, 10900, 10901, 10909, 10911, 10912, 10915, 10916, 10917, 10927, 10928, 10929, 10933, 10934, 10937, 10938, 10941, 10942, 10945, 10946, 10947, 10951, 10952, 10953, 10955, 10957, 10960, 10961, 10963, 10964, 10965, 14914, 15261, 15262, 15263, 15264, 15265, 15266, 15267, 19289, 19291, 19292, 19293, 20770, 25210, 25213, 25217, 25218, 25221, 25222, 25233, 25235, 25308, 25314, 25315, 25316, 25363, 25364, 25367, 25368, 25372, 25375, 25379, 25380, 25384, 25389, 25429, 25431, 25435, 25450, 25596, 32375, 32379, 32546, 32996, 33076, 34433, 34863, 34864, 34865, 34866, 0 },		// CLASS 5
			{ 0 },		// CLASS 6
			{ 131, 324, 325, 332, 370, 421, 526, 529, 546, 547, 548, 556, 905, 913, 915, 930, 939, 943, 945, 959, 1064, 1535, 2008, 2484, 2645, 2860, 2870, 3599, 5394, 5675, 5730, 6041, 6196, 6363, 6364, 6365, 6375, 6377, 6390, 6391, 6392, 6495, 8004, 8005, 8008, 8010, 8012, 8017, 8024, 8027, 8030, 8033, 8038, 8042, 8044, 8045, 8046, 8050, 8052, 8053, 8056, 8058, 8071, 8075, 8134, 8143, 8154, 8155, 8160, 8161, 8166, 8170, 8177, 8181, 8184, 8190, 8227, 8232, 8235, 8249, 8498, 8499, 8512, 8835, 10391, 10392, 10395, 10396, 10406, 10407, 10408, 10412, 10413, 10414, 10427, 10428, 10431, 10432, 10437, 10438, 10442, 10447, 10448, 10456, 10462, 10463, 10466, 10467, 10468, 10472, 10473, 10478, 10479, 10486, 10495, 10496, 10497, 10526, 10537, 10538, 10585, 10595, 10600, 10601, 10605, 10613, 10622, 10623, 10627, 11314, 11315, 15107, 15111, 15112, 15207, 15208, 16339, 16341, 16342, 16355, 16356, 16362, 16387, 20608, 20609, 20610, 20776, 20777, 24398, 25357, 25359, 25361, 25391, 25396, 25420, 25422, 25423, 25439, 25442, 25448, 25449, 25454, 25457, 25464, 25469, 25472, 25489, 25500, 25505, 25508, 25509, 25525, 25528, 25533, 25546, 25547, 25557, 25560, 25563, 25567, 25570, 25574, 25577, 25908, 29228, 33736, 36936, 0 },		// CLASS 7
			{ 10, 66, 116, 118, 120, 122, 130, 143, 145, 205, 475, 543, 587, 597, 604, 759, 837, 865, 990, 1008, 1449, 1459, 1460, 1461, 1463, 1953, 2120, 2121, 2136, 2137, 2138, 2139, 2855, 2948, 3140, 3552, 5143, 5144, 5145, 5504, 5505, 5506, 6117, 6127, 6129, 6131, 6141, 6143, 7300, 7301, 7302, 7320, 7322, 8400, 8401, 8402, 8406, 8407, 8408, 8412, 8413, 8416, 8417, 8422, 8423, 8427, 8437, 8438, 8439, 8444, 8445, 8446, 8450, 8451, 8455, 8457, 8458, 8461, 8462, 8492, 8494, 8495, 10053, 10054, 10138, 10139, 10140, 10144, 10145, 10148, 10149, 10150, 10151, 10156, 10157, 10159, 10160, 10161, 10169, 10170, 10173, 10174, 10177, 10179, 10180, 10181, 10185, 10186, 10187, 10191, 10192, 10193, 10197, 10199, 10201, 10202, 10205, 10206, 10207, 10211, 10212, 10215, 10216, 10219, 10220, 10223, 10225, 10230, 12051, 12824, 12825, 12826, 22782, 22783, 25304, 25306, 25345, 27070, 27071, 27072, 27073, 27074, 27075, 27078, 27079, 27080, 27082, 27085, 27086, 27087, 27088, 27101, 27124, 27125, 27126, 27128, 27130, 27131, 28609, 28612, 30449, 30451, 30455, 30482, 32796, 33717, 33944, 33946, 37420, 38699, 0 },		// CLASS 8
			{ 126, 132, 172, 348, 603, 688, 689, 691, 693, 695, 696, 697, 698, 699, 702, 704, 705, 706, 707, 709, 710, 712, 755, 980, 1010, 1014, 1086, 1088, 1094, 1098, 1106, 1108, 1120, 1122, 1454, 1455, 1456, 1490, 1714, 1949, 2362, 2941, 3698, 3699, 3700, 5138, 5484, 5500, 5676, 5697, 5699, 5740, 5782, 5784, 6201, 6202, 6205, 6213, 6215, 6217, 6219, 6222, 6223, 6226, 6229, 6353, 6366, 6789, 7641, 7646, 7648, 7651, 7658, 7659, 8288, 8289, 11659, 11660, 11661, 11665, 11667, 11668, 11671, 11672, 11675, 11677, 11678, 11683, 11684, 11687, 11688, 11689, 11693, 11694, 11695, 11699, 11700, 11703, 11704, 11707, 11708, 11711, 11712, 11713, 11717, 11719, 11721, 11722, 11725, 11726, 11729, 11730, 11733, 11734, 11735, 11739, 11740, 17727, 17728, 17862, 17919, 17920, 17921, 17922, 17923, 17924, 17925, 17926, 17928, 17937, 18540, 18647, 20752, 20755, 20756, 20757, 23161, 25307, 25309, 25311, 27209, 27210, 27211, 27212, 27213, 27215, 27216, 27217, 27218, 27219, 27220, 27221, 27222, 27223, 27224, 27226, 27228, 27229, 27230, 27238, 27243, 27259, 27260, 28172, 28176, 28189, 28610, 29722, 29858, 29893, 30459, 30545, 30908, 30909, 30910, 32231, 0 },		// CLASS 9
			{ 0 },		// CLASS 10
			{ 99, 339, 467, 740, 768, 769, 770, 774, 778, 779, 780, 782, 783, 1058, 1062, 1066, 1075, 1079, 1082, 1126, 1178, 1430, 1735, 1822, 1823, 1824, 1850, 2090, 2091, 2637, 2782, 2893, 2908, 2912, 3025, 3029, 3137, 3627, 5177, 5178, 5179, 5180, 5186, 5187, 5188, 5189, 5195, 5196, 5201, 5209, 5211, 5215, 5217, 5221, 5225, 5229, 5232, 5234, 6756, 6778, 6780, 6783, 6785, 6787, 6793, 6795, 6798, 6800, 6807, 6808, 6809, 8903, 8905, 8907, 8910, 8914, 8918, 8921, 8924, 8925, 8926, 8927, 8928, 8929, 8936, 8938, 8939, 8940, 8941, 8946, 8949, 8950, 8951, 8955, 8972, 8983, 8992, 8998, 9005, 9490, 9492, 9493, 9634, 9745, 9747, 9749, 9750, 9752, 9754, 9756, 9758, 9821, 9823, 9827, 9829, 9830, 9833, 9834, 9835, 9839, 9840, 9841, 9845, 9846, 9849, 9850, 9852, 9853, 9856, 9857, 9858, 9862, 9863, 9866, 9867, 9875, 9876, 9880, 9881, 9884, 9885, 9888, 9889, 9894, 9896, 9898, 9901, 9904, 9907, 9908, 9910, 9912, 9913, 16914, 16952, 16954, 16958, 16961, 17401, 17402, 18657, 18658, 18960, 20484, 20719, 20739, 20742, 20747, 20748, 22568, 22570, 22812, 22827, 22828, 22829, 22842, 22895, 22896, 24248, 25297, 25298, 25299, 26978, 26979, 26980, 26981, 26982, 26983, 26984, 26985, 26986, 26987, 26988, 26989, 26990, 26992, 26993, 26994, 26995, 26996, 26997, 26998, 26999, 27000, 27001, 27002, 27003, 27005, 27006, 27008, 27012, 29166, 31018, 33357, 33745, 33763, 33786, 33943, 34090, 0 },		// CLASS 11
		};

		uint32 c = plr->getClass();
		for(uint32 i = 0; spellarray[c][i] != 0; ++i)
		{
			plr->addSpell(spellarray[c][i]);
		}

		static uint32 paladinspellarray[RACE_DRAENEI+1][2] = {
			{ 0 },		// RACE 0
			{ 13819, 0 },		// HUMAN
			{ 0 },		// ORC
			{ 13819, 0 },		// DWARF
			{ 0 },		// NIGHTELF
			{ 0 },		// UNDEAD
			{ 0 },		// TAUREN
			{ 0 },		// GNOME
			{ 0 },		// TROLL
			{ 34769, 0 },		// BLOODELF
			{ 13819, 0 },		// DRAENEI
		};

		static uint32 priestspellarray[RACE_DRAENEI+1][16] = {
			{ 0 },		// RACE 0
			{ 13896, 13908, 19236, 19238, 19240, 19241, 19242, 19243, 19271, 19273, 19274, 19275, 25437, 25441, 0 },		// HUMAN
			{ 0 },		// ORC
			{ 13908, 19236, 19238, 19240, 44041, 19241, 19242, 19243, 25437, 44043, 44044, 44045, 44046, 44047, 0 },		// DWARF
			{ 2651, 10797, 19296, 19299, 19302, 19303, 19304, 19305, 25446, 0 },		// NIGHTELF
			{ 2652, 2944, 19261, 19262, 19264, 19265, 19266, 19276, 19277, 19278, 19279, 19280, 25461, 25467, 0 },		// UNDEAD
			{ 0 },		// TAUREN
			{ 0 },		// GNOME
			{ 9035, 18137, 19281, 19282, 19283, 19284, 19285, 19308, 19309, 19310, 19311, 19312, 25470, 25477, 0 },		// TROLL
			{ 2652, 19261, 19262, 19264, 19265, 19266, 25461, 32676, 0 },		// BLOODELF
			{ 32548, 44041, 44043, 44044, 44045, 44046, 44047, 0 },		// DRAENEI
		};

		static uint32 shamanspellarray[RACE_DRAENEI+1][2] = {
			{ 0 },		// RACE 0
			{ 0 },		// HUMAN
			{ 2825, 0 },		// ORC
			{ 0 },		// DWARF
			{ 0 },		// NIGHTELF
			{ 0 },		// UNDEAD
			{ 2825, 0 },		// TAUREN
			{ 0 },		// GNOME
			{ 2825, 0 },		// TROLL
			{ 0 },		// BLOODELF
			{ 32182, 0 },		// DRAENEI
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
		switch(c)
		{
		case PALADIN:
			for( uint32 i = 0; paladinspellarray[r][i] != 0; ++i )
			{
				plr->addSpell( paladinspellarray[r][i] );
			}
			break;
		case PRIEST:
			for( uint32 i = 0; priestspellarray[r][i] != 0; ++i )
			{
				plr->addSpell( priestspellarray[r][i] );
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

	SpellEntry * sp = dbcSpell.LookupEntry(spell);
	if(!sp)
	{
		SystemMessage(m_session, "Invalid spell %u", spell);
		return true;
	}

	if(!plr->GetSession()->HasGMPermissions() && (sp->Effect[0]==SPELL_EFFECT_INSTANT_KILL||sp->Effect[1]==SPELL_EFFECT_INSTANT_KILL||sp->Effect[2]==SPELL_EFFECT_INSTANT_KILL))
	{
		SystemMessage(m_session, "don't be an idiot and teach players instakill spells. this action has been logged.");
		return true;
	}

	if (plr->HasSpell(spell)) // check to see if char already knows
	{
		std::string OutStr = plr->GetName();
		OutStr += " already knows that spell.";

		SystemMessage(m_session, OutStr.c_str());
		return true;
	}

	plr->addSpell(spell);
	sGMLog.writefromsession(m_session, "taught %s spell %u", plr->GetName(), spell);
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
	SelectedPlayer->SetUInt32Value(UNIT_FIELD_HEALTH, SelectedPlayer->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
	SelectedPlayer->SetUInt32Value(UNIT_FIELD_POWER1, SelectedPlayer->GetUInt32Value(UNIT_FIELD_MAXPOWER1));
	SelectedPlayer->SetUInt32Value(UNIT_FIELD_POWER4, SelectedPlayer->GetUInt32Value(UNIT_FIELD_MAXPOWER4));


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

	if (strcmp(args, "on") == 0)
	{
		SystemMessage(m_session, "%s has explored all zones now.", chr->GetName());
		SystemMessage(m_session, "%s has explored all zones for you.", m_session->GetPlayer()->GetName());
		sGMLog.writefromsession( m_session, "explored all zones for player %s", chr->GetName() );
	}
	else if(strcmp(args, "off") == 0)
	{
		SystemMessage(m_session, "%s has no more explored zones.", chr->GetName());
		SystemMessage(m_session, "%s has hidden all zones from you.", m_session->GetPlayer()->GetName());
		sGMLog.writefromsession( m_session, "hid all zones for player %s", chr->GetName() );
	}
	else 
		return false;

	for (uint8 i=0; i<64; i++)
	{
		if (strcmp(args, "on") == 0)
		{
			chr->SetFlag(PLAYER_EXPLORED_ZONES_1+i,0xFFFFFFFF);
		}
		else if(strcmp(args, "off") == 0)
		{
			chr->SetFlag(PLAYER_EXPLORED_ZONES_1+i,0);
		}
	}
	return true;
}

bool ChatHandler::HandleBanCharacterCommand(const char* args, WorldSession *m_session)
{
	if(!*args)
		return false;

	// this is rather complicated due to ban reasons being able to have spaces. so we'll have to some c string magic
	// rather than just sscanf'ing it.
	char * pCharacter = (char*)args;
	char * pBanDuration = strchr(pCharacter, ' ');
	PlayerInfo * pInfo = NULL;
	if(pBanDuration == NULL)
		return false;

	char * pReason = strchr(pBanDuration+1, ' ');
	if(pReason == NULL)
		return false;

	// zero them out to create sepearate strings.
	*pBanDuration = 0;
	++pBanDuration;
	*pReason = 0;
	++pReason;

	int32 BanTime = GetTimePeriodFromString(pBanDuration);
	if(BanTime < 1)
		return false;

	Player * pPlayer = objmgr.GetPlayer(pCharacter, false);
	if(pPlayer == NULL)
	{
		pInfo = objmgr.GetPlayerInfoByName(pCharacter);
		if(pInfo == NULL)
		{
			SystemMessage(m_session, "Player not found.");
			return true;
		}

		SystemMessage(m_session, "Banning player '%s' in database for '%s'.", pCharacter, pReason);
		string escaped_reason = CharacterDatabase.EscapeString(string(pReason));
		
		CharacterDatabase.Execute("UPDATE characters SET banned = %u, banReason = '%s' WHERE guid = %u",
			BanTime ? BanTime+(uint32)UNIXTIME : 1, escaped_reason.c_str(), pInfo->guid);
	}
	else
	{
		SystemMessage(m_session, "Banning player '%s' ingame for '%s'.", pCharacter, pReason);
		string sReason = string(pReason);
		uint32 uBanTime = BanTime ? BanTime+(uint32)UNIXTIME : 1;
		pPlayer->SetBanned(uBanTime, sReason);
		pInfo = pPlayer->m_playerInfo;
	}

	SystemMessage(m_session, "This ban is due to expire %s%s.", BanTime ? "on " : "", BanTime ? ConvertTimeStampToDataTime(BanTime+(uint32)UNIXTIME).c_str() : "Never");
	if(pPlayer)
	{
		SystemMessage(m_session, "Kicking %s.", pPlayer->GetName());
		pPlayer->Kick();
	}

	sGMLog.writefromsession(m_session, "banned %s, reason %s, for %s", pCharacter, pReason, BanTime ? ConvertTimeStampToString(BanTime).c_str() : "ever");
	char msg[200];
	snprintf( msg, 200, "%sGM: %s has been banned by %s for %s. Reason: %s", MSG_COLOR_RED, pCharacter, m_session->GetPlayer()->GetName(), BanTime ? ConvertTimeStampToString( BanTime ).c_str() : "ever", pReason );
	sWorld.SendWorldText( msg, NULL );
	if( sWorld.m_banTable && pInfo )
	{
		CharacterDatabase.Execute("INSERT INTO %s VALUES('%s', '%s', %u, %u, '%s')", sWorld.m_banTable, m_session->GetPlayer()->GetName(), pInfo->name, (uint32)UNIXTIME, (uint32)UNIXTIME + BanTime, CharacterDatabase.EscapeString(string(pReason)).c_str() );
	}
	return true;
}

bool ChatHandler::HandleUnBanCharacterCommand(const char* args, WorldSession *m_session)
{
	if(!*args)
		return false;

	char Character[255];
	if(sscanf(args, "%s", Character) == 0)
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
	uint32 guid = GUID_LOPART(m_session->GetPlayer()->GetSelection());
	Creature *crt = getSelectedCreature(m_session);
	if(!crt) return false;
	if(crt->GetCreatureInfo())
		BlueSystemMessage(m_session, "Showing creature info for %s", crt->GetCreatureInfo()->Name);
	SystemMessage(m_session, "GUID: %d", guid);
	SystemMessage(m_session, "Faction: %d", crt->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
	SystemMessage(m_session, "NPCFlags: %d", crt->GetUInt32Value(UNIT_NPC_FLAGS));
	SystemMessage(m_session, "DisplayID: %d", crt->GetUInt32Value(UNIT_FIELD_DISPLAYID));
	if(crt->m_faction)
		SystemMessage(m_session, "Combat Support: 0x%.3X", crt->m_faction->FriendlyMask);
	SystemMessage(m_session, "Health (cur/max): %d/%d", crt->GetUInt32Value(UNIT_FIELD_HEALTH), crt->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
	SystemMessage(m_session, "Mana (cur/max): %d/%d", crt->GetUInt32Value(UNIT_FIELD_POWER1), crt->GetUInt32Value(UNIT_FIELD_MAXPOWER1));
	SystemMessage(m_session, "Armor/Holy/Fire/Nature/Frost/Shadow/Arcane");
	SystemMessage(m_session, "%d/%d/%d/%d/%d/%d/%d", crt->GetUInt32Value(UNIT_FIELD_RESISTANCES), crt->GetUInt32Value(UNIT_FIELD_RESISTANCES_01), crt->GetUInt32Value(UNIT_FIELD_RESISTANCES_02), crt->GetUInt32Value(UNIT_FIELD_RESISTANCES_03), crt->GetUInt32Value(UNIT_FIELD_RESISTANCES_04), crt->GetUInt32Value(UNIT_FIELD_RESISTANCES_05), crt->GetUInt32Value(UNIT_FIELD_RESISTANCES_06));
	
	/*GreenSystemMessage(m_session, "Base Armor: %d", crt->GetUInt32Value(UNIT_FIELD_RESISTANCES));
	GreenSystemMessage(m_session, "Base Mana: %d", crt->GetUInt32Value(UNIT_FIELD_MAXPOWER1));
	GreenSystemMessage(m_session, "Base Holy: %d", crt->GetUInt32Value(UNIT_FIELD_RESISTANCES_01));
	GreenSystemMessage(m_session, "Base Fire: %d", crt->GetUInt32Value(UNIT_FIELD_RESISTANCES_02));
	GreenSystemMessage(m_session, "Base Nature: %d", crt->GetUInt32Value(UNIT_FIELD_RESISTANCES_03));
	GreenSystemMessage(m_session, "Base Frost: %d", crt->GetUInt32Value(UNIT_FIELD_RESISTANCES_04));
	GreenSystemMessage(m_session, "Base Shadow: %d", crt->GetUInt32Value(UNIT_FIELD_RESISTANCES_05));
	GreenSystemMessage(m_session, "Base Arcane: %d", crt->GetUInt32Value(UNIT_FIELD_RESISTANCES_06));*/
	SystemMessage(m_session, "Damage (min/max): %f/%f", crt->GetFloatValue(UNIT_FIELD_MINDAMAGE),crt->GetFloatValue(UNIT_FIELD_MAXDAMAGE));
	
	ColorSystemMessage(m_session, MSG_COLOR_RED, "Entry ID: %d", crt->GetUInt32Value(OBJECT_FIELD_ENTRY));
	ColorSystemMessage(m_session, MSG_COLOR_RED, "SQL Entry ID: %d", crt->GetSQL_id());
	// show byte
	std::stringstream sstext;
	uint32 theBytes = crt->GetUInt32Value(UNIT_FIELD_BYTES_0);
	sstext << "UNIT_FIELD_BYTES_0 are " << uint16((uint8)theBytes & 0xFF) << " " << uint16((uint8)(theBytes >> 8) & 0xFF) << " ";
	sstext << uint16((uint8)(theBytes >> 16) & 0xFF) << " " << uint16((uint8)(theBytes >> 24) & 0xFF) << '\0';
	SystemMessage(m_session, sstext.str().c_str());
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
		case 17: // spears
			SubClassSkill = SKILL_SPEARS;
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
		pr->_AddSkillLine(skill, 1, 300);   
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

	SystemMessage(m_session, "Reset talents of %s.", plr->GetName());;
	BlueSystemMessageToPlr(plr, "%s reset all your talents.", m_session->GetPlayer()->GetName());
	sGMLog.writefromsession(m_session, "reset talents of %s", plr->GetName());
	return true;
}

bool ChatHandler::HandleResetSpellsCommand(const char* args, WorldSession *m_session)
{
	Player *plr = this->getSelectedChar(m_session);
	if(!plr) return true;

	plr->Reset_Spells();
	
	SystemMessage(m_session, "Reset spells of %s to level 1.", plr->GetName());;
	BlueSystemMessage(m_session, "%s reset all your spells to starting values.", m_session->GetPlayer()->GetName());
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
    if(!*args) return false;

	/*char account[100];
	uint32 banned;
	int argc = sscanf(args, "%s %u", account, (unsigned int*)&banned);
	if(argc != 2)
		return false;*/

	char * pAccount = (char*)args;
	char * pDuration = strchr(pAccount, ' ');
	if( pDuration == NULL )
		return false;
	*pDuration = 0;
	++pDuration;

	int32 timeperiod = GetTimePeriodFromString(pDuration);
	if( timeperiod < 0 )
		return false;

	uint32 banned = (timeperiod ? (uint32)UNIXTIME+timeperiod : 1);

	char emptystring = 0;
	char * pReason = strchr(pDuration+1, ' ');
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
	sGMLog.writefromsession(m_session, "banned account %s until %s", pAccount, timeperiod ? ConvertTimeStampToDataTime(timeperiod+(uint32)UNIXTIME).c_str() : "permanant");
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

	BlueSystemMessage(m_session, "Removing ressurection sickness...");
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

	ItemSetEntry *entry = dbcItemSet.LookupEntry(setid);
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
			itm->SoulBind();

		if(!chr->GetItemInterface()->AddItemToFreeSlot(itm))
		{
			m_session->SendNotification("No free slots left!");
			itm->DeleteMe();
			return true;
		} else {
			//SystemMessage(m_session, "Added item: %s [%u]", (*itr)->Name1, (*itr)->ItemId);
			SlotResult * le = chr->GetItemInterface()->LastSearchResult();
			chr->GetSession()->SendItemPushResult(itm,false,true,false,true,le->ContainerSlot,le->Slot,1);
		}
	}
	GreenSystemMessage(m_session, "Added set to inventory complete. Time: %u ms", getMSTime() - start);
	return true;
}

bool ChatHandler::HandleCastTimeCheatCommand(const char* args, WorldSession* m_session)
{
	Player * plyr = getSelectedChar(m_session, true);
	if(!plyr) 
		return true;

	if(!*args) 
		if (plyr->CastTimeCheat) 
			args = "off"; 
		else 
			args = "on";

	if(strcmp(args, "on") == 0)
	{
		plyr->CastTimeCheat = true;
		BlueSystemMessage(m_session, "activated the cast time cheat on %s.", plyr->GetName());
		GreenSystemMessageToPlr(plyr, "activated the cast time cheat on you.", m_session->GetPlayer()->GetName());
	}
	else if(strcmp(args, "off") == 0)
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
		if (plyr->CooldownCheat) 
			args = "off"; 
		else 
			args = "on";

	if(strcmp(args, "on") == 0)
	{
		plyr->CooldownCheat = true;
		//best case we could simply iterate through cooldowns or create a special function ...
		SpellSet::const_iterator itr = plyr->mSpells.begin();
		for(; itr != plyr->mSpells.end(); ++itr)
			plyr->ClearCooldownForSpell( (*itr) );
		BlueSystemMessage(m_session, "activated the cooldown cheat on %s.", plyr->GetName());
		GreenSystemMessageToPlr(plyr, "activated the cooldown cheat on you.", m_session->GetPlayer()->GetName());
	}
	else if(strcmp(args, "off") == 0)
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
		if (plyr->GodModeCheat) 
			args = "off"; 
		else 
			args = "on";

	if(strcmp(args, "on") == 0)
	{
		plyr->GodModeCheat = true;
		BlueSystemMessage(m_session, "activated the god mode cheat on %s.", plyr->GetName());
		GreenSystemMessageToPlr(plyr, "activated the god mode cheat on you.", m_session->GetPlayer()->GetName());
	}
	else if(strcmp(args, "off") == 0)
	{
		plyr->GodModeCheat = false;
		BlueSystemMessage(m_session, "deactivated the god mode cheat on %s.", plyr->GetName());
		GreenSystemMessageToPlr(plyr, "deactivated the god mode cheat on you.", m_session->GetPlayer()->GetName());
	
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
		if (plyr->PowerCheat) 
			args = "off"; 
		else 
			args = "on";

	if(strcmp(args, "on") == 0)
	{
		plyr->PowerCheat = true;
		BlueSystemMessage(m_session, "activated the power cheat on %s.", plyr->GetName());
		GreenSystemMessageToPlr(plyr, "activated the power cheat on you.", m_session->GetPlayer()->GetName());
	}
	else if(strcmp(args, "off") == 0)
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
	print_cheat_status("TriggerPass", plyr->TriggerpassCheat);
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
		if (chr->FlyCheat) 
			args = "off"; 
		else 
			args = "on";

	if(strcmp(args, "on") == 0)
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
	else if(strcmp(args, "off") == 0)
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
	if (0 == stricmp(args, "spellfixes"))
	{
		objmgr.LoadSpellFixes();
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
		snprintf(str, 256, "%sDatabase reload failed.", MSG_COLOR_LIGHTRED);
	else
		snprintf(str, 256, "%sDatabase reload completed in %u ms.", MSG_COLOR_LIGHTBLUE, (unsigned int)(getMSTime() - mstime));
	sWorld.SendWorldText(str, 0);
	sGMLog.writefromsession(m_session, "reloaded table %s", args);
	return true;
}

bool ChatHandler::HandleFlySpeedCheatCommand(const char* args, WorldSession* m_session)
{
	float Speed = (float)atof(args);
	if(Speed == 0)
		Speed = 20;

	Player * plr = getSelectedChar(m_session);
	if(plr == 0)
		return true;

	BlueSystemMessage(m_session, "Setting the fly speed of %s to %f.", plr->GetName(), Speed);
	GreenSystemMessage(plr->GetSession(), "%s set your fly speed to %f.", m_session->GetPlayer()->GetName(), Speed);
	if ( plr != m_session->GetPlayer() )
		sGMLog.writefromsession( m_session, "set %s's fly speed to %2.2f", plr->GetName(), Speed );
	
	WorldPacket data(SMSG_FORCE_MOVE_SET_FLY_SPEED, 16);
	data << plr->GetNewGUID();
	data << uint32(0) << Speed;
	plr->SendMessageToSet(&data, true);
	
	plr->m_flySpeed = Speed;
	
	return true;
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
	plr->Reset_Talents();
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
	sp->channel_spell=sp->channel_target_creature=sp->channel_target_go=0;
	pCreature->Load(sp, (uint32)NULL, NULL);

	Pet *old_tame = plr->GetSummon();
	if(old_tame != NULL)
	{
		old_tame->Dismiss(true);
	}

	// create a pet from this creature
	Pet * pPet = objmgr.CreatePet();
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

	uint32 botprice = m_session->GetPlayer()->GetUInt32Value(UNIT_FIELD_LEVEL)*10000; //1 gold per level ?

	if( m_session->GetPlayer()->GetUInt32Value(PLAYER_FIELD_COINAGE) < botprice )
	{
		GreenSystemMessage(m_session, "You need a total of %u coins to afford a bot", botprice);
		return false;
	}

	uint8 botType = (uint8)atof((char*)args);

	if ( botType!=0 )
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
	Unit *newguard = plr->create_guardian(Entry,2*60*1000,float(-M_PI*2), plr->getLevel() );
	AiAgentHealSupport *new_interface = new AiAgentHealSupport;
	new_interface->Init(newguard,AITYPE_PET,MOVEMENTTYPE_NONE,plr);
	newguard->ReplaceAIInterface( (AIInterface *) new_interface );

/*	Pet *old_tame = plr->GetSummon();
	if(old_tame != NULL)
	{
		old_tame->Dismiss(true);
	}

	// create a pet from this creature
	Pet * pPet = objmgr.CreatePet();
	pPet->SetInstanceID(plr->GetInstanceID());
	pPet->SetMapId(plr->GetMapId());

	pPet->SetFloatValue ( OBJECT_FIELD_SCALE_X, pTemplate->Scale / 2); //we do not wish to block visualy other players
	AiAgentHealSupport *new_interface = new AiAgentHealSupport;
	pPet->ReplaceAIInterface( (AIInterface *) new_interface );
//	new_interface->Init(pPet,AITYPE_PET,MOVEMENTTYPE_NONE,plr); // i think this will get called automatically for pet

	pPet->CreateAsSummon(Entry, pCreatureInfo, pCreature, plr, NULL, 0x2, 0);

	pPet->Rename(name);

	//healer bot should not have any specific ations
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

	Pet * pPet = plr->GetSummon();
	if(pPet == 0)
	{
		RedSystemMessage(m_session, "%s has no pet.", plr->GetName());
		return true;
	}

	uint32 SpellId = atol(args);
	SpellEntry * spell = dbcSpell.LookupEntry(SpellId);
	if(!SpellId || !spell)
	{
		RedSystemMessage(m_session, "Invalid spell id requested.");
		return true;
	}

	pPet->AddSpell(spell, true);
	GreenSystemMessage(m_session, "Added spell %u to %s's pet.", SpellId, plr->GetName());
	return true;
}

bool ChatHandler::HandleRemovePetSpellCommand(const char* args, WorldSession* m_session)
{
	Player * plr = getSelectedChar(m_session, false);
	if(!plr)
		return false;

	Pet * pPet = plr->GetSummon();
	if(pPet == 0)
	{
		RedSystemMessage(m_session, "%s has no pet.", plr->GetName());
		return true;
	}

	uint32 SpellId = atol(args);
	SpellEntry * spell = dbcSpell.LookupEntry(SpellId);
	if(!SpellId || !spell)
	{
		RedSystemMessage(m_session, "Invalid spell id requested.");
		return true;
	}

	pPet->RemoveSpell(SpellId);
	GreenSystemMessage(m_session, "Added spell %u to %s's pet.", SpellId, plr->GetName());
	return true;
}

bool ChatHandler::HandleRenamePetCommand(const char* args, WorldSession* m_session)
{
	Player * plr = m_session->GetPlayer();
	Pet * pPet = plr->GetSummon();
	if(pPet == 0)
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
	pPet->Rename(args);
	return true;
}

bool ChatHandler::HandleShutdownCommand(const char* args, WorldSession* m_session)
{
	uint32 shutdowntime = atol(args);
	if(!args)
		shutdowntime = 5;

	
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
	uint32 shutdowntime = atol(args);
	if(!args)
		shutdowntime = 5;

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
	Player * plr = objmgr.GetPlayer(args, false);
	if(!plr)
	{
		RedSystemMessage(m_session, "Player not found.");
		return true;
	}

	m_session->GetPlayer()->gmTargets.insert(plr);
	BlueSystemMessage(m_session, "Now accepting whispers from %s.", plr->GetName());
	return true;
}

bool ChatHandler::HandleBlockWhispersCommand(const char* args, WorldSession* m_session)
{
	if(args == 0 || strlen(args) < 2) return false;
	Player * plr = objmgr.GetPlayer(args, false);
	if(!plr)
	{
		RedSystemMessage(m_session, "Player not found.");
		return true;
	}

	m_session->GetPlayer()->gmTargets.erase(plr);
	BlueSystemMessage(m_session, "Now blocking whispers from %s.", plr->GetName());
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
	uint32 c=0;
	for (itr = objmgr._players.begin(); itr != objmgr._players.end(); itr++)
	{
		plr = itr->second;
		if(plr->GetSession() && plr->IsInWorld())
		{
			//plr->SafeTeleport(summoner->GetMapId(), summoner->GetInstanceID(), summoner->GetPosition());
			/* let's do this the blizz way */
			plr->SummonRequest(summoner->GetLowGUID(), summoner->GetZoneId(), summoner->GetMapId(), summoner->GetInstanceID(), summoner->GetPosition());
			++c;
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
	SpellEntry * info = dbcSpell.LookupEntry(spellid);
	if(!info)
	{
		RedSystemMessage(m_session, "Invalid spell specified.");
		return true;
	}

	// this makes sure no moron casts a learn spell on everybody and wrecks the server
	for (int i = 0; i < 3; i++)
	{
		if (info->Effect[i] == 36) //SPELL_EFFECT_LEARN_SPELL - 36
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
				Spell * sp = SpellPool.PooledNew();
				sp->Init(plr, info, true, 0);
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

	if(m_session->GetPlayer()->linkTarget == 0 || m_session->GetPlayer()->linkTarget->GetTypeId() != TYPEID_UNIT)
	{
		RedSystemMessage(m_session, "Master not selected. select the master, and use formationlink1.");
		return true;
	}

	Creature * slave = getSelectedCreature(m_session, true);
	if(slave == 0) return true;

	slave->GetAIInterface()->m_formationFollowDistance = dist;
	slave->GetAIInterface()->m_formationFollowAngle = ang;
	slave->GetAIInterface()->m_formationLinkTarget = static_cast< Creature* >( m_session->GetPlayer()->linkTarget );
	slave->GetAIInterface()->m_formationLinkSqlId = slave->GetAIInterface()->m_formationLinkTarget->GetSQL_id();
	slave->GetAIInterface()->SetUnitToFollowAngle(ang);
	
	// add to db
	WorldDatabase.Execute("INSERT INTO creature_formations VALUES(%u, %u, '%f', '%f')", 
		slave->GetSQL_id(), slave->GetAIInterface()->m_formationLinkSqlId, ang, dist);

	BlueSystemMessage(m_session, "%s linked up to %s with a distance of %f at %f radians.", slave->GetCreatureInfo()->Name, 
		static_cast< Creature* >( m_session->GetPlayer()->linkTarget )->GetCreatureInfo()->Name, dist, ang );

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
	c->GetAIInterface()->SetUnitToFollow(0);
	
	WorldDatabase.Execute("DELETE FROM creature_formations WHERE spawn_id=%u", c->GetSQL_id());
	return true;
}

bool ChatHandler::HandleNullFollowCommand(const char* args, WorldSession * m_session)
{
	Creature * c = getSelectedCreature(m_session, true);
	if(!c) return true;

	// restart movement
	c->GetAIInterface()->SetAIState(STATE_IDLE);
	c->GetAIInterface()->SetUnitToFollow(0);

	sGMLog.writefromsession( m_session, "cancelled npc follow command on %s, sqlid %u", c->GetCreatureInfo()->Name, c->GetSQL_id() );
	return true;
}

bool ChatHandler::HandleAuraStackCheatCommand(const char* args, WorldSession * m_session)
{
	Player * plyr = getSelectedChar(m_session, true);
	if(!plyr) 
		return true;

	if(!*args) 
		if (plyr->AuraStackCheat)
			args = "off"; 
		else 
			args = "on";

	if(strcmp(args, "on") == 0)
	{
		plyr->AuraStackCheat = true;
		BlueSystemMessage(m_session, "activated the aura stack cheat on %s.", plyr->GetName());
		GreenSystemMessageToPlr(plyr, "activated the aura stack cheat on you.", m_session->GetPlayer()->GetName());
	}
	else if(strcmp(args, "off") == 0)
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
		if (plyr->TriggerpassCheat) 
			args = "off"; 
		else 
			args = "on";

	if(strcmp(args, "on") == 0)
	{
		plyr->TriggerpassCheat = true;
		BlueSystemMessage(m_session, "activated the triggerpass cheat on %s.", plyr->GetName());
		GreenSystemMessageToPlr(plyr, "activated the triggerpass cheat on you.", m_session->GetPlayer()->GetName());
	}
	else if(strcmp(args, "off") == 0)
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
	{"None","Warrior", "Paladin", "Hunter", "Rogue", "Priest", "None", "Shaman", "Mage", "Warlock", "None", "Druid"};
	static const char* races[12] =
	{"None","Human","Orc","Dwarf","Night Elf","Undead","Tauren","Gnome","Troll","None","Blood Elf","Draenei"};

	char playedLevel[64];
	char playedTotal[64];

	int seconds = (plr->GetPlayedtime())[0];
	int mins=0;
	int hours=0;
	int days=0;
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
	mins=0;
	hours=0;
	days=0;
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

	BlueSystemMessage(m_session, "%s uses %s (build %u)", (plr->getGender()?"She":"He"),
		(sess->HasFlag(ACCOUNT_FLAG_XPACK_01)?"WoW Burning Crusade":"WoW"), sess->GetClientBuild());

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
	char * pIp = (char*)args;
	char * pDuration = strchr(pIp, ' ');
	if(pDuration == NULL)
		return false;
	*pDuration = 0;
	++pDuration;

	int32 timeperiod = GetTimePeriodFromString(pDuration);
	if(timeperiod < 1)
		return false;

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
		RedSystemMessage(m_session, "Lack of CIDR address assumes a 32bit match (if you don't understand, dont worry, it worked)");
		IP.append("/32");
		pIp = (char*)IP.c_str(); //is this correct? - optical
	}

	//temporal IP or real pro flooder who will change it tomorrow ?
	char emptystring = 0;
	char * pReason = strchr(pDuration+1, ' ');
	if( pReason == NULL )
		pReason = &emptystring;

	SystemMessage(m_session, "Adding [%s] to IP ban table, expires %s.Reason is :%s", pIp, (expire_time == 0)? "Never" : ctime( &expire_time ),pReason);
	sLogonCommHandler.IPBan_Add( pIp, (uint32)expire_time, pReason );
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
		RedSystemMessage(m_session, "Lack of CIDR address assumes a 32bit match (if you don't understand, dont worry, it worked)");
		pIp.append("/32");
	}
	/**
	 * We can afford to be less fussy with the validty of the IP address given since
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
	info->GenerateModelId(&sp->displayid);
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
	sp->bytes0 = 0;
	sp->bytes1 = 0;
	sp->bytes2 = 0;
	//sp->respawnNpcLink = 0;
	sp->stand_state = 0;
	sp->channel_spell=sp->channel_target_creature=sp->channel_target_go=0;


	Creature * p = m_session->GetPlayer()->GetMapMgr()->CreateCreature(entry);
	ASSERT(p);
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
	if(!plr) return true;;

	BlueSystemMessage(m_session, "Setting standing of %u to %d on %s.", faction, standing, plr->GetName());
	plr->SetStanding(faction, standing);
	GreenSystemMessageToPlr(plr, "%s set your standing of faction %u to %d.", m_session->GetPlayer()->GetName(), faction, standing);
	sGMLog.writefromsession(m_session, "set standing of faction %u to %u for %s", faction,standing,plr->GetName());
	return true;
}

void SendHighlightedName(WorldSession * m_session, const char* prefix, char* full_name, string& lowercase_name, string& highlight, uint32 id)
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

	sChatHandler.SystemMessage(m_session, message);
}

void SendItemLinkToPlayer(ItemPrototype * iProto, WorldSession * pSession, bool ItemCount, Player * owner = NULL, uint32 language = NULL)
{
	if(!iProto || !pSession)
		return;
	if(ItemCount && owner == NULL)
		return;
		
 	if(ItemCount)
	{
		int8 count = static_cast<int8>(owner->GetItemInterface()->GetItemCount(iProto->ItemId, true));
		int8 slot = owner->GetItemInterface()->GetBagSlotByGuid(iProto->ItemId);
		sChatHandler.SystemMessage(pSession,"Item %u %s Count %u Slot %u ContainerSlots %u", iProto->ItemId, GetItemLinkByProto(iProto, language).c_str(), count, slot, iProto->ContainerSlots);
	}
 	else
		sChatHandler.SystemMessage(pSession,"Item %u %s ContainerSlots %u", iProto->ItemId, GetItemLinkByProto(iProto, language).c_str(), iProto->ContainerSlots);
}


bool ChatHandler::HandleLookupItemCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	string x = string(args);
	arcemu_TOLOWER(x);
	if(x.length() < 4)
	{
		RedSystemMessage(m_session, "Your search string must be at least 5 characters long.");
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
			//string ObjectID = i.c_str();
			const char*objectName=i->Name;
			recout="|cfffff000Object ";
			recout+=strm.str();
			recout+="|cffFFFFFF: ";
			recout+=objectName;
			recout = recout + Name;
			SendMultilineMessage(m_session,recout.c_str());
			++count;
			if(count==50 || count > 50)
			{
				RedSystemMessage(m_session,"More than 50 results returned. aborting.");
				break;
			}
		}
		if(!itr->Inc()) break;
	}
	itr->Destruct();
	if (count==0)
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
		RedSystemMessage(m_session, "Your search string must be at least 5 characters long.");
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
		RedSystemMessage(m_session, "Your search string must be at least 5 characters long.");
		return true;
	}

	GreenSystemMessage(m_session, "Starting search of spell `%s`...", x.c_str());
	uint32 t = getMSTime();
	uint32 count = 0;
	for (uint32 index = 0; index < dbcSpell.GetNumRows(); ++index)
	{
		SpellEntry* spell = dbcSpell.LookupRow(index);
		string y = string(spell->Name);
		arcemu_TOLOWER(y);
		if(FindXinYString(x, y))
 		{
 			// Print out the name in a cool highlighted fashion
			SendHighlightedName(m_session, "Spell", spell->Name, y, x, spell->Id);
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
		RedSystemMessage(m_session, "Your search string must be at least 5 characters long.");
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
		RedSystemMessage(m_session, "Your search string must be at least 5 characters long.");
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

	/* this is crap 
	// let's try rotation_0
	go->ModFloatValue(GAMEOBJECT_ROTATION, rad);
	go->ModFloatValue(GAMEOBJECT_ROTATION_01, rad);
	go->ModFloatValue(GAMEOBJECT_ROTATION_02, rad);
	go->ModFloatValue(GAMEOBJECT_ROTATION_03, rad);
	go->SaveToDB();*/
	float rad = deg * (float(M_PI) / 180.0f);
	// despawn and respawn
	//go->Despawn(1000);

	switch(tolower(Axis))
	{
	case 'x':
		go->ModFloatValue(GAMEOBJECT_ROTATION, rad);
		break;
	case 'y':
		go->ModFloatValue(GAMEOBJECT_ROTATION_01, rad);
		break;
	case 'o':
		if(m_session->GetPlayer()){
			float ori = m_session->GetPlayer()->GetOrientation();
			go->SetFloatValue(GAMEOBJECT_ROTATION_02, sinf(ori / 2));
			go->SetFloatValue(GAMEOBJECT_ROTATION_03, cosf(ori / 2));}
		break;
	default:
		RedSystemMessage(m_session, "Invalid Axis, Please use x, y, or o.");
		return true;
	}

	uint32 NewGuid = m_session->GetPlayer()->GetMapMgr()->GenerateGameobjectGuid();
	go->RemoveFromWorld(true);
	go->SetNewGuid(NewGuid);
	go->SaveToDB();
	go->PushToWorld(m_session->GetPlayer()->GetMapMgr());
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

	go->RemoveFromWorld(true);
	go->SetPosition(m_session->GetPlayer()->GetPosition());
	go->SetFloatValue(GAMEOBJECT_POS_X, m_session->GetPlayer()->GetPositionX());
	go->SetFloatValue(GAMEOBJECT_POS_Y, m_session->GetPlayer()->GetPositionY());
	go->SetFloatValue(GAMEOBJECT_POS_Z, m_session->GetPlayer()->GetPositionZ());
	go->SetFloatValue(GAMEOBJECT_FACING, m_session->GetPlayer()->GetOrientation());
	uint32 NewGuid = m_session->GetPlayer()->GetMapMgr()->GenerateGameobjectGuid();
	go->SetNewGuid(NewGuid);
	go->SaveToDB();
	go->PushToWorld(m_session->GetPlayer()->GetMapMgr());
	m_session->GetPlayer()->m_GM_SelectedGO = NewGuid;
	return true;
}

bool ChatHandler::HandleNpcPossessCommand(const char * args, WorldSession * m_session)
{
	Unit * pTarget = getSelectedChar(m_session, false);
	if(!pTarget)
	{
		pTarget = getSelectedCreature(m_session, false);
		if(pTarget && (pTarget->IsPet() || pTarget->GetUInt32Value(UNIT_FIELD_CREATEDBY) != 0))
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
			sGMLog.writefromsession( m_session, "used possess command on CREATURE %s, sqlid %u", static_cast< Creature* >( pTarget )->GetCreatureInfo() ? static_cast< Creature* >( pTarget )->GetCreatureInfo()->Name : "unknown", static_cast< Creature* >( pTarget )->GetSQL_id() );
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
		ASSERT(result);
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
		targets.m_targetMask = 2;
		targets.m_unitTarget = dst->GetGUID();
	}
	else if(targettype == TTYPE_SOURCE)
	{
		targets.m_targetMask = 32;
		targets.m_srcX = src->GetPositionX();
		targets.m_srcY = src->GetPositionY();
		targets.m_srcZ = src->GetPositionZ();
	}
	else if(targettype == TTYPE_DESTINATION)
	{
		targets.m_targetMask = 64;
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
		ASSERT(it != aiagent_extra.end());

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
	CharacterDatabase.Execute("UPDATE guilds SET `guildName` = \"%s\" WHERE `guildId` = '%u'", CharacterDatabase.EscapeString(string(args)).c_str(), plr->GetGuild()->GetGuildId());
	return true;
}

//People seem to get stuck in guilds from time to time. This should be helpfull. -DGM
bool ChatHandler::HandleGuildRemovePlayerCommand(const char* args, WorldSession *m_session)
{
	Player * plr = getSelectedChar(m_session);
	if(!plr || !plr->GetGuildId() || !plr->GetGuild() || plr->GetGuild()->GetGuildLeader() == plr->GetLowGUID())
		return false;

	GreenSystemMessage(m_session, "Kicked %s from Guild: %s", plr->GetName(), plr->GetGuild()->GetGuildName());

	if(plr->GetLowGUID() != m_session->GetPlayer()->GetLowGUID())
		sGMLog.writefromsession(m_session, "Kicked %s from Guild %s", plr->GetName(), plr->GetGuild()->GetGuildName());

	plr->GetGuild()->RemoveGuildMember(plr->m_playerInfo, plr->GetSession());
	return true;
}

//-DGM
bool ChatHandler::HandleGuildDisbandCommand(const char* args, WorldSession *m_session)
{
	Player * plr = getSelectedChar(m_session);
	if(!plr || !plr->GetGuildId() || !plr->GetGuild()) 
		return false;

	GreenSystemMessage(m_session, "Disbanded Guild: %s", plr->GetGuild()->GetGuildName());
	sGMLog.writefromsession(m_session, "Disbaned Guild %s", plr->GetGuild()->GetGuildName());
	plr->GetGuild()->Disband();
	return true;
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
		real_type=0;
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
	t->AddMember(plr->m_playerInfo);
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
		real_type=0;
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
	t->SetLeader(plr->m_playerInfo);
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
	if(m_session->GetPlayer()->bGMTagOn)
		return false;

	m_session->GetPlayer()->bGMTagOn = true;
	return true;
}

bool ChatHandler::HandleGenderChanger(const char* args, WorldSession *m_session)
{
	int gender;
	Player* target = objmgr.GetPlayer((uint32)m_session->GetPlayer()->GetSelection());
	if(!target) {
		SystemMessage(m_session, "Select A Player first.");
		return true;
	}
	uint32 displayId = target->GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID);
	if (!*args)
	{
		if (target->getGender()== 1)
			gender = 0;
		else
			gender = 1;
	}
	else
		gender = min((int)atoi((char*)args),1);
	target->setGender(gender);
	if( target->getGender() == 0 )
	{
		target->SetUInt32Value(UNIT_FIELD_DISPLAYID, (target->getRace()==RACE_BLOODELF)?displayId+1:displayId-1 );
		target->SetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID, (target->getRace()==RACE_BLOODELF)?displayId+1:displayId-1 );
	} else {
		target->SetUInt32Value(UNIT_FIELD_DISPLAYID, (target->getRace()==RACE_BLOODELF)?displayId-1:displayId+1 );
		target->SetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID, (target->getRace()==RACE_BLOODELF)?displayId-1:displayId+1 );
	}
	SystemMessage(m_session, "Set %s's gender to %s(%u).", target->GetName(), gender?"Female":"Male", gender);
	return true;
}

bool ChatHandler::HandleDispelAllCommand(const char * args, WorldSession * m_session)
{
	uint32 pos=0;
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

	ItemIterator itr(plr->GetItemInterface());
	itr.BeginSearch();
	for(; !itr.End(); itr.Increment())
	{
		if(!(*itr))
			return false;

		SendItemLinkToPlayer((*itr)->GetProto(), m_session, true, plr, m_session->language);
	}
	itr.EndSearch();
	sGMLog.writefromsession(m_session, "used show items command on %s,", plr->GetName());

   return true;
}

bool ChatHandler::HandleShowSkills(const char * args, WorldSession * m_session)
{
	Player * plr = getSelectedChar(m_session, true);
	if(!plr) 
		return true;

	SkillIterator itr2(plr);
	itr2.BeginSearch();
	for(; !itr2.End(); itr2.Increment())
	{
		SystemMessage(m_session, "Skill %u %s %u/%u", itr2->Skill->id, itr2->Skill->Name ,itr2->CurrentValue, itr2->MaximumValue);
	}
	itr2.EndSearch();
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

	pCreature->SetFloatValue(OBJECT_FIELD_SCALE_X, sc);
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
	if(pSpell==NULL)
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
		plr->SetUInt64Value( PLAYER_FIELD_KNOWN_TITLES, 0 );
	else if( title > 0 )
		plr->SetKnownTitle( static_cast< RankTitles >( title ), true );
	else
		plr->SetKnownTitle( static_cast< RankTitles >( -title ), false );

	plr->SetUInt32Value( PLAYER_CHOSEN_TITLE, 0 ); // better remove chosen one
	SystemMessage( m_session, "Title has been %s.", title > 0 ? "set" : "reset" );
	return true;
}
