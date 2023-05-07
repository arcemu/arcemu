-- Ejector Mechano-Tank
DELETE FROM spellclickspells WHERE CreatureID = 39715;
INSERT INTO spellclickspells VALUES( 39715, 74204 );

-- Scuttling Mechano-Tank
UPDATE creature_proto SET spell4 = 74153 WHERE entry = 39713;
DELETE FROM spellclickspells WHERE CreatureID = 39716;
INSERT INTO spellclickspells VALUES( 39716, 74203 );

-- Shooting Mechano-Tank
UPDATE creature_proto SET spell1 = 74174 WHERE entry = 39714;
UPDATE creature_proto SET spell2 = 74153 WHERE entry = 39714;
DELETE FROM spellclickspells WHERE CreatureID = 39717;
INSERT INTO spellclickspells VALUES( 39717, 74205 );
