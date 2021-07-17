
English | [中文](./README-CH.md)

# Introductions
Snowflake is an  distributed unique id generator that generates roughly ordered 64bit integer IDs.

The algorithm generates a time based ID that gains its entropy from the region id, worker id, and sequence bits. Region id and worker id must be a unique combination for each snowflaked process that is generating IDs for a single ID space. The sequence number is reset to 0 at the beginning of each millisecond and the first ID generated by a snowflaked daemon will use 0 as its sequence and then increment the sequence number for the next request within that millisecond. If clocks on all servers running snowflaked process are in sync, the IDs are ordered down to the millisecond, but IDs generated within a single millisecond are not ordered. In practice, due to clock skew, the IDs generated could be unordered at a higher granularity. NTP clock adjustments are handled by the daemon to ensure that IDs are not regenerated when NTP adjusts the system clock backwards. Note: NTP should be configured and running on all hosts running snowflaked.

Parts of a snowflake ID:

sign bit: will always be 0

this is because some languages don't support unsigned integers properly (Java and PHP are notable cases)

next 41 bits: milliseconds since custom epoch

next 4 bits: region id

next 10 bits: worker id

lowest 12 bits: sequence number
![avatar](https://img-blog.csdnimg.cn/20191009093154467.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ZseTkxMDkwNQ==,size_16,color_FFFFFF,t_70)


### Benchmark test：
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
3、Benchmark code：[More examples](https://github.com/Rock-liyi/SeasSnowflake/blob/master/tests/benchmark.phpt)


### How to use it：

#### Building

    phpize
    ./configure
    make
    sudo make install

Once built and installed, be sure to add 'seassnowflake.so' to the php.ini extensions configuration.

    extension=seassnowflake.so


```php  
$config=["worker_id"=>1,"datacenter_id"=>2];
$client = new SeasSnowflake($config);
$id= $client->generate();
var_dump("id is ====",$id);
$data=$client->degenerate($id);
var_dump("data is ====",$data);
```
### echo:
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

#### [More examples](https://github.com/Rock-liyi/SeasSnowflake/blob/master/tests/003.phpt)

## Support
SeasX Group 




