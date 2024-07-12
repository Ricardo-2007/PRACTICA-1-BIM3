#include <Wire.h>    
#include <LiquidCrystal_I2C.h>

#define IN1  9
#define IN2  10
#define IN3  11
#define IN4  12

#define direccion_lcd 0x27
#define filas 2
#define columnas 16

#define canal0_pot A0 
#define sensor_IR 2
#define sensor_hall 3

int velocidad;
unsigned int valor_adc_pot; 
unsigned long contadorRevoluciones = 0;
unsigned long ultimaMedicion = 0;
bool state_sensorhall = false;
bool state_sensorIR = false;
volatile bool flag = false;

LiquidCrystal_I2C lcdRicardo(direccion_lcd, columnas, filas);

int paso[8][4] =
{
  {1, 0, 0, 0},
  {1, 1, 0, 0},
  {0, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 0},
  {0, 0, 1, 1},
  {0, 0, 0, 1},
  {1, 0, 0, 1}
};

void Interrupcion() {
  contadorRevoluciones++;
}
 
void setup() {
  Serial.begin(9600);
  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(canal0_pot, INPUT);         

  lcdRicardo.init();
  lcdRicardo.backlight();
  lcdRicardo.setCursor(0,0);
  lcdRicardo.print("Contador de rev.");
  lcdRicardo.setCursor(0,1);
  lcdRicardo.print("No. de Rev:");

  pinMode(sensor_IR, INPUT);
  Serial.println("Uso del sensor de obstaculos");

  pinMode(sensor_hall, INPUT); 

  attachInterrupt(digitalPinToInterrupt(sensor_hall), Interrupcion, FALLING);
}

void loop() { 
  if (millis() - ultimaMedicion >= 1000) {
    ultimaMedicion = millis();
    lcdRicardo.setCursor(0,1);
    lcdRicardo.print("No. De Rev:");
    lcdRicardo.print(contadorRevoluciones);
    lcdRicardo.print("");
    contadorRevoluciones = 0; 
  }

  state_sensorIR = digitalRead(sensor_IR);
  if (state_sensorIR == LOW) {
    velocidad = map(analogRead(canal0_pot), 0, 1023, 1, 3);     

    for (int i = 0; i < 8; i++) {
      digitalWrite(IN1, paso[i][0]);
      digitalWrite(IN2, paso[i][1]);
      digitalWrite(IN3, paso[i][2]);
      digitalWrite(IN4, paso[i][3]);
      delay(velocidad);
    }   
  }
}
