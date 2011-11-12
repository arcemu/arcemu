DROP TABLE IF EXISTS `arcemu_db_version`;
DROP TABLE IF EXISTS `character_db_version`;

CREATE TABLE `character_db_version`(
  `LastUpdate` VARCHAR(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`LastUpdate`)
);

INSERT INTO `character_db_version`(`LastUpdate`) VALUES ( '2011-11-12_20-00_initial');