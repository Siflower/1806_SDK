
#include <sf_mmap.h>
#include <types.h>

#define MULTIPLIER 0x015a4e35L
#define INCREMENT 1

static long Seed = 1;


void srand(unsigned seed)
{
	Seed = seed;
}

int rand(void)
{
	Seed = MULTIPLIER*Seed + INCREMENT;
	//Seed>>16是将seed向右移16位，seed是long型，只取高16位
	//& 0x7fff，0x7fff转成2进制是0111111111111111，作用是把这个整数变为正整数
	return ((int)(Seed>>16) & 0x7fff);
}



