ALTER TABLE `quests` ADD `MailTemplateId` INT( 10 ) UNSIGNED NOT NULL DEFAULT '0' AFTER `CastSpell`;
ALTER TABLE `quests` ADD `MailDelaySecs` INT( 10 ) UNSIGNED NOT NULL DEFAULT '0' AFTER `MailTemplateId`;
ALTER TABLE `quests` ADD `MailSendItem` INT( 10 ) UNSIGNED NOT NULL DEFAULT '0' AFTER `MailDelaySecs`;
UPDATE `arcemu_db_version` SET `LastUpdate` = '4681';