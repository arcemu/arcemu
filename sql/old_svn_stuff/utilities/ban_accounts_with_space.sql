-- This SQL query will ban all accounts with a space in their name
UPDATE accounts SET banned = 1 WHERE login LIKE '% %'