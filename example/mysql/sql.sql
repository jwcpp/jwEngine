CREATE DATABASE IF NOT EXISTS `jw_test` DEFAULT CHARSET utf8mb4 COLLATE utf8mb4_general_ci;

USE `jw_test`;

CREATE TABLE IF NOT EXISTS `test` (
  `id` int(11) DEFAULT NULL,
  `num` int(11) DEFAULT NULL,
  `name` varchar(100) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- ----------------------------
-- Records of test
-- ----------------------------
INSERT INTO `test` VALUES ('2', '101', 'lisi');
INSERT INTO `test` VALUES ('1', '100', 'zhangsan');