-- New table
DROP TABLE IF EXISTS `playerskills`;

CREATE TABLE `playerskills` (
  `GUID` INT(10) UNSIGNED NOT NULL,
  `SkillID` INT(10) UNSIGNED NOT NULL,
  `CurrentValue` INT(10) UNSIGNED NOT NULL,
  `MaximumValue` INT(10) UNSIGNED NOT NULL,
  PRIMARY KEY (`GUID`,`SkillID`)
) ENGINE=MYISAM DEFAULT CHARSET=latin1;


-- Filling up the new table with the old data
DROP PROCEDURE IF EXISTS `parse_and_insert_skill_rows`;
DROP PROCEDURE IF EXISTS `insert_skill_row`;
DROP PROCEDURE IF EXISTS `convert_skills`;

DELIMITER $$

CREATE PROCEDURE `insert_skill_row`( `in_guid` INTEGER, `in_skillid` INTEGER, `in_currval` INTEGER, `in_maxval` INTEGER )
BEGIN
	INSERT INTO `playerskills` VALUES( `in_guid`, `in_skillid`, `in_currval`, `in_maxval` );
END$$

DELIMITER ;


DELIMITER $$

CREATE PROCEDURE `parse_and_insert_skill_rows`( `in_index` INTEGER, `in_string` TEXT )
BEGIN
	DECLARE `prevpos` INTEGER;
	DECLARE `currpos` INTEGER;
	DECLARE `str_len` INTEGER;
	DECLARE `in_delim` CHAR;
	DECLARE `value` VARCHAR(10);	
	DECLARE `skillid` VARCHAR(10);
	DECLARE `currval` VARCHAR(10);
	DECLARE `maxval` VARCHAR(10);	
	DECLARE `substrlen` INTEGER;
	DECLARE `startpos` INTEGER;
	DECLARE `currchar` CHAR;
	DECLARE `token_number` INTEGER;
	
	SET `in_delim` = ';';
	SET `prevpos` = '0';
	SET `currpos` = LOCATE( `in_delim`, `in_string` );
	SET `str_len` = LENGTH( `in_string` );
	SET `token_number` = '0';
	SET `skillid` = '';
	SET `currval` = '';
	SET `maxval` = '';
	
	IF `currpos` != '0' THEN
		WHILE `currpos` <= `str_len` DO
			SET `currchar` = SUBSTR( `in_string`, `currpos`, 1 );
			
			IF `currchar` = `in_delim` THEN
				SET `token_number` = `token_number` + '1';
				SET `startpos` = `prevpos` + '1';
				SET `substrlen` = `currpos`- `prevpos` - '1';
				SET `value` = SUBSTR( `in_string`, `startpos`, `substrlen` );
				
				IF `token_number` = '1' THEN
					SET `skillid` = `value`;
				END IF;
				
				IF `token_number` = '2' THEN
					SET `currval` = `value`;
				END IF;
				
				IF `token_number` = '3' THEN
					SET `maxval` = `value`;
					
					CALL `insert_skill_row`( `in_index`, `skillid`, `currval`, `maxval` );
					
					SET `skillid` = '';
					SET `currval` = '';
					SET `maxval` = '';
					SET `token_number` = '0';
				END IF;
			
				SET `prevpos` = `currpos`;
			END IF;		
			SET `currpos` = `currpos` + '1';
		END WHILE;		
	END IF;

END$$

DELIMITER ;

DELIMITER $$

CREATE PROCEDURE `convert_skills`()
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
		SELECT `skills` INTO `myfield` FROM `characters` WHERE `guid` = `ci`;
		CALL `parse_and_insert_skill_rows`( `ci`, `myfield` );
		FETCH `c` INTO `ci`;
	END WHILE;
	
	CLOSE `c`;
END$$

DELIMITER ;

CALL `convert_skills`();

DROP PROCEDURE IF EXISTS `parse_and_insert_skill_rows`;
DROP PROCEDURE IF EXISTS `insert_skill_row`;
DROP PROCEDURE IF EXISTS `convert_skills`;

-- Dropping the old field
ALTER TABLE `characters` DROP COLUMN `skills`;
