#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "Jerry.h"

int planets_count = 0;
int jerry_count = 0;
int origin_count = 0;
int phyiscal_count = 0;
Origin** all_origins = NULL;
PhysicalCharacteristics** all_physical_characteristics = NULL;
Planet** all_planets = NULL;
Jerry** all_jerry = NULL;

status addJerryToManager(Jerry*** all_jerry, int* jerry_count, Jerry* new_jerry) {
    // Check if the new Jerry is NULL
    if (new_jerry == NULL) {
        return failure; // Return failure if the new Jerry is invalid
    }

    // Increment the count of Jerries (preparing to add a new one)
    (*jerry_count)++;

    // Reallocate memory for the array to hold one more Jerry pointer
    Jerry** temp = realloc(*all_jerry, (*jerry_count) * sizeof(Jerry*));
    if (!temp) { // Check if reallocation failed
        printf("Memory Problem\n");

        // Free the memory allocated for the new Jerry, as it was not added
        free(new_jerry->ID);
        free(new_jerry);
        return failure; // Return failure if memory allocation failed
    }

    // Assign the reallocated memory back to the original array pointer
    *all_jerry = temp;

    // Add the new Jerry pointer to the end of the array
    (*all_jerry)[(*jerry_count) - 1] = new_jerry;

    return success; // Return success after successfully adding the new Jerry
}

status addPlanetToManager(Planet*** all_planets, int* planets_count, Planet* new_planet) {
    // Check if the new Planet is NULL
    if (new_planet == NULL) {
        return failure; // Return failure if the new Planet is invalid
    }

    // Increment the count of Planets (preparing to add a new one)
    (*planets_count)++;

    // Reallocate memory for the array to hold one more Planet pointer
    Planet** temp = realloc(*all_planets, (*planets_count) * sizeof(Planet*));
    if (!temp) { // Check if reallocation failed
        printf("Memory Problem\n");

        // Free the memory allocated for the new Planet, as it was not added
        free(new_planet->name);
        free(new_planet);
        return failure; // Return failure if memory allocation failed
    }

    // Assign the reallocated memory back to the original array pointer
    *all_planets = temp;

    // Add the new Planet pointer to the end of the array
    (*all_planets)[(*planets_count) - 1] = new_planet;

    return success; // Return success after successfully adding the new Planet
}

status addPhysicalToManager(PhysicalCharacteristics*** all_physical, int* physical_count, PhysicalCharacteristics* new_physical) {
    // Check if the new PhysicalCharacteristic is NULL
    if (new_physical == NULL) {
        return failure; // Return failure if the new PhysicalCharacteristic is invalid
    }

    // Increment the count of PhysicalCharacteristics (preparing to add a new one)
    (*physical_count)++;

    // Reallocate memory for the array to hold one more PhysicalCharacteristics pointer
    PhysicalCharacteristics** temp = realloc(*all_physical, (*physical_count) * sizeof(PhysicalCharacteristics*));
    if (!temp) { // Check if reallocation failed
        printf("Memory Problem\n");

        // Free the memory allocated for the new PhysicalCharacteristic, as it was not added
        free(new_physical->name);
        free(new_physical);
        return failure; // Return failure if memory allocation failed
    }

    // Assign the reallocated memory back to the original array pointer
    *all_physical = temp;

    // Add the new PhysicalCharacteristic pointer to the end of the array
    (*all_physical)[(*physical_count) - 1] = new_physical;

    return success; // Return success after successfully adding the new PhysicalCharacteristic
}

status addOriginToManager(Origin*** all_origins, int* origin_count, Origin* new_origin) {
    // Check if the new Origin is NULL
    if (!new_origin) {
        return failure; // Return failure if the new Origin is invalid
    }

    // Increment the count of Origins (preparing to add a new one)
    (*origin_count)++;

    // Reallocate memory for the array to hold one more Origin pointer
    Origin** temp = realloc(*all_origins, (*origin_count) * sizeof(Origin*));
    if (!temp) { // Check if reallocation failed
        printf("Memory Problem\n");

        // Free the memory allocated for the new Origin, as it was not added
        free(new_origin->meimad);
        free(new_origin);
        return failure; // Return failure if memory allocation failed
    }

    // Assign the reallocated memory back to the original array pointer
    *all_origins = temp;

    // Add the new Origin pointer to the end of the array
    (*all_origins)[(*origin_count) - 1] = new_origin;

    return success; // Return success after successfully adding the new Origin
}

