-- Delete Kael's Advisors loot, they should not drop the legendaries
delete from creatureloot where entryid in (20064, 20060, 20062, 20063);

-- Advisors should have equiped the legendary weapons
UPDATE creature_proto SET equipmodel1 = 45345, equipinfo1 = 33490690, equipslot1 = 789  where entry = 20060; -- Lord Sanguinar
update creature_proto set equipmodel2 = '40867', equipslot2='1038', equipinfo2='33490436' where entry='20060'; -- Lord Sanguinar Shield

UPDATE creature_proto SET equipmodel1 = 41895, equipinfo1 = 50268674, equipslot1 = 512  where entry = 20062; -- capernian
UPDATE creature_proto SET equipmodel1 = 41872, equipinfo1 = 33490690, equipslot1 = 789  where entry = 20063; -- Telonicus
UPDATE creature_proto SET equipmodel1 = 41560, equipinfo1 = 33490946, equipslot1 = 256  where entry = 20064; -- Thaladred The Darkener

-- Thaladread should 1 shot ppl
UPDATE creature_proto SET mindamage = 15000, maxdamage = 20000 where entry = 20064; -- Thaladred The Darkener

-- Legendary wep's have equip stuff messed up.
update creature_proto set equipmodel1 = '0', equipslot1='0', equipinfo1='0' where entry='21273'; -- Phaseshift Bulwark
update creature_proto set equipmodel2 = '40867', equipslot2='1038', equipinfo2='33490436' where entry='21273'; -- Phaseshift Bulwark

update creature_proto set equipslot1='512',  equipinfo1='50268674' where entry='21274'; -- Staf of disintegraton
update creature_proto set equipslot1='256',  equipinfo1='33490946' where entry='21269'; -- Devastation