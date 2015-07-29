/* 
 * File:   encoder.h
 * Author: eLimones <limonesu.me@gmail.com>
 *
 * Created on July 29, 2015, 3:26 AM
 */

#ifndef ENCODER_H
#define	ENCODER_H

#ifdef	__cplusplus
extern "C" {
#endif
	
	void encoder_init(void);
	int encoder_hasChanged(void);
	int encoder_getPosition(void);
	
#ifdef	__cplusplus
}
#endif

#endif	/* ENCODER_H */

