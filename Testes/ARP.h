
/*Biblioteca Protocolo ARP
  Versão: 0.1
  Autores:Breno Borrasqui;Gabriel Candido;Larisse Souza;Rafaela Borges;Vittorio Maretto
  Data:25/03/22
*/

//=====================================================Bibliotecas=================================================================================================

//=====================================================Definições==================================================================================================

//==================================================Variáveis globais==============================================================================================

typedef struct Protocol {
  unsigned char hardType[2];
  unsigned char protType[2];
  unsigned char hardSize[1];
  unsigned char protSize[1];
  unsigned char op[2];
  unsigned char senderMac[6];
  unsigned char senderIp[4];
  unsigned char targetMac[6];
  unsigned char targetIp[4];
}ARP;

ARP teste = {0x00,0x01,0x08,0x00,0x06,0x04,0x00,0x01,0x7c,0x8a,0xe1,0xdf,0xfe,0x9f,0xc0,0xa8,0x64,0xbc,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} ;





//=================================================Protótipo de funções============================================================================================
void Send_Packet(ARP mensagem) {
  
  //    Serial.println(addr);
  //    addr = &mensagem.b;
  //    Serial.println(addr);
  //    addr = &mensagem.c;
  //    Serial.println(addr);
  //    addr = &mensagem.d;
  //    Serial.println(addr);
  
}
//=====================================================Funções=====================================================================================================
