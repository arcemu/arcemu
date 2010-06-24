ALTER TABLE ai_agents CHANGE instance_mode difficulty TINYINT(2) NOT NULL DEFAULT '0';
ALTER TABLE ai_agents CHANGE `type` agent_type TINYINT(2) NOT NULL DEFAULT '4';
ALTER TABLE ai_agents CHANGE event creature_event TINYINT(2) NOT NULL DEFAULT '0';
ALTER TABLE ai_agents CHANGE spell spell_id;
ALTER TABLE ai_agents DROP COLUMN floatmisc1;
ALTER TABLE ai_agents DROP COLUMN misc2;
ALTER TABLE ai_agents ADD COLUMN attributes VARCHAR(255);