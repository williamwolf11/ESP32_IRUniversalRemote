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
-IR Receiver TSOP38238: receive IR signals so that users can program Bluetooth app buttons\
-BluefruitConnect App (iOS and Android): app for programming and commanding the remote

Code Logic:\
Our code uses some interesting logic to give our remote the functionality we desire. We programmed the remote essentially as a finite state machine, with two main states: send mode and remap mode. Remap mode is triggered by the user typing "remap" into the UART input on BluefruitConnect and, if the user presses one of the buttons on the BluefruitConnect controller while the device is in this mode, the main loop of our code is then set up to decode and assign the next IR signal it receives to the button the user previously pressed. Upon receiving the UART input "cancel" in the BluefruitConnect app, the remote exits Remap Mode and returns to send mode. In send mode, the main loop continuously polls the BluefruitConnect controller buttons and, upon receiving a button press, the assigned signal is sent out of our IR LED on the remote. An interesting thing about our code is that, while it technically uses polling, pressing the buttons on the controller sends a UART signal that we are able to detect by checking the first four characters of it, which are "!BXX", with the two X's representing integers that correspond to the number of the button (1-8) and whether it is being pressed down or released (1/0), respectively. When our program receives a UART signal of that nature in send mode, for example, we then assign the value of our integer variable buttonPressed to the corresponding button's number, which is where polling comes in: we have an if statement in our main loop that checks for buttonPressed to be anything but 0 in send mode, and when buttonPressed is not 0, we send out the signal once and then immediately set buttonPressed back to 0.

User Interface:\
Our remote does not have a user interface -- rather, the user interface of our project is simply the BluefruitConnect app's interface. The user will turn Bluetooth on on their phone, and upon opening the app will see our device on the list. They will then press connect and, as previously mentioned, type "remap" into the UART input, press the back arrow, and then press the controller button to choose which button to remap. They will then press the desired button and subsequently point their remote (whether it be a TV, light, or AC remote) at our remote and press the button on their remote that corresponds to the desired functionality of the button they are remapping on BluefruitConnect. The user can repeat this process (press BluefruitConnect controller button, send IR signal to remote) as many times as they'd like before going back to the UART input screen and typing "cancel" to end Remap Mode. At this point, all the user needs to do to use our remote is position it near the devices it will be communicating with and press the reprogrammed BluefruitConnect controller buttons whenever they need to use their devices' functionalities!

Bill of Materials:\
5mm IR LED - 25 Pack - $7.95 - https://www.adafruit.com/product/388 \
IR Receiver Sensor - $1.95 - https://www.adafruit.com/product/157 \
Full Sized Breadboard - $5.95	- https://www.adafruit.com/product/239 \
Premium Silicone Covered Male-Male Jumper Wires	- $9.95	- https://www.adafruit.com/product/4482 \
Lithium Ion Battery - 3.7V 350mAH - $6.95	- https://www.adafruit.com/product/2750

# Result:

<img src="https://https://github.com/williamwolf11/ESP32_IRUniversalRemote/blob/main/ESP32_IRUniversalRemote/controller.jpeg">

# Accessibility:

# Ethical Implications:

# Schedule:

# Issues:

# Future Work:

# References:

https://crankyoldgit.github.io/IRremoteESP8266/doxygen/html/ \
https://github.com/Arduino-IRremote/Arduino-IRremote \
https://github.com/crankyoldgit/IRremoteESP8266/blob/v2.8.0/examples/IRsendDemo/IRsendDemo.ino \
https://www.maxphi.com/ir-remote-control-decoder-using-arduino
