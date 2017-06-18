#ifndef PORTS_H_
#define PORTS_H_


/* DEFINICJE */
//macro for checking individual bits
//Sprawdzenie czy w liczbie k jest zapalony bit n
#define IS_BIT_SET(k, n)     ((k) & (1 << (n)))

/*\brief Pin kierunku transmisji RS485 aktywny stanem niskim (odbiór danych).*/
/*! Ustawia port RS485 nr 2 jako wyjscie*/
#define RS485_2_PORT_OUT		PORTD_DIRSET = PIN5_bm;
/*! Wlacza pin wlaczajacy transmisje (RE/DE)*/
#define RS485_2_RX_ON			PORTD.OUTCLR = PIN5_bm;					
/*! Wylacza pin wlaczajacy transmisje (RE/DE)*/
#define RS485_2_RX_OFF			PORTD.OUTSET = PIN5_bm;

/*! Ustawia port RS485 nr. 1 jako wyjscie*/
#define RS485_1_PORT_OUT		PORTE_DIRSET = PIN4_bm;
/*! Wlacza pin wlaczajacy transmisje (RE/DE)*/
#define RS485_1_RX_ON			PORTE.OUTCLR = PIN4_bm;
/*! Wylacza pin wlaczajacy transmisje (RE/DE)*/
#define RS485_1_RX_OFF			PORTE.OUTSET = PIN4_bm;


/*\brief piny portów przekaŸników */
/*! Ustawia port */
//ustawiamy wszystkie piny portów przekaŸnikowych jako wyjœcia
#define RLY_4_OUT				PORTD_DIRSET = PIN0_bm		
#define RLY_3_OUT				PORTD_DIRSET = PIN1_bm
#define RLY_2_OUT				PORTD_DIRSET = PIN2_bm	
#define RLY_1_OUT				PORTD_DIRSET = PIN3_bm


#define RLY_4_ON				PORTD.OUTSET = PIN0_bm
#define RLY_3_ON				PORTD.OUTSET = PIN1_bm
#define RLY_2_ON				PORTD.OUTSET = PIN2_bm
#define RLY_1_ON				PORTD.OUTSET = PIN3_bm

#define RLY_4_OFF				PORTD.OUTCLR = PIN0_bm
#define RLY_3_OFF				PORTD.OUTCLR = PIN1_bm
#define RLY_2_OFF				PORTD.OUTCLR = PIN2_bm
#define RLY_1_OFF				PORTD.OUTCLR = PIN3_bm

// definicja portów dla HC595
#define SCLK_OUT				HC595_PORT.DIRSET = SCLK
#define SER_OUT					HC595_PORT.DIRSET = SER
#define RCLK_OUT				HC595_PORT.DIRSET = RCLK

//pins assignment
#define SCLK_PIN				PIN6_bp
#define SER_PIN					PIN7_bp
#define RCLK_PIN				PIN5_bp
//pins assignments bitmask
#define SCLK					PIN6_bm		//serial clock
#define SER						PIN7_bm		//serial data input
#define RCLK					PIN5_bm		//refresh output
//port definitions
#define HC595_PORT				PORTC
#define HC595_SCLKSET			HC595_PORT.OUTSET = SCLK;
#define HC595_SCLKCLR			HC595_PORT.OUTCLR = SCLK;
#define HC595_SERSET			HC595_PORT.OUTSET = SER;
#define HC595_SERCLR			HC595_PORT.OUTCLR = SER;
#define HC595_RCLKSET			HC595_PORT.OUTSET = RCLK;
#define HC595_RCLKCLR			HC595_PORT.OUTCLR = RCLK;

/* STRUKTURY DANYCH, ZMIENNE GLOBALNE */


/* PROTOTYPY FUNKCJI */


#endif /* PORTS_H_ */