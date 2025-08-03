# IOT-Biometrics-Attendance-System

📌 Project Overview
This is an IoT-based Biometrics Attendance System developed using the ESP32 Dev Module, Fingerprint Sensor, RFID Reader, and a TFT ILI9341 Display. It allows users to mark their attendance through either fingerprint authentication or an RFID card, and logs the data directly into a Google Sheet in real-time via the internet.

The goal is to offer a simple, reliable, and remotely accessible attendance solution for schools, offices, or any environment where efficient tracking is needed. The attendance log is stored in a Google Sheet, allowing the admin to monitor records anytime, from anywhere.

🔧 Features :
📶 IoT-Enabled: Connected to the internet for live attendance syncing.
🛂 Dual Authentication: Accepts both fingerprint and RFID for flexible user input.
🧠 Real-time Logging: Automatically updates attendance data to Google Sheets.
🌐 Remote Access: Admin can access the attendance records from any location using Google Sheets.
💾 Non-volatile Memory: Stores and recognizes users even after a reboot (can be extended with EEPROM or SPIFFS).
🔒 Secure: Each entry is linked to a unique fingerprint ID or RFID UID.
🖥️ Color Display: Uses a TFT ILI9341 screen to show user feedback, system status, and attendance confirmation.

🧰 Hardware Components
- ESP32 Dev Module --------->	Main microcontroller with WiFi
- R305 / AS608 Fingerprint Sensor--------->	Biometric scanner for fingerprint input
- RC522 RFID Module--------->	RFID reader for card scanning
- TFT ILI9341---------> Display	Color screen for system feedback
- Breadboard & Jumper Wires--------->	Prototyping and connections
- Power Supply--------->	5V for stable operation

💻 Software Tools
- Arduino IDE (for programming the ESP32)
- Google Apps Script (to connect Google Sheets to ESP32)
- ESP32 WiFi Library
- TFT_eSPI (for ILI9341 TFT)
- Adafruit Fingerprint Library
- MFRC522 RFID Library
- HTTPSRedirect Library (for Google Sheets integration)

🚀 How It Works
- User authenticates via fingerprint or RFID card.
- ESP32 reads the input and verifies the user.
- If valid, the attendance log is created with:
    * User ID
    * Authentication type (Fingerprint/RFID)
    * International Date & Time
- The data is sent securely to a Google Apps Script Web App endpoint.
- The Google Sheet is updated in real-time with each entry.

📂 Google Sheets Integration
- To store attendance data:
- Create a new Google Sheet.
- Go to Extensions → Apps Script and paste your Google Apps Script (you can find an example in this repo).
- Deploy the script as a Web App with access set to Anyone.
- Copy the Web App URL and paste it in your ESP32 sketch.

✅ Future Improvements
- Add support for user registration through web or app interface.
- Integrate facial recognition.
- Display logs in a custom web dashboard.

