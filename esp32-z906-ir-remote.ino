#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <IRremote.hpp>

// ============ WiFi 配置信息 =============
const char* ssid     = "你的wifi名称";
const char* password = "你的wifi密码";
// ========================================

// 红外发射引脚 (OUT→GPIO13、+5→5V、GND→GND)
#define IR_SEND_PIN 13

// 创建网页服务器
AsyncWebServer server(80);

// --------------------------
// 这里放置你整理好的 rawData 数组
// 每个按钮 68 个脉冲 (你串口显示就是68)；
// 我这里只给出示例结构，你需把“...省略”替换成 真实数字
// 例如 rawPower, rawInput, rawMute, etc.
// --------------------------
uint16_t rawPower[71] = {9014, 4510,  564, 568,  564, 1696,  566, 564,  566, 564,  566, 564,  566, 566,  564, 564,  566, 564,  566, 564,  542, 588,  568, 564,  566, 564,  566, 564,  566, 1696,  566, 566,  566, 1696,  540, 590,  566, 564,  566, 564,  566, 564,  566, 564,  564, 566,  566, 564,  568, 1694,  568, 1694,  566, 1696,  568, 1694,  540, 1722,  568, 1694,  566, 1696,  566, 1696,  566, 564,  566, 45392,  9012, 2272,  566
  // 参考开关按钮对应的 rawData
  // 把 +9000,-4450 => 9000,4450
  // 依次写完 68 个
  // ...
};

uint16_t rawInput[67] = {9010, 4516,  562, 568,  562, 1700,  562, 570,  562, 570,  560, 568,  562, 568,  562, 568,  562, 568,  564, 568,  562, 568,  562, 570,  558, 572,  562, 570,  562, 1700,  560, 570,  562, 1700,  562, 570,  562, 568,  564, 586,  544, 1700,  562, 568,  562, 568,  562, 568,  564, 568,  560, 1702,  562, 1700,  562, 1702,  562, 568,  562, 1700,  562, 1700,  562, 1716,  546, 1716,  546
  // 依次填入 "输入" 按钮
  // ...
};

uint16_t rawMute[71] = {8980, 4540,  514, 616,  626, 1636,  534, 596,  652, 478,  622, 510,  532, 596,  562, 566,  562, 568,  536, 594,  562, 570,  534, 594,  536, 596,  534, 594,  564, 1700,  560, 570,  564, 1698,  624, 508,  536, 1726,  624, 506,  534, 1726,  650, 480,  648, 1612,  536, 1726,  626, 1636,  566, 1696,  626, 506,  560, 1700,  560, 570,  512, 1750,  560, 568,  534, 596,  532, 598,  536, 45420,  9098, 2186,  536
  // "静音"
  // ...
};

uint16_t rawVolDown[71] = {8984, 4534,  542, 588,  542, 1720,  542, 588,  542, 590,  540, 612,  518, 588,  542, 588,  542, 588,  542, 588,  540, 590,  542, 588,  542, 588,  542, 590,  540, 1720,  542, 590,  540, 1720,  542, 588,  542, 1720,  542, 588,  542, 1720,  542, 588,  542, 1720,  542, 1720,  542, 588,  540, 1720,  542, 588,  542, 1720,  542, 586,  542, 1718,  544, 586,  544, 586,  546, 1716,  600, 45352,  8988, 2294,  662
  // "音量-"
  // ...
};

uint16_t rawVolUp[71] = {8994, 4528,  574, 558,  544, 1716,  546, 584,  546, 584,  546, 584,  546, 584,  546, 584,  546, 584,  572, 558,  546, 584,  546, 584,  546, 584,  546, 584,  546, 1714,  546, 584,  548, 1714,  548, 584,  548, 1714,  546, 584,  546, 1716,  546, 608,  522, 1716,  546, 584,  546, 1716,  546, 1716,  546, 608,  524, 1714,  548, 584,  546, 1714,  548, 584,  546, 1716,  546, 586,  544, 45410,  9042, 2244,  602
  // "音量+"
  // ...
};

uint16_t rawChannel[67] = {9070, 4460,  618, 514,  648, 1614,  620, 512,  620, 512,  650, 482,  648, 482,  648, 482,  620, 512,  650, 480,  620, 512,  620, 510,  620, 512,  622, 508,  620, 1644,  650, 482,  620, 1644,  620, 512,  618, 1644,  622, 512,  678, 1584,  618, 512,  650, 482,  646, 484,  620, 510,  648, 1614,  652, 478,  646, 1618,  646, 484,  620, 1642,  618, 1644,  650, 1612,  620, 1644,  650
  // "声道调节"
  // ...
};

uint16_t rawEffect[71] = {9004, 4538,  544, 586,  546, 1716,  544, 586,  544, 584,  544, 586,  544, 586,  544, 586,  544, 586,  544, 588,  542, 586,  544, 586,  544, 586,  544, 586,  546, 1716,  544, 586,  544, 1718,  544, 584,  544, 1718,  546, 1716,  546, 1716,  544, 586,  546, 584,  546, 586,  544, 586,  546, 1716,  544, 586,  544, 584,  546, 586,  544, 1718,  544, 1716,  546, 1716,  544, 1718,  544, 45412,  9004, 2280,  556
  // "音效模式"
  // ...
};

