#include <avr/io.h>
#include "usart_driver.h"
#include "Procesor.h"
#include "protocol.h"
#include <string.h>
#include "analiza_ramki.h"
#include <avr/pgmspace.h>
#include "serial.h"


bool byteToBuffer=false;

void USART_init()
{
	PORTC_OUTSET=PIN3_bm;
	PORTC_DIRSET=PIN3_bm;                          //Pin TxD musi byæ wyjœciem
	USARTC0.CTRLB=USART_TXEN_bm | USART_RXEN_bm;   //W³¹cz nadajnik USART
	USARTC0.CTRLC=USART_CHSIZE_8BIT_gc;            //Ramka 8 bitów, bez parzystoœci, 1 bit stopu
	usart_set_baudrate(&USARTC0, 1200, F_CPU);
}

	volatile int xxx;
	#define ROZMIAR_BUFORA 200
	volatile uint8_t bufor[ROZMIAR_BUFORA];
	
	volatile uint8_t bufor_do_wyslania[ROZMIAR_BUFORA];

int main(void)
{
	
	
#ifdef TESTOWANIE	
	if(TEST_algorytmu_analizy_ramek())
	{
		//s¹ b³êdy
	}
	else
	{
		//nie wykryto b³êdu 
	}
#endif
	

	//SEKCJA ŒMIECIOWA*****************************************************************


	uint32_t index;
	struct dane_z_ramki_t dane_z_ramki;
	struct ramka_t odebranaRamka;
	struct ramka_t nadawczaRamka;
	uint8_t tab_pom[]= {0x53,0x07,0x00, 0x01, 0x00, 0x00, 0xE4, 0x00, 0x45};


	USART_init();
	
	while(1)
	{
		if(USARTC0_STATUS & USART_RXCIF_bm)
		{
			uint8_t data=USARTC0_DATA;
			
			bufor[(index++)%ROZMIAR_BUFORA]=data;
			
			if(analizaRamki(&bufor,&odebranaRamka))
			{
				//TEST_budujRamke()
				
				memcpy(nadawczaRamka.dane_w_ramce,odebranaRamka.dane_w_ramce,odebranaRamka.dlugosc);
				nadawczaRamka.gdzieSkierowana=odebranaRamka.skadPrzyszla;
				nadawczaRamka.skadPrzyszla=odebranaRamka.gdzieSkierowana;
				nadawczaRamka.dlugosc=odebranaRamka.dlugosc;
				nadawczaRamka.polecenie=odebranaRamka.polecenie;
				
				generujRamke(&nadawczaRamka,&bufor_do_wyslania);
				
				
				for(int i=0;i<ROZMIAR_BUFORA ;i++)
					bufor[i]=0;
				//_delay_ms(100);
	 USART_send_block(&USARTC0,&bufor_do_wyslania,(nadawczaRamka.dlugosc + 2));

					
				xxx++;
				if(xxx>0)
				xxx++;
					
			}
		}
	}









//***************SEKCJA DRUGA:
/*	unsigned char val;


  initbootuart();

	volatile int xxx;
	#define ROZMIAR_BUFORA 200
	uint8_t bufor[ROZMIAR_BUFORA];
	uint32_t index;
	struct dane_z_ramki_t dane_z_ramki;
	struct ramka_t odebranaRamka;
	uint8_t tab_pom[]= {0x53,0x07,0x00, 0x01, 0x00, 0x00, 0xE4, 0x00, 0x45};
	while(1)
	{	
		val = recchar();
		
		bufor[(index++)%ROZMIAR_BUFORA]=val;
		
		for(int i=0;i<9;i++)
			bufor[i]=tab_pom[i];
				
		if(analizaRamki(&bufor,&odebranaRamka))
		{
			xxx++;
			
			for(int i=0;i<ROZMIAR_BUFORA ;i++)
				bufor[i]=0xFF;
		}
		
	}
	
	//SEKCJA ŒMIECIOWA***KONIEC****************************************
	
	InitProtocol(&USART_data);						//inicjalizacja protokolu usart - tak naprawdê tylko inicjalizacja struktury USART_data
	USART_Init();
	
	sei();
			
	//ten while wysyla literke na uart zeby sprawdziæ, ¿e dzia³a poprawnie.
	while(1)
	{
		RS485_1_RX_OFF;
		byteToBuffer = USART_TXBuffer_PutByte(&USART_data, 'H');
		_delay_ms(1);
		RS485_1_RX_ON;
		_delay_ms(1000);
	}*/
}

void InitProtocol(USART_data_t * usart_data)
{
	//to do: pusta funkcja która tylko inicjuje strukturê usart
}