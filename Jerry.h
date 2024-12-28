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
Jerry* createJerry(char *id, int happines, Origin *his_origin);

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
Planet* createPlanet(char *name, float x, float y, float z);

/**
 * @brief Creates a new PhysicalCharacteristic with the given name and value.
 *
 * @param name   Pointer to the PhysicalCharacteristic's name (string).
 * @param value  Value of the PhysicalCharacteristic (float).
 *
 * @return Pointer to the newly created PhysicalCharacteristic,
 *         or NULL if creation fails.
 */
PhysicalCharacteristics* createPhysicalCharacteristics(char *name, float value);

/**
 * @brief Creates a new Origin with the given planet and dimension (meimad).
 *
 * @param planet  Pointer to the Planet associated with this Origin.
 * @param meimad  Pointer to the name of the dimension (string).
 *
 * @return Pointer to the newly created Origin,
 *         or NULL if creation fails.
 */
Origin* createOrigin(Planet *planet, char *meimad);

/**
 * @brief Adds a new Jerry to the Jerry manager and updates the Jerry count.
 *
 * @param all_jerry     Pointer to the pointer of the array of pointers to Jerry objects.
 * @param jerry_count   Pointer to the current count of Jerries (int).
 * @param new_jerry     Pointer to the new Jerry to be added.
 *
 * @return status       success if the Jerry was added successfully,
 *                      failure if memory allocation fails or new_jerry is NULL.
 */
status addJerryToManager(Jerry*** all_jerry, int* jerry_count, Jerry* new_jerry);

/**
 * @brief Adds a new Planet to the Planet manager and updates the Planet count.
 *
 * @param all_planets     Pointer to the pointer of the array of pointers to Planet objects.
 * @param planets_count   Pointer to the current count of Planets (int).
 * @param new_planet      Pointer to the new Planet to be added.
 *
 * @return status         success if the Planet was added successfully,
 *                        failure if memory allocation fails or new_planet is NULL.
 */
status addPlanetToManager(Planet*** all_planets, int* planets_count, Planet* new_planet);

/**
 * @brief Adds a new PhysicalCharacteristic to the manager and updates the count.
 *
 * @param all_physical      Pointer to the pointer of the array of pointers to PhysicalCharacteristics.
 * @param physical_count    Pointer to the current count of PhysicalCharacteristics (int).
 * @param new_physical      Pointer to the new PhysicalCharacteristic to be added.
 *
 * @return status           success if the PhysicalCharacteristic was added successfully,
 *                          failure if memory allocation fails or new_physical is NULL.
 */
status addPhysicalToManager(PhysicalCharacteristics*** all_physical, int* physical_count, PhysicalCharacteristics* new_physical);

/**
 * @brief Adds a new Origin to the manager and updates the Origin count.
 *
 * @param all_origins     Pointer to the pointer of the array of pointers to Origin objects.
 * @param origin_count    Pointer to the current count of Origins (int).
 * @param new_origin      Pointer to the new Origin to be added.
 *
 * @return status         success if the Origin was added successfully,
 *                        failure if memory allocation fails or new_origin is NULL.
 */
status addOriginToManager(Origin*** all_origins, int* origin_count, Origin* new_origin);

/**
 * @brief Checks if a Jerry with the given ID exists in the array.
 *
 * @param all_jerry     Pointer to the array of pointers to Jerry objects.
 * @param jerry_count   The current count of Jerries in the array.
 * @param id            Pointer to the ID string to search for.
 *
 * @return bool         true if a Jerry with the given ID exists, false otherwise.
 */
bool is_id_exists(Jerry** all_jerry, int jerry_count, char *id);

//the function gets:
//pointer to the pointer of the pointers to planets array, the number of planets,char *planet_name
//and check if this planet exits
bool is_planet_exists(Planet** all_planets, int planets_count, char *planet_name);

