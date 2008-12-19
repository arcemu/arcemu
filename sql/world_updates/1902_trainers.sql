alter table `trainer_defs` add column `RequiredRace` int DEFAULT '0' NOT NULL after `req_class`;
alter table add column `RequiredReputation` int DEFAULT '0' NOT NULL after `RequiredRace`;
alter table add column `RequiredReputationValue` int DEFAULT '0' NOT NULL after `RequiredReputation`;