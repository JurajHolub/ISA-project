# ISA project
# @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
# @date 2019

CC = g++
CFLAGS = -I -g -lresolv
OBJ = main.o \
	  dns_query.o \
	  argument_parser.o \
	  tcp_socket.o \
	  whois_query.o \
	  geolocation_database.o
EXECUTABLE = isa-tazatel

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.cpp %.h
	$(CC) -c $< $(CFLAGS)

.PHONY: pack clean
pack:
	cp doc/manual.pdf . && tar -czvf xholub40.tar *.cpp *.h Makefile README manual.pdf

clean:
	rm -rf *.o *.out $(EXECUTABLE)
