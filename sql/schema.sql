-- phpMyAdmin SQL Dump
-- version 3.1.3.1
-- http://www.phpmyadmin.net
--
-- Anamakine: localhost
-- Üretim Zamanı: 31 Mart 2010 saat 00:38:42
-- Sunucu sürümü: 5.1.33
-- PHP Sürümü: 5.2.9

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Veritabanı: `findik`
--

-- --------------------------------------------------------

--
-- Tablo yapısı: `acl_filter_param`
--

CREATE TABLE IF NOT EXISTS `acl_filter_param` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `rule_id` int(11) NOT NULL,
  `filter_key` varchar(32) NOT NULL,
  `param` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `rule_id` (`rule_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Tablo döküm verisi `acl_filter_param`
--


-- --------------------------------------------------------

--
-- Tablo yapısı: `acl_ip_cross`
--

CREATE TABLE IF NOT EXISTS `acl_ip_cross` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `rule_id` int(10) unsigned NOT NULL,
  `acl_id` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=3 ;

--
-- Tablo döküm verisi `acl_ip_cross`
--


-- --------------------------------------------------------

--
-- Tablo yapısı: `acl_rule`
--

CREATE TABLE IF NOT EXISTS `acl_rule` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `rank` bigint(10) unsigned NOT NULL,
  `deny` tinyint(1) NOT NULL DEFAULT '0',
  `name` varchar(128) DEFAULT NULL,
  `desc` text,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Tablo döküm verisi `acl_rule`
--


-- --------------------------------------------------------

--
-- Tablo yapısı: `acl_time_cross`
--

CREATE TABLE IF NOT EXISTS `acl_time_cross` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `rule_id` int(10) unsigned NOT NULL,
  `acl_id` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=6 ;

--
-- Tablo döküm verisi `acl_time_cross`
--


-- --------------------------------------------------------

--
-- Tablo yapısı: `blacklist_category`
--

CREATE TABLE IF NOT EXISTS `blacklist_category` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `catid` int(10) unsigned NOT NULL,
  `group_id` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Tablo döküm verisi `blacklist_category`
--


-- --------------------------------------------------------

--
-- Tablo yapısı: `btk_blacklist`
--

CREATE TABLE IF NOT EXISTS `btk_blacklist` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `domain` varchar(128) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `domain` (`domain`) USING HASH
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=4 ;

--
-- Tablo döküm verisi `btk_blacklist`
--

-- --------------------------------------------------------

--
-- Tablo yapısı: `btk_db_update`
--

CREATE TABLE IF NOT EXISTS `btk_db_update` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `update_id` int(10) NOT NULL,
  `update_date` bigint(20) NOT NULL,
  `new_record_count` int(10) unsigned NOT NULL,
  `deleted_record_count` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=3 ;

--
-- Tablo döküm verisi `btk_db_update`
--

-- --------------------------------------------------------

--
-- Tablo yapısı: `btk_service_status`
--

CREATE TABLE IF NOT EXISTS `btk_service_status` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `start_date` bigint(20) NOT NULL,
  `end_date` bigint(20) NOT NULL,
  `status` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=10 ;

--
-- Tablo döküm verisi `btk_service_status`
--

-- --------------------------------------------------------

--
-- Tablo yapısı: `btk_timestamp_history`
--

CREATE TABLE IF NOT EXISTS `btk_timestamp_history` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(128) NOT NULL,
  `log_size` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=2 ;

--
-- Tablo döküm verisi `btk_timestamp_history`
--


-- --------------------------------------------------------

--
-- Tablo yapısı: `btk_user_info`
--

CREATE TABLE IF NOT EXISTS `btk_user_info` (
  `ts_username` varchar(128) NOT NULL,
  `ts_password` varchar(128) NOT NULL,
  `reg_username` varchar(128) NOT NULL,
  `reg_password` varchar(128) NOT NULL,
  `update_url` varchar(128) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Tablo döküm verisi `btk_user_info`
--

INSERT INTO `btk_user_info` (`ts_username`, `ts_password`, `reg_username`, `reg_password`, `update_url`) VALUES
('tsuser', 'tspass', 'admin', 'findik', 'https://btkupdate.medratech.com/findik-btk/btk-updater.php');

-- --------------------------------------------------------

--
-- Tablo yapısı: `btk_warning_page`
--

CREATE TABLE IF NOT EXISTS `btk_warning_page` (
  `reply_html` text NOT NULL,
  `time` bigint(20) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Tablo döküm verisi `btk_warning_page`
--

-- --------------------------------------------------------

--
-- Tablo yapısı: `category`
--

CREATE TABLE IF NOT EXISTS `category` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(128) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `catname` (`name`) USING HASH
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Tetiklemeler `category`
--
DROP TRIGGER IF EXISTS `findik`.`categoryinsref`;
DELIMITER //
CREATE TRIGGER `findik`.`categoryinsref` AFTER INSERT ON `findik`.`category`
 FOR EACH ROW BEGIN
    INSERT INTO category_count VALUES (NEW.id,NEW.name,'0','0','0');
  END
//
DELIMITER ;
DROP TRIGGER IF EXISTS `findik`.`categorydelref`;
DELIMITER //
CREATE TRIGGER `findik`.`categorydelref` AFTER DELETE ON `findik`.`category`
 FOR EACH ROW BEGIN
    DELETE FROM category_count WHERE name = OLD.name;
    DELETE FROM domain WHERE catid = OLD.id;   
    DELETE FROM url WHERE catid = OLD.id;   
    DELETE FROM content WHERE catid = OLD.id;   
  END
//
DELIMITER ;

--
-- Tablo döküm verisi `category`
--


-- --------------------------------------------------------

--
-- Tablo yapısı: `category_count`
--

CREATE TABLE IF NOT EXISTS `category_count` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(128) NOT NULL,
  `domain_count` int(10) unsigned NOT NULL,
  `url_count` int(10) unsigned NOT NULL,
  `content_count` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Tablo döküm verisi `category_count`
--


-- --------------------------------------------------------

--
-- Tablo yapısı: `content`
--

CREATE TABLE IF NOT EXISTS `content` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `content` text NOT NULL,
  `catid` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=9 ;

--
-- Tetiklemeler `content`
--
DROP TRIGGER IF EXISTS `findik`.`contentinsref`;
DELIMITER //
CREATE TRIGGER `findik`.`contentinsref` AFTER INSERT ON `findik`.`content`
 FOR EACH ROW BEGIN
    UPDATE category_count SET content_count = content_count + 1 WHERE id = NEW.catid;
  END
//
DELIMITER ;
DROP TRIGGER IF EXISTS `findik`.`contentdelref`;
DELIMITER //
CREATE TRIGGER `findik`.`contentdelref` AFTER DELETE ON `findik`.`content`
 FOR EACH ROW BEGIN
    UPDATE category_count SET content_count = content_count - 1 WHERE id = OLD.catid;
  END
//
DELIMITER ;

--
-- Tablo döküm verisi `content`
--


-- --------------------------------------------------------

--
-- Tablo yapısı: `domain`
--

CREATE TABLE IF NOT EXISTS `domain` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `domain` varchar(128) NOT NULL,
  `catid` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `domain` (`domain`) USING HASH
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Tetiklemeler `domain`
--
DROP TRIGGER IF EXISTS `findik`.`domaininsref`;
DELIMITER //
CREATE TRIGGER `findik`.`domaininsref` AFTER INSERT ON `findik`.`domain`
 FOR EACH ROW BEGIN
    UPDATE category_count SET domain_count = domain_count + 1 WHERE id = NEW.catid;
  END
//
DELIMITER ;
DROP TRIGGER IF EXISTS `findik`.`domaindelref`;
DELIMITER //
CREATE TRIGGER `findik`.`domaindelref` AFTER DELETE ON `findik`.`domain`
 FOR EACH ROW BEGIN
    UPDATE category_count SET domain_count = domain_count - 1 WHERE id = OLD.catid;
  END
//
DELIMITER ;

--
-- Tablo döküm verisi `domain`
--


-- --------------------------------------------------------

--
-- Tablo yapısı: `ip_table`
--

CREATE TABLE IF NOT EXISTS `ip_table` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(128) NOT NULL,
  `local_ip` bigint(20) NOT NULL,
  `local_mask` bigint(20) NOT NULL,
  `local_masked_ip` bigint(20) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=5 ;

--
-- Tablo döküm verisi `ip_table`
--


-- --------------------------------------------------------

--
-- Tablo yapısı: `mime_type`
--

CREATE TABLE IF NOT EXISTS `mime_type` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `file_ext` varchar(16) NOT NULL,
  `mime_type` varchar(128) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=644 ;

--
-- Tablo döküm verisi `mime_type`
--


-- --------------------------------------------------------

--
-- Tablo yapısı: `mime_type_cross`
--

CREATE TABLE IF NOT EXISTS `mime_type_cross` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `mime_group_id` int(10) unsigned NOT NULL,
  `mime_type_id` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=69 ;

--
-- Tablo döküm verisi `mime_type_cross`
--


-- --------------------------------------------------------

--
-- Tablo yapısı: `mime_type_group`
--

CREATE TABLE IF NOT EXISTS `mime_type_group` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(128) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=7 ;

--
-- Tablo döküm verisi `mime_type_group`
--


-- --------------------------------------------------------

--
-- Tablo yapısı: `time_table`
--

CREATE TABLE IF NOT EXISTS `time_table` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(128) NOT NULL,
  `start_time` time NOT NULL,
  `end_time` time NOT NULL,
  `day_of_week` tinyint(4) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=4 ;

--
-- Tablo döküm verisi `time_table`
--


-- --------------------------------------------------------

--
-- Tablo yapısı: `url`
--

CREATE TABLE IF NOT EXISTS `url` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `url` varchar(512) NOT NULL,
  `catid` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `url` (`url`) USING HASH
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=661056 ;

--
-- Tetiklemeler `url`
--
DROP TRIGGER IF EXISTS `findik`.`urlinsref`;
DELIMITER //
CREATE TRIGGER `findik`.`urlinsref` AFTER INSERT ON `findik`.`url`
 FOR EACH ROW BEGIN
    UPDATE category_count SET url_count = url_count + 1 WHERE id = NEW.catid;
  END
//
DELIMITER ;
DROP TRIGGER IF EXISTS `findik`.`urldelref`;
DELIMITER //
CREATE TRIGGER `findik`.`urldelref` AFTER DELETE ON `findik`.`url`
 FOR EACH ROW BEGIN
    UPDATE category_count SET url_count = url_count - 1 WHERE id = OLD.catid;
  END
//
DELIMITER ;

--
-- Tablo döküm verisi `url`
--


-- --------------------------------------------------------

--
-- Tablo yapısı: `users`
--

CREATE TABLE IF NOT EXISTS `users` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(128) NOT NULL,
  `password` varchar(64) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=5 ;

--
-- Tablo döküm verisi `users`
--

INSERT INTO `users` (`id`, `username`, `password`) VALUES
(4, 'admin', '4ff591af51b48e9ca0a7c7dba39cbbd823e39b63');
