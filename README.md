# Player Piano Controller
Teensy-arduino code for making a [DIY Player Piano](https://www.youtube.com/watch?v=atJ_YsPFDjQ).  
This is a complete rewrite from the [project that inspired me](https://brandonswitzer.squarespace.com/player-piano), but using only one microcontroller for everything (and custom made PCBs for wiring convenience).

##### How does this work ?
A Teensy 3.1 board shows itself as a usb-midi device when plugged into a computer.  
You can then use any midi-player (like winamp for example :D) to output notes to the piano.  
The teensy then translates midi notes to solenoid compatible infos.  
The solenoid [control boards](https://github.com/vikbez/PlayerPianoController/tree/master/PCB) are chained together using 74HC595.  
Solenoids are PWM-driven, with 2 periods:  
- the first period for note volume (with a PWM % corresponding to the volume %)  
- the second period where pwm is reduced only for keeping the note pressed.  
  
See the WIP-code for more infos !  
  
I'm not responsible for your house catching fire if you use this.  

