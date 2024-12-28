#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "Jerry.h"


//
// Created by Avishur on 11/29/2024.
//






Jerry* createJerry(char* id, int happiness, Origin* his_origin) {
    // Validate input parameters: id and origin must not be NULL
    if (id == NULL || his_origin == NULL) {
        return NULL; // Return NULL if inputs are invalid
    }

    // Allocate memory for the Jerry struct
    Jerry* jerry = (Jerry*)malloc(sizeof(Jerry));
    if (jerry == NULL) { // Check if memory allocation failed
        return NULL; // Return NULL if allocation failed
    }

    // Allocate memory for the Jerry's ID string
    jerry->ID = (char*)malloc(strlen(id) + 1); // +1 for the null-terminator
    if (!jerry->ID) { // Check if memory allocation for the ID failed
        free(jerry); // Free the previously allocated memory for Jerry
        return NULL; // Return NULL if allocation failed
    }

    // Copy the ID string to the allocated memory
    strcpy(jerry->ID, id);

    // Initialize the remaining Jerry struct fields
    jerry->happines = happiness;          // Set happiness level
    jerry->his_origin = his_origin;       // Set pointer to the origin
    jerry->his_physical = NULL;           // Initialize physical characteristics array to NULL
    jerry->num_of_pyhshical = 0;          // Initialize the count of physical characteristics to 0

    return jerry; // Return the pointer to the newly created Jerry object
}

PhysicalCharacteristics* createPhysicalCharacteristics(char* name, float value) {
    // Check if the input name is NULL
    if (!name) {
        return NULL; // Return NULL if the name is invalid
    }

    // Allocate memory for the PhysicalCharacteristics struct
    PhysicalCharacteristics* characteristics = (PhysicalCharacteristics*)malloc(sizeof(PhysicalCharacteristics));
    if (!characteristics) { // Check if memory allocation failed
        printf("Memory Problem\n");
        return NULL; // Return NULL if allocation failed
    }

    // Allocate memory for the name string
    characteristics->name = (char*)malloc(strlen(name) + 1); // +1 for the null-terminator
    if (!characteristics->name) { // Check if memory allocation for the name failed
        printf("Memory Problem\n");
        free(characteristics); // Free the previously allocated struct memory
        return NULL; // Return NULL if allocation failed
    }

    // Copy the name string into the allocated memory
    strcpy(characteristics->name, name);

    // Set the value of the physical characteristic
    characteristics->value = value;

    // Return the pointer to the newly created PhysicalCharacteristics object
    return characteristics;
}

Planet* createPlanet(char* name, float x, float y, float z) {
    // Check if the input name is NULL
    if (name == NULL) {
        return NULL; // Return NULL if the name is invalid
    }

    // Allocate memory for the Planet struct
    Planet* planet = (Planet*)malloc(sizeof(Planet));
    if (!planet) { // Check if memory allocation failed
        printf("Memory Problem\n");
        return NULL; // Return NULL if allocation failed
    }

    // Allocate memory for the planet name
    planet->name = (char*)malloc(strlen(name) + 1); // +1 for the null-terminator
    if (!planet->name) { // Check if memory allocation for the name failed
        printf("Memory Problem\n");
        free(planet); // Free the previously allocated Planet struct
        return NULL; // Return NULL if allocation failed
    }

    // Copy the planet name into the allocated memory
    strcpy(planet->name, name);

    // Assign the X, Y, and Z coordinates to the Planet struct
    planet->x = x;
    planet->y = y;
    planet->z = z;

    // Return the pointer to the newly created Planet object
    return planet;
}

Origin* createOrigin(Planet* planet, char* meimad) {
    // Check if the input planet or meimad is NULL
    if (!planet || !meimad) {
        return NULL; // Return NULL if either input is invalid
    }

    // Allocate memory for the Origin struct
    Origin* origin = (Origin*)malloc(sizeof(Origin));
    if (!origin) { // Check if memory allocation failed
        printf("Memory Problem\n");
        return NULL; // Return NULL if allocation failed
    }

    // Allocate memory for the meimad (dimension name) string
    origin->meimad = (char*)malloc(strlen(meimad) + 1); // +1 for the null-terminator
    if (!origin->meimad) { // Check if memory allocation for the meimad failed
        printf("Memory Problem\n");
        free(origin); // Free the previously allocated Origin struct
        return NULL; // Return NULL if allocation failed
    }

    // Copy the meimad (dimension name) into the allocated memory
    strcpy(origin->meimad, meimad);

    // Assign the planet pointer to the Origin struct
    origin->planet = planet;

    // Return the pointer to the newly created Origin object
    return origin;
}

