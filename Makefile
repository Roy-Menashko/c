JerryBoree: Jerry.o LinkedList.o KeyValuePair.o HashTable.o MultiValueHashTable.o JerryBoreeMain.o
	gcc Jerry.o LinkedList.o KeyValuePair.o HashTable.o MultiValueHashTable.o JerryBoreeMain.o -o JerryBoree
Jerry.o: Jerry.c Jerry.h
	gcc -c Jerry.c
LinkedList.o: LinkedList.c LinkedList.h
	gcc -c LinkedList.c
KeyValuePair.o: KeyValuePair.c KeyValuePair.h
	gcc -c KeyValuePair.c
HashTable.o: LinkedList.c LinkedList.h KeyValuePair.c KeyValuePair.h HashTable.c HashTable.h
	gcc -c HashTable.c
MultiValueHashTable.o: LinkedList.c LinkedList.h KeyValuePair.c KeyValuePair.h HashTable.c HashTable.h MultiValueHashTable.c MultiValueHashTable.h
	gcc -c MultiValueHashTable.c
JerryBoreeMain.o: LinkedList.c LinkedList.h KeyValuePair.c KeyValuePair.h HashTable.c HashTable.h MultiValueHashTable.c MultiValueHashTable.h JerryBoreeMain.c
	gcc -c JerryBoreeMain.c
clean:
	rm -f *.o JerryBoree