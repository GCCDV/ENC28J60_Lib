
/*Exemplo Retorna a versão do chip
Versão: 0.3
Autores:Breno Borrasqui;Gabriel Candido;Larisse Souza;Rafaela Borges;Vittorio Maretto
Data:25/03/22  
*/

//=====================================================Bibliotecas=================================================================================================
//#include "ENC28J60_UFTM.h"
#include "ARP.h"
//=====================================================Definições==================================================================================================

//=====================================================Variáveis Globais==================================================================================================
unsigned char MAC[6] = {0x45, 0x4c, 0x44, 0x45, 0x52, 0x21};
//=====================================================Definições iniciais=========================================================================================
void setup() {
  
  Serial.begin(9600);
  SPI_MasterBegin();
  
  ENC28J60_Reset();
  
  ENC28J60_BlinkLEDs(1000);

  Serial.print("Version: 0x");
  Serial.println(ENC28J60_Revision(),HEX);

  ENC28J60_Init(MAC);
}
//
void loop() {
//  // put your main code here, to run repeatedly:
 Send_Packet(&teste);
 delay(5000);
}
