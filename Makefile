pipeline: main.o Setting.o ALU.o File.o Decode.o Hazard.o Pipeline.o ErrorDetect.o
	gcc -O3 -o pipeline main.o Setting.o ALU.o File.o Decode.o Hazard.o Pipeline.o ErrorDetect.o
%.o: %.c
	gcc -O3 -c $^

clean:
	rm -f *.o
