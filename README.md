# Smart-IV-Monitoring-System-Using-IoT
In hospitals, administering Intravenous Fluids (IV) is common, but manual monitoring is challenging due to the rising patient-to-doctor ratio. Leveraging IoT technology and sensors, the system automates real-time IV fluid level monitoring, transmitting data to a mobile application through MQTT.

# Implementation
 Initially, the system 
is activated when a power supply is given to it. After configuring the NodeMCU to work in station mode, such that it connects to the local WIFI network and gets linked to the MQTT broker. The load cell measures the weight of the IV bottle, NodeMCU will then publish the fluid level to the MQTT broker. In the monitoring application, the medical personnel will set the level threshold, below which its considered critical. Whenever the IV fluid level of the patient is low, it will give the hospital personnel a buzzer alert. So, the personnel can replace the intravenous fluid. If the nurse fails to attend the patient immediately then an automatic clamp locking system is done which suppresses and flattens the saline tube. This prevents the upward flow of saline from the veins to the bottle. This system makes the work trouble-free for the hospitals. All the data from sensors is accessed using an open-source platform, Adafruit IO.

