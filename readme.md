# Database System

曹伦郗 *2020011020*

#### 内容说明

各模块代码存放于各自文件目录下，defines.h和defines.cpp中定义了各种结构体，如文件头、页头、数据库和表的META信息等。主程序入口为main.cpp。

#### 使用说明

若未指定所读取的.sql文件，程序从输入读取命令，例如

```
./main
```

若指定了.sql文件，则从文件中读取命令，例如

```
./main ./create.sql
```

#### 功能

- 支持的命令

  - 退出命令：

    新增了指令`QUIT ;`，用于结束主程序；

  - 数据库相关命令：

    创建数据库、删除数据库、展示系统内所有数据库名、使用某数据库，展示数据库内所有表名。

    新增了指令`SHOW USE`，用于展示当前正在使用的数据库；

  - 表相关命令：

    创建表、删除表、描述某表、插入若干条记录、删（改、查、连接）符合条件的记录。

    新增了指令`SAFE INSERT INTO identifier VALUES values_lists ;`，用于将大规模的、符合约束的安全记录插入表中，避免反复调用插入命令带来的反复开关记录和索引文件的开销。

  - 修改相关命令：

    新建索引、删除索引、新建主键、删除主键、新建外键、删除外键。

    在这里，我采用了主键自动创建的索引复用该列上之前手动创建的索引的做法；

- 数据类型

  支持INT，FLOAT，VARCHAR三种数据类型，VARCHAR被实现为定长。