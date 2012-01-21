CREATE TABLE `display_bounding_boxes` (
  `displayid` int(30) NOT NULL,
  `lowx` float NOT NULL,
  `lowy` float NOT NULL,
  `lowz` float NOT NULL,
  `highx` float NOT NULL,
  `highy` float NOT NULL,
  `highz` float NOT NULL,
  PRIMARY KEY (`displayid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;