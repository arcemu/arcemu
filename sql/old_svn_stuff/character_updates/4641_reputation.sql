DROP TABLE IF EXISTS `playerreputations`;

CREATE TABLE `playerreputations` (
  `guid` INT UNSIGNED NOT NULL,
  `faction` INT UNSIGNED NOT NULL,
  `flag` INT UNSIGNED NOT NULL DEFAULT '0',
  `basestanding` INT NOT NULL DEFAULT '0',
  `standing` INT NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`faction`)
) ENGINE=MYISAM;

DROP PROCEDURE IF EXISTS `insert_rep_row`;
DROP PROCEDURE IF EXISTS `parse_and_insert_rep_rows`;
DROP PROCEDURE IF EXISTS `convert_rep`;

DELIMITER $

CREATE PROCEDURE `insert_rep_row` ( `in_guid` INTEGER, `in_faction` INTEGER, `in_flag` INTEGER, `in_basestanding` INTEGER, `in_standing` INTEGER )
BEGIN
        INSERT INTO `playerreputations` VALUE (`in_guid`, `in_faction`, `in_flag`, `in_basestanding`, `in_standing`);
END $

DELIMITER ;


DELIMITER $

CREATE PROCEDURE `parse_and_insert_rep_rows` ( `in_index` INTEGER, `in_string` TEXT)
BEGIN

        DECLARE `prevpos` INTEGER;
        DECLARE `currpos` INTEGER;
        DECLARE `str_len` INTEGER;
        DECLARE `in_delim` CHAR;
        DECLARE `faction` VARCHAR(10);
        DECLARE `flag` VARCHAR(10);
        DECLARE `basestanding` VARCHAR(10);
        DECLARE `standing` VARCHAR(10);
        DECLARE `startpos` INTEGER;
        DECLARE `substrlen` INTEGER;
        DECLARE `currchar` CHAR;
        DECLARE `counts` INTEGER;

        SET `in_delim` = ',';
        SET `prevpos` = '0';
        SET `counts` = '1';
        SET `currpos` = LOCATE( `in_delim`, `in_string` );
        SET `str_len` = LENGTH( `in_string` );
                
        IF `currpos` != '0' THEN
                WHILE `currpos` <= `str_len` DO
                
                                SET `currchar` = SUBSTR( `in_string`, `currpos`, 1);
                                                
                                IF `currchar` = `in_delim` THEN
                                        SET `startpos` = `prevpos` + '1';
                                        SET `substrlen` = `currpos` - `prevpos` - '1';
                                        
                                        IF `counts` = '1' THEN
                                                SET `faction` = SUBSTR( `in_string`, `startpos`, `substrlen` );
                                                SET `counts` = '2';
                                                SET `prevpos` = `currpos`;
                                        ELSE
                                                IF `counts` = '2' THEN
                                                        SET `flag` = SUBSTR( `in_string`, `startpos`, `substrlen` );
                                                        SET `counts` = '3';
                                                        SET `prevpos` = `currpos`;                                                      
                                                ELSE
                                                        IF `counts` = '3' THEN
                                                                SET `basestanding` = SUBSTR( `in_string`, `startpos`, `substrlen` );
                                                                SET `counts` = '4';
                                                                SET `prevpos` = `currpos`;
                                                        ELSE
                                                                IF `counts` = '4' THEN
                                                                        SET `standing` = SUBSTR( `in_string`, `startpos`, `substrlen` );
                                                                        SET `counts` = '1';
                                                                        CALL `insert_rep_row`( `in_index`, `faction`, `flag`, `basestanding`, `standing` );
                                                                        SET `prevpos` = `currpos`;
                                                                END IF;
                                                        END IF;
                                                END IF;
                                        END IF;
                                END IF;
                                
                                SET `currpos` = `currpos` + '1';
                                                                        
                END WHILE;
        END IF;

END$

DELIMITER ;

DELIMITER $

CREATE PROCEDURE `convert_rep`()
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
                SELECT `reputation` INTO `myfield` FROM `characters` WHERE `guid` = `ci`;
                CALL `parse_and_insert_rep_rows`( `ci`, `myfield` );
                FETCH `c` INTO `ci`;
        END WHILE;
        
        CLOSE `c`;
END$

DELIMITER ;

CALL `convert_rep`();

DROP PROCEDURE IF EXISTS `insert_rep_row`;
DROP PROCEDURE IF EXISTS `parse_and_insert_rep_rows`;
DROP PROCEDURE IF EXISTS `convert_rep`;

ALTER TABLE `characters` DROP COLUMN `reputation`;

UPDATE `arcemu_db_version` SET `LastUpdate` = '4641';
