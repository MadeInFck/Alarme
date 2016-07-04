#include <VirtualWire.h>

//============================================================================
//  Détecteur de mouvements à infrarouge avec transmission RF
//  Allume une LED si détection
//  03/07/16
//============================================================================
//  Voir aussi  http://www.madeinfck.com
//  Licence Creative Commons https://creativecommons.org/licenses/by-nc-sa/4.0/
//  PIR sensor tester by MadeInFck
// ===========================================================================



int ledPin = 13;       // LED qui s'allume si détection
int inputPin = 2;      // capteur infrarouge PIR 
int pirState = LOW;    // pas encore de mouvement détecté au départ
bool sending = false;
int val = 0;           // Lecture deu statut
long previousMillis = 0;   //instant du déclenchement

void setup() {
  pinMode(ledPin, OUTPUT);      //  LED en sortie
  pinMode(inputPin, INPUT);     // Détecteur PIR en entrée
  Serial.begin(9600);           //Affichage série

  vw_set_ptt_inverted(true);   // Initialisation de l'émetteur RF
  vw_setup(2000);
}

void loop(){
  const char *msg="Intrusion";
  val = digitalRead(inputPin);  // Lire le statut
  unsigned long currentMillis = millis();  //Heure
  
  if (val == HIGH) {            // Si HIGH, détection
    digitalWrite(ledPin, HIGH);  // Allumer la LED
    //delay(150);
    
          if (pirState == LOW) {  //Front montant (début de détection)
              Serial.println("Mouvement detecte ");  //Affiche une seule fois
              pirState = HIGH;  //Mémoriser l'état haut
              previousMillis = currentMillis;    //Mémoriser debut du signal
          }
    
  } else {   //Sinon pas de mouvement détecté
      digitalWrite(ledPin, LOW); // éteindre la ELd
      //delay(300);    
          if (pirState == HIGH){ //Détecter le front descendant
              Serial.print(" Stop : ");
              pirState = LOW;  //Mémoriser l'état bas
              sending = false;
              unsigned long duree = currentMillis - previousMillis;
              Serial.print(" Duree ");
              Serial.println(duree);  //Durée en millisecondes
          }
  }

  if (pirState == HIGH && sending == false) {
      vw_send((uint8_t*)msg, strlen(msg));
      vw_wait_tx();
      Serial.println("Envoi du message d'intrusion");
      sending = true;
  }
}
