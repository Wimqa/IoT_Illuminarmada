 
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
// voir https://learn.adafruit.com/thermistor/using-a-thermistor

// which analog pin to connect
#define THERMISTORPIN A0         
// resistance at 25 degrees C
#define THERMISTORNOMINAL 10000      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25   
// how many samples to take and average, more takes longer
// but is more 'smooth'
//#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
// the value of the 'other' resistor // on a mis un 10k ohms
#define SERIESRESISTOR 10000    

// Variables pour la temperature :
float reading;
float steinhart;
float voltage;
float temperatureC;
char publishStringTemperature [30];














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
      Serial.println("Boat connecting to MQTT...");
      if (client.connect("MQTT-LE-BATEAU-88", mqttUser, mqttPassword ))
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
  
    client.publish("Test/Illuminarmada"," LE BATEAU LARGUE LA THERMO-ANCRE ");
    //client.publish("Test/Illuminarmada"," TEMPERATURE BIENTOT MESUREE ");
  
    // FAIRE ATTENTION A BIEN METTRE LE MEME NOM DE TOPIC AVEC LES SLASH AU DEBUT
    
    
  
  
     //sprintf(buff_msg, "%d", temperature);
  
     //sprintf(publishStringTemperature, "%.2f", temperatureC);
     // mettre %.2f pour que 2 decimales, %.3f pour 3 decimales etc
       
     //Particle.publish("Temperature", publishStringTemperature);
     //client.publish("topic", buff_msg);
  
    
  } // ========================================= FIN METHODE setup() ===========================================================
  



void loop() //================================== DEBUT METHODE loop() ========================================================
{   

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

    

    

    // ======================================  VALEUR TEMPERATURE =======================================================================
    
    // lecture de la valeur récupérée par le thermomètre + petit délai entre les lectures    
    reading = analogRead(A0)+1;
    //reading = analogRead(THERMISTORPIN);
    Serial.print("Le reading : ");Serial.println(reading);
    delay(1000);

    // Methode la plus OP : voir https://learn.adafruit.com/thermistor/using-a-thermistor
    reading = 1023 / reading - 1;
    //reading = SERIESRESISTOR / reading;
    reading = 10000 / reading;
  Serial.print("Thermistor resistance "); 
  Serial.println(reading);
  
  
  //steinhart = reading / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = reading / 10000;
  steinhart = log(steinhart);                  // ln(R/Ro)
  //steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart /= 3950;
  //steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart += 1.0 / (25+273.15);
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15; 

  temperatureC = steinhart;
  Serial.print("Steinhart : "); Serial.println(steinhart);
  Serial.print("Temperature "); 
  Serial.print(temperatureC);
  Serial.println(" *C");
 
    // converting that reading to voltage, for 3.3v arduino use 3.3
    // Comme on utilise 5v, on multiplie par *5.0
    //voltage = reading * 3.3;
    //Serial.print("voltage : ");Serial.println(voltage);
    //Serial.print("reading : ");Serial.println(reading);
    
    // POUR ARDUINO :
    //voltage /= 1024.0; 
    // POUR PHOTON PARTICLE
    //voltage /= 4096.0; 
     
    // now print out the temperature
        
    //temperatureC = (voltage - 0.5) * 10 ;  //converting from 10 mv per degree wit 500 mV offset
                                                  //to degrees ((voltage - 500mV) times 100)
    
    
    // publier la valeur de temperatureC : => on doit transformer la valeur de type float en une chaine de caractères
    
    //string publisStringTemperature = null;
    // remarque : string existe pas, il faut faire un tableau de char
    //char publishStringTemperature[40] = "";
        
   //sprintf(publixhStringTemperature, "%.2f", temperatureC);  // fonctionne pas et envoie en msg %.2f
    
    //sprintf(publishStringTemperature,"", temperatureC);
    // mettre %.2f pour que 2 decimales, %.3f pour 3 decimales etc   

    dtostrf(temperatureC, 4, 2, publishStringTemperature);  //4 is mininum width, 6 is precision
     
   
    Serial.print("temperatureC : ");Serial.println(temperatureC);
    client.publish("Test/Illuminarmada",publishStringTemperature);
     
    // ====================================  FIN VALEUR TEMPERATURE ==================================================================

    // ==================================== DETERMINATION RGB + CLIGNOTEMENT ========================================================

    
    // bornes min et max du range de valeurs possibles du thermo 
    tempMinBlue = 26;
    tempMaxBlue = 36;
    
    tempMinRed = 38;
    tempMaxRed=48;
  
    // borne min et max du range de valeurs idéales du thermo
    
    borneInfTempIdeal = 36;
    borneSupTempIdeal = 38;  


        //Clignotement violet sur la temperature = 0 pour signifier qu on a tjrs rien reçu
    if(temperatureC==0.00)   // remarque : il faut mettre 0.00 et pas 0,00
    {     
      blink=true;
      mappedGreen=0;
      mappedBlue=126;
      mappedRed=126;
      
    }

    else
    {
    //on check si c est dans l intervalle de température idéal, ici [17-19] bornes incluses
    if (temperatureC >= float(borneInfTempIdeal) && temperatureC <= float(borneSupTempIdeal)) 
    {
        blink = true;  // clignotement 
        mappedGreen = 255; // clignotement vert
        mappedBlue = 0;
        mappedRed = 0;
        
        
    }
    
    else 
    {
        blink = false;
        mappedGreen = 0;

        // donc si on est à 100 dans [0-4000] , on aura un mappedBlue très proche de 255 et un mappedRed très proche de 0
        //      si on est à 3800 dans [0-4000], on aura un mappedBlue très proche de 0 et un mappedRed très proche de 255
        // => c'est simultané, quand une valeur mappedBlue ou mappedRed descend, l'autre augmente
        
        int temperatureCInt = temperatureC;
        
        if( temperatureC < float(tempMaxBlue))
        {
            mappedRed=0; 
            if(temperatureC> float(tempMinBlue))
            {
            mappedBlue = map(temperatureCInt,tempMinBlue,tempMaxBlue,255,30); 
            // on mappe la value du thermometre TemperatureCInt dans l'intervalle [tempMinBlue,tempMaxBlue] sur un intervalle de [255-30]
            // donc si on est à 15 dans [14-17] , on aura un mappedBlue très proche de 255.
            //si on est à 17 dans [14-17], on aura un mappedBlue à 30
            // ici on aura mappedGreen à 0, mappedRed à 0 et mappedBlue à une valeur entre 255 et 30 (plus c'est froid, plus c'est proche de 255)
            }
            else
            {
              mappedBlue=255;
            }
        }
        
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
        }
    }
  
    rainbow(mappedRed, mappedBlue, mappedGreen, blink); // methode définie en dessous à laquelle on passe 4 arguments ( R,B,G,blink)
    
}
 // ==========================================================  FIN METHODE loop() =============================================================





// ======================================== DEBUT METHODE rainbow() =======================================================================
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
    
    
}    // ======================================== FIN METHODE rainbow() =====================================================================




//============================================== DEBUT METHODE callback() ================================================================================
void callback(char* topic, byte* payload, unsigned int length)
{

  Serial.println("Message arrived in topic: ");
  Serial.println("Le bateau a reçu : ");
  Serial.println(topic);
  Serial.print("Message:");
  for(int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
} //=============================================== FIN METHODE callback () ===================================================================================
