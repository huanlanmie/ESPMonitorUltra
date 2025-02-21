#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <lvgl.h>
#include <demos/lv_demos.h>
#include <NTP.h>
// http
#include <HTTPClient.h>
// jsonformat
#include <ArduinoJson.h>
// dht11
#include <DHT.h>
// 单按钮
#include <OneButton.h>
// Preferences库
#include <Preferences.h>
// wifi
#include <esp_wifi.h>

#include "../include/ui_h/ui.h"
#include "../include/setWifi.h"

// 初始化dht11 引脚
#define DHTPIN 47
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// 按键引脚初始化
OneButton button21(21, true);

static String city_code;                                                   // 城市代码
static const String tianxing_api_key = "445f75c8daa9aababfb7e23b20f4801b"; // 天行秘钥
int dht11_temperature, dht11_humidity;                                     // dht11温湿度
int update_times_current_weather = 60;                                     // 更新当前天气信息的时间间隔  单位/分钟
int update_times_weather_forecast = 360;                                   // 更新天气预报信息的时间间隔  单位/分钟

bool attachDuringLongPressNumber; // 是否在长按事件中附加数字
String PrefSSID, PrefPassword;    // 存储wifi信息
bool setWiFi_Flag = false;        // 是否设置wifi

Arduino_DataBus *bus = new Arduino_ESP32LCD16(
    16, 17, 18 /* WR */, 19 /* RD */,
    0 /* D0 */, 1 /* D1 */, 2 /* D2 */, 3 /* D3 */, 4 /* D4 */, 5 /* D5 */, 6 /* D6 */, 7 /* D7 */,
    8 /* D8 */, 9 /* D9 */, 10 /* D10 */, 11 /* D11 */, 12 /* D12 */, 13 /* D13 */, 14 /* D14 */, 15 /* D15 */);
Arduino_GFX *gfx = new Arduino_NT35510(bus, 20 /* RST */, 3 /* rotation */);

/* Change to your screen resolution */
static uint32_t screenWidth;
static uint32_t screenHeight;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *disp_draw_buf;
static lv_disp_drv_t disp_drv;

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

#if (LV_COLOR_16_SWAP != 0)
  gfx->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#else
  gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#endif

  lv_disp_flush_ready(disp);
}


void setWiFi()
/**
 * 设置wifi
*/
{
  initSoftAP();
  initWebServer();
  initDNS();
  while (setWiFi_Flag == false)
  {
    lv_timer_handler();
    server.handleClient();
    dnsServer.processNextRequest();
    if (WiFi.status() == WL_CONNECTED)
    {
      server.stop();
      setWiFi_Flag = true;
    }
  }
}



void displayWeatherPng(String weatherimg, lv_obj_t *obj)
{
  /**
   * String weatherimg 天气图标
   * lv_obj_t *obj 要显示天气图标的对象
   *
   * 设置天气图标的函数
   * weathercodes数组的索引与天气图标的索引对应，所以需要遍历数组中的每个元素，
   * 如果当前元素的值等于传入的 weathercode，则设置对应的天气图标。
   */

  // 处理weatherimg 去掉.png后缀
  weatherimg.replace(".png", "");
  // 创建一个字符串数组用于存放天气名
  String weathercodes[] = {"baoxue", "baoyu", "bingbao", "dabaoyu", "dawu", "daxue", "dayu", "dongyu", "duoyun", "fuchen", "leizhenyu", "mai", "qing", "shachenbao", "tedabaoyu", "wu", "xiaoxue", "xiaoyu", "xue", "yangsha", "yin", "yu", "yujiaxue", "zhenyu", "zhongxue", "zhongyu"};
  // 创建一个存放天气图标的数组
  const lv_img_dsc_t *image_array[] = {
      &baoxue,
      &baoyu,
      &bingbao,
      &dabaoyu,
      &dawu,
      &daxue,
      &dayu,
      &dongyu,
      &duoyun,
      &fuchen,
      &leizhenyu,
      &mai,
      &qing,
      &shachenbao,
      &tedabaoyu,
      &wu,
      &xiaoxue,
      &xiaoyu,
      &xue,
      &yangsha,
      &yin,
      &yu,
      &yujiaxue,
      &zhenyu,
      &zhongxue,
      &zhongyu,
  };
  // 遍历数组中的每个元素
  for (int i = 0; i < sizeof(weathercodes) / sizeof(weathercodes[0]); i++)
  {
    // 如果当前元素的值等于传入的 weathercode
    if (weathercodes[i].equals(weatherimg))
    {
      lv_img_set_src(obj, image_array[i]);
      break;
    }
  }
}

