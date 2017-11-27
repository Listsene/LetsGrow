tlcd2: tlcdtest.o tlcd.o
	arm-none-linux-gnueabi-gcc -o tlcd tlcdtest.o tlcd.o

tlcd.o: tlcd.c tlcd.h
	arm-none-linux-gnueabi-gcc -c tlcd.c

tlcdtest.o: tlcdtest.c tlcd.h
	arm-none-linux-gnueabi-gcc -c tlcdtest.c

clean:
	rm -f *.o

