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

Below are some pictures of the remote and BluefruitConnect app with accompanying instructions for how to use it.

This first picture shows a general view of the remote:\
![Test Image 1](/ESP32_IRUniversalRemote/sammy1.jpeg)

Next up, we've got a picture of the IR LED, which is used to send out user-programmed IR signals when the user presses the appropriate button on the BluefruitConnect app:\
![Test Image 2](/ESP32_IRUniversalRemote/sammy2.jpeg)

This is the IR receiver that receives the IR signals from the user's devices in Remap Mode to make the remote user-programmable:\
![Test Image 3](/ESP32_IRUniversalRemote/sammy3.jpeg)

This is the UART terminal on the BluefruitConnect app, into which the user can type "remap" and "cancel" to enter and exit Remap Mode, respectively:\
![Test Image 4](/ESP32_IRUniversalRemote/uart.jpeg)

And finally, this is the user interface of the controller buttons on the BluefruitConnect app. In Remap Mode, the user will press a singular button and subsequently send an IR signal to the remote (by pointing their device's remote at our remote and pressing the desired button on their remote) to remap that specific button. In send mode, the user will simply press any button and the assigned IR signal will be immediately sent out of the IR LED on the remote!\
![Test Image 5](/ESP32_IRUniversalRemote/controller.jpeg)


# Accessibility:

The main accessibility victory for our project is that, by its nature, our remote can help the various groups of people in the world who are overwhelmed by the many devices and remotes often necessary to set up modern entertainment systems. Not only that but, because it is user-programmable, a user could theoretically verbally instruct someone to help them program the remote, after which all they would need to do is open the BluefruitConnect app, connect to our device, press "controller," and press their desired buttons as needed. Additionally, the low-cost nature of our project allows this device to be accessible in an economic way. Possibilities for additional accessibility are detailed in the "Future Work" section below.

# Ethical Implications:

The biggest ethical implication/issue of our remote can best be described by the following question: do we really need more remotes? Whereas this type of remote/device can make modern entertainment setups much more accessible for less tech-savvy users, which is a positive outcome, the origin of this problem in the first place is that people often own several devices with their own remotes that can be confusing to use in parallel. Unfortunately, because of this, adding another remote into the picture (which uses plastic and other potentially environmentally harmful resources) is not particularly environmentally conscious. Additionally, most modern TVs, for example, are "Smart TVs" which can perform many different functions that previously required multiple devices with potentially multiple remotes. So, as these TVs improve in their own user interfaces, perhaps there will no longer be a need for universal remotes down the road, as people will be able to do everything they need with just a TV and one remote, or a TV and one or two other devices. 

# Schedule:

Our initial, planned schedule was as follows:

Week 9 - Recognize and read IR signals - we will quantify/test this milestone by having our signals print unique and appropriate output into the Serial Port. Successfully send out a simple, one-button command to the TV. We will start with an on/off button, and we will test this by making sure it works on both Will's TV AND Sammy’s TV.

Week 10 - Send out a series of signals with proper timing, and start to work on bluetooth.

Week 11 - Allow the user to program the remote with a sequence of signals of their choosing and finish bluetooth work.

Week 12 - Finish up and debug - final touches.

Our actual schedule is detailed below:

Week 9 - Order parts.

Week 10 - Parts arrive, Thanksgiving break (no work done).

Week 11 - Build remote, send out simple on/off signal from our remote to Sammy's TV.

Week 12 - Implement Bluetooth, program all relevant logic, debug/test.

Ultimately, while the last two weeks ended up being quite busy for us, due to the busy nature of both of our schedules, we ended up having the most time to work on the project during those last two weeks, so we don't see the change in schedule as a mismanagement of time, but rather an adjustment we made based on when we would have the most time to work on the project and maintain healthy sleep schedules and work/life balance.

# Issues:

The biggest issue we ran into while creating this project was that initially, instead of polling the buttonPressed variable in our main loop during send mode and calling the sendRaw function from there, we were sending IR signals directly from the UART Bluetooth callback, which was breaking our Bluetooth connection and causing all sorts of problems. This was a difficult issue to overcome because we were not receiving informative error messages as it was happening, and the remote was "sort of" working in this state, which led to a great deal of confusion. Luckily, we were able to figure it out in the end and get everything up and running in a very smooth manner!

# Future Work:

If we had more time to work on this project, there are a few things we would have liked to do/implement. First and foremost, we would have done some arts and crafts. In other words, we would have hidden the wires with some cardboard and made the remote look like an actual remote! Additionally, we would have dedicated our extra time to increasing the accessibility of our remote. In its current state, we rely on an app that can only be as accessible as the user's phone settings allow it to be. With more time, it would have been awesome to develop our own app that would be designed to include as many users as possible and be as accessible as possible for differently abled people who might have trouble using a standard touchscreen app. For example, voice controls would be a nice feature for users with limited mobility. One other idea we had accessibility-wise is that, because our device has Bluetooth functionality, we could theoretically program it to be usable by many other Bluetooth devices which are already designed to be accessible and inclusive to differently abled users.

# Demo Video:

https://drive.google.com/file/d/1Ix689PLs6tVH6722Z8_cGZKShVGIWbWg/view?usp=sharing

# References:

https://crankyoldgit.github.io/IRremoteESP8266/doxygen/html/ \
https://github.com/Arduino-IRremote/Arduino-IRremote \
https://github.com/crankyoldgit/IRremoteESP8266/blob/v2.8.0/examples/IRsendDemo/IRsendDemo.ino \
https://www.maxphi.com/ir-remote-control-decoder-using-arduino
