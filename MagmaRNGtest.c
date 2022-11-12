#include<stdio.h>
#include<stdlib.h>
#include<locale.h>
#include<limits.h>
int Pods(int result, long long int table, short StrNum, short Pos)
{
	for (short i = 0; i < 4; i++)
		if (table & ((long long int)1<<(i+StrNum*4)))
			result |= (1<<(i+Pos*4));
		else
			result &= ~(1 << (i+Pos*4));
	return result;
}
short FourBitInNum(unsigned int vector, short Pos)
{
	short Num = 0;
	for (short i = 0; i < 4; i++)
		if (vector & (1<<(i+Pos*4)))
			Num |= (1<<i);
		else
			Num &= ~(1 << i);
	return Num;
}
int BlockPods(unsigned int vector)
{
	const long long int K[8] = {0xC462A5B9E8D703F1, 0x68239A5C1E47BD0F, 0xB3582FADE174C960, 0xC821D4F670A53E9B, 0x7F5A816D093EB42C, 0x5DF692CAB78143E0, 0x8E25691CF4B0DA37, 0x17ED05834FA69CB2};
	int result = 0;
	for (short i = 0; i<8; i++)
	{
		short StrNum = FourBitInNum(vector, i);
		result = Pods(result, K[i], StrNum, i);
	}
	return result;
}
unsigned int BitShift(unsigned int num, int shift)
{
	return (num<<shift) | (num>>32-shift);
}
unsigned int sumator2v32(unsigned int num1, unsigned int num2)
{
	const long long unsigned int modul = 4294967296;
	if ((long long unsigned int)num1+(long long unsigned int)num2>=modul)
        return (unsigned int)((long long unsigned int)num1+(long long unsigned int)num2-modul);
    else
        return (num1+num2);
}
unsigned int sumator2v32m1(unsigned int num1, unsigned int num2)
{
	const long long unsigned int modul = 4294967295;
	if ((long long unsigned int)num1+(long long unsigned int)num2>=modul)
        return (unsigned int)((long long unsigned int)num1+(long long unsigned int)num2-modul);
    else
        return (num1+num2);
}
unsigned int* TrueRand(int BitNum)
{
	FILE* random = fopen("/dev/random","r");
	if (random == NULL)
	{
		printf("Не удалось открыть файл random");
		return NULL;
	}
	unsigned int* RandPos = (unsigned int*)malloc(BitNum/CHAR_BIT);
	for (int i = 0; i<BitNum/(CHAR_BIT*sizeof(unsigned char)); i++)
		*(((unsigned char*)RandPos)+i) = fgetc(random);
	fclose(random);
	return RandPos;
}
void SimpleZamena(unsigned int* nakop1, unsigned int* nakop2, unsigned int* key)
{
	for (int i = 0; i<24; i++)
	{
		unsigned int temp = BitShift(BlockPods(sumator2v32(*nakop1, key[i%8])), 11)^(*nakop2);
		*nakop2 = *nakop1;
		*nakop1 = temp;
	}
	for (int i = 24; i<31; i++)
	{
		unsigned int temp = BitShift(BlockPods(sumator2v32(*nakop1, key[31-i])), 11)^(*nakop2);
		*nakop2 = *nakop1;
		*nakop1 = temp;
	}
	*nakop2 = BitShift(BlockPods(sumator2v32(*nakop1, key[0])), 11)^(*nakop2);
}
long long unsigned int* MakeGamma (int BitNum)
{
	unsigned int* key = TrueRand(256);
	const unsigned int const1 = 16843012;
	const unsigned int const2 = 16843009;
	long long unsigned int* gamma = (long long unsigned int*)malloc((BitNum/(sizeof(long long unsigned int)*CHAR_BIT)+1)*sizeof(long long unsigned int));
	unsigned int* sinhropos = TrueRand(64);
	unsigned int nakop1 = sinhropos[0];
	unsigned int nakop2 = sinhropos[1];
	free(sinhropos);
	SimpleZamena(&nakop1, &nakop2, key);
	unsigned int nakop3 = nakop1;
	unsigned int nakop4 = nakop2;
	for (int i = 0; i < BitNum/(sizeof(long long unsigned int)*CHAR_BIT)+1; i++)
	{
		nakop3 = sumator2v32(nakop3, const2);
		nakop4 = sumator2v32m1(nakop4, const1);
		nakop1 = nakop3;
		nakop2 = nakop4;
		SimpleZamena(&nakop1, &nakop2, key);
		gamma[i] = ((unsigned long long int)nakop1)<<32 | ((unsigned long long int)nakop2);
	}
	free(key);
	return gamma;
}
int main ()
{
	setlocale(LC_ALL, "Rus");
	int BitNum;
	scanf("%d", &BitNum);
	long long unsigned int* gamma = MakeGamma(BitNum);
	FILE* File = fopen("RandomPos.txt","w");
	if (File == NULL)
	{
		printf("Не удалось создать файл RandomPos.txt");
		return -1;
	}
	for (int i = 0; i<BitNum; i++)
	{
		if (gamma[i/(sizeof(long long unsigned int)*CHAR_BIT)] & (((long long unsigned)1) << i%(sizeof(long long unsigned int)*CHAR_BIT)))
			fputc('1',File);
		else
			fputc('0',File);
	}
	fclose(File);
	free(gamma);
	return 0;
}
