# ChatServer
基于muduo库，redis，使用nginx tcp负载均衡的简易集群聊天服务器和客户端源码 

1、需要安装muduo库

2、配置nginx的tcp负载均衡：在conf目录里配置nginx.conf文件：
  stream {
    upstream MyServer {
      server <ip>:<port> weight = 1 max fails = 3 fail_timeout = 30s;
      ...
  
    }
    server {
      proxy_connect_timeout 1s;
      proxy_timeout 3s;
      listen 8000; #服务的端口
      proxy_pass MyServer;
      tcp_nodelay on;
    }
  }
  
3、需要安装服务器中间间redis使用其发布订阅功能
  
4、使用mysql创建以下表来存储需要记录的信息：
  User表：
  字段名称  字段类型                      字段说明        约束
  id        INT                           用户id        PRIMARY KEY、AUTO_INCREMENT
  name      VARCHAR(50)                   用户名        NOT NULL, UNIQUE
  password  VARCHAR(50)                   用户密码      NOT NULL
  state     ENUM('online', 'offline')     当前登录状态   DEFAULT 'offline'
  
  Friend表：
  userid INT 用户id NOT NULL、联合主键
  friendid INT 好友id NOT NULL、联合主键
  
  AllGroup表：（用于群聊功能）
  id INT 组id PRIMARY KEY、AUTO_INCREMENT
  groupname VARCHAR(50) 组名称 NOT NULL,UNIQUE
  groupdesc VARCHAR(200) 组功能描述 DEFAULT ''
  
  GruopUser表：（用于群聊功能）
  groupid INT 组id NOT NULL、联合主键
  userid INT 组员id NOT NULL、联合主键
  grouprole ENUM('creator', 'normal') 组内角色 DEFAULT ‘normal’
  
  OfficeMessage表：（离线消息）
  userid INT 用户id NOT NULL
  message VARCHAR(500) 离线消息（存储Json字符串） NOT NULL
