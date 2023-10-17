/*
"ifconfig lo 127.0.0.1 netmask 255.0.0.0 up",
"route add 127.0.0.0 netmask 255.0.0.0 lo",

"ifconfig eth0 192.168.2.128 netmask 255.255.255.0 hw ether AA:BB:CC:DD:EE:02 up",
"route add 192.168.2.0 netmask 255.255.255.0 eth0",
*/
"net_manager"
"msleep 1000",

"ls /dev",
"mkdir -v /conf",
"mount -t DumbFS /dev/stm32flash0 /conf",
"led_driver init",
"flash_settings restore",
"modbus_server &",
"ifconfig",
"stl_demo_sort1",
"httpd http_admin",
