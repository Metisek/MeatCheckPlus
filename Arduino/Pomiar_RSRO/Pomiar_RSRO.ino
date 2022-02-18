float RL = 8700;  //The value of resistor RL 
float analog_value = 0;
float VRL;
float Rs;
float Ro,k;

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

void setup() //Runs only once
{
dallas(10,1);
Serial.begin(9600); //Initialise serial COM for displaying the value
}

void loop() 
{
for(int test_cycle = 1 ; test_cycle <= 200 ; test_cycle++) 
//Read the analog output of the sensor for 200 times

{
analog_value = analog_value + analogRead(A0); //add the values for 200
}

analog_value = analog_value/200.0;
Serial.print("Analog value at fresh air = ");
Serial.println(analog_value);

//Convert analog value to voltage
VRL = analogRead(A0)*(5.0/1023.0);

//RS = ((Vc/VRL)-1)*RL is the formulae we obtained from datasheet
Rs = ((5.0/VRL)-1) * RL;
Serial.print("Rs at fresh air = ");
Serial.println(Rs);

//Temperatura jest wymagana do poprawnego wyliczenia zależności RS/R0

float currentTemp = dallas(10,0)/4.0;
float PomiarTemp = ((0.001*pow(currentTemp, 1.95))/0.7 * (-((0.006*currentTemp)+0.1))+1.1);
//Rs/Ro is k as we obtained from graph of datasheet

Serial.print("Ratio Rs/Ro:");
Serial.println(PomiarTemp);
Ro = Rs/PomiarTemp;

Serial.print("Ro at fresh air = ");
Serial.println(Ro); //Display calculated Ro
delay(2500); 
}
