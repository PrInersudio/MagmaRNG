#ifndef __MAGMARNG_H__
#define __MAGMARNG_H__

#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

// генерирует динамический массив с криптографически точной случайной последовательностью размера
// (BitNum/(sizeof(long long unsigned int)*CHAR_BIT)+1)*sizeof(long long unsigned int)
long long unsigned int* MagmaRNG (int BitNum);

#endif
