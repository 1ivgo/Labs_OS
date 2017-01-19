all: main setup
main: writer.c reader.c
	g++ -pthread writer.c -o writed
	g++ -pthread reader.c -o readed
setup: create clean
create:
	rm shar_mem_file -f
	touch shar_mem_file
clean:
	rm File -f