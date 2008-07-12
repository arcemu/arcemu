-- Trenton Lighthammer Gossip
DELETE FROM npc_gossip_textid WHERE (`creatureid`='7804');
INSERT INTO `npc_gossip_textid` (`creatureid`,`textid`) VALUES ('7804','1758');
UPDATE `creature_proto` SET `npcflags`='3' WHERE (`entry`='7804');