#include "usart.h"

volatile char buffer[BUFFER_SIZE];
volatile uint8_t pos=0;
volatile uint8_t endB=0;

ISR(USART_RX_vect)
{
    buffer[pos]=UDR0;
    pos++;
    endB=pos-1;
}


uint8_t serialAvailable()
{
    return pos;
}

uint8_t serialData(){
    return buffer[endB];
}

uint8_t serialRead(){
    uint8_t data=buffer[endB];
    pos--;
    endB--;
    return data;
}

uint8_t serialBufferSize(){
    return endB;
}

void serialPurge()
{
    for(uint8_t x=0;x<BUFFER_SIZE;x++)
        buffer[x]=0;
    endB=0;
    pos=0;
}

void serialStart(void)
{
    //UCSR0A |= (1 << U2X0);
    UBRR0H=((UBRRH_VALUE)>>8);
    UBRR0L=UBRR_VALUE;        //baud rate
    UCSR0B|=0b10000000; //interrupção em RX
    UCSR0B|=0b00011000; //ativa tx e rx
    UCSR0C|=0b00000110; //8bits e 1bits de parada no parity
}

void serialSend(uint8_t data)
{
    while(!(UCSR0A&(1<<UDRE0)));//Verifica se ainda há envio pendente
    UDR0=data;
}

uint8_t serialReceive()
{
    while(!(UCSR0A&(1<<RXC0))); //verifica se ainda há recebimento pendente
    return UDR0;
}

void serialString(const char stg[])
{
    for(uint8_t x=0; stg[x]!=0; x++) //Enquanto não for o null terminator
    {
        serialSend(stg[x]);         //Envia o caractere
    }

}

void serialStringLN(const char stg[])
{
    serialString(stg); //Envia string
    serialSend('\r');  //Return carriage
    serialSend('\n');  //New line
}

void serialSendInt(long int data,uint8_t base,uint8_t linefeed)
{
    char buffer[10];
    switch (base)
    {
    case DEC:
        sprintf(buffer,"%ld",data);
        break;
    case OCT:
        sprintf(buffer,"0x%lo",data);
        break;
    case HEX:
        sprintf(buffer,"%lX",data);
        break;
    default:
        break;
    }

    if(linefeed)
    {
        serialStringLN(buffer);
    }
    else
    {
        serialString(buffer);
    }
}

void serialSendFloat(float data,uint8_t linefeed)
{
    char buffer[10];
    uint16_t temp=data;
    sprintf(buffer,"%d",temp);
    serialString(buffer);
    temp=(data*100);
    temp=temp%100;
    sprintf(buffer,".%d",temp);


    if(linefeed)
    {
        serialStringLN(buffer);
    }
    else
    {
        serialString(buffer);
    }
}


void serialSendLongInt(long int data,uint8_t base,uint8_t linefeed)
{
    char buffer[10];
    switch (base)
    {
    case DEC:
        sprintf(buffer,"%ld",data);
        break;
    case OCT:
        sprintf(buffer,"%lo",data);
        break;
    case HEX:
        sprintf(buffer,"%lX",data);
        break;
    default:
        break;
    }

    if(linefeed)
    {
        serialStringLN(buffer);
    }
    else
    {
        serialString(buffer);
    }
}

void serialSendByte(uint8_t data,uint8_t base,uint8_t linefeed)
{
    char buffer[4];
    switch (base)
    {
    case DEC:
        sprintf(buffer,"%d",data);
        break;
    case OCT:
        sprintf(buffer,"%o",data);
        break;
    case HEX:
        sprintf(buffer,"0x%X",data);
        break;
    default:
        break;
    }

    if(linefeed)
    {
        serialStringLN(buffer);
    }
    else
    {
        serialString(buffer);
    }
}

void serialSendBuffer(uint16_t buffersize)
{
    for(uint16_t x=0; x<buffersize&&buffer[x]!=0; x++)
    {
        serialSend(buffer[x]);
    }
    serialStringLN("");
}

void serialSendIp(uint8_t ip[4])
{
    for(uint8_t x=0; x<3; x++)
    {
        serialSendByte(ip[x],DEC,0);
        serialSend('.');
    }
    serialSendByte(ip[3],DEC,1);
}


//Aqui utilizamos memoria de programa para salvar as STRINGS
// use serialString_P(PSTR("EXEMPLO"));
void serialString_P(const char stg[])
{
    while (pgm_read_byte(stg) != 0x00)
        serialSend(pgm_read_byte(stg++));
}

void serialStringLN_P(const char stg[])
{
    serialString_P(stg);
    serialStringLN("");
}

