#include <avr/io.h>
#include <util/crc16.h>


uint16_t gm2_crc(unsigned char* bufor, int p)
{
	#define mask 0x1021//zmienna opisuj¹ca wielomian CCITT
	uint16_t i;
	uint16_t c=0, crc=0, counter=0;
	while(counter<p)
	{
		c=bufor[counter++];
		c<<=8;
		for(i=0;i<8;i++)
		{
			if((crc ^ c) & 0x8000)
				crc=(crc<<1)^mask;
			else
				crc<<=1;
			c<<=1;
		}
	}
	crc=((crc&0xFF) << 8 ) | ((crc&0xFF00) >> 8 );//odwrócenie bajtów
	return crc;
}


 int crc_16bit(uint8_t *dane, uint16_t offset, uint16_t dlugosc)
 {
    uint16_t crc = 0, i;

    for (i = offset; i < dlugosc; i++)
		crc = _crc16_update(crc, dane[i]);

    return crc; 
 }