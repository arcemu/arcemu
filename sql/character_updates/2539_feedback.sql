
CREATE TABLE `feedback` (
  `arbitrary_index` int(11) NOT NULL auto_increment,
  `feedback_type` varchar(10) default NULL,
  `user_content` text,
  `state_data` text,
  PRIMARY KEY  (`arbitrary_index`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
