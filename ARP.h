/*Biblioteca Protocolo ARP
  Versão: 0.1
  Autores:Breno Borrasqui;Gabriel Candido;Larisse Souza;Rafaela Borges;Vittorio Maretto
  Data:01/04/22
*/

//=====================================================Bibliotecas=================================================================================================
#include "ENC28J60_UFTM.h"

//=====================================================Definições==================================================================================================
#define ARP_packet 42
//==================================================Variáveis globais==============================================================================================

//int len = 42;
unsigned char MAC1[6] = {0x45,0x4C, 0x44, 0x45, 0x52, 0x21};


//ARP teste = {0x00,0x01,0x08,0x00,0x06,0x04,0x00,0x01,0x7c,0x8a,0xe1,0xdf,0xfe,0x9f,0xc0,0xa8,0x64,0xbc,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} ;
//ARP teste = {0x7c, 0x8a, 0xe1, 0xdf, 0xfe, 0x9f, 0x45, 0x4c, 0x44, 0x45, 0x52, 0x21, 0x08, 0x06, 0x00, 0x01, 0x08, 0x00, 0x06, 0x04, 0x00, 0x02, 0x45, 0x4c, 0x44, 0x45, 0x52, 0x21, 0xc0, 0xa8, 0x64, 0xbc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


//=================================================Protótipo de funções============================================================================================
unsigned char ARP_Processing(unsigned char *packet){
    
    packet[0] = packet[6];
    packet[1] = packet[7];
    packet[2] = packet[8];
    packet[3] = packet[9];
    packet[4] = packet[10];
    packet[5] = packet[11];
    
    packet[6]  = MAC1[0];
    packet[7]  = MAC1[1];
    packet[8]  = MAC1[2];
    packet[9]  = MAC1[3];
    packet[10] = MAC1[4];
    packet[11] = MAC1[5];

    packet[21] = 0x02;
    packet[22]  = MAC1[0];
    packet[23]  = MAC1[1];
    packet[24]  = MAC1[2];
    packet[25]  = MAC1[3];
    packet[26] = MAC1[4];
    packet[27] = MAC1[5];

//    packet[28]  = packet[38];
//    packet[29]  = packet[39];
//    packet[30] =  packet[40];
//    packet[31] =  packet[41];
//    
//    packet[38]  = MAC1[2];
//    packet[39]  = MAC1[3];
//    packet[40] = MAC1[4];
//    packet[41] = MAC1[5];
  
  //return packet
}

void Send_Packet(ARP *mensagem, int len) {

  //Serial.println("Entro Send Packet");Debug
  ENC28J60_SetBank(BANK0);
  // Set the write pointer to start of transmit buffer area
  ENC28J60_Write(OP_WCR, EWRPTL, ETXST_INIT & 0xFF);
  ENC28J60_Write(OP_WCR, EWRPTH, ETXST_INIT >> 8);
  // Set the TXND pointer to correspond to the packet size given
  ENC28J60_Write(OP_WCR, ETXNDL, (ETXST_INIT + len) & 0xFF);
  ENC28J60_Write(OP_WCR, ETXNDH, (ETXST_INIT + len) >> 8);
  // write per-packet control byte (0x00 means use macon3 settings)
  ENC28J60_Write(OP_WBM, RBM, 0x00);
  // copy the packet into the transmit buffer
  ENC28J60_WriteBuffer(len, *mensagem);
  // send the contents of the transmit buffer onto the network
  ENC28J60_Write(OP_BFS, ECON1, ECON1_TXRTS);
  if ((ENC28J60_Read_RCR(BANK0, EIR) & EIR_TXERIF)) {
    ENC28J60_Write(OP_BFC, ECON1, ECON1_TXRTS);
  }
}

int Packet_Receive_Reply(int maxlen, unsigned char *packet,unsigned char *MAC) {
  int len, rxstat;
  ENC28J60_SetBank(BANK0);
  ENC28J60_Write(OP_WCR, ERDPTL, (NextPacketPtr));
  ENC28J60_Write(OP_WCR, ERDPTH, (NextPacketPtr >> 8));


  NextPacketPtr  = ENC28J60_Read_RCR(BANK0, OP_RBM | RBM);
  NextPacketPtr |= ENC28J60_Read_RCR(BANK0, OP_RBM | RBM) << 8;
  Serial.print("NewPacketPtr:");
  Serial.println(NextPacketPtr, HEX);

  len  = ENC28J60_Read_RCR(BANK0, OP_RBM | RBM);
  len |= ENC28J60_Read_RCR(BANK0, OP_RBM | RBM) << 8;
  len -= 4; //remove the CRC count

  Serial.print("Tamanho:");
  Serial.println(len);

  // limit retrieve length
  if (len > maxlen - 1) {
    len = maxlen - 1;
  }

  rxstat  = ENC28J60_Read_RCR(BANK0, OP_RBM | RBM);
  rxstat |= ENC28J60_Read_RCR(BANK0, OP_RBM | RBM) << 8;

  if ((rxstat & 0x80) == 0) {
    Serial.println("Pacote com defeito");
    len = 0;
  }
  else {
    Serial.println("Pacote recebido com sucesso");


    //unsigned char *pack;
    free(packet);
    packet = calloc(len + 1, sizeof(unsigned char));

    ENC28J60_Continuos_Read_Buffer(len, packet);
    Serial.print("Print_pacote:");
    ENC28J60_Print_packet(packet, len);

    //Serial.print("antes struct Arp criada");

    if(packet[12] == 0x08 && packet[13] == 0x06){
      Serial.println("Pacote ARP identificado");
      ARP_Processing(packet);

    ARP reply;

    unsigned char *aux = &reply.senderMacEther[0];

    for (int i = 0; i < ARP_packet; i++) {
      *aux = *(packet + i);
       aux++;
    }


    Send_Packet(&reply, ARP_packet);
  
    }
     else{

      Serial.println("Pacote não identificado");      
     }
     
    //Move the RX read pointer to the start of the next received packet
    // This frees the memory we just read out
    ENC28J60_SetBank(BANK0);//TROCA PARA BANCO 0
    ENC28J60_Write(OP_WCR, ERXRDPTL, NextPacketPtr);
    ENC28J60_Write(OP_WCR, ERXRDPTH, (NextPacketPtr >> 8));
    // decrement the packet counter indicate we are done with this packet
    ENC28J60_SetBank(BANK0);//TROCA PARA BANCO 0
    ENC28J60_Write(OP_BFS, ECON2, ECON2_PKTDEC);

    return len;
  }
}


//=====================================================Funções=====================================================================================================
