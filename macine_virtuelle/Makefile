CC = gcc
CFLAGS = -g
LDFLAGS = 

EXEC = test_machine

all  : $(EXEC)

$(EXEC) : main.o machine.o
	$(CC) $^ -o $@ $(LDFLAGS)

.PHONY : clean

clean : 
	-rm *.o

mrproper : clean
	-rm $(EXEC)