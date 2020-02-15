#include "coyote.h"


/**
 * @brief Set what behaviors a coyote has.
 * 
 * They either move or die.
 * If there is a roadrunner adjacent to it, they will move there and eat it.
 * If coyotes go more than 4 time steps without eating a roadrunner.
 */
Coyote::Coyote()
{
    behaviors = new behaviorTypes[3] {moveCoyote, die};
    numberOfBehaviors = 2;
}

/**
 * @brief Converts coyote object to a string.
 * 
 * @returns "C" to denote it is a coyote.
 */
std::string Coyote::toString()
{
    return "C";
}