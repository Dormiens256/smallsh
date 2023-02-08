CC=gcc --std=c99 -g

exe_file = smallsh

$(exe_file): commands.o smallsh.c
	$(CC) commands.o smallsh.c -o $(exe_file)

commands.o: commands.c commands.h
	$(CC) -c commands.c


clean:
	rm -f *.out *.o $(exe_file)