/**
 * @file coyote.h
 * @brief A concrete class that inherits from agent.
 * 
 * Coyotes are different than roadrunners because of their behaviors.
 * Coyotes move to a roadrunner if there is one adjacent to itself, otherwise they move to a random empty cell adjacent to itself.
 * Coyotes breed after each 8th consecutive time step if there is an empty cell adjacent to it.
 * Coyotes die if they go 4 time steps without eating a roadrunner.
 */
#ifndef COYOTE_H
#define COYOTE_H
#include "agent.h"

class Coyote : public Agent
{
    public:
        /**
         * @brief Set what behaviors a coyote has.
         * 
         * They either move or die.
         * If there is a roadrunner adjacent to it, they will move there and eat it.
         * If coyotes go more than 4 time steps without eating a roadrunner.
         */
        Coyote();
        /**
         * @brief Converts coyote object to a string.
         * 
         * @returns "C" to denote it is a coyote.
         */
        virtual std::string toString() override;
};
#endif