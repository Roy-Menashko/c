JerryBoree: Jerry.o LinkedList.o KeyValuePair.o HashTable.o MultiValueHashTable.o JerryBoreeMain.o
	gcc Jerry.o LinkedList.o KeyValuePair.o HashTable.o MultiValueHashTable.o JerryBoreeMain.o -o JerryBoree
Jerry.o: Jerry.c Jerry.h
	gcc -c Jerry.c
LinkedList.o: LinkedList.c LinkedList.h
	gcc -c LinkedList.c
KeyValuePair.o: KeyValuePair.c KeyValuePair.h
	gcc -c KeyValuePair.c
HashTable.o: HashTable.c LinkedList.h KeyValuePair.h HashTable.h
	gcc -c HashTable.c
MultiValueHashTable.o:MultiValueHashTable.c LinkedList.h KeyValuePair.h HashTable.h MultiValueHashTable.h
	gcc -c MultiValueHashTable.c
JerryBoreeMain.o: JerryBoreeMain.c LinkedList.h KeyValuePair.h HashTable.h MultiValueHashTable.h
	gcc -c JerryBoreeMain.c
clean:
	rm -f *.o JerryBoree