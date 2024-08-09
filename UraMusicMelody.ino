#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Defining Notes
#define NOTE_B0 31
#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62
#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C8 4186
#define NOTE_CS8 4435
#define NOTE_D8 4699
#define NOTE_DS8 4978

// Melodia da música tema do Super Mario Bros
int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0, 0,
  NOTE_G6, 0, 0, 0,
  NOTE_C7, 0, 0, NOTE_G6,
};



// Duração das notas: 4 = semínima, 8 = colcheia, etc.
int noteDurations[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12
};

// Super Mario Bros course ending melody
int finishMelody[] = {
  NOTE_C5, NOTE_G4, NOTE_E4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_GS4, NOTE_AS4, 
  NOTE_GS4, NOTE_G4, NOTE_F4, NOTE_G4, 0, NOTE_E4, NOTE_C4, NOTE_D4, 
  NOTE_B3, 0
};

// Course ending note durations: 4 = semínima, 8 = colcheia, etc.
int finishNoteDurations[] = {
  8, 8, 8, 8, 8, 8, 8, 8, 
  8, 8, 8, 8, 4, 8, 8, 8, 
  4, 4
};

#define musicButtonPin 2
int musicButtonState = 0;






// Motor Connections (ENA & ENB must use PWM pins)
#define IN1 9
#define IN2 8
#define IN3 7
#define IN4 6
#define ENA 10
#define ENB 5

// Buzzer
#define buzzerPin 3

// Joypad
#define joy A0

// Color Sensor
#define rIn A1
#define lIn A2
#define rCal A3
#define gCal A4
#define bCal A5
#define PIN_NEO_PIXEL 4  // Arduino pin that connects to NeoPixel

int lLDR = A1; // Left LDR
int rLDR = A2; // Right LDR
int readTresholdR;
int readTresholdG;
int readTresholdB;
int mappedTresholdR = 0;
int mappedTresholdG = 0;
int mappedTresholdB = 0;
int lSensorR = 0;
int lSensorG = 0;
int lSensorB = 0;
int rSensorR = 0;
int rSensorG = 0;
int rSensorB = 0;
int lLineSensorValue = 0;
int rLineSensorValue = 0;

int joypos = 512;
int currColor = 0;
int colorLatch = 0;
unsigned long previousMillis = 0;
unsigned long previousBuzzerMillis = 0;

unsigned long DELAY_INTERVAL = 50;
bool finishedColorCycle = 0;

bool debugColors = 1;
bool debugBuzzer = 0;


Adafruit_NeoPixel NeoPixel(1, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);
void setup() {

  // Set motor connections as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Start with motors off
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  pinMode(buzzerPin, OUTPUT);

  NeoPixel.begin();

  Serial.begin(9600);
}

