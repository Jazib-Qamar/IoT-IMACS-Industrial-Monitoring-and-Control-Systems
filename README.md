# IoT-Based Industrial Monitoring and Control System 🏭

This project, developed as our Final Year Project (FYP) at PIEAS, is an **IoT-based Industrial Monitoring and Control System**. The system includes three tanks: two upper supporting tanks and one lower monitoring tank. The supporting tanks hold chemicals, which are pumped into the monitoring tank using pumps controlled by relays ⚙️, all interfaced with an ESP8266 microcontroller 📡.

Real-time monitoring 🕒 is achieved through strategically placed sensors, including:
- pH sensors
- Air quality sensors
- Temperature and humidity sensors
- Ultrasonic sensors
- RGB color sensor 📊

These sensors automate the mixing process based on real-time feedback 📈. For example, when the pH and RGB sensors detect the desired chemical levels, the system halts the process ⛔ and safely discharges the mixture 🚰.

This system showcases the practical application of **Industry 4.0** 🏗️ and **Industrial IoT (IIoT)** 🌐 principles, improving automation, reducing human intervention, and ensuring energy efficiency ⚡. With IoT technology, it represents a modern approach to solving industrial challenges, making processes more efficient and reliable.

---

## Installation

Follow these steps to set up the system:

### 1. **Install Arduino IDE**
   - Download and install the [Arduino IDE](https://www.arduino.cc/en/software).
   - Open the Arduino IDE, and go to **File > Preferences**.
   - Add the ESP8266 board manager URL in **Additional Boards Manager URLs**:  
     `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
   - Install the ESP8266 board by going to **Tools > Board > Boards Manager**, search for **ESP8266** and click **Install**.

### 2. **Install Necessary Libraries**
   - **ESP8266WiFi.h**: This library is used to connect your ESP8266 to WiFi.
     - Install by going to **Sketch > Include Library > Manage Libraries**, search for **ESP8266WiFi** and click **Install**.
   - **RTOS**: Install the **FreeRTOS** library to enable real-time operating system features.
     - Install by going to **Sketch > Include Library > Manage Libraries**, search for **FreeRTOS** and click **Install**.

### 3. **Install Node-RED**
   - Install Node-RED by following the instructions [here](https://nodered.org/docs/getting-started/).
   - Node-RED helps in creating flow-based programming for monitoring and controlling IoT devices.

### 4. **Upload Code to ESP8266**
   - After installing the necessary libraries, connect your ESP8266 to your PC via USB.
   - Select the correct board and port in **Tools > Board** and **Tools > Port**.
   - Upload the code for the Industrial Monitoring system.

### 5. **Configure Node-RED**
   - Launch Node-RED and configure flows to interface with the ESP8266 for real-time data processing.
   - Use appropriate nodes in Node-RED to display data and control relays for the system's operation.

---

## Usage

- Once the setup is complete, the ESP8266 will connect to the WiFi and start collecting data from sensors.
- The data will be sent to Node-RED for processing and visualization in real time.
- The pumps in the supporting tanks are controlled based on the sensor readings (e.g., pH, RGB, etc.), and the system will halt or continue the process depending on the detected values.
- Use the Node-RED interface to monitor and control the process remotely.

---

## Results

This system showcases real-time automation and control in industrial environments. The key results are:

- **Efficient Automation**: The system reduces human intervention by automating the mixing process based on real-time sensor feedback.
- **Real-time Monitoring**: Continuous tracking of chemical levels, air quality, and environmental conditions ensures that the process is always within safe operating parameters.
- **Energy Efficiency**: The use of IoT technology minimizes energy consumption by controlling pumps and relays only when necessary, optimizing power usage.
- **Industry 4.0 Implementation**: The system integrates IoT technologies to enhance industrial processes, making them more efficient and reliable.
- **Reduced Operational Risks**: Automatic stopping of processes when chemical levels are reached, ensuring worker safety and process accuracy.

---

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
