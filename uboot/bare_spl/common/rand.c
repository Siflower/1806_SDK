
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
	//Seed>>16�ǽ�seed������16λ��seed��long�ͣ�ֻȡ��16λ
	//& 0x7fff��0x7fffת��2������0111111111111111�������ǰ����������Ϊ������
	return ((int)(Seed>>16) & 0x7fff);
}



