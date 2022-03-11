
/*Exemplo Retorna a versão do chip
Versão: 0.1
Autores:Breno Borrasqui;Gabriel Candido;Larisse Souza;Rafaela Borges;Vittorio Maretto
Data:11/03/22  
*/

//=====================================================Bibliotecas=================================================================================================

//=====================================================Definições==================================================================================================
#include "ENC28J60_UFTM.h"

//=====================================================Definições iniciais=========================================================================================
void setup() {
  Serial.begin(9600);
  SPI_MasterBegin();
  
  ENC28J60_BlinkLEDs(1000);
  //ENC28J60_Reset();
  
  Serial.print("Version: 0x");
  Serial.println(ENC28J60_Revision(),HEX);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  
}
