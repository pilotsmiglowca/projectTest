#include <avr/io.h>
#include <string.h>
//#include "main.h"
#include "crc_check.h"
#include "analiza_ramki.h"
#include "Procesor.h"

volatile uint16_t dlugosc=0;
uint8_t tablica_pomocnicza[BUFOR_RAMKI];
uint16_t crc;
	uint16_t crcMajor;
	uint16_t crcMinor;
uint8_t analizaRamki(uint8_t *bufor, struct ramka_t *objRamka)
{

	

	
	for(int i=0;i<BUFOR_RAMKI;i++)
	{
		tablica_pomocnicza[i]=bufor[i];
	}
	
	
	
	for(int i=0;i<BUFOR_RAMKI;i++)
	{
		if(bufor[i]!=POCZATEK_RAMKI)
			continue;
		
		dlugosc= (bufor[(i+1)%BUFOR_RAMKI] &0xFF);
		dlugosc+= (((uint16_t)bufor[(i+2)%BUFOR_RAMKI])<<8);
		
		
		//updater podaje b³êdn¹ d³ugoœæ dlatego trzeba +2
		//dlugosc;
		
		if(bufor[(i+dlugosc+1)%BUFOR_RAMKI]!=KONIEC_RAMKI)
			continue;
			
		crcMajor = bufor[((i+dlugosc-1))%BUFOR_RAMKI];
		crcMinor = bufor[(i+dlugosc)%BUFOR_RAMKI];
		
		for(int j=0;j<dlugosc+2;j++)
		{
			tablica_pomocnicza[j]=bufor[(i+j)%BUFOR_RAMKI];
		}
		
		//crc=gm2_crc(tablica_pomocnicza,dlugosc-3);
		crc=crc_16bit(tablica_pomocnicza,1,dlugosc-1);
		
		if(crcMajor!=(crc&0xFF)|| crcMinor!=(crc>>8))
			continue;
		
		for(int j=0;j<dlugosc-7;j++)
			objRamka->dane_w_ramce[j]=tablica_pomocnicza[j+6];
		
		objRamka->crc=crc;
		objRamka->dlugosc=dlugosc;
		objRamka->gdzieSkierowana=bufor[(i+3)%BUFOR_RAMKI];
		objRamka->skadPrzyszla=bufor[(i+4)%BUFOR_RAMKI];
		objRamka->polecenie=bufor[(i+5)%BUFOR_RAMKI];
		objRamka->liczba_bajtow_w_ramce=dlugosc-7;

		return 1;
	}
	
	return 0;
}

void deepAnalision(int cm)
{
	if( cm > 20 )
		; //auaa
	else
	if( cm > 8)
		; //normal
	else
		; //imposible
}

void analizaDanych(struct ramka_t* ramka,struct dane_z_ramki_t *dane_z_ramki)
{
	uint16_t index=0;
	uint32_t numerRamki;//= (((uint32_t)(ramka->dane_w_ramce[index++])<<24) + ((uint32_t)(ramka->dane_w_ramce[index++])<<16)+ ((uint32_t)(ramka->dane_w_ramce[index++])<<8) +ramka->dane_w_ramce[index++]);
	
	numerRamki= ((uint32_t)(ramka->dane_w_ramce[index++])<<0);
	numerRamki += ((uint32_t)(ramka->dane_w_ramce[index++])<<8);
	numerRamki += ((uint32_t)(ramka->dane_w_ramce[index++])<<16);
	numerRamki += ((uint32_t)(ramka->dane_w_ramce[index++])<<24);
	
	uint16_t ilosc_bajtow_programu;// = (((uint16_t)(ramka->dane_w_ramce[index++])<<8)+ramka->dane_w_ramce[index++]);
	
	ilosc_bajtow_programu=((uint16_t)(ramka->dane_w_ramce[index++])<<0);
	ilosc_bajtow_programu+=((uint16_t)(ramka->dane_w_ramce[index++])<<8);
	
	uint32_t offset=0;// = (((uint32_t)(ramka->dane_w_ramce[index++])<<24) + ((uint32_t)(ramka->dane_w_ramce[index++])<<16)+ ((uint32_t)(ramka->dane_w_ramce[index++])<<8) +ramka->dane_w_ramce[index++]);
	
	offset=((uint32_t)(ramka->dane_w_ramce[index++])<<0);
	offset+=((uint32_t)(ramka->dane_w_ramce[index++])<<8);
	offset+=((uint32_t)(ramka->dane_w_ramce[index++])<<16);
	offset+=((uint32_t)(ramka->dane_w_ramce[index++])<<24);	
	
	uint16_t dlugosc_bufora_z_programem = ramka->liczba_bajtow_w_ramce - index;
			
	dane_z_ramki->numerRamki = numerRamki;
	dane_z_ramki->ilosc_bajtow_programu = ilosc_bajtow_programu;
	dane_z_ramki->offset = offset;
	dane_z_ramki->dlugosc_bufora_z_programem = dlugosc_bufora_z_programem; 
	
	for(int i=0;i<BUFOR_NA_PROGRAM;i++)
	{
		dane_z_ramki->program[i]=0xff;
	}
		
	for(int i=0;i< dlugosc_bufora_z_programem;i++)
	{
		dane_z_ramki->program[i]= ramka->dane_w_ramce[index+i];
	}
	
}

