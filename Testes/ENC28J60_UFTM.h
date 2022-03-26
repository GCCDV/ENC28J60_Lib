//Biblioteca controle do módulo ENC28J60 utilizando atmega328p
//Versão: 0.3
//Autores:Breno Borrasqui;Gabriel Candido;Larisse Souza;Rafaela Borges;Vittorio Maretto
//Data:25/03/22

//=====================================================Bibliotecas=================================================================================================
#include "SPI_UFTM.h"
#include "ARP.h"
//=====================================================Definições==================================================================================================

#define OP_WCR    0x40 //Write Control Register
#define OP_RCR    0x00 //Read Control Register
#define OP_WBM    0x60 //Write Buffer Memory
#define OP_BFS    0x80 //Bit Fild Set

#define ECON1     0x1F //used to control the main functions of the ENC28J60.
#define ECON1_RXEN       0x04

  #define BANK0   0x00
  #define BANK1   0x01
  #define BANK2   0x02
  #define BANK3   0x03
  
#define MIREGADR  0x14 // MII Register Address
  #define  MIWRL  0x16 // MII Write Data Low Byte
  #define  MIWRH  0x17 // MII Write Data High Byte

#define PHLCON    0x14 // PHY MODULE LED CONTROL REGISTER 0011 0100 0010 001x

#define EREVID    0x12 // Revision Information



#define ERXST_INIT   0x0000  
#define ERXND_INIT   (0x1FFF-0x0600-1)
#define ETXST_INIT   (0x1FFF-0x0600)
#define ETXND_INIT   0x1FFF

#define ETXSTL    0x04
#define ETXSTH    0x05
#define ETXNDL    0x06
#define ETXNDH    0x07
#define ERXSTL    0x08
#define ERXSTH    0x09
#define ERXNDL    0x0A
#define ERXNDH    0x0B
#define ERXRDPTL  0x0C
#define ERXRDPTH  0x0D
#define ERXWRPTL  0x0E
#define ERXWRPTH  0x0F
#define EDMASTL   0x10
#define EDMASTH   0x11
#define EDMANDL   0x12
#define EDMANDH   0x13
#define EDMADSTL  0x14
#define EDMADSTH  0x15
#define EDMACSL   0x16
#define EDMACSH   0x17

#define ERXFCON          0x18
// ENC28J60 ERXFCON Register Bit Definitions(Receive Filters)
#define ERXFCON_UCEN     0x80
#define ERXFCON_ANDOR    0x40
#define ERXFCON_CRCEN    0x20
#define ERXFCON_PMEN     0x10
#define ERXFCON_MPEN     0x08
#define ERXFCON_HTEN     0x04
#define ERXFCON_MCEN     0x02
#define ERXFCON_BCEN     0x01

#define EPMM0            0x08
#define EPMM1            0x09

#define EPMCSL           0x10
#define EPMCSH           0x11

#define MACON1           0x00
// ENC28J60 MACON1 Register Bit Definitions
#define MACON1_LOOPBK    0x10
#define MACON1_TXPAUS    0x08
#define MACON1_RXPAUS    0x04
#define MACON1_PASSALL   0x02
#define MACON1_MARXEN    0x01

#define MACON2           0x01

#define MACON3           0x02
// ENC28J60 MACON3 Register Bit Definitions
#define MACON3_PADCFG2   0x80
#define MACON3_PADCFG1   0x40
#define MACON3_PADCFG0   0x20
#define MACON3_TXCRCEN   0x10
#define MACON3_PHDRLEN   0x08
#define MACON3_HFRMLEN   0x04
#define MACON3_FRMLNEN   0x02
#define MACON3_FULDPX    0x01


#define MABBIPG          0x04
#define MAIPGL           0x06
#define MAIPGH           0x07

#define MAMXFLL          0x0A
#define MAMXFLH          0x0B

#define MAADR1           0x00
#define MAADR0           0x01
#define MAADR3           0x02
#define MAADR2           0x03
#define MAADR5           0x04
#define MAADR4           0x05

#define EIE              0x1B
// ENC28J60 EIE Register Bit Definitions
#define EIE_INTIE        0x80
#define EIE_PKTIE        0x40

#define PHCON2           0x10
// ENC28J60 PHY PHCON2 Register Bit Definitions
#define PHCON2_HDLDIS    0x0100






// max frame length which the conroller will accept:
#define        MAX_FRAMELEN        1500        // (note: maximum ethernet frame length would be 1518)
//#define MAX_FRAMELEN     600

//==================================================Variáveis globais==============================================================================================

//=================================================Protótipo de funções============================================================================================
void ENC28J60_SetBank(unsigned char op,unsigned char bank){
  if(debug)Serial.println("SetBank");
  COM_start;
  
  SPI_Write(op|ECON1);

  SPI_Write(bank);

  COM_end;
}
void ENC28J60_Write(unsigned char op,unsigned char address, unsigned char data){
  COM_start;

  SPI_Write(op|address);

  SPI_Write(data);

  COM_end;
  
}
void ENC28J60_BlinkLEDs(int ms){
  ENC28J60_SetBank(OP_WCR,BANK2);

  ENC28J60_Write(OP_WCR,MIREGADR,PHLCON);
  ENC28J60_Write(OP_WCR,MIWRL,0x82);
  ENC28J60_Write(OP_WCR,MIWRH,0x38);

  delay(ms);

  ENC28J60_Write(OP_WCR,MIREGADR,PHLCON);
  ENC28J60_Write(OP_WCR,MIWRL,0x92);
  ENC28J60_Write(OP_WCR,MIWRH,0x39);
  
  delay(ms);
  
  ENC28J60_Write(OP_WCR,MIREGADR,PHLCON);
  ENC28J60_Write(OP_WCR,MIWRL,0x82);
  ENC28J60_Write(OP_WCR,MIWRH,0x38);

  delay(ms);

  ENC28J60_Write(OP_WCR,MIREGADR,PHLCON);
  ENC28J60_Write(OP_WCR,MIWRL,0x92);
  ENC28J60_Write(OP_WCR,MIWRH,0x39);

  
}
void ENC28J60_Reset(void){             // System Reset Command (Soft Reset) 
    COM_start;                 // Enable
    SPI_Write(0xFF);
    COM_end ;                 // Disable
    _delay_ms(52);
}
unsigned char ENC28J60_Read(unsigned char bank,unsigned char address){
 
  ENC28J60_SetBank(OP_RCR,bank);
  return SPI_Read(address);
 
}
unsigned char ENC28J60_Read_Buffer(){
 
}
unsigned char ENC28J60_Revision(){
  return ENC28J60_Read(BANK3,EREVID);
}

