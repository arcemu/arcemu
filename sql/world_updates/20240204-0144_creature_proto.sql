ALTER TABLE creature_proto ADD COLUMN mount_display_id INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER rooted;
ALTER TABLE creature_proto ADD COLUMN item1_display_id INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER mount_display_id;
ALTER TABLE creature_proto ADD COLUMN item2_display_id INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER item1_display_id;
ALTER TABLE creature_proto ADD COLUMN item3_display_id INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER item2_display_id;

UPDATE world_db_version SET LastUpdate = '20240204-0144_creature_proto';
