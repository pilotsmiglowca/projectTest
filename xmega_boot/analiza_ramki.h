#ifndef ANALIZA_RAMKI_H_
#define ANALIZA_RAMKI_H_

#define POCZATEK_RAMKI 0x53
#define KONIEC_RAMKI 0x45

#define ADRES_DOCELOWY 0x00
#define ADDRES_SKAD_PRZYSZLO 0x00
#define POLECENIE 0x00
#define BUFOR_NA_DANE_Z_RAMKI 30
#define BUFOR_NA_PROGRAM 20
#define BUFOR_RAMKI 200

#include "Procesor.h"

//#include "main.h"



struct ramka_t
{
	uint16_t dlugosc;
	uint8_t skadPrzyszla;
	uint8_t gdzieSkierowana;
	uint8_t polecenie;
	uint16_t liczba_bajtow_w_ramce;
	uint8_t  dane_w_ramce[BUFOR_NA_DANE_Z_RAMKI];
	uint16_t crc;
};


struct dane_z_ramki_t
{
	uint32_t numerRamki;
	uint16_t ilosc_bajtow_programu;
	uint32_t offset;
	
	uint16_t dlugosc_bufora_z_programem;
	uint8_t  program[BUFOR_NA_PROGRAM];
};


#ifdef TESTOWANIE
	int TEST_algorytmu_analizy_ramek(void);
#endif

void generujRamke(struct ramka_t *nadawczaRamka,uint8_t *bufor_do_wyslania);
void analizaDanych(struct ramka_t* ramka,struct dane_z_ramki_t *dane_z_ramki);
uint8_t analizaRamki(uint8_t *bufor, struct ramka_t *objRamka);




#endif /* ANALIZA_RAMKI_H_ */