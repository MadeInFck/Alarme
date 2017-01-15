# Alarme
###MyHome app Alarm

**04/07/16 :**  
First commit. Code for transmitter and receiver. This sketch uses a PIR sensor connected with Arduino Nano.

  [Check this article on my site!](http://www.madeinfck.com/?p=169)
  
**14/01/17 :**
Tried to reduce the consumption of the device using this sketch by making the processor go to sleep when unused. An interruption on pin 2 wakes it up and does the job. Fully fonctionnal, test with the same receptor. Just changed the message transferred by the emitter to standardize the messages that will be received.

#More to come :
Aim is to design from stard to end a whole alarm system with small devices I can add to the main management device. This way, I'll be able to set several movement detectors at home and be warned when something happens. The alarm manager will need to send notification to MyHomeApp as well whenever that happens.
