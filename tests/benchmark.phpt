--TEST--
SeasSnowflake testBenchmark
--FILE--
<?php

/*
$config=['worker_id'=>10,'datacenter_id'=>1];
$client = new SeasSnowflake($config);

echo "\nmake 100000 id=================\n";
$start = microtime(true);
for($i=0;$i<100000;$i++){
    $id= $client->generate();
}
$end = (float)microtime(true);
echo 'cost:'.round($end-$start,6).' second ';

echo ' and memory usage: ' . memory_get_usage() . 'byte';



echo "\nmake 200000 id=================\n";
$start = microtime(true);
for($i=0;$i<200000;$i++){
    $id= $client->generate();
}
$end = (float)microtime(true);
echo 'cost:'.round($end-$start,6).' second ';

echo ' and memory usage: ' . memory_get_usage() . 'byte';


echo "\nmake 300000 id=================\n";
$start = microtime(true);
for($i=0;$i<300000;$i++){
    $id= $client->generate();
}
$end = (float)microtime(true);
echo 'cost:'.round($end-$start,6).' second ';

echo ' and memory usage: ' . memory_get_usage() . 'byte';



echo "\nmake 500000 id=================\n";
$start = microtime(true);
for($i=0;$i<300000;$i++){
    $id= $client->generate();
}
$end = (float)microtime(true);
echo 'cost:'.round($end-$start,6).' second ';

echo ' and memory usage: ' . memory_get_usage() . 'byte';
*/

echo 1;

?>

--EXPECT--
1