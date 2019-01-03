int ledPin            =   7;
int switchPin         =   2;
boolean ledState     = LOW;
boolean lastButton    = LOW;
boolean currentButton = LOW;
#define longState         4
#define shortState        2
#define shortLedBlink     4
#define longLedBlink      8
#define shortLoopCount    8
#define longLoopCount    16

long blinkInterval      =   500;
long goTime             =     0;
unsigned int counter    =     0;
boolean shortMode        = false;
boolean longMode         = false;

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
      //longMod = true;
      //modChange(ledPin, longState);
    }
  } else {
    if (buttonActive == true) {
      if (longPressActive == true) {// Long loop start
        longMode = true;
        //modChange(ledPin, longState);
        longPressActive = false;
      } else { // Short loop start
        shortMode = true;
        //modChange(ledPin, shortState);
      }
      buttonActive = false;
    }
  }
  lastButton = currentButton;
  if(longMode){
    if (millis() >= goTime) {
      goFunction(longLoopCount, longLedBlink, blinkInterval, longMode);
    }    
  }else if (shortMode){
    if (millis() >= goTime) {
      goFunction(shortLoopCount, shortLedBlink, blinkInterval, shortMode);
    }
  }
}

void goFunction(int loopCount, int blinkCount, int blinkInterval, boolean mode) {
  Serial.print("Counter is ");
  Serial.println(counter);
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
    mode = false;
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
