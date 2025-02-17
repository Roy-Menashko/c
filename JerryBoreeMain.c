

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Defs.h"
#include "Jerry.h"               /* עבור פונקציות יצירה/הרס של Jerry, Planet וכו' */
#include "LinkedList.h"
#include "HashTable.h"
#include "MultiValueHashTable.h"


static LinkedList g_planetsList = NULL;       /* LinkedList של Planet* */
static LinkedList g_jerriesList = NULL;       /* LinkedList של Jerry* */
static hashTable g_jerriesHash = NULL;        /* key=ID(string), value=Jerry* */
static hashTableProMax g_physicalHash = NULL; /* key=PhysicalName(string), value=LinkedList<Jerry*> */

/* get functions*/
LinkedList getPlanetsList()      { return g_planetsList; }
LinkedList getJerriesList()      { return g_jerriesList; }
hashTable getJerriesHash()       { return g_jerriesHash; }
hashTableProMax getPhysicalHash(){ return g_physicalHash; }

/* ------------------------------------------------------------------
   פונקציות עזר מינימליות עבור מבני הנתונים השונים (Hash/LinkedList)
   ------------------------------------------------------------------ */
void clearInputBuffer() {
    int c; // Variable to store each character read

    // Read characters from the input buffer until a newline ('\n') or EOF is encountered
    while ((c = getchar()) != '\n' && c != EOF);
}

/*
 * printJerryWrapper:
 * A wrapper function to print a `Jerry` object when the input is provided as a generic `Element`.
 * - Converts the generic `Element` (void*) to a specific `Jerry*`.
 * - Calls the `printJerry` function to handle the actual printing.
 *
 * Parameters:
 * - e: A generic pointer (`Element`) expected to point to a `Jerry` object.
 *
 * Returns:
 * - success: If the `Jerry` object is printed successfully.
 * - failure: If the input is NULL or the printing fails.
 */
status printJerryWrapper(Element e) {
    // Validate input to ensure it is not NULL.
    if (!e) {
        return failure; // Invalid input, cannot print.
    }

    // Cast the generic `Element` (void*) to a `Jerry*`.
    Jerry* j = (Jerry*)e;

    // Call the `printJerry` function to print the `Jerry` object.
    return printJerry(j);
}



/*
 * compareStrings:
 * Compares two strings (used as keys in hash tables).
 * - Converts the generic `Element` type (void*) to `char*` for string comparison.
 * - Uses `strcmp` to check if the two strings are equal.
 *
 * Parameters:
 * - e1: A generic pointer (`Element`) expected to point to a string (char*).
 * - e2: A generic pointer (`Element`) expected to point to a string (char*).
 *
 * Returns:
 * - true: If the two strings are equal.
 * - false: If either input is NULL or the strings are not equal.
 */
static bool compareStrings(Element e1, Element e2) {
    // Validate inputs to ensure neither is NULL.
    if (!e1 || !e2) {
        return false; // One or both inputs are invalid, return false.
    }

    // Compare the two strings using strcmp.
    // strcmp returns 0 if the strings are equal.
    return (strcmp((char*)e1, (char*)e2) == 0);
}


/*
 * transformStringToNumber:
 * - Purpose: Converts a string into a numeric value, typically used as a transformation function for a hash table.
 * - Input Validation: If `key` is NULL, the function returns 0.
 * - Logic:
 *   - Iterates through each character in the string.
 *   - Adds the ASCII value of each character to the variable `sum`.
 *   - The final result is the sum of all ASCII values of the characters in the string.
 * - Stability: Casts each character to `unsigned char` to handle potential issues with negative values.
 * - Output: Returns the sum as an integer.
 */

static int transformStringToNumber(Element key)
{
    if (!key) return 0;
    int sum = 0;
    const char* str = (const char*)key;
    while (*str)
        sum += (unsigned char)(*str++);
    return sum;
}

/*
 * copyString:
 * - Purpose: Creates a duplicate of a string.
 * - Input Validation: If `e` is NULL, the function returns NULL.
 * - Logic:
 *   - Uses the `strdup` function to create a dynamically allocated copy of the string.
 *   - Ensures independent memory management for the duplicate.
 * - Stability: Returns NULL if `strdup` fails.
 * - Output: Returns a pointer to the newly allocated string or NULL in case of an error.
 */
static Element copyString(Element e) {
    if (!e) return NULL;
    char* copy = strdup((char*)e);
    return copy;
}

/*
 * comparePlanets:
 * - Purpose: Compares two `Planet` objects by their `name` field.
 * - Input Validation: Both `e1` and `e2` are cast from generic `Element` pointers to `Planet*`.
 * - Logic:
 *   - Uses `strcmp` to compare the `name` field of both objects.
 *   - Returns `true` if the names match, otherwise `false`.
 * - Stability: Ensure the `name` field in each object is a valid string.
 */
static bool comparePlanets(Element e1, Element e2)
{
    Planet* p1 = (Planet*)e1;
    Planet* p2 = (Planet*)e2;
    return (strcmp(p1->name, p2->name) == 0);
}

/*
 * compareJerries:
 * - Purpose: Compares two `Jerry` objects by their `ID`.
 * - Input Validation: Both `e1` and `e2` are cast from generic `Element` pointers to `Jerry*`.
 * - Logic:
 *   - Uses `strcmp` to compare the `ID` field of both objects.
 *   - Returns `true` if the IDs match, otherwise `false`.
 * - Stability: Ensure the `ID` field in each object is a valid string.
 */
static bool compareJerries(Element e1, Element e2)
{
    Jerry* j1 = (Jerry*)e1;
    Jerry* j2 = (Jerry*)e2;
    return (strcmp(j1->ID, j2->ID) == 0);
}


/*
 * printStringPtr:
 * - Purpose: Prints a string stored as a generic `Element` pointer.
 * - Input Validation: If `e` is NULL, the function returns `failure`.
 * - Logic:
 *   - Casts the input `e` to a `char*`.
 *   - Uses `printf` to print the string.
 * - Output: Returns `success` if the printing succeeds, otherwise `failure`.
 */
static status printStringPtr(Element e)
{
    if (!e) return failure;
    printf("%s", (char*)e);
    return success;
}

/*
 * copyShallow:
 * - Purpose: Returns the input pointer as-is without creating a new copy.
 * - Logic: Simply returns the `Element` without modifying or duplicating it.
 * - Use Case: Useful for shallow copying in hash tables or data structures where duplication is unnecessary.
 * - Output: The same pointer as the input.
 */
static Element copyShallow(Element e)
{
    return e;
}

