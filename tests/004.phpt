--TEST--
SeasSnowflake testCreateId
--SKIPIF--
<?php if (!extension_loaded("SeasSnowflake")) print "it  is not exits "; ?>
--FILE--
<?php

clientTest();

function clientTest()
{
    $workerId=10;
    $datacenterId=20;
    $client = new SeasSnowflake(['worker_id'=>$workerId,'datacenter_id'=>$datacenterId]);
    $id= $client->generate();
    $data=$client->degenerate($id);
    echo  gettype($data);
}
?>
--EXPECT--
array
