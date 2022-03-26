
/*Biblioteca controle da interfece SPI do atmega328p
Versão: 0.1
Autores:Breno Borrasqui;Gabriel Candido;Larisse Souza;Rafaela Borges;Vittorio Maretto
Data:01/03/22  
*/

//=====================================================Bibliotecas=================================================================================================

//=====================================================Definições==================================================================================================
#define nop __asm__("nop\n\t")              //Processador não execulta nada durante um ciclo (nop migrado do assembly)
#define debug 0                             //Váriavel para ligar ou deligar o debug via terminal
#define COM_start  PORTB &= ~(1<<PB2)       //Inicia a comunicação SPI escrevendo 0 no pino 10
#define COM_end    PORTB |=  (1<<PB2)       //Finaliza a comunicação SPI escrevendo 1 no pino 10
//==================================================Variáveis globais==============================================================================================

//=================================================Protótipo de funções============================================================================================
void SPI_MasterBegin();
void SPI_Write(unsigned char data);
void SPI_Message(unsigned char data);
unsigned char SPI_Read(unsigned char andress);


//=====================================================Funções=====================================================================================================
void SPI_MasterBegin(){ //Configura os registradores do SPI
  //Configurando pinos 
  DDRB = (1<<PB2)|(1<<PB3)|(0<<PB4)|(1<<PB5);//0b00101100 : PB5(MOSI)output, PB4(MISO)input, PB3(SCK)output, PB2(/SS)output
  PORTB |=(1<<PB2);//Pino 10 em HIGH
  //Configurando SPI
  SPCR = (1<<SPE)|(0<<DORD)|(1<<MSTR)|(0<<CPOL)|(0<<CPHA)|(0<<SPR1)|(1<<SPR0);
  //(Ligando o SPI),(MSB primeiro),(Borda de subida do clock),(Fase do clock),(Velocidade do clock)
  /*SPR1  SPR0  CLOCK
    0     0     fosk/4
    0     1     fosk/16
    1     0     fosk/64
    1     1     fosk/128
  */
  nop;//Clock sem instrução para sincronização
  if(debug)Serial.begin(9600);
}

void SPI_Write(unsigned char data){
  if(debug)Serial.println(data,HEX);

  SPDR = data; //Grava o dado no registrador de comunicação
  while(!(SPSR&(1<<SPIF)));// Testa a flag de termino de trasmissão e aguarda se setada
}

void SPI_Message(unsigned char *data){//Envia um vetor de bytes via SPI em pares
  int aux=1;
  while(*data != NULL){
    if(aux%2 == 1){
      COM_start;
      if(debug)Serial.println("Comunicação iniciada");
    }
    SPI_Write(*data);
    data++;
    if(aux%2 == 0){
      COM_end;
      if(debug)Serial.println("Comunicação finalizada");
    }
    aux++;
  }
}

unsigned char SPI_read(unsigned char andress){
  COM_start;

  SPDR = andress; //Grava o dado no registrador de comunicação
  
  while(!(SPSR & (1<<SPIF)));// Testa a flag de termino de trasmissão e aguarda se setada

  SPDR = 0b00000000; //Grava o dado no registrador de comunicação
  
  while(!(SPSR & (1<<SPIF)));// Testa a flag de termino de trasmissão e aguarda se setada

  unsigned char message = SPDR;
 
  COM_end;

  return message;
}