/*
 * freeNoOp:
 * - Purpose: A "no-op" function for freeing resources, does nothing with the input.
 * - Logic: Suppresses unused variable warnings with `(void)e`.
 * - Use Case: Used when no destruction or freeing is required for a value.
 * - Output: Always returns `success`.
 */
static status freeNoOp(Element e)
{
    (void)e;
    return success;
}


/*
 * get_abs:
 * - Purpose: Calculates the absolute difference between two floating-point numbers.
 * - Logic: Checks if `x > y` and returns `x - y`; otherwise, returns `y - x`.
 * - Output: The positive difference between `x` and `y`.
 */
float get_abs(float x,float y) {
    if (x > y){
        return x-y;
    }
    return y - x;
}

/*
 * min_abs:
 * - Purpose: Finds the `Jerry` object in a linked list with the closest physical characteristic value to the given `value`.
 * - Input Validation: Returns `NULL` if the linked list is empty.
 * - Logic:
 *   - Iterates over all nodes in the linked list.
 *   - Uses `has_physical` to check if the `Jerry` has the specified physical characteristic.
 *   - Computes the absolute difference between the characteristic value and the target `value`.
 *   - Tracks the minimum difference and selects the `Jerry` with the closest value.
 * - Output: Pointer to the `Jerry` object with the closest characteristic value or `NULL` if no match is found.
 */
Jerry* min_abs(LinkedList l,char* physical,float value) {
    if (!l) return NULL;
    node current = getFirstElement(l);
    float min = INFINITY;
    Jerry* chosen = NULL;
    while (current) {
        if (get_data(current)) {
            Jerry* j = (Jerry*)current;
            if (has_physical(*j, (char*)physical)){
                float jerry_val = get_value(j,physical);
                if (get_abs(jerry_val,value) < min) {
                    min = get_abs(jerry_val,value);
                    chosen = j;
                }
            }
        }
        current = getNextElement(l,current);
    }
    return chosen;
}

/*
 * max:
 * - Purpose: Returns the larger of two integers.
 * - Logic: Uses a ternary operator to compare `a` and `b`.
 * - Output: The greater of the two integers.
 */
int max(int a,int b) {
    return a>b?a:b;
}

/*
 * min:
 * - Purpose: Returns the smaller of two integers.
 * - Logic: Uses a ternary operator to compare `a` and `b`.
 * - Output: The smaller of the two integers.
 */
int min(int a,int b) {
    return a<b?a:b;
}

/*
 * hasPlanetName:
 * - Purpose: Checks if a `Planet` object matches a given name.
 * - Input Validation: Returns `false` if either input is `NULL`.
 * - Logic:
 *   - Casts `element` to `Planet*` and `dataKey` to `char*`.
 *   - Compares the `name` field of the `Planet` with the input `dataKey` using `strcmp`.
 * - Output: `true` if the names match; otherwise, `false`.
 */
bool hasPlanetName(Element element, Element dataKey) {
    if (!element || !dataKey) return false;
    Planet* planet = (Planet*)element;
    char* name = (char*)dataKey;
    return strcmp(planet->name, name) == 0;
}

/*
 * isJerryIDEqual:
 * - Purpose: Checks if a `Jerry` object's ID matches a given string ID.
 * - Input Validation: Returns `false` if either `jerry` or `id` is `NULL`.
 * - Logic:
 *   - Uses `strcmp` to compare the `Jerry` object's `ID` field with the input `id`.
 * - Output: `true` if the IDs match; otherwise, `false`.
 */
bool isJerryIDEqual(const Jerry* jerry, const char* id) {
    if (jerry == NULL || id == NULL) {
        return false;
    }
    return strcmp(jerry->ID, id) == 0;
}

/*
 * isJerryIDEqualWrapper:
 * - Purpose: A wrapper function for `isJerryIDEqual` to compare a `Jerry` object's ID with a given string ID.
 * - Logic:
 *   - Casts `e1` to `Jerry*` and `e2` to `char*`.
 *   - Calls `isJerryIDEqual` to perform the comparison.
 * - Output: `true` if the IDs match; otherwise, `false`.
 */
bool isJerryIDEqualWrapper(Element e1, Element e2) {
    Jerry* j = (Jerry*)e1;
    char* id = (char*)e2;
    return isJerryIDEqual(j, id);
}

/*
 * findPlanetByName:
 * - Purpose: Finds a `Planet` object in the global linked list (`g_planetsList`) by its name.
 * - Input Validation: Returns `NULL` if the list or name is `NULL`.
 * - Logic:
 *   - Calls `searchByKeyInList` with the global list, name, and `hasPlanetName` function.
 *   - Casts and returns the result as a `Planet*`.
 * - Output: Pointer to the matching `Planet` or `NULL` if not found.
 */
Planet* findPlanetByName(char* name) {
    if (g_planetsList == NULL || name == NULL) {
        return NULL;
    }

    Element result = searchByKeyInList(g_planetsList, name, hasPlanetName);
    return (Planet*)result;
}

/*
 * freePlanetPtr:
 * - Purpose: Frees a `Planet` object, compatible with generic `Element` types.
 * - Input Validation: Returns `failure` if `e` is `NULL`.
 * - Logic:
 *   - Casts `e` to `Planet*` and calls `destroyPlanet`.
 * - Output: `success` if the operation succeeds; otherwise, `failure`.
 */
static status freePlanetPtr(Element e)
{
    if (!e) return failure;
    destroyPlanet((Planet*)e);
    return success;
}

/*
 * printPlanetPtr:
 * - Purpose: Prints a `Planet` object, compatible with generic `Element` types.
 * - Input Validation: Returns `failure` if `e` is `NULL`.
 * - Logic:
 *   - Casts `e` to `Planet*` and calls `printPlanet`.
 * - Output: `success` if the operation succeeds; otherwise, `failure`.
 */
static status printPlanetPtr(Element e)
{
    if (!e) return failure;
    printPlanet((Planet*)e);
    return success;
}

/*
 * comparePlanetByName:
 * - Purpose: Compares two `Planet` objects by their names.
 * - Logic:
 *   - Calls `comparePlanets` to perform the comparison.
 * - Output: `true` if the names match; otherwise, `false`.
 */
static bool comparePlanetByName(Element e1, Element e2)
{
    return comparePlanets(e1, e2);
}

