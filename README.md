# embox_project_modbus_iocontrol

embox_project_modbus_iocontrol is an Embox external project. It demonstrates how to libmodbus work in Embox. There are several target to run this demo:
* Standalong on Linux host
* In Embox on ARM QEMU
* In Embox on STM32F4-discovery

LEDs are emulated On QEMU and on host. The real on-board LEDs are used on STM32f4-Discovery.

To build on host (client and server) you need follow:
* Move to 'modbus/host_cmds' folder
* Build with 'make'

Run an emulation on the host:
* Build client and server
* Move to 'modbus/emulate' folder
* Run server './led-server'
* To run web-site you can go to 'modbus/http_admin' and run built-in python http server `python3 -m http.server --cgi -d .` Then you can check the LEDs states on a browser.
* To run the modbus client. Go to 'modbus/emulate' and run './led-client -a 127.0.0.1 set 78' to set up led 78 to high state or './led-client -a 127.0.0.1 clr 78' to low state

Run an emulation on the ARM QEMU:
* Build the modbus client
* Setup the project as external in Embox `make ext_conf EXT_PROJECT_PATH=<path to root folder of this repo>`. For example `make ext_conf EXT_PROJECT_PATH=~/git/embox_project_modbus_iocontrol`
* Configure Embox `make confload-ext_project/modbus/arm_qemu`
* Build Embox `make`
* Run Embox `./scripts/qemu/auto_qemu`
* To run the modbus client. Go to 'modbus/emulate' and run './led-client -a 10.0.2.16 set 78' to set up led 78 to high state or './led-client -a 10.0.2.16 clr 78' to low state
* Additionally you can check state in your browser if visit page with 10.0.2.16 address

Run on a STM32F4-discovery board
* Build the modbus client
* Setup the project as external in Embox `make ext_conf EXT_PROJECT_PATH=<path to root folder of this repo>`. For example `make ext_conf EXT_PROJECT_PATH=~/git/embox_project_modbus_iocontrol`
* Configure Embox `make confload-ext_project/modbus/stm32f4_discovery_demo`
* Setup you network settings in 'conf/start_script.inc' file
* Build Embox `make`
* Upload image on the board
* To run the modbus client. Go to 'modbus/emulate' and run './led-client -a "board IP" set 4' to set up led 4 to high state or './led-client -a "board IP" clr 4' to low state
* Additionally you can check state in your browser if visit page with "board IP" address
