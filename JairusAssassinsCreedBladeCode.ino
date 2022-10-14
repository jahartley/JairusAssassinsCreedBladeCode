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
unsigned long btTs = 0;

void setup() {
  pinMode(boSoPin, OUTPUT);
  pinMode(boStPin, OUTPUT);
  pinMode(boPin, INPUT_PULLUP);
  pinMode(btSoPin, OUTPUT);
  pinMode(btStPin, OUTPUT);
  pinMode(btPin, INPUT_PULLUP);
}


void loop() {

  int boReading = digitalRead(boPin);
  int btReading = digitalRead(btPin);

  unsigned long now = millis();

  if (boReading != lBoState) {
    // reset the debouncing timer
    debounceTsO = now;
  }
  if (btReading != lBtState) {
    // reset the debouncing timer
    debounceTsT = now;
  }

  if ((now - debounceTsO) > debD) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (boReading != boState) {
      boState = boReading;

      // only toggle the blade if the new button state is LOW
      if (boState == LOW) {
        //UPDATE BLADE STATE MACHINE HERE...
        if (cBoState == 0) {
          cBoState = 1;
        } else {
          cBoState = 2;
        }
      }
    }
  }
  lBoState = boReading;

  if ((now - debounceTsT) > debD) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (btReading != btState) {
      btState = btReading;

      // only toggle the blade if the new button state is LOW
      if (btState == LOW) {
        //UPDATE BLADE STATE MACHINE HERE...
        if (cBtState == 0) {
          cBtState = 1;
        } else {
          cBtState = 2;
        }
      }
    }
  }
  lBtState = btReading;
  
  //Blade1 State Machine
  if (cBoState >= 4) {
    if ((now - boTs) > bRetractTime) {
      cBoState = 0;
    }
  }
  if (cBoState == 3) {
    if ((now - boSolTs) > bSDelay) {
        digitalWrite(boStPin, LOW);
        boTs = now;
        cBoState = 4;
      }
  }
  if (cBoState == 2) {
      digitalWrite(boSoPin, HIGH);
      boSolTs = now;
      cBoState = 3;
  }
  if (cBoState == 1) {
    digitalWrite(boSoPin, LOW);
  }
  if (cBoState == 0) {
    digitalWrite(boSoPin, HIGH);
    digitalWrite(boStPin, HIGH);
  }
  
  //Blade2 State Machine
  if (cBtState >= 4) {
    if ((now - btTs) > bRetractTime) {
      cBtState = 0;
    }
  }
  if (cBtState == 3) {
    if ((now - btSolTs) > bSDelay) {
        digitalWrite(btStPin, LOW);
        btTs = now;
        cBtState = 4;
      }
  }
  if (cBtState == 2) {
      digitalWrite(btSoPin, HIGH);
      btSolTs = now;
      cBtState = 3;
  }
  if (cBtState == 1) {
    digitalWrite(btSoPin, LOW);
  }
  if (cBtState == 0) {
    digitalWrite(btSoPin, HIGH);
    digitalWrite(btStPin, HIGH);
  }
}