bool is_planet_exists(Planet** all_planets, int planets_count, char *planet_name) {
    // Check if the input planet name or planets array is NULL
    if (!planet_name || !all_planets) {
        return false; // Return false if inputs are invalid
    }

    // Iterate through the array of planets
    for (int i = 0; i < planets_count; i++) {
        // Compare the name of the current planet with the input name
        if (strcmp(all_planets[i]->name, planet_name) == 0) {
            return true; // Return true if a match is found
        }
    }

    // Return false if no matching planet name was found
    return false;
}

bool is_id_exists(Jerry** all_jerry, int jerry_count, char *id) {
    // Check if the input ID or Jerries array is NULL
    if (!id || !all_jerry) {
        return false; // Return false if inputs are invalid
    }

    // Iterate through the array of Jerries
    for (int i = 0; i < jerry_count; i++) {
        // Compare the ID of the current Jerry with the input ID
        if (strcmp(all_jerry[i]->ID, id) == 0) {
            return true; // Return true if a match is found
        }
    }

    // Return false if no matching ID was found
    return false;
}

Jerry* get_jerry(Jerry** all_jerry, int jerry_count, char *id) {
    // Check if the input ID or Jerries array is NULL
    if (!id || !all_jerry) {
        return NULL; // Return NULL if inputs are invalid
    }

    // Iterate through the array of Jerries
    for (int i = 0; i < jerry_count; i++) {
        // Compare the ID of the current Jerry with the input ID
        if (strcmp(all_jerry[i]->ID, id) == 0) {
            return all_jerry[i]; // Return the pointer to the matching Jerry
        }
    }

    // Return NULL if no matching Jerry was found
    return NULL;
}

Planet* get_planet(Planet** all_planets, int planets_count, char *name) {
    // Check if the input name or planets array is NULL
    if (!name || !all_planets) {
        return NULL; // Return NULL if inputs are invalid
    }

    // Iterate through the array of planets
    for (int i = 0; i < planets_count; i++) {
        // Compare the name of the current planet with the input name
        if (strcmp(all_planets[i]->name, name) == 0) {
            return all_planets[i]; // Return the pointer to the matching Planet
        }
    }

    // Return NULL if no matching planet name was found
    return NULL;
}

bool is_origin_exits(Origin** all_origins, int origin_count, Origin* origin) {
    // Check if the input Origin or the Origins array is NULL
    if (!origin || !all_origins) {
        return failure; // Return failure (false) if inputs are invalid
    }

    // Iterate through the array of Origins
    for (int i = 0; i < origin_count; i++) {
        // Compare the 'meimad' (dimension name) and the planet name of the current Origin
        if (strcmp(origin->meimad, all_origins[i]->meimad) == 0 &&
            strcmp(origin->planet->name, all_origins[i]->planet->name) == 0) {
            return true; // Return true if a matching Origin is found
            }
    }

    // Return false if no matching Origin was found
    return false;
}

void destroyEverything(Planet** all_planets, int planets_count,
                       PhysicalCharacteristics** all_physical_characteristics, int physical_count,
                       Origin** all_origins, int origin_count,
                       Jerry** all_jerry, int jerry_count) {
    int i; // Loop variable

    // Free all Planet objects
    for (i = 0; i < planets_count; i++) {
        destroyPlanet(all_planets[i]); // Call destroyPlanet for each planet
    }

    // Free all PhysicalCharacteristics objects
    for (i = 0; i < physical_count; i++) {
        destroyPhysicalCharacteristics(all_physical_characteristics[i]); // Call destroyPhysicalCharacteristics
    }

    // Free all Origin objects
    for (i = 0; i < origin_count; i++) {
        destroyOrigin(all_origins[i]); // Call destroyOrigin for each origin
    }

    // Free all Jerry objects
    for (i = 0; i < jerry_count; i++) {
        destoyJerry(all_jerry[i]); // Call destroyJerry for each Jerry
    }

    // Free the arrays themselves
    free(all_jerry);                      // Free the array of pointers to Jerry objects
    free(all_planets);                    // Free the array of pointers to Planet objects
    free(all_physical_characteristics);   // Free the array of pointers to PhysicalCharacteristics
    free(all_origins);                    // Free the array of pointers to Origin objects
}

