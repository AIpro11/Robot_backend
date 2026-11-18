Hand Following Robot
An Arduino Uno robot that follows your hand using an ultrasonic sensor on a servo. It comes in two versions: v1 with obstacle detection and v2 follow-only (no obstacle detection).
Hardware
Part
Qty
Notes
Arduino Uno
1
Main controller
L293D motor driver shield (Adafruit Motor Shield V1 clone)
1
Drives the 4 motors and the servo
DC gear motors with wheels
4
Connect to M1 to M4
HC-SR04 ultrasonic sensor
1
Mounted on the servo, front center
SG90 servo
1
Sweeps the ultrasonic sensor
IR obstacle sensor modules
2
v1 only
7-12 V battery pack
1
Do not run the motors from USB
Libraries
• AFMotor (Adafruit Motor Shield V1 library)
• Servo (built into the Arduino IDE)
Wiring
Device
Pin
Front left motor
M1
Rear left motor
M2
Front right motor
M3
Rear right motor
M4
HC-SR04 TRIG
A0
HC-SR04 ECHO
A1
SG90 signal
D10 (SERVO_1 header)
Left IR OUT (v1 only)
A2
Right IR OUT (v1 only)
A3
Power every sensor from 5V and GND. Power the shield from the battery pack.
Sensor mounting
• Ultrasonic + servo: front center, facing forward.
• IR sensors (v1 only): front corners, angled outward 30 to 45 degrees, 3 to 5 cm above the ground, facing level. Keep them 8 to 10 cm or more apart, and keep the wheels and chassis out of their beams.
• Keep wires and the IR modules out of the servo sweep (45 to 135 degrees).
How it works
1.  The servo points the ultrasonic sensor left (45), center (90) and right (135).
2.  At each angle the robot takes 3 readings and uses the median, which filters out bad echoes.
3.  The closest object within 100 cm is the target.
4.  The robot turns toward the target angle, or drives forward if it is at the center.
5.  It stops when the target is within 30 cm, or when the target is lost for 2 scans in a row.
Versions
Feature
v1 (obstacle detection)
v2 (follow-only)
Front safety stop at 25 cm
Yes
No
Side IR stop
Yes
No
Forward-path-blocked check
Yes
No
IR sensors needed
Yes
No
Loop speed
Slower
Faster
Follows walls and other objects
No, it stops
Yes, it treats them as targets
v1 note: the robot only follows when your hand is between 30 and 100 cm away. Anything closer than 25 cm triggers SAFETY STOP, and it never reverses.
v2 note: the nearest object within 100 cm becomes the target, so a wall or chair will be followed like a hand.
Settings
Constant
Default
Meaning
MOTOR_SPEED
160
Motor PWM (0 to 255)
FOLLOW_DISTANCE
30
Stop distance in cm
MAX_TRACK_DISTANCE
100
Maximum tracking range in cm
OBSTACLE_STOP_DISTANCE
25
Safety stop in cm (v1 only)
SCAN_DELAY
120
Servo settle time in ms
TURN_TIME
200
Turn pulse length in ms
FORWARD_TIME
150
Forward pulse length in ms
LEFT_ANGLE / RIGHT_ANGLE
45 / 135
Servo scan angles
Setup and test
1.  Install the AFMotor library and open the sketch in the Arduino IDE.
2.  Select Arduino Uno, then upload.
3.  Put the robot on a stand with the wheels in the air.
4.  Open the Serial Monitor at 9600 baud.
5.  Hold your hand about 50 cm away, in front, then left, then right. The "Scanning" distances should match your hand.
6.  Put the robot on the floor and test slowly.
Troubleshooting
Problem
Fix
Robot turns away from your hand
Swap LEFT_ANGLE and RIGHT_ANGLE, or swap turnLeft() and turnRight()
One side drives backward
Swap that motor's two wires
Motors whine but do not move
Raise MOTOR_SPEED or TURN_TIME
Arduino resets when motors start
Use a 7-12 V battery pack, not USB, and add a 470 uF capacitor across the supply
TARGET LOST appears constantly
Check the TRIG and ECHO wiring, and keep your hand within 100 cm
SAFETY STOP: SIDE OBJECT with nothing nearby (v1)
Turn the blue pot on each IR module until its LED goes off in open space, and make sure the beams do not hit the wheels or the floor
Safety
Test with the wheels off the ground first. Keep your fingers away from the wheels while the robot is powered.
Developed by The_Missing_Tensors
