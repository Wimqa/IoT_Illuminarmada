  // ==================================================================== NEOPIXEL LED =========================================================================

// Librairie pour la Led neopixel
#include <Adafruit_NeoPixel.h> // librairie pour les neopixel

// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN D2    // Pin digitale d'où on envoie le signal vers les leds, 
#define PIXEL_COUNT 1  // nbr de led dans la bande
#define PIXEL_TYPE NEO_GRB + NEO_KHZ800

//instanciation de l'objet strip
Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);




// Mes variables pour le Neo + Thermometre :

// Variables pour la temperature :
//int reading;
//float voltage;
float temperatureC;
char receiveStringTemperature [30];


// Variables pour le rgb et clignotement

bool blink = false;   // booléen pour le clignotement ; true = clignotement ; false = continu
int mappedGreen =0;
int mappedBlue =0;
int mappedRed =0;  

int tempMinBlue;    // Les 4 variables ici définissent les intervalles de températures pour lequel on est en bleu (froid) ou en rouge (chaud)
int tempMaxBlue;
int tempMinRed;
int tempMaxRed;

float borneInfTempIdeal ;  // Les 2 variables ici détermine l'intervalle de témpérature idéale où on clignotera en vert
float borneSupTempIdeal ;

//===================================================================  FIN NEOPIXEL LED ===========================================================================







//===================================================================  MQTT PROTOCOLE =============================================================================

// Librairies pour le MQTT 
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>



// Mes variables pour le MQTT :


const char* ssid = "PROXIMUS";  // nom du sous-réseau
const char* password = "digitalent"; // mdp du sous-réseau


const char* mqttServer = "broker.hivemq.com";  // serveur public open test, à changer après sur un serveur privé pour plus de sécurité
const int mqttPort = 1883;  // par défaut 1883
const char* mqttUser = "Jean";
const char* mqttPassword = "Poche";


WiFiClient espClient;  // instanciation de l'objet espClient de type WiFiClient issu de la library
PubSubClient client(espClient);  // instanciation de l'objet client de type PubSubClient issu de la library  
// l'instanciation de l'objet de type PubSubClient peut être surchargée avec plus d'arguments


//char* buff_msg[32]; // mqtt message


//=================================================================== FIN MQTT PROTOCOLE =============================================================================




//=================================================================== SON VIA BUZZER  =============================================================================


//#define BUZZER_PIN D7
//const int buzzer = 9;
const int buzzer = D5; //Define pin 10, can use other PWM pins  (5,6 or 9)
//Note pins 3 and 11 can't be used when using the tone function in Arduino Uno
const int songspeed = 1.5; //Change to 2 for a slower version of the song, the bigger the number the slower the song
//*****************************************
//Defining note frequency
#define NOTE_C4  262   
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784
#define NOTE_A5  880
#define NOTE_B5  988

//*****************************************
int mesNotes[] = {       //Note of the song, 0 is a rest/pulse  (1 est le rest/pulse pour wemos)
//================================================  REMARQUE : POUR LA WEMOS, ON A DU REMPLACER TOUS LES 0 PAR DES 1 SINON CA CALAIT AU 1er 0 ==========================
  NOTE_C5, NOTE_G4, NOTE_E4,NOTE_C4,1
};

int mesDurations[] = {         //duration of each note (in ms) Quarter Note is set to 250 ms
  250, 250, 250, 250,125 
};

