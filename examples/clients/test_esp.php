<?php
$address = gethostbyname("localhost");
$port = 50000;

$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);

if ($socket === false) {
	echo "socket_create failed";
}

$result = socket_connect($socket, $address, $port);


$sub_packet = '~~~subscribe{{name,"datatest"},{devices{device{"data_esp","ExE101@89#sp"}}}}';



//$sub_packet = '~~~subscribe{{name,ttest},{devices{device{data_xbee,Pass1234}}}}';
//$sub_packet = '~~~subscribe{{name,"datatest"},{devices{device{"data_xbee","DxB81^62%fg"}}}}';

$sub_packet = '~~~subscribe{{name,"ttest"},{devices{device{"data_xbee","Pass1234"}}}}';
socket_write($socket, $sub_packet, strlen($sub_packet)+1);

$out = '';

while ($out = socket_read($socket, 2048)) {
    echo $out;
}

socket_close($socket); 
