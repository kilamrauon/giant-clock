/*******************station meteo*************************

  wemos d1 et afficheur led 4 chiffres de 7 segments tm1637

 **************************************************************/


#include <FastLED.h>

#include "RTClib.h"

RTC_DS1307 rtc;  //D1 scl d2 sca pour wemos mini d1
// How many leds in your strip?
#define LED_PIN     D4
#define NUM_LEDS    172
#define BRIGHTNESS  16
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100
#define LOGICIEL "horloge_wemos_mini_ledstrip"
// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI



//nécessaire wifi et mail #include <time.h>


// nécessaire pour le wifi avec
// point d'accès pour pouvoir se connecter
// sur n'importe quel réseau en se connectant d'abord sur 192.168.4.1

//#include <ESP8266WebServer.h>
//#include <EEPROM.h>
//#include <ESP8266WiFi.h>
//#include <DNSServer.h>
//#include <WiFiManager.h>

//#include <ESP_Mail_Client.h>
//#define SMTP_HOST "smtp.gmail.com"
//#define SMTP_PORT 465

/* The sign in credentials */
//#define AUTHOR_EMAIL "xxxxxxxxxxxxxxxxxx"
//#define AUTHOR_PASSWORD "xxxxx"

/* Recipient's email*/
//#define RECIPIENT_EMAIL "xxxxxxxxxxxxxxxxxxxxxxxxxx"

/* The SMTP Session object used for Email sending */
//SMTPSession smtp;

/* Callback function to get the Email sending status */
//void smtpCallback(SMTP_Status status);



/*
  ESP8266WebServer  serverweb(8080);
  HTTPClient http;
  WiFiClient wifiClient;
*/
String ssid, password;


//int decalage = 2; // la valeur dépend de votre fuseau horaire. Essayez 2 pour la France.
/*const int delaiDemande = 10; // nombre de secondes entre deux demandes consécutives au serveur NTP

unsigned long derniereDemande = millis(); // moment de la plus récente demande au serveur NTP
unsigned long derniereMaJ = millis(); // moment de la plus récente mise à Jour de l'affichage de l'heure
time_t maintenant;
struct tm * timeinfo;*/

int bouton4 = 1024, bouton3 = 885, bouton2 = 662, bouton1 = 448;


int decalage = 1;

int clavier = A0, touche;




int heure;
int jour, seconde, mois, annee, minute1;
int lum = BRIGHTNESS;

//variables pour l'affichage
int largeur_segment = 6;
int decalage_separateur = 4;
int couleur_actuelle = 6;
long couleur = CRGB::Magenta;
int coul[] = {CRGB::White, CRGB::Blue, CRGB::Red, CRGB::Green, CRGB::Orange, CRGB::Magenta};
int nb_couleur = 5;
bool changeColor =0;


void maj_rtc() {
  timeinfo = localtime(&maintenant);




  heure = timeinfo->tm_hour;
  minute1 = timeinfo->tm_min;
  seconde = timeinfo->tm_sec ;
  jour = timeinfo->tm_mday ;

  mois = timeinfo->tm_mon + 1; //localtime(&maintenant) renvoie le mois de 0 à 11 - rtclib compte à partir de 1 jusqu'à 12
  annee = timeinfo->tm_year + 1900;

  rtc.adjust(DateTime(annee, mois, jour, heure, minute1, seconde));

}

