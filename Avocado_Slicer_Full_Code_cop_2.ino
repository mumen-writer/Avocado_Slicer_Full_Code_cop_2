///Pin Assignments For Stepper Motor
//microstepping pins (digital)
const int MS1 = 13;
const int MS2 = 12;
//step and direction pins (digital)
const int stepPin = 10;
const int dirPin = 9;

///Pin Assignments For Camera
// Pin A0: Analog output for shutter because digital has current running through at all times
int SHUTTER_PIN = A0;

///Pin Assignemnts For Solenoid
int SOLENOID = 2;

///Pin and Variable Assignments For Sensor 1
//pin for break beam sensor 1 (initial photo stage) (digital)
const int sensorPin1 = 7;
int sensorState1 = 0;
int sensor1Counter;

///Pin and Variable Assignments For Sensor 2
//pin for break beam sensor 2 (cutting phase) (digital)
const int sensorPin2 = 8;
int sensorState2 = 0;
int sensor2Counter;

///Pin and Variable Assignments For Sensor 3
//pin for break beam sensor 3 (final photo stage) (digital)
const int sensorPin3 = 11;
int sensorState3 = 0;
int sensor3Counter;

///Pin and Variable Assignments for Emergency Stops
const int stopPin1 = 3;
const int stopPin2 = 4;
int emergencyState1 = 0;
int emergencyState2 = 0;




//set pin functions
void setup() {
 pinMode(sensorPin1,INPUT);
 pinMode(sensorPin2,INPUT);
 pinMode(sensorPin3,INPUT);
 pinMode(MS1,OUTPUT);
 pinMode(MS2,OUTPUT);
 pinMode(stepPin,OUTPUT);
 pinMode(dirPin,OUTPUT);
 pinMode(SOLENOID,OUTPUT);
 //leave SHUTTER_PIN as an input so that no signals are sent to it until it's time to take a picture
 pinMode(SHUTTER_PIN,INPUT);

 //set the sensor pins to HIGH
 digitalWrite(sensorPin1, HIGH);
 digitalWrite(sensorPin2, HIGH);
 digitalWrite(sensorPin3, HIGH);
 digitalWrite(stopPin1, HIGH);
 digitalWrite(stopPin2, HIGH);

 //set microstepping (default: (LOW,LOW) for full steps)
 digitalWrite(MS1, LOW);
 digitalWrite(MS2, LOW);

//set baudrate to 9600
 Serial.begin(9600); 

//set sensor pin counters.
//The sensor counters are used to ensure that the avocados pass through the sensors without re-triggering them. It is assumed that an avocado can pass through a sensor after 3000 motor steps (for stepper motor with gear box). 
sensor1Counter = 3001;
sensor2Counter = 3001;
sensor3Counter = 3001;
 
} 



//main code loop
void loop() {


//to fix bug where sensor counters would turn negative once reaching 32500
if (sensor1Counter < 0) {
  sensor1Counter = 3001;
}
if (sensor2Counter < 0) {
  sensor2Counter = 3001;
}
if (sensor3Counter < 0) {
  sensor3Counter = 3001;
}


//set sensors back to inputs when the avocado has passed through
if (sensor1Counter == 3001) {
  pinMode(sensorPin1,INPUT);
  digitalWrite(sensorPin1, HIGH);
}
if (sensor2Counter == 3001) {
  pinMode(sensorPin2,INPUT);
  digitalWrite(sensorPin2, HIGH);
}
if (sensor3Counter == 3001) {
  pinMode(sensorPin3,INPUT);
  digitalWrite(sensorPin3, HIGH);
}


//display sensor counters in the Serial Monitor (Optional. For monitoring purposes)
//if (sensor3Counter % 500 == 0) {
//Serial.print(sensor3Counter);
//Serial.print("\t");
//Serial.print(sensor2Counter);
//Serial.print("\n");
//}


//read the sensor inputs
sensorState1 = digitalRead(sensorPin1);
sensorState2 = digitalRead(sensorPin2);
sensorState3 = digitalRead(sensorPin3);
emergencyState1 = digitalRead(stopPin1);
emergencyState2 = digitalRead(stopPin2);


//if either emergency stop sensor is broken, stop the machine!! (WARNING: FOR DEMONSTRATION PURPOSES, THE MACHINE STOPPING IS REPRESENTED BY A DELAY RATHER THAN ACTUALLY STOPPING THE CODE)
if (emergencyState1 == LOW) {
  delay(6000);
}
if (emergencyState2 == LOW) {
  delay(6000);
}


//if Sensor 1 is broken, stop the motor for a certain amount of time and take a picture
if (sensorState1 == LOW) {
  if (sensor1Counter > 3000) {
    //you can insert a delay here to delay the motor from starting after being stopped. This delay will allow time for a photo to be taken or an avocado to be cut. (default: 1000)


//TAKE PICTURE 1
  captureImage1();
    
    sensor1Counter = 0;
    pinMode(sensorPin1,OUTPUT);
  }
}
\

//if Sensor 2 is broken, stop the motor for a certain amount of time and trigger the actuator
if (sensorState2 == LOW) {
  if (sensor2Counter > 3000) {

//CUT AVOCADO
  cutAvocado();

  sensor2Counter = 0;
  pinMode(sensorPin2,OUTPUT);
  }
}


//if Sensor 3 is broken, stop the motor for a certain amount of time and take a picture
if (sensorState3 == LOW) {
  if (sensor3Counter > 3000) {

//TAKE PICTURE 2
  captureImage();

  sensor3Counter = 0;
  pinMode(sensorPin3,OUTPUT);
  }
}


//if all beams are unbroken, keep the motor turning

//the next 4 lines are to turn the stepper motor. Change the value of "delayMicroseconds" to change the speed of the motor. (default: 600 600)
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(700);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(700);

//increment sensor counters to allow avocados to pass through each sensor without re-triggering it. Each sensor counter increments by 1 for every step taken by the stepper motor.
    sensor1Counter = sensor1Counter + 1;
    sensor2Counter = sensor2Counter + 1;
    sensor3Counter = sensor3Counter + 1;
    
}







//function captureImage1() takes initial picture
void captureImage1()
{
delay(1000);

//CODE FOR TAKING INITIAL PICTURE GOES HERE

delay(1000);
}



//function cutAvocado() triggers solenoid and cuts avocado
void cutAvocado()
{
  delay(1000);
digitalWrite(SOLENOID, HIGH);
  delay(1000);
digitalWrite(SOLENOID, LOW);
  delay(1000);
}




//function captureImage() takes final picture
void captureImage()
{
  delay(1000);
  pinMode(SHUTTER_PIN, OUTPUT);
  digitalWrite(SHUTTER_PIN, 0);
  delay(2000);
  pinMode(SHUTTER_PIN, INPUT);
  delay(1000);
}
