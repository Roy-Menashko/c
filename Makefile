# Makefile עבור פרויקט JerryBoree

# הגדרת משתנים
CC = gcc
CFLAGS = -Wall -Wextra -I. -g
LDFLAGS =
EXEC = main

# רשימת קבצי המקור
SRC = JerryBoree.c Jerry.c LinkedList.c KeyValuePair.c HashTable.c MultiValueHashTable.c

# המרה לקבצי אובייקט
OBJ = $(SRC:.c=.o)

# היעד הראשי
all: $(EXEC)

# כלל לבניית התוכנית
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ) $(LDFLAGS)

# כלל לבניית קובץ אובייקט מתוך קובץ מקור עם תלותיות
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

# כלל לניקוי הקבצים שנוצרו
clean:
	rm -f $(OBJ) $(EXEC)

# הגדרת .PHONY
.PHONY: all clean
