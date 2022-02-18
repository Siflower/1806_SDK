#include<stdio.h>
#include<stdlib.h>

int main(int arg ,char *arv[])
{
	int j;
	FILE *in ,*out0;
	unsigned char buf[1024*128+16]={0};
	unsigned int header[4]={0};
	unsigned int size;
	unsigned char checksum;

	if(!arv[1])
	{
		printf("!!Please input the correct file name to generate header file!!\n");
		return 0;
	}
	in=fopen(arv[1],"rb");
	if(in == NULL)
	{
		printf("Error:can't open file %s!\n",arv[1]);
		return 0;
	}
	out0=fopen("spl_uboot_header.bin","wb+");

	size = fread(buf,sizeof(unsigned char),1024*128+16,in);
	printf("**** %s size = %d! ****\n",arv[1],size);
	for(j=0;j<size;j++)
	{
		checksum+=buf[j];
	}
	header[0] = size;
	header[1] = 0x38316173;
	header[2] = (unsigned int)checksum;
	printf("**** %s checksum = 0x%x!****\n",arv[1],checksum);
	fwrite((char *)header,sizeof(unsigned char),16,out0);
	fwrite(buf,sizeof(unsigned char),size,out0);
	fclose(out0);
	fclose(in);
	return 0;
}
