avrdude: ser_open(): can't open device "/dev/ttyACM0": Permission denied
ioctl("TIOCMGET"): Inappropriate ioctl for device

To fix it, enter the command:
$ sudo usermod -a -G dialout <username>
$ sudo chmod a+rw /dev/ttyACM0