void setup() {

  // initialisation des entrées et sorties
  Serial.begin(115200);

  /* // WiFiManager
    // Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;

    // Uncomment and run it once, if you want to erase all the stored information
    //wifiManager.resetSettings();

    //variables ssid et password
    ssid = wifiManager.getWiFiSSID();
    password = wifiManager.getWiFiPass();

    // set custom ip for portal
    //wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

    // fetches ssid and pass from eeprom and tries to connect
    // if it does not connect it starts an access point with the specified name
    // here  "AutoConnectAP"
    // and goes into a blocking loop awaiting configuration
    wifiManager.autoConnect("AutoConnectAP");
    // or use this for auto generated name ESP + ChipID
    //wifiManager.autoConnect();

    // if you get here you have connected to the WiFi
    Serial.println("Connected.");
    Serial.print("Nom du programme : ");
    Serial.println(LOGICIEL);
    //  server.begin();

    /* EEPROM.begin(704);
    EEPROM.get(700,decalage);*/
  /* configTime(decalage * 3600, 3600, "fr.pool.ntp.org");  //serveurs canadiens
    // en Europe, essayez europe.pool.ntp.org ou fr.pool.ntp.org

    Serial.print("Attente date et heure");

    while (time(nullptr) <= 100000) {
     Serial.print(WiFi.status());

     delay(1000);


    }



    time(&maintenant);

    /** Enable the debug via Serial port
      none debug or 0
      basic debug or 1
  */
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
  }
  // following line sets the RTC to the date & time this sketch was compiled
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));


  //maj_rtc(); 

  Serial.print("decalage : " );
  Serial.println(decalage);


  Serial.print("Logiciel utilisé : ");
  Serial.println(LOGICIEL);

  delay(3000); // sanity delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );

couleur = random(256*256*256);
  Serial.println("fin setup");

}

void affiche_chiffre(int chiffre, int position_affichage, int separateur)
{
  separateur = separateur * decalage_separateur;
  position_affichage = position_affichage * largeur_segment * 7 + separateur ;
  //Serial.println(chiffre);
  //  Serial.println(position_affichage);
  if (chiffre == 0)

  {
    affiche_segment1(position_affichage, separateur);
    affiche_segment2(position_affichage, separateur);
    affiche_segment3(position_affichage, separateur);

    affiche_segment5(position_affichage, separateur);
    affiche_segment6(position_affichage, separateur);
    affiche_segment7(position_affichage, separateur);

  }

  if (chiffre == 1)

  {

    affiche_segment3(position_affichage, separateur);

    affiche_segment7(position_affichage, separateur);

  }

  if (chiffre == 2)

  {
    //affiche_segment1(position_affichage, separateur);
    affiche_segment2(position_affichage, separateur);
    affiche_segment3(position_affichage, separateur);
    affiche_segment4(position_affichage, separateur);
    affiche_segment5(position_affichage, separateur);
    affiche_segment6(position_affichage, separateur);
    //  affiche_segment7(position_affichage, separateur);

  }

  if (chiffre == 3)

  {
    // affiche_segment1(position_affichage, separateur);
    affiche_segment2(position_affichage, separateur);
    affiche_segment3(position_affichage, separateur);
    affiche_segment4(position_affichage, separateur);
    //affiche_segment5(position_affichage, separateur);
    affiche_segment6(position_affichage, separateur);
    affiche_segment7(position_affichage, separateur);

  }

  if (chiffre == 4)

  {
    affiche_segment1(position_affichage, separateur);
    // affiche_segment2(position_affichage, separateur);
    affiche_segment3(position_affichage, separateur);
    affiche_segment4(position_affichage, separateur);
    // affiche_segment5(position_affichage, separateur);
    // affiche_segment6(position_affichage, separateur);
    affiche_segment7(position_affichage, separateur);

  }

  if (chiffre == 5)

  {
    affiche_segment1(position_affichage, separateur);
    affiche_segment2(position_affichage, separateur);
    // affiche_segment3(position_affichage, separateur);
    affiche_segment4(position_affichage, separateur);
    // affiche_segment5(position_affichage, separateur);
    affiche_segment6(position_affichage, separateur);
    affiche_segment7(position_affichage, separateur);

  }
  
  if (chiffre == 6)
  {
    affiche_segment1(position_affichage, separateur);
    affiche_segment2(position_affichage, separateur);
    // affiche_segment3(position_affichage, separateur);
    affiche_segment4(position_affichage, separateur);
    affiche_segment5(position_affichage, separateur);
    affiche_segment6(position_affichage, separateur);
    affiche_segment7(position_affichage, separateur);
  }

  if (chiffre == 7)
  {
    // affiche_segment1(position_affichage, separateur);
    affiche_segment2(position_affichage, separateur);
    affiche_segment3(position_affichage, separateur);
    /* affiche_segment4(position_affichage, separateur);
      affiche_segment5(position_affichage, separateur);
      affiche_segment6(position_affichage, separateur);*/
    affiche_segment7(position_affichage, separateur);

  }

  if (chiffre == 8)

  {
    affiche_segment1(position_affichage, separateur);
    affiche_segment2(position_affichage, separateur);
    affiche_segment3(position_affichage, separateur);
    affiche_segment4(position_affichage, separateur);
    affiche_segment5(position_affichage, separateur);
    affiche_segment6(position_affichage, separateur);
    affiche_segment7(position_affichage, separateur);

  }

  if (chiffre == 9)

  {
    affiche_segment1(position_affichage, separateur);
    affiche_segment2(position_affichage, separateur);
    affiche_segment3(position_affichage, separateur);
    affiche_segment4(position_affichage, separateur);
    //affiche_segment5(position_affichage, separateur);
    affiche_segment6(position_affichage, separateur);
    affiche_segment7(position_affichage, separateur);

  }


}

