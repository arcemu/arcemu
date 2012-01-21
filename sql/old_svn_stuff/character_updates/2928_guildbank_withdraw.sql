-- fix unlimited gold withdraw for guild masters
UPDATE guild_ranks SET goldLimitPerDay = '-1' WHERE rankId = '0';
