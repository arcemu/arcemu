ALTER TABLE `gameobject_names`
  ADD COLUMN `Size` float NOT NULL DEFAULT '1' after `UnkStr`,
  ADD COLUMN `QuestItem1` int(11) UNSIGNED DEFAULT '0' NOT NULL after `Size`,
  ADD COLUMN `QuestItem2` int(11) UNSIGNED DEFAULT '0' NOT NULL after `QuestItem1`,
  ADD COLUMN `QuestItem3` int(11) UNSIGNED DEFAULT '0' NOT NULL after `QuestItem2`,
  ADD COLUMN `QuestItem4` int(11) UNSIGNED DEFAULT '0' NOT NULL after `QuestItem3`;
