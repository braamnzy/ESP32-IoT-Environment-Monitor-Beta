# 🌡 ESP32 IoT Environmental Monitor (Beta v0.2)

Firmware IoT berbasis **ESP32** untuk membaca:

* Temperatur
* Kelembaban
* Kualitas udara (tegangan sensor gas)

Data dikirim ke server melalui HTTP POST dalam format JSON.

Project ini ditujukan sebagai firmware dasar yang dapat dikembangkan lebih lanjut untuk sistem monitoring lingkungan berbasis IoT.

---

## 📌 Features

* Non-blocking sampling (millis-based)
* Auto WiFi reconnect
* HTTP timeout protection
* Correct ADC scaling (ESP32 12-bit)
* JSON data transmission
* Modular configuration (sampling interval & timeout)

---

## 🧰 Hardware Requirements

* 1x **ESP32**
* 1x **DHT11**
* 1x **MQ-135**
* Jumper wires
* Breadboard
* Power supply 5V / USB

---

## 🔌 Pin Configuration

| Sensor | ESP32 Pin |
| ------ | --------- |
| DHT11  | GPIO 4    |
| MQ-135 | GPIO 34   |

> Pastikan MQ-135 terhubung ke pin ADC (GPIO 34/35/32/33).

---

## 📡 Data Format (JSON)

Firmware mengirim data dalam format berikut:

```json
{
  "temperature": 27.35,
  "humidity": 65.20,
  "air_quality": 1.82
}
```

* `temperature` → Celsius
* `humidity` → %
* `air_quality` → Tegangan ADC (Volt)

---

## 🌐 Server Requirement

Contoh endpoint server (Flask):

```python
@app.route('/receive_sensor', methods=['POST'])
def receive_sensor():
    data = request.get_json()
    print(data)
    return {"status": "ok"}, 200
```

Server harus menerima:

```
POST /receive_sensor
Content-Type: application/json
```

---

## ⚙ Configuration

Ubah bagian berikut pada firmware:

```cpp
const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";
String serverURL = "http://YOUR_SERVER_IP:5000/receive_sensor";
```

Parameter penting:

```cpp
#define SAMPLE_INTERVAL 120000   // interval pengiriman (ms)
#define HTTP_TIMEOUT 5000        // timeout HTTP (ms)
```

---

## 🧠 System Architecture

```
[DHT11 + MQ135]
        ↓
     ESP32
        ↓ (HTTP POST)
     Web Server
        ↓
   Data Processing / Database
```

Firmware ini hanya bertugas sebagai data acquisition node.

---

## 🚀 How to Use

1. Install Arduino IDE
2. Install ESP32 Board Manager
3. Install library:

   * WiFi
   * HTTPClient
   * DHT sensor library
4. Upload firmware ke ESP32
5. Pastikan server aktif
6. Monitor output melalui Serial Monitor (115200 baud)

---

## ⚠ Current Limitations (Beta)

* Masih menggunakan HTTP (belum HTTPS)
* Belum ada API authentication
* Belum ada sensor calibration (MQ-135)
* Konfigurasi masih hardcoded
* Belum ada OTA update

---

## 🛠 Planned Improvements

* API key authentication
* HTTPS support
* Moving average filter untuk MQ-135
* Config via web interface
* OTA firmware update
* Data buffering saat server down

---

## 📄 Version

v0.2 – Stable Beta
Non-blocking system + WiFi reconnect + Correct ADC scaling

---

## 📜 License

MIT License (Recommended for open-source IoT firmware)

---

## 👨‍💻 Author

Developed as an IoT environmental monitoring firmware for modular use and further development.


