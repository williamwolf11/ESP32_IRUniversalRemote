# ESP32_IRUniversalRemote
by Will Wolf and Sammy Van Cleve


Code for user-programmable (via the BluefruitConnect app) Universal Remote that communicates via IR signals.

# Introduction:

The goal of this project was for us to apply what we had learned throughout the semester in our Embedded Systems class
to a hands-on project that has relevance to today's world of technology. The idea we landed on was an infrared (IR), user-programmable
universal remote that could be controlled via Bluetooth. We chose this idea for a few reasons, ranging from personal relevance
(we both love simplicity in controlling our entertainment setups) to accessibility (stripping away complex layers of modern technology for the general population) to challenging ourselves, as neither of us had worked with IR before. What followed was an emotional rollercoaster of victories and challenges that culminated in one last hurrah as we witnessed Sammy programming his IR devices in full force to be controlled by this remote and its accompanying Bluetooth app.

Listed below are some sources we cited during our research and development (also available in the References section):


https://crankyoldgit.github.io/IRremoteESP8266/doxygen/html/ \
https://github.com/Arduino-IRremote/Arduino-IRremote \
https://github.com/crankyoldgit/IRremoteESP8266/blob/v2.8.0/examples/IRsendDemo/IRsendDemo.ino \
https://www.maxphi.com/ir-remote-control-decoder-using-arduino

# Methods:

Devices Used (with rationale):\
-ESP32: provide Bluetooth support\
-Rechargeable battery: allow device to function away from a computer\
-5mm IR LED: send IR signals to user's devices\
-4x 560 Ω resistors: strengthen IR signal that we send out\
-IR Receiver TSOP38238: receive IR signals so that users can program Bluetooth app buttons

Code Logic:\
-logic for code

User Interface:\
-stuff

Bill of Materials:\
-bill of mats

# References:

https://crankyoldgit.github.io/IRremoteESP8266/doxygen/html/ \
https://github.com/Arduino-IRremote/Arduino-IRremote \
https://github.com/crankyoldgit/IRremoteESP8266/blob/v2.8.0/examples/IRsendDemo/IRsendDemo.ino \
https://www.maxphi.com/ir-remote-control-decoder-using-arduino
