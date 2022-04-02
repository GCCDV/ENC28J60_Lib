/*Exemplo aguarda o reccebimento de um papcote ARP e responde
Versão: 0.3
Autores:Breno Borrasqui;Gabriel Candido;Larisse Souza;Rafaela Borges;Vittorio Maretto
Data:31/03/22 
*/

//=====================================================Bibliotecas=================================================================================================
#include "Ethernet.h"
//=====================================================Definições==================================================================================================
#define Print_MAC_op 0
//=====================================================Variáveis Globais==================================================================================================
unsigned char MAC[6] = {0x45,0x4C, 0x44, 0x45, 0x52, 0x21};
//unsigned char IP[4] = {192,168,1,40};

char pacotes,auxi;
//=====================================================Definições iniciais=========================================================================================
void setup() {
  
  Serial.begin(9600);
  SPI_MasterBegin();
  ENC28J60_BlinkLEDs(500);
  ENC28J60_Reset();
  
  Serial.print("Version: 0x");
  Serial.println(ENC28J60_Revision(),HEX);
  
  ENC28J60_Init(MAC);
  Serial.print("Endereço MAC:");
  Print_MAC();

}

void loop() {
 
while(ENC28J60_Read_RCR(BANK1,OP_RCR|EPKTCNT) == 0){
    Serial.print(".");
    delay(500);
  }
  Serial.print("PACOTES:");
  Serial.println(ENC28J60_Read_RCR(BANK1, OP_RCR | EPKTCNT));
  Packet_Receive_Reply(200,packet,MAC);

}

void Print_MAC(){
  if(Print_MAC_op){
    char c;
  for(int i=0;i<6;i++){
    c = MAC[i];
    c = c + '!' - 33;
    Serial.print(c);
  }
  Serial.println();
  }
  else{
    for(int i=0;i<6;i++){
    Serial.print(MAC[i],HEX);
    Serial.print(":");
  }
  Serial.println();
  }
 
}