// 按键映射结构
struct Z906Button {
  const char* name;             // 按键中文
  const uint16_t* rawData;      // 指向那 68 个脉冲数组
};

// 7 个按钮条目
Z906Button z906Buttons[] = {
  {"开关",       rawPower},
  {"输入",       rawInput},
  {"静音",       rawMute},
  {"音量-",      rawVolDown},
  {"音量+",      rawVolUp},
  {"声道调节",   rawChannel},
  {"音效模式",   rawEffect}
};
const int NUM_BUTTONS = sizeof(z906Buttons)/sizeof(z906Buttons[0]);

void setup() {
  Serial.begin(115200);
  delay(1000);

  // 连接WiFi
  WiFi.begin(ssid, password);
  Serial.print("正在连接WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi连接成功！");
  Serial.print("在浏览器访问: http://");
  Serial.println(WiFi.localIP());

  // 初始化红外发射
  IrSender.begin(IR_SEND_PIN);

  // 首页按钮
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
  String html = R"rawliteral(
  <!DOCTYPE html>
  <html lang="zh">
  <head>
    <meta charset="UTF-8">
    <title>Z906 遥控</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <style>
      :root {
        --bg: #f9f9f9;
        --text: #111;
        --card: #ffffff;
        --btn: #007aff;
        --btn-text: #ffffff;
      }
      @media (prefers-color-scheme: dark) {
        :root {
          --bg: #1c1c1e;
          --text: #f2f2f2;
          --card: #2c2c2e;
          --btn: #0a84ff;
          --btn-text: #ffffff;
        }
      }

      body {
        background: var(--bg);
        color: var(--text);
        font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, "Helvetica Neue", sans-serif;
        margin: 0;
        padding: 2em 1em;
        text-align: center;
      }

      h2 {
        font-weight: 600;
        margin-bottom: 1.5em;
      }

      .grid {
        display: grid;
        grid-template-columns: 1fr 1fr;
        gap: 1em;
        max-width: 420px;
        margin: 0 auto;
      }

      .full {
        grid-column: span 2;
      }

      .vol-row {
        display: flex;
        justify-content: center;
        gap: 1em;
        margin: 1em auto;
      }

      .vol-btn {
        flex: 1;
        padding: 1.2em;
        font-size: 24px;
        border-radius: 14px;
        background: var(--btn);
        color: var(--btn-text);
        border: none;
        box-shadow: 0 4px 12px rgba(0,0,0,0.15);
        transition: all 0.2s ease;
      }

      .vol-btn:active {
        transform: scale(0.96);
      }

      .btn {
        padding: 1em;
        font-size: 18px;
        border-radius: 12px;
        background: var(--btn);
        color: var(--btn-text);
        border: none;
        box-shadow: 0 4px 10px rgba(0,0,0,0.12);
        transition: all 0.2s ease;
      }

      .btn:active {
        transform: scale(0.97);
      }

      .label {
        display: block;
        font-size: 14px;
        margin-top: 4px;
      }
    </style>
  </head>
  <body>
    <h2>Logitech Z906</h2>

    <div class="grid">
      <button class="btn" onclick="send(0)">🕹️<span class="label">开关</span></button>
      <button class="btn" onclick="send(1)">🔀<span class="label">输入</span></button>
      <button class="btn full" onclick="send(2)">🔇 静音</button>
    </div>

    <div class="vol-row">
      <button class="vol-btn" onclick="send(3)">🔉 音量 -</button>
      <button class="vol-btn" onclick="send(4)">🔊 音量 +</button>
    </div>

    <div class="grid">
      <button class="btn" onclick="send(5)">🎛️<span class="label">声道</span></button>
      <button class="btn" onclick="send(6)">🎵<span class="label">音效</span></button>
    </div>

    <script>
      function send(id) {
        fetch(`/send?btn=${id}`)
          .then(() => {
            console.log("发送成功：" + id);
          })
          .catch(err => {
            alert("发送失败: " + err);
          });
      }
    </script>
  </body>
  </html>
  )rawliteral";

  request->send(200, "text/html; charset=utf-8", html);
});





  // 处理 /send?btn=xxx
  server.on("/send", HTTP_GET, [](AsyncWebServerRequest *request) {
    if(request->hasParam("btn")){
      int index = request->getParam("btn")->value().toInt();
      if(index >= 0 && index < NUM_BUTTONS){
        Serial.print("发送 Raw 波形: ");
        Serial.println(z906Buttons[index].name);

        // 多次重复发送 (这里发3次, 你可酌情更改)
        for(int i=0; i<1; i++){
          // 发送 raw, 第3参数=载波频率, 大多数是38
          IrSender.sendRaw(
            z906Buttons[index].rawData, 
            68, // length
            38  // kHz
          );
          delay(40); // 间隔
        }
        String msg = String(z906Buttons[index].name) + " Raw 指令已发送(多次)";
        request->send(200, "text/plain; charset=utf-8", msg);
        return;
      }
    }
    request->send(400, "text/plain; charset=utf-8", "参数错误");
  });

  server.begin();
}

void loop() {
  // AsyncWebServer 不需要在此处理
}
