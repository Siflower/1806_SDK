#ifndef	__SECURITY__H
#define __SECURITY__H
#define	RSA_KEY_LEN	128	/*rsa key len is 1024 bit*/
int rsa_verify_image(void *image, int len, void *sign);

#endif
