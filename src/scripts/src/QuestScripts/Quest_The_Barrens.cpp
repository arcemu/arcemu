/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
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

#include "Setup.h"

#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, plr); \
    Menu->SendTo(plr);

class BeatenCorpse : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3557, plr);

			if(plr->GetQuestLogForEntry(4921) != NULL)
				Menu->AddItem(0, "I inspect the body further.", 1);

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char* Code)
		{
			GossipMenu* Menu;

			switch(IntId)
			{
				case 1:
					{
						SendQuickMenu(3558);

						QuestLogEntry* qle = plr->GetQuestLogForEntry(4921);
						if(qle == NULL)
							return;

						if(qle->GetMobCount(0) != 0)
							return;

						qle->SetMobCount(0, 1);
						qle->SendUpdateAddKill(0);
						qle->UpdatePlayerFields();
					}
					break;
			}
		}

};

class TheEscape : public QuestScript
{
	public:

		void OnQuestStart(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			float SSX = mTarget->GetPositionX();
			float SSY = mTarget->GetPositionY();
			float SSZ = mTarget->GetPositionZ();


			Creature* creat = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, 3439);
			if(creat == NULL)
				return;
			creat->m_escorter = mTarget;
			creat->GetAIInterface()->setMoveType(11);
			creat->GetAIInterface()->SetAllowedToEnterCombat(false);
			creat->GetAIInterface()->StopMovement(3000);
			creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Okay let's do this, you gotta protect me and stuff, I can't fight on my own!");
			creat->SetUInt32Value(UNIT_NPC_FLAGS, 0);


			sEAS.CreateCustomWaypointMap(creat);

