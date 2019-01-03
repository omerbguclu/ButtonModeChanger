// Proje tamamlandı düzenlemeler ve isim değişiklikleri yapılacak teslim için hazırlanacak
int ledPin            =   7;
int switchPin         =   2;
boolean ledState      = LOW;
boolean lastButton    = LOW;
boolean currentButton = LOW;
#define longState         4
#define shortState        2
#define shortLedBlink     4
#define longLedBlink      8
#define shortLoopCount    8
#define longLoopCount    16

long blinkInterval       =  1000;
long goTime              =     0;
unsigned int counter     =     0;
boolean shortMode        =  true;
boolean longMode         = false;

int debounceTime        =    25;
long buttonTimer        =     0;
long shortModeTimer      =     0;
long longModeTimer       =     0;
long longPressTime      =  3000;
boolean buttonActive    = false;
boolean longPressActive = false;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(switchPin, INPUT);
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
    }
  } else {
    if (buttonActive == true) {
      if (longPressActive == true) {// Long loop start
        longMode = true;
        longModeTimer = millis();
        longPressActive = false;
      } else { // Short loop start
        shortMode = true;
        shortModeTimer = millis();
      }
      buttonActive = false;
    }
  }
  lastButton = currentButton;
  if (longMode) {
    if (millis() >= goTime) {
      goFunction(longLoopCount, longLedBlink, blinkInterval);
    }
  } else if (shortMode) {
    if (millis() >= goTime) {
      goFunction(shortLoopCount, shortLedBlink, blinkInterval);
    }
  }
  if (longMode && shortMode) { 
    counter = 0;
    if (longModeTimer > shortModeTimer) {
      shortMode = false;
    } else {
      longMode = false;
    }
  }
}

void goFunction(int loopCount, int blinkCount, int blinkInterval) {
  if (counter < blinkCount) {
    counter++;
    goTime = millis() + blinkInterval;
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
  } else if (counter < loopCount && counter >= blinkCount) {
    counter++;
    goTime = millis() + blinkInterval * 2;
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
  } else if (counter == loopCount) {
    shortMode = false;
    longMode = false;
    counter = 0;
  }
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
