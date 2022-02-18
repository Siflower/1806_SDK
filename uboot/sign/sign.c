#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pk.h"
#include "rsa.h"
#include "sha1.h"
#define HASH_LEN	20		//SHA1 hash mesage len

typedef struct{
	void *data;
	int len;
}data_info;

void main(int argc, char *argv[])
{
	if(argc != 4){
		printf("Usage error!\n");
		printf("Usage: ./sign private_key_file img_bin_file signed_info_file\n");
		return ;
	}
	int i;
	FILE *fp;
	data_info data[2];	//data[0] is key data info;data[1] is img data info
	mbedtls_pk_context pk;
	mbedtls_rsa_context *rsa;
	unsigned char *buf;
	//get key and img
	for( i = 0; i < 2; i++){
		if((fp = fopen(argv[i + 1], "rb"))!= NULL){
			fseek(fp, 0L, SEEK_END);
			data[i].len = ftell(fp);
		//	printf("%s size is %d\n", argv[i + 1], data[i].len);
			data[i].data = malloc(data[i].len);
			memset(data[i].data, 0, data[i].len);

			if(!data[i].data){
				printf("Malloc mem failed\n");
				fclose(fp);
				return ;
			}
			fseek(fp, 0L, SEEK_SET);
			fread(data[i].data, data[i].len, 1, fp);
		}
	}
	// get rsa key
	mbedtls_pk_init(&pk);
	mbedtls_pk_parse_key(&pk, (const unsigned char *)data[0].data, strlen(data[0].data) + 1, NULL, 0);
	rsa = mbedtls_pk_rsa(pk);

	if(rsa->len){
		printf("rsa len is %d\n", (unsigned int)rsa->len);
		buf = malloc(rsa->len);
		if(!buf){
			printf("Malloc buf failed\n");
			return ;
		}
	}


	//get hash value
	unsigned char sha1sum[HASH_LEN];
    mbedtls_sha1(data[1].data, data[1].len, sha1sum);
	//sign the hash value
	if(mbedtls_rsa_pkcs1_sign(rsa, NULL, NULL, MBEDTLS_RSA_PRIVATE, MBEDTLS_MD_SHA1, 0,
				sha1sum, buf) != 0)
	{
		printf("SIGN:Sign image failed!\n");
		return ;
	}
	fp = fopen(argv[3], "wb+");
	if(!fp){
		printf("Can't open %s\n", argv[3]);
		goto end;
	}

	printf("The signed info:");
	for( i = 0; i < rsa->len; i++)
	{
		if(i % 16 == 0)
			printf("\n");
		if(i % 4 == 0)
			printf("\t");
		printf("%02x", buf[i]);
	}
	printf("\n");
	fwrite(buf, rsa->len, 1, fp);
	fclose(fp);

	if(mbedtls_rsa_pkcs1_verify(rsa, NULL, NULL, MBEDTLS_RSA_PUBLIC, MBEDTLS_MD_SHA1, 0,
					sha1sum, buf) != 0)
	{
		printf("SIGN: Verify img failed\n");
		goto end;
	}

	printf("SIGN: Verify img ok!\n");

end:
	free(buf);
	free(data[0].data);
	free(data[1].data);
	return ;
}

