<?php
$address = gethostbyname("localhost");
$port = 50020;

$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);

if ($socket === false) {
	echo "socket_create failed";
}

$result = socket_connect($socket, $address, $port);


$sub_packet = '~~~subscribe{{name,"datatest"},{devices{device{"data_esp","ExE101@89#sp"}}}}';



//$sub_packet = '~~~subscribe{{name,ttest},{devices{device{data_xbee,Pass1234}}}}';
//$sub_packet = '~~~subscribe{{name,"datatest"},{devices{device{"data_xbee","DxB81^62%fg"}}}}';

//$sub_packet = '~~~subscribe{{name,"ttest"},{devices{device{"data_xbee","Pass1234"}}}}';
socket_write($socket, $sub_packet, strlen($sub_packet));
echo "sleep 1\n";
sleep(1);

echo "send at\n";

/*$at_packet = "AT\r\n";

$at_packet = "AT+CIPSEND=0,5\r\n";
socket_write($socket, $at_packet, strlen($sub_packet));

sleep(1);
$at_packet = "hello";
socket_write($socket, $at_packet, strlen($sub_packet));
sleep(1);*/



$at_packet = "AT+CIPSTO?\r\n";
socket_write($socket, $at_packet, strlen($sub_packet));

echo "wait\n";
$out = '';

while ($out = socket_read($socket, 2048)) {
    echo $out;
}

socket_close($socket); 
