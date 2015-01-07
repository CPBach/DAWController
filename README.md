#DAWController


**DAWController** is a project started two years ago with the goal to provide a software framework for AVR-microcontrollers to be able to use old analog controllers and turn them into MIDI-controllers.
This is done by using the AVR's intern AD-converter to discretize voltage on the resistors and then turning it into 
MIDI-messages. To be able to measure a large amount of resistors, we use Multiplexer-ICs.
Every knob/slider/switch is configurable with a custom MIDI-message-number.

<img src="https://raw.githubusercontent.com/CPBach/DAWController/master/mixer10.jpg" align="left" hspace="10" vspace="6">

<img src="https://raw.githubusercontent.com/CPBach/DAWController/master/1.jpg" align="left" hspace="10" vspace="6">

As it stands, we are supporting some HD44870 based LCD's with Peter Fleurys Library.
The general concept can be seen on <a href="https://www.youtube.com/watch?v=eYuYX_v-R0A">Youtube</a>.

<img src="https://raw.githubusercontent.com/CPBach/DAWController/master/6.jpg" align="left" hspace="10" vspace="6">
We simply turned one Mic-In into a DC-IN, another MIC-In into a MIDI-OUT and a MIC-in into serial programmable interface.

<img src="https://raw.githubusercontent.com/CPBach/DAWController/master/mixer12.jpg" align="left" hspace="10" vspace="6">
This is how the mixer looks from the inside. Quite a mess.