/*
int mesNotes2[] = {       //Note of the song, 0 is a rest/pulse  (1 est le rest/pulse pour wemos)
//================================================  REMARQUE : POUR LA WEMOS, ON A DU REMPLACER TOUS LES 0 PAR DES 1 SINON CA CALAIT AU 1er 0 ==========================
  NOTE_G4, 1, NOTE_G4, 1, NOTE_G4,NOTE_A4, NOTE_B4
};

int mesDurations2[] = {         //duration of each note (in ms) Quarter Note is set to 250 ms
  250, 125, 250, 125, 500, 250, 250 
};
*/

  
int notes[] = {       //Note of the song, 0 is a rest/pulse  (1 est le rest/pulse pour wemos)
//================================================  REMARQUE : POUR LA WEMOS, ON A DU REMPLACER TOUS LES 0 PAR DES 1 SINON CA CALAIT AU 1er 0 ==========================
  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 1,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 1,
  NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 1,
  NOTE_A4, NOTE_G4, NOTE_A4, 1,

  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 1,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 1,
  NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 1,
  NOTE_A4, NOTE_G4, NOTE_A4, 1,

  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 1,
  NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, 1,
  NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, 1,
  NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, 1,

  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 1,
  NOTE_D5, NOTE_E5, NOTE_A4, 1,
  NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, 1,
  NOTE_C5, NOTE_A4, NOTE_B4, 1,


  NOTE_A4, NOTE_A4,
  //Repeat of first part
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 1,
  NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 1,
  NOTE_A4, NOTE_G4, NOTE_A4, 1,

  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 1,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 1,
  NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 1,
  NOTE_A4, NOTE_G4, NOTE_A4, 1,

  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 1,
  NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, 1,
  NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, 1,
  NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, 1,


  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 1,
  NOTE_D5, NOTE_E5, NOTE_A4, 1,
  NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, 1,
  NOTE_C5, NOTE_A4, NOTE_B4, 1,
  //End of Repeat

  NOTE_E5, 1, 1, NOTE_F5, 1, 1,
  NOTE_E5, NOTE_E5, 1, NOTE_G5, 1, NOTE_E5, NOTE_D5, 1, 1,
  NOTE_D5, 1, 1, NOTE_C5, 1, 1,
  NOTE_B4, NOTE_C5, 1, NOTE_B4, 1, NOTE_A4,

  NOTE_E5, 1, 1, NOTE_F5, 1, 1,
  NOTE_E5, NOTE_E5, 1, NOTE_G5, 1, NOTE_E5, NOTE_D5, 1, 1,
  NOTE_D5, 1, 1, NOTE_C5, 1, 1,
  NOTE_B4, NOTE_C5, 1, NOTE_B4, 1, NOTE_A4
};



//*****************************************
int duration[] = {         //duration of each note (in ms) Quarter Note is set to 250 ms
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125,

  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125,

  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 125, 250, 125,

  125, 125, 250, 125, 125,
  250, 125, 250, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 375,

  250, 125,
  //Rpeat of First Part
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125,

  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125,

  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 125, 250, 125,

  125, 125, 250, 125, 125,
  250, 125, 250, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 375,

  //End of Repeat

  250, 125, 375, 250, 125, 375,
  125, 125, 125, 125, 125, 125, 125, 125, 375,
  250, 125, 375, 250, 125, 375,
  125, 125, 125, 125, 125, 500,

  250, 125, 375, 250, 125, 375,
  125, 125, 125, 125, 125, 125, 125, 125, 375,
  250, 125, 375, 250, 125, 375,
  125, 125, 125, 125, 125, 500
};



//============================================================== FIN SON VIA BUZZER ===========================================================================








//============================================================== METHODE setup()  =============================================================================

void setup()
{   
    Serial.begin(9600);
        
    //int mappedGreen =0;
    //int mappedBlue =0;
    //int mappedRed =0;  
    
    strip.begin(); // Methode de la librairie - Initialize all pixels to 'off'
    strip.show();  // 

    // Pour la connexion au wifi :  =====================================================================================================================
    WiFi.begin(ssid, password); // on try la connexion au wifi

    // on check si le wifi est connecté sinon on affiche connecting
    while(WiFi.status()!= WL_CONNECTED)
    {
     delay(500);
     Serial.println("Connecting to WiFi..");
    }  
    Serial.println("Connected to the WiFi network");
  
    // Pour set l'adresse et le port du serveur MQTT : ===================================================================================================
    client.setServer(mqttServer, mqttPort);
  
    // Pour dire au programme ce qu'il doit faire avec les messages MQTT entrants : ========================================================================
    //setCallback method of the PubSub object to set a function that is executed when a MQTT message is received. Traditionally this function is called ‘callback’, but you can give it any other name
    // on passe donc en argument de setCallback une autre méthode appelée généralement callback
    client.setCallback(callback);
  
    // Pour se connecter au serveur MQTT ( de pref dans une boucle jusqu'au succès de la connexion)========================================================
    while (!client.connected())  // tant que client.connected retourne false, on boucle  => client.connected devient true quand client.connect() retourne true
    {
      Serial.println("Lighthhouse connecting to MQTT...");
      if (client.connect("MQTT-LA-LAMPE-88", mqttUser, mqttPassword ))
      //if (client.connect("ESP8266Client"))
      {
        Serial.println("connected");
      }
      else
      {
        Serial.println("failed with state ");
        Serial.println(client.state());
        delay(2000);
      }
    }
    
    // Pour subscribe à un topic pour informer le broker quels messages d'autres publishers on veut recevoir =============================================================
    // we do that with the subscribe method, with the topic name as argument. We can take any topic we desire, but for now lets subscribe to the same topic as we publish.
  
    client.subscribe("Test/Illuminarmada");
  
    // Pour publier un premier message MQTT style hello world pour tester ========================================================================================
    // The setup function is a good place to publish our first MQTT message to tell the world we are there. Kind of an “Hello World” to let the broker know we are on line. 
    // We do this with the publish method, that takes a topic and a message as arguments. As a topic we choose “test/esp8266”, and for a message “Hello ESP world”.
  
    client.publish("Test/Illuminarmada"," LE PHARE EST EN STANDBY ");
    //client.publish("Test/Illuminarmada"," TEMPERATURE = SETUPAGE");
  
    // FAIRE ATTENTION A BIEN METTRE LE MEME NOM DE TOPIC AVEC LES SLASH AU DEBUT
    
    
  
  
     //sprintf(buff_msg, "%d", temperature);
  
     //sprintf(publishStringTemperature, "%.2f", temperatureC);
     // mettre %.2f pour que 2 decimales, %.3f pour 3 decimales etc
       
     //Particle.publish("Temperature", publishStringTemperature);
     //client.publish("topic", buff_msg);
  
    
  } 
