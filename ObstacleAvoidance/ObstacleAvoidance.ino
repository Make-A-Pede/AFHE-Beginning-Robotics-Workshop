/**
 * ObstacleAvoidance.ino
 * Example obstacle avoidance code for the Build Your Own Robot workshop at the 2019 AFHE Convention
 *
 * Copyright (c) 2019 Jude Brauer
 */

#include <NewPing.h>
#include <Servo.h>

#define TRIGGER_PIN  3
#define ECHO_PIN     2
#define MAX_DISTANCE 200

#define LEFT_MOTOR_SPEED 11
#define LEFT_MOTOR_DIR_1 9
#define LEFT_MOTOR_DIR_2 8

#define RIGHT_MOTOR_SPEED 5
#define RIGHT_MOTOR_DIR_1 7
#define RIGHT_MOTOR_DIR_2 6

#define SERVO_PIN 10

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
Servo servo; // 0: right, 90: center, 180: left

/**
 * Set the speed of both sides of the drive
 *
 * @param left Speed of the left side of the drive (-255 to 255)
 * @param right Speed of the right side of the drive (-255 to 255)
 */
void setDrive(int left, int right) {
    digitalWrite(LEFT_MOTOR_DIR_1, left > 0);
    digitalWrite(LEFT_MOTOR_DIR_2, left < 0);

    digitalWrite(RIGHT_MOTOR_DIR_1, right > 0);
    digitalWrite(RIGHT_MOTOR_DIR_2, right < 0);

    analogWrite(LEFT_MOTOR_SPEED, abs(left));
    analogWrite(RIGHT_MOTOR_SPEED, abs(right));
}

/** 
 * Wait until the ultrasonic sensor sees an object
 *
 * @param range The range (cm) within which to look for objects
 */
void waitForUltrasonic(int range) {
    int i = 0;
    while (i < 2) {
        if (sonar.ping_cm() > range) {
            i = 0;
        } else {
            i++;
        }
        delay(50);
    }
}

void setup() {
    Serial.begin(9600);

    pinMode(LEFT_MOTOR_SPEED, OUTPUT);
    pinMode(LEFT_MOTOR_DIR_1, OUTPUT);
    pinMode(LEFT_MOTOR_DIR_2, OUTPUT);

    pinMode(RIGHT_MOTOR_SPEED, OUTPUT);
    pinMode(RIGHT_MOTOR_DIR_1, OUTPUT);
    pinMode(RIGHT_MOTOR_DIR_2, OUTPUT);

    servo.attach(SERVO_PIN);
}

/**
 * Drives forward until an object is seen, then looks both directions and decides which way to turn
 */
void loop() {
    // Look straight ahead
    servo.write(90);
    delay(500);

    // Drive straight until an object is seen within 20 cm
    setDrive(255, 255);
    waitForUltrasonic(20);
    setDrive(0, 0);

    // Look left
    servo.write(180);
    delay(500);
    int lDistance = sonar.ping_cm();

    // Look right
    servo.write(0);
    delay(500);
    int rDistance = sonar.ping_cm();

    // Check which direction has a longer open distance and turn in that direction
    if (lDistance > rDistance) {
        setDrive(-255, 255);
    } else {
        setDrive(255, -255);
    }

    delay(500);
    setDrive(0, 0);
}
