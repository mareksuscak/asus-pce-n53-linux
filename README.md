# Asus PCE-N53 linux driver for kernel 3.x

When I bought this Wireless network card it wasn't working in Ubuntu 14.10 with kernel 3.16. Then I have found [patched driver](https://github.com/unused/patched-Asus-PCE-N53-linux-driver) here on GitHub that was tested with Fedora but I wasn't able to compile it on Ubuntu so I've decided to create this repository and provide working sources for Ubuntu users. The instructions were taken from [this superuser thread](http://superuser.com/questions/663190/asus-pce-n53-11n-n600-pci-e-adapter-on-3-x-kernel). There's nothing more except for the need of having ethernet connection to install build dependencies.

## Important Note

Reportedly [5 GHz is not working anymore](https://github.com/mareksuscak/asus-pce-n53-linux/issues/2) with this patched driver as of 4.2.0-17-generic kernel. If you happen to know what's wrong or where I can find a patch for that, let me know and I'll gladly update the repo providing all the instructions so that all of us can get it to work.

## How to Install

Simply clone the repository, install build dependencies, compile sources, install compiled module and use modprobe to activate it.

```
$ sudo apt-get update
$ sudo apt-get upgrade -y
$ sudo apt-get install build-essential linux-headers-generic linux-headers-$(uname -r) -y
$ cd ~
$ git clone https://github.com/mareksuscak/asus-pce-n53-linux.git
$ cd asus-pce-n53-linux
$ make
$ sudo make install
$ sudo modprobe rt5592sta
```

If everything worked well you should be able to connect to the Wireless network now.

## Seeing Compile Errors?

Some users have had success compiling this driver after [making minor changes](https://github.com/mareksuscak/asus-pce-n53-linux/issues/4#issue-215234424) to the source code which normally result in a compile error.

## Contributors

These are people who contributed to this project free of charge. I'm thankful and grateful for your generosity guys.

[@jsuann](https://github.com/jsuann) - contributed [fixes for kernel 4.15+](https://github.com/mareksuscak/asus-pce-n53-linux/pull/6)
