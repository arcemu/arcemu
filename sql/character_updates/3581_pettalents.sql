DROP TABLE IF EXISTS pet_talents;
CREATE TABLE pet_talents 
(
	pet_owner INT(11) UNSIGNED NOT NULL,
	pet_family TINYINT(3) NOT NULL,
	talent_array VARCHAR(255)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;