void affiche_segment1(int position_affichage, int separateur)
{
  for (int i = 0; i < largeur_segment; i++)
    leds[i + position_affichage] = couleur;

  /*leds[0 + position_affichage] = couleur;
    leds[1 + position_affichage] = couleur;
    leds[2 + position_affichage] = couleur;
    leds[3 + position_affichage] = couleur;
    leds[4 + position_affichage] = couleur;
    leds[5 + position_affichage] = couleur;
    /* for (int i = 0 ; i < largeur_segment ; i++)
    {
     leds[i + position_affichage + separateur] = couleur;
    }*/

}

void affiche_segment2(int position_affichage, int separateur)
{
  for (int i = largeur_segment; i < largeur_segment * 2; i++)
    leds[i + position_affichage] = couleur;
  /*
    leds[6 + position_affichage] = couleur;
    leds[7 + position_affichage] = couleur;
    leds[8 + position_affichage] = couleur;
    leds[9 + position_affichage] = couleur;
    leds[10 + position_affichage] = couleur;
    leds[11 + position_affichage] = couleur;/*
    for (int i = largeur_segment; i < largeur_segment * 2 ; i++)
    {
    leds[i + position_affichage + separateur] = couleur;
    }*/

}


void affiche_segment3(int position_affichage, int separateur)
{
  for (int i = largeur_segment * 2; i < largeur_segment * 3; i++)
    leds[i + position_affichage] = couleur;
  /*
    leds[12 + position_affichage] = couleur;
    leds[13 + position_affichage] = couleur;
    leds[14 + position_affichage] = couleur;
    leds[15 + position_affichage] = couleur;
    leds[16 + position_affichage] = couleur;
    leds[17 + position_affichage] = couleur;
  */
}

void affiche_segment4(int position_affichage, int separateur)
{
  for (int i = largeur_segment * 3; i < largeur_segment * 4; i++)
    leds[i + position_affichage] = couleur;
  /*
    leds[18 + position_affichage] = couleur;
    leds[19 + position_affichage] = couleur;
    leds[20 + position_affichage] = couleur;
    leds[21 + position_affichage] = couleur;
    leds[22 + position_affichage] = couleur;
    leds[23 + position_affichage] = couleur;*/
}


void affiche_segment5(int position_affichage, int separateur)
{
  for (int i = largeur_segment * 4; i < largeur_segment * 5; i++)
    leds[i + position_affichage] = couleur;
  /*
    leds[24 + position_affichage] = couleur;
    leds[25 + position_affichage] = couleur;
    leds[26 + position_affichage] = couleur;
    leds[27 + position_affichage] = couleur;
    leds[28 + position_affichage] = couleur;
    leds[29 + position_affichage] = couleur;
  */
}


void affiche_segment6(int position_affichage, int separateur)
{
  for (int i = largeur_segment * 5; i < largeur_segment * 6; i++)
    leds[i + position_affichage] = couleur;
  /*
    leds[30 + position_affichage] = couleur;
    leds[31 + position_affichage] = couleur;
    leds[32 + position_affichage] = couleur;
    leds[33 + position_affichage] = couleur;
    leds[34 + position_affichage] = couleur;
    leds[35 + position_affichage] = couleur;*/
}

