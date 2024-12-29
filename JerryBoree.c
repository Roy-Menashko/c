/***************************************************
 * File: ManageJerries.c (a.k.a "JerryBoree")
 *
 * קורא קובץ קונפיגורציה ומבצע:
 *   1) בניית LinkedList של Planet*
 *   2) בניית LinkedList של Jerry*
 *   3) בניית HashTable< string(ID) -> Jerry* >
 *   4) בניית MultiValueHashTable< string(PhysicalName) -> LinkedList<Jerry*> >
 *
 ***************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "Defs.h"
#include "Jerry.h"               /* עבור פונקציות יצירה/הרס של Jerry, Planet וכו' */
#include "LinkedList.h"
#include "HashTable.h"
#include "MultiValueHashTable.h"

/* ---------- משתנים גלובליים (סטטיים בקובץ) ---------- */
static LinkedList g_planetsList = NULL;       /* LinkedList של Planet* */
static LinkedList g_jerriesList = NULL;       /* LinkedList של Jerry* */
static hashTable g_jerriesHash = NULL;        /* key=ID(string), value=Jerry* */
static hashTableProMax g_physicalHash = NULL; /* key=PhysicalName(string), value=LinkedList<Jerry*> */

/* ---------- מימוש Getter-ים גלובליים ---------- */
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

status printJerryWrapper(Element e)
{
    if (!e) return failure;
    /* המרת ה־void* ל־Jerry* */
    Jerry* j = (Jerry*)e;
    return printJerry(j);
}


/* השוואת מחרוזות (מפתח לטבלאות גיבוב) */
static bool compareStrings(Element e1, Element e2)
{
    if (!e1 || !e2) return false;
    return (strcmp((char*)e1, (char*)e2) == 0);
}

/* המרת מחרוזת לערך מספרי (פונקציית טרנספורמציה לטבלת גיבוב) */
static int transformStringToNumber(Element key)
{
    if (!key) return 0;
    int sum = 0;
    const char* str = (const char*)key;
    while (*str)
        sum += (unsigned char)(*str++);
    return sum;
}

static Element copyString(Element e) {
    if (!e) return NULL;
    char* copy = strdup((char*)e);
    return copy;
}

/* השוואת Planets לפי שם */
static bool comparePlanets(Element e1, Element e2)
{
    Planet* p1 = (Planet*)e1;
    Planet* p2 = (Planet*)e2;
    return (strcmp(p1->name, p2->name) == 0);
}

/* השוואת Jerries לפי ID */
static bool compareJerries(Element e1, Element e2)
{
    Jerry* j1 = (Jerry*)e1;
    Jerry* j2 = (Jerry*)e2;
    return (strcmp(j1->ID, j2->ID) == 0);
}

/* השמדת מחרוזת שהוקצתה דינאמית
static status freeStringPtr(Element e)
{
    if (!e) return failure;
    free(e);
    return success;
}*/

/* פונקציית הדפסה למחרוזת */
static status printStringPtr(Element e)
{
    if (!e) return failure;
    printf("%s", (char*)e);
    return success;
}

/* copyShallow – החזרת המצביע כפי שהוא (עבור מפתח/ערך בטבלאות) */
static Element copyShallow(Element e)
{
    return e;
}

/* פונקציה "ריקה" להשמדה (נשתמש בה כאשר לא נרצה להרוס ערך) */
static status freeNoOp(Element e)
{
    (void)e;
    return success;
}

/* הדפסת זיהוי Jerry בלבד (נשתמש עבור ערך ב-hash של Jerry*).
static status printJerryIDOnly(Element e)
{
    if (!e) return failure;
    Jerry* j = (Jerry*)e;
    printf("JerryID=%s", j->ID);
    return success;
}*/

float get_abs(float x,float y) {
    if (x > y){
        return x-y;
    }
    return y - x;
}

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

bool hasPlanetName(Element element, Element dataKey) {
    if (!element || !dataKey) return false;
    Planet* planet = (Planet*)element;
    char* name = (char*)dataKey;
    return strcmp(planet->name, name) == 0;
}

bool isJerryIDEqual(const Jerry* jerry, const char* id) {
    if (jerry == NULL || id == NULL) {
        return false;
    }
    return strcmp(jerry->ID, id) == 0;
}

bool isJerryIDEqualWrapper(Element e1, Element e2) {
    Jerry* j = (Jerry*)e1;
    char* id = (char*)e2;
    return isJerryIDEqual(j, id);
}

