🌊 Real-Time Monitoring of Water Bodies

Keeping our lakes, rivers, and reservoirs healthy — one data point at a time.

🧭 Overview

MyYogna – Real-Time Monitoring of Water Bodies is an IoT-based project that tracks the quality and condition of natural water bodies in real time.
The system uses multiple environmental sensors (pH, TDS, turbidity, and flow) connected to a microcontroller (ESP32/Arduino).
The collected data is then sent to a web dashboard, allowing users to monitor live readings, detect pollution levels, and take preventive action before contamination spreads.

This project bridges hardware and software — combining sensors, IoT, and web technologies — to promote sustainable water management and early warning systems.

⚙️ Features

✅ Real-time sensor data collection (pH, TDS, Turbidity, Temperature, Flow)
✅ Wireless data transmission via ESP32
✅ Live monitoring dashboard (web-based visualization)
✅ Automatic alerts for abnormal readings
✅ Scalable for multiple water bodies and locations
✅ Open-source and easy to customize

🧩 Tech Stack
Layer	Technology Used
Microcontroller	ESP32 / Arduino UNO
Sensors	pH, TDS, Turbidity, Temperature, Water Flow
Connectivity	Wi-Fi (IoT data transfer)
Backend / Database	Firebase / ThingSpeak / Cloud Server
Frontend Dashboard	HTML, CSS, JavaScript (or React-based UI)
Display	LCD (16x2) for on-site values
Programming Language	C / C++ for Arduino, JavaScript for Web UI
🔄 Project Workflow

Here’s a simplified data flow of how everything works:

graph TD
A[💧 Water Body] --> B[📡 Sensors (pH, TDS, Turbidity, Temp)]
B --> C[🔌 Microcontroller (ESP32/Arduino)]
C --> D[🌐 Wi-Fi / IoT Module]
D --> E[(☁️ Cloud Database)]
E --> F[📊 Web Dashboard / Mobile App]
F --> G[⚠️ Alerts & Reports (Abnormal Values)]


Step-by-step workflow:

Sensors continuously measure parameters like pH, TDS, and turbidity.

Data is processed by the ESP32, which acts as the IoT node.

The device connects to Wi-Fi and transmits readings to the cloud database.

The web dashboard fetches and displays the latest readings visually.

If parameters go beyond thresholds, alerts are triggered for immediate attention.

🧠 Motivation

Water pollution is one of the most under-monitored threats to life and ecology.
I wanted to create something that empowers communities and researchers to understand water health in real time — without needing expensive lab equipment.
This project is my step towards a smarter, cleaner, and sustainable future 🌏.

🚀 How to Run

Clone the repository:

git clone https://github.com/your-username/realtime-monitoring-of-water-bodies.git
cd realtime-monitoring-of-water-bodies


Set up hardware:

Connect pH, TDS, Turbidity, Temperature, and Flow sensors to the ESP32.

Ensure Wi-Fi credentials are added to your code.

Upload the code to the ESP32 using the Arduino IDE.

Run the web dashboard:

Open the index.html or start the React app (if included).

View results:

Live readings appear on the LCD and in the online dashboard.

🧾 Expected Results

Live visualization of water quality data

Early detection of contamination

Secure cloud-based data logging

Potential integration with ML for pollution prediction (future scope)

🌱 Future Improvements

AI-based prediction of water quality trends

Integration with Google Maps for location tagging

SMS / Email alert system for critical values

Mobile app for remote access

💡 Key Learnings

Building this project taught me how to:

Interface multiple sensors with ESP32

Manage IoT data pipelines efficiently

Build a clean, real-time web dashboard

Work with cloud databases for live data monitoring

🤝 Credits

Built with 💙 by Yash Pandey
Under the guidance of [Your Faculty / Mentor Name]
Department of Computer Engineering, [Your College Name]

🪄 License

This project is open-source under the MIT License.
Feel free to use, modify, and build upon it — just give credit where due.
