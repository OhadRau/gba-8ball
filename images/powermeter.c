/*
 * Exported with nin10kit v1.7
 * Invocation command was nin10kit --mode=3 powermeter power-meter.png 
 * Time-stamp: Friday 04/05/2019, 03:46:11
 * 
 * Image Information
 * -----------------
 * power-meter.png 15@80
 * 
 * All bug reports / feature requests are to be filed here https://github.com/TricksterGuy/nin10kit/issues
 */

#include "powermeter.h"

const unsigned short powermeter[1200] =
{
	0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,
	0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,
	0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,
	0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,
	0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,
	0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,
	0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,
	0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,
	0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,
	0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,
	0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,
	0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,
	0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,
	0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,
	0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,
	0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,
	0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,
	0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,
	0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,
	0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,
	0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,
	0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0e5f,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,
	0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0e5f,0x0066,0x0066,0x0066,0x0e5f,0x0066,0x0066,0x0066,
	0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0e5f,0x0066,0x0066,0x0e5f,0x0066,0x0066,0x0066,0x0066,
	0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0e5f,0x0e5f,0x0e5f,0x0e5f,0x0e5f,0x0066,0x0066,0x0066,0x0066,0x2108,
	0x2108,0x2108,0x2108,0x0066,0x0066,0x0e5f,0x0066,0x0066,0x0066,0x0e5f,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,
	0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,
	0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,
	0x0066,0x0066,0x0066,0x0e5f,0x0e5f,0x0066,0x0e5f,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,
	0x0066,0x0e5f,0x0066,0x0e5f,0x0066,0x0e5f,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,
	0x0e5f,0x0066,0x0e5f,0x0066,0x0e5f,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0e5f,
	0x0066,0x0e5f,0x0066,0x0e5f,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0e5f,
	0x0e5f,0x0e5f,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,
	0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0e5f,0x0066,0x0066,0x0066,
	0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0e5f,0x0e5f,0x0e5f,0x0e5f,0x0066,
	0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0e5f,0x0066,0x0066,0x0066,0x0e5f,0x0066,
	0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,0x0e5f,0x0e5f,0x0066,0x0066,0x0066,
	0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0e5f,0x0066,0x0066,0x0066,0x0e5f,0x0066,0x0066,0x0066,
	0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0e5f,0x0e5f,0x0e5f,0x0e5f,0x0066,0x0066,0x0066,0x0066,0x0066,
	0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0e5f,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,
	0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,
	0x2108,0x2108,0x0066,0x0066,0x0066,0x0e5f,0x0e5f,0x0e5f,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,
	0x2108,0x0066,0x0066,0x0e5f,0x0066,0x0066,0x0066,0x0e5f,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,
	0x0066,0x0066,0x0e5f,0x0066,0x0066,0x0066,0x0e5f,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,
	0x0066,0x0e5f,0x0066,0x0066,0x0066,0x0e5f,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,
	0x0066,0x0e5f,0x0e5f,0x0e5f,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,
	0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,
	0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0e5f,0x0e5f,
	0x0e5f,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0e5f,0x0066,0x0066,0x0066,
	0x0e5f,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0e5f,0x0066,0x0066,0x0066,0x0e5f,
	0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0e5f,0x0066,0x0066,0x0e5f,0x0066,
	0x0e5f,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0e5f,0x0e5f,0x0e5f,0x0e5f,0x0e5f,0x0e5f,0x0e5f,
	0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0e5f,0x0066,0x0066,0x0066,0x0066,0x0066,0x0e5f,0x0066,
	0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,
	0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,
	0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,
	0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,
	0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,
	0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,
	0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,
	0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,
	0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,
	0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,
	0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,
	0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,
	0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,
	0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,
	0x0066,0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,
	0x0066,0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,
	0x2108,0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,
	0x2108,0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,
	0x2108,0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x2108,0x2108,0x2108,
	0x2108,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,
	0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,
	0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066,0x0066
};

