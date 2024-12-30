#ifndef JERRY_H
#define JERRY_H

#include <stdbool.h>
#include "Defs.h"

//planet struct contain name(str) and 3 cordination(float pointer)
typedef struct {
    char *name;
    float x;
    float y;
    float z;
} Planet;

//origin struct contain a pointer to a planet and meimad name(str pointer)
typedef struct {
    Planet *planet;
    char *meimad;
} Origin;

//PhysicalCharacteristics contains name(str pointer) and vlaue(float)
typedef struct {
    char *name;
    float value;
} PhysicalCharacteristics;

//jerry struct contains id(str pointer) happines(int 0-100) his origin(origin pointer)
//his pyhisical(an array of pointers to all his PhysicalCharacteristics) and num_of_pyhshical(how many phyisical he has)
typedef struct {
    char *ID;
    int happines;
    Origin *his_origin;
    PhysicalCharacteristics **his_physical;
    int num_of_pyhshical;
} Jerry;


/**
 * @brief Creates a new Jerry with no PhysicalCharacteristics.
 *
 * @param id          Pointer to the Jerry's ID (string).
 * @param happines    Jerry's happiness level (integer, range: 0-100).
 * @param his_origin  Pointer to the Jerry's Origin (planet and dimension).
 *
 * @return Pointer to the newly created Jerry, or NULL if creation fails.
 */
Jerry* createJerry(char *id, int happines, Origin *his_origin); //+++

/**
 * @brief Creates a new Planet with the given name and coordinates.
 *
 * @param name   Pointer to the Planet's name (string).
 * @param x      X-coordinate of the Planet (float).
 * @param y      Y-coordinate of the Planet (float).
 * @param z      Z-coordinate of the Planet (float).
 *
 * @return Pointer to the newly created Planet, or NULL if creation fails.
 */
Planet* createPlanet(char *name, float x, float y, float z); //+++

/**
 * @brief Creates a new PhysicalCharacteristic with the given name and value.
 *
 * @param name   Pointer to the PhysicalCharacteristic's name (string).
 * @param value  Value of the PhysicalCharacteristic (float).
 *
 * @return Pointer to the newly created PhysicalCharacteristic,
 *         or NULL if creation fails.
 */
PhysicalCharacteristics* createPhysicalCharacteristics(char *name, float value); //+++

/**
 * @brief Creates a new Origin with the given planet and dimension (meimad).
 *
 * @param planet  Pointer to the Planet associated with this Origin.
 * @param meimad  Pointer to the name of the dimension (string).
 *
 * @return Pointer to the newly created Origin,
 *         or NULL if creation fails.
 */
Origin* createOrigin(Planet *planet, char *meimad); //+++



/**
 * @brief Checks if the given Jerry has a specific PhysicalCharacteristic.
 *
 * @param jerry      A Jerry object to check.
 * @param physical   Pointer to the name of the PhysicalCharacteristic (string).
 *
 * @return bool      true if the Jerry has the specified PhysicalCharacteristic,
 *                   false otherwise.
 */
bool has_physical(Jerry jerry, char* physical); //+++

/**
 * @brief Adds a PhysicalCharacteristic to the given Jerry.
 *
 * @param jerry       Pointer to the Jerry to which the PhysicalCharacteristic will be added.
 * @param physical    Pointer to the PhysicalCharacteristic to add.
 *
 * @return status     success if the PhysicalCharacteristic was added successfully,
 *                    failure if memory allocation fails.
 *
 * @note Use this function only after verifying that Jerry does not already have
 *       the specified PhysicalCharacteristic (use has_physical beforehand).
 */
status add_physical_to_jerry(Jerry* jerry, PhysicalCharacteristics* physical); //+++


/**
 * @brief Deletes a PhysicalCharacteristic from the given Jerry.
 *
 * @param jerry            Pointer to the Jerry from which the PhysicalCharacteristic will be removed.
 * @param physical_name    Pointer to the name of the PhysicalCharacteristic to delete (string).
 *
 * @return status          success if the PhysicalCharacteristic was successfully deleted,
 *                         failure if the operation fails.
 *
 * @note Use this function only after verifying that Jerry has the specified
 *       PhysicalCharacteristic (use has_physical beforehand).
 */