String str_time_hour()
{
  /**
   * 小时补全
   */
  int hour_ = hour();
  if (hour_ < 10)
  {
    return "0" + String(hour_);
  }
  else
  {
    return String(hour_);
  }
}

String str_time_minute()
{
  /**
   * 分钟补全
   */
  int minute_ = minute();
  if (minute_ < 10)
  {
    return "0" + String(minute_);
  }
  else
  {
    return String(minute_);
  }
}

void getCityCode()
{
  /**
   * 获取城市代码地址
   */

  HTTPClient http;
  // 获取地址
  http.begin("http://wgeo.weather.com.cn/ip/?_=" + String(now() - 28800));
  http.addHeader("referer", "http://wgeo.weather.com.cn/");
  http.setUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/131.0.0.0 Safari/537.36");
  int httpCode = http.GET();
  if (httpCode > 0)
  {
    if (httpCode == HTTP_CODE_OK)
    {
      String httpText = http.getString();
      int from = httpText.indexOf(";var id=");
      int to = httpText.indexOf(";var addr=");
      city_code = httpText.substring(from + 9, to - 1);
      ESP_LOGI("运行日志", "获取城市代码：成功");
    }
  }
  else
  {
    // Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    ESP_LOGI("运行日志", "获取城市代码：失败");
  }
}

