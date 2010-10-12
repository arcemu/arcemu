-- Cleaning up character_achievements_progress table
-- These values are updated in the world server when a character logs in, so there's no reason to store these progresses again to this table

-- Reach level X
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 34 AND 41;

-- Complete X quests, Total quests, Complete X quests in zone Y
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 73 AND 81;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 230 and 234;
DELETE FROM `character_achievement_progress` WHERE `criteria`=236;
DELETE FROM `character_achievement_progress` WHERE `criteria`=2239;
DELETE FROM `character_achievement_progress` WHERE `criteria`=3631;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 3683 AND 3589;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 3912 AND 3914;
DELETE FROM `character_achievement_progress` WHERE `criteria`=5019;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 5040 AND 5044;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 5900 AND 5925;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 5927 AND 5950;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 5952 AND 5977;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 5979 AND 6002;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 6004 AND 6028;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 6030 AND 6046;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 6099 AND 6123;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 6125 AND 6139;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 7884 AND 7899;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 8098 AND 8099;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 9080 AND 9081;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 9219 AND 9222;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 9238 AND 9243;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 9422 AND 9423;

-- X reputations to exalted, total Exalted Reputations
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 977 AND 982;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 984 AND 985;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 2428 AND 2429;

-- Obtain X companion pets, total Vanity pets owned, Obtain X mounts
DELETE FROM `character_achievement_progress` WHERE `criteria`=3686;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 3689 AND 3690;
DELETE FROM `character_achievement_progress` WHERE `criteria`=3692;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 4787 AND 4788;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 7550 AND 7552;
DELETE FROM `character_achievement_progress` WHERE `criteria`=9218;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 9223 AND 9224;

-- Skill X raised to level Y (Weapon/400 Profession/450)
DELETE FROM `character_achievement_progress` WHERE `criteria`=167;
DELETE FROM `character_achievement_progress` WHERE `criteria`=641;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 651 AND 657;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 753 AND 757;
DELETE FROM `character_achievement_progress` WHERE `criteria`=834;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 5240 AND 5244;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 5249 AND 5257;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 5562 AND 5592;
DELETE FROM `character_achievement_progress` WHERE `criteria`=5696;
DELETE FROM `character_achievement_progress` WHERE `criteria`=5701;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 5711 AND 5720;
DELETE FROM `character_achievement_progress` WHERE `criteria`=5722;
DELETE FROM `character_achievement_progress` WHERE `criteria`=9378;

-- Raise Skill X to level Y (Journeyman/Expert/...)
DELETE FROM `character_achievement_progress` WHERE `criteria`=168;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 612 AND 626;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 835 AND 844;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 848 AND 878;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 881 AND 902;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 1870 AND 1873;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 5551 AND 5561;

-- Explore area
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 502 AND 519;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 822 AND 832;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 903 AND 941;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 944 AND 945;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 949 AND 976;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 1014 AND 1233;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 1248 AND 1276;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 1299 AND 1398;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 1401 AND 1451;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 1453 AND 1460;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 1462 AND 1464;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 1468 AND 1470;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 1472 AND 1485;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 1504 AND 1532;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 1536 AND 1546;
DELETE FROM `character_achievement_progress` WHERE `criteria`=1548;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 1552 AND 1568;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 1573 AND 1599;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 1603 AND 1636;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 1638 AND 1639;
DELETE FROM `character_achievement_progress` WHERE `criteria`=1641;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 1647 AND 1659;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 1667 AND 1675;
DELETE FROM `character_achievement_progress` WHERE `criteria`=1679;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 1688 AND 1708;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 1713 AND 1727;
DELETE FROM `character_achievement_progress` WHERE `criteria`=1729;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 1731 AND 1743;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 1745 AND 1747;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 1762 AND 1768;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 1770 AND 1773;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 1775 AND 1781;
DELETE FROM `character_achievement_progress` WHERE `criteria`=1789;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 1791 AND 1795;
DELETE FROM `character_achievement_progress` WHERE `criteria`=2044;
DELETE FROM `character_achievement_progress` WHERE `criteria`=2104;
DELETE FROM `character_achievement_progress` WHERE `criteria`=2238;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 4122 AND 4132;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 4134 AND 4144;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 4146 AND 4155;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 4157 AND 4158;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 4159 AND 4168;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 4170 AND 4215;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 4217 AND 4218;
DELETE FROM `character_achievement_progress` WHERE `criteria`=4940;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 5290 AND 5297;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 5843 AND 5869;
DELETE FROM `character_achievement_progress` WHERE `criteria` BETWEEN 5871 AND 5874;
DELETE FROM `character_achievement_progress` WHERE `criteria`=8749;
