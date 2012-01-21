alter table `playercreateinfo` add column taximask text(255);

-- human
update `playercreateinfo` set taximask="2" where race=1;
-- orc (2), troll (8)
update `playercreateinfo` set taximask="4194304" where race in (2, 8);
-- dwarf (3), gnome (7)
update `playercreateinfo` set taximask="32" where race in (3, 7);
-- night elf
update `playercreateinfo` set taximask="100663296" where race=4;
-- undead
update `playercreateinfo` set taximask="1024" where race=5;
-- tauren
update `playercreateinfo` set taximask="2097152" where race=6;
-- blood elf
update `playercreateinfo` set taximask="0 0 131072" where race=10;
-- draenei
update `playercreateinfo` set taximask="0 0 536870912" where race=11;

-- death knight
update `playercreateinfo` set taximask="4294967295 4093640703 830406655 0 33570816 1310944 3250593812 73752 896 67111952" where class=6;
