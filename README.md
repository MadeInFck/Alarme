# Alarme
###MyHome app Alarm
Aim is to design from stard to end a whole alarm system with small devices I can add to the main management device. This way, I'll be able to set several movement detectors at home and be warned when something happens. The alarm manager needs to send notification to MyHomeApp as well whenever that happens.

**04/07/16 :**  
First commit. Code for transmitter and receiver. This sketch uses a PIR sensor connected with Arduino Nano.

  [Check this article on my site!](http://www.madeinfck.com/?p=169)
  
**14/01/17 :**
Tried to reduce the consumption of the device using this sketch by making the processor go to sleep when unused. An interruption on pin 2 wakes it up and does the job. Fully fonctionnal, test with the same receptor. Just changed the message transferred by the emitter to standardize the messages that will be received.

Bottom part of the detector case added. 1st try to expect for 3D printing this model.

**04/02/17 :**
Firebase notification and push to database implemented. Notification sent to phone and received by MyHome app.

#More to come :
Design upper part of the detector's case.
Design case for main manager. Add a touch TFT screen to implement menu ?
