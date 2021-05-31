/*

Implementação da FATFS escrita por elmchan
Filipe Coelho
TODO
    Organizar biblioteca RTC
    Organizar fontes para sdcard
    testes com git
    Makefile

*/

#include "hal.h"                          //System DEFS and LIBS
#include "ds3231.h"                       //RTC On elmchan lib we have the option to not use a rtc and use a fixed timestamp to save the files
#include "sdcard.h"                       //Header to organize fatfs and make the visualization of the cody easyer

volatile UINT Timer;	                  //Performance timer (100Hz increment)
ISR(TIMER0_COMPA_vect){
	Timer++;			                  //Performance counter for the fatfs module
	disk_timerproc();	                  //Drive timer procedure of low level disk I/O module */
}

DWORD get_fattime (void);                 //Converts the rtc data on a 32bit number http://elm-chan.org/fsw/ff/doc/fattime.html
void printTime(void);                     //Put the RTC data on USART
void ioinit(void);                        //Timers and interrupts init
void sdcardTest(void);                    //Demo function to write a string to a file
void fatfsStatusToSerial(uint8_t r);      //USART output for the return to the fatfs functions


FATFS FatF;		//FatFs work area needed for each volume
FIL Fil;        //Pointer to the file
uint16_t bw;    //Pointer to fatfs return


int main(void){
    ioinit();          //start uC hardware
    i2cConfigStart();  //Start i2c
    ds3231Start();     //Start RTC
    serialStart();     //Start USART

    serialStringLN("");
    serialStringLN_P(PSTR("FatFS ff14b by ELM CHAN"));
    serialStringLN_P(PSTR("Modfied by Filipe Coelho 2021.1"));

    sdcardTest();      //Write the file and out the results

    serialString("HEX da timestamp: ");
    serialSendLongInt(get_fattime(),HEX,1);
    printTime();

    while(1);
    return 0;
}

void printTime(void){
    char str[20];
    ds3231UpdateStruct();
    sprintf(str,"%d:%d:%d %d-%d-%d",RTC.hour,RTC.minute,RTC.second,RTC.monthday,RTC.month,RTC.year);
    serialStringLN_P(PSTR("RTC TIME:"));
    serialStringLN(str);
}


DWORD get_fattime (void){
	ds3231UpdateStruct();
	/* Pack date and time into a DWORD variable */
	return	  ((DWORD)(RTC.year - 1980) << 25)
			| ((DWORD)RTC.month << 21)
			| ((DWORD)RTC.monthday << 16)
			| ((DWORD)RTC.hour << 11)
			| ((DWORD)RTC.minute << 5)
			| ((DWORD)RTC.second >> 1);
}

void ioinit (void){
	/* Start 100Hz system timer with TC0 */
	OCR0A = F_CPU / 1024 / 100 - 1;
	TCCR0A = _BV(WGM01);
	TCCR0B = 0b101;
	TIMSK0 = _BV(OCIE0A);
	sei();
}

void fatfsStatusToSerial(uint8_t r){
    switch(r)
    {
        case FR_OK:
            serialStringLN_P(PSTR("(0)Succeeded"));
            break;
        case FR_DISK_ERR:
            serialStringLN_P(PSTR("(1)A hard error occurred in the low level disk I/O layer"));
            break;
        case FR_INT_ERR:
            serialStringLN_P(PSTR("(2)Assertion failed"));
            break;
        case FR_NOT_READY:
            serialStringLN_P(PSTR("(3)The physical drive cannot work "));
            break;
        case FR_NO_FILE:
            serialStringLN_P(PSTR("(4)Could not find the file"));
            break;
        case FR_NO_PATH:
            serialStringLN_P(PSTR("(5)Could not find the path"));
            break;
        case FR_INVALID_NAME:
            serialStringLN_P(PSTR("(6)The path name format is invalid"));
            break;
        case FR_DENIED:
            serialStringLN_P(PSTR("(7)Access denied due to prohibited access or directory full"));
            break;
        case FR_EXIST:
            serialStringLN_P(PSTR("(8)Access denied due to prohibited access"));
            break;
        case FR_INVALID_OBJECT:
            serialStringLN_P(PSTR("(9)The file/directory object is invalid"));
            break;
        case FR_WRITE_PROTECTED:
            serialStringLN_P(PSTR("(10)The physical drive is write protected"));
            break;
        case FR_INVALID_DRIVE:
            serialStringLN_P(PSTR("(11)The logical drive number is invalid"));
            break;
        case FR_NOT_ENABLED:
            serialStringLN_P(PSTR("(12)The volume has no work area"));
            break;
        case FR_NO_FILESYSTEM:
            serialStringLN_P(PSTR("(13)There is no valid FAT volume"));
            break;
        case FR_MKFS_ABORTED:
            serialStringLN_P(PSTR("(14)The f_mkfs() aborted due to any problem "));
            break;
        case FR_TIMEOUT:
            serialStringLN_P(PSTR("(15)Could not get a grant to access the volume within defined period"));
            break;
        case FR_LOCKED:
            serialStringLN_P(PSTR("(16)The operation is rejected according to the file sharing policy"));
            break;
        case FR_NOT_ENOUGH_CORE:
            serialStringLN_P(PSTR("(17)LFN working buffer could not be allocated"));
            break;
        case FR_TOO_MANY_OPEN_FILES:
            serialStringLN_P(PSTR("(18)Number of open files > FF_FS_LOCK"));
            break;
        case FR_INVALID_PARAMETER:
            serialStringLN_P(PSTR("(19)Given parameter is invalid"));
            break;
        default:
            serialStringLN_P(PSTR("Error not found"));
            break;
    }
}


void sdcardTest(void){
    uint8_t result=0;
    result=f_mount(&FatF,"",DRV_MMC);
    fatfsStatusToSerial(result);
    result=f_open(&Fil, "TESTE.TXT",FA_OPEN_APPEND|FA_WRITE);
    fatfsStatusToSerial(result);
    result=f_write(&Fil, "TESTE DE ESCRITA", 8, &bw);
    fatfsStatusToSerial(result);
    result=f_close(&Fil);// Close the file
    fatfsStatusToSerial(result);
}


