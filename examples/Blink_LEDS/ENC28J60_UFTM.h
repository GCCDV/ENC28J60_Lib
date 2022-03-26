//Biblioteca controle do módulo ENC28J60 utilizando atmega328p
//Versão: 0.1
//Autores:Breno Borrasqui;Gabriel Candido;Larisse Souza;Rafaela Borges;Vittorio Maretto
//Data:01/03/22

//=====================================================Bibliotecas=================================================================================================
#include "SPI_UFTM.h"
//=====================================================Definições==================================================================================================
#
#define OP_WCR    0x40 //Write Control Register
#define ECON1     0x1F //used to control the main functions of the ENC28J60.
  #define BANK0   0x00
  #define BANK1   0x01
  #define BANK2   0x02
  #define BANK3   0x03
  
#define MIREGADR  0x14 // MII Register Address
  #define  MIWRL  0x16 // MII Write Data Low Byte
  #define  MIWRH  0x17 // MII Write Data High Byte

#define PHLCON    0x14 // PHY MODULE LED CONTROL REGISTER 0011 0100 0010 001x

//==================================================Variáveis globais==============================================================================================

//=================================================Protótipo de funções============================================================================================
void ENC28J60_SetBank(unsigned char op,unsigned char bank){
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

//=====================================================Funções=====================================================================================================
