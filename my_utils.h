//
// Created by Rácz Attila on 2019-03-12.
//
#ifndef MY_AVR_PROJECT_MY_UTILS_H
#define MY_AVR_PROJECT_MY_UTILS_H

#define bit_get(p,m) ((p) & (m))
#define bit_set(p,m) ((p) |= (m))
#define bit_clear(p,m) ((p) &= ~(m))
#define bit_flip(p,m) ((p) ^= (m))
//#define bit_write(c,p,m) (c ? bit_set(p,m) : bit_clear(p,m))
#define BIT(x) (0x01 << (x))



#endif //MY_AVR_PROJECT_MY_UTILS_H
