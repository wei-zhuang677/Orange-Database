# Orange数据库

## 1.简介
这是一个由c++编写的键值对数据库,支持字符串、列表、哈希表、集合四种数据结构,用MVCC支持并行


## 2.设计框架
服务端与客户端通过socket通信（详见Orange通讯协议），服务端**server.cpp**负责传入数据的命令提取与事务的封装，  
**COND.cpp**负责命令的解析与运行，**DateTable.cpp**是数据库的主体，数据库中所有的键为**SDS.cpp**实现的SDS类，  
值均为**Value.cpp**中实现的Value类，由Value实例中不同的对象的指针实现不同数据结构的存储，**SDS.cpp**实现了字符串，  
**HashTable.cpp**实现了哈希表和集合，**List.cpp**实现了列表  

## 操作与命令
1. delete(key):删除数据库中键为 key 的 value
2. 字符串
   - set(key, value):给数据库中键为 key 的属性赋予值 value（字符串）
   - get(key):返回数据库中键为 key 的 value（字符串）
3. 列表：简单的字符串列表，按照插入顺序排序。可以在头部或尾部添加元素
   - addr(key, value): 给数据库中键为key的列表右侧插入值value
   - addl(key, value): 给数据库中键为key的列表左侧插入值value
   - lindex(key, index): 获取数据库中键为key的列表中索引为index的值（索引从0开始）
   - lrange(key, start, stop): 获取数据库中键为key的列表中指定范围内的元素
   - popr(key): 数据库中键为key的列表删除最右侧的一个值
   - popl(key): 数据库中键为key的列表删除最左侧的一个值
4. 哈希：键和值的映射
   - hset(key, field, value): 数据库中键为key的哈希表添加一个键值对
   - hget(key, field): 获取数据库中键为key的哈希表中键为field的值
5. 集合：无序的字符串集合，所有元素都是唯一的。
   - sadd(key, value): 数据库中键为key的集合添加一个值。
   - smembers(key): 获取数据库中键为key的集合的所有值。
   - srem(key, value): 数据库中键为key的集合删除一个值。
6. 开始事务
   - 对应指令：begin
   - 注：如果不先使用begin直接使用命令，将会导致操作无效。服务端返回：“Illegal Input”。
7. 命令入队
   - 对应指令：无。在begin后执行，commit提交之前的所有命令都是事务中的命令。
8. 事务执行并提交
   - 对应指令：commit
9. 事务取消
   - 对应指令：reset
10. 持久化    
    - 手动触发：使用SAVE或者RGSAVE命令。SAVE会阻塞服务端，直接生成ODB保存文件，而RGSAVE则会生成一个子线程去创建快照，不会阻塞服务端。
    - 自动触发：通过配置服务端的save参数实现，例如save(a,b)指的是：每隔a秒，如果有大于等于b个键被创建或者被更改，则进行一次ODB保存。

