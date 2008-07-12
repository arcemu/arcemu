-- Netherspite Infernal Level Fix, so players dont resist its spells
update `creature_proto` set `maxlevel`='70', `minlevel`='70' where `entry`='17646';