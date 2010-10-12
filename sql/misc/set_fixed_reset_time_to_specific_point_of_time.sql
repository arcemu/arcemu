-- You can change the following two variables to the value you like.
-- Do not forget to replace the database names at the last two lines.

-- day of week: Wednesday
SET @DAY_OF_WEEK = 3;
-- hour of day: 5:00AM
SET @HOUR_OF_DAY = 5;
-- Timezone: GMT+1
SET @TIMEZONE = 1;

-- You have to insert your database names into the sql statement before executing it.
-- Example:
--   If your character database is 'cdb', it should look like: REPLACE INTO `cdb`.`server_settings` ( ...
--   If your world database is 'wdb', it should look like: FROM `wdb`.`worldmap_info` AS ...


--REPLACE INTO `!!! Insert your character database here !!!`.`server_settings` (`setting_id`, `setting_value`) SELECT CONCAT('next_instance_reset_', `map`.`entry`) AS `setting_id`, (@@TIMESTAMP - 345600 - (@@TIMESTAMP % 604800) - (@TIMEZONE * 3600) + (@DAY_OF_WEEK * 86400) + ((@HOUR_OF_DAY - @TIMEZONE) * 3600)) AS `setting_value`
--        FROM `!!! Insert your world database here!!!`.`worldmap_info` AS `map` WHERE `map`.`type` IN (1,4);
