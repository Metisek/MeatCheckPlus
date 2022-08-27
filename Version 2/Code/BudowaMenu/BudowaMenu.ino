enum sampleMenuCommandId
{
  cmdPowrot = 0,
  cmdPomiar,
  cmdWybor,
  cmdKurczak,
  cmdWedlina,
  cmdPiers,
  cmdLencz,
  cmdPoledwica,
  cmdSzynkaGotowana,
};

byte glebokoscMenu = 0;

PROGMEM const char Menu_1[] = "Pomiar na żywo";
PROGMEM const char Menu_2[] = "Wybór pomiaru";
PROGMEM const char Menu_2_1[] = "Kurczak";
PROGMEM const char Menu_2_1_1[] = "Pierś";
PROGMEM const char Menu_2_2[] = "Wędlina";
PROGMEM const char Menu_2_2_1[] = "Lencz";
PROGMEM const char Menu_2_2_2[] = "Poledwica";
PROGMEM const char Menu_2_2_3[] = "Szynka Gotowana";




void setup() {
  Serial.begin(9600);
}

void loop() {
  delay(1000);
  Serial.print(Menu_1);
}
