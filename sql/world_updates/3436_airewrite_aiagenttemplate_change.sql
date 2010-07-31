ALTER TABLE ai_agents 
	DROP PRIMARY KEY,
	DROP COLUMN floatMisc1,
	DROP COLUMN Misc2,
	ADD COLUMN attributes VARCHAR(255),
	CHANGE entry entry INT(6),
	CHANGE instance_mode difficulty TINYINT(2) UNSIGNED NOT NULL DEFAULT '0',
	CHANGE `type` agent_type TINYINT(2) UNSIGNED NOT NULL DEFAULT '2',
	CHANGE spell spell_id INT(6) UNSIGNED NOT NULL,
	CHANGE spelltype spelltype TINYINT(2) UNSIGNED NOT NULL DEFAULT '1',
	CHANGE targettype_overwrite targettype TINYINT(2) UNSIGNED NOT NULL DEFAULT '3',
	CHANGE event creature_event TINYINT(2) UNSIGNED NOT NULL,
	CHANGE cooldown_overwrite `interval` INT(11) UNSIGNED NOT NULL;

DROP TABLE IF EXISTS petdefaultspells;