void getCurrentWeather()
{
  /**
   * 获取当前的天气信息
   */

  getCityCode();
  HTTPClient http;
  // 获取天气信息
  http.begin("https://apis.tianapi.com/tianqi/index?key=" + tianxing_api_key + "&city=" + city_code + "&type=1");
  http.addHeader("referer", "https://apis.tianapi.com/");
  http.setUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/131.0.0.0 Safari/537.36");
  int httpCode = http.GET();
  if (httpCode > 0)
  {
    if (httpCode == HTTP_CODE_OK)
    {
      String httpText = http.getString();
      ESP_LOGI("运行日志", "获取实时天气：成功");
      // String input;
      StaticJsonDocument<4096> doc;

      DeserializationError error = deserializeJson(doc, httpText);

      if (error)
      {
        // Serial.print("deserializeJson() failed: ");
        // Serial.println(error.c_str());
        return;
      }

      int code = doc["code"];       // 200
      const char *msg = doc["msg"]; // "success"

      JsonObject result = doc["result"];
      const char *result_date = result["date"];               // "2024-12-06"
      const char *result_week = result["week"];               // "星期五"
      const char *result_province = result["province"];       // "福建"
      const char *result_area = result["area"];               // "福州"
      const char *result_areaid = result["areaid"];           // "101230101"
      const char *result_weather = result["weather"];         // "阴"
      const char *result_weatherimg = result["weatherimg"];   // "yin.png"
      const char *result_weathercode = result["weathercode"]; // "yin"
      const char *result_real = result["real"];               // "17.2℃"
      const char *result_lowest = result["lowest"];           // "15℃"
      const char *result_highest = result["highest"];         // "21℃"
      const char *result_wind = result["wind"];               // "西北风"
      const char *result_windspeed = result["windspeed"];     // "4"
      const char *result_windsc = result["windsc"];           // "1级"
      const char *result_sunrise = result["sunrise"];         // "06:37"
      const char *result_sunset = result["sunset"];           // "17:11"
      const char *result_moonrise = result["moonrise"];       // nullptr
      const char *result_moondown = result["moondown"];       // nullptr
      const char *result_pcpn = result["pcpn"];               // "0"
      const char *result_uv_index = result["uv_index"];       // "0级"
      const char *result_aqi = result["aqi"];                 // "30"
      const char *result_quality = result["quality"];         // "优"
      const char *result_vis = result["vis"];                 // "16"
      const char *result_humidity = result["humidity"];       // "75"

      const char *result_tips = result["tips"]; // "天气凉，适宜着一到两件羊毛衫、大衣、毛套装、皮夹克等春秋着装；年老体弱者宜着风衣加羊毛衫 ...

      lv_label_set_text_fmt(ui_page2_panel_main_label_suggest, "%s", result_tips);                              // 生活天气建议
      lv_label_set_text_fmt(ui_page2_panel_main_label_current_temp, "%s", result_real);                         // 当前温度
      lv_label_set_text_fmt(ui_page2_panel_main_label_current_text, "%s | %s", result_weather, result_quality); // 天气现象 空气质量
      lv_label_set_text_fmt(ui_page2_label_location, "%s", result_province);                                    // 城市
      lv_label_set_text_fmt(ui_page2_panel_main_label_week, "%s", result_week);                                 // 星期
      lv_label_set_text_fmt(ui_page2_panel_main_label_date, "%d年%d月%d日", year(), month(), day());
      lv_label_set_text_fmt(ui_page2_label_update, "最后更新时间 %d月%d日 %s:%s", month(), day(), str_time_hour(), str_time_minute());

      // 设置当前的天气图标
      String weatherimg = result_weatherimg;
      displayWeatherPng(weatherimg, ui_page2_image_current_weather);

      const char *warnlevel = doc["result"]["alarmlist"][0]["level"];
      const char *warntype = doc["result"]["alarmlist"][0]["type"];

      // 气象预警级别：蓝 黄 橙 红
      String warnlevel_ = warnlevel;
      String warntype_ = warntype;

      // 设置预警图标
      if (warnlevel_.length() > 0)
      {
        if (warnlevel_ == "蓝色")
        {
          lv_obj_set_style_bg_color(ui_page2_panel_main_warn, lv_color_hex(0x0000FF), LV_PART_MAIN | LV_STATE_DEFAULT); // 蓝
        }
        else if (warnlevel_ == "黄色")
        {
          lv_obj_set_style_bg_color(ui_page2_panel_main_warn, lv_color_hex(0xFFFF00), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        else if (warnlevel_ == "橙色")
        {
          lv_obj_set_style_bg_color(ui_page2_panel_main_warn, lv_color_hex(0xFFA500), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        else if (warnlevel_ == "红色")
        {
          lv_obj_set_style_bg_color(ui_page2_panel_main_warn, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT); // 红
        }
        lv_label_set_text_fmt(ui_page2_panel_main_label_current_temp2, "%s预警", warntype_);
      }
      else
      {
        lv_obj_set_style_bg_color(ui_page2_panel_main_warn, lv_color_hex(0x89d961), LV_PART_MAIN | LV_STATE_DEFAULT); // 黑
        lv_label_set_text_fmt(ui_page2_panel_main_label_current_temp2, "美好时光^_^");
      }
    }
  }
  else
  {
    ESP_LOGI("运行日志", "获取实时天气：失败");
    // Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
}

void get7DaysWeather()
{

  /**
   * 获取七天的天气信息
   */

  getCityCode();
  HTTPClient http;
  // 获取七天的天气信息
  http.begin("https://apis.tianapi.com/tianqi/index?key=" + tianxing_api_key + "&city=" + city_code + "&type=7");
  http.addHeader("referer", "https://apis.tianapi.com/");
  http.setUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/131.0.0.0 Safari/537.36");
  int httpCode = http.GET();
  if (httpCode > 0)
  {
    if (httpCode == HTTP_CODE_OK)
    {
      String httpText = http.getString();
      // String input;
      ESP_LOGI("运行日志", "获取七日天气信息：成功");

      StaticJsonDocument<4096> doc;

      DeserializationError error = deserializeJson(doc, httpText);

      if (error)
      {
        // Serial.print("deserializeJson() failed: ");
        // Serial.println(error.c_str());
        return;
      }

      int code = doc["code"];       // 200
      const char *msg = doc["msg"]; // "success"

      JsonObject result = doc["result"];

      const char *lowest0 = doc["result"]["list"][0]["lowest"];
      const char *highest0 = doc["result"]["list"][0]["highest"];
      const String weatherimg0 = doc["result"]["list"][0]["weatherimg"];

      const char *lowest1 = doc["result"]["list"][1]["lowest"];
      const char *highest1 = doc["result"]["list"][1]["highest"];
      const char *week1 = doc["result"]["list"][1]["week"];
      const String weatherimg1 = doc["result"]["list"][1]["weatherimg"];

      const char *lowest2 = doc["result"]["list"][2]["lowest"];
      const char *highest2 = doc["result"]["list"][2]["highest"];
      const char *week2 = doc["result"]["list"][2]["week"];
      const String weatherimg2 = doc["result"]["list"][2]["weatherimg"];

      lv_label_set_text_fmt(ui_page2_panel_main_label_current_day1_temp, "%s-%s", lowest0, highest0);
      lv_label_set_text_fmt(ui_page2_panel_main_label_current_day2_temp, "%s-%s", lowest1, highest1);
      lv_label_set_text_fmt(ui_page2_panel_main_label_current_day2, "%s", week1);
      lv_label_set_text_fmt(ui_page2_panel_main_label_current_day3_temp, "%s-%s", lowest2, highest2);
      lv_label_set_text_fmt(ui_page2_panel_main_label_current_day3, "%s", week2);

      displayWeatherPng(weatherimg0, ui_page2_image_current_weather_day1);
      displayWeatherPng(weatherimg1, ui_page2_image_current_weather_day2);
      displayWeatherPng(weatherimg2, ui_page2_image_current_weather_day3);
    }
  }
  else
  {
    // Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    ESP_LOGI("运行日志", "获取七日天气信息：失败");
  }
  http.end();
}

void getChineseAlmanac()
{
  /**
   * 获取中国黄历信息
   */

  HTTPClient http;
  // 获取中国黄历信息
  http.begin("https://apis.tianapi.com/lunar/index?key=" + tianxing_api_key + "&date=" + year() + "-" + month() + "-" + day());
  String url = "https://apis.tianapi.com/lunar/index?key=" + tianxing_api_key + "&date=" + year() + "-" + month() + "-" + day();
  http.addHeader("referer", "https://apis.tianapi.com/");
  http.setUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/131.0.0.0 Safari/537.36");
  int httpCode = http.GET();

  if (httpCode > 0)
  {
    if (httpCode == HTTP_CODE_OK)
    {
      ESP_LOGI("运行日志", "获取中国黄历信息：成功");
      String httpText = http.getString();
      // String input;
      StaticJsonDocument<4096> doc;

      DeserializationError error = deserializeJson(doc, httpText);

      if (error)
      {
        // Serial.print("deserializeJson() failed: ");
        // Serial.println(error.c_str());
        return;
      }

      int code = doc["code"];       // 200
      const char *msg = doc["msg"]; // "success"

      JsonObject result = doc["result"];
      const char *result_gregoriandate = result["gregoriandate"];         // "2024-12-09"
      const char *result_lunardate = result["lunardate"];                 // "2024-11-9"
      const char *result_lunar_festival = result["lunar_festival"];       // nullptr
      const char *result_festival = result["festival"];                   // "纪念一二·九运动 世界足球日"
      const char *result_fitness = result["fitness"];                     // "经营.交易.求官.纳畜.动土"
      const char *result_taboo = result["taboo"];                         // "登高.行船.安床.入宅.博彩"
      const char *result_shenwei = result["shenwei"];                     // "喜神：正南 福神：东南 财神：正西 阳贵：西北 阴贵：正西 "
      const char *result_taishen = result["taishen"];                     // "仓库忌修弄,厕道莫修移,厨灶莫相干胎神在房内东停留5天"
      const char *result_chongsha = result["chongsha"];                   // "羊日冲(辛丑)牛"
      const char *result_suisha = result["suisha"];                       // "岁煞西"
      const char *result_wuxingjiazi = result["wuxingjiazi"];             // "火"
      const char *result_wuxingnayear = result["wuxingnayear"];           // "佛灯火"
      const char *result_wuxingnamonth = result["wuxingnamonth"];         // "洞下水"
      const char *result_xingsu = result["xingsu"];                       // "南方张月鹿-吉"
      const char *result_pengzu = result["pengzu"];                       // "丁不剃头 未不服药"
      const char *result_jianshen = result["jianshen"];                   // "危"
      const char *result_tiangandizhiyear = result["tiangandizhiyear"];   // "甲辰"
      const char *result_tiangandizhimonth = result["tiangandizhimonth"]; // "丙子"
      const char *result_tiangandizhiday = result["tiangandizhiday"];     // "丁未"
      const char *result_lmonthname = result["lmonthname"];               // "仲冬"
      const char *result_shengxiao = result["shengxiao"];                 // "龙"
      const char *result_lubarmonth = result["lubarmonth"];               // "十一月"
      const char *result_lunarday = result["lunarday"];                   // "初九"
      const char *result_jieqi = result["jieqi"];                         // nullptr

      String result_tiangandizhiyear_ = result_tiangandizhiyear;
      String result_shengxiao_ = result_shengxiao;
      String result_lubarmonth_ = result_lubarmonth;
      String result_lunarday_ = result_lunarday;

      lv_label_set_text_fmt(ui_page2_panel_main_label_lunar, "%s%s年%s%s", result_tiangandizhiyear_, result_shengxiao_, result_lubarmonth_, result_lunarday_);
    }
  }
  else
  {
    // Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    ESP_LOGI("运行日志", "获取中国黄历信息：失败");
  }
  http.end();
}

// 图片数据结构
lv_img_dsc_t img_dsc;

// 下载图片
bool downloadImage(const char *url, uint8_t **buffer, size_t *length)
{
  HTTPClient http;
  http.begin(url);
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK)
  {
    *length = http.getSize();
    *buffer = (uint8_t *)malloc(*length);
    // 检测能否分配内存
    if (*buffer)
    {
      WiFiClient *stream = http.getStreamPtr();
      stream->readBytes(*buffer, *length);
      http.end();
      ESP_LOGD("运行日志", "获取图片：成功");
      return true;
    }
    ESP_LOGD("运行日志", "获取图片：失败空间不足");
  }
  http.end();
  return false;
}

// 转换 BMP 图片为 LVGL 可用的格式
bool convertBmpToLvgl(uint8_t *buffer, size_t length, lv_img_dsc_t *img_dsc)
{
  if (!buffer || length == 0)
  {
    return false;
  }

  // 验证 BMP 文件头
  if (buffer[0] != 'B' || buffer[1] != 'M')
  {
    Serial.println("Invalid BMP format");
    return false;
  }

  // 解析 BMP 宽高
  int32_t width = *(int32_t *)&buffer[18];
  int32_t height = *(int32_t *)&buffer[22];
  uint16_t bpp = *(uint16_t *)&buffer[28]; // 每像素位数

  if (bpp != 24)
  {
    ESP_LOGD("运行日志", "转换图片：失败 只支持 24 位深度的 BMP 图片");
    return false;
  }

  // BMP 数据起始位置
  int32_t data_offset = *(int32_t *)&buffer[10];

  // 为 RGB565 图像分配内存
  size_t img_size = width * height * 2; // 每像素 2 字节
  uint8_t *img_data = (uint8_t *)malloc(img_size);
  if (!img_data)
  {
    ESP_LOGD("运行日志", "转换图片：失败 内存不足");
    return false;
  }

  // BMP -> RGB565 转换
  uint8_t *src = buffer + data_offset;
  uint16_t *dst = (uint16_t *)img_data;
  for (int y = height - 1; y >= 0; y--)
  { // BMP 从底部行开始
    for (int x = 0; x < width; x++)
    {
      uint8_t b = src[y * width * 3 + x * 3 + 0];
      uint8_t g = src[y * width * 3 + x * 3 + 1];
      uint8_t r = src[y * width * 3 + x * 3 + 2];
      uint16_t color = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3); // RGB888 -> RGB565

#if LV_COLOR_16_SWAP
      // 如果启用了 16 位色深的位交换
      color = (color >> 8) | (color << 8);
#endif

      *dst++ = color;
    }
  }

  // 填充 LVGL 图片描述符
  img_dsc->header.always_zero = 0;
  img_dsc->header.w = width;
  img_dsc->header.h = height;
  img_dsc->data_size = img_size;
  img_dsc->header.cf = LV_IMG_CF_TRUE_COLOR;
  img_dsc->data = img_data;

  return true;
}

// 显示图片
void displayImageAsBackground(lv_img_dsc_t *img_dsc)
{
  if (!img_dsc)
    return;
  lv_img_set_src(ui_page5_image_background, img_dsc); // 设置图片源
}

char page_number = 0;
void attachClick21()
{
  /**
   * 按钮单击事件
   */
  page_number++;
  switch (page_number)
  {
  case 1:
    lv_disp_load_scr(ui_page5_screen_main);
    break;
  case 2:
    lv_disp_load_scr(ui_page2_screen_weather);
    break;
  case 3:
    lv_disp_load_scr(ui_page3_screen_monitor);
    page_number = 0;
    break;
  default:
    break;
  }
}

void attachDoubleClick21()
{
  /**
   * 按钮双击事件
   */
  if (page_number == 1 && attachDuringLongPressNumber == false)
  {
    uint8_t *imageBuffer = nullptr;
    size_t imageLength = 0;

    // 清除上一次获取到的图片数据
    if (img_dsc.data)
    {
      // 说明当前页面处于：时间页面
      lv_img_set_src(ui_page5_image_background, &ui_img_images_bizhi_yejing1_png); // 回复默认背景图片
      free((void *)img_dsc.data);                                                  // 释放动态分配的图片数据内存
      img_dsc.data = NULL;                                                         // 将指针置为 NULL，避免悬空指针
    }
    if (downloadImage("http://192.168.1.6:5500/bg.bmp", &imageBuffer, &imageLength))
    {
      convertBmpToLvgl(imageBuffer, imageLength, &img_dsc);
      displayImageAsBackground(&img_dsc);
      free(imageBuffer);
      ESP_LOGI("运行日志", "获取背景图片成功");
    }
    else
    {
      ESP_LOGI("运行日志", "获取背景图片失败");
    }
  }
  
  if (attachDuringLongPressNumber == true)
  {
    attachDuringLongPressNumber = false;
    // 执行配网流程
    setWiFi_Flag = false;
    lv_label_set_text(ui_page4_label_set_mode_info, "双击按钮进入配网模式，单击按钮退出此页面\n已经进入配网模式\n请打开手机WiFi，搜索并连接ESPMonitorPlus++\n连接成功后会自动打开网页\n如未打开，请前往浏览器输入192.168.4.1即可");
    setWiFi();
  }
}

void attachDuringLongPress21()
{
  /**
   * 按钮长按事件
   */

  if (WiFi.status() == WL_CONNECTED)
  {
    attachDuringLongPressNumber = true;
    lv_label_set_text(ui_page4_label_wifi_connection_status, "WIFI：已连接");
    lv_label_set_text_fmt(ui_page4_label_wifi_ip_address, "IP地址：%s", WiFi.localIP().toString().c_str());
    lv_label_set_text_fmt(ui_page4_label_wifi_signal_strength, "信号强度：%ddBm", WiFi.RSSI());
    lv_label_set_text(ui_page4_label_set_mode_info, "双击按钮进入配网模式，单击按钮退出此页面");
    lv_disp_load_scr(ui_page4_screen_set_about);
  }
}

String Aida64Data_JSON;
void getAida64Data()
{
  /**
   *  从Aida64获取数据
   *  Serial.available(); // -> 获取串口接收缓冲区的字节数量
   *  Serial.read();      // -> 从串口接收缓冲区读取1个字节数据,读完之后,缓冲区的字节数量减1
   */
  int serial_data_length = Serial.available();
  if (serial_data_length > 0)
  {
    // 接收全部的数据
    for (int i = 0; i < serial_data_length; i++)
    {
      char buf = Serial.read();
      Aida64Data_JSON += buf;
      delayMicroseconds(100); // 替代 delay(10)，减少阻塞
    }

    // 解析数据
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, Aida64Data_JSON);
    if (error)
    {
      // Serial.print("deserializeJson() failed: ");
      // Serial.println(error.c_str());
      return;
    }
    const char *SCPUCLK = doc["SCPUCLK"];         // "3187"   cpu主频
    const char *SCPUUTI = doc["SCPUUTI"];         // "10"     cpu使用率
    const char *SMEMUTI = doc["SMEMUTI"];         // "42"     内存使用率
    const char *SNIC5DLRATE = doc["SNIC5DLRATE"]; // "0.3"    网速下载
    const char *SNIC5ULRATE = doc["SNIC5ULRATE"]; // "0.8"    网速上传
    const char *TCPU = doc["TCPU"];               // "53"     cpu温度
    const char *TGPU1 = doc["TGPU1"];             // "51"     gpu温度
    const char *PGPU1TDPP = doc["PGPU1TDPP"];     // "9"      gpu功耗
    const char *FCPU = doc["FCPU"];               // "42"     cpu风扇转速
    const char *FGPU1 = doc["FGPU1"];             // "0"      gpu风扇转速

    // cpuinfo
    lv_label_set_text_fmt(ui_page3_label_cpu_use_, "%s", SCPUUTI);
    lv_label_set_text_fmt(ui_page3_label_cpu_use_temp, "温度: %s℃", TCPU);
    lv_label_set_text_fmt(ui_page3_label_cpu_freq, "主频: %sHz", SCPUCLK);
    lv_label_set_text_fmt(ui_page3_label_cpu_fanspeed, "风扇: %sRPM", FCPU);

    // meminfo
    lv_label_set_text_fmt(ui_page3_label_mem_use_, "%s", SMEMUTI);

    // gpuinfo
    lv_label_set_text_fmt(ui_page3_label_gpu_use_temp, "温度: %s℃", TGPU1);
    String PGPU1TDPP_ = PGPU1TDPP;
    int PGPU1TDPP_Value = (PGPU1TDPP_.toInt() * 170) / 100;
    lv_label_set_text_fmt(ui_page3_label_gpu_tdp, "%dW", PGPU1TDPP_Value);
    lv_label_set_text_fmt(ui_page3_label_gpu_fanspeed, "风扇: %sRPM", FGPU1);

    // netinfo
    lv_label_set_text_fmt(ui_page3_label_download_speed, "%sKb/s", SNIC5DLRATE);
    lv_label_set_text_fmt(ui_page3_label_uploadspeed, "%sKb/s", SNIC5ULRATE);

    // 圆弧Arc
    String SCPUUTI_ = SCPUUTI;
    int SCPUUTI_Value = SCPUUTI_.toInt();
    lv_arc_set_value(ui_page3_arc_cpu_use, SCPUUTI_Value);

    String SMEMUTI_ = SMEMUTI;
    int SMEMUTI_Value = SMEMUTI_.toInt();
    lv_arc_set_value(ui_page3_arc_gpu_use, SMEMUTI_Value);
  }
  Aida64Data_JSON = "";
}

void updateDHTData()
{
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // 检查读取是否成功
  if (isnan(temperature) || isnan(humidity))
  {
    // Serial.println("Failed to read from DHT sensor!");
    return;
  }

  int dht11_temperature = static_cast<int>(temperature);
  int dht11_humidity = static_cast<int>(humidity);

  lv_label_set_text_fmt(ui_page2_panel_main_label_inroom_temp, "%d℃", dht11_temperature);
  lv_label_set_text_fmt(ui_page2_panel_main_label_inroom_hub, "%d%%", dht11_humidity);
  lv_label_set_text_fmt(ui_page3_label_net_speed_text2, "%d℃ 湿度：%d%%", dht11_temperature, dht11_humidity);
}

void setup(void)
{
  // 设置波特率
  Serial.begin(115200);
  // 关闭背光
  pinMode(38, OUTPUT);
  digitalWrite(38, LOW);
  gfx->begin();
  // 打开背光
  digitalWrite(38, HIGH);

   // 打开dht11
  dht.begin();

  // LVGL初始化获取屏幕尺寸
  lv_init();
  screenWidth = gfx->width();
  screenHeight = gfx->height();
  disp_draw_buf = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * screenWidth * 32, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, screenWidth * 32);
  /* Initialize the display */
  lv_disp_drv_init(&disp_drv);
  /* Change the following line to your display resolution */
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  ui_init();

  // 首次使用自动进入配网模式,读取NVS存储空间内的ssid、password和citycode
  Preferences prefs;
  prefs.begin("wifi");
  if (prefs.isKey("ssid"), "nano")
    PrefSSID = prefs.getString("ssid", "nano"); // 如果键值为空，返回0
  if (prefs.isKey("password"))
    PrefPassword = prefs.getString("password");
  if (prefs.isKey("citycode"))
    city_code = prefs.getString("citycode");
  prefs.end(); // 从nvs获取到wifi信息后，关闭Preferences

  if (city_code.length() != 9)
  {
    city_code = "101010100";
  }

  pinMode(21, INPUT);

  if (PrefSSID == "nano" || digitalRead(21) == LOW)
  {
    lv_label_set_text(ui_page4_label_set_mode_info, "已经进入配网模式\n请打开手机WiFi，搜索并连接ESPMonitorPlus++\n连接成功后会自动打开网页\n如未打开，请前往浏览器输入192.168.4.1即可");
    lv_disp_load_scr(ui_page4_screen_set_about);
    setWiFi();
  }

  uint8_t wifi_bar_value = 0;
  WiFi.mode(WIFI_MODE_STA); // 设置wifi的工作模式为：STA
  WiFi.begin(PrefSSID.c_str(), PrefPassword.c_str()); // 连接指定wifi
  while (WiFi.status() != WL_CONNECTED) // 等待连接成功
  {
    wifi_bar_value++; // 更新进度条
    lv_bar_set_value(ui_page1_Bar_loading, wifi_bar_value, LV_ANIM_OFF);
    lv_timer_handler();
    delay(100);
    // 判断wifi_bar_value是否大于100 判断超时
    if (wifi_bar_value > 100)
    {
      lv_label_set_text_fmt(ui_page1_Label_loading_text, "%s连接超时,请检查WIFI信息是否有误", PrefSSID);
      while (1)
      {
        lv_timer_handler();
      }
    }
  }

  lv_bar_set_value(ui_page1_Bar_loading, wifi_bar_value += 10, LV_ANIM_OFF);
  lv_label_set_text(ui_page1_Label_loading_text, "正在同步时间.....");
  lv_timer_handler();
  Udp.begin(localPort);
  setSyncProvider(getNtpTime);
  while (timeStatus() != timeSet) // 等待同步时间完成
  {
    setSyncProvider(getNtpTime);
  }
  setSyncInterval(300);
  delay(200);

  lv_bar_set_value(ui_page1_Bar_loading, wifi_bar_value += 10, LV_ANIM_OFF);
  lv_label_set_text(ui_page1_Label_loading_text, "正在自动定位.....");
  lv_timer_handler();
  getCityCode();
  delay(200);

  lv_bar_set_value(ui_page1_Bar_loading, wifi_bar_value += 10, LV_ANIM_OFF);
  lv_label_set_text(ui_page1_Label_loading_text, "正在获取天气信息.....");
  lv_timer_handler();
  getCurrentWeather();
  get7DaysWeather();
  delay(200);

  lv_bar_set_value(ui_page1_Bar_loading, wifi_bar_value += 10, LV_ANIM_OFF);
  lv_label_set_text(ui_page1_Label_loading_text, "正在获取黄历信息.....");
  lv_timer_handler();
  getChineseAlmanac();
  delay(200);

  // 加载完进度条
  while (wifi_bar_value <= 100)
  {
    wifi_bar_value++;
    lv_bar_set_value(ui_page1_Bar_loading, wifi_bar_value, LV_ANIM_OFF);
    lv_timer_handler();
    vTaskDelay(20);
  }

  lv_disp_load_scr(ui_page2_screen_weather);

  // 按钮事件绑定
  button21.attachClick(attachClick21);
  button21.attachDoubleClick(attachDoubleClick21);
  button21.attachDuringLongPress(attachDuringLongPress21);
}

// 用于刷新的计时变量
unsigned long update_currentweather_last;
unsigned long update_dht11_last;
unsigned long update_others_last;
unsigned long update_time_position_last;

void loop()
{
  getAida64Data();
  // 定时一秒更新一次温湿度
  if (millis() - update_dht11_last >= 1000)
  {
    update_dht11_last = millis();

    // 刷新温湿度DHT11数据
    updateDHTData();

    // 天气页面时间刷新
    lv_label_set_text_fmt(ui_page2_panel_main_label_time, "%s:%s", str_time_hour(), str_time_minute());

    // 主页面时间刷新
    lv_label_set_text_fmt(ui_page5_label_time, "%s:%s", str_time_hour(), str_time_minute());
    lv_label_set_text_fmt(ui_page5_label_date, "%d年%d月%d日", year(), month(), day());

    // monitor页面时间刷新
    lv_label_set_text_fmt(ui_page3_label_gpu_use2, "%s:%s", str_time_hour(), str_time_minute());
    lv_label_set_text_fmt(ui_page3_label_download_speed2, "%d年%d月%d日", year(), month(), day());
  }

  // // 定时1小时更新一次天气信息
  if (millis() - update_currentweather_last >= (update_times_current_weather * 60 * 1000))
  {
    update_currentweather_last = millis();
    getCurrentWeather();
  }

  // // 定时更新一次7天预报、黄历信息
  if (millis() - update_others_last >= (update_times_weather_forecast * 60 * 1000))
  {
    update_others_last = millis();
    get7DaysWeather();
    getChineseAlmanac();
  }

  // 定时更新 时间位置
  if (millis() - update_time_position_last >= 5000)
  {
    update_time_position_last = millis();
    int pos_x = random(50, 400);
    int pos_y = random(50, 200);

    lv_obj_set_x(ui_page5_label_time, pos_x);
    lv_obj_set_y(ui_page5_label_time, pos_y);

    lv_obj_set_x(ui_page5_label_date, pos_x + 5);
    lv_obj_set_y(ui_page5_label_date, pos_y + 45);
  }

  lv_timer_handler(); /* let the GUI do its work */
  delay(10);

  button21.tick();
}
