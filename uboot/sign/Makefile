CSOURCES:= asn1parse.c base64.c bignum.c md.c md_wrap.c oid.c pem.c pk.c pk_wrap.c pkparse.c rsa.c sha1.c
COBJECTS=$(CSOURCES:.c=.o)
sign: $(COBJECTS) sign.o
	cc -o sign $(COBJECTS) sign.o
	openssl genrsa -F4 -out private.key 1024

verify: $(COBJECTS) verify.o
	cc -o verify verify.o $(COBJECTS)
	openssl rsa -in private.key -pubout -out pub.key

clean:
	find -name "*.o" -exec rm {} \;
	find -name "*.key" -exec rm {} \;
	find -name "*.bin" -exec rm {} \;
	find -name "sign" -exec rm {} \;
	find -name "verify" -exec rm {} \;