bool jerry_from_planet(Jerry* jerry, char *name) {
    // Compare the input planet name with Jerry's planet name
    if (strcmp(name, jerry->his_origin->planet->name) == 0) {
        return true; // If names match, return true
    }
    return false; // Otherwise, return false
}

float roundTwoDecimals(float number) {
    // Multiply the number by 100 and add 0.5 to ensure proper rounding
    int rounded = (int)(number * 100 + 0.5);

    // Divide the rounded integer by 100.0f to get the float with two decimal places
    return (float)rounded / 100.0f;
}



status add_physical_to_jerry(Jerry* jerry, PhysicalCharacteristics* physical) {
    // Reallocate memory for the array of physical characteristics
    // Increase the size of the array by one
    PhysicalCharacteristics** temp = realloc(jerry->his_physical, sizeof(PhysicalCharacteristics*) * (jerry->num_of_pyhshical + 1));
    if (!temp) { // Check if reallocation failed
        printf("Memory Problem\n");
        return failure; // Exit the function if memory allocation failed
    }

    // Assign the newly reallocated memory back to Jerry's physical characteristics array
    jerry->his_physical = temp;

    // Increment the number of physical characteristics Jerry has
    jerry->num_of_pyhshical++;

    // Add the new PhysicalCharacteristic pointer to the end of the array
    jerry->his_physical[jerry->num_of_pyhshical - 1] = physical;
    return success;
}

bool has_physical(Jerry jerry, char* physical) {
    // Iterate through all physical characteristics of the given Jerry
    for (int i = 0; i < jerry.num_of_pyhshical; i++) {
        // Compare the name of the current physical characteristic with the input name
        if (strcmp(jerry.his_physical[i]->name, physical) == 0) {
            return true; // If a match is found, return true
        }
    }
    // If no match is found after checking all characteristics, return false
    return false;


}