void clearInputBuffer() {
    int c; // Variable to store each character read

    // Read characters from the input buffer until a newline ('\n') or EOF is encountered
    while ((c = getchar()) != '\n' && c != EOF);
}

FILE* openConfigFile(const char *filename) {
    // Open the file in read mode ("r")
    FILE *file = fopen(filename, "r");

    // Check if the file failed to open
    if (file == NULL) {
        return NULL; // Return NULL to indicate failure
    }

    // Return the pointer to the opened file
    return file;
}

void readConfigFile(FILE *file) {
    char line[301];                  // Buffer to store each line read from the file
    bool readingPlanets = false;     // Flag to indicate whether we are reading planet data
    bool readingJerries = false;     // Flag to indicate whether we are reading Jerry data

    Jerry* jerry = NULL;             // Temporary pointer to hold a Jerry object being created

    // Loop through each line of the file
    while (fgets(line, sizeof(line), file)) {
        // Remove newline characters ('\n' or '\r') from the end of the line
        line[strcspn(line, "\r\n")] = '\0';

        // Check for section headers ("Planets" or "Jerries")
        if (strcmp(line, "Planets") == 0) {
            readingPlanets = true;   // Start reading planet data
            readingJerries = false;  // Stop reading Jerry data
            continue;
        } else if (strcmp(line, "Jerries") == 0) {
            readingJerries = true;   // Start reading Jerry data
            readingPlanets = false;  // Stop reading planet data
            continue;
        }

        // If reading planet data
        if (readingPlanets) {
            char planetName[301];
            float x, y, z;

            // Parse planet information: name and coordinates (x, y, z)
            if (sscanf(line, "%300[^,],%f,%f,%f", planetName, &x, &y, &z) == 4) {
                // Create a new planet and add it to the manager
                Planet* planet = createPlanet(planetName, x, y, z);
                if (addPlanetToManager(&all_planets, &planets_count, planet) == failure) {
                    // Cleanup and exit if memory allocation fails
                    destroyEverything(all_planets, planets_count, all_physical_characteristics, phyiscal_count,
                                      all_origins, origin_count, all_jerry, jerry_count);
                    exit(EXIT_FAILURE); // Exit the program with failure status
                }
            }
        }
        // If reading Jerry data
        else if (readingJerries) {
            if (line[0] == '\t') { // Check if this line defines a physical characteristic
                char propertyName[301];
                float propertyValue;

                // Parse physical characteristic name and value
                sscanf(line + 1, "%300[^:]:%f", propertyName, &propertyValue);

                // Create a new physical characteristic and add it to the manager
                PhysicalCharacteristics* physical = createPhysicalCharacteristics(propertyName, propertyValue);
                if (addPhysicalToManager(&all_physical_characteristics, &phyiscal_count, physical) == failure) {
                    destroyEverything(all_planets, planets_count, all_physical_characteristics, phyiscal_count,
                                      all_origins, origin_count, all_jerry, jerry_count);
                    exit(EXIT_FAILURE); // Exit the program with failure status
                }

                // Add the physical characteristic to the current Jerry
                if(add_physical_to_jerry(jerry, physical)==failure) {
                    destroyEverything(all_planets, planets_count, all_physical_characteristics, phyiscal_count,
                                      all_origins, origin_count, all_jerry, jerry_count);
                    exit(EXIT_FAILURE); // Exit the program with failure status
                }
            } else { // This line defines a new Jerry
                char id[301], dimension[301], planetName[301];
                int happiness;

                // Parse Jerry information: ID, dimension, planet name, and happiness level
                if (sscanf(line, "%300[^,],%300[^,],%300[^,],%d", id, dimension, planetName, &happiness) == 4) {
                    // Retrieve the planet for the current Jerry
                    Planet* planet = get_planet(all_planets, planets_count, planetName);

                    // Create an origin for the Jerry
                    Origin* origin = createOrigin(planet, dimension);
                    if (addOriginToManager(&all_origins, &origin_count, origin) == failure) {
                        destroyEverything(all_planets, planets_count, all_physical_characteristics, phyiscal_count,
                                          all_origins, origin_count, all_jerry, jerry_count);
                        exit(EXIT_FAILURE); // Exit the program with failure status
                    }

                    // Create a new Jerry and add it to the manager
                    jerry = createJerry(id, happiness, origin);
                    if (addJerryToManager(&all_jerry, &jerry_count, jerry) == failure) {
                        destroyEverything(all_planets, planets_count, all_physical_characteristics, phyiscal_count,
                                          all_origins, origin_count, all_jerry, jerry_count);
                        exit(EXIT_FAILURE); // Exit the program with failure status
                    }
                }
            }
        }
    }
}

