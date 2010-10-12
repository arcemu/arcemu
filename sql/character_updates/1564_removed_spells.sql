# DEVS: Keep updating this file, rather then making more then one :)
# Users!: Make sure to enable continue on errors if you run this file after the first time!

UPDATE `characters` SET `spells` = REPLACE(spells, ",20218,",",");
UPDATE `characters` SET `spells` = REPLACE(spells, ",31869,",",");
UPDATE `characters` SET `spells` = REPLACE(spells, ",31870,",",");
