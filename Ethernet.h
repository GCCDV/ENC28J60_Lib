/*Biblioteca Ethernet Utilizando Arduino + ENC28J60
Versão: 0.1
Autores:Breno Borrasqui;Gabriel Candido;Larisse Souza;Rafaela Borges;Vittorio Maretto
Data:01/04/22  
*/
//=====================================================Bibliotecas=================================================================================================
#include "ARP.h"


//=====================================================Definições==================================================================================================

//==================================================Variáveis globais==============================================================================================

//=================================================Protótipo de funções============================================================================================

//=====================================================Funções=====================================================================================================
void Print_packet(unsigned char *packet,int len){
  Serial.print("Print_pacote:");
  for(int i =0;i<len;i++){
   if(*(packet+i) == '/0'){
    Serial.print("FIM");
    return 0;
   }
   Serial.print(*(packet+i),HEX);
   Serial.print("|");
   
  }
  Serial.println();
}
void Print_poiter(){
  
  Serial.print("Endereço apontado no Buffer de recebimento: 0x");
  Serial.print(ENC28J60_Read_RCR(BANK0,OP_RCR|ERDPTL),HEX);
  Serial.println(ENC28J60_Read_RCR(BANK0,OP_RCR|ERDPTH),HEX);
}

void First_packet(){
  int NextP=0,len,rxstat;
  
  while(ENC28J60_Read_RCR(BANK1,OP_RCR|EPKTCNT) == 0){
    Serial.println("Aguardando");
  }
  Serial.print("PACOTES:");
  Serial.println(ENC28J60_Read_RCR(BANK1, OP_RCR | EPKTCNT));
  
  ENC28J60_SetBank(BANK0);
  ENC28J60_Write(OP_WCR, ERDPTL, (NextPacketPtr));
  ENC28J60_Write(OP_WCR, ERDPTH, (NextPacketPtr >> 8));

  
  NextP  = ENC28J60_Read_RCR(BANK0,OP_RBM|RBM);
  NextP |= ENC28J60_Read_RCR(BANK0,OP_RBM|RBM) << 8;
  Serial.print("NewPacketPtr:");
  Serial.println(NextP,HEX);
  
  len  = ENC28J60_Read_RCR(BANK0,OP_RBM|RBM);
  len |= ENC28J60_Read_RCR(BANK0,OP_RBM|RBM) << 8;
  len -= 4; //remove the CRC count

  Serial.print("Tamanho:");
  Serial.println(len);

  rxstat  = ENC28J60_Read_RCR(BANK0,OP_RBM|RBM);
  rxstat |= ENC28J60_Read_RCR(BANK0,OP_RBM|RBM) << 8;

  if ((rxstat & 0x80) == 0) {
    Serial.println("Pacote com defeito");
    len =0;
  }
  else{
    Serial.println("Pacote recebido com sucesso");
  }

  unsigned char *pack;
  pack = calloc(len+1,sizeof(unsigned char));
  
  ENC28J60_Continuos_Read_Buffer(len,pack);
  
  Serial.print("Print_pacote:");
  Print_packet(pack,len);
  free(pack);
  
  ENC28J60_SetBank(BANK0);//TROCA PARA BANCO 0
  ENC28J60_Write(OP_BFS, ECON2, ECON2_PKTDEC);
  

}
