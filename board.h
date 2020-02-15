/** 
 * @file board.h
 * @brief A singleton class that holds the agents.
 * This class is responsible for doing the simulation.
 */
#ifndef BOARD_H
#define BOARD_H
#include <iostream>
#include <vector>
#include <utility>
#include "roadrunner.h"
#include "coyote.h"

class Board
{
	public:
		/**
		 * @brief Get the single instance of the board.
		 * 
		 * @param[in] file The configuration file to be parsed.
		 * @returns The board object.
		 */
		static Board* getInstance(char *file);
		/**
		 * @brief Board simulation.
		 * 
		 * This function will loop through the vector of available agents and perform the necessary actions 
		 * based on its location and what's surrounding it.
		 */
		void simulate();

	private:
		/**
		 * @brief Constructor for the board which initializes the board and adds the agents to the board in random locations.
		 * 
		 * @param[in] file The configuration file to be parsed.
		 */
		Board(char *file);

		/**
		 * @brief Copy constructor.
		 * 
		 * Never implemented as a singleton class can only have one instance.
		 */
		Board(Board const &);

		/**
		 * @brief Parse and validate the configuration file.
		 * 
		 * @param[in] file The configuration file to be parsed. 
		 */
		void parseFile(char *file);
		
		/** 
		 * @brief Check if a location is empty.
		 */
		bool isLocEmpty(int loc);

		/**
		 * @brief Prints the current state of the board.
		 * 
		 * @param[in] loc The location on the board.
		 * @returns True if the location is empty, False if not.
		 */
		void printBoard();

		/**
		 * @brief Overloaded = operator.
		 * 
		 * Never implemented as we shouldn't be allowed to copy a singleton object.
		 */
		Board &operator=(Board const &);

		/**
		 * @brief The singleton instance
		 */
		static Board* m_pInstance;

		/**
		 * @brief The board full of agents.
		 */
		Agent** board;

		/**
		 * @brief A vector to hold the current number of agents that are ready to be moved.
		 */
		std::vector<Agent*> agents;

		int timeStep;								// the time step we are on
		int areaOfBoard;							// number of rows * columns
		int rows;    								// y dimension of the board
        int columns;     							// x dimension of the board
        int roadrunners;    						// number of roadrunners
        int coyotes;        						// number of coyotes
        int iterations;     						// number of iterations to run 
		
};

#endif