/*
 * freeJerryPtr:
 * - Purpose: Frees a `Jerry` object, compatible with generic `Element` types.
 * - Input Validation: Returns `failure` if `e` is `NULL`.
 * - Logic:
 *   - Casts `e` to `Jerry*` and calls `destroyJerry`.
 *   - Ensure the function name matches the implementation in `Jerry.c`.
 * - Output: `success` if the operation succeeds; otherwise, `failure`.
 */
static status freeJerryPtr(Element e)
{
    if (!e) return failure;
    destoyJerry((Jerry*)e);  // <-- Make sure this matches your Jerry.c function name
    return success;
}

/*
 * printJerryPtr:
 * - Purpose: Prints a `Jerry` object, compatible with generic `Element` types.
 * - Input Validation: Returns `failure` if `e` is `NULL`.
 * - Logic:
 *   - Casts `e` to `Jerry*` and calls `printJerry`.
 * - Output: `success` if the operation succeeds; otherwise, `failure`.
 */
static status printJerryPtr(Element e)
{
    if (!e) return failure;
    printJerry((Jerry*)e);
    return success;
}

/*
 * freeStringPtr:
 * - Purpose: Frees a dynamically allocated string, compatible with generic `Element` types.
 * - Input Validation: Returns `failure` if `e` is `NULL`.
 * - Logic:
 *   - Calls `free` to deallocate memory for the string.
 * - Output: `success` if the operation succeeds; otherwise, `failure`.
 */
static status freeStringPtr(Element e) {
    if (!e) return failure;
    free(e);
    return success;
}

/*
 * isPrime:
 * - Purpose: Determines whether a given integer `n` is a prime number.
 * - Input Validation: Returns `false` for numbers less than or equal to 1.
 * - Logic:
 *   - Returns `true` for 2 and 3 since they are prime.
 *   - Checks divisibility by 2 or 3; if divisible, returns `false`.
 *   - Iterates through numbers of the form 6k ± 1 up to √n to check for divisors.
 * - Output: `true` if `n` is prime; otherwise, `false`.
 */
bool isPrime(int n) {
    if (n <= 1) {
        return false;
    }
    if (n <= 3) {
        return true;
    }

    if (n % 2 == 0 || n % 3 == 0) {
        return false;
    }


    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }
    return true;
}

/*
 * nextPrime:
 * - Purpose: Finds the smallest prime number greater than the given integer `n`.
 * - Logic:
 *   - Increments `n` and checks if it is prime using `isPrime`.
 *   - Continues until a prime number is found.
 * - Output: The next prime number after `n`.
 */
int nextPrime(int n) {
    // נתחיל מהמספר הבא
    n++;
    // נמשיך להעלות את n עד שיהיה ראשוני
    while (!isPrime(n)) {
        n++;
    }
    return n;
}

/*
 * delete_jerry:
 * - Purpose: Removes a `Jerry` object from all relevant data structures.
 * - Input Validation: Returns `failure` if the input `Jerry` is NULL.
 * - Logic:
 *   - Removes the `Jerry` from the global hash table of Jerries by ID.
 *   - Iterates through the `Jerry`'s physical characteristics and removes them from the physical hash table.
 *   - Removes the `Jerry` from the global linked list of Jerries.
 *   - Handles failures in each removal step.
 * - Output: `success` if all removals succeed; otherwise, `failure`.
 */
status delete_jerry(Jerry* j) {
    if (!j) {
        return failure;
    }


    if (removeFromHashTable(g_jerriesHash, j->ID) == failure) {
        return failure;
    }


    for (int i = 0; i < j->num_of_pyhshical; i++) {
        char* physName = j->his_physical[i]->name;
        if (removeFromHashTableProMax(g_physicalHash, physName, j) == failure) {

        }
    }


    if (deleteNode(g_jerriesList, j) == failure) {

        return failure;
    }

    return success;
}

/*
 * destroyAll:
 * - Purpose: Frees all global data structures and resets their pointers to NULL.
 * - Logic:
 *   - Checks if each global structure exists before attempting to free it.
 *   - Frees:
 *     - `g_jerriesHash`: The hash table of Jerries.
 *     - `g_physicalHash`: The multi-value hash table of physical characteristics.
 *     - `g_planetsList`: The linked list of planets.
 *     - `g_jerriesList`: The linked list of Jerries.
 *   - Sets each global pointer to NULL after freeing.
 * - Output: Cleans up all resources and prevents dangling pointers.
 */
void destroyAll()
{
    if (g_jerriesHash) {
        destroyHashTable(g_jerriesHash);  /* Your function to free the hashTable */
        g_jerriesHash = NULL;
    }

    if (g_physicalHash) {
        destroyHashTableProMax(g_physicalHash); /* Frees the multi-value hash table */
        g_physicalHash = NULL;
    }

    if (g_planetsList) {
        destroyLinkedList(g_planetsList); /* Frees the linked list of Planet* */
        g_planetsList = NULL;
    }

    if (g_jerriesList) {
        destroyLinkedList(g_jerriesList); /* Frees the linked list of Jerry* */
        g_jerriesList = NULL;
    }
}
/* ------------------------------------------------------------------
   הפונקציה המרכזית: readConfigAndBuild – קוראת את הקובץ ומבנה את המבנים
   ------------------------------------------------------------------ */


