-- CN_Ghamoora 4887
REPLACE INTO `ai_agents`(`entry`,`instance_mode`,`type`,`event`,`chance`,`maxcount`,`spell`,`spelltype`,`targettype_overwrite`,`cooldown_overwrite`,`floatMisc1`,`Misc2`)VALUES('4887','4','4','0','12','0','5568','8','4','0','0','0'); 
-- CN_Gelihast 6243
REPLACE INTO `ai_agents`(`entry`,`instance_mode`,`type`,`event`,`chance`,`maxcount`,`spell`,`spelltype`,`targettype_overwrite`,`cooldown_overwrite`,`floatMisc1`,`Misc2`)VALUES('6243','4','4','0','10','0','6533','1','1','-1','0','0'); 
-- CN_LorgusJett 12902
UPDATE `creature_proto` SET `auras`='12550' WHERE `entry`='6243';
