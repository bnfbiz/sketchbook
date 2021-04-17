#include <Servo.h>

Servo servoController3;
Servo servoController5;
Servo servoController9;
Servo servoController10;

// https://store.arduino.cc/usa/leonardo
// PWM: 3, 5, 6, 9, 10, 11, and 13. Provide 8-bit PWM output with the analogWrite() function
int pwmController3 = 3; // assigns pin 3 to variable pwm
int pwmController5 = 5; // assigns pin 5 to variable pwm
int pwmController9 = 9; // assigns pin 9 to variable pwm
int pwmController10 = 10; // assigns pin 10 to variable pwm

unsigned long watchDogTimeout = 5000;
unsigned long watchDogTimmer;
bool watchDogDisabled = false;
bool debug = false;

void setup()  // setup loop
{
  servoController3.attach(pwmController3);
  servoController5.attach(pwmController5);
  servoController9.attach(pwmController9);
  servoController10.attach(pwmController10);
  Serial.begin(115200);
  Serial.setTimeout(100);
  watchDogTimmer = millis();
}
void loop()
{
    int location;
    int port;
    int value;
    String line;
    unsigned long currentTime;
    int start,end;
    char buffer[21];

    currentTime = millis();
    if ((currentTime > (watchDogTimmer + watchDogTimeout)) && !watchDogDisabled) {
      watchDogShutdown();
    }
    if (Serial.available()) {
      start = millis();
      line = Serial.readStringUntil('\n');
      // line = Serial.readBytesUntil('\n',buffer, 20);
      if (debug) {
        Serial.print("It was this many characters long: ");
        Serial.println(line.length(), DEC);
        Serial.print("You sent me: ");
        Serial.println(line);
      }
      // end = millis();
      location = line.indexOf("P");
      while (location >= 0) {
        if (debug) {
          Serial.print("location is: ");
          Serial.println(String(location));
        }
        // Get the port number
        port = line.substring(location+1).toInt();
        location = line.indexOf(" ", location+1);
        value = line.substring(location).toInt();
        if (debug) {
          Serial.print("The port is: ");
          Serial.println(String(port));
          Serial.print("The desired value is: ");
          Serial.println(String(value));
          Serial.println("Finished searching the string");
        }
        location = line.indexOf("P",location+1);
        switch (port) {
          case 3:
            servoController3.write(value);
            break;
          case 5:
            servoController5.write(value);
            break;
          case 6:
            Serial.print("Port ");
            Serial.print(port,DEC);
            Serial.println(" is not configured for a device");          
            break;
          case 9:
            servoController9.write(value);
            break;
          case 10:
            servoController10.write(value);
            break;
          case 11:
            Serial.print("Port ");
            Serial.print(port,DEC);
            Serial.println(" is not configured for a device");
            break;
          case 13:
            Serial.print("Port ");
            Serial.print(port,DEC);
            Serial.println(" is not configured for a device");
            break;
          default:
            Serial.print("Unknown port ");
            Serial.println(port,DEC);
            break;
        }
        end = millis();
        Serial.print("took:");
        Serial.println(end-start, DEC);
        watchDogTimmer = millis();
        watchDogDisabled = false;
      }
    }
    delayMicroseconds(10000);
}

void watchDogShutdown() {
  int value = 90;   // set the pwm value to 90 degrees which will turn off traditional motors

  Serial.println("Watchdog is turning off the system as no recent updates were found");
  servoController3.write(value);
  servoController5.write(value);
  servoController9.write(value);
  servoController10.write(value);
  watchDogDisabled = true;
}