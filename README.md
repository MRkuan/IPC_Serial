# IPC_Serial
基于IPC的串口通讯程序

## Build
### qmake
使用Qt打开*.pro文件，点击编译既可以
### cmake
使用cmake文件进行编译需进行如下配置
1. 修CMakeLists.txt文件，设置SYSROOT_DIR为Qt本地路径，例如我的是/home/fan/Qt5.4.0/5.4/gcc
2. 在终端cd到src目录，运行如下命令，生成moc文件
```
moc SerialServer.h > moc_SerialServer.cpp
```

或者将上面步骤qmake生成的moc_SerialServer.cpp文件拷贝过来使用，也可以

## UML类图叠加分层设计
![image](http://www.fanjiafei.cn/wp-content/uploads/2018/03/Overview-of-Design-Model-of-IPC-2.jpg)
