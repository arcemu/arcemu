UPDATE `gameobject_names` SET `sound3`='45145' WHERE (`entry`='183957');
UPDATE `creature_proto` SET `minhealth`='100', `maxhealth`='100', `mindamage`='20', `maxdamage`='25' WHERE `entry`='19921' or `entry`='19833';
DELETE FROM ai_agents WHERE `entry`= '19833';
DELETE FROM ai_agents WHERE `entry`= '19921';
INSERT INTO ai_agents
   (`entry`, `type`, `event`, `chance`, `maxcount`, `spell`, `spelltype`, `targettype`, `cooldown`, `floatMisc1`, `Misc2`)
VALUES
   (19833, 4, 2, 10, 0, 25809, 11, 1, 2000, 0, 0);

INSERT INTO ai_agents
   (`entry`, `type`, `event`, `chance`, `maxcount`, `spell`, `spelltype`, `targettype`, `cooldown`, `floatMisc1`, `Misc2`)
VALUES
   (19833, 4, 2, 10, 0, 25810, 11, 1, 2000, 0, 0);

INSERT INTO ai_agents
   (`entry`, `type`, `event`, `chance`, `maxcount`, `spell`, `spelltype`, `targettype`, `cooldown`, `floatMisc1`, `Misc2`)
VALUES
   (19833, 4, 2, 10, 0, 41192, 8, 1, 2000, 0, 0);

INSERT INTO ai_agents
   (`entry`, `type`, `event`, `chance`, `maxcount`, `spell`, `spelltype`, `targettype`, `cooldown`, `floatMisc1`, `Misc2`)
VALUES
   (19921, 4, 2, 10, 0, 25809, 11, 1, 2000, 0, 0);

INSERT INTO ai_agents
   (`entry`, `type`, `event`, `chance`, `maxcount`, `spell`, `spelltype`, `targettype`, `cooldown`, `floatMisc1`, `Misc2`)
VALUES
   (19921, 4, 2, 10, 0, 25810, 11, 1, 2000, 0, 0);

INSERT INTO ai_agents
   (`entry`, `type`, `event`, `chance`, `maxcount`, `spell`, `spelltype`, `targettype`, `cooldown`, `floatMisc1`, `Misc2`)
VALUES
   (19921, 4, 2, 10, 0, 34655, 8, 1, 2000, 0, 0);
