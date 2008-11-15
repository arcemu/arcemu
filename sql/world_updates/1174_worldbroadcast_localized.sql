-- --------------------------------------------------------

--
-- Table structure for table `worldbroadcast_localized`
--

CREATE TABLE IF NOT EXISTS `worldbroadcast_localized` (
  `entry` int(11) NOT NULL,
  `language_code` varchar(5) CHARACTER SET latin1 NOT NULL,
  `text` varchar(255) COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