			sEAS.WaypointCreate(creat, 1113.4f, -3110.22f, 82.4277f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1119.19f, -3122.02f, 82.5178f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1118.31f, -3139.95f, 82.8615f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1121.38f, -3147.83f, 84.6616f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1124.61f, -3156.1f, 88.7833f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1130.02f, -3162.84f, 91.8255f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1135.39f, -3169.53f, 92.1331f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1140.67f, -3182.66f, 91.89f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1152.04f, -3208.41f, 91.6341f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1146.66f, -3232.49f, 91.6839f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1134.9f, -3271.74f, 91.6708f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1128.68f, -3292.49f, 92.4652f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1122.19f, -3314.16f, 92.6764f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1115.32f, -3337.09f, 91.7156f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1109.38f, -3356.92f, 92.9116f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1105.75f, -3369.05f, 94.1855f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1100.28f, -3387.33f, 94.1077f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1095.44f, -3403.48f, 92.2158f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1089.72f, -3422.56f, 91.4141f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1088.96f, -3430.99f, 90.8609f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1087.98f, -3441.86f, 89.2736f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1086.62f, -3456.92f, 85.1732f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1081.47f, -3467.23f, 82.0964f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1076.72f, -3476.72f, 79.1774f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1071.38f, -3487.4f, 77.395f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1076.77f, -3495.27f, 75.1605f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1088.36f, -3504.89f, 73.3598f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1084.75f, -3514.3f, 71.8212f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1080.68f, -3524.89f, 69.9172f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1076.34f, -3536.21f, 66.6681f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1072.30f, -3547.52f, 63.0944f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1066.32f, -3562.31f, 60.1643f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1069.11f, -3573.12f, 58.152f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1071.14f, -3580.96f, 56.8904f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1061.25f, -3590.68f, 54.482f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1050.3f, -3601.45f, 52.8842f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1045.51f, -3606.16f, 51.8961f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1039.6f, -3615.77f, 49.1786f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1035.35f, -3630.47f, 45.3412f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1032.73f, -3639.51f, 42.9659f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1029.05f, -3652.25f, 38.7237f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1025.66f, -3663.95f, 35.4199f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1023.06f, -3672.94f, 33.6937f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1018.17f, -3689.83f, 29.327f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1014.29f, -3703.26f, 28.1314f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1011.5f, -3712.89f, 28.3098f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1007.59f, -3726.41f, 28.5626f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1005.23f, -3734.59f, 28.226f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 1000.71f, -3750.21f, 27.2183f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 991.184f, -3745.22f, 27.3147f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 980.426f, -3743.73f, 27.1928f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 967.803f, -3739.3f, 27.1358f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 960.049f, -3736.57f, 27.1334f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 950.89f, -3728.36f, 27.1419f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 944.546f, -3722.67f, 27.1419f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 932.694f, -3720.85f, 27.1827f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 918.623f, -3721.25f, 27.155f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 906.234f, -3722.75f, 27.1815f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 888.526f, -3720.43f, 27.1757f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 872.566f, -3719.05f, 27.1022f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 860.014f, -3717.95f, 27.143f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 829.08f, -3720.65f, 27.1734f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 806.728f, -3719.8f, 27.2459f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 739.49f, -3716.49f, 27.3376f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 721.539f, -3719.41f, 27.1518f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 681.091f, -3718.2f, 26.8029f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 650.641f, -3722.22f, 26.7092f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 635.284f, -3724.97f, 26.9639f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 603.042f, -3719.17f, 27.1852f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 576.173f, -3714.34f, 27.1907f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 564.184f, -3713.83f, 27.1849f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 539.246f, -3712.76f, 27.1729f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 517.794f, -3701.6f, 27.257f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 502.569f, -3693.69f, 27.1875f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 497.299f, -3690.95f, 27.1848f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 462.66f, -3687.48f, 27.1848f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 431.287f, -3686.83f, 27.1848f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 413.891f, -3686.46f, 27.1848f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 394.705f, -3676.41f, 27.1848f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 374.136f, -3659.1f, 27.1848f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 349.007f, -3649.73f, 27.1857f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 319.978f, -3648.94f, 27.1862f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 319.978f, -3648.94f, 27.1862f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 303.064f, -3628.28f, 27.4367f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 291.016f, -3613.57f, 29.0272f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 281.84f, -3611.71f, 30.2372f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 266.977f, -3612.43f, 31.1779f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 254.811f, -3613.01f, 31.6233f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 245.462f, -3613.47f, 31.5706f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 231.717f, -3617.76f, 30.9447f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 218.659f, -3621.84f, 30.1022f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 209.439f, -3624.72f, 29.4542f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 199.989f, -3627.67f, 29.2537f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 186.186f, -3631.98f, 28.14f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 167.974f, -3637.67f, 27.2364f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 154.744f, -3641.8f, 27.2517f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 143.806f, -3645.22f, 27.3126f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 132.638f, -3648.7f, 27.2088f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 120.611f, -3652.46f, 27.1889f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 107.037f, -3656.7f, 27.185f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 95.5258f, -3660.3f, 27.185f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 82.1243f, -3664.48f, 27.185f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 70.9563f, -3667.97f, 27.1943f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 59.0439f, -3671.69f, 27.9463f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 45.5851f, -3675.89f, 29.0178f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 36.5935f, -3678.7f, 29.3503f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 26.3991f, -3681.89f, 29.4141f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 22.4474f, -3683.12f, 29.4396f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, 8.81682f, -3687.38f, 29.0755f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -5.01755f, -3688.22f, 27.2689f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -18.7321f, -3689.06f, 27.1837f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -27.6556f, -3689.6f, 27.1804f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -42.5709f, -3696.07f, 27.0441f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -55.1746f, -3701.55f, 28.0819f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -68.5347f, -3702.28f, 29.0473f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -79.0191f, -3702.85f, 27.8198f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -92.7386f, -3703.6f, 26.9801f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -107.716f, -3704.42f, 27.0756f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -122.42f, -3708.86f, 27.1443f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -140.8f, -3714.41f, 27.1919f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -157.17f, -3719.35f, 27.7222f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -175.697f, -3730.1f, 27.2898f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -185.142f, -3735.58f, 27.2565f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -199.367f, -3743.84f, 28.6632f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -214.832f, -3752.81f, 27.2439f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -227.442f, -3760.13f, 26.4763f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -237.718f, -3766.09f, 26.0274f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -249.758f, -3773.08f, 26.997f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -259.71f, -3775.02f, 28.0756f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -273.843f, -3777.78f, 29.9517f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -286.622f, -3780.27f, 31.3675f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -303.287f, -3783.53f, 32.0795f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -319.305f, -3786.65f, 30.1144f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -330.553f, -3788.85f, 27.8853f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -342.072f, -3797.98f, 27.1848f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -352.839f, -3806.52f, 27.7424f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -364.782f, -3815.98f, 28.0461f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -377.015f, -3818.45f, 27.8833f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -391.249f, -3821.32f, 29.0991f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -402.541f, -3823.6f, 30.3326f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -416.411f, -3829.47f, 30.8822f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -428.845f, -3834.73f, 30.4149f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -445.092f, -3841.6f, 28.8669f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -455.425f, -3845.97f, 28.8621f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -470.898f, -3852.51f, 28.5759f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -487.256f, -3859.43f, 27.1612f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -499.413f, -3864.57f, 27.2075f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -509.691f, -3868.92f, 27.9372f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -520.246f, -3873.38f, 29.2394f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -529.364f, -3877.24f, 29.7561f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -539.753f, -3881.64f, 29.5172f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -548.208f, -3885.21f, 29.2254f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -561.084f, -3890.66f, 28.795f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -576.06f, -3896.99f, 28.4577f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -592.748f, -3904.05f, 27.6594f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -606.95f, -3910.06f, 28.5744f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -615.792f, -3913.79f, 28.8637f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -627.286f, -3918.66f, 28.731f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -627.286f, -3918.66f, 28.731f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -647.512f, -3927.21f, 26.4148f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -658.491f, -3927.29f, 26.6015f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -672.711f, -3927.4f, 27.173f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -683.811f, -3927.48f, 26.1342f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -692.03f, -3927.54f, 25.6526f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -702.11f, -3927.62f, 25.1975f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -721.55f, -3927.76f, 23.8796f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -732.469f, -3927.84f, 23.1171f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -746.962f, -3923.75f, 21.7104f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -753.487f, -3921.9f, 21.0421f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -762.783f, -3919.28f, 19.5729f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -773.176f, -3916.34f, 16.9823f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -779.354f, -3914.6f, 15.1756f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -784.848f, -3908.98f, 13.2143f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -790.426f, -3903.27f, 11.2414f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -796.382f, -3897.18f, 8.97999f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -800.074f, -3888.44f, 8.34248f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -803.347f, -3880.71f, 8.0567f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -807.413f, -3871.09f, 7.89321f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -811.2f, -3862.14f, 7.98673f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -814.963f, -3853.24f, 8.50162f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -819.941f, -3841.47f, 9.26294f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -825.901f, -3827.38f, 8.80521f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -828.589f, -3821.03f, 9.38863f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -832.959f, -3810.69f, 10.1485f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -836.746f, -3801.74f, 11.6839f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -839.807f, -3794.5f, 13.3517f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -846.928f, -3788.8f, 13.8683f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -850.98f, -3782.56f, 15.4662f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -855.229f, -3776.02f, 16.6032f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -857.811f, -3772.05f, 17.612f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -861.766f, -3765.96f, 18.6269f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -868.647f, -3761.04f, 18.5869f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -877.786f, -3758.52f, 16.6433f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -883.917f, -3756.83f, 14.9905f, 3.098398f, 0, 256, 11912);
			sEAS.WaypointCreate(creat, -892.883f, -3760.42f, 12.4808f, 3.098398f, 0, 256, 11912);
			sEAS.EnableWaypoints(creat);

		}
};