void loop() {

	joypos = analogRead(joy); 

  if (joypos < 460){
		// This is Backward
    back();
 
	}
	else if (joypos > 564){
		// This is Forward
    front();
	}
  else{
    stop();
  }




  unsigned long currentMillis = millis();
  // Calibration Reading
  readTresholdR = analogRead(rCal);
  readTresholdG = analogRead(gCal);
  readTresholdB = analogRead(bCal);
  // Maps potentiometer reading to another interval, if needed.
  mappedTresholdR = map(readTresholdR, 0, 1023, 0, 1023);
  mappedTresholdG = map(readTresholdG, 0, 1023, 0, 1023);
  mappedTresholdB = map(readTresholdB, 0, 1023, 0, 1023);


  NeoPixel.clear();
  
  if (currentMillis - previousMillis <= DELAY_INTERVAL) {
    // Green LEDs flash
    NeoPixel.setPixelColor(0, NeoPixel.Color(0, 255, 0));
    lSensorR = analogRead(lLDR);
    rSensorR = analogRead(rLDR);

  }else if (currentMillis - previousMillis  <= 2 * DELAY_INTERVAL){
    // RED LEDs flash
    NeoPixel.setPixelColor(0, NeoPixel.Color(255, 0, 0));
    lSensorG = analogRead(lLDR);
    rSensorG = analogRead(rLDR);
  }
  else if (currentMillis - previousMillis  <= 3 * DELAY_INTERVAL){
    // Blue LEDs flash
    NeoPixel.setPixelColor(0, NeoPixel.Color(0, 0, 255));
    lSensorB = analogRead(lLDR);
    rSensorB = analogRead(rLDR);
    
  }else if (currentMillis - previousMillis  <= 4 * DELAY_INTERVAL){
    previousMillis = 0;
    finishedColorCycle = 1;
  }else{
    previousMillis = currentMillis;
    
  }

  NeoPixel.show();

  // Button for testing songs
      if (digitalRead(musicButtonPin) == 1){
        musicButtonState = musicButtonState + 1;

        if (musicButtonState == 1){
          startSFX();
        }
        else if (musicButtonState == 2){
          victoryFanfare();
          musicButtonState = 0;
        };

        
      };

  
  if (finishedColorCycle == 1){

    if (debugColors == 1){
      Serial.print("Joy: ");
      Serial.print(joypos);

      Serial.print(" // Butt: ");
      Serial.print(digitalRead(musicButtonPin));
      Serial.print("/");
      Serial.print(musicButtonState);

      Serial.print(" // R: "); 
      Serial.print(lSensorR);
      Serial.print(",");
      Serial.print(rSensorR);
      Serial.print(":");
      Serial.print(mappedTresholdR);


      Serial.print(" // G: ");
      Serial.print(lSensorG);
      Serial.print(",");
      Serial.print(rSensorG);
      Serial.print(":");
      Serial.print(mappedTresholdG);

      Serial.print(" // B: ");
      Serial.print(lSensorB);
      Serial.print(",");
      Serial.print(rSensorB);
      Serial.print(":");
      Serial.print(mappedTresholdB);
    }

    // Left Sensor reading
    // Colors are inverted on serial printing
    if(lSensorR > mappedTresholdR && lSensorG < mappedTresholdG && lSensorB < mappedTresholdB){
      if (debugColors == 1) Serial.print(" L: G");
      lLineSensorValue = 4;
    }
    else if(lSensorR < mappedTresholdR && lSensorG > mappedTresholdG && lSensorB < mappedTresholdB){
      if (debugColors == 1) Serial.print(" L: R");
      lLineSensorValue = 3;
    }
    else if (lSensorR < mappedTresholdR && lSensorG < mappedTresholdG && lSensorB < mappedTresholdB){
      if (debugColors == 1) Serial.print(" L: B");
      lLineSensorValue = 2;
    }
    else if (lSensorR > mappedTresholdR && lSensorG > mappedTresholdG && lSensorB > mappedTresholdB){
      if (debugColors == 1) Serial.print(" L: W");
      lLineSensorValue = 1;
    }
    else{
      if (debugColors == 1) Serial.print(" L: ?");
      lLineSensorValue = 0;
    }

    // Right Sensor reading
    if(rSensorR > mappedTresholdR && rSensorG < mappedTresholdG && rSensorB < mappedTresholdB){
      if (debugColors == 1) Serial.print(" R: G");
      rLineSensorValue = 4;
    }
    else if(rSensorR < mappedTresholdR && rSensorG > mappedTresholdG && rSensorB < mappedTresholdB){
      if (debugColors == 1) Serial.print(" R: R");
      rLineSensorValue = 3;
    }
    else if (rSensorR < mappedTresholdR && rSensorG < mappedTresholdG && rSensorB < mappedTresholdB){
      if (debugColors == 1) Serial.print(" R: B");
      rLineSensorValue = 2;
    }
    else if (rSensorR > mappedTresholdR && rSensorG > mappedTresholdG && rSensorB > mappedTresholdB){
      if (debugColors == 1) Serial.print(" R: W");
      rLineSensorValue = 1;
    }
    else{
      if (debugColors == 1) Serial.print(" R: ?");
      rLineSensorValue = 0;
    }

    if (colorLatch == 1 || colorLatch == 2){
      Serial.print(" | Andando ");
    } else {
      Serial.print(" | Parado ");
    }

    Serial.print(" | Latch: ");
    Serial.print(colorLatch);
    Serial.print(" currColor: ");
    Serial.println(currColor);
    
  }


  finishedColorCycle = 0;



  // If passed through black line
  if (rLineSensorValue == 2 && colorLatch == 0){
    startSFX();
    Serial.print("");
    Serial.print(" Começou ");
    colorLatch = 1;
    };

  // If on a non-black surface
  if (rLineSensorValue == 1 && colorLatch == 1) {
    colorLatch = 2;
  }
  if (rLineSensorValue == 2 && colorLatch == 2) {
    victoryFanfare();
    Serial.println(" ----------------------------------- ");
    colorLatch = 3;
  }
  if (rLineSensorValue == 1 && colorLatch == 3) {
    victoryFanfare();
    Serial.println(" ----------------------------------- ");
    colorLatch = 0;
  }
  
  //Serial.println("");
  









}


void front(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
};

void back(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
};

void stop(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
};


void startSFX(){
  // Percorrer as notas da melodia
  for (int thisNote = 0; thisNote < 11; thisNote++) {

    // Calcular a duração da nota
    int noteDuration = 1000 / noteDurations[thisNote];

    // Tocar a nota no pino 8
    tone(buzzerPin, melody[thisNote], noteDuration);

    // Pausar entre as notas
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);

    // Parar o som
    noTone(buzzerPin);
  }
  
};

void victoryFanfare(){
  // Percorrer as notas da melodia
  for (int thisNote = 0; thisNote < 17; thisNote++) {

    // Calcular a duração da nota
    int noteDuration = 1000 / finishNoteDurations[thisNote];

    // Tocar a nota no pino 8
    tone(buzzerPin, finishMelody[thisNote], noteDuration);

    // Pausar entre as notas
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);

    // Parar o som
    noTone(buzzerPin);
  }

};

