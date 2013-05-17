-- Fakoli Login Component Schema
-- 
-- Each version update must begin with the following:
-- START Version xx
-- and end with
-- END Version xx


-- START Version 1.0

CREATE TABLE `login_token` (
  `token_id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `token` varchar(10) NOT NULL,
  `expire_date` date NOT NULL,
  `user_id` int(10) unsigned NOT NULL,
  PRIMARY KEY (`token_id`),
  UNIQUE KEY `token_idx` (`token`)
) ENGINE=InnoDB;

-- END Version 1.0

-- START Version 1.1

CREATE TABLE `access_log` (
  `access_id` INTEGER UNSIGNED NOT NULL PRIMARY KEY AUTO_INCREMENT,
  `timestamp` DATETIME NOT NULL,
  `action` VARCHAR(100) NOT NULL,
  `username` VARCHAR(100),
  `password` VARCHAR(100),
  `ip` VARCHAR(100)
) ENGINE = InnoDB;

-- END Version 1.1
