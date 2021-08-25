--TEST--
SeasSnowflake testCreateId
--SKIPIF--
<?php if (!extension_loaded("SeasSnowflake")) print "it  is not exits "; ?>
--FILE--
<?php

clientTest();

function clientTest()
{
     $config=['worker_id'=>1,'datacenter_id'=>2];
    $client = new SeasSnowflake($config);
    $id= $client->generate();
   // var_dump("id is ====",$id);
    $data=$client->degenerate($id);
  //  var_dump("data is ====",$data);
    echo  gettype($data);
}
?>
--EXPECT--
array
