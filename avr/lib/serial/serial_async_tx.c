/*
 * An asynchronous implementation of the tx portion of the serial library.  This will let you submit a bunch
 * of data to a buffer, and let the AVR send it using interrupts.  This results in shorter
 * send times (as the write methods do not block), but there are downsides.  It a) causes code 
 * to be run in interrupts (which may interere with timing-critical code), b) adds substantially
 * to the total SRAM used by a program (defaults to a 64 byte buffer for transmitting and receiving, 
 * although this buffer size can be configured at compile time), c) introduces the possibility 
 * that data will be partially unsent, if the program tries to write too much at a time and overflows 
 * the buffer, and d) for single byte messages which are not sent continuously, there is actually a 
 * performance penalty as there is an extra layer of abstraction (the buffer) in between the hardware 
 * and the sending program.
 * 
 * With the downsides, though, come a definite advantage -- for longer strings (experiments
 * show this seems to be effective for more than 2 bytes), the speed is much faster.  When
 * sending 3 bytes, it takes 1215µs (1.2ms) to return from the synchronous implementation;
 * while taking only 280µs (0.3ms) to return from the asynchronous one.  For longer 
 * strings the difference is even more pronounced: if you send a 26 character string, it 
 * will take the synchronous implementation about 25131µs (25ms) to return from the 
 * write_s function; the asynchronous implementation takes about 669µs (less than 1ms) 
 * to return.
 */

#include "serial.h"
#include "../Ring/Ring.h"
#include <avr/interrupt.h>

static Ring tx_buffer(SERIAL_BUFFER_SIZE);

void _serial_init_tx(){
	//Enable interrupts if the NO_INTERRUPT_ENABLE define is not set.  If it is, you need to call sei() elsewhere.
#ifndef NO_INTERRUPT_ENABLE
	sei();
#endif
}

void serial_write_c(char data){
	//Disable UCSR interrupts temporarily to avoid clobbering the buffer
	UCSR0B &= ~_BV(UDRIE0);
	tx_buffer.put(data);
	
	//Signal that there is data available; the UDRE interrupt will fire.
	UCSR0B |= _BV(UDRIE0);
}

void serial_write_s(char *data){
	while (*data){
		serial_write_c(*data++);
	}
}

void serial_write_a(uint8_t *data, uint8_t len){
	for (uint8_t i = 0; i < len; i++){
		serial_write_b(data[i]);
	}
}

void serial_write_b(uint8_t data){
	serial_write_c((char) data);
}


#if defined(__AVR_ATtiny2313__)    || \
	defined(__AVR_ATmega48P__)     || \
	defined(__AVR_ATmega88P__)     || \
	defined(__AVR_ATmega168P__)    || \
	defined(__AVR_ATmega328P__)    || \
	defined(__AVR_ATmega48__)      || \
	defined(__AVR_ATmega88__)      || \
    defined(__AVR_ATmega168__)
ISR(USART_UDRE_vect){
#elif defined(__AVR_ATmega324P__)  || \
	defined(__AVR_ATmega644__)     || \
	defined(__AVR_ATmega644P__)    || \
	defined(__AVR_ATmega644PA__)   || \
	defined(__AVR_ATmega1284P__)
ISR(USART0_UDRE_vect){
#else 
#error You must define USART vectors for your chip!  Please verify that MMCU is set correctly, and that there is a matching vector definition in serial_asynch_tx.c
void error2() {
#endif
	if (!tx_buffer.isEmpty()){
		UDR0 = tx_buffer.get();
	}
	else {
		//Once the ring buffer is empty (i.e. head == tail), we disable
		// USART Data Register Empty interrupts until someone writes to
		// the tx buffer again.
		UCSR0B &= ~_BV(UDRIE0);
	}
}
