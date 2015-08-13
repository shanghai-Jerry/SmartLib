DROP TABLE IF EXISTS admin;
CREATE TABLE `admin` (`rid` varchar(15) NOT NULL DEFAULT '',`pwd` varchar(15) DEFAULT NULL,`name` varchar(15) DEFAULT NULL,`sex` varchar(15) DEFAULT NULL,`birth` varchar(15) DEFAULT NULL,`major` varchar(15) DEFAULT NULL,`rtype` varchar(20) DEFAULT NULL,`status` varchar(15) DEFAULT NULL,`images` longblob,`qrcode` mediumtext,`login` varchar(255) NOT NULL DEFAULT '0',PRIMARY KEY (`rid`));



DROP TABLE IF EXISTS book;
CREATE TABLE `book` (`bookid` varchar(15) NOT NULL DEFAULT '',`isbn` varchar(255) DEFAULT NULL,`bookname` varchar(15) DEFAULT NULL,`author` varchar(15) DEFAULT NULL,`publish` varchar(15) DEFAULT NULL,`pubtime` varchar(15) DEFAULT NULL,`price` int(15) DEFAULT NULL,`storenum` int(15) DEFAULT NULL,`introduction` mediumtext,`qrcode` mediumtext,`id` int(11) NOT NULL AUTO_INCREMENT,PRIMARY KEY (`id`));


DROP TABLE IF EXISTS borrow;
CREATE TABLE `borrow` (`rid` varchar(15) NOT NULL DEFAULT '',`bookid` varchar(15) NOT NULL DEFAULT '',`bookname` varchar(15) DEFAULT NULL,`publish` varchar(15) DEFAULT NULL,`borrowtime` varchar(15) DEFAULT NULL,`returntime` varchar(255) DEFAULT NULL, `times` varchar(15) DEFAULT '0',  `dealwith` varchar(30) DEFAULT NULL,  `status` varchar(15) DEFAULT NULL,  PRIMARY KEY (`bookid`,`rid`)) ;




DROP TABLE IF EXISTS reader;
CREATE TABLE `reader` (  `rid` varchar(15) NOT NULL DEFAULT '',  `pwd` varchar(15) DEFAULT NULL,  `name` varchar(15) DEFAULT NULL,  `sex` varchar(15) DEFAULT NULL,  `birth` varchar(15) DEFAULT NULL,`major` varchar(15) DEFAULT NULL, `rtype` varchar(20) DEFAULT NULL,  `status` varchar(15) DEFAULT NULL,  `images` longblob,  `qrcode` text,  `login` varchar(255) NOT NULL DEFAULT '0', PRIMARY KEY (`rid`));



DROP TABLE IF EXISTS readertype;
CREATE TABLE `readertype` (  `rtype` varchar(15) NOT NULL,  `borrlimit` varchar(15) DEFAULT NULL,  `conlimit` varchar(15) DEFAULT NULL,  PRIMARY KEY (`rtype`)) ;





