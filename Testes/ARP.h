
/*Biblioteca Protocolo ARP
  Versão: 0.1
  Autores:Breno Borrasqui;Gabriel Candido;Larisse Souza;Rafaela Borges;Vittorio Maretto
  Data:25/03/22
*/

#include "ENC28J60_UFTM.h"

typedef struct Protocol {
  unsigned char senderMacEther[6];
  unsigned char targetMacEther[6];
  unsigned char senderIpEther[4];
  unsigned char hardType[2];
  unsigned char protType[2];
  unsigned char hardSize[1];
  unsigned char protSize[1];
  unsigned char op[2];
  unsigned char senderMac[6];
  unsigned char senderIp[4];
  unsigned char targetMac[6];
  unsigned char targetIp[4];
} ARP;

ARP teste = {0x7c, 0x8a, 0xe1, 0xdf, 0xfe, 0x9f, 0x45, 0x4c, 0x44, 0x45, 0x52, 0x21, 0x08, 0x06, 0x00, 0x01, 0x08, 0x00, 0x06, 0x04, 0x00, 0x02, 0x45, 0x4c, 0x44, 0x45, 0x52, 0x21, 0xc0, 0xa8, 0x64, 0xbc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

int len = 42;
void ENC28J60_WriteBuffer(int len, ARP data)
{
  Serial.println("envio1");
  unsigned char *kk = &data.senderMacEther[0];
  COM_start;
  SPI_Write(OP_WBM | ENC28J60_WRITE_BUF_MEM);
  Serial.println("envio2");
  while (len)
  {
    len--;
    Serial.println(*kk,HEX);
    SPI_Write(*kk);
    kk++;
  }
  COM_end;
  Serial.println("termino");
}

int lenght(ARP *data)
{
  int len = 42;
  return len;
}

void Send_Packet(ARP *mensagem) {
  Serial.println("Entro Send Packet");
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
  if ((ENC28J60_Read(BANK0, EIR) & EIR_TXERIF)) {
    ENC28J60_Write(OP_BFC, ECON1, ECON1_TXRTS);
  }
//    Serial.print("ESTAT: ");
//    Serial.println(ENC28J60_Read(BANK0,ESTAT),HEX);
}
