ALTER TABLE `mailbox` CHANGE `sender_guid` `sender_guid` BIGINT(30) DEFAULT '0' NOT NULL;
ALTER TABLE `mailbox_insert_queue` CHANGE `sender_guid` `sender_guid` BIGINT(30) DEFAULT '0' NOT NULL;
UPDATE `arcemu_db_version` SET `LastUpdate`='4477';