void print_menu() {

    // Print the title and the menu options to the console
    printf("AW JEEZ RICK, what do you want to do now ?\n");
    printf("1 : Print all Jerries\n");                          // Option 1: Print all Jerry objects
    printf("2 : Print all Planets\n");                          // Option 2: Print all Planet objects
    printf("3 : Add physical characteristic to Jerry\n");       // Option 3: Add a physical characteristic to a Jerry
    printf("4 : Remove physical characteristic from Jerry\n");  // Option 4: Remove a physical characteristic from a Jerry
    printf("5 : Print Jerries by a planet\n");                  // Option 5: Print all Jerries originating from a specific planet
    printf("6 : Print Jerries by a physical characteristic\n"); // Option 6: Print Jerries that have a specific physical characteristic
    printf("7 : Go home\n");                                    // Option 7: Exit the program
}
void print_uppercase(const char *str) {
    // Iterate through each character in the string until the null terminator ('\0') is reached
    for (int i = 0; str[i] != '\0'; i++) {
        // Convert the current character to uppercase and print it
        printf("%c", toupper((unsigned char)str[i]));
    }
}

int main(int argc, char *argv[]) {
    const char *filename = argv[3]; // Retrieve the configuration file name from the command-line arguments
    FILE *file;

    // Open the configuration file
    file = openConfigFile(filename);
    if (file == NULL) {
        return failure; // Return failure if the file could not be opened
    }

    // Read data from the configuration file and initialize objects
    readConfigFile(file);
    fclose(file); // Close the configuration file

    bool done = false; // Flag to control the main program loop
    while (!done) {
        print_menu(); // Print the main menu for user options

        char input[301]; // Buffer to store user input

        // Read user input and remove the newline character
        if (fgets(input, sizeof(input), stdin) != NULL) {
            input[strcspn(input, "\n")] = '\0';
        }

        // Option 1: Print all Jerries
        if (strcmp(input, "1") == 0) {
            for (int i = 0; i < jerry_count; i++) {
                printJerry(all_jerry[i]);
            }

        // Option 2: Print all Planets
        } else if (strcmp(input, "2") == 0) {
            for (int i = 0; i < planets_count; i++) {
                printPlanet(all_planets[i]);
            }

        // Option 3: Add physical characteristic to a specific Jerry
        } else if (strcmp(input, "3") == 0) {
            char id[301];
            printf("What is your Jerry's ID ?\n");

            if (fgets(id, sizeof(id), stdin) == NULL) {
                printf("Error reading input.\n");
                return 1;
            }
            id[strcspn(id, "\n")] = '\0';

            // Check if the Jerry exists
            if (!is_id_exists(all_jerry, jerry_count, id)) {
                printf("OH NO! I CAN'T FIND HIM RICK !\n");
            } else {
                char physical[301];
                printf("What physical characteristic can you add to Jerry - %s ?\n", id);
                fgets(physical, sizeof(physical), stdin);
                physical[strcspn(physical, "\n")] = '\0';

                Jerry* jerry = get_jerry(all_jerry, jerry_count, id);
                if (has_physical(*jerry, physical)) {
                    printf("RICK I ALREADY KNOW HIS ");
                    print_uppercase(physical);
                    printf(" !\n");
                } else {
                    float value;
                    printf("What is the value of his %s ?\n", physical);
                    scanf("%f", &value);

                    // Create the new physical characteristic and add it to Jerry and manager
                    PhysicalCharacteristics* physical_characteristics = createPhysicalCharacteristics(physical, value);
                    if (addPhysicalToManager(&all_physical_characteristics, &phyiscal_count, physical_characteristics) == failure) {
                        destroyEverything(all_planets, planets_count, all_physical_characteristics, phyiscal_count,
                                          all_origins, origin_count, all_jerry, jerry_count);
                        return 1; //failed
                    }
                    add_physical_to_jerry(jerry, physical_characteristics);
                    printJerry(jerry);
                    clearInputBuffer();
                }
            }

        // Option 4: Remove physical characteristic from a Jerry
        } else if (strcmp(input, "4") == 0) {
            char id[301];
            printf("What is your Jerry's ID ?\n");
            if (fgets(id, sizeof(id), stdin) == NULL) {
                printf("Error reading input.\n");
            }
            id[strcspn(id, "\n")] = '\0';

            if (!is_id_exists(all_jerry, jerry_count, id)) {
                printf("OH NO! I CAN'T FIND HIM RICK !\n");
            } else {
                char physical[301];
                printf("What physical characteristic do you want to remove from Jerry - %s ?\n", id);
                fgets(physical, sizeof(physical), stdin);
                physical[strcspn(physical, "\n")] = '\0';

                Jerry* jerry = get_jerry(all_jerry, jerry_count, id);
                if (!has_physical(*jerry, physical)) {
                    printf("RICK I DON'T KNOW HIS ");
                    print_uppercase(physical);
                    printf(" !\n");
                } else {
                    delete_physical_from_jerry(jerry, physical);
                    printJerry(jerry);
                }
            }

        // Option 5: Print all Jerries from a specific planet
        } else if (strcmp(input, "5") == 0) {
            char name[301];
            printf("What planet is your Jerry from ?\n");
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = '\0';

            if (!is_planet_exists(all_planets, planets_count, name)) {
                printf("RICK I NEVER HEARD ABOUT ");
                print_uppercase(name);
                printf(" !\n");
            } else {
                for (int i = 0; i < jerry_count; i++) {
                    if (jerry_from_planet(all_jerry[i], name)) {
                        printJerry(all_jerry[i]);
                    }
                }
            }

        // Option 6: Print all Jerries with a specific physical characteristic
        } else if (strcmp(input, "6") == 0) {
            char physical[301];
            printf("What do you know about your Jerry ?\n");
            fgets(physical, sizeof(physical), stdin);
            physical[strcspn(physical, "\n")] = '\0';

            bool is_a_physical = false;
            for (int i = 0; i < jerry_count; i++) {
                if (has_physical(*all_jerry[i], physical)) {
                    is_a_physical = true;
                    printJerry(all_jerry[i]);
                }
            }
            if (!is_a_physical) {
                printf("OH NO! I DON'T KNOW ANY JERRY'S ");
                print_uppercase(physical);
                printf(" !\n");
            }

        // Option 7: Exit the program and clean up all memory
        } else if (strcmp(input, "7") == 0) {
            printf("AW JEEZ RICK, ALL THE JERRIES GOT FREE !\n");
            destroyEverything(all_planets, planets_count, all_physical_characteristics, phyiscal_count,
                              all_origins, origin_count, all_jerry, jerry_count);
            done = true;

        // Invalid input
        } else {
            printf("RICK WE DON'T HAVE TIME FOR YOUR GAMES !\n");
        }
    }
    return 0; // Return 0 on successful execution
}






