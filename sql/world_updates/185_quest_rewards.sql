alter table quests modify column RewMoney int(10) not null default 0;
alter table quests change column RequiredMoney RewardMoneyAtMaxLevel int(10) unsigned not null default 0;