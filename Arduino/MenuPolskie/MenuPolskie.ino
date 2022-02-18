/*  Proste MENU dla Arduino ...by wojtekizk@warcaby.lh.pl
-------------------------------------------------------------------------------------------
Opis działania:
Za pomocą 3 przycisków: UP, DOWN i OK budujemy dośc spore menu.
W tym przykładzie nasze głowne MENU ma 5 opcji, a każda z opcji ma swoje 
SUBMENU. W sumie mamy 31 opcji do wyboru. Naturalnie potem w programie każdy 
taki wybór trzeba obsłuzyć np. w konstrukcji switch - case, ale to juz drugorzędna sprawa.
Masz tu przykład jak budować sobie takie nawet spore menu. 
Poniższy przykład jest opatrzony dość szczegółowym komentarzem. Dla prezentacji samego
MENU wykorzystujemy tu komunikację po Serialu, ale bez problemu można ten kod
łatwo zaadoptować na wyświetlacz LCD.
Klawiszami UP - DOWN poruszamy się po poszczególnych opcjach, a klawiszem OK
zatwierdzamy wybór. Symbol "<<<-" oznacza powrót do głównego menu.
--------------------------------------------------------------------------------------------
Instrukcja:
1) 3 MIKROSWITCHE podepnij do pinów 2,3,4 Twojego Arduino, drugi biegun do masy.
2) Wgraj ten szkic na swoje Arduino
3) Odpal Serial i patrz co się dzieje jak będziesz bawił się przyciskami :-)
 */
// --- definicje przycisków ------------------------------------
#define UP 7       // przycisk  UP na Pin3
#define DOWN 6     // przycisk  DOWN na pinie 2
#define OK 8       // przycisk  OK na pinie 4
#define BACK 5     
// --- zmienne: ------------------------------------------------
String s[50]={     // tablica wszystkich opcji (tutaj przesadziłem z tym 50, bo jest ich 31)
  "PLIK","EDYCJA","SZKIC","NARZEDZIA","POMOC",                   // poziom 0 MENU GŁÓWNE
  "<<<-","Nowy","Otworz","Zapisz","Zamknij",                     // poziom 1 Submenu dla PLIK
  "<<<-","Kopiuj","Wkej","Wytnij","Szukaj","Zamien",             // poziom 2 Submenu dla EDYCJA
  "<<<-","Sprawdz","Wgraj","Export","Import",                    // poziom 3 Submenu dla SZKIC
  "<<<-","Archiwum","Port","Plytka","Programator","Wypal",       // poziom 4 Submenu dla NARZEDZIA
  "<<<-","Samouczek","Pytanie","O nas"};                         // poziom 5 Submenu dla POMOC
const int Minimum[6] ={0,5,10,16,21,27};   // tablica położeń dla pierwszej opcji dla danego poziomu
const int Maximum[6]={4,9,15,20,26,30};   // tablica położeń dla ostatniej opcji dla danego poziomu
volatile int ak;                  // numer aktualnej opcji
volatile int poziom;              // numer aktualnego poziomu menu 
int minimum,maximum;                      // minimum i maximum dla danego poziomu, czyli pierwsza i ostatnia opcja 
int pop=-1;                        // zmienna pomocnicza do sprawdzenia czy nastąpiła zmiana w MENU


int co()            //funkcja zwraca wartość równą indeksowi tablicy s (lub -1)
{
maximum=Maximum[poziom];    // określamy granicę przesuwania menu dla klawisza UP
minimum=Minimum[poziom];    // określamy granicę przesuwania menu dla klawisza UP
if(digitalRead(DOWN)==HIGH){delay(40);if(digitalRead(DOWN)==HIGH) {ak--;if(ak<minimum)ak=minimum;}}  // jeśli DOWN
if(digitalRead(UP)==HIGH){delay(40);if(digitalRead(UP)==HIGH) {ak++;if(ak>maximum)ak=maximum;}}      // jeśli UP
if(digitalRead(OK)==HIGH){delay(40);if(digitalRead(OK)==HIGH)                                // jeśli OK
  { 
  if(poziom ==0 && ak==Minimum[0]){poziom=1;ak=Minimum[1]+1;return -1;}            // takie tam sztuczki :-)         
  if(poziom ==0 && ak>Minimum[0]){poziom=ak+1;ak=Minimum[poziom]+1;return -1;}     // takie tam sztuczki :-) 
  if(poziom>0 && ak>Minimum[poziom]){return ak;}                               // takie tam sztuczki :-) 
  if(poziom>0 && ak==Minimum[poziom]){ak=poziom-1;poziom=0;return -1;}         // takie tam sztuczki :-) 
  } 
}
return -1;                                // jak nic nie naciśnięto to jednak coś trza zwrócić   :-)
}
// --- funkcja do prezentacji MENU na Serialu --------------------------
void pokazSerial()
{
  for(int i=Minimum[poziom];i<=Maximum[poziom];i++)    // wyświetlam wszystkie aktualne opcje
    { 
      if(i==ak) {Serial.print(" [ ");Serial.print(s[i]);Serial.print(" ] ");} // jeśli ta opcja jest aktywna
                                                                   // to wyświetam ją w nawiasie kwadratowym
      else Serial.print(s[i]);                                     // jeśli nie to bez nawiasu
      Serial.print(" ");                                           // odstęp 
    }
    Serial.println();                                              // nowa linia
} 
// ---------------------------------------------------------------------
void setup()
{
 Serial.begin(9600);            // inicjalizacja Seriala
 pinMode(DOWN,INPUT);    // konfiguracja pinów dla przycisków
 pinMode(UP,INPUT);      // konfiguracja pinów dla przycisków
 pinMode(OK,INPUT);      // konfiguracja pinów dla przycisków
 ak=0; poziom=0;                // parametry początkowe MENU
 maximum=Maximum[poziom];               // index dla ost. pozycji menu
 minimum=Minimum[poziom];               // index dla pierwszej pozycji menu
 }


// === neverending story ===============================================
void loop()
{
  //  --- dla testów pokazujemy raport na Serialu : ---------------
  if(co()!=-1)
  {
    Serial.print("Wybrano klawisz OK --- Wykonuje Akcje dla opcji:  ");
    Serial.println(s[ak]);   // wyświetlam komunikat o podjętej akcji
  }
  //  --- ale  w programie korzystamy z konstrukcji switch - case ----
  //  --- de facto to tu trzeba obsłuzyć wszystkie opcje :-)
  /* 
  switch(co())
  {
  case 6:      // funkcja obsługi opcji "Nowy"
    break;
  case 7:     //  funkcja obsługi opcji "Otworz"
    break;
  // itd .... pozostałe funkcje obsługi .........  
  }
   */
  if(pop!=ak) {pokazSerial();pop=ak;}  // aby nie śmiecić na serialu, wyświetlam tylko zmiany :-)
  delay(120);                          // aby był czas na oderwanie palucha od przycisku :-)
                      // ten delay dobrać doświadczalnie, bo osoby z Parkinsonem mają refleks :-)
}
// === KONIEC ===========================================================
