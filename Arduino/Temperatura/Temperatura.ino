#include <OneWire.h>
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
void setup (){
  dallas(10,1);
  Serial.begin(9600);
}
void loop (){
  float currentTemp = dallas(10,0)/4.0;
  Serial.println(String("Temperatura: ") + String(currentTemp) + String(" C"));
  delay(1000);
}
