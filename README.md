# ESPMonitorUltra
ESPMonitorUltra  功能有时间显示、天气预报、黄历信息、电脑运行状态监测，支持AP Web配网 

基于xutoubee/ESPMonitorPlus++ 项目完整重写 更新了部分引用库的版本 lvgl升级到8.3.11 

优化了整个项目lvgl的目录结构

原作者地址 

- [ESPMonitorPlus++桌面监视器](https://gitee.com/xutoubee/ESPMonitorPlus )

###  目录说明

- AIDA64InfoReader - 上位机源码
- code - 项目源码
- others - 其他文件 字体库文件等
- ui_pro - SquareLine Studio工程 画的页面
- test -  用于项目从网络更新的背景图的测试项目目前还在完善

###  使用了哪些API接口

> 本项目主要以天行数据的API接口为主，大家自行去[注册](https://gitee.com/link?target=https%3A%2F%2Fwww.tianapi.com%2F)，程序中，需将自己的apikey进行替换，接口有：
>
> - [中国老黄历](https://gitee.com/link?target=https%3A%2F%2Fwww.tianapi.com%2Fapiview%2F45)
> - [天气预报](https://gitee.com/link?target=https%3A%2F%2Fwww.tianapi.com%2Fapiview%2F72)

###  如何获取cityCode城市代码

[中国天气网](https://gitee.com/link?target=http%3A%2F%2Fwww.weather.com.cn%2Fweather1d%2F101010100.shtml)，定位到自己的所在地，然后网址里面的9位数字就是你的城市代码

