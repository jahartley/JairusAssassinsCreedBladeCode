#define button1Pin 2
#define blade1Sol1OutPin 4
#define blade1Sol2OutPin 5
#define button2Pin 3
#define blade2Sol1OutPin 6
#define blade2Sol2OutPin 7


int button1State = HIGH;
int button2State = HIGH;
int lastButton1State = HIGH;
int lastButton2State = HIGH;
int currentBlade1State = 0;
int currentBlade2State = 0;

//define blade states
#define bladeRetracted 0
#define bladeExtend 1
#define bladeRetract 2
#define bladeRetractDelay 3
#define bladeRetracting 4

const unsigned long debounceDuration = 20; // This controls button debounce duration. 
const unsigned long bladeRetractTime = 300; // adjust this if we're stabbing produce. @jairus 
const unsigned long bladeSoldDelay = 100; // This controls how long till we switch between the extending solenoid and the retracting solenoid. 

unsigned long debounce1TimeStamp = 0;
unsigned long debounce2TimeStamp = 0;

unsigned long blade1SolTS = 0;
unsigned long blade2SolTS = 0;

unsigned long blade1Timer = 0;
unsigned long blade2Timer = 0;

void setup() {
  pinMode(blade1Sol1OutPin, OUTPUT);
  pinMode(blade1Sol2OutPin, OUTPUT);
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(blade2Sol1OutPin, OUTPUT);
  pinMode(blade2Sol2OutPin, OUTPUT);
  pinMode(button2Pin, INPUT_PULLUP);
}


void loop() {

  int b1reading = digitalRead(button1Pin);
  int b2reading = digitalRead(button2Pin);

  unsigned long now = millis();

  if (b1reading != lastButton1State) {
    // reset the debouncing timer
    debounce1TimeStamp = now;
  }
  if (b2reading != lastButton2State) {
    // reset the debouncing timer
    debounce2TimeStamp = now;
  }

  if ((now - debounce1TimeStamp) > debounceDuration) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (b1reading != button1State) {
      button1State = b1reading;

      // only toggle the blade if the new button state is LOW
      if (button1State == LOW) {
        //UPDATE BLADE STATE MACHINE HERE...
        if (currentBlade1State == bladeRetracted) {
          currentBlade1State = bladeExtend;
        } else {
          currentBlade1State = bladeRetract;
        }
      }
    }
  }
  lastButton1State = b1reading;

  if ((now - debounce2TimeStamp) > debounceDuration) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (b2reading != button2State) {
      button2State = b2reading;

      // only toggle the blade if the new button state is LOW
      if (button2State == LOW) {
        //UPDATE BLADE STATE MACHINE HERE...
        if (currentBlade2State == bladeRetracted) {
          currentBlade2State = bladeExtend;
        } else {
          currentBlade2State = bladeRetract;
        }
      }
    }
  }
  lastButton2State = b2reading;
  
  //Blade1 State Machine
  if (currentBlade1State == bladeRetracting) {
    if (now - blade1Timer > bladeRetractTime) {
      currentBlade1State = bladeRetracted;
    }
  }
  if (currentBlade1State == bladeRetractDelay) {
    if (now - blade1SolTS > bladeSoldDelay) {
        digitalWrite(blade1Sol2OutPin, LOW);
        blade1Timer = now;
        currentBlade1State = bladeRetracting;
      }
  }
  if (currentBlade1State == bladeRetract) {
      digitalWrite(blade1Sol1OutPin, HIGH);
      blade1SolTS = now;
      currentBlade1State = bladeRetractDelay;
  }
  if (currentBlade1State == bladeExtend) {
    digitalWrite(blade1Sol1OutPin, LOW);
  }
  if (currentBlade1State == bladeRetracted) {
    digitalWrite(blade1Sol1OutPin, HIGH);
    digitalWrite(blade1Sol2OutPin, HIGH);
  }
  
  //Blade2 State Machine
  if (currentBlade2State == bladeRetracting) {
    if (now - blade2Timer > bladeRetractTime) {
      currentBlade2State = bladeRetracted;
    }
  }
  if (currentBlade2State == bladeRetractDelay) {
    if (now - blade2SolTS > bladeSoldDelay) {
        digitalWrite(blade2Sol2OutPin, LOW);
        blade2Timer = now;
        currentBlade2State = bladeRetracting;
      }
  }
  if (currentBlade2State == bladeRetract) {
      digitalWrite(blade2Sol1OutPin, HIGH);
      blade2SolTS = now;
      currentBlade2State = bladeRetractDelay;
  }
  if (currentBlade2State == bladeExtend) {
    digitalWrite(blade2Sol1OutPin, LOW);
  }
  if (currentBlade2State == bladeRetracted) {
    digitalWrite(blade2Sol1OutPin, HIGH);
    digitalWrite(blade2Sol2OutPin, HIGH);
  }
}
