ALTER TABLE `worldmap_info` ADD COLUMN `minlevel_heroic` INT(10) UNSIGNED DEFAULT '0' NOT NULL AFTER `minlevel`;
-- thanks heavyathan for compiling the list http://arcemu.org/forums/index.php?showtopic=10979&view=findpost&p=53941
UPDATE `worldmap_info` SET `minlevel_heroic`=80 WHERE `entry` IN (595,578,602,575,599,604,608,600,619,601,576,574,533,616,615,624);
UPDATE `worldmap_info` SET `minlevel_heroic`=70 WHERE `entry` IN (555,269,545,540,585,552,553,554,556,560,558,557,546,547,542,543,548,565,544,532,568,580);