class Wizzlecranks_Shredder : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(Wizzlecranks_Shredder);
		Wizzlecranks_Shredder(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnReachWP(uint32 iWaypointId, bool bForwards)
		{
			if(iWaypointId == 195)
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Thank you Young warior!");
				_unit->Despawn(5000, 1000);
				sEAS.DeleteWaypoints(_unit);
				if(_unit->m_escorter == NULL)
					return;
				Player* plr = _unit->m_escorter;
				_unit->m_escorter = NULL;
				plr->GetQuestLogForEntry(863)->SendQuestComplete();
			}
		}
};

class FreeFromtheHold : public QuestScript
{
	public:

		void OnQuestStart(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			float SSX = mTarget->GetPositionX();
			float SSY = mTarget->GetPositionY();
			float SSZ = mTarget->GetPositionZ();


			Creature* creat = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, 3465);
			if(creat == NULL)
				return;
			creat->m_escorter = mTarget;
			creat->GetAIInterface()->setMoveType(11);
			creat->GetAIInterface()->SetAllowedToEnterCombat(false);
			creat->GetAIInterface()->StopMovement(3000);
			creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Enough talk, help me get back to Ratchet will you? Let me know when you're ready and we'll make our break!");
			creat->SetUInt32Value(UNIT_NPC_FLAGS, 0);

