-- [Mage] Arcane Focus
INSERT INTO spellfixes(spellId, groupRelation0) VALUES(11222, 0x20001800) ON DUPLICATE KEY UPDATE groupRelation0=VALUES(groupRelation0);
INSERT INTO spellfixes(spellId, groupRelation0) VALUES(12839, 0x20001800) ON DUPLICATE KEY UPDATE groupRelation0=VALUES(groupRelation0);
INSERT INTO spellfixes(spellId, groupRelation0) VALUES(12840, 0x20001800) ON DUPLICATE KEY UPDATE groupRelation0=VALUES(groupRelation0);

-- [Mage] Elemental Precision
INSERT INTO spellfixes(spellId, groupRelation0) VALUES(29438, 0xD000D7) ON DUPLICATE KEY UPDATE groupRelation0=VALUES(groupRelation0);
INSERT INTO spellfixes(spellId, groupRelation0) VALUES(29439, 0xD000D7) ON DUPLICATE KEY UPDATE groupRelation0=VALUES(groupRelation0);
INSERT INTO spellfixes(spellId, groupRelation0) VALUES(29440, 0xD000D7) ON DUPLICATE KEY UPDATE groupRelation0=VALUES(groupRelation0);

-- [Rogue] Surprise Attacks
INSERT INTO spellfixes(spellId, groupRelation1) VALUES(32601, 0x20800008) ON DUPLICATE KEY UPDATE groupRelation1=VALUES(groupRelation1);

-- [Rogue] Improved Sprint
INSERT INTO spellfixes(spellId, groupRelation0) VALUES(13743, 0x40) ON DUPLICATE KEY UPDATE groupRelation0=VALUES(groupRelation0);
INSERT INTO spellfixes(spellId, groupRelation0) VALUES(13875, 0x40) ON DUPLICATE KEY UPDATE groupRelation0=VALUES(groupRelation0);

-- [Hunter] Clever Traps
INSERT INTO spellfixes(spellId, groupRelation0) VALUES(19239, 0x18) ON DUPLICATE KEY UPDATE groupRelation0=VALUES(groupRelation0);
INSERT INTO spellfixes(spellId, groupRelation1) VALUES(19239, 0x200000000004) ON DUPLICATE KEY UPDATE groupRelation1=VALUES(groupRelation1);
INSERT INTO spellfixes(spellId, groupRelation0) VALUES(19245, 0x18) ON DUPLICATE KEY UPDATE groupRelation0=VALUES(groupRelation0);
INSERT INTO spellfixes(spellId, groupRelation1) VALUES(19245, 0x200000000004) ON DUPLICATE KEY UPDATE groupRelation1=VALUES(groupRelation1);

-- [Hunter] Barrage
INSERT INTO spellfixes(spellId, groupRelation0) VALUES(19461, 0x3000) ON DUPLICATE KEY UPDATE groupRelation0=VALUES(groupRelation0);
INSERT INTO spellfixes(spellId, groupRelation1) VALUES(19461, 0x3000) ON DUPLICATE KEY UPDATE groupRelation1=VALUES(groupRelation1);
INSERT INTO spellfixes(spellId, groupRelation0) VALUES(19462, 0x3000) ON DUPLICATE KEY UPDATE groupRelation0=VALUES(groupRelation0);
INSERT INTO spellfixes(spellId, groupRelation1) VALUES(19462, 0x3000) ON DUPLICATE KEY UPDATE groupRelation1=VALUES(groupRelation1);
INSERT INTO spellfixes(spellId, groupRelation0) VALUES(24691, 0x3000) ON DUPLICATE KEY UPDATE groupRelation0=VALUES(groupRelation0);
INSERT INTO spellfixes(spellId, groupRelation1) VALUES(24691, 0x3000) ON DUPLICATE KEY UPDATE groupRelation1=VALUES(groupRelation1);

-- [Warlock] Mana Feed
INSERT INTO spellfixes(spellId, groupRelation0) VALUES(30326, 0x40010) ON DUPLICATE KEY UPDATE groupRelation0=VALUES(groupRelation0);
INSERT INTO spellfixes(spellId, groupRelation0) VALUES(30327, 0x40010) ON DUPLICATE KEY UPDATE groupRelation0=VALUES(groupRelation0);
INSERT INTO spellfixes(spellId, groupRelation0) VALUES(30328, 0x40010) ON DUPLICATE KEY UPDATE groupRelation0=VALUES(groupRelation0);

-- [Paladin] Divine Favor
INSERT INTO spellfixes(spellId, groupRelation0) VALUES(20216, 0x10000C0200000) ON DUPLICATE KEY UPDATE groupRelation0=VALUES(groupRelation0);

-- [Paladin] Improved Concentration Aura	
INSERT INTO spellfixes(spellId, groupRelation1) VALUES(20254, 0x20000) ON DUPLICATE KEY UPDATE groupRelation1=VALUES(groupRelation1);
INSERT INTO spellfixes(spellId, groupRelation2) VALUES(20254, 0x20000) ON DUPLICATE KEY UPDATE groupRelation2=VALUES(groupRelation2);
INSERT INTO spellfixes(spellId, groupRelation1) VALUES(20255, 0x20000) ON DUPLICATE KEY UPDATE groupRelation1=VALUES(groupRelation1);
INSERT INTO spellfixes(spellId, groupRelation2) VALUES(20255, 0x20000) ON DUPLICATE KEY UPDATE groupRelation2=VALUES(groupRelation2);
INSERT INTO spellfixes(spellId, groupRelation1) VALUES(20256, 0x20000) ON DUPLICATE KEY UPDATE groupRelation1=VALUES(groupRelation1);
INSERT INTO spellfixes(spellId, groupRelation2) VALUES(20256, 0x20000) ON DUPLICATE KEY UPDATE groupRelation2=VALUES(groupRelation2);

-- Improved Bloodrage
INSERT INTO spellfixes(spellId, groupRelation0) VALUES(12301, 0x100) ON DUPLICATE KEY UPDATE groupRelation0=VALUES(groupRelation0);
