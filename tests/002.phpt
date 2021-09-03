--TEST--
SeasSnowflake testCreateId
--SKIPIF--
<?php if (!extension_loaded("SeasSnowflake")) print "it  is not exits "; ?>
--FILE--
<?php

clientTest();

function clientTest()
{
    $config=['worker_id'=>3,'datacenter_id'=>5];
    $client = new SeasSnowflake($config);
    $id= $client->generate();
    echo gettype($id);
}
?>
--EXPECT--
integer
