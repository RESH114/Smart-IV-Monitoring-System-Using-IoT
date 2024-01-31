# Smart-IV-Monitoring-System-Using-IoT
Intravenous (IV) infusion therapy is a traditional medical procedure that is applied in 
all branches of medicine and involves the insertion of fluid directly into a patient’s vein using 
a needle/cannula. IV infusion therapy is especially used during operative and postoperative 
procedures for the placement of drugs directly into the patient’s bloodstream and for the 
transfusion of blood or some of its components. In addition, it is used to treat dehydration, 
correct electrolyte imbalances, and treat patients with cancer who have digestive system 
problems. Therefore, there is a 60–80% chance that a hospitalised patient may undergo 
intravenous infusion therapy of some kind.
Although the use of IV infusion is mostly painless, in some cases the procedure is 
extremely painful especially in oncology patients using cytostatic. While not all cytostatic are 
equally aggressive several months of intravenous cytostatic therapy leaves serious and 
permanent damage to the veins. After several cycles of IV chemotherapy, at some point there 
is no longer a vein on the patient’s hands that can withstand daily cytostatic therapy which is a 
nightmare and a serious psychological problem for almost all oncology patients. It is crucial to 
follow the IV chemotherapy drip schedule in order to get the maximum cytostatic impact and 
recovery of patients in the hospital.

Initial chemotherapy lasts four cycles shorter/longer, if necessary, with one cycle 
involving 5 days of active IV therapy with cytostatic, 5 IV bottles per day, which is a total of 
25 IV bottles during only one cycle. As a result, each subsequent cycle becomes more difficult 
for patients, not only due to exhaustion but also because the veins are no longer able to 
withstand all-day chemotherapy. The veins become weak and porous over time and quickly 
become inflamed, which is why daily therapy, instead of a few hours, lasts significantly longer. 
A solution for these and other problems would greatly encourage patients and thus increase the 
success of IV chemotherapy, which is our goal.


# Implementation
 Initially, the system 
is activated when a power supply is given to it. After configuring the NodeMCU to work in station mode, such that it connects to the local WIFI network and gets linked to the MQTT broker. The load cell measures the weight of the IV bottle, NodeMCU will then publish the fluid level to the MQTT broker. In the monitoring application, the medical personnel will set the level threshold, below which its considered critical. Whenever the IV fluid level of the patient is low, it will give the hospital personnel a buzzer alert. So, the personnel can replace the intravenous fluid. If the nurse fails to attend the patient immediately then an automatic clamp locking system is done which suppresses and flattens the saline tube. This prevents the upward flow of saline from the veins to the bottle. This system makes the work trouble-free for the hospitals. All the data from sensors is accessed using an open-source platform, Adafruit IO.

