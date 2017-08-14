mylang: ci.c yu.c main.c
	gcc -g -o mylang $^

clean:
	rm -rf mylang