Planet* findPlanetByName(char* name) {
    if (g_planetsList == NULL || name == NULL) {
        return NULL;
    }

    Element result = searchByKeyInList(g_planetsList, name, hasPlanetName);
    return (Planet*)result;
}

/* פונקציית השמדה ל-Planet (שימוש בפונקציה מה- Jerry.h) */
static status freePlanetPtr(Element e)
{
    if (!e) return failure;
    destroyPlanet((Planet*)e);
    return success;
}

/* פונקציית הדפסה ל-Planet (שימוש בפונקציה מה- Jerry.h) */
static status printPlanetPtr(Element e)
{
    if (!e) return failure;
    printPlanet((Planet*)e);
    return success;
}

/* פונקציית השוואה שמשתמשת במבנה Planet מקובץ Jerry.h (נדרש ל-LinkedList) */
static bool comparePlanetByName(Element e1, Element e2)
{
    return comparePlanets(e1, e2);
}

/* השמדת Jerry (מתוך Jerry.h) */
static status freeJerryPtr(Element e)
{
    if (!e) return failure;
    destoyJerry((Jerry*)e);  // <-- Make sure this matches your Jerry.c function name
    return success;
}

/* הדפסת Jerry מלאה (מתוך Jerry.h) */
static status printJerryPtr(Element e)
{
    if (!e) return failure;
    printJerry((Jerry*)e);
    return success;
}

/* הדפסת רשימת Jerry-ים (נדרש עבור ה-value ב-HashTableProMax)
static status printListOfJerries(Element e)
{
    if (!e) return failure;
     כאן אנחנו מניחים ש-e הוא LinkedList של Jerry*,
       ולכן קוראים לפונקציית displayList מה-LinkedList.
    displayList((LinkedList)e);
    return success;
}*/
static status freeStringPtr(Element e) {
    if (!e) return failure;
    free(e);
    return success;
}

bool isPrime(int n) {
    if (n <= 1) {
        return false;
    }
    if (n <= 3) {
        return true;  // 2 ו-3 הם ראשוניים
    }
    // אם מתחלק ב-2 או ב-3 => לא ראשוני
    if (n % 2 == 0 || n % 3 == 0) {
        return false;
    }

    // בדיקה בכל הצעדים של 6k ± 1, עד לשורש של n
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }
    return true;
}

/* פונקציה שמקבלת מספר ומוצאת מספר ראשוני גדול ממנו */
int nextPrime(int n) {
    // נתחיל מהמספר הבא
    n++;
    // נמשיך להעלות את n עד שיהיה ראשוני
    while (!isPrime(n)) {
        n++;
    }
    return n;
}

status delete_jerry(Jerry* j) {
    if (!j) {
        return failure;
    }

    // הסרת הג'רי מטבלת הג'רים לפי ID
    if (removeFromHashTable(g_jerriesHash, j->ID) == failure) {
        printf("Failed to remove Jerry with ID %s from g_jerriesHash.\n", j->ID);
        // ניתן להחליט להמשיך או להחזיר כשלון
    }

    // הסרת הג'רי מכל הרשימות הפיזיות שלו
    for (int i = 0; i < j->num_of_pyhshical; i++) {
        char* physName = j->his_physical[i]->name;
        if (removeFromHashTableProMax(g_physicalHash, physName, j) == failure) {
            printf("Failed to remove Jerry with ID %s from physical characteristic '%s'.\n", j->ID, physName);
            // ניתן להחליט להמשיך או להחזיר כשלון
        }
    }

    // הסרת הג'רי מהרשימה הראשית
    if (deleteNode(g_jerriesList, j) == failure) {
        printf("Failed to remove Jerry with ID %s from g_jerriesList.\n", j->ID);
        return failure;
    }

    return success;
}

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
 * Fix: in C, `fgets` requires a `FILE*`, not a `const char*`.
 * So we must open the file using `fopen`.
 */
