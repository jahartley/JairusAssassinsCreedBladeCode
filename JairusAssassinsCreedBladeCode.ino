#define boPin 2
#define boSoPin 4
#define boStPin 5
#define btPin 3
#define btSoPin 6
#define btStPin 7


int boState = HIGH;
int btState = HIGH;
int lBoState = HIGH;
int lBtState = HIGH;
int cBoState = 0;
int cBtState = 0;

const unsigned long debD = 20; // This controls button debounce duration. 
const unsigned long bRetractTime = 300; // adjust this if we're stabbing produce. @jairus 
const unsigned long bSDelay = 100; // This controls how long till we switch between the extending solenoid and the retracting solenoid. 

unsigned long debounceTsO = 0;
unsigned long debounceTsT = 0;

unsigned long boSolTs = 0;
unsigned long btSolTs = 0;

unsigned long boTs = 0;
unsigned long btTS = 0;

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
        if (currentBlade1State == 0) {
          currentBlade1State = 1;
        } else {
          currentBlade1State = 2;
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
        if (currentBlade2State == 0) {
          currentBlade2State = 1;
        } else {
          currentBlade2State = 2;
        }
      }
    }
  }
  lastButton2State = b2reading;
  
  //Blade1 State Machine
  if (currentBlade1State >= 4) {
    if (now - blade1Timer > bladeRetractTime) {
      currentBlade1State = 0;
    }
  }
  if (currentBlade1State == 3) {
    if (now - blade1SolTS > bladeSoldDelay) {
        digitalWrite(blade1Sol2OutPin, LOW);
        blade1Timer = now;
        currentBlade1State = 4;
      }
  }
  if (currentBlade1State == 2) {
      digitalWrite(blade1Sol1OutPin, HIGH);
      blade1SolTS = now;
      currentBlade1State = 3;
  }
  if (currentBlade1State == 1) {
    digitalWrite(blade1Sol1OutPin, LOW);
  }
  if (currentBlade1State == 0) {
    digitalWrite(blade1Sol1OutPin, HIGH);
    digitalWrite(blade1Sol2OutPin, HIGH);
  }
  
  //Blade2 State Machine
  if (currentBlade2State >= 4) {
    if (now - blade2Timer > bladeRetractTime) {
      currentBlade2State = 0;
    }
  }
  if (currentBlade2State == 3) {
    if (now - blade2SolTS > bladeSoldDelay) {
        digitalWrite(blade2Sol2OutPin, LOW);
        blade2Timer = now;
        currentBlade2State = 4;
      }
  }
  if (currentBlade2State == 2) {
      digitalWrite(blade2Sol1OutPin, HIGH);
      blade2SolTS = now;
      currentBlade2State = 3;
  }
  if (currentBlade2State == 1) {
    digitalWrite(blade2Sol1OutPin, LOW);
  }
  if (currentBlade2State == 0) {
    digitalWrite(blade2Sol1OutPin, HIGH);
    digitalWrite(blade2Sol2OutPin, HIGH);
  }
}
