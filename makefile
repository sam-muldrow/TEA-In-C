cbc:
	gcc tea-cbc-enc.c -o tea-cbc-enc
	gcc tea-cbc-dec.c -o tea-cbc-dec
clean:
	rm -f *.o tea-cbc-enc tea-cbc-dec