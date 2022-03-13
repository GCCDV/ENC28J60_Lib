//Biblioteca controle do módulo ENC28J60 utilizando atmega328p
//Versão: 0.1
//Autores:Breno Borrasqui;Gabriel Candido;Larisse Souza;Rafaela Borges;Vittorio Maretto
//Data:01/03/22

//=====================================================Bibliotecas=================================================================================================
#include "SPI_UFTM.h"
//=====================================================Definições==================================================================================================

#define OP_WCR    0x40 //Write Control Register
#define OP_RCR    0x00 //Read Control Register

#define ECON1     0x1F //used to control the main functions of the ENC28J60.
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

void ENC28J60_Init(){
  //BANK0
  ENC28J60_SetBank(OP_WCR,BANK0);
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
}

//=====================================================Funções=====================================================================================================
