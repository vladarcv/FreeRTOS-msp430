# Deferred Interrupt Processing System

This project implements a system for deferred interrupt processing using software timers and various tasks on an embedded platform.

## Description

The system manages acquisition from channels A14, A15, A8, and A9 every 100 ms via a software timer. It utilizes deferred interrupt processing for an AD converter, where conversion results are queued as messages. Task `xTask1` is notified of new messages using a binary semaphore.

Each message contains the channel information and the upper 12 bits of the AD conversion result. `xTask1` calculates the average of the last 4 received samples for each channel and stores the computed average in a variable protected by a mutex.

A UART interface allows setting a threshold value for signaling. From the UART callback routine, the threshold value is placed in message queue `xQueueT2`. `xTask2` reads this value and controls LEDs based on the average values received. If the average value of a specific channel is below the threshold, the corresponding LED is turned on.

`xTask3` monitors the state of buttons S1 and S2. It notifies `xTask2` via queue `xQueueT2` upon detecting a button press. `xTask2` increments/decrements the current threshold value on button press and sends information about the current state of LEDs (on/off) via UART.

When all four LEDs are turned on, a multiplexed LED display starts counting from 0 with a period of 100 ms, as long as all four LEDs remain on.