status readConfigAndBuild(const char* fileName)
{
    if (!fileName)
        return failure;

    FILE* file = fopen(fileName, "r");
    if (!file) {
        perror("Error opening file");
        return failure;
    }

    /* אתחול מבני הנתונים: רשימות מקושרות */
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

    /* משתנים לזיהוי שלב הקריאה */
    bool readingPlanets = false;
    bool readingJerries = false;

    char line[301];
    Jerry* currentJerry = NULL;

    /* ספירת כמות ה-Jerry והמאפיינים הפיזיים לבחירת גודל טבלה (אם רוצים דינאמי) */
    int jerryCount = 0;
    int physicalCount = 0;

    /* ------------ מעבר ראשון בקובץ: בניית הרשימות (כוכבים וג'רים) ----------- */
    while (fgets(line, sizeof(line), file))
    {
        /* הסרה של \n או \r\n בסוף השורה */
        line[strcspn(line, "\r\n")] = '\0';

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

        /* --- שלב כוכבי לכת --- */
        if (readingPlanets)
        {
            char planetName[300];
            float x=0, y=0, z=0;
            if (sscanf(line, "%299[^,],%f,%f,%f", planetName, &x, &y, &z) == 4) {
                Planet* p = createPlanet(planetName, x, y, z);
                if (!p) {
                    fclose(file);
                    return failure;
                }

                if (appendNode(g_planetsList, p) == failure) {
                    destroyPlanet(p);
                    fclose(file);
                    return failure;
                }
            }
        }
        /* --- שלב ג'רי (Jerry) --- */
        else if (readingJerries)
        {
            /* אם השורה מתחילה בטאב => נתון פיזי */
            if (line[0] == '\t') {
                char propName[300];
                float propValue=0;
                /* פורמט: "\tName:Value" */
                if (sscanf(line+1, "%299[^:]:%f", propName, &propValue) == 2) {
                    PhysicalCharacteristics* pc = createPhysicalCharacteristics(propName, propValue);
                    if (!pc) {
                        fclose(file);
                        return failure;
                    }

                    if (add_physical_to_jerry(currentJerry, pc) == failure) {
                        destroyPhysicalCharacteristics(pc);
                        fclose(file);
                        return failure;
                    }
                    physicalCount++;
                }
            }
            else {
                /* פורמט ג'רי חדש: "id,dimension,planet,happiness" */
                char id[300], dimension[300], planetName[300];
                int happiness=0;
                if (sscanf(line, "%299[^,],%299[^,],%299[^,],%d",
                           id, dimension, planetName, &happiness) == 4)
                {
                    /* איתור הכוכב ברשימת הכוכבים */
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
                        fclose(file);
                        return failure; /* שגיאה אם כוכב לא נמצא */
                    }

                    /* יוצרים Origin ואז Jerry */
                    Origin* orig = createOrigin(pFound, dimension);
                    if (!orig) {
                        fclose(file);
                        return failure;
                    }

                    Jerry* j = createJerry(id, happiness, orig);
                    if (!j) {
                        destroyOrigin(orig);
                        fclose(file);
                        return failure;
                    }

                    /* מוסיפים לרשימת הג'רים */
                    if (appendNode(g_jerriesList, j) == failure) {
                        destoyJerry(j);
                        fclose(file);
                        return failure;
                    }
                    currentJerry = j;
                    jerryCount++;
                }
            }
        }
    }

    /* Done reading file; close it. */
    fclose(file);

    /* ---------------- בניית טבלאות גיבוב ---------------- */

    /* טבלת ג'רים לפי ID */
    /* לדוגמה נקבע גודל קבוע או דינאמי. כאן ניקח סתם גודל 31. */
    int l = getLength(g_jerriesList);
    g_jerriesHash = createHashTable(
        copyString,       /* העתקת המפתח (string) – העתקה שטחית */
        freeStringPtr,     /* השמדת מפתח (string) */
        printStringPtr,    /* הדפסת מפתח (string) */
        copyShallow,       /* העתקה שטחית ל-Jerry* */
        freeNoOp,          /* לא משמידים את Jerry* (כבר ינוהל ע"י הרשימה) */
        printJerryWrapper,  /* הדפסת ID של Jerry בלבד */
        compareStrings,    /* השוואת מפתחות */
        transformStringToNumber,  /* פונקציית טרנספורמציה */
        nextPrime(l)                 /* גודל הטבלה */
    );
    if (!g_jerriesHash) return failure;

    /* הכנסה של כל Jerry מהרשימה לטבלת הגיבוב */
    Element node = getFirstElement(g_jerriesList);
    while (node) {
        Jerry* j = (Jerry*)node;
        addToHashTable(g_jerriesHash, j->ID, j);
        node = getNextElement(g_jerriesList, node);
    }

    /* טבלת מאפיינים פיזיים (מפתח: שם פיזי, ערך: רשימת ג'רים) */
    g_physicalHash = createHashTableProMax(
        copyString,    /* copyKey   */
        freeStringPtr,  /* freeKey   */
        printStringPtr, /* printKey  */
        copyShallow,    /* copyValue (שומר LinkedList) */
        freeNoOp,       /* freeValue (לא נשמיד, כי הג'רי עצמו מנוהל בחוץ) */
        printJerryWrapper, /* printValue = מציג את הרשימה */
        compareStrings, /* השוואת מפתח (string) */
        compareStrings, /* השוואת ערכים (לא באמת בשימוש כאן) */
        transformStringToNumber,
        nextPrime(l)              /* גודל הטבלה, אפשר גם להתאים */
    );
    if (!g_physicalHash) return failure;

    /* לכל ג'רי, לכל מאפיין פיזי => מוסיפים ל-HashTableProMax */
    node = getFirstElement(g_jerriesList);
    while (node) {
        Jerry* j = (Jerry*)node;
        for (int i = 0; i < j->num_of_pyhshical; i++) {
            /* Make sure this matches the correct field name in your Jerry struct
               (was previously 'num_of_pyhshical' in your snippet) */
            char* physName = j->his_physical[i]->name;
            addToHashTableProMax(g_physicalHash, physName, j);
        }
        node = getNextElement(g_jerriesList, node);
    }

    return success;
}