			sEAS.CreateCustomWaypointMap(creat);
			sEAS.WaypointCreate(creat, -1607.61f, -3846.03f, 14.3572f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1601.69f, -3845.79f, 14.4817f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1594.94f, -3844.55f, 14.823f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1588.44f, -3843.31f, 15.354f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1581.88f, -3842.06f, 16.2485f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1575.89f, -3840.92f, 17.1104f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1570.30f, -3839.8f, 17.9687f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1564.34f, -3838.72f, 18.9013f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1558.74f, -3837.65f, 19.0262f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1552.89f, -3836.54f, 18.7007f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1547.43f, -3835.5f, 18.6492f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1541.33f, -3834.44f, 18.9799f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1536.76f, -3835.05f, 19.4686f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1531.07f, -3835.98f, 20.3061f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1523.45f, -3837.19f, 21.3995f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1517.71f, -3837.22f, 22.2306f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1511.25f, -3837.25f, 23.0413f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1505.6f, -3837.28f, 23.6246f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1500.16f, -3837.31f, 23.8903f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1495.08f, -3837.33f, 23.9026f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1489.17f, -3837.59f, 23.8869f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1482.45f, -3837.57f, 23.63f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1476.31f, -3837.61f, 23.194f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1467.31f, -3839.26f, 22.3576f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1460.5f, -3841.34f, 21.7234f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1454.07f, -3843.84f, 21.0815f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1445.73f, -3846.94f, 20.289f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1437.76f, -3847.57f, 19.8442f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1429.79f, -3848.21f, 19.3784f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1423.26f, -3848.67f, 18.9816f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1415.31f, -3847.78f, 18.5469f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1406.03f, -3845.68f, 17.8986f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1399.99f, -3843.36f, 17.8729f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1375.81f, -3840.52f, 18.7323f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1372.16f, -3838.88f, 18.8057f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1361.21f, -3832.72f, 17.926f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1355.54f, -3828.14f, 17.5429f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1348.75f, -3822.75f, 17.5781f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1338.12f, -3816.78f, 17.5464f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1328.21f, -3811.28f, 17.6097f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1320.31f, -3807.99f, 18.1579f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1313.69f, -3804.48f, 19.2114f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1305.6f, -3799.6f, 20.4015f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1302.12f, -3792.88f, 20.6411f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1296.79f, -3786.92f, 22.0117f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1290.65f, -3780.14f, 24.335f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1286.06f, -3777.14f, 26.583f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1280.86f, -3776.57f, 27.7688f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1273.94f, -3779.63f, 26.4915f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1264.78f, -3783.29f, 26.6199f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1256.33f, -3783.53f, 26.094f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1248.37f, -3784.3f, 25.5332f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1240.41f, -3785.08f, 25.1034f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1231.61f, -3784.21f, 24.472f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1223.92f, -3782.33f, 23.623f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1216.24f, -3779.79f, 24.0898f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1208.55f, -3777.58f, 24.5246f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1197.85f, -3772.64f, 23.8734f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1191.31f, -3770.45f, 23.4831f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1184.86f, -3768.21f, 23.2267f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1174.51f, -3761.98f, 22.8914f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1167.65f, -3757.85f, 22.2859f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1159.04f, -3752.67f, 21.3435f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1152.09f, -3748.49f, 20.238f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1145.65f, -3744.69f, 18.7876f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1138.34f, -3742.05f, 17.6977f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1131.72f, -3739.67f, 17.0391f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1121.53f, -3736.45f, 17.7398f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1115.49f, -3736.1f, 17.7868f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1107.49f, -3735.9f, 18.1145f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1099.49f, -3735.69f, 18.714f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1091.5f, -3735.49f, 19.1923f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1083.5f, -3735.28f, 19.3328f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1074.13f, -3734.84f, 19.4273f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1065.55f, -3730.66f, 19.8925f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1063.96f, -3727.33f, 20.5922f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1060.76f, -3719.66f, 22.14f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1057.56f, -3712.33f, 24.3349f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1054.37f, -3705.33f, 25.9164f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1050.46f, -3697.17f, 26.9811f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1048.16f, -3694.28f, 26.3632f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1042.54f, -3689.67f, 24.7195f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1036.35f, -3684.61f, 23.4694f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1030.22f, -3679.16f, 23.1939f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1024.87f, -3671.21f, 22.8851f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1019.65f, -3668.06f, 22.237f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1013.34f, -3666.59f, 21.184f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -1003.49f, -3665.46f, 19.6068f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -998.347f, -3661.75f, 19.4684f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -991.859f, -3657.07f, 18.869f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -985.371f, -3652.39f, 18.4673f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -980.152f, -3648.56f, 18.5684f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -974.281f, -3643.16f, 18.2057f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -965.768f, -3640.76f, 16.6939f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -960.052f, -3642.37f, 15.2018f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -956.27f, -3647.81f, 12.6505f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -955.801f, -3655.79f, 10.7971f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -955.332f, -3663.78f, 9.82122f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -954.707f, -3671.47f, 9.07335f, 3.098398f, 0, 256, 19805);
			sEAS.WaypointCreate(creat, -951.746f, -3678.91f, 8.21153f, 3.098398f, 0, 256, 19805);
			sEAS.EnableWaypoints(creat);
		}
};

