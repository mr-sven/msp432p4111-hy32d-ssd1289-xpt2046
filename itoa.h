/*
 * itoa.h
 *
 *  Created on: 15.10.2018
 *      Author: Sven
 */

#ifndef ITOA_H_
#define ITOA_H_

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus


extern char* itoa(int value, char *string, int radix);
extern char* ltoaEx(long value, char *string, int radix);
extern char* utoa(unsigned long value, char *string, int radix);
extern char* ultoa(unsigned long value, char *string, int radix);


#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif /* ITOA_H_ */
