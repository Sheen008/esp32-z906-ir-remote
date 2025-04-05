# ESP32-Z906 红外遥控网页控制器

一个使用 **ESP32 + 红外发射模块 + 网页控制界面** 的项目，实现对 **Logitech Z906 音响系统** 的远程控制。

通过 Wi-Fi 接入后，用户可通过浏览器访问设备并控制音响电源、音量、输入源、静音等常用功能。

## 📷 项目演示

## ✨ 特性

- 📡 **ESP32 原生 Wi-Fi 模块**，无需额外服务器
- 🌐 **网页控制界面**，适配手机竖屏浏览体验
- 🔘 **7 个音响控制按钮**，包括音量、电源、静音、输入源等
- 💡 **使用 Raw Data 精准红外控制**
- 📱 **支持多个客户端同时访问**
- 🚀 可拓展用于控制其他红外设备

## 🔧 硬件连接

| 模块 | 接线说明 |
|------|----------|
| 红外发射器 OUT | 接 ESP32 GPIO13 |
| 红外发射器 VCC | 接 ESP32 5V |
| 红外发射器 GND | 接 ESP32 GND |

## 📦 所需库

请确保以下库已通过 Arduino Library Manager 或手动安装：

- [`ESPAsyncWebServer`](https://github.com/me-no-dev/ESPAsyncWebServer)
- [`AsyncTCP`](https://github.com/me-no-dev/AsyncTCP)
- [`IRremote`](https://github.com/Arduino-IRremote/Arduino-IRremote)（版本 4.4.1 推荐）
- ESP32 Arduino Core (推荐使用 2.0.11)

## 🚀 快速开始

1. 将本项目的 `.ino` 文件上传至你的 ESP32；
2. 修改代码中的 WiFi 名称与密码；
3. 连接后串口监视器将显示设备 IP；
4. 在同一网络下浏览器访问 IP，即可控制音响设备。

## 🌈 支持的按键功能

| 按钮名称 | 功能说明 |
|----------|----------|
| 开关     | 开/关音响 |
| 输入     | 切换输入源 |
| 静音     | 静音切换 |
| 音量+    | 增加音量 |
| 音量-    | 减小音量 |
| 声道调节 | 切换音频通道模式 |
| 音效模式 | 切换音响音效 EQ 模式 |

## 💻 使用技术

- ESP32
- Arduino IDE
- HTML + CSS + JS（嵌入式网页）
- 红外 Raw 波形控制

## 🌐 集成 Home Assistant（可选）

可通过 ESPHome 或 MQTT 网关将该项目整合至 Home Assistant，实现自动化联动（如“开电视同步开音响”等场景）。

## 🙏 致谢

本项目部分红外波形数据（`rawData[]`）参考自以下优秀开源项目，在此感谢作者分享：

- [sakai-labo / Logitech-Z906-IR-remote-code-for-Arduino-](https://github.com/sakai-labo/Logitech-Z906-IR-remote-code-for-Arduino-/tree/main)

---

欢迎修改、美化或拓展本项目。如你实现了对其他品牌音响/设备的适配，也欢迎 PR 与分享！

---
