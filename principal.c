/*********************************************************/
// Modèle pour le µ-projet de commande par microcontrôleur
// 			mars 2011
// Programme utile pour l'identification des paramètres
// du moteur. Attention ! faire les bonnes connexions !
/*********************************************************/
//cablage 
//J4 POT1 <--> J13 RA0
//J10 VDD <--> J1 P1 ou P3
//J10 RD2 <--> J1 N2 ou N4

#include <htc.h>
#include "afficheur.h"

void interrupt gestionIT(void){
	static int compteur0;
	volatile char drapeau=0;
	volatile int capteur;
	volatile int commande;
	volatile int erreur;
	
	if(TMR1IF){
	
		//PORTD^=0x40;
		RD6=!RD6; //changement d'état du bit 6		

		compteur0=0;
		TMR1IF=0;
		drapeau=1;
	
	}
	if(T0IF){
		
			
		capteur = TMR1L;
		TMR1L = 0;
		
		//gestion de la conversion
		ADCON0|=0x02;
		while((ADCON0&0x02)==0x02);
		//AfficherOctet(ADRESH);
///////////////////////////////////////////////////////
//marche presque

		erreur=ADRESH-capteur*0x15;
		AfficherOctet(capteur);
		if(erreur>0x00){
			commande = 0xFF;	
		}else{
			commande = 0;
		}
		CCPR2L = commande;
///////////////////////////////////////////////////////
		T0IF=0;
		drapeau=1 ;

	}
	RC7=!drapeau;
/*	if(drapeau){
		PORTC|=0x80;
	}*/
}



void main(void){


//Configuration du Port D
TRISD|=0x10; // RD4 en entrée -> gère le "gate"
TRISD&=0x1F; // RD5,RD6,RD7 en sortie
TRISC&=0x7F; // RC7 en sortie
//TRISC|=0x10; // RC4 en entrée
OPTION_REG=0x05;
RD6=0;
T1CON=0x03; //configuration de Timer1 en source externe; préscaler = 00;

//initialistion timer
TMR0=0x00;
TMR1L=0x00;
TMR1H=0x00;

// INITIALISATION DU LCD
InitialisationAfficheur();

TRISA=0x01;
ANSEL=0x01;
ADCON0=0x01;
ADCON1|=0x10;
ADCON1&=0x9F;

// Initialisation du PWM	
PR2=0xFF;   //période
CCPR2L = 0x40; //duty cycle
TRISD &= 0xFB; //pin de sortie -> RD3
T2CON =0x04;  //configuration de Timer2
PIR2 =0;
PIE2 = 0;
CCP2CON = 0x0C;  //configuration en PWM


PIE1=0x01; //0 correspond à ne pas autoriser les interruptions et 1 autorise pour timer1
PIR1=0x00;
INTCON=0xE0;



while(1){
		ADCON0=0x03;
 		RD5=!RD4;
		AllumerMarqueur1();  // allume un marqueur
		//TMR1ON = RD4;       //gestion de la "gate" 
    }
}
