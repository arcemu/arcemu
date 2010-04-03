ALTER TABLE `gameobject_names`
  ADD COLUMN `QuestItem5` int(11) UNSIGNED DEFAULT '0' NOT NULL after `QuestItem4`,
  ADD COLUMN `QuestItem6` int(11) UNSIGNED DEFAULT '0' NOT NULL after `QuestItem5`;

ALTER TABLE `creature_names`
  ADD COLUMN `QuestItem1` int(11) UNSIGNED DEFAULT '0' NOT NULL after `info_str`,
  ADD COLUMN `QuestItem2` int(11) UNSIGNED DEFAULT '0' NOT NULL after `QuestItem1`,
  ADD COLUMN `QuestItem3` int(11) UNSIGNED DEFAULT '0' NOT NULL after `QuestItem2`,
  ADD COLUMN `QuestItem4` int(11) UNSIGNED DEFAULT '0' NOT NULL after `QuestItem3`,
  ADD COLUMN `QuestItem5` int(11) UNSIGNED DEFAULT '0' NOT NULL after `QuestItem4`,
  ADD COLUMN `QuestItem6` int(11) UNSIGNED DEFAULT '0' NOT NULL after `QuestItem5`;

ALTER TABLE `items`
  ADD COLUMN `faction` int(11) UNSIGNED DEFAULT '0' NOT NULL after `flags`;
