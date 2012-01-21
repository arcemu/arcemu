alter table `trainer_defs` add column `RequiredRace` int DEFAULT '0' NOT NULL after `req_class`;
alter table `trainer_defs` add column `RequiredReputation` int DEFAULT '0' NOT NULL after `RequiredRace`;
alter table `trainer_defs` add column `RequiredReputationValue` int DEFAULT '0' NOT NULL after `RequiredReputation`;