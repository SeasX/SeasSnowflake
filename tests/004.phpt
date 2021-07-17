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
    var_dump("id is ====",$id);
    $data=$client->degenerate($id);
    var_dump("data is ====",$data);
    echo  gettype($data);
}
?>
--EXPECT--
array