/*
 * readConfigAndBuild:
 * - Purpose: Reads a configuration file, builds linked lists of Planets and Jerries, and constructs hash tables.
 * - Input Validation:
 *     - Returns `failure` if the `fileName` is NULL.
 *     - Returns `failure` if the file cannot be opened.
 * - Logic:
 *     1. **File Opening**:
 *         - Attempts to open the file specified by `fileName` in read mode.
 *         - If the file fails to open, prints an error message, calls `destroyAll` to clean up any initialized structures, and returns `failure`.
 *     2. **Initialization of Linked Lists**:
 *         - Initializes `g_planetsList` using `createLinkedList` with appropriate functions for freeing, comparing, and printing Planets.
 *             - If initialization fails, closes the file and returns `failure`.
 *         - Initializes `g_jerriesList` using `createLinkedList` with appropriate functions for freeing, comparing, and printing Jerries.
 *             - If initialization fails, destroys `g_planetsList`, closes the file, and returns `failure`.
 *     3. **Reading Configuration File**:
 *         - Sets up variables to track the current reading section (`readingPlanets`, `readingJerries`).
 *         - Uses a buffer `line` to read each line from the file.
 *         - Initializes `currentJerry` to keep track of the current Jerry being processed.
 *         - Initializes counters `jerryCount` and `physicalCount` for potential dynamic sizing of hash tables.
 *         - **File Parsing Loop**:
 *             - Reads the file line by line using `fgets`.
 *             - Removes any trailing newline characters (`\n` or `\r\n`) from the line.
 *             - Checks for section headers ("Planets" or "Jerries") to switch the reading context.
 *             - **Planets Section**:
 *                 - Parses each line expected to be in the format `"name,x,y,z"`.
 *                 - Uses `sscanf` to extract `planetName`, `x`, `y`, and `z` coordinates.
 *                 - Creates a new `Planet` object using `createPlanet`.
 *                     - If creation fails, closes the file, destroys all initialized structures, prints an error message, and returns `failure`.
 *                 - Appends the new `Planet` to `g_planetsList` using `appendNode`.
 *                     - If appending fails, destroys the created `Planet`, closes the file, destroys all initialized structures, and returns `failure`.
 *             - **Jerries Section**:
 *                 - **Physical Characteristics**:
 *                     - Lines starting with a tab (`\t`) indicate physical characteristics in the format `"\tName:Value"`.
 *                     - Parses `propName` and `propValue` using `sscanf`.
 *                     - Creates a new `PhysicalCharacteristics` object using `createPhysicalCharacteristics`.
 *                         - If creation fails, closes the file, destroys all initialized structures, prints an error message, and returns `failure`.
 *                     - Adds the physical characteristic to the current `Jerry` using `add_physical_to_jerry`.
 *                         - If adding fails, destroys the created `PhysicalCharacteristics`, closes the file, destroys all initialized structures, prints an error message, and returns `failure`.
 *                     - Increments `physicalCount`.
 *                 - **New Jerry Entry**:
 *                     - Lines not starting with a tab indicate a new Jerry in the format `"id,dimension,planet,happiness"`.
 *                     - Parses `id`, `dimension`, `planetName`, and `happiness` using `sscanf`.
 *                     - Searches for the corresponding `Planet` in `g_planetsList` by comparing `planetName`.
 *                         - Iterates through `g_planetsList` using `getFirstElement` and `getNextElement`.
 *                         - If the `Planet` is not found, closes the file and returns `failure`.
 *                     - Creates an `Origin` object using `createOrigin` with the found `Planet` and `dimension`.
 *                         - If creation fails, closes the file, destroys all initialized structures, prints an error message, and returns `failure`.
 *                     - Creates a new `Jerry` object using `createJerry` with `id`, `happiness`, and the created `Origin`.
 *                         - If creation fails, destroys the `Origin`, closes the file, destroys all initialized structures, prints an error message, and returns `failure`.
 *                     - Appends the new `Jerry` to `g_jerriesList` using `appendNode`.
 *                         - If appending fails, destroys the created `Jerry`, closes the file, destroys all initialized structures, prints an error message, and returns `failure`.
 *                     - Updates `currentJerry` to point to the newly created `Jerry`.
 *                     - Increments `jerryCount`.
 *     4. **Closing the File**:
 *         - After finishing the reading loop, closes the file using `fclose`.
 *     5. **Building Hash Tables**:
 *         - **Jerries Hash Table (`g_jerriesHash`)**:
 *             - Determines the size of the hash table using `nextPrime` based on the length of `g_jerriesList`.
 *             - Creates the hash table using `createHashTable` with appropriate functions for copying, freeing, printing keys and values, comparing keys, and transforming strings to numbers.
 *                 - If creation fails, returns `failure`.
 *             - Inserts all `Jerries` from `g_jerriesList` into `g_jerriesHash` using `addToHashTable`.
 *         - **Physical Characteristics Hash Table (`g_physicalHash`)**:
 *             - Creates the multi-value hash table using `createHashTableProMax` with appropriate functions for handling keys and values.
 *                 - Keys are physical characteristic names (`physName`), and values are lists of `Jerries` having that characteristic.
 *                 - If creation fails, destroys all initialized structures, prints an error message, and returns `failure`.
 *             - Iterates through each `Jerry` in `g_jerriesList` and adds them to `g_physicalHash` based on their physical characteristics using `addToHashTableProMax`.
 *     6. **Final Output**:
 *         - Returns `success` if all operations complete without failures.
 * - Output:
 *     - Returns `success` if the configuration file is read and all data structures are built successfully.
 *     - Returns `failure` if any step fails during the process.
 */