class Gilthares_Firebough : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(Gilthares_Firebough);
		Gilthares_Firebough(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnReachWP(uint32 iWaypointId, bool bForwards)
		{
			if(iWaypointId == 100)
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Finally, I am rescued");
				_unit->Despawn(5000, 1000);
				sEAS.DeleteWaypoints(_unit);
				if(_unit->m_escorter == NULL)
					return;
				Player* plr = _unit->m_escorter;
				_unit->m_escorter = NULL;
				plr->GetQuestLogForEntry(898)->SendQuestComplete();
			}
		}
};

#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, plr); \
    Menu->SendTo(plr);

int kolkarskilled = 0;
class VerogtheDervish : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(VerogtheDervish);
		VerogtheDervish(Creature* pCreature) : CreatureAIScript(pCreature) {}
		void OnDied(Unit* mKiller)
		{
			kolkarskilled++;
			if(mKiller->IsPlayer())
			{
				Player* mPlayer = TO_PLAYER(mKiller);

				if(kolkarskilled > 8 && mPlayer->GetQuestLogForEntry(851))
				{
					_unit->GetMapMgr()->GetInterface()->SpawnCreature(3395, -1209.8f, -2729.84f, 106.33f, 4.8f, true, false, 0, 0)->Despawn(600000, 0);
					kolkarskilled = 0;
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I am slain! Summon Verog!");
				}
			}
		}

};

void SetupBarrens(ScriptMgr* mgr)
{
	GossipScript* gos = new BeatenCorpse;
	mgr->register_gossip_script(10668, gos);
	mgr->register_creature_script(3439, &Wizzlecranks_Shredder::Create);
	/*mgr->register_quest_script(863, new TheEscape());
	mgr->register_quest_script(898, new FreeFromtheHold());*/
	mgr->register_creature_script(3465, &Gilthares_Firebough::Create);
	mgr->register_creature_script(3275, &VerogtheDervish::Create);
	mgr->register_creature_script(3274, &VerogtheDervish::Create);
	mgr->register_creature_script(3397, &VerogtheDervish::Create);
	mgr->register_creature_script(4316, &VerogtheDervish::Create);
}
