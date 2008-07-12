/*Solarian & Friends faction chain pull, & so they dont attack eachother*/
UPDATE creature_proto SET faction = 16 where entry = 18925;
UPDATE creature_proto SET faction = 16 where entry = 18806;
UPDATE creature_proto SET faction = 16 where entry = 18805;