status readConfigAndBuild(const char* fileName)
{
    if (!fileName)
        return failure;

    FILE* file = fopen(fileName, "r");
    if (!file) {
        printf("A memory problem has been detected in the program");
        destroyAll();
        return failure;
    }

    /* Initialize data structures: Linked lists */
    g_planetsList = createLinkedList(freePlanetPtr, comparePlanetByName, printPlanetPtr);
    if (!g_planetsList) {
        fclose(file);
        return failure;
    }

    g_jerriesList = createLinkedList(freeJerryPtr, compareJerries, printJerryPtr);
    if (!g_jerriesList) {
        destroyLinkedList(g_planetsList);
        fclose(file);
        return failure;
    }

    /* Variables to track reading stage */
    bool readingPlanets = false;
    bool readingJerries = false;

    // Buffer for reading lines from the file
    char line[301];
    Jerry* currentJerry = NULL;

    /* Counters for Jerry and physical characteristics for hash table sizing (if dynamic) */
    int jerryCount = 0;
    int physicalCount = 0;

    /* ------------ First Pass in the File: Building Lists (Planets and Jerries) ----------- */
    while (fgets(line, sizeof(line), file))
    {
        /* Remove \n or \r\n from the end of the line */
        line[strcspn(line, "\r\n")] = '\0';

        // Check for section headers and switch reading stages
        if (strcmp(line, "Planets") == 0) {
            readingPlanets = true;
            readingJerries = false;
            continue;
        }
        else if (strcmp(line, "Jerries") == 0) {
            readingPlanets = false;
            readingJerries = true;
            continue;
        }

        /* --- Planets Section --- */
        if (readingPlanets)
        {
            char planetName[300];
            float x=0, y=0, z=0;
            // Parse the line for Planet data (format: "name,x,y,z")
            if (sscanf(line, "%299[^,],%f,%f,%f", planetName, &x, &y, &z) == 4) {
                // Create a new Planet object
                Planet* p = createPlanet(planetName, x, y, z);
                if (!p) {
                    // Handle memory allocation failure
                    fclose(file);
                    destroyAll();
                    printf("A memory problem has been detected in the program");
                    return failure;
                }

                // Append the Planet to the global list
                if (appendNode(g_planetsList, p) == failure) {
                    destroyPlanet(p);  // Free the planet if appending fails
                    fclose(file);
                    destroyAll();
                    printf("A memory problem has been detected in the program");
                    return failure;
                }
            }
        }
        /* --- Jerries Section --- */
        else if (readingJerries)
        {
            /* If the line starts with a tab, it indicates a physical characteristic */
            if (line[0] == '\t') {
                char propName[300];
                float propValue=0;
                /* Format: "\tName:Value" */
                if (sscanf(line+1, "%299[^:]:%f", propName, &propValue) == 2) {
                    // Create a new PhysicalCharacteristics object
                    PhysicalCharacteristics* pc = createPhysicalCharacteristics(propName, propValue);
                    if (!pc) {
                        // Handle memory allocation failure
                        fclose(file);
                        destroyAll();
                        printf("A memory problem has been detected in the program");
                        return failure;
                    }

                    // Add the physical characteristic to the current Jerry
                    if (add_physical_to_jerry(currentJerry, pc) == failure) {
                        destroyPhysicalCharacteristics(pc);  // Free the characteristic if adding fails
                        fclose(file);
                        destroyAll();
                        printf("A memory problem has been detected in the program");
                        return failure;
                    }
                    physicalCount++;  // Increment physical characteristic counter
                }
            }
            else {
                /* New Jerry format: "id,dimension,planet,happiness" */
                char id[300], dimension[300], planetName[300];
                int happiness=0;
                // Parse the line for Jerry data
                if (sscanf(line, "%299[^,],%299[^,],%299[^,],%d",
                           id, dimension, planetName, &happiness) == 4)
                {
                    /* Locate the planet in the planets list */
                    Planet* pFound = NULL;
                    Element el = getFirstElement(g_planetsList);
                    while (el) {
                        Planet* pCheck = (Planet*)el;
                        if (strcmp(pCheck->name, planetName) == 0) {
                            pFound = pCheck;
                            break;
                        }
                        el = getNextElement(g_planetsList, el);
                    }
                    if (!pFound) {
                        // Error if planet not found
                        fclose(file);
                        return failure;
                    }

                    /* Create Origin and then Jerry */
                    Jerry* j = createJerry(id, happiness, pFound, dimension);
                    if (!j) {
                        fclose(file);
                        destroyAll();
                        printf("A memory problem has been detected in the program");
                        return failure;
                    }

                    /* Append Jerry to the list */
                    if (appendNode(g_jerriesList, j) == failure) {
                        destoyJerry(j);  // Free the Jerry if appending fails
                        fclose(file);
                        destroyAll();
                        printf("A memory problem has been detected in the program");
                        return failure;
                    }
                    currentJerry = j;  // Update the current Jerry pointer
                    jerryCount++;      // Increment Jerry counter
                }
            }
        }
    }

    /* Done reading file; close it. */
    fclose(file);

    /* ---------------- Build Hash Tables ---------------- */

    /* Jerries Hash Table by ID */
    /*
     * Determine the size of the hash table using `nextPrime` based on the number of Jerries.
     * Creates the hash table with appropriate functions for handling keys and values.
     */
    int l = getLength(g_jerriesList);
    g_jerriesHash = createHashTable(
        copyString,            /* Key copying function (string) – shallow copy */
        freeStringPtr,         /* Key destruction function (string) */
        printStringPtr,        /* Key printing function (string) */
        copyShallow,           /* Value copying function (Jerry*) – shallow copy */
        freeNoOp,              /* Value destruction function (no-op, managed by the list) */
        printJerryWrapper,     /* Value printing function – prints only Jerry ID */
        compareStrings,        /* Key comparison function */
        transformStringToNumber, /* Transformation function */
        nextPrime(l)           /* Hash table size */
    );
    if (!g_jerriesHash) return failure;

    /* Insert all Jerries from the list into the hash table */
    Element node = getFirstElement(g_jerriesList);
    while (node) {
        Jerry* j = (Jerry*)node;
        if(addToHashTable(g_jerriesHash, j->ID, j)==failure) {
            destroyAll();
            printf("A memory problem has been detected in the program");
            return 1;
        }
        node = getNextElement(g_jerriesList, node);
    }

    /* Physical Characteristics Hash Table (key: physical name, value: list of Jerries) */
    /*
     * Creates a multi-value hash table where each key is a physical characteristic name,
     * and the value is a linked list of Jerries that possess that characteristic.
     */
    g_physicalHash = createHashTableProMax(
        copyString,          /* copyKey */
        freeStringPtr,       /* freeKey */
        printStringPtr,      /* printKey */
        copyShallow,         /* copyValue (stores LinkedList) */
        freeNoOp,            /* freeValue (do not free, because Jerry is managed externally) */
        printJerryWrapper,   /* printValue = displays the list */
        compareStrings,      /* Key comparison function (string) */
        compareStrings,      /* Value comparison function (not really used here) */
        transformStringToNumber,
        nextPrime(l)         /* Hash table size, can be adjusted */
    );
    if (!g_physicalHash) {
        // If hash table creation fails, clean up and report
        destroyAll();
        printf("A memory problem has been detected in the program");
        return failure;
    }

    /* For each Jerry, for each physical characteristic => add to HashTableProMax */
    node = getFirstElement(g_jerriesList);
    while (node) {
        Jerry* j = (Jerry*)node;
        for (int i = 0; i < j->num_of_pyhshical; i++) {
            /*
             * Ensure that the field name `his_physical` and `num_of_pyhshical`
             * correctly match the Jerry structure definition.
             */
            char* physName = j->his_physical[i]->name;
            if(addToHashTableProMax(g_physicalHash, physName, j)==failure) {
                destroyAll();
                printf("A memory problem has been detected in the program");
                return 1;
            }
        }
        node = getNextElement(g_jerriesList, node);
    }

    return success;
}

/*
 * runTest:
 * - Purpose: To execute the test by reading the configuration file and building the necessary data structures.
 * - Steps:
 *   1. Calls `readConfigAndBuild` with the provided `configFile`.
 *      - This function reads the configuration file and initializes the global data structures (linked lists, hash tables, etc.).
 *      - Returns `success` if the operation completes successfully or `failure` if any step fails.
 *   2. Checks the `result` of `readConfigAndBuild`.
 *      - If the result is `failure`, prints an error message to `stderr` indicating that the configuration file could not be read or processed.
 * - Notes:
 *   - Assumes that `readConfigAndBuild` properly handles memory cleanup and error reporting for internal failures.
 *   - The function is a minimal wrapper for testing the configuration file parsing and data structure initialization logic.
 */

