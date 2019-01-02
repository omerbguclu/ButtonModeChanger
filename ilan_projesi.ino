int ledPin            =   7;
int switchPin         =   2;
boolean led_state     = LOW;
boolean lastButton    = LOW;
boolean currentButton = LOW;
#define longState         4
#define shortState        2

boolean shortMod        = false;
boolean longMod         = false;
int debounceTime        =   25;
int shortTime           =  250;
int longTime            =  500;
long buttonTimer        =     0;
long longPressTime      =  3000;
boolean buttonActive    = false;
boolean longPressActive = false;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(switchPin, INPUT);
  Serial.begin(9600);
  modChange(ledPin, shortState);
}

boolean debounce(boolean last)
{
  boolean current = digitalRead(switchPin);
  if (last != current)
  {
    delay(debounceTime);
    current = digitalRead(switchPin);
  }
  return current;
}

void modChange(int ledPin, int ledState) {
  for (int i = 0; i < ledState; i++) {
    digitalWrite(ledPin, HIGH);
    delay(shortTime);
    digitalWrite(ledPin, LOW);
    delay(shortTime);
  }
  for (int i = 0; i < ledState; i++) {
    digitalWrite(ledPin, HIGH);
    delay(longTime);
    digitalWrite(ledPin, LOW);
    delay(longTime);
  }
}


void loop() {
  currentButton = debounce(lastButton);
  if (currentButton == HIGH) {
    if (buttonActive == false) {
      buttonActive = true;
      buttonTimer = millis();
    }
    if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)) {
      longPressActive = true;
      longMod = !longMod;
      modChange(ledPin, longState);
    }
  } else {
    if (buttonActive == true) {
      if (longPressActive == true) {
        longPressActive = false;
      } else {
        shortMod = !shortMod;
        modChange(ledPin, shortState);
      }
      buttonActive = false;
    }
  }
  lastButton = currentButton;

//  currentButton = debounce(lastButton);
//  if (lastButton == LOW && currentButton == HIGH)
//  {
//    modChange(ledPin, shortState);
//  }
//  lastButton = currentButton;
}
