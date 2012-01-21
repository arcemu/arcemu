-- reverting 3006 and 3007
ALTER TABLE `creature_proto` DROP COLUMN `FlagsExtra`;
ALTER TABLE `creature_proto` ADD COLUMN `summonguard` int UNSIGNED DEFAULT '0' NOT NULL after `guardtype`;
