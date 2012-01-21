ALTER TABLE vendors ADD extended_cost INT NOT NULL DEFAULT '0';

-- fill extended_cost collumn with basic data from items_extendedcost table
UPDATE vendors,items_extendedcost SET extended_cost = CostId where ItemId = item;

-- uncomment this to delete old table
-- DROP TABLE items_extendedcost;
