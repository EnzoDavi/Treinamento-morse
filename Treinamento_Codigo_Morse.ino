#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

bool estavaApertado = false;
unsigned long inicio = 0;

bool finalizou = true;

int valor;
String morse;
String morseC;

const char* LETRAS[26] = {".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--.."};

bool jaMudou = false;
unsigned long ultimaAtividade = 0;

void setup() {
  Wire.begin(21,22);
  lcd.init();
  lcd.backlight();

  Serial.begin(115200);
  pinMode(4, INPUT_PULLUP);
  pinMode(18, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);

  digitalWrite(5,HIGH);
  delay(500);
  digitalWrite(5, LOW);

}

void loop() {
  if (finalizou) {
    lcd.clear();
    morse = "";
    lcd.setCursor(0, 0);
    valor = random(0,26);
    lcd.print((char)(valor+65));
    morseC = LETRAS[valor];
    lcd.setCursor(0,1);
    finalizou = false;
    ultimaAtividade = millis();
    jaMudou = false;
  }

  bool apertado = (digitalRead(4) == LOW);

  if (apertado && !estavaApertado) { 
    ultimaAtividade = millis();
    jaMudou = false;     
    inicio = millis();
    digitalWrite(18, HIGH);
  }

  if (!apertado && estavaApertado) {        
    digitalWrite(18, LOW);
    unsigned long duracao = millis() - inicio;
    if (duracao > 10) {
      if (duracao <= 150) {
        morse += ".";
        lcd.print(".");
      } else {
        morse += ("-");
        lcd.print("-");
      }
    }
  }

  if (!apertado && !jaMudou && millis() - ultimaAtividade >= 3000) {
    if (morse == morseC) {
      digitalWrite(18, HIGH);
      digitalWrite(2, HIGH);
      delay(100);
      digitalWrite(18, LOW);
      digitalWrite(2, LOW);
      delay(100);
      digitalWrite(18, HIGH);
      digitalWrite(2, HIGH);
      delay(100);
      digitalWrite(18, LOW);
      digitalWrite(2, LOW);
    } else {
      digitalWrite(18, HIGH);
      digitalWrite(5, HIGH);
      delay(300);
      digitalWrite(18, LOW);
      digitalWrite(5, LOW);
    }
    finalizou = true;
    jaMudou = true;
  }

  estavaApertado = apertado;
}