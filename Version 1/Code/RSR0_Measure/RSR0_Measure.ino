#include <OneWire.h>

int RL = 10000; //Rezistor used asvoltage divider
float RS;
int Ro = 12000;
float VRL1 = 0;
float VRL2 = 0;
float VRL3 = 0;
float VRL = 0;
float VC = 0;
int czas = 0;

//Fromula for Trimetyl amine: 10*0.04x^-0.9
//1 ppm for 0,39 RS/R0 ratio
//taking for account RS/Ro y = x^-0.8 -1

int16_t dallas(int x,byte start){
    OneWire ds(x);
    byte i;
    byte data[2];
    int16_t result;
    do{
        ds.reset();
        ds.write(0xCC);
        ds.write(0xBE);
        for (int i = 0; i < 2; i++) data[i] = ds.read();
        result=(data[1]<<8) |data[0];
        result >>= 2; if (data[1]&128) result |=61440;
        if (data[0]&8) ++result;
        ds.reset();
        ds.write(0xCC);
        ds.write(0x44, 1);
        if (start) delay(1000);
    } while (start--);
    return result;
}

void setup() {
  dallas(10,1);
  Serial.begin(9600);
}

void loop() {
    float currentTemp = dallas(10,0)/4.0;
    float PomiarTemp = ((0.001*pow(currentTemp, 1.95))/0.7 * (-((0.006*currentTemp)+0.1))+1.1);
    
    Serial.print(currentTemp);
    Serial.print(" ");
    czas += 1;
    VRL1 = (analogRead(A0) / 1024.0) * 5.0;
    delay(30);
    VRL2 = (analogRead(A0) / 1024.0) * 5.0;
    delay(30);
    VRL3 = (analogRead(A0) / 1024.0) * 5.0;
    VC = (analogRead(A5) / 1024.0) * 5.0;
    VRL = (VRL1 + VRL2 + VRL3) / 3.0;

    RS = (VC/VRL -1) * RL;
    float Trimetyloamina = (pow(RS/Ro, -0.7) - 1.04)*PomiarTemp;
    
    Serial.print(czas);
    Serial.print(" ");
    Serial.print(VRL);
    Serial.print(" ");
    Serial.print(VC);
    Serial.print(" ");
    Serial.print(Trimetyloamina);
    Serial.print(" ");
    Serial.println(RS);
    delay(930);
}
