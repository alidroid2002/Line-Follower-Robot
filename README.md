Line Follower Robot using PID Control
This project implements a line-following robot using an Arduino (Uno/Nano) with a 5-channel IR sensor array and an L298N (or similar) motor driver. The control algorithm uses a PID (Proportional-Integral-Derivative) controller to keep the robot centered on a black line over a white background.

Features
5‑sensor array for accurate line position detection

PID control for smooth and fast following

Automatic handling of line loss (turn left/right until line is found)

Configurable base speed and PID gains (tunable via constants)

Serial debugging output showing left/right motor speeds

Hardware Required
Component	Quantity
Arduino (Uno/Nano)	1
5‑channel IR sensor array (or 5 individual IR modules)	1
L298N or L293D motor driver	1
2 DC motors (with wheels)	2
Battery pack (7.2V–12V)	1
Chassis, castor wheel, jumper wires	as needed
Pin Connections
The code defines the following pin mappings. Connect your hardware accordingly.

Arduino Pin	Component / Function
3	ENA – Motor A enable (PWM)
4	IN1 – Motor A direction
5	IN2 – Motor A direction
6	IN3 – Motor B direction
7	IN4 – Motor B direction
8	ENB – Motor B enable (PWM)
A0 – A4	IR sensor outputs (analog)
Motor A (left motor) is controlled by IN1, IN2 and ENA.

Motor B (right motor) is controlled by IN3, IN4 and ENB.

How It Works
Sensor Reading (sensor() function):

Reads the 5 analog IR sensors.

Compares each value against a threshold (default 500) to get a digital line‑present state (1 = line, 0 = background).

Calculates a weighted average to determine the line position (range 10–50).

Computes an error = 30 – linePosition (where 30 is the centre position for 5 sensors equally spaced).

If no sensor detects the line (state == 0), returns 404 to indicate line loss.

PID Control (PIDlineflw() function):

Reads the current error (or handles line loss).

Computes:

P = current error

I = integral of error over time (I += error * dt)

D = derivative of error ((error – prevError) / dt)

Calculates the steering value: strValue = kp*P + ki*I + kd*D.

Applies the steering to the base speed:

leftSpeed = baseSpeed + strValue

rightSpeed = baseSpeed – strValue

Both speeds are constrained to 0–255 (PWM range) and passed to the motor() function.

Motor Control (motor(int lSpeed, int rSpeed)):

Sets direction pins according to the sign of the speed (positive = forward, negative = backward, zero = brake).

Writes the absolute speed value to ENA and ENB via analogWrite().

Line Loss Recovery:

If the line is lost (error == 404), the robot rotates in the direction of the last known line edge (recorded in lastSensor) until the line is found again.

Tuning the PID Parameters
The default gains are:

cpp
float kp = 5;
float kd = 5;
float ki = 1;
Increase kp for more aggressive response to error.

Increase kd to reduce overshoot and damp oscillations.

Increase ki to eliminate steady‑state offset (but beware of integral wind‑up).

You may also adjust:

threshold (default 500) – the analog value above which a sensor is considered to be on the line.

baseSpeed (default 100) – the nominal motor speed (0–255).

Serial Output
During operation, the robot prints the left and right motor speeds (PWM values) to the Serial Monitor at 9600 baud. This helps with debugging and tuning.

Installation & Upload
Download or clone this repository.

Open the .ino file in the Arduino IDE.

Connect your Arduino board to the computer.

Select the correct board and port from the Tools menu.

Click Upload.

Open the Serial Monitor to view debug output.

Customization & Extensions
Different sensor array: Adjust sensNum and the irPins array to match your setup.

Speed limits: Modify the baseSpeed constant.

Change PID update rate: The loop runs as fast as possible; you can add a fixed‑time delay if needed.

Add LED indicators: Use the lastSensor value to light up LEDs for debugging.

Notes
Ensure that the motor driver’s ground is common with the Arduino and the battery.

Calibrate the threshold value according to your surface – the sensors should read above the threshold on the line and below on the background.

The integral term ki may need to be very small to avoid wind‑up. Consider adding integral clamping if necessary.
