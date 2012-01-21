CREATE TABLE `playersummons` (
  `ownerguid` INT(11) UNSIGNED NOT NULL DEFAULT '0',
  `entry` INT(11) UNSIGNED NOT NULL DEFAULT '0',
  `name` varchar(64) NOT NULL,
  KEY `a` (`ownerguid`)
)
