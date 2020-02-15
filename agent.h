/**
 * @file agent.h
 * @brief An abstract class that roadrunner and coyote inherit from.
 */
#ifndef AGENT_H
#define AGENT_H
#include <string>

class Agent
{
    public:
        /**
         * @brief Pure virtual function to convert the position on the board to an "R" or "C" depending if there is a roadrunner or coyote existing there.
         * 
         */
        virtual std::string toString() = 0;

        /**
         * @brief The possible behaviors an agent can have.
         * 
         * An agent can either moveRoadrunner if it's a roadrunner, moveCoyote if it's a coyote, movePanic if it's a roadrunner, or die if it's a coyote.
         */
        enum behaviorTypes
        {
            moveRoadrunner,
            moveCoyote,
            movePanic,
            die
        };

        /**
         * @brief Gets the agents position.
         * 
         * @returns The position on the board where the agent is.
         */
        int getPosition()
        {
            return position;
        }

        /**
         * @brief Sets the agents position.
         * 
         * @param[in] position The position to set the agent to.
         */
        void setPosition(int position)
        {
            this->position = position;
        }

        /**
         * @brief The number of behaviors an agent has.
         * 
         * @returns The number of behaviors an agent has.
         */
        int getNumberOfBehaviors()
        {
            return numberOfBehaviors;
        }

        /**
         * @brief Get the behaviors of the agent.
         * 
         * @returns The behaviors the agent has.
         */
        behaviorTypes* getBehaviors()
        {
            return behaviors;
        }

    protected:
        int position;
        behaviorTypes *behaviors;
        int numberOfBehaviors;    
};
#endif
