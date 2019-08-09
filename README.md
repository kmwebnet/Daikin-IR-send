# Daikin-Air-conditioner-commander

This emulates ARC478A30 IR remote transceiver comes with Daikin Air conditioner by ESP32-DevkitC.  

# Requirements

  Platformio with VS Code environment.
  install "Espressif 32" platform definition on Platformio  

# Environment reference
  
  Espressif ESP32-DevkitC  
  [IR-LED](http://akizukidenshi.com/catalog/g/gI-03261/)    
  the rest of components needes as below:   
  ![irled](https://user-images.githubusercontent.com/46954791/62774036-b72a0080-bade-11e9-92ec-cfe6f5bf2e5f.png)

# Usage

  you need to change a serial port number which actually connected to ESP32 in platformio.ini.   
  Base pin for transistor should connect to GPIO 26(defined by daikindef.h)  

# Run this project

by default, this code emits power on, 24 celsius cool and fanmode Auto command signal as defined on main.c    
you can try different settings before executing sendframe function.   
  
# License

This software is released under the MIT License, see LICENSE.