//volatile uint8_t pomocniczy[200];
volatile uint16_t crcx; 
void generujRamke(struct ramka_t *nadawczaRamka,uint8_t *bufor_do_wyslania)
{
	uint16_t index=0;
	bufor_do_wyslania[index++]=POCZATEK_RAMKI;
	bufor_do_wyslania[index++]=( (nadawczaRamka->dlugosc) % 0xff);
	bufor_do_wyslania[index++]=( (nadawczaRamka->dlugosc) <<8);
	bufor_do_wyslania[index++]= nadawczaRamka->gdzieSkierowana;
	bufor_do_wyslania[index++]= nadawczaRamka->skadPrzyszla;
	bufor_do_wyslania[index++]= nadawczaRamka->polecenie;
	
	if(nadawczaRamka->dlugosc > 7)
	{
		for(int i=0;i<dlugosc-7 ;i++)
		{
			
			bufor_do_wyslania[index++]=nadawczaRamka->dane_w_ramce[i];
		}
	}
	
// 	for(int i=0; i<200;i++)
// 		pomocniczy[i]=bufor_do_wyslania[i];
	
	crcx= crc_16bit(bufor_do_wyslania,1,dlugosc-1);
	
	bufor_do_wyslania[index++]=crcx&0xFF;
	bufor_do_wyslania[index++]=crcx>>8;
	bufor_do_wyslania[index++]=KONIEC_RAMKI;
	
	//bufor_do_wyslania[index++]= nadawczaRamka
	
}

// static int budujRamke(unsigned char *data,int adresDocelowy, int adresNadawcy,unsigned char *builded_frame)
// {
// 	unsigned int frameLenght = lenght + 9 ;
// 	
// 	unsigned char buffer[frameLenght];
// 	int index=0;
// 	
// 	buffer[index++]= POCZATEK_RAMKI;
// 	buffer[index++]=((frameLenght-2)&0xFF);
// 	buffer[index++]= ((frameLenght-2)>>8);
// 	buffer[index++]= ADRES_DOCELOWY;
// 	buffer[index++]= ADDRES_SKAD_PRZYSZLO;
// 	buffer[index++]= POLECENIE;
// 	
// 	for(int i=0;i<lenght;i++)
// 	{
// 		buffer[index++]= data[i];
// 	}
// 	
// 	int crc=crc_16bit(buffer,1,index);
// 	
// 	buffer[index++]= (crc&0xFF);
// 	buffer[index++]= (crc>>8);
// 	buffer[index++]= KONIEC_RAMKI;
// 	
// 	memcpy(builded_frame,buffer,index);
// 	
// 	return index;
// }



#ifdef TESTOWANIE

static void TEST_wstaw_dane_do_flesza(uint8_t *flesz,struct dane_z_ramki_t *dane_z_ramki);
static int TEST_budujRamke(unsigned char *data,int lenght,unsigned char *builded_frame);
volatile uint8_t TEST_pamiec_flesz[1000];

static void TEST_wstaw_dane_do_flesza(uint8_t *flesz,struct dane_z_ramki_t *dane_z_ramki)
{
	uint32_t offset = dane_z_ramki->offset;
	uint16_t iloscBajtowProgramu = dane_z_ramki->ilosc_bajtow_programu;
	uint16_t licznik;
	
	for(int i=0;i<iloscBajtowProgramu;i++)
	TEST_pamiec_flesz[offset+i]=dane_z_ramki->program[i];
	
}

