<?php
$address = gethostbyname("data.traceyemery.net");
$port = 50020;

$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);

if ($socket === false) {
	echo "socket_create failed";
}

$result = socket_connect($socket, $address, $port);
$sub_packet = '~~~subscribe{{name,datatest},{devices{device{data_esp,ExE101@89#sp}}}}';


//$sub_packet = '~~~subscribe{{name,"datatest"},{devices{device{"data_xbee","DxB81^62%fg"}}}}';
socket_write($socket, $sub_packet, strlen($sub_packet));

$out = '';

while ($out = socket_read($socket, 2048)) {
    echo $out;
}

socket_close($socket); 
