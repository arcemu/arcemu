ALTER TABLE vendors ADD extended_cost INT NOT NULL DEFAULT '0';

-- fill extended_cost collumn with basic data from items_extendedcost table
UPDATE vendors SET extended_cost = ( SELECT CostId FROM items_extendedcost WHERE ItemId = vendors.item );

-- uncomment this to delete old table
-- DROP TABLE items_extendedcost;
