DIROBJ := obj/
DIREXE := exec/
DIRHEA := include/
DIRSRC := src/
DIREST := directories/
FILELOG := files/log.txt

CFLAGS := -I $(DIRHEA) -c -ansi
LDLIBS := -lpthread -lrt
CC := gcc

all : dirs manager pa pb pc pd

dirs:
	mkdir -p $(DIROBJ) $(DIREXE)

manager: $(DIROBJ)manager.o 
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)

pa: $(DIROBJ)pa.o 
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)

pb: $(DIROBJ)pb.o 
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)

pc: $(DIROBJ)pc.o 
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)
	
pd: $(DIROBJ)pd.o 
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)

$(DIROBJ)%.o: $(DIRSRC)%.c
	$(CC) $(CFLAGS) $^ -o $@


solution:
	./$(DIREXE)manager

clean : 
	rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIREST) $(FILELOG) $(DIRHEA)*~ $(DIRSRC)*~ 