static int TEST_budujRamke(unsigned char *data,int lenght,unsigned char *builded_frame)
{
	unsigned int frameLenght = lenght + 9 ;
	
	unsigned char buffer[frameLenght];
	int index=0;
	
	buffer[index++]= POCZATEK_RAMKI;
	buffer[index++]=((frameLenght-2)&0xFF);
	buffer[index++]= ((frameLenght-2)>>8);
	buffer[index++]= ADRES_DOCELOWY;
	buffer[index++]= ADDRES_SKAD_PRZYSZLO;
	buffer[index++]= POLECENIE;
	
	for(int i=0;i<lenght;i++)
	{
		buffer[index++]= data[i];
	}
	
	int crc=crc_16bit(buffer,1,index);
	
	buffer[index++]= (crc&0xFF);
	buffer[index++]= (crc>>8);	
	buffer[index++]= KONIEC_RAMKI;
	
	memcpy(builded_frame,buffer,index);
	
	return index;
}
volatile int xxx,xxx2;
volatile uint8_t liczba_bledow;
int TEST_algorytmu_analizy_ramek(void)
{
	struct dane_z_ramki_t dane_z_ramki;
	struct ramka_t odebranaRamka;
	liczba_bledow=0;

	//#define BUFOR_ODBIORCZY 500
	volatile uint8_t bufor[BUFOR_RAMKI];
	volatile uint32_t licznik_bufora;
	
	licznik_bufora=0;
	uint16_t licznik_wstawionych_ramek=0;
	
	//uint8_t test_CRC[]={ 0x53, 7, 0, 1, 0, 0, 0xE4, 0x00 ,0x45 };//
	uint8_t test_CRC[]={ 0x07, 0x00, 0x01, 0x00, 0x00 };//
	uint8_t test_CRC2[]={ 0x07, 0x00, 0x00, 0x01, 0x00 };
		
		uint8_t test_CRC3[]={ 0x53,10, 0x00, 0x01, 0x00, 0x00, 1,2,3,0xc9,0xc1,0x45 };
	xxx= crc_16bit(test_CRC,0,5);
	xxx2= crc_16bit(test_CRC3,0,5);
		
	//	uint8_t tab[]={0x53, 0x00, 0x0F ,0x00, 0x00, 0x00 ,0x66 ,0x66, 0x66 ,0x66, 0x66 ,0x67 ,0xD3, 0x55 ,0x45};
	
	uint8_t dane1[]={0,0,0,0, 6,0,  0,0,0,0,    1 , 2,  3,  4,  5,  6   };
	uint8_t dane2[]={1,0,0,0, 6,0,  6,0,0,0,    7 , 8,  9, 10, 11, 12   };
	uint8_t dane3[]={2,0,0,0, 6,0, 12,0,0,0,   13 ,14, 15, 16, 17, 18   };
	uint8_t dane4[]={3,0,0,0, 2,0, 18,0,0,0,   19 ,20, 0xFF ,0xFF, 0xFF ,0xFF};
	uint8_t dane5[]={4,0,0,0, 5,0, 20,0,0,0,   21 ,22, 23, 24, 25, 0xFF};
	
	//big endian:
	//uint8_t dane1[]={0,0,0,0,0,6,0,0,0,0,1 ,2, 3 ,4, 5 ,6};
	//uint8_t dane2[]={0,0,0,1,0,6,0,0,0,6,7 ,8, 9 ,10, 11 ,12};
	//uint8_t dane3[]={0,0,0,2,0,6,0,0,0,12,13 ,14, 15 ,16, 17 ,18};
	//uint8_t dane4[]={0,0,0,3,0,2,0,0,0,18,19 ,20, 0xFF ,0xFF, 0xFF ,0xFF};
	//uint8_t dane5[]={0,0,0,4,0,5,0,0,1,20,21 ,22, 23 ,24, 25 ,0xFF};
	
	uint8_t ramkaDoWyslania[6+9+10];

	TEST_budujRamke(dane1,6+10,ramkaDoWyslania);
	
	for(int i=0;i<6+9+10;i++)
	{
		bufor[(licznik_bufora)%BUFOR_RAMKI]=ramkaDoWyslania[i];
		licznik_bufora++;
	}
	
	if(analizaRamki(bufor,&odebranaRamka))
	{
		analizaDanych(&odebranaRamka,&dane_z_ramki);
		
		if(dane_z_ramki.numerRamki != licznik_wstawionych_ramek)
		{
			liczba_bledow++;
		}
		else
		{
			licznik_wstawionych_ramek++;
		}
		
		TEST_wstaw_dane_do_flesza(TEST_pamiec_flesz,&dane_z_ramki);
		
		xxx++;
	}
	else
	{
		liczba_bledow++;
		xxx++;
	}
	
	for(int i=0;i<BUFOR_RAMKI;i++)
		bufor[i]=0xFF;
	
	TEST_budujRamke(dane2,6+10,ramkaDoWyslania);
	for(int i=0;i<6+9+10;i++)
	{
		bufor[(licznik_bufora)%BUFOR_RAMKI]=ramkaDoWyslania[i];
		licznik_bufora++;
	}
	

	if(analizaRamki(bufor,&odebranaRamka))
	{
		analizaDanych(&odebranaRamka,&dane_z_ramki);
		if(dane_z_ramki.numerRamki!=licznik_wstawionych_ramek)
		{
			liczba_bledow++;
		}
		else
		{
			licznik_wstawionych_ramek++;
		}
		TEST_wstaw_dane_do_flesza(TEST_pamiec_flesz,&dane_z_ramki);
		xxx++;
	}
	else
	{
		liczba_bledow++;
		xxx++;
	}
	
	for(int i=0;i<BUFOR_RAMKI;i++)
		bufor[i]=0xFF;
	
	
	TEST_budujRamke(dane3,6+10,ramkaDoWyslania);
	for(int i=0;i<6+9+10;i++)
	{
		bufor[(licznik_bufora)%BUFOR_RAMKI]=ramkaDoWyslania[i];
		licznik_bufora++;
	}
	
	if(analizaRamki(bufor,&odebranaRamka))
	{
		analizaDanych(&odebranaRamka,&dane_z_ramki);
		if(dane_z_ramki.numerRamki!=licznik_wstawionych_ramek)
		{
			liczba_bledow++;
		}
		else
		{
			licznik_wstawionych_ramek++;
		}
		TEST_wstaw_dane_do_flesza(TEST_pamiec_flesz,&dane_z_ramki);
		xxx++;
	}
	else
	{
		liczba_bledow++;
		xxx++;
	}
	for(int i=0;i<BUFOR_RAMKI;i++)
		bufor[i]=0xFF;
	
	TEST_budujRamke(dane4,6+10,ramkaDoWyslania);
	for(int i=0;i<6+9+10;i++)
	{
		bufor[(licznik_bufora)%BUFOR_RAMKI]=ramkaDoWyslania[i];
		licznik_bufora++;
	}
	
	if(analizaRamki(bufor,&odebranaRamka))
	{
		analizaDanych(&odebranaRamka,&dane_z_ramki);
		if(dane_z_ramki.numerRamki!=licznik_wstawionych_ramek)
		{
			liczba_bledow++;
		}
		else
		{
			licznik_wstawionych_ramek++;
		}
		TEST_wstaw_dane_do_flesza(TEST_pamiec_flesz,&dane_z_ramki);
		xxx++;
	}
	else
	{
		liczba_bledow++;
		xxx++;
	}
	for(int i=0;i<BUFOR_RAMKI;i++)
		bufor[i]=0xFF;
	
	TEST_budujRamke(dane5,6+10,ramkaDoWyslania);
	for(int i=0;i<6+9+10;i++)
	{
		bufor[(licznik_bufora)%BUFOR_RAMKI]=ramkaDoWyslania[i];
		licznik_bufora++;
	}
	
	if(analizaRamki(bufor,&odebranaRamka))
	{
		analizaDanych(&odebranaRamka,&dane_z_ramki);
		if(dane_z_ramki.numerRamki!=licznik_wstawionych_ramek)
		{
			liczba_bledow++;
		}
		else
		{
			licznik_wstawionych_ramek++;
		}
		
		TEST_wstaw_dane_do_flesza(TEST_pamiec_flesz,&dane_z_ramki);
		
		xxx++;
	}
	else
	{
		liczba_bledow++;
		xxx++;
	}
	for(int i=0;i<BUFOR_RAMKI;i++)
		bufor[i]=0xFF;

	if(liczba_bledow>0)
	{
		xxx++;
		return liczba_bledow;
	}
	else
	{
		xxx++;
		return 0;
	}	
	
}

int odejmowanie(int a, int b)
{ 
	return a-b;
}


int dzielenie()
{
	//TODO trzeba to zrobiæ!!!
	return;
	
}

#endif