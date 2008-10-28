-- Curse of Shadows: This spell has been removed.

UPDATE `characters` SET `spells` = REPLACE(spells, ",27229,",",");
