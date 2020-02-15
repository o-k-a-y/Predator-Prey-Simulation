#include "roadrunner.h"

/**
 * @brief Set what behaviors a roadrunner has.
 * 
 * They either move if there is an open space or panic if there is a coyote adjacent to it.
 */
Roadrunner::Roadrunner()
{
    behaviors = new behaviorTypes[2] {moveRoadrunner, movePanic};
    numberOfBehaviors = 2;
}

/**
 * @brief Converts coyote object to a string.
 * 
 * @returns "R" to denote it is a coyote.
 */
std::string Roadrunner::toString()
{
    return "R";
}