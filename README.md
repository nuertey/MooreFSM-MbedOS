# MooreFSM-MbedOS - Formalized Moore Model Finite State Machine For LED Control

MbedOS application that controls the MCU LED1 based upon progression in an FSM driven by a rising edge interrupt received when the User button is depressed.

![Problem Statement Moore Model Finite State Machine](https://github.com/nuertey/MooreFSM-MbedOS/blob/main/resources/FiniteStateMachine-MooreModel.jpeg?raw=true)

## Execution Output Snippet: 

```shell-session

Connecting to /dev/ttyACM0, speed 9600
 Escape character: Ctrl-\ (ASCII 28, FS): enabled
Type the escape character followed by C to get back,
or followed by ? to see other options.
----------------------------------------------------
[StateMachine_t][process_event] boost::ext::sml::v1_1_5::back::on_entry<boost::ext::sml::v1_1_5::back::_, boost::ext::sml::v1_1_5::back::initial>


MooreFSM-MbedOS Application - Beginning... 

Mbed OS version: 6.16.0

Built: Sep 22 2022, 05:59:35

[StateMachine_t][process_event] {anonymous}::buttonPressed
[StateMachine_t][transition] Off -> On
[StateMachine_t][action] {anonymous}::do_light_on {anonymous}::buttonPressed
[StateMachine_t][process_event] {anonymous}::buttonPressed
[StateMachine_t][transition] On -> Blink
[StateMachine_t][action] {anonymous}::do_light_blink {anonymous}::buttonPressed
[StateMachine_t][process_event] {anonymous}::buttonPressed
[StateMachine_t][transition] Blink -> Off
[StateMachine_t][action] {anonymous}::do_light_off {anonymous}::buttonPressed
[StateMachine_t][process_event] {anonymous}::buttonPressed
[StateMachine_t][transition] Off -> On
[StateMachine_t][action] {anonymous}::do_light_on {anonymous}::buttonPressed
[StateMachine_t][process_event] {anonymous}::buttonPressed
[StateMachine_t][transition] On -> Blink
[StateMachine_t][action] {anonymous}::do_light_blink {anonymous}::buttonPressed
[StateMachine_t][process_event] {anonymous}::buttonPressed
[StateMachine_t][transition] Blink -> Off
[StateMachine_t][action] {anonymous}::do_light_off {anonymous}::buttonPressed
[StateMachine_t][process_event] {anonymous}::buttonPressed
[StateMachine_t][transition] Off -> On
[StateMachine_t][action] {anonymous}::do_light_on {anonymous}::buttonPressed
[StateMachine_t][process_event] {anonymous}::buttonPressed
[StateMachine_t][transition] On -> Blink
[StateMachine_t][action] {anonymous}::do_light_blink {anonymous}::buttonPressed
[StateMachine_t][process_event] {anonymous}::buttonPressed
[StateMachine_t][transition] Blink -> Off
[StateMachine_t][action] {anonymous}::do_light_off {anonymous}::buttonPressed
[StateMachine_t][process_event] {anonymous}::buttonPressed
[StateMachine_t][transition] Off -> On
[StateMachine_t][action] {anonymous}::do_light_on {anonymous}::buttonPressed
[StateMachine_t][process_event] {anonymous}::buttonPressed
[StateMachine_t][transition] On -> Blink
[StateMachine_t][action] {anonymous}::do_light_blink {anonymous}::buttonPressed

...

```

## License
MIT License

Copyright (c) 2022 Nuertey Odzeyem

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
