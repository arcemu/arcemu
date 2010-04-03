alter table `playeritems` change column `expireson` `duration_expireson` int(10) UNSIGNED DEFAULT '0' NOT NULL;

-- noob proofing because some people tend to readd this with earlier updates...
alter table `playeritems` drop column `expireson`;

alter table `playeritems` add column `refund_purchasedon` int(10) UNSIGNED DEFAULT '0' NOT NULL after `duration_expireson`;
alter table `playeritems` add column `refund_costid` int(10) UNSIGNED DEFAULT '0' NOT NULL after `refund_purchasedon`;
