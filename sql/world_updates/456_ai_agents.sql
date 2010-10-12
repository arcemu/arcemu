ALTER TABLE ai_agents CHANGE cooldown cooldown_ovewrite INT(11)  DEFAULT "-1" NOT NULL;
ALTER TABLE ai_agents CHANGE targettype targettype_overwrite INT(11)  DEFAULT "-1" NOT NULL;