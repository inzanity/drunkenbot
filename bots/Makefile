all:
	make -C test

alot: all
	for i in 3 4 5; do cp test.so test$$i.so; done

clean:
	make -C test clean
	
distclean:
	make -C test distclean
	rm *.so