// =================================================================== FIN METHODE setup() =======================================================================
  







 //==================================================================== METHODE loop() ===========================================================================
void loop() 
{   


  //=============================================================== GESTION DES DECONNEXIONS ======================================================================
    client.loop();
    //WiFi.begin(ssid, password); // on try la connexion au wifi

    // on check si le wifi est connecté sinon on affiche connecting
    if (WiFi.status()!=WL_CONNECTED)
    {
      while(WiFi.status()!= WL_CONNECTED)
      {
       delay(500);
       Serial.println("Connecting to WiFi..");
      }  
      Serial.println("Connected to the WiFi network");
    }


    if (!client.connected())
    {
      Serial.println("Pas connecté au broker MQTT");
       while (!client.connected())  // tant que client.connected retourne false, on boucle  => client.connected devient true quand client.connect() retourne true
      {
        Serial.println("Connecting to MQTT...");
        if (client.connect("MQTT-LA-LAMPE-88", mqttUser, mqttPassword ))
        //if (client.connect("ESP8266Client"))
        {
          Serial.println("connected");
          client.subscribe("Test/Illuminarmada");
        }
        else
        {
          Serial.println("failed with state ");
          Serial.println(client.state());
          delay(2000);
        }
      }
    }

    client.loop();


    
    // ==================================== DETERMINATION RGB + CLIGNOTEMENT ======================================================

    //Serial.println(temperatureC);
    //delay(2000);
    // bornes min et max du range de valeurs possibles du thermo 
    tempMinBlue = 26;
    tempMaxBlue = 36;
    
    tempMinRed = 38;
    tempMaxRed=48;
  
    // borne min et max du range de valeurs idéales du thermo
    
    borneInfTempIdeal =36;
    borneSupTempIdeal = 38;  

    //Clignotement violet sur la temperature = 0 pour signifier qu on a tjrs rien reçu
    if(temperatureC==0.00)   // remarque : il faut mettre 0.00 et pas 0,00
    {     
      blink=true;
      mappedGreen=0;
      mappedBlue=126;
      mappedRed=126;
      
    }

    // si on est pas = 0.00, alors on a reçu un message du broker avec une valeur, on passe alors dans le else pour faire des tests sur cette valeur
    else
    {
      //on check si c est dans l intervalle de température idéal, ici [19-20] bornes incluses pour led verte + musique
      if (temperatureC >= float(borneInfTempIdeal) && temperatureC <= float(borneSupTempIdeal)) 
      {
          //blink = true;  // clignotement 
          mappedGreen = 255; // clignotement vert
          mappedBlue = 0;
          mappedRed = 0;
          rainbow(mappedRed, mappedBlue, mappedGreen, blink);
          //music();  
          myMusic();            
      }

      // si on est pas dans l'intervalle, on va determiner si on est en dessous ou en haut => on met le vert à 0 car ca sera soit rouge ou bleu
      else 
      {
        blink = false;
        mappedGreen = 0;
        
        int temperatureCInt = temperatureC;

        //si c'est en dessous de la temp max bleue, on met le rouge à 0
        if( temperatureC < float(tempMaxBlue))
        {
            mappedRed=0; 

            // si c'est au dessus de la temp min bleue, on fait un mapping de la valeur temperatureCINT qui est dans l intervalle ]tempMinBlue,tempMaxBlue[ sur un intervalle de [255,30]
            // donc plus c est froid, plus le bleu sera fort
            if(temperatureC> float(tempMinBlue))
            {
            mappedBlue = map(temperatureCInt,tempMinBlue,tempMaxBlue,255,30); 
            // on mappe la value du thermometre TemperatureCInt dans l'intervalle [tempMinBlue,tempMaxBlue] sur un intervalle de [255-30]
            // donc si on est à 15 dans [14-17] , on aura un mappedBlue très proche de 255.
            //si on est à 17 dans [14-17], on aura un mappedBlue à 30
            // ici on aura mappedGreen à 0, mappedRed à 0 et mappedBlue à une valeur entre 255 et 30 (plus c'est froid, plus c'est proche de 255)
            }

            //si temperatureC est < à tempMinBlue, alors il fait très froid. On met le mappedBlue au max et on bloque, qu il fasse 5 degrés ou - 10 ca sera le meme bleu sans variation
            else
            {
              mappedBlue=255;
            }
        
        }


        // Meme raisonnement que pour le bleu en terme d'if etc, sauf qu ici ca sera rouge
        else if (temperatureC > float(tempMinRed))
        {
    
          mappedBlue=0;

          if(temperatureC < float(tempMaxBlue))
          {
          mappedRed = map(temperatureCInt, tempMinRed,tempMaxRed,30,255);
        // on mappe la value du thermometre TemperatureCInt dans l'intervalle [tempMinRed,tempMaxRed] sur un intervalle de [30-255]
        // ici on aura mappedGreen à 0, mappedBlue à 0 et mappedRed à une valeur entre 30 et 255 (plus c'est chaud, plus c'est proche de 255)
          }
          else
          {
            mappedRed=255;
          }
        }
        //rainbow(mappedRed, mappedBlue, mappedGreen, blink);
    }
  
    //rainbow(mappedRed, mappedBlue, mappedGreen, blink); // methode définie en dessous à laquelle on passe 4 arguments ( R,B,G,blink)
    
  }

  // Pour chaque cas de figure, on a set des mappedRed, mappedBlue, mappedGreen spécifiques. On les envoie ici comme argument à rainbow() qui va se charger de l'allumage
  rainbow(mappedRed,mappedBlue,mappedGreen,blink);
}