void affiche_segment7(int position_affichage, int separateur)
{
  for (int i = largeur_segment * 6; i < largeur_segment * 7; i++)
    leds[i + position_affichage] = couleur;
  /*
    leds[36 + position_affichage] = couleur;
    leds[37 + position_affichage] = couleur;
    leds[38 + position_affichage] = couleur;
    leds[39 + position_affichage] = couleur;
    leds[40 + position_affichage] = couleur;
    leds[41 + position_affichage] = couleur;*/
}





void loop()
{

  DateTime now = rtc.now();


  Serial.print(now.day(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.year(), DEC);
  Serial.print(" --- ");
  //    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  //    Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  heure = now.hour();
  minute1 = now.minute();

  // calculate a date which is 7 days, 12 hours, 30 minutes, and 6 seconds into the future
  //  DateTime future (now + TimeSpan(7,12,30,6));

  Serial.println();
  delay(50);
  touche = analogRead(clavier);

  //ajoute 1 heure
  if (touche >= bouton1 - 10 && touche <= bouton1 + 10)
  {
    heure = now.hour() + 1;
    if (heure > 23) heure = 0;
    rtc.adjust(DateTime( now.year() , now.month(), now.day() , heure , now.minute() , now.second() ) ) ;
delay(500);
  }

  //ajoute 1 minute 

  if (touche >= bouton2 - 10 && touche <= bouton2 + 10)
  {
    minute1 = now.minute() + 1;
    if (minute1 < 0) minute1 = 59;
    rtc.adjust(DateTime( now.year() , now.month(), now.day() , now.hour() , minute1 , now.second() ) ) ;
delay(500);
  }


  //luminosite +
  if (touche >= bouton3 - 10 && touche <= bouton3 + 10)
  {
    lum++;

    if (lum > 100) lum = 1;
    FastLED.setBrightness( lum );
    //   Serial.println(lum);
    delay(100);
  }

  Serial.print ("couleur_actuelle : ");
  Serial.print(couleur_actuelle);
  Serial.print ("numero couleur : ");
  Serial.println(couleur);

  //couleur
  if (couleur_actuelle == nb_couleur+1 && !changeColor) 
  {
    changeColor =1;
  couleur =random(256*256*256);
  }

  if (changeColor)
    {
      couleur+=256;
      if (couleur > 167772165) couleur = 0;
    }
    
  if (touche >= bouton4 - 10 && touche <= bouton4 + 10)
  {
    couleur_actuelle++;
    Serial.println(couleur_actuelle);
    

    if (couleur_actuelle > nb_couleur+1) 
    {
      couleur_actuelle = 0;
      changeColor = 0;
    }


    couleur = coul[couleur_actuelle];
    // Serial.println(couleur);
//long randomColor = random(256*256*256);
 delay(200);
  }

  for (int i = 0 ; i < NUM_LEDS; i++)
    leds[i] = 0;
  /*for (int j = 0; j < 10; j++)
    {
     affiche_chiffre(j, 3, 1);
     FastLED.show();
     delay(500);
     for (int i = 0 ; i < NUM_LEDS; i++)
     leds[i] = 0;
    }*/

  //affichage heure
  // heure = now.hour();
  if (heure > 9) affiche_chiffre (heure / 10, 0, 0);

  affiche_chiffre (heure % 10, 1, 0);

  //affichage minute

  // minute1=now.minute();
  if (minute1 < 10) affiche_chiffre (0, 2, 1);
  else affiche_chiffre (minute1 / 10, 2, 1);

  minute1 = minute1 % 10;
  affiche_chiffre (minute1, 3, 1);

  if (now.second() % 2)
  {
    for (int k = 7 * 2 * largeur_segment; k < 7 * 2 * largeur_segment + decalage_separateur; k++)
      leds[k] = couleur;
  }
  FastLED.show();
  // delay(100);
}
