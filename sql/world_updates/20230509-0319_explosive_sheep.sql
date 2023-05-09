UPDATE creature_proto SET can_ranged = 0 WHERE entry = 2675;
DELETE FROM ai_agents WHERE entry = 2675;
INSERT INTO ai_agents VALUES( 2675, 4, 4, 0, 100, 0, 4050, 7, -1, 0, 0, 0 );
