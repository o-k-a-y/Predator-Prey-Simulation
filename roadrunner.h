/**
 * @file roadrunner.h
 * @brief A concrete class that inherits from the agent class.
 * Roadrunners are different than coyotes because of their behaviors.
 * Roadrunners move if no coyote is in an adjacent cell and there is at least one empty cell adjacent to it.
 * Roadrunners breed after each 3rd consecutive time step, if there is a free adjacent cell.
 * Roadrunners die if a coyote moves onto its location.
 */
#ifndef ROADRUNNER_H
#define ROADRUNNER_H
#include "agent.h"

class Roadrunner : public Agent
{
    public:
        /**
         * @brief Set what behaviors a roadrunner has.
         * 
         * They either move if there is an open space to move or panic if there is a coyote adjacent to it.
         */
        Roadrunner();

         /**
         * @brief Converts coyote object to a string.
         * 
         * @returns "R" to denote it is a coyote.
         */
        virtual std::string toString() override;
};
#endif