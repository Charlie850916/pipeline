simulator: main.o Setting.o ALU.o File.o Decode.o Hazard.o Pipeline.o Error.o
	gcc -O3 -o simulator main.o Setting.o ALU.o File.o Decode.o Hazard.o Pipeline.o Error.o
%.o: %.c
	gcc -O3 -c $^

clean:
	rm -f *.o *.rpt


test: b a c d
a:
	./simulator
b:
	./pipeline
c:
	diff a.rpt snapshot.rpt
d:
	diff b.rpt error_dump.rpt
