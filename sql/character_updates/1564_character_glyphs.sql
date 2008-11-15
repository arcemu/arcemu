ALTER TABLE `characters` ADD COLUMN `glyphs` longtext CHARSET latin1 NOT NULL after `difficulty`;
UPDATE `characters` SET glyphs = '0,0,0,0,0,0,0,0,';