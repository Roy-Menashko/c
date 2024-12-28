//
// Created by Avishur on 12/6/2024.
//

#ifndef DEFS_H
#define DEFS_H


/**
 * @brief Enum representing the status of a function's execution.
 *
 * @enum e_status
 *
 * @var success   Indicates that the function executed successfully.
 * @var failure   Indicates that the function encountered an error or failed.
 */
typedef enum e_status {
    success,  /**< Successful execution */
    failure   /**< Failed execution */
} status;
typedef void * Element;



typedef Element(*CopyFunction) (Element);
typedef status(*FreeFunction) (Element);
typedef status(*PrintFunction) (Element);
typedef int(*TransformIntoNumberFunction) (Element);
typedef bool(*EqualFunction) (Element, Element);

#endif //DEFS_H
