/* 
 * File:   clock.h
 * Author: eLimones <limonesu.me@gmail.com>
 *
 * Created on July 29, 2015, 1:08 AM
 */

#ifndef CLOCK_H
#define	CLOCK_H

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif
	
	typedef struct{
		uint8_t seconds;
		uint8_t minutes;
		uint8_t hours;
		uint8_t days;
	}clock_time_t;
	
	void clock_init(void);
	uint8_t clock_getSeconds(void);
	uint32_t clock_getTime(void);
	
#ifdef	__cplusplus
}
#endif

#endif	/* CLOCK_H */