void ENC28J60_Init(unsigned char *macaddr){
  ENC28J60_Reset();
  //BANK0
  ENC28J60_SetBank(OP_WCR,BANK0);//TROCA PARA BANCO 0
  // Rx start
  ENC28J60_Write(OP_WCR,ERXSTL, ERXST_INIT&0xFF);
  ENC28J60_Write(OP_WCR,ERXSTH, ERXST_INIT>>8);
  // set receive pointer address
  ENC28J60_Write(OP_WCR,ERXRDPTL, ERXST_INIT&0xFF);
  ENC28J60_Write(OP_WCR,ERXRDPTH, ERXST_INIT>>8);
  // RX end
  ENC28J60_Write(OP_WCR,ERXNDL, ERXND_INIT&0xFF);
  ENC28J60_Write(OP_WCR,ERXNDH, ERXND_INIT>>8);
  // TX start
  ENC28J60_Write(OP_WCR,ETXSTL, ETXST_INIT&0xFF);
  ENC28J60_Write(OP_WCR,ETXSTH, ETXST_INIT>>8);
  // TX end
  ENC28J60_Write(OP_WCR,ETXNDL, ETXND_INIT&0xFF);
  ENC28J60_Write(OP_WCR,ETXNDH, ETXND_INIT>>8);
  //BANK1
  ENC28J60_SetBank(OP_WCR,BANK1);//TROCA PARA BANCO 1
  
    ENC28J60_Write(OP_WCR, ERXFCON, ERXFCON_UCEN|ERXFCON_CRCEN|ERXFCON_PMEN);//BROADCAST ...//conferir opcode

  
    ENC28J60_Write(OP_WCR,EPMM0,  0x3f);//Conferir codigo OP
    ENC28J60_Write(OP_WCR,EPMM1,  0x30);
    ENC28J60_Write(OP_WCR,EPMCSL, 0xf9);
    ENC28J60_Write(OP_WCR,EPMCSH, 0xf7);

  // do bank 2 stuff
  ENC28J60_SetBank(OP_WCR,BANK2);//TROCA PARA BANCO 2
  // enable MAC receive
  
  ENC28J60_Write(OP_WCR,MACON1, MACON1_MARXEN|MACON1_TXPAUS|MACON1_RXPAUS);
  // bring MAC out of reset
  ENC28J60_Write(OP_WCR,MACON2, 0x00);
 // enable automatic padding to 60bytes and CRC operations
  ENC28J60_Write(OP_BFS, MACON3, MACON3_PADCFG0|MACON3_TXCRCEN|MACON3_FRMLNEN);
  // set inter-frame gap (non-back-to-back)
  ENC28J60_Write(OP_WCR, MAIPGL, 0x12);
  ENC28J60_Write(OP_WCR, MAIPGH, 0x0C);
  // set inter-frame gap (back-to-back)
  ENC28J60_Write(OP_WCR, MABBIPG, 0x12);
  // Set the maximum packet size which the controller will accept
  // Do not send packets longer than MAX_FRAMELEN:
  ENC28J60_Write(OP_WCR, MAMXFLL, MAX_FRAMELEN&0xFF);  
  ENC28J60_Write(OP_WCR, MAMXFLH, MAX_FRAMELEN>>8);
   
   // do bank 3 stuff
   ENC28J60_SetBank(OP_WCR,BANK3);//TROCA PARA BANCO 3
   
  // write MAC address
        // NOTE: MAC address in ENC28J60 is byte-backward
        ENC28J60_Write(OP_WCR, MAADR5, macaddr[0]);
        ENC28J60_Write(OP_WCR, MAADR4, macaddr[1]);
        ENC28J60_Write(OP_WCR, MAADR3, macaddr[2]);
        ENC28J60_Write(OP_WCR, MAADR2, macaddr[3]);
        ENC28J60_Write(OP_WCR, MAADR1, macaddr[4]);
        ENC28J60_Write(OP_WCR, MAADR0, macaddr[5]);
 // no loopback of transmitted frames
     //                 enc28j60PhyWrite(PHCON2, PHCON2_HDLDIS);
     ENC28J60_Write(OP_WCR,MIREGADR,PHCON2);
     ENC28J60_Write(OP_WCR,MIWRL,PHCON2_HDLDIS);// 01010101 11111111
     ENC28J60_Write(OP_WCR,MIWRH,PHCON2_HDLDIS>>8);
  
  // switch to bank 0
  
  ENC28J60_SetBank(OP_WCR,BANK0);//TROCA PARA BANCO 0
  // enable interrutps
  ENC28J60_Write(OP_BFS, EIE, EIE_INTIE|EIE_PKTIE);
  // enable packet reception
  ENC28J60_Write(OP_BFS, ECON1, ECON1_RXEN);
  
}

//void Send_Packet(ARP mensagem) {
  

  
//}


//

//=====================================================Funções=====================================================================================================
