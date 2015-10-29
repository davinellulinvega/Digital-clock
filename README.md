# Digital clock

This application is a small clock widget for Linux desktop.  
In bin folder folder you have, ready binary release, currently only for x64 Linux.  
I will add 32-bit version as soon as possible.  
  
If you want to compile it you need a least Qt5.4, please install Qt5.4 before compiling.  
You can download Qt5 from here: https://www.qt.io/download-open-source/  
Install it using:  
qmake -o Makefile digital_clock.pro  
make  
sudo install -o root -g root -m 755 -T digital_clock /bin/dclock  
  
  
On first run will be created config file ~/.config/digital_clock.ini.  
There you can configure yout widget.  
