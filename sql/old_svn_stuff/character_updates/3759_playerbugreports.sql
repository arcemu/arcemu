CREATE TABLE `playerbugreports` (
  `UID` int(10) unsigned NOT NULL,
  `AccountID` int(10) unsigned NOT NULL,
  `TimeStamp` int(10) unsigned NOT NULL,
  `Suggestion` int(10) unsigned NOT NULL,
  `Type` text NOT NULL,
  `Content` text NOT NULL,
  PRIMARY KEY (`UID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