/**
 * @brief Checks if a given Origin exists in the array of Origins.
 *
 * @param all_origins     Pointer to the array of pointers to Origin objects.
 * @param origin_count    The current count of Origins in the array.
 * @param origin          Pointer to the Origin to search for.
 *
 * @return bool           true if the given Origin exists in the array, false otherwise.
 */
bool is_origin_exits(Origin** all_origins, int origin_count, Origin* origin);

/**
 * @brief Retrieves a pointer to a Jerry with the specified ID from the array.
 *
 * @param all_jerry     Pointer to the array of pointers to Jerry objects.
 * @param jerry_count   The current count of Jerries in the array.
 * @param id            Pointer to the ID string of the Jerry to retrieve.
 *
 * @return Jerry*       Pointer to the Jerry with the given ID.
 *                      Assumes the ID exists; use is_id_exists beforehand to verify.
 */
Jerry* get_jerry(Jerry** all_jerry, int jerry_count, char *id);

/**
 * @brief Retrieves a pointer to a Planet with the specified name from the array.
 *
 * @param all_planets     Pointer to the array of pointers to Planet objects.
 * @param planets_count   The current count of Planets in the array.
 * @param name            Pointer to the name string of the Planet to retrieve.
 *
 * @return Planet*        Pointer to the Planet with the given name.
 *                        Assumes the name exists; use is_planet_exists beforehand to verify.
 */
Planet* get_planet(Planet** all_planets, int planets_count, char *name);

/**
 * @brief Checks if the given Jerry has a specific PhysicalCharacteristic.
 *
 * @param jerry      A Jerry object to check.
 * @param physical   Pointer to the name of the PhysicalCharacteristic (string).
 *
 * @return bool      true if the Jerry has the specified PhysicalCharacteristic,
 *                   false otherwise.
 */
bool has_physical(Jerry jerry, char* physical);

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
status add_physical_to_jerry(Jerry* jerry, PhysicalCharacteristics* physical);


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
status delete_physical_from_jerry(Jerry *jerry, char *physical_name);

/**
 * @brief Rounds a given float number to two decimal places.
 *
 * @param number    The float number to be rounded.
 *
 * @return float    The rounded float value with two decimal places.
 */
float roundTwoDecimals(float number);

/**
 * @brief Frees the memory allocated for a Planet object.
 *
 * @param planet   Pointer to the Planet to be destroyed.
 *
 * @note The function safely frees the name field of the Planet
 *       and the Planet object itself.
 */
status destroyPlanet(Planet* planet);

//function gets PhysicalCharacteristics* physical and destroy this physical free it from the memory
status destroyPhysicalCharacteristics(PhysicalCharacteristics* physical);

/**
 * @brief Frees the memory allocated for an Origin object.
 *
 * @param origin   Pointer to the Origin to be destroyed.
 *
 * @note The function safely frees the meimad field of the Origin
 *       and the Origin object itself.
 */
status destroyOrigin(Origin* origin);

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
status destoyJerry(Jerry* jerry);

/**
 * @brief Clears the input buffer to prevent unwanted behavior during input handling.
 *
 * @note This function discards all remaining characters in the input buffer,
 *       up to and including the newline character ('\n'), ensuring the buffer is clean.
 *       It is typically used to avoid issues when switching between input operations.
 */
void clearInputBuffer();

/**
 * @brief Checks if a given Jerry is from the specified planet.
 *
 * @param jerry   Pointer to the Jerry to be checked.
 * @param name    Pointer to the name of the planet (string).
 *
 * @return bool   true if the Jerry is from the specified planet, false otherwise.
 */
bool jerry_from_planet(Jerry* jerry, char *name);

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
status printJerry(Jerry* jerry);

/**
 * @brief Prints all the data of a given Planet.
 *
 * @param planet   Pointer to the Planet whose data will be printed.
 *
 * @note The function prints:
 *       - Planet's name.
 *       - Planet's coordinates: x, y, and z rounded to two decimal places.
 */
status printPlanet(Planet* planet);

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


bool compare_planets(Planet* planet,char* name);

bool compare_jerry(Jerry* jerry,char* id);

#endif // JERRY_H