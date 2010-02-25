-- MySQL dump 10.11
--
-- Host: localhost    Database: findik
-- ------------------------------------------------------
-- Server version	5.0.45

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `acl_filter_param`
--

CREATE TABLE `acl_filter_param` (
  `id` int(11) NOT NULL auto_increment,
  `rule_id` int(11) NOT NULL,
  `filter_key` varchar(32) NOT NULL,
  `param` int(11) unsigned NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `rule_id` (`rule_id`)
) ENGINE=MyISAM AUTO_INCREMENT=7 DEFAULT CHARSET=latin1;

--
-- Table structure for table `acl_ip_cross`
--

CREATE TABLE `acl_ip_cross` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `rule_id` int(10) unsigned NOT NULL,
  `acl_id` int(10) unsigned NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=13 DEFAULT CHARSET=latin1;

--
-- Table structure for table `acl_rule`
--

CREATE TABLE `acl_rule` (
  `id` int(11) NOT NULL auto_increment,
  `rank` bigint(10) unsigned NOT NULL,
  `deny` tinyint(1) NOT NULL default '0',
  `name` varchar(128) default NULL,
  `desc` text,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

--
-- Table structure for table `acl_time_cross`
--

CREATE TABLE `acl_time_cross` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `rule_id` int(10) unsigned NOT NULL,
  `acl_id` int(10) unsigned NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=10 DEFAULT CHARSET=latin1;

--
-- Table structure for table `blacklist_category`
--

CREATE TABLE `blacklist_category` (
  `id` bigint(20) unsigned NOT NULL auto_increment,
  `catid` int(10) unsigned NOT NULL,
  `group_id` int(10) unsigned NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

--
-- Table structure for table `category`
--

CREATE TABLE `category` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `name` varchar(128) NOT NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `catname` USING HASH (`name`)
) ENGINE=MyISAM AUTO_INCREMENT=85 DEFAULT CHARSET=latin1;

--
-- Table structure for table `content`
--

CREATE TABLE `content` (
  `id` bigint(20) unsigned NOT NULL auto_increment,
  `content` text NOT NULL,
  `catid` int(10) unsigned NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=9 DEFAULT CHARSET=utf8;

--
-- Table structure for table `domain`
--

CREATE TABLE `domain` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `domain` varchar(128) NOT NULL,
  `catid` int(10) unsigned NOT NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `domain` USING HASH (`domain`)
) ENGINE=MyISAM AUTO_INCREMENT=1687879 DEFAULT CHARSET=latin1;

--
-- Table structure for table `ip_table`
--

CREATE TABLE `ip_table` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `name` varchar(128) NOT NULL,
  `local_ip` bigint(20) NOT NULL,
  `local_mask` bigint(20) NOT NULL,
  `local_masked_ip` bigint(20) NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=7 DEFAULT CHARSET=latin1;

--
-- Table structure for table `mime_type`
--

CREATE TABLE `mime_type` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `file_ext` varchar(16) NOT NULL,
  `mime_type` varchar(128) NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=645 DEFAULT CHARSET=utf8;

--
-- Table structure for table `mime_type_cross`
--

CREATE TABLE `mime_type_cross` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `mime_group_id` int(10) unsigned NOT NULL,
  `mime_type_id` int(10) unsigned NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=37 DEFAULT CHARSET=latin1;

--
-- Table structure for table `mime_type_group`
--

CREATE TABLE `mime_type_group` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `name` varchar(128) NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;

--
-- Table structure for table `time_table`
--

CREATE TABLE `time_table` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `name` varchar(128) NOT NULL,
  `start_time` time NOT NULL,
  `end_time` time NOT NULL,
  `day_of_week` tinyint(4) unsigned NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=16 DEFAULT CHARSET=latin1;

--
-- Table structure for table `url`
--

CREATE TABLE `url` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `url` varchar(512) NOT NULL,
  `catid` int(10) unsigned NOT NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `url` USING HASH (`url`)
) ENGINE=MyISAM AUTO_INCREMENT=661040 DEFAULT CHARSET=latin1;

--
-- Table structure for table `users`
--

CREATE TABLE `users` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `username` varchar(128) NOT NULL,
  `password` varchar(64) NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;

INSERT INTO `users` values('', 'admin', sha1('findik'));


