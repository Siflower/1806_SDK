#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pk.h"
#include "rsa.h"

int main(int argc, char *argv[])
{
	int ret, len;
	FILE *fp;
	void *pubkey;
	mbedtls_pk_context pk;
	mbedtls_rsa_context *rsa;
	void *sign, *image;
	int image_len;
	unsigned char sha1sum[20];
	if(argc != 5){
		printf("Bad usage!\n");
		printf("Usage: ./verify pub_key_file image_file sign_file pub_key_bin");
		return 0;
	}

	mbedtls_pk_init(&pk);
	ret = mbedtls_pk_parse_public_keyfile( &pk, argv[1]);
	if( ret != 0){
		printf("Get pubkey failed\n");
		return 1;
	}
	rsa = mbedtls_pk_rsa(pk);
	//Get image data
	fp = fopen(argv[2], "rb");
	if(!fp){
		printf("Can't open %s\n", argv[2]);
		return 1;
	}
	fseek(fp, 0L, SEEK_END);
	image_len = ftell(fp);
	image = malloc(image_len);
	if(!image){
		printf("Malloc mem failed!\n");
		return 1;
	}

	memset(image, 0, image_len);
	fseek(fp, 0L, SEEK_SET);
	fread(image, image_len, 1, fp);
	fclose(fp);

	//Get sign data
	fp = fopen(argv[3], "rb");
	if(!fp){
		printf("Can't open %s\n", argv[3]);
		return 1;
	}
	fseek(fp, 0L, SEEK_END);
	len = ftell(fp);
	sign = malloc(len);
	memset(sign, 0, len);
	fseek(fp, 0L, SEEK_SET);
	fread(sign, len, 1, fp);
	fclose(fp);

	//get image hash message
    mbedtls_sha1(image, image_len, sha1sum);
	printf("0x%x\n", (unsigned int)rsa->N.p[0]);

	if(mbedtls_rsa_pkcs1_verify(rsa, NULL, NULL, MBEDTLS_RSA_PUBLIC, MBEDTLS_MD_SHA1, 0,
					sha1sum, sign) != 0)
	{
		printf("VERIFY: Verify img failed\n");
		goto end;
	}

	printf("VERIFY: Verify img ok!\n");

	fp = fopen(argv[4], "wb");
	if(!fp){
		printf("Can't open %s\n", argv[4]);
		return 1;
	}

	fwrite(rsa->N.p, rsa->len, 1, fp);
	printf("Write pubkey to file %s\n", argv[4]);

	fclose(fp);
end:
	free(sign);
	free(image);
	return 0;
}
