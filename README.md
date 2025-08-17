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

### Projectile & Airsoft Testing  
- Works best at **5–10 m**.  
- Piezo sensors generate up to **14V**, while ESP32 ADC max is **3.3V** (4095).  
- Tested with **SIG Sauer P226 MK25 GBB handgun** (~100 m/s muzzle velocity).  
- Reliable detection at 5 m distance.  

### Heatmap Analysis  
- A **grid test** (60×60 mm and 100×200 mm) was used to simulate projectile impacts on the vest pad.  
- **Heatmaps** were generated using Python visualization to show impact distribution:  
  - **Center areas** (closest to the sensor) produce the highest ADC readings.  
  - **Edges of the pad** show weaker signals, sometimes too low to be detected.  
  - Vibrations propagate to neighboring pads, but the **highest ADC value always corresponds to the direct hit pad**.  
- This proves SmartVest can **localize hits accurately** by identifying the strongest signal zone.  

*(Add images of heatmap here, e.g. 60x60 grid & 100x200 pad results)*  

### ESP-NOW Communication  
- Average latency: **1–2 ms**, with occasional spikes up to 45 ms due to interference.  
- Still fast and stable enough for **real-time weapon lock & feedback**.  

---

## 📂 Repository Contents  

- `firmware/` → ESP32 & ESP32-C3 source code  
- `hardware/` → PCB design files  
- `docs/` → diagrams, mockups, and test results  

---

## 📸 Prototype & Dashboard  

(Add your prototype photos, PCB screenshots, heatmap images, and monitoring UI here)  

---

## ⚡ Conclusion  

SmartVest integrates **real-time hit detection**, **automatic weapon lock**, and **centralized monitoring** into one system.  
Through **heatmap testing**, the system shows reliable hit localization and clear differentiation of impact zones.  
It enhances **fairness, sportmanship, and immersion** in airsoft games by combining IoT technology with game-like mechanics.  

---