status delete_physical_from_jerry(Jerry *jerry, char *physical_name) {
    if (!jerry || !physical_name) {
        return failure;
    }

    // אם אין תכונה כזו, נכשל
    if (!has_physical(*jerry, physical_name)) {
        return failure;
    }

    int index = -1;
    // מוצאים את האינדקס שבו נמצא ה-physical_name
    for (int i = 0; i < jerry->num_of_pyhshical; i++) {
        if (strcmp(jerry->his_physical[i]->name, physical_name) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        return failure; // לא אמור לקרות, אבל ליתר ביטחון
    }

    // ראשית משחררים את האובייקט
    destroyPhysicalCharacteristics(jerry->his_physical[index]);

    // מעבירים את כל אלו שאחרי האיבר – מקום אחד אחורה
    for (int i = index; i < jerry->num_of_pyhshical - 1; i++) {
        jerry->his_physical[i] = jerry->his_physical[i + 1];
    }

    // מורידים את מספר התכונות ב-1
    jerry->num_of_pyhshical--;

    // עכשיו, אם יש עדיין תכונות, נקצה מחדש
    if (jerry->num_of_pyhshical > 0) {
        PhysicalCharacteristics** temp = realloc(
            jerry->his_physical,
            jerry->num_of_pyhshical * sizeof(PhysicalCharacteristics*)
        );

        // אם הצליח, עדכן
        if (temp != NULL) {
            jerry->his_physical = temp;
        }
        // אם נכשל, אפשר או להחזיר failure, או להשאיר את המצב הקודם
    } else {
        // אם אין יותר מאפיינים, שחרר את המערך ושים אותו כ-NULL
        free(jerry->his_physical);
        jerry->his_physical = NULL;
    }

    return success;
}




status printJerry(Jerry *jerry) {
    // Check if the input Jerry pointer is NULL
    if (jerry == NULL) {
        return failure; // Exit the function if Jerry is NULL
    }

    // Print Jerry's ID
    printf("Jerry , ID - %s :\n", jerry->ID);

    // Print Jerry's happiness level
    printf("Happiness level : %d\n", jerry->happines);

    // Check if Jerry's origin exists
    if (jerry->his_origin != NULL) {
        // Print the dimension (meimad) of Jerry's origin
        printf("Origin : %s\n", jerry->his_origin->meimad);

        // Check if Jerry's origin has a planet
        if (jerry->his_origin->planet != NULL) {
            // Print the planet's name and coordinates, rounded to two decimal places
            printf("Planet : %s (%.2f,%.2f,%.2f)\n",
                   jerry->his_origin->planet->name,
                   roundTwoDecimals(jerry->his_origin->planet->x),
                   roundTwoDecimals(jerry->his_origin->planet->y),
                   roundTwoDecimals(jerry->his_origin->planet->z));
        }
    }

    // Check if Jerry has physical characteristics
    if (jerry->num_of_pyhshical > 0 && jerry->his_physical != NULL) {
        printf("Jerry's physical Characteristics available :\n");

        // Iterate through all of Jerry's physical characteristics and print them
        for (int i = 0; i < jerry->num_of_pyhshical; i++) {
            if (jerry->his_physical[i] != NULL) {
                if (i == 0) {
                    // Print a tab before the first physical characteristic
                    printf("\t%s : %.2f", jerry->his_physical[i]->name,
                           roundTwoDecimals(jerry->his_physical[i]->value));
                } else {
                    // Print a space before the rest of the characteristics
                    printf(" , %s : %.2f", jerry->his_physical[i]->name,
                           roundTwoDecimals(jerry->his_physical[i]->value));
                }

                // Print a newline at the end
                if (i == jerry->num_of_pyhshical - 1) {
                    printf("\n");
                }
            }
        }
    }
    return success;
}

status printPlanet(Planet *planet) {
    // Check if the input planet pointer is NULL
    if (planet == NULL) {
        return failure; // Return if the planet does not exist
    }

    // Print the planet's name and its coordinates rounded to two decimal places
    printf("Planet : %s (%.2f,%.2f,%.2f)\n",
           planet->name,                      // Planet name
           roundTwoDecimals(planet->x),       // X coordinate rounded to 2 decimals
           roundTwoDecimals(planet->y),       // Y coordinate rounded to 2 decimals
           roundTwoDecimals(planet->z));      // Z coordinate rounded to 2 decimals
    return success;
}

status destroyPlanet(Planet* planet) {
    // Check if the input planet pointer is NULL
    if (planet == NULL) {
        return failure; // Exit the function if the planet is NULL
    }

    // Free the memory allocated for the planet's name
    free(planet->name);

    // Free the memory allocated for the Planet struct itself
    free(planet);
    return success;
}

status destroyPhysicalCharacteristics(PhysicalCharacteristics* physical) {
    // Check if the input physical pointer is NULL
    if (!physical) return failure;
    /* if pc->name was dynamically allocated */
    if (physical->name) {
        free(physical->name);
        physical->name = NULL;
    }
    free(physical);
    return success;
}

status destroyOrigin(Origin* origin) {
    // Check if the input origin pointer is NULL
    if (origin == NULL) {
        return failure; // Exit the function if the pointer is NULL
    }

    // Free the memory allocated for the meimad (dimension name) string
    free(origin->meimad);

    // Free the memory allocated for the Origin struct itself
    free(origin);
    return success;
}

status destoyJerry(Jerry* jerry) {
    // Check if the input Jerry pointer is NULL
    if (!jerry) return failure;

    /* free j->ID if it's truly owned by Jerry
       (unless you're doing the no-op approach in the HashTable). */
    if (jerry->ID) {
        free(jerry->ID);
        jerry->ID = NULL;
    }

    /* free j->origin if allocated */
    if (jerry->his_origin) {
        /* e.g., free j->origin->dimensionName if allocated.
           Possibly do not free j->origin->planet since it's managed
           in the global planets list. */
        destroyOrigin(jerry->his_origin);
        jerry->his_origin = NULL;
    }

    /* free each physical characteristic */
    for (int i = 0; i < jerry->num_of_pyhshical; i++) {
        destroyPhysicalCharacteristics(jerry->his_physical[i]);
    }
    free(jerry->his_physical);  // free the array itself

    free(jerry); // finally free the Jerry
    return success;
}
bool compare_planets(Planet* planet,char* name) {
    if (planet == NULL || name == NULL) {
        return false;
    }
    if (strcmp(planet->name, name) == 0) {
        return true;
    }
    return false;
}
bool compare_jerry(Jerry* jerry,char* id) {
    if (jerry == NULL || jerry->ID == NULL || id == NULL) {
        return false;
    }
    if (strcmp(jerry->ID, id) == 0) {
        return true;
    }
    return false;

}

float get_value(Jerry* jerry,char* physical) {
    if (jerry == NULL || jerry->ID == NULL || physical == NULL) {
        return failure;
    }
    for (int i = 0; i < jerry->num_of_pyhshical; i++) {
        if (strcmp(jerry->his_physical[i]->name, physical) == 0) {
            return jerry->his_physical[i]->value;
        }
    }
    return failure;

}

