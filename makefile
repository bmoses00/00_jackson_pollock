all:
	gcc -o image_maker image_maker.c -lm
	./image_maker

display:
	eog image.ppm

clean:
	rm image_maker
	rm image.ppm
