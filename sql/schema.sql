CREATE TABLE IF NOT EXISTS `acl_filter_param` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `rule_id` int(11) NOT NULL,
  `filter_key` varchar(32) NOT NULL,
  `param` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `rule_id` (`rule_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

CREATE TABLE IF NOT EXISTS `acl_ip_cross` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `rule_id` int(10) unsigned NOT NULL,
  `acl_id` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=3 ;

CREATE TABLE IF NOT EXISTS `acl_rule` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `rank` bigint(10) unsigned NOT NULL,
  `deny` tinyint(1) NOT NULL DEFAULT '0',
  `name` varchar(128) DEFAULT NULL,
  `desc` text,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

CREATE TABLE IF NOT EXISTS `acl_time_cross` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `rule_id` int(10) unsigned NOT NULL,
  `acl_id` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=6 ;

CREATE TABLE IF NOT EXISTS `blacklist_category` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `catid` int(10) unsigned NOT NULL,
  `group_id` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

CREATE TABLE IF NOT EXISTS `btk_db_update` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `update_id` varchar(128) NOT NULL,
  `update_date` datetime NOT NULL,
  `new_record_count` int(10) unsigned NOT NULL,
  `deleted_record_count` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=2 ;

CREATE TABLE IF NOT EXISTS `btk_service_status` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `start_date` bigint NOT NULL,
  `end_date` bigint NOT NULL,
  `status` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=10 ;

CREATE TABLE IF NOT EXISTS `btk_timestamp_history` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `start_date` bigint NOT NULL,
  `end_date` bigint NOT NULL,
  `log_size` int(10) unsigned NOT NULL,
  `status` smallint(6) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=2 ;

CREATE TABLE IF NOT EXISTS `btk_user_info` (
  `username` varchar(128) NOT NULL,
  `password` varchar(128) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

INSERT INTO `btk_user_info` (`username`, `password`) VALUES
('admin', 'findik');

CREATE TABLE IF NOT EXISTS `category` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(128) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `catname` (`name`) USING HASH
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

DROP TRIGGER IF EXISTS `categoryinsref`;
DELIMITER //
CREATE TRIGGER `categoryinsref` AFTER INSERT ON `category`
 FOR EACH ROW BEGIN
    INSERT INTO category_count VALUES (NEW.id,NEW.name,'0','0','0');
  END
//
DELIMITER ;
DROP TRIGGER IF EXISTS `categorydelref`;
DELIMITER //
CREATE TRIGGER `categorydelref` AFTER DELETE ON `category`
 FOR EACH ROW BEGIN
    DELETE FROM category_count WHERE name = OLD.name;
    DELETE FROM domain WHERE catid = OLD.id;   
    DELETE FROM url WHERE catid = OLD.id;   
    DELETE FROM content WHERE catid = OLD.id;   
  END
//
DELIMITER ;

CREATE TABLE IF NOT EXISTS `category_count` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(128) NOT NULL,
  `domain_count` int(10) unsigned NOT NULL,
  `url_count` int(10) unsigned NOT NULL,
  `content_count` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

CREATE TABLE IF NOT EXISTS `content` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `content` text NOT NULL,
  `catid` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=9 ;

DROP TRIGGER IF EXISTS `contentinsref`;
DELIMITER //
CREATE TRIGGER `contentinsref` AFTER INSERT ON `content`
 FOR EACH ROW BEGIN
    UPDATE category_count SET content_count = content_count + 1 WHERE id = NEW.catid;
  END
//
DELIMITER ;
DROP TRIGGER IF EXISTS `contentdelref`;
DELIMITER //
CREATE TRIGGER `contentdelref` AFTER DELETE ON `content`
 FOR EACH ROW BEGIN
    UPDATE category_count SET content_count = content_count - 1 WHERE id = OLD.catid;
  END
//
DELIMITER ;

CREATE TABLE IF NOT EXISTS `domain` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `domain` varchar(128) NOT NULL,
  `catid` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `domain` (`domain`) USING HASH
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

DROP TRIGGER IF EXISTS `domaininsref`;
DELIMITER //
CREATE TRIGGER `domaininsref` AFTER INSERT ON `domain`
 FOR EACH ROW BEGIN
    UPDATE category_count SET domain_count = domain_count + 1 WHERE id = NEW.catid;
  END
//
DELIMITER ;
DROP TRIGGER IF EXISTS `domaindelref`;
DELIMITER //
CREATE TRIGGER `domaindelref` AFTER DELETE ON `domain`
 FOR EACH ROW BEGIN
    UPDATE category_count SET domain_count = domain_count - 1 WHERE id = OLD.catid;
  END
//
DELIMITER ;

CREATE TABLE IF NOT EXISTS `ip_table` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(128) NOT NULL,
  `local_ip` bigint(20) NOT NULL,
  `local_mask` bigint(20) NOT NULL,
  `local_masked_ip` bigint(20) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=5 ;

CREATE TABLE IF NOT EXISTS `mime_type` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `file_ext` varchar(16) NOT NULL,
  `mime_type` varchar(128) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=644 ;

CREATE TABLE IF NOT EXISTS `mime_type_cross` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `mime_group_id` int(10) unsigned NOT NULL,
  `mime_type_id` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=69 ;

CREATE TABLE IF NOT EXISTS `mime_type_group` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(128) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=7 ;

CREATE TABLE IF NOT EXISTS `time_table` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(128) NOT NULL,
  `start_time` time NOT NULL,
  `end_time` time NOT NULL,
  `day_of_week` tinyint(4) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=4 ;

CREATE TABLE IF NOT EXISTS `url` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `url` varchar(512) NOT NULL,
  `catid` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `url` (`url`) USING HASH
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=661056 ;

DROP TRIGGER IF EXISTS `urlinsref`;
DELIMITER //
CREATE TRIGGER `urlinsref` AFTER INSERT ON `url`
 FOR EACH ROW BEGIN
    UPDATE category_count SET url_count = url_count + 1 WHERE id = NEW.catid;
  END
//
DELIMITER ;
DROP TRIGGER IF EXISTS `urldelref`;
DELIMITER //
CREATE TRIGGER `urldelref` AFTER DELETE ON `url`
 FOR EACH ROW BEGIN
    UPDATE category_count SET url_count = url_count - 1 WHERE id = OLD.catid;
  END
//
DELIMITER ;

CREATE TABLE IF NOT EXISTS `users` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(128) NOT NULL,
  `password` varchar(64) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=5 ;

INSERT INTO `users` (`username`, `password`) VALUES
('admin', '4ff591af51b48e9ca0a7c7dba39cbbd823e39b63');