void runTest(const char* configFile)
{
    /* 1) Build data from the config file */
    status result = readConfigAndBuild(configFile);
    if (result == failure) {
        fprintf(stderr, "Failed to read and build configuration from '%s'.\n", configFile);
        return;
    }

    printf("Successfully built data from file: %s\n", configFile);

    /* 2) Optionally, do some printing or checks here */
    /* e.g., show how many planets or jerries we have, etc.
       If you have utility functions to print them:

       printf("Planets:\n");
       displayList(g_planetsList);   // Hypothetical function

       printf("Jerries:\n");
       displayList(g_jerriesList);

       // Or if you have a function to display all keys/values in a hash table...
       displayHashTable(g_jerriesHash);
       displayHashTableProMax(g_physicalHash);
    */

    /* 3) Destroy everything to free memory */

}
void printMenu() {
    printf("Welcome Rick, what are your Jerry's needs todayyyyy?\n");
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
int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <configFile>\n", argv[0]);
        return 1;
    }

    /* We pass argv[1] as the config file name. */
    runTest(argv[2]);
    bool done = false; // Flag to control the main program loop
    while (!done) {
        printMenu(); // Print the main menu for user options

        char input[301]; // Buffer to store user input

        // Read user input and remove the newline character
        if (fgets(input, sizeof(input), stdin) != NULL) {
            input[strcspn(input, "\n")] = '\0';
        }
        if (strcmp(input, "1") == 0) {
            char id[301];
            printf("What is your Jerry's ID ?\n");

            if (fgets(id, sizeof(id), stdin) == NULL) {
                printf("Error reading input.\n");
                return 1;
            }
            id[strcspn(id, "\n")] = '\0';
            if(lookupInHashTable(g_jerriesHash,id)) {
                printf("Rick did you forgot ? you already left him here !\n");
            }
            else {
                char name[301];
                printf("What planet is your Jerry from ?\n");
                if (fgets(name, sizeof(name), stdin) == NULL) {
                    printf("Error reading input.\n");
                    return 1;
                }
                name[strcspn(name, "\r\n")] = '\0';
                Planet* planet = findPlanetByName(name);
                if (!planet) {
                    printf("%s is not a known planet!\n", name);
                }
                else {
                    char dimension[301]; // מאגר לאחסון המימד (מקסימום 300 תווים + תו סיום)
                    printf("What is your Jerry's dimension?\n");
                    fgets(dimension, sizeof(dimension), stdin);
                    dimension[strcspn(dimension, "\n")] = '\0';
                    Origin* origin = createOrigin(planet,dimension);
                    if (!origin) {
                        printf("Failed to create origin.\n");
                        return 1;
                    }
                    int happiness; // משתנה לאחסון רמת האושר

                    // הדפסת ההודעה
                    printf("How happy is your Jerry now?\n");
                    scanf("%d", &happiness);
                    clearInputBuffer();
                    Jerry* j = createJerry(id,happiness,origin);
                    if (!j) {
                        printf("Failed to create jerry.\n");
                        return 1;
                    }
                    addToHashTable(g_jerriesHash, j->ID, j);
                    appendNode(g_jerriesList,j);
                    printJerry(j);

                }
            }
        } else if (strcmp(input, "2") == 0) {
            char id[301];
            printf("What is your Jerry's ID ?\n");

            if (fgets(id, sizeof(id), stdin) == NULL) {
                printf("Error reading input.\n");
                return 1;
            }
            id[strcspn(id, "\n")] = '\0';
            if(!lookupInHashTable(g_jerriesHash,id)) {
                printf("Rick this Jerry is not in the daycare !\n");
            } else {
                char characteristic[301];

                printf("What physical characteristic can you add to Jerry - %s?\n", id);
                if (fgets(characteristic, sizeof(characteristic), stdin) == NULL) {
                    printf("Error reading input.\n");
                    return 1;
                }
                characteristic[strcspn(characteristic, "\n")] = '\0';
                LinkedList l = lookupInHashTableProMax(g_physicalHash,characteristic);
                Jerry* j = searchByKeyInList(l, id, isJerryIDEqualWrapper);
                if (j) {
                    printf("The information about his %s already available to the daycare!\n", characteristic);
                } else {
                    float value; // המשתנה לקליטת הערך

                    // הדפסת ההודעה עם השם של ה-info
                    printf("What is the value of his %s?\n", characteristic);

                    // קריאה לקלט מהמשתמש
                    printf("Enter the value: ");
                    scanf("%f", &value);
                    clearInputBuffer();
                    PhysicalCharacteristics* physical = createPhysicalCharacteristics(characteristic,value);
                    if (!physical) {
                        printf("Failed to create physical characteristics.\n");
                        return 1;
                    }
                    j = lookupInHashTable(g_jerriesHash,id);
                    add_physical_to_jerry(j,physical);
                    addToHashTableProMax(g_physicalHash,characteristic,j);
                    displayList(lookupInHashTableProMax(g_physicalHash,characteristic));
                }

            }


        } else if (strcmp(input, "3") == 0) {
            char id[301];
            printf("What is your Jerry's ID ?\n");

            if (fgets(id, sizeof(id), stdin) == NULL) {
                printf("Error reading input.\n");
                return 1;
            }
            id[strcspn(id, "\n")] = '\0';
            if(!lookupInHashTable(g_jerriesHash,id)) {
                printf("Rick this Jerry is not in the daycare !\n");
            } else {
                char characteristic[301];
                printf("What physical characteristic do you want to remove from Jerry - %s?\n", id);
                if (fgets(characteristic, sizeof(characteristic), stdin) == NULL) {
                    printf("Error reading input.\n");
                    return 1;
                }
                characteristic[strcspn(characteristic, "\n")] = '\0';
                LinkedList l = lookupInHashTableProMax(g_physicalHash,characteristic);
                if (!l) {
                    printf("Rick this Jerry is not in the daycare !\n");
                }
                else {
                    Jerry* j = searchByKeyInList(l, id, isJerryIDEqualWrapper);
                    if (j) {
                        delete_physical_from_jerry(j,characteristic);
                        deleteNode(l,j);
                        printJerry(j);
                    } else {
                        printf("The information about his %s not available to the daycare!\n", characteristic);
                    }
                }
            }


        } else if (strcmp(input, "4") == 0) {
            char id[301];
            printf("What is your Jerry's ID?\n");

            if (fgets(id, sizeof(id), stdin) == NULL) {
                printf("Error reading input.\n");
                return 1;
            }
            id[strcspn(id, "\n")] = '\0';

            Jerry *j = lookupInHashTable(g_jerriesHash, id);
            if(!j) {
                printf("Rick, this Jerry is not in the daycare!\n");
            }
            else {
                printJerry(j);
                delete_jerry(j);
                printf("Rick, thank you for using our daycare service! Your Jerry awaits!\n");
            }

        } else if (strcmp(input, "5") == 0) {
            char characteristic[301];
            printf("What do you remember about your Jerry ?\n");
            if (fgets(characteristic, sizeof(characteristic), stdin) == NULL) {
                printf("Error reading input.\n");
                return 1;
            }
            characteristic[strcspn(characteristic, "\n")] = '\0';
            LinkedList l = lookupInHashTableProMax(g_physicalHash,characteristic);
            if (!l) {
                printf("Rick this Jerry is not in the daycare !\n");
            } else {
                float value;
                printf("What do you remember about the value of his %s?\n", characteristic);
                scanf("%f", &value);
                clearInputBuffer();
                Jerry *j = min_abs(l,characteristic, value);
                if(j) {
                    printJerry(j);
                    delete_jerry(j);
                    printf("Rick, thank you for using our daycare service! Your Jerry awaits!\n");
                } else {
                    return 1;
                }
            }




        } else if (strcmp(input, "6") == 0) {
            printf("Giving you the saddest Jerry...\n");
        } else if (strcmp(input, "7") == 0) {
            printf("Showing you what we've got...\n");
        } else if (strcmp(input, "8") == 0) {
            printf("Letting the Jerries play...\n");
        } else if (strcmp(input, "9") == 0) {
            printf("Closing this place. Goodbye!\n");
            destroyAll();
            done = true;
        } else {
            printf("Invalid option\n");
        }
    }
}