
-- remove void reaver agents
DELETE FROM ai_agents WHERE entry=19516;

-- Void Reaver: arcane orb target
UPDATE creature_proto SET faction =14,minlevel =70,maxlevel =70 WHERE entry =19577;
UPDATE creature_names SET male_displayid =11686,female_displayid =0 WHERE entry =19577;