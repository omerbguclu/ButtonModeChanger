//Pin ayarları
int ledPin               =     7; // Led 7. pine ayarlandı
int switchPin            =     2; // Button 2. pine ayarlandı
boolean ledState         =   LOW; // Led sönük olarak başlıyor

// Led yanıp sönme ayarları
long blinkInterval       =  1000; // En kısa yanıp sönme aralığı
long intervalTimer       =     0; // Yanıp sönme aralığı sayacı
long shortModeTimer      =     0; // Mod değişikliği yaparken hangi modun
long longModeTimer       =     0; //    son basıldığını tutmak için sayaçlar
int counter              =     0; // Döngü kontrol sayacı
boolean shortMode        =  true; // Kısa modu başlatma (başlangıçta kısa mod seçili)
boolean longMode         = false; // Uzun modu başlatma

// Debounce ayarları
int debounceTime         =   100; // İlanda istenen debounce süresi
boolean lastButton       =   LOW; // Butonun son basılan hali
boolean currentButton    =   LOW; // Butonun bir önceki basılmış hali

// Buton basma süresi ayarları
long buttonTimer         =     0; // Buton basma sayacı
long longPressTime       =  3000; // Butona uzun basma süresi
boolean buttonActive     = false; // Kısa basma modu 
boolean longPressActive  = false; // Uzun basma modu

// Mod ayarları
#define shortLedBlink     4 // Kısa basma modunda bir tur (farklı saniye aralıkları için) 
#define longLedBlink      8 // Uzun basma modunda bir tur (farklı saniye aralıkları için)
#define shortLoopCount    8 // Kısa basma modu bir tur sayısı
#define longLoopCount    16 // Uzun basma modu bir tur sayısı

void setup() {
  pinMode(ledPin, OUTPUT);                  // Led output moduna alındı
  pinMode(switchPin, INPUT);                // Buton input moduna alındı
}

void loop() {
  currentButton = debounce(lastButton);     // Buton debounce süresi ayarlama
  if (currentButton == HIGH) {              // Buton basılı iken
    if (buttonActive == false) {            // Buton basılı durum kontrolü
      buttonActive = true;                  // Butonu basılı duruma getir
      buttonTimer = millis();               // Buton basma süresini başlat
    }
    if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)) { 
      longPressActive = true;               // Buton 3 saniyeden fazla basıldıysa uzun basma moduna geç
    }
  } else {                                  // Buton basılı durumda değilse
    if (buttonActive == true) {             // Buton kontrolü doğruysa
      if (longPressActive == true) {        // Uzun basma modu kontrolü
        longMode = true;                    // Uzun basma modu aktif
        longModeTimer = millis();           // Uzun basma moduna başlangıç süresi
        longPressActive = false;            // Uzun basma modunu false yap
      } else {                              // Kısa basma modu kontrolü
        shortMode = true;                   // Kısa basma modu aktif
        shortModeTimer = millis();          // Kısa basma moduna başlangıç süresi
      }
      buttonActive = false;                 // Buton basılı durumunu false yap
    }
  }
  lastButton = currentButton;               // Butonun bir önceki basılmış halini son hale eşitle

  if (longMode) {                           //*  
    if (millis() >= intervalTimer) {        // 
      ledBlink(longLoopCount, longLedBlink);//  ### Uzun basma modu için ledleri yak
    }                                       //*  
  } else if (shortMode) {                   //*
    if (millis() >= intervalTimer) {        //
      ledBlink(shortLoopCount, shortLedBlink);//### Kısa basma modu için ledleri yak
    }                                       //
  }                                         //*

  if (longMode && shortMode) {              // Uzun ve kısa basma modu aynı anda aktif olursa
    counter = 0;                            // Blink loop sayacını sıfırla
    if (longModeTimer > shortModeTimer) {   // Uzun basma modu kısa basma modundan daha sonra başladıysa
      shortMode = false;                    // Kısa modu iptal et
    } else {                                // Kısa basma modu uzun basma modundan daha sonra başladıysa
      longMode = false;                     // Uzun modu iptal et
    }
  }
}

void ledBlink(int loopCount, int blinkCount) {                // Led yakıp sönme metodu
  if (counter < blinkCount) {                                 // Döngü sayaçı aralık değiştirme kontrolü
    counter++;                                                //    kısa mod için 4 uzun mod için 8
    intervalTimer = millis() + blinkInterval;                 // Blink aralık sayacına 1 saniye ekleme
    ledState = !ledState;                                     // Led durumunu değiştir
    digitalWrite(ledPin, ledState);                           // Led durumunu pine gönder
  } else if (counter < loopCount && counter >= blinkCount) {  // Döngü sayaçı aralık değiştirme kontrolü
    counter++;                                                // 
    intervalTimer = millis() + blinkInterval * 2;             // Blink aralık sayacına 2 saniye ekleme
    ledState = !ledState;                                     // Led durumunu değiştir
    digitalWrite(ledPin, ledState);                           // Led durumunu pine gönder
  } else if (counter == loopCount) {                          // Döngüyü sonlandırma durumu
    shortMode = false;                                        // Modları kapat
    longMode = false;                                         // Modları kapat
    counter = 0;                                              // Döngü sayacını sıfırla
  }
}

boolean debounce(boolean last)                // Debounce fonksiyonu başlangıç
{
  boolean current = digitalRead(switchPin);   // Buton durumunu okuyup son duruma yaz
  if (last != current)                        // Son buton durumuyla bir önceki buton durumu farklıysa
  {
    delay(debounceTime);                      // Debounce süresi kadar bekle(delay kullanılan tek yer)
    current = digitalRead(switchPin);         // Buton durumunu tekrar oku 
  }
  return current;                             // Buton durumunu gönder
}
