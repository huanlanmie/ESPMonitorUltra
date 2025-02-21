# 项目名称：AIDA64InfoReader
## 一、本项目基于C# WinForm开发，用于读取AIDA64的共享内存数据，然后将数据以指定格式通过串口发送到Heltec WiFi_kit_32开发板上。开发板会解析接受到的数据并显示在开发板自带的OLED上。
>本项目参考自Bilibili网站up主“会飞的阿卡林”的[视频](https://www.bilibili.com/video/av77823747)以及他的github项目[AIDA64Reader](https://github.com/flyAkari/AIDA64Reader)
### 使用步骤如下:</br>
1. 安装AIDA64，并在该软件设置里面打开内存共享功能；
2. 使用VisualStudio2019打开本项目并编译运行；
3. 可以通过给程序传参数来指定设备端口；若没传参数且只有一个串口设备连接到了电脑，则在程序启动的时候自动使用该唯一串口；若串口设备大于1个，则需要在程序打开后的界面上手动选择串口，串口选择完成后，程序会自动使用该串口发送数据</br>
### 程序运行截图如下：
![主界面](./example/2020-01-20_23-32-51.png)