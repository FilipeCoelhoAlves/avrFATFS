/*

Implementação da FATFS escrita por elmchan
Filipe Coelho
TODO
    Organizar biblioteca RTC
    Organizar fontes para sdcard
    Makefile

*/


#define   DRV_MMC    0
#include "hal.h"                 //System DEFS and LIBS
#include "ds3231.h"              //RTC
#include "ff14b/source/ff.h"     //Fat by ELM CHAN
#include "ff14b/source/diskio.h" //Disk Functions

void printTime();          //Put the RTC data on USART
DWORD get_fattime (void);  //Converts the rtc data on a 32bit number http://elm-chan.org/fsw/ff/doc/fattime.html
static void ioinit(void);  //Timers and interrupts init
void dataLog();

volatile UINT Timer;	   // Performance timer (100Hz increment)

ISR(TIMER0_COMPA_vect){
	Timer++;			   // Performance counter for this module
	disk_timerproc();	   // Drive timer procedure of low level disk I/O module */
}

FATFS FatF;		//FatFs work area needed for each volume
FIL Fil;
uint16_t bw;    //Ponteiro nescessario para retorno das funcoes da fatfs


int main(void){
    _delay_ms(20);   //litle delay to let everything settle
    ioinit();        //inicializa o timer
    i2cConfigStart();
    ds3231Start();
    serialStart();
    serialStringLN("");
    serialStringLN_P(PSTR("FatFS ff14b by ELM CHAN"));
    serialStringLN_P(PSTR("Modfied by Filipe Coelho"));
    printTime();
    dataLog();
    serialSendLongInt(get_fattime(),HEX,1);


    while(1)
    ;

    return 0;
}



void printTime(){
    char str[20];
    ds3231UpdateStruct();
    sprintf(str,"%d:%d:%d %d-%d-%d",RTC.hour,RTC.minute,RTC.second,RTC.monthday,RTC.month,RTC.year);
    serialStringLN(str);
}


DWORD get_fattime (void)
{
	ds3231UpdateStruct();
	/* Pack date and time into a DWORD variable */
	return	  ((DWORD)(RTC.year - 1980) << 25)
			| ((DWORD)RTC.month << 21)
			| ((DWORD)RTC.monthday << 16)
			| ((DWORD)RTC.hour << 11)
			| ((DWORD)RTC.minute << 5)
			| ((DWORD)RTC.second >> 1);
}

static
void ioinit (void)
{	/* Start 100Hz system timer with TC0 */
	OCR0A = F_CPU / 1024 / 100 - 1;
	TCCR0A = _BV(WGM01);
	TCCR0B = 0b101;
	TIMSK0 = _BV(OCIE0A);
	sei();
}


void dataLog(){
    serialSendInt(f_mount(&FatF,"", DRV_MMC),DEC,1);
    serialSendInt(f_open(&Fil, "TESTE.TXT",FA_OPEN_APPEND|FA_WRITE),DEC,1);
    f_write(&Fil, "TESTE DE ESCRITA", 8, &bw);
    f_close(&Fil);// Close the file

}

