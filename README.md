# 🎯 SmartVest - Airsoft Hit Detection System  

**SmartVest** is a smart wearable system designed for airsoft games and tactical training scenarios.  
It provides **real-time hit detection** by sensing BB pellet or rubber projectile impacts through an array of pressure and vibration sensors.  
Inspired by video game mechanics, SmartVest delivers immersive and measurable training by mapping impact zones and giving instant feedback to the player or trainer.  

---

## 🧩 Project Mockup  

Here's a visual mockup of the SmartVest system design:  

![SmartVest Design Mockup](Asset/Mockup.png)  

---

## 💡 What Does It Do?  

SmartVest detects and classifies hits from projectiles (BBs or rubber bullets) on a player’s body using multiple force and vibration sensors embedded within a vest. It is especially designed to:  

- **Track and identify projectile impacts**  
- **Map hit locations across multiple zones**  
- **Differentiate real hits from fake or foreign object contacts**  
- **Deliver immediate feedback (visual/audio)**  
- **Log and transmit hit data to a central server or controller**  

---

## 🚀 Features  

- 🔹 **Multi-zone sensor detection** across the vest  
- 🔹 **Impact classification** (projectile vs. non-projectile)  
- 🔹 **Wireless communication options**:  
  - **ESP-NOW** → ultra-fast (1–2 ms latency) between vest & gun  
  - **Wi-Fi** → real-time logging to server/dashboard  
  - **LoRa (optional)** → long-range outdoor operations  
- 🔹 **Customizable & modular system** → supports different body sizes, teams, and modes  
- 🔹 **Game-style feedback** → LEDs, buzzer, vibration  
- 🔹 **Weapon lock system** → disables gun when player is “eliminated”  

---

## ⚙️ System Architecture  

- **Vest Module (ESP32)**  
  Processes sensor data, tracks health, and gives feedback via buzzer, LEDs, or vibration.  

- **Weapon Module (ESP32-C3 Mini + Servo)**  
  Automatic lock/unlock mechanism to prevent eliminated players from shooting.  

- **Monitoring Dashboard (Server/Raspberry Pi)**  
  Displays real-time player health, team stats, and hit locations.  

---

## 🧪 Testing & Results  

- **Projectile Testing**  
  - Works best at **5–10 m**.  
  - Piezo sensors generate up to **14V**, while ESP32 ADC max is **3.3V** (4095).  

- **Airsoft GBB Test**  
  - Tested with SIG Sauer P226 MK25 (GBB).  
  - Velocity ≈ **100 m/s**, detected reliably at 5 m.  

- **Heatmap Analysis**  
  - Strongest detection near sensor, weaker at pad edges.  
  - Vibrations spread, but direct hit always produces the highest ADC value.  

- **ESP-NOW Communication**  
  - Average latency: **1–2 ms**, occasional spikes up to 45 ms (due to interference).  
  - Stable and responsive for real-time weapon lock.  

---

## 📂 Repository Contents  

- `firmware/` → ESP32 & ESP32-C3 source code  
- `hardware/` → PCB design files  
- `docs/` → diagrams, mockups, and test results  

---

## 📸 Prototype & Dashboard  

(Add your prototype photos, PCB screenshots, and monitoring UI here)  

---

## ⚡ Conclusion  

SmartVest integrates **real-time hit detection**, **automatic weapon lock**, and **centralized monitoring** into one system.  
It enhances **fairness, sportmanship, and immersion** in airsoft games by combining IoT technology with game-like mechanics.  

---
