#define DEBUG
#include "board.h"
#include <stdlib.h>             // for rand
#include <fstream>              // for parsing file
#include <unordered_map>        // for hashmaps
#include <unordered_set>        // to calculate new random numbers
#include <limits.h>             // for INT_MAX

// Link static variable
Board* Board::m_pInstance;      

/**
 * @brief Get the single instance of the board.
 * 
 * @param[in] file The configuration file to be parsed.
 * @returns The board object.
 */
Board* Board::getInstance(char *file)
{   
    if (m_pInstance == NULL)
    {
        try
        {
            m_pInstance = new Board(file);
        }
        catch (std::bad_alloc)
        {
            std::cerr << "Board size too large, exiting.." << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    return m_pInstance;
}

/**
 * @brief Constructor for the board which initializes the board and adds the agents to the board in random locations.
 * 
 * @param[in] file The configuration file to be parsed.
 */
Board::Board(char *file)
{
    parseFile(file);
    // Initialize board with null values
    board = new Agent*[rows*columns] {NULL};

    // Save area of board to Board class
    areaOfBoard = rows*columns;

    // Reserve memory for agents
    agents.reserve(areaOfBoard);

    // Push roadrunners to agent vector
    for (int i = 0; i < roadrunners; i++)
    {
        agents.push_back(new Roadrunner);
    }

    // Push coyotes to agent vector
    for (int i = 0; i < coyotes; i++)
    {
        agents.push_back(new Coyote);
    }

    // Add agents to the board and set their position
    std::unordered_set<int> randomSet;
    for (int i = 0, randomLocation; i < (int)agents.size(); i++)
    {
        // Generate a random number that's not in use
        do {
            randomLocation = rand() % areaOfBoard;
        } while (randomSet.find(randomLocation) != randomSet.end());

        // Make sure we don't use the same location
        randomSet.insert(randomLocation);

        // Insert agent into board and set agent's position
        board[randomLocation] = agents[i];
        agents[i]->setPosition(randomLocation);

        std::cout << randomLocation << std::endl;
    }

    timeStep = 0;

    #ifdef DEBUG
    printBoard();
    #endif
}

/**
 * @brief Parse and validate the configuration file.
 * 
 * @param[in] file The configuration file to be parsed. 
 */
void Board::parseFile(char *file)
{
    // Open file and check if successful
    std::fstream fs;
    fs.open(file);

    // Abort program if opened failed
    if (!fs)
    {
        std::cerr << "Config file could not be opened, exiting.." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Hashmap to store values from config file
    std::unordered_map<std::string, int> configValues;

    // Get all numbers from config file
    std::string line;
    while (getline(fs, line))
    {
        // Get location of where the colon is
        int colonLocation = line.find(':');

        // The key for the hashmap is everything before the colon
        std::string key = line.substr(0, colonLocation);

        // Add to hashmap
        try
        {   
            // The value for the hashmap is everything after the colon
            try
            {
                int value = stoi(line.substr(colonLocation + 2, line.length()));
                configValues[key] = value;
            }
            catch (std::out_of_range)
            {
                std::cerr << "Value for " + key + " is out of range " + "exiting.." << std::endl;
                exit(EXIT_FAILURE);
            }       
        }
        catch (std::invalid_argument)
        {
            std::cerr << "Could not convert " + line.substr(colonLocation + 2, line.length())\
            + " to an integer" << std::endl;
            exit(EXIT_FAILURE);
        } 
    }

    // Validate the hashmap
    for (std::unordered_map<std::string, int>::iterator it = configValues.begin(); it != configValues.end(); ++it)
    {
        if (it->first == "Board length")
        {
            columns = it->second;
        }
        if (it->first == "Board width")
        {
            rows = it->second;
        }
        if (it->first == "Roadrunners")
        {
            roadrunners = it->second;
        }
        if (it->first == "Coyotes")
        {
            coyotes = it->second;
        }
        if (it->first == "Iterations")
        {
            iterations = it->second;
        }         
    }

    // Make sure board is at least a 1 by 1 matrix
    if (columns <= 0 || rows <= 0 || roadrunners <= 0 || coyotes <= 0 || iterations <= 0)
    {
        std::cerr << "All values must be greater than 0, exiting.." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Make sure there aren't too many agents
    if ((coyotes + roadrunners) > (columns * rows))
    {
        std::cerr << "So many agents, so little room." << std::endl;
        exit(EXIT_FAILURE);
    }
}

/** 
 * @brief Check if a location is empty.
 */
bool Board::isLocEmpty(int loc)
{
    return board[loc] != NULL;
}

/**
 * @brief Prints the current state of the board.
 * 
 * @param[in] loc The location on the board.
 * @returns True if the location is empty, False if not.
 */
void Board::printBoard()
{
    std::cout << std::endl;
    for (int i = 0; i < areaOfBoard; i++)
    {
        std::cout << "|";
        if (board[i] != NULL)
        {
            std::cout << board[i]->toString();  
        }
        else
        {
            std::cout << " ";
        }

        if ((i + 1) % columns == 0 && i != 0)
        {
            std::cout << "|";
            std::cout << std::endl;
        }
    }

    std::cout << std::endl;
}

#ifdef DEBUG
/**
 * @brief Overloaded = operator.
 * 
 * Never implemented as we shouldn't be allowed to copy a singleton object.
 */
std::ostream& operator<<(std::ostream& os, Agent& agent)
{
    os << agent.getPosition();
    return os;
}
#endif

/**
 * @brief Constructor for the board which initializes the board and adds the agents to the board in random locations.
 * @param[in] file The configuration file to be parsed.
 */
void Board::simulate()
{
    while (iterations > timeStep)
    {
        // Loop backwards through agent vector to pop off agent to run in linear time
        for (std::vector<Agent*>::reverse_iterator rit = agents.rbegin(); rit != agents.rend(); ++rit)
        {
            // Get the list of possible behaviors for each agent
            Agent::behaviorTypes *behaviors = {(**rit).getBehaviors()};

            // Store current location of agent and their adjacent locations
            int location = (**rit).getPosition();
            
            // If we're at the right side, we can't go right as it would start the next row
            int right = location + 1;
            if ((right % columns) == 0)
            {
                right = -1;
            }

            // If we're at the left side, we can't go left as it would go back a row
            int left = location - 1;
            if ((left + 1) % columns == 0)
            {
                left = -1;
            }

            int up = location - columns;
            int down = location + columns;
            int possibleAgents[4] = {right, left, up, down};
            std::vector<std::vector<int>> adjacentAgents(3);

            enum agentType
            {
                empty = 0,
                roadrunner,
                coyote
            };

            // Make sure locations are within bounds of board
            for (int i = 0; i < 4; i++)
            {
                if (!(possibleAgents[i] < 0 || possibleAgents[i] > areaOfBoard))
                {
                    // If cell is empty, push to empty vector
                    if (board[possibleAgents[i]] == NULL)
                    {
                        adjacentAgents[empty].push_back(possibleAgents[i]); // for empty locations
                    }
                    // If cell is Roadrunner
                    else if (board[possibleAgents[i]]->toString() == "R")
                    {
                        adjacentAgents[roadrunner].push_back(possibleAgents[i]); // for roadrunner locations
                    }
                    // If cell is Coyote
                    else
                    {
                        adjacentAgents[coyote].push_back(possibleAgents[i]); // for coyote locations
                    }
                }
            }

            // Decide which behavior to execute
            for (int i = 0; i < (**rit).getNumberOfBehaviors(); i++)
            {
                switch(behaviors[i])
                {
                    // Move (roadrunner)
                    case Agent::behaviorTypes::moveRoadrunner:
                        // If there are no coyotes, move to a random open location
                        if (adjacentAgents[coyote].size() == 0)
                        {
                            #ifdef DEBUG
                            printBoard();
                            std::cout << "DEBUG ROADRUNNER" << std::endl;
                            #endif


                            if (adjacentAgents[empty].size() != 0)
                            {
                                int random = rand() % adjacentAgents[empty].size();
                                int newLocation = adjacentAgents[empty][random];
                                
                                #ifdef DEBUG
                                printBoard();
                                std::cout << newLocation << " move here " << std::endl;
                                std::cout << "we are safe " << std::endl;
                                #endif


                                board[newLocation] = *rit;

                                board[(**rit).getPosition()] = NULL;
                            }
                            printBoard();
                            
                            break;
                        }
                        

                        #ifdef DEBUG
                        std::cout << *(board[(**rit).getPosition()]) << std::endl;
                        printBoard();
                        #endif
                    // Move (coyote)
                    case Agent::behaviorTypes::moveCoyote:
                        // if adjagents[1].size > 0 , we pick random spot from rand % adjagents[1]
                        if (adjacentAgents[roadrunner].size() > 0)
                        {
                            printBoard();
                            #ifdef DEBUG
                            std::cout << "DEBUG COYOTE" << std::endl;
                            #endif
                            
                            int random = rand() % adjacentAgents[roadrunner].size();
                            int newLocation = adjacentAgents[roadrunner][random];
                            
                            #ifdef DEBUG
                            std::cout << newLocation << " coyote will kill here " << std::endl;
                            #endif
                            
                            board[newLocation] = *rit;
                            board[(**rit).getPosition()] = NULL;

                        }
                        printBoard();
                        #ifdef DEBUG
                        // std::cout << *(board[(**rit).getPosition()]) << std::endl;
                        #endif
                        break;
                    // Move panic (roadrunner)
                    case Agent::behaviorTypes::movePanic:
                        // check all perimeters of adjagents[2] (coyotes) and check for open locations

                        #ifdef DEBUG
                        // std::cout << *(board[(**rit).getPosition()]) << std::endl;
                        #endif
                        break;
                    // Die
                    case Agent::behaviorTypes::die:
                        board[(**rit).getPosition()] = NULL;
                        #ifdef DEBUG
                        // std::cout << *(board[(**rit).getPosition()]) << std::endl;
                        #endif
                        break;
                }
            }
        }
        printBoard();
        timeStep++;
    }
}