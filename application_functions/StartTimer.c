#include "application_functions.h"

void StartTimer( int ticks)
{
	// ks. manuaalia doc2549.pdf sivulta 118 alkaen
	// 0 = stop  1 = clock  2 = clock/8 3 = clock/64  4 = clock/256 5 = clock/1024
	TCCR0B = (1<<FOC0A) | ( 1<<CS02) | (1<<CS00); // prosessorin kellotaajuus/1024   , jos 16MHz => 64 us
	TIMSK0 |= (1 << OCIE0A); // vertailuarvokeskeytys
	OCR0A = ticks; // laitetaan vertailuarvoksi annettu lukema
	TCNT0 = 0;    // laskuri alkuun
}
