#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <Preferences.h>

#include "ui_h/ui.h"
#include "ui_h/ui_helpers.h"

const char *AP_NAME = "ESPMonitorPlus++"; // Web配网模式下的AP-wifi名字
extern String PrefSSID, PrefPassword, cityCode;

// 暂时存储wifi账号密码
char sta_ssid[32] = {0};
char sta_password[64] = {0};
char sta_citycode[32] = {0};

// 配网页面代码
//  String text = R"()"; //多行字符串的表达方式
String page_html = R"(
<!DOCTYPE html>
<html lang='en'>
  <head>
    <meta charset='UTF-8'>
   
    <meta name="viewport" content="width=device-width,initial-scale=1.0,maximum-scale=1.0,minimum-scale=1.0,user-scalable=no">
    <title>ESPMonitorPlus++配置</title>
    <style type="text/css">
      * { margin: 0; padding: 0; }
       html { height: 100%; }
       h2 {text-align: center;color: #fff;line-height: 2.2;}
       body { height: 100%; background-color: #1F6F4A; 50% 50% no-repeat; background-size: cover;}
       .dowebok { position: absolute; left: 50%; top: 30%; width: 380px; height: 500px; margin: -200px 0 0 -200px; border: 3px solid #fff; border-radius: 10px; overflow: hidden;}
       
       .form-item { position: relative; width: 360px; margin: 0 auto; padding-bottom: 20px;}
       .form-item input { width: 288px; height: 48px; padding-left: 10px; border: 1px solid #fff; border-radius: 25px; font-size: 18px; color: #fff; background-color: transparent; outline: none;}
       .send_button { width: 360px; height: 50px; border: 0; border-radius: 25px; font-size: 18px; color: #1f6f4a; outline: none; cursor: pointer; background-color: #fff; }
       
       .tip { display: none; position: absolute; left: 20px; top: 52px; font-size: 14px; color: #f50; }
       .reg-bar { width: 360px; margin: 20px auto 0; font-size: 14px; overflow: hidden;}
       .reg-bar a { color: #fff; text-decoration: none; }
       .reg-bar a:hover { text-decoration: underline; }
       .reg-bar .reg { float: left; }
       .reg-bar .forget { float: right; }
       .dowebok ::-webkit-input-placeholder { font-size: 18px; line-height: 1.4; color: #fff;}
       .dowebok :-moz-placeholder { font-size: 18px; line-height: 1.4; color: #fff;}
       .dowebok ::-moz-placeholder { font-size: 18px; line-height: 1.4; color: #fff;}
       .dowebok :-ms-input-placeholder { font-size: 18px; line-height: 1.4; color: #fff;}
        
       @media screen and (max-width: 500px) {
       * { box-sizing: border-box; }
       .dowebok { position: static; width: auto; height: auto; margin: 0 30px; border: 0; border-radius: 0; }
       .logo { margin: 50px auto; }
       .form-item { width: auto; }
       .form-item input, .form-item button, .reg-bar { width: 100%; }
       }
       
    </style>
  </head>
  
  <body>
    <div class="dowebok">
      <h2>参 数 配 置</h2>
      <form style='text-align: center;padding-top: 20px' name='input' action='/' method='POST'>  
         <div class="form-item">
          <input id="username" type="text" name='ssid' autocomplete="off" placeholder="WiFi名称">
         </div>
         <div class="form-item">
          <input id="password" type="password" name='password' autocomplete="off" placeholder="WiFi密码">
         </div>
         <div class="form-item">
          <input id="citycode" type="citycode" name='citycode' autocomplete="off" placeholder="城市代码,留空则自动定位获取">
         </div>
         <div class="form-item">
           <div id="">
            <input id="send_button" type='submit' value='保存并连接'>
           </div>
        </div>
        <div class="form-item">
          <div class="user_text">
            <br>
            <p><h3>如何获取cityCode：</h3></p>
              <h5>
                城市代码由9位阿拉伯数字组成，超过位数会导致获取不到数据而无限重启
              </h5>
            </p>
          </div>
         </div>
        
      </form> 
     </div>
  </body>
</html>
)";

const byte DNS_PORT = 53;       // DNS端口号
IPAddress apIP(192, 168, 4, 1); // esp32-AP-IP地址
DNSServer dnsServer;            // 创建dnsServer实例
WebServer server(80);           // 创建WebServer

void handleRoot()
{ // 访问主页回调函数
    server.send(200, "text/html", page_html);
}
void initSoftAP(void)
{ // 初始化AP模式
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    Serial.println(WiFi.softAPIP());
    //    Serial.print("本地IP： ");
    //  Serial.println(WiFi.localIP());
    if (WiFi.softAP(AP_NAME))
    {
        Serial.println("ESP32 SoftAP is right");
    }
}

void initDNS(void)
{ // 初始化DNS服务器
    if (dnsServer.start(DNS_PORT, "*", apIP))
    { // 判断将所有地址映射到esp8266的ip上是否成功
        Serial.println("start dnsserver success.");
    }
    else
        Serial.println("start dnsserver failed.");
}
void handleRootPost()
{ // Post回调函数
    String wifiid = "", wifipass = "", cityid = "";
    Serial.println("handleRootPost");
    if (server.hasArg("ssid"))
    { // 判断是否有账号参数
        Serial.print("got ssid:");
        strcpy(sta_ssid, server.arg("ssid").c_str()); // 将账号参数拷贝到sta_ssid中
        Serial.println(sta_ssid);
    }
    else
    { // 没有参数
        Serial.println("error, not found ssid");
        server.send(200, "text/html", "<meta charset='UTF-8'>提示：请输入WiFi名称"); // 返回错误页面
        return;
    }
    // 密码与账号同理
    if (server.hasArg("password"))
    {
        Serial.print("got password:");
        strcpy(sta_password, server.arg("password").c_str());
        Serial.println(sta_password);
    }
    else
    {
        Serial.println("error, not found password");
        server.send(200, "text/html", "<meta charset='UTF-8'>提示：请输入WiFi密码");
        return;
    }

    if (server.hasArg("citycode"))
    {
        Serial.print("got citycode:");
        strcpy(sta_citycode, server.arg("citycode").c_str());
        Serial.println(sta_citycode);
    }
    else
    {
        Serial.println("error, not found citycode");
        server.send(200, "text/html", "<meta charset='UTF-8'>提示：请输入城市代码");
        return;
    }
    //将网页发过来的数据：ssid、password、citycode存入到NVS存储中
    Preferences prefs;
    prefs.begin("wifi");
    wifiid = sta_ssid;
    wifipass = sta_password;
    cityid = sta_citycode;
    prefs.putString("ssid", wifiid);
    prefs.putString("password", wifipass);
    prefs.putString("citycode", cityid);
    prefs.end();

    lv_label_set_text(ui_page4_label_set_mode_info, "已经进入配网模式\n请打开手机WiFi，搜索并连接ESPMonitorPlus++\n连接成功后会自动打开网页\n如未打开，请前往浏览器输入192.168.4.1即可\n收到数据，正在配置并重启...");
    for(int i=0;i<=10;i++) {
        lv_timer_handler(); 
        delay(10);
    }
    //网页数据提交后，在手机网页端显示如下信息
    server.send(200, "text/html", "<meta charset='UTF-8'><h1>保存成功，ESP32重启中...</h1>"); // 返回保存成功页面
    delay(2000);
    // 连接wifi
    // connectNewWifi();

    ESP.restart(); // 重启ESP32
}
void initWebServer(void)
{ // 初始化WebServer
    // server.on("/",handleRoot);
    // 上面那行必须以下面这种格式去写否则无法强制门户
    server.on("/", HTTP_GET, handleRoot);      // 设置主页回调函数
    server.onNotFound(handleRoot);             // 设置无法响应的http请求的回调函数
    server.on("/", HTTP_POST, handleRootPost); // 设置Post请求回调函数
    server.begin();                            // 启动WebServer
    Serial.println("WebServer started!");
}
void connectNewWifi(void)
{
    Preferences prefs;
    prefs.begin("wifi");
    if (prefs.isKey("ssid"))
        PrefSSID = prefs.getString("ssid");
    Serial.println(PrefSSID);
    if (prefs.isKey("password"))
        PrefPassword = prefs.getString("password");
    Serial.println(PrefPassword);
    if (prefs.isKey("citycode"))
        cityCode = prefs.getString("citycode");
    Serial.println(cityCode);
    prefs.end();
    // WiFi.mode(WIFI_STA);
    WiFi.begin(PrefSSID.c_str(), PrefPassword.c_str());

    WiFi.mode(WIFI_STA);       // 切换为STA模式
    WiFi.setAutoConnect(true); // 设置自动连接
    //  WiFi.begin(PrefSSID.c_str(), PrefPassword.c_str());//连接上一次连接成功的wifi
    Serial.println("");
    Serial.print("Connect to wifi");
    int count = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        count++;
        if (count > 20)
        { // 如果10秒内没有连上，就开启Web配网 可适当调整这个时间
            initSoftAP();
            initWebServer();
            initDNS();
            break; // 跳出 防止无限初始化
        }
        Serial.print(".");
    }
    Serial.println("");
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("WIFI Connected!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        server.stop();
    }
}