status runTest(const char* configFile)
{
    /* 1) Build data from the config file */
    status result = readConfigAndBuild(configFile);
    if (result == failure) {
        return failure;
    }
    return success;
}
/*
 * printMenu:
 * - Purpose: Displays a menu of options for the user, themed around Rick and Morty.
 * - Functionality:
 *   - Prints a welcome message addressing Rick.
 *   - Lists nine numbered options, each corresponding to a potential action Rick can take regarding Jerries.
 *     1. Option to remove a Jerry.
 *     2. Option to recall information about a Jerry.
 *     3. Option to correct or adjust something related to a Jerry.
 *     4. Option to retrieve a previously removed Jerry.
 *     5. Option to find a similar Jerry when the original one is missing.
 *     6. Option to request the "saddest" Jerry, possibly due to losing a bet.
 *     7. Option to display all Jerries.
 *     8. Option to engage Jerries in some form of activity or interaction.
 *     9. Option to close the program or "end the experience."
 * - Notes:
 *   - The menu is thematic and humorously written to align with the Rick and Morty series' style.
 *   - Assumes the menu options correspond to specific actions handled elsewhere in the program.
 *   - Provides user-friendly prompts with clear option numbering for input.
 */

void printMenu() {
    printf("Welcome Rick, what are your Jerry's needs today ?\n");
    printf("1 : Take this Jerry away from me\n");
    printf("2 : I think I remember something about my Jerry\n");
    printf("3 : Oh wait. That can't be right\n");
    printf("4 : I guess I will take back my Jerry now\n");
    printf("5 : I can't find my Jerry. Just give me a similar one\n");
    printf("6 : I lost a bet. Give me your saddest Jerry\n");
    printf("7 : Show me what you got\n");
    printf("8 : Let the Jerries play\n");
    printf("9 : I had enough. Close this place\n");
}
/************************************************************/
/* Below is the entire code with added detailed English     */
/* comments. Nothing in the original code (variables,       */
/* functions, structure, logic, or Hebrew comments) has     */
/* been changed or removed. All original code and comments  */
/* remain intact, and only new English comments have been   */
/* inserted for clarification.                              */
/************************************************************/

