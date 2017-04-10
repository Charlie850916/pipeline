simulator: main.o Setting.o ALU.o File.o Decode.o Hazard.o Pipeline.o
	gcc -O3 -o simulator main.o Setting.o ALU.o File.o Decode.o Hazard.o Pipeline.o
%.o: %.c
	gcc -O3 -c $^

clean:
	rm -f *.o *.rpt *.bin
