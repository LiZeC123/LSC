在Win10子系统下运行程序
=======================

win10提供了一个ubuntu子系统, 在这个子系统中可以和真实的ubuntu系统一样编译和运行本项目. 但由于win10是64位的, 因此ubuntu子系统也默认只能运行64位程序. 

本项目编译的可执行文件是32位程序. 为了使得编译后的程序可以在win10子系统中运行, 需要执行如下的操作


``` bash
# 安装必要的服务
sudo apt update
sudo apt install qemu-user-static
sudo update-binfmts --install i386 /usr/bin/qemu-i386-static --magic '\x7fELF\x01\x01\x01\x03\x00\x00\x00\x00\x00\x00\x00\x00\x03\x00\x03\x00\x01\x00\x00\x00' --mask '\xff\xff\xff\xff\xff\xff\xff\xfc\xff\xff\xff\xff\xff\xff\xff\xff\xf8\xff\xff\xff\xff\xff\xff\xff'

# 启动服务
sudo service binfmt-support start
```

安装并启动服务以后, 就可以正常的运行32位程序. 由于win10子系统会随着win10系统一同启动和关闭, 因此每次重启系统后, 都需要手动启动这个服务.



参考资料
---------------

- [win10子系统linux下cmake编译32位程序](https://blog.csdn.net/zhangquan2015/article/details/91354443)
- [让64位的WSL（windows 子 linux系统）支持运行32位程序](https://www.jianshu.com/p/3df082840b40)
- [VisualStudio2017+SSH+WSL 开发调试Linux C/C++](https://blog.csdn.net/weixin_38337701/article/details/83903525)