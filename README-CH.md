
中文 | [English](./README.md)

# 简介
Snowflake id 是一个分布式雪花生成器，由 Twitter在2010 年提出， 可保证在分布式场景下生成唯一的 ID 号。其优点是：高性能，低延迟；按时间有序，特别适合需要进行分表的业务场景，可通过degenerate方法 反解出生成的时间

SeasSnowflake 是Snowflake 的变种算法,支持20位的雪花id 生成,解决Snowflake出现的时钟回退,分布式场景下 k8s docker 部署下的节点飘逸、docker重启、IP动态变化导致的ID生成重复等问题，支持PHP7 PHP8 版本


### 算法实现：
41位的时间序列（精确到毫秒，41位的长度可以使用69年）；

10位的机器标识（10位的长度单体版本最多支持部署1024个节点,过滤器版本没有此限制）；

12位的计数顺序号（12位的计数顺序号支持每个节点每毫秒产生4096个ID序号）最高位是符号位，始终为0。



![avatar](https://img-blog.csdnimg.cn/20191009093154467.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ZseTkxMDkwNQ==,size_16,color_FFFFFF,t_70)

41-bit的时间可以表示（1L<<41）/(1000L*3600*24*365)=69年的时间，10-bit机器可以分别表示1024台机器。如果我们对IDC划分有需求，还可以将10-bit分5-bit给IDC，分5-bit给工作机器。这样就可以表示32个IDC，每个IDC下可以有32台机器，可以根据自身需求定义。12个自增序列号可以表示2^12个ID，理论上snowflake方案的QPS约为409.6w/s，这种分配方式可以保证在任何一个IDC的任何一台机器在任意毫秒内生成的ID都是不同的。

原生方案优点：

毫秒数在高位，自增序列在低位，整个ID都是趋势递增的，整体上按照时间自增排序，并且整个分布式系统内不会产生ID碰撞(由数据中心ID和机器ID作区分)，并且效率较高，经测试，SnowFlake每秒能够产生26万ID左右
不依赖redis mysql 等第三方系统，稳定性更高，生成ID的性能也是非常高的。

原生方案缺点：
1、强依赖机器时钟，如果机器上时钟回拨，会导致ID重复
2、在微服务场景、或多系统场景下、如果要对多个不同服务/系统的数据做聚合，由于每个服务的ID是独立生成的，100%会出现重复


优化后的方案：
SeasSnowflake 


### 基础版本特点：
  不依赖数据库等第三方系统，以扩展的方式安装，稳定性更高，生成ID的性能也是非常高的，可保证单体架构下不会生成重复id

业务场景：替代mysql 的自增id， 业务需要进行按照id 或时间进行分表

##### 注意事项：

1、注意PHP多进程的问题、需自行传入workerid 跟dataceter_id（开启redis 过滤器可省略这一步）

2、时钟回退问题

3、多个服务聚合数据时仍然会有重复，比如：单据系统分了四个不同服务、每个服务一个数据库。 当对四个服务的数据进行聚合处理时、就会出现重复单据ID。跨系统的数据做聚合也会出现相同问题（该问题在生产环境出现过、如果有相似业务场景需要留意）

### 过滤器版本特点:
基于Redis 有序集合(sorted set) 开发的过滤器,保证同一秒中生成的id不会重复，即便时钟回退，部署超过1024个节点也不会产生重复，突破雪花算法限制，解决大规模集群如k8s docker 部署下的多节点问题

算法思路：

1、基于redis 分批生成id, 以10240为一个批号,每个批号的第一个id 大于上个批号的最后一个id

2、基于PHP扩展单个进程缓存申请到10240个id，缓存最大时间默认为5分钟，当请求过来时先从本地内存中获取，如果内存中数量不足再从redis中申请新批次id，当内存中存储id数量达到20%，会申请新的批次。

3、内置workerid 跟datacenterid 生成器，理论上低于1024个PHP进程同时运行时不开启批次生成也不会重复（默认开启，通过配置关闭后需自行传入worker_id跟datacenter_id）

业务场景：微服务、分布式场景、所有业务的id 均保证不会出现重复。

说明：为什么使用Redis 而不用mysql, 原因是作者认为雪花id 本身是按照时间递增的,除了时间回退、节点重复问题，历史已经生成的ID不会再重复生成，持久化保留ID意义并不大，如果您想用mysql来存储，欢迎给我们提PR，其它通过mysql 实现的变种算法可参考:

[美团雪花变种算法实现](https://github.com/zhuzhong/idleaf)

[百度雪花变种算法实现](https://github.com/baidu/uid-generator)


### 性能测试：
#### 单体版本性能测试
1、测试生成10万个id  只需要0.73秒  20万个id 需要1.4秒, 大于20万的id生成 消耗的时间没有明显的区别
```php
--TEST--
SeasSnowflake testBenchmark
--FILE--
<?php
//test code
$config=['worker_id'=>10,'datacenter_id'=>1];
$client = new SeasSnowflake($config);
$start = microtime(true);
for($i=0;$i<100000;$i++){
    $id= $client->generate();
}
$end = (float)microtime(true);
echo 'cost:'.round($end-$start,6).' second ';

echo ' and memory usage: ' . memory_get_usage() . 'byte';

?>

---- ACTUAL OUTPUT
cost:0.731212 second  and memory usage: 387520byte
---- FAILED
```


```php
2、分别测试 生成10 万20万、30万、50万id
---- ACTUAL OUTPUT
make 100000 id=================
cost:0.712473 second  and memory usage: 395816byte
make 200000 id=================
cost:1.401492 second  and memory usage: 395816byte
make 300000 id=================
cost:1.415514 second  and memory usage: 395816byte
make 500000 id=================
cost:1.422977 second  and memory usage: 395816byte
---- FAILED
```

3、基准测试代码：[More examples](./tests/benchmark.phpt)


### 使用方式：

#### 编译

    phpize
    ./configure
    make
    sudo make install

编译完后,安装seassnowflake.so  记得 在php.ini extensions configuration 开启扩展

    extension=seassnowflake.so


```php
$config=["worker_id"=>1,"datacenter_id"=>2];
$client = new SeasSnowflake($config);
$id= $client->generate();
var_dump("id is ====",$id);
$data=$client->degenerate($id);
var_dump("data is ====",$data);
```
### 输出结果
```php
string(10) "id is ===="
int(273387429541355520)
string(12) "data is ===="
array(6) {
  ["worker_id"]=>
  int(10)
  ["datacenter_id"]=>
  int(15)
  ["sequence"]=>
  int(0)
  ["timestamp"]=>
  int(1626140642495)
  ["begin_timestamp"]=>
  int(1560960000000)
  ["interval"]=>
  int(65180642495)
}
```

#### [More examples](./tests/003.phpt)


### 特别说明
本扩展是Twitter 的雪花ID的变种实现，算法原理来自Twitter , 在实现的过程中，参考了大量网上教程，主要参考的文章在README跟源码注释中均有说明，并在此向Twitter及雪花变种算法的作者表达敬意，特别感谢在刚接触雪花算法时、hyperf 团队李铭昕对作者的一些疑惑解答
 
开始是准备采用PHP语言来实现过滤器，但是实现的过程中发现并没有找到很好的PHP进程内存缓存ID的方式，转而采用了PHP扩展的形式，生成的ID参考了hyperf 的格式，故可跟hyperf 的雪花ID组件无缝替换。

Redis 过滤器方式的雪花变种算法为SeasX 团队原创、如本扩展为你解决了一些问题、或提供了一些解决问题的思路，也请在项目中保留本代码库的链接。

### 支持
SeasX 团队 