int main(int argc, char* argv[]) {
    // Check if there is at least one argument (the configuration file name).
    // If there's none, print usage and return with error code 1.
    if (argc < 2) {
        printf("Usage: %s <configFile>\n", argv[0]);
        return 1;
    }

    /* We pass argv[1] as the config file name. */
    // Here, the code calls runTest(argv[2]), which implies it expects a second argument beyond the config file.
    // However, we are NOT changing anything in the code; just noting that argv[2] is being used.
    if (runTest(argv[2]) == failure) {
        destroyAll();
        return 1;
    }

    // A boolean flag to control the main loop of the program.
    bool done = false; // Flag to control the main program loop

    // Main loop that continues until 'done' becomes true.
    while (!done) {
        // Print the main menu to the user.
        printMenu(); // Print the main menu for user options

        char input[301]; // Buffer to store user input

        // Read user input from stdin and remove the trailing newline character, if any.
        if (fgets(input, sizeof(input), stdin) != NULL) {
            input[strcspn(input, "\n")] = '\0';
        }

        // If the user enters "1", we handle the creation of a new Jerry.
        if (strcmp(input, "1") == 0) {
            char id[301];
            printf("What is your Jerry's ID ?\n");

            // Read the Jerry's ID from the user.
            if (fgets(id, sizeof(id), stdin) == NULL) {
                // If reading fails, release all resources and exit with an error message.
                destroyAll();
                printf("A memory problem has been detected in the program");
                return 1;
            }
            // Remove trailing newline.
            id[strcspn(id, "\n")] = '\0';

            // Check if a Jerry with this ID already exists in the Hash Table.
            if(lookupInHashTable(g_jerriesHash,id)) {
                printf("Rick did you forgot ? you already left him here !\n");
            }
            else {
                char name[301];
                printf("What planet is your Jerry from ?\n");
                if (fgets(name, sizeof(name), stdin) == NULL) {
                    // If reading fails, destroy resources and exit.
                    destroyAll();
                    printf("A memory problem has been detected in the program");
                    return 1;
                }
                // Remove possible trailing '\r' or '\n'.
                name[strcspn(name, "\r\n")] = '\0';

                // Find the planet by name from a stored planet list or repository.
                Planet* planet = findPlanetByName(name);
                if (!planet) {
                    // If the planet is not known, print a message and continue.
                    printf("%s is not a known planet !\n", name);
                }
                else {
                    // If planet is found, prompt for the Jerry's dimension.
                    char dimension[301]; // מאגר לאחסון המימד (מקסימום 300 תווים + תו סיום)
                    printf("What is your Jerry's dimension ?\n");
                    fgets(dimension, sizeof(dimension), stdin);
                    dimension[strcspn(dimension, "\n")] = '\0';



                    int happiness; // משתנה לאחסון רמת האושר
                    // Prompt the user for the current happiness level.
                    printf("How happy is your Jerry now ?\n");
                    scanf("%d", &happiness);
                    clearInputBuffer();

                    // Create a new Jerry with the provided ID, happiness, and origin.
                    Jerry* j = createJerry(id, happiness, planet, dimension);
                    if (!j) {
                        destroyAll();
                        printf("A memory problem has been detected in the program");
                        return 1;
                    }

                    // Add the new Jerry to the Hash Table and LinkedList.
                    if(addToHashTable(g_jerriesHash, j->ID, j)==failure) {
                        destroyAll();
                        printf("A memory problem has been detected in the program");
                        return 1;
                    }
                    if(appendNode(g_jerriesList,j)==failure) {
                        destroyAll();
                        printf("A memory problem has been detected in the program");
                        return 1;
                    }

                    // Print out the newly created Jerry.
                    if(printJerry(j)==failure) {
                        destroyAll();
                        printf("A memory problem has been detected in the program");
                        return 1;
                    }
                }
            }

        // If the user enters "2", we add a physical characteristic to an existing Jerry.
        } else if (strcmp(input, "2") == 0) {
            char id[301];
            printf("What is your Jerry's ID ?\n");

            // Read the ID from the user.
            if (fgets(id, sizeof(id), stdin) == NULL) {
                destroyAll();
                printf("A memory problem has been detected in the program");
                return 1;
            }
            id[strcspn(id, "\n")] = '\0';

            // Check if the Jerry is in the Hash Table.
            if(!lookupInHashTable(g_jerriesHash,id)) {
                printf("Rick this Jerry is not in the daycare !\n");
            } else {
                char characteristic[301];

                printf("What physical characteristic can you add to Jerry - %s ?\n", id);
                if (fgets(characteristic, sizeof(characteristic), stdin) == NULL) {
                    destroyAll();
                    printf("A memory problem has been detected in the program");
                    return 1;
                }
                characteristic[strcspn(characteristic, "\n")] = '\0';

                // Look up a list of Jerries that share this physical characteristic key.
                LinkedList l = lookupInHashTableProMax(g_physicalHash,characteristic);
                if (!l) {
                    destroyAll();
                    printf("A memory problem has been detected in the program");
                    return 1;
                }
                Jerry* j = searchByKeyInList(l, id, isJerryIDEqualWrapper);

                // If the characteristic already exists for this Jerry, print a message.
                if (j) {
                    printf("The information about his %s already available to the daycare !\n", characteristic);
                } else {
                    float value; // המשתנה לקליטת הערך

                    // Prompt for the new characteristic's value.
                    printf("What is the value of his %s ?\n", characteristic);
                    scanf("%f", &value);
                    clearInputBuffer();

                    // Create a new PhysicalCharacteristics structure.
                    PhysicalCharacteristics* physical = createPhysicalCharacteristics(characteristic,value);
                    if (!physical) {
                        destroyAll();
                        printf("A memory problem has been detected in the program");
                        return 1;
                    }

                    // Retrieve the Jerry instance from the Hash Table and add the physical characteristic.
                    j = lookupInHashTable(g_jerriesHash,id);
                    if (!j) {
                        destroyAll();
                        printf("A memory problem has been detected in the program");
                        return 1;
                    }
                    if(add_physical_to_jerry(j,physical)==failure) {
                        destroyAll();
                        printf("A memory problem has been detected in the program");
                        return 1;
                    }

                    // Update the specialized Hash Table that indexes by physical characteristic.
                    if(addToHashTableProMax(g_physicalHash,characteristic,j)==failure) {
                        destroyAll();
                        printf("A memory problem has been detected in the program");
                        return 1;
                    }

                    // Display all Jerries that share this newly added characteristic.
                    if(displayHashTableProMaxElementsByKey(g_physicalHash,characteristic)==failure) {
                        destroyAll();
                        printf("A memory problem has been detected in the program");
                        return 1;
                    }
                }
            }

        // If the user enters "3", we remove a physical characteristic from a Jerry.
        } else if (strcmp(input, "3") == 0) {
            char id[301];
            printf("What is your Jerry's ID ?\n");

            // Read the ID.
            if (fgets(id, sizeof(id), stdin) == NULL) {
                destroyAll();
                printf("A memory problem has been detected in the program");
                return 1;
            }
            id[strcspn(id, "\n")] = '\0';

            // Check if Jerry is in the daycare (Hash Table).
            if(!lookupInHashTable(g_jerriesHash,id)) {
                printf("Rick this Jerry is not in the daycare !\n");
            } else {
                char characteristic[301];
                printf("What physical characteristic do you want to remove from Jerry - %s ?\n", id);

                // Read the characteristic to remove.
                if (fgets(characteristic, sizeof(characteristic), stdin) == NULL) {
                    destroyAll();
                    printf("A memory problem has been detected in the program");
                    return 1;
                }
                characteristic[strcspn(characteristic, "\n")] = '\0';

                // Lookup the list of Jerries for that characteristic.
                LinkedList l = lookupInHashTableProMax(g_physicalHash,characteristic);
                if (!l) {
                    // If there is no list for that characteristic, we assume Jerry is not in the daycare or unknown characteristic.
                    printf("Rick this Jerry is not in the daycare !\n");
                }
                else {
                    // Search for the specific Jerry in the list.
                    Jerry* j = searchByKeyInList(l, id, isJerryIDEqualWrapper);
                    if (j) {
                        // If found, remove this characteristic from the Jerry and update the list.
                        delete_physical_from_jerry(j,characteristic);
                        deleteNode(l,j);
                        printJerry(j);
                    } else {
                        printf("The information about his %s not available to the daycare !\n", characteristic);
                    }
                }
            }

        // If the user enters "4", we remove a Jerry entirely from the daycare.
        } else if (strcmp(input, "4") == 0) {
            char id[301];
            printf("What is your Jerry's ID ?\n");

            // Read ID.
            if (fgets(id, sizeof(id), stdin) == NULL) {
                destroyAll();
                printf("A memory problem has been detected in the program");
                return 1;
            }
            id[strcspn(id, "\n")] = '\0';

            // Look for the Jerry in the Hash Table.
            Jerry *j = lookupInHashTable(g_jerriesHash, id);
            if(!j) {
                printf("Rick this Jerry is not in the daycare !\n");
            }
            else {
                // If found, delete Jerry completely.
                delete_jerry(j);
                printf("Rick thank you for using our daycare service ! Your Jerry awaits !\n");
            }

        // If the user enters "5", we try to match a Jerry based on a remembered characteristic and its approximate value.
        } else if (strcmp(input, "5") == 0) {
            char characteristic[301];
            printf("What do you remember about your Jerry ?\n");
            if (fgets(characteristic, sizeof(characteristic), stdin) == NULL) {
                destroyAll();
                printf("A memory problem has been detected in the program");
                return 1;
            }
            characteristic[strcspn(characteristic, "\n")] = '\0';

            // Lookup all Jerries that have this characteristic.
            LinkedList l = lookupInHashTableProMax(g_physicalHash,characteristic);
            if (!l) {
                printf("Rick we can not help you - we do not know any Jerry's %s !\n", characteristic);;
            } else {
                float value;
                printf("What do you remember about the value of his %s ?\n", characteristic);
                scanf("%f", &value);
                clearInputBuffer();

                // Find the Jerry whose characteristic value is closest to the given value (minimum absolute difference).
                Jerry *j = min_abs(l,characteristic, value);
                if(j) {
                    printf("Rick this is the most suitable Jerry we found :\n");
                    printJerry(j);
                    // Remove the Jerry from the daycare after matching.
                    delete_jerry(j);
                    printf("Rick thank you for using our daycare service ! Your Jerry awaits !\n");
                } else {
                    destroyAll();
                    printf("A memory problem has been detected in the program");
                    return 1;
                }
            }

        // If the user enters "6", we find the "saddest" Jerry (lowest happiness) and remove it from the daycare.
        } else if (strcmp(input, "6") == 0) {
            int saddest = 101;
            // If there are no Jerries in the daycare, we cannot proceed.
            if (!getFirstElement(g_jerriesList)) {
                printf("Rick we can not help you - we currently have no Jerries in the daycare !\n");
            }
            else {
                // Traverse the list to find the Jerry with the lowest happiness.
                Jerry* current = getFirstElement(g_jerriesList);
                Jerry* j = current;
                while (current) {
                    if (current->happines < saddest) {
                        j = current;
                        saddest = current->happines;
                    }
                    current = getNextElement(g_jerriesList, current);
                }
                if(j) {
                    printf("Rick this is the most suitable Jerry we found :\n");
                    printJerry(j);
                    delete_jerry(j);
                    printf("Rick thank you for using our daycare service ! Your Jerry awaits !\n");
                } else {
                    destroyAll();
                    printf("A memory problem has been detected in the program");
                    return 1;
                }
            }

        // If the user enters "7", we display data about Jerries or planets, depending on user choice.
        } else if (strcmp(input, "7") == 0) {
            char userInput[301]; // משתנה לאחסון הקלט (300 תווים + '\0')

            // Prompt user for the type of information to display.
            printf("What information do you want to know ?\n");
            printf("1 : All Jerries\n");
            printf("2 : All Jerries by physical characteristics\n");
            printf("3 : All known planets\n");

            // Read user input.
            if (fgets(userInput, sizeof(userInput), stdin) == NULL) {
                destroyAll();
                printf("A memory problem has been detected in the program");
                return 1;
            }

            // Remove trailing newline.
            userInput[strcspn(userInput, "\n")] = '\0';

            // Option "1": Display all Jerries in the daycare.
            if (strcmp(userInput, "1") == 0) {
                if (getLength(g_jerriesList) == 0) {
                    printf("Rick we can not help you - we currently have no Jerries in the daycare !\n");
                }
                if(displayList(g_jerriesList)==failure) {
                    destroyAll();
                    printf("A memory problem has been detected in the program");
                    return 1;
                }

            // Option "2": Display all Jerries filtered by a given physical characteristic.
            } else if (strcmp(userInput, "2") == 0) {
                char characteristic[301];
                printf("What physical characteristics ?\n");
                if (fgets(characteristic, sizeof(characteristic), stdin) == NULL) {
                    destroyAll();
                    printf("A memory problem has been detected in the program");
                    return 1;
                }
                characteristic[strcspn(characteristic, "\n")] = '\0';

                // Check if we have any Jerries with this characteristic.
                if (!lookupInHashTableProMax(g_physicalHash, characteristic)) {
                    printf("Rick we can not help you - we do not know any Jerry's %s !\n", characteristic);
                } else {
                    if(displayHashTableProMaxElementsByKey(g_physicalHash, characteristic)==failure) {
                        destroyAll();
                        printf("A memory problem has been detected in the program");
                        return 1;
                    }
                }

            // Option "3": Display all known planets.
            } else if (strcmp(userInput, "3") == 0) {
                if(displayList(g_planetsList)==failure) {
                    destroyAll();
                    printf("A memory problem has been detected in the program");
                    return 1;
                }

            // Otherwise, the option is unknown.
            } else {
                printf("Rick this option is not known to the daycare !\n");
            }

        // If the user enters "8", we perform an activity that modifies the happiness of all Jerries.
        } else if (strcmp(input, "8") == 0) {
            // If we have no Jerries, we cannot run an activity.
            if (getLength(g_jerriesList) == 0) {
                printf("Rick we can not help you - we currently have no Jerries in the daycare !\n");
            } else {
                char userInput[301]; // משתנה לאחסון הקלט (300 תווים + '\0')

                // Prompt the user to choose an activity.
                printf("What activity do you want the Jerries to partake in ?\n");
                printf("1 : Interact with fake Beth\n");
                printf("2 : Play golf\n");
                printf("3 : Adjust the picture settings on the TV\n");

                if (fgets(userInput, sizeof(userInput), stdin) == NULL) {
                    destroyAll();
                    printf("A memory problem has been detected in the program here");
                    return 1;
                }
                userInput[strcspn(userInput, "\n")] = '\0';

                // Activity "1": Interact with fake Beth.
                if (strcmp(userInput, "1") == 0) {
                    Jerry* current = getFirstElement(g_jerriesList);
                    // For each Jerry:
                    while (current) {
                        Jerry* j = current;
                        if (j) {
                            // If happiness < 20, reduce it by 5 (but not below 0).
                            // Otherwise, increase it by 15 (but not above 100).
                            if (j->happines < 20) {
                                j->happines = max(j->happines-5,0);
                            } else {
                                j->happines = min(j->happines+15,100);
                            }
                        }
                        current = getNextElement(g_jerriesList, current);
                    }
                    printf("The activity is now over !\n");
                    if(displayList(g_jerriesList)==failure) {
                        destroyAll();
                        printf("A memory problem has been detected in the program");
                        return 1;
                    }

                // Activity "2": Play golf.
                } else if (strcmp(userInput, "2") == 0) {
                    Jerry* current = getFirstElement(g_jerriesList);
                    while (current) {
                        Jerry* j = current;
                        if (j) {
                            // If happiness < 50, reduce by 10. Otherwise, increase by 10.
                            if (j->happines < 50) {
                                j->happines = max(j->happines-10,0);
                            } else {
                                j->happines = min(j->happines+10,100);
                            }
                        }
                        current = getNextElement(g_jerriesList, current);
                    }
                    printf("The activity is now over !\n");
                    if(displayList(g_jerriesList)==failure) {
                        destroyAll();
                        printf("A memory problem has been detected in the program");
                        return 1;
                    }

                // Activity "3": Adjust the TV's picture settings.
                } else if (strcmp(userInput, "3") == 0) {
                    Jerry* current = getFirstElement(g_jerriesList);
                    while (current) {
                        Jerry* j = current;
                        if (j) {
                            // Increase happiness by 20, up to a maximum of 100.
                            j->happines = min(j->happines+20,100);
                        }
                        current = getNextElement(g_jerriesList, current);
                    }
                    printf("The activity is now over !\n");
                    if(displayList(g_jerriesList)==failure) {
                        destroyAll();
                        printf("A memory problem has been detected in the program");
                        return 1;
                    }

                // If the option is not recognized, print an error message.
                } else {
                    printf("Rick this option is not known to the daycare !\n");
                }
            }

        // If the user enters "9", we close the daycare, clean up, and exit the loop.
        } else if (strcmp(input, "9") == 0) {
            printf("The daycare is now clean and close !\n");
            destroyAll();
            done = true;

        // If the user enters an unknown command, notify them.
        } else {
            printf("Rick this option is not known to the daycare !\n");
        }
    }
}
