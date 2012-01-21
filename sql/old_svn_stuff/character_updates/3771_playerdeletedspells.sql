-- New table
DROP TABLE IF EXISTS `playerdeletedspells`;

CREATE TABLE `playerdeletedspells` (
  `GUID` INT(10) UNSIGNED NOT NULL,
  `SpellID` INT(10) UNSIGNED NOT NULL,
  PRIMARY KEY (`GUID`,`SpellID`)
) ENGINE=MYISAM DEFAULT CHARSET=latin1;

-- Filling up the new table with the old data
DROP PROCEDURE IF EXISTS `insert_dspell_row`;
DROP PROCEDURE IF EXISTS `parse_and_insert_dspell_rows`;
DROP PROCEDURE IF EXISTS `convert_dspells`;

DELIMITER $$

CREATE PROCEDURE `insert_dspell_row`( `in_entry` INTEGER, `in_value` INTEGER )
BEGIN
	INSERT INTO `playerdeletedspells` VALUES( `in_entry`, `in_value` );
END$$

DELIMITER ;


DELIMITER $$

CREATE PROCEDURE `parse_and_insert_dspell_rows`( `in_index` INTEGER, `in_string` TEXT )
BEGIN
	DECLARE `prevpos` INTEGER;
	DECLARE `currpos` INTEGER;
	DECLARE `str_len` INTEGER;
	DECLARE `in_delim` CHAR;
	DECLARE `spellid` VARCHAR(10);
	DECLARE `substrlen` INTEGER;
	DECLARE `startpos` INTEGER;
	DECLARE `currchar` CHAR;
	
	SET `in_delim` = ',';
	SET `prevpos` = '0';
	SET `currpos` = LOCATE( `in_delim`, `in_string` );
	SET `str_len` = LENGTH( `in_string` );
	
	IF `currpos` != '0' THEN
		WHILE `currpos` <= `str_len` DO
			SET `currchar` = SUBSTR( `in_string`, `currpos`, 1 );
			
			IF `currchar` = `in_delim` THEN
				SET `startpos` = `prevpos` + '1';
				SET `substrlen` = `currpos`- `prevpos` - '1';
				SET `spellid` = SUBSTR( `in_string`, `startpos`, `substrlen` );
				
				CALL `insert_dspell_row`( `in_index`, `spellid` );
				
				SET `prevpos` = `currpos`;
			END IF;		
			SET `currpos` = `currpos` + '1';
		END WHILE;		
	END IF;

END$$

DELIMITER ;

DELIMITER $$

CREATE PROCEDURE `convert_dspells`()
BEGIN
	DECLARE `ci` INTEGER;
	DECLARE `no_more_rows` INTEGER;
	DECLARE `myfield` TEXT;
	
	DECLARE `c` CURSOR FOR
		SELECT `guid` FROM `characters`;
		
	DECLARE CONTINUE HANDLER FOR NOT FOUND SET `no_more_rows` = '1';
	
	SET `no_more_rows` = '0';
	
	OPEN `c`;
	
	FETCH `c` INTO `ci`;
	
	WHILE `no_more_rows` = '0' DO
		SELECT `deleted_spells` INTO `myfield` FROM `characters` WHERE `guid` = `ci`;
		CALL `parse_and_insert_dspell_rows`( `ci`, `myfield` );
		FETCH `c` INTO `ci`;
	END WHILE;
	
	CLOSE `c`;
END$$

DELIMITER ;

CALL `convert_dspells`();

DROP PROCEDURE IF EXISTS `insert_dspell_row`;
DROP PROCEDURE IF EXISTS `parse_and_insert_dspell_rows`;
DROP PROCEDURE IF EXISTS `convert_dspells`;

-- Dropping the old field
ALTER TABLE `characters` DROP COLUMN `deleted_spells`;
