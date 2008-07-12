alter table `creature_proto` add column `spell1` int(30) DEFAULT '0' NOT NULL after `extra_a9_flags`;
alter table `creature_proto` add column `spell2` int(30) DEFAULT '0' NOT NULL after `spell1`;
alter table `creature_proto` add column `spell3` int(30) DEFAULT '0' NOT NULL after `spell2`;
alter table `creature_proto` add column `spell4` int(30) DEFAULT '0' NOT NULL after `spell3`;