// ================================================================  FIN METHODE loop() ======================================================================







// ====================================================================  METHODE rainbow() =======================================================================
void rainbow(int argMappedRed, int argMappedBlue, int argMappedGreen, bool argBlink)
{
  //uint16_t i, j;
  //uint16_t funcvaleur = 255/2;
    int i;
    
    
    for(i=0; i<strip.numPixels(); i++)
    {
        // argument pour la methode qui suit : (NumeroLed, Green, Red, Blue)  donc c'est GRB
        strip.setPixelColor(i, argMappedGreen ,argMappedRed,argMappedBlue);
        strip.show();
    }
    delay(100);
  
    if(argBlink == true)
    {
        for(i=0; i<strip.numPixels(); i++)
        {
            // argument pour la methode qui suit : (NumeroLed, Green, Red, Blue)  donc c'est GRB
            //Music();
            strip.setPixelColor(i, 0,0,0);
            strip.show();
            
        }
        delay(100);
    }
    
    
}   
// =============================================================== FIN METHODE rainbow() =====================================================================






//===============================================================   METHODE callback() ================================================================================
void callback(char* topic, byte* payload, unsigned int length)
{
  
  Serial.println("Message arrived in topic: ");
  Serial.println("Le phare a reçu : ");
  Serial.println(topic);
  Serial.print("Message:");
  char receivedChars [4] ;
  for(int i = 0; i < length; i++) {
    receivedChars[i] = (char)payload[i];
    //Serial.print((char)payload[i]);
  }
   //Serial.println(receivedChars);

    //transfo de char vers float
  temperatureC= atof(receivedChars);
  Serial.println(temperatureC);
  return; 
} 
//============================================================= FIN METHODE callback () ===================================================================================




// ==========================================================  METHODE music() ========================================================================================
void music()
{
  
  for (int i = 0; i <3 ; i++) {       //203 is the total number of music notes in the song
    int wait = duration[i] * songspeed;
    tone(buzzer, notes[i], wait);        //tone(pin,frequency,duration)    
    delay(wait);
  }                        //delay is used so it doesn't go to the next loop before tone is finished playing
  //You can click reset on Arduino to replay the song

}

// ==========================================================  METHODE myMusic() ========================================================================================
void myMusic()
{
  
  for (int i = 0; i <5  ; i++) {       //203 is the total number of music notes in the song
    int wait = mesDurations[i] * songspeed;
    tone(buzzer, mesNotes[i], wait);        //tone(pin,frequency,duration)    
    delay(wait);
  }                        //delay is used so it doesn't go to the next loop before tone is finished playing
  //You can click reset on Arduino to replay the song

}