status delete_physical_from_jerry(Jerry *jerry, char *physical_name); //+++

/**
 * @brief Rounds a given float number to two decimal places.
 *
 * @param number    The float number to be rounded.
 *
 * @return float    The rounded float value with two decimal places.
 */
float roundTwoDecimals(float number); //+++

/**
 * @brief Frees the memory allocated for a Planet object.
 *
 * @param planet   Pointer to the Planet to be destroyed.
 *
 * @note The function safely frees the name field of the Planet
 *       and the Planet object itself.
 */
status destroyPlanet(Planet* planet); //+++

//function gets PhysicalCharacteristics* physical and destroy this physical free it from the memory
status destroyPhysicalCharacteristics(PhysicalCharacteristics* physical); //+++

/**
 * @brief Frees the memory allocated for an Origin object.
 *
 * @param origin   Pointer to the Origin to be destroyed.
 *
 * @note The function safely frees the meimad field of the Origin
 *       and the Origin object itself.
 */
status destroyOrigin(Origin* origin); //+++

/**
 * @brief Frees the memory allocated for a Jerry object, including its components.
 *
 * @param jerry   Pointer to the Jerry to be destroyed.
 *
 * @note The function safely frees:
 *       - The ID field of the Jerry.
 *       - The array of PhysicalCharacteristics pointers (if it exists).
 *       - The Jerry object itself.
 */
status destoyJerry(Jerry* jerry); //+++

/**
 * @brief Clears the input buffer to prevent unwanted behavior during input handling.
 *
 * @note This function discards all remaining characters in the input buffer,
 *       up to and including the newline character ('\n'), ensuring the buffer is clean.
 *       It is typically used to avoid issues when switching between input operations.
 */
void clearInputBuffer(); //+++

/**
 * @brief Checks if a given Jerry is from the specified planet.
 *
 * @param jerry   Pointer to the Jerry to be checked.
 * @param name    Pointer to the name of the planet (string).
 *
 * @return bool   true if the Jerry is from the specified planet, false otherwise.
 */
bool jerry_from_planet(Jerry* jerry, char *name); //+++

/**
 * @brief Prints all the data of a given Jerry.
 *
 * @param jerry   Pointer to the Jerry whose data will be printed.
 *
 * @note The function prints:
 *       - Jerry's ID.
 *       - Happiness level.
 *       - Origin (dimension name).
 *       - Planet's name and coordinates (x, y, z).
 *       - List of physical characteristics (if available).
 */
status printJerry(Jerry* jerry); //+++

/**
 * @brief Prints all the data of a given Planet.
 *
 * @param planet   Pointer to the Planet whose data will be printed.
 *
 * @note The function prints:
 *       - Planet's name.
 *       - Planet's coordinates: x, y, and z rounded to two decimal places.
 */
status printPlanet(Planet* planet); //+++

/**
 * @brief Frees all allocated memory for Jerries, Planets, Origins, and PhysicalCharacteristics.
 *
 * @param all_planets                 Pointer to the array of pointers to Planet objects.
 * @param planets_count               The number of Planet objects in the array.
 * @param all_physical_characteristics Pointer to the array of pointers to PhysicalCharacteristics objects.
 * @param physical_count              The number of PhysicalCharacteristics objects in the array.
 * @param all_origins                 Pointer to the array of pointers to Origin objects.
 * @param origin_count                The number of Origin objects in the array.
 * @param all_jerry                   Pointer to the array of pointers to Jerry objects.
 * @param jerry_count                 The number of Jerry objects in the array.
 *
 * @note This function iterates over all arrays, destroys each individual object using its
 *       respective destroy function, and then frees the arrays themselves.
 */
void destroyEverything(Planet** all_planets, int planets_count,
                       PhysicalCharacteristics** all_physical_characteristics, int physical_count,
                       Origin** all_origins, int origin_count,
                       Jerry** all_jerry, int jerry_count);


bool compare_planets(Planet* planet,char* name); //+++

bool compare_jerry(Jerry* jerry,char* id); //+++

float get_value(Jerry* jerry,char* physical); //+++

#endif // JERRY_H