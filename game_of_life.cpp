/*This code is a simulation of Conway's Game of Life!
Rules: each cell within the box of dead cells is randomly chosen to be alive(0) or dead(.)
        if the cell is alive but next to more than 3 or less than 2 living cells, it will die
        if the cell is dead but next to 3 living cells, it will also live
        if cell is alive and next to 2 living cells, it will survive.
        if all cells die, the game exits
        The game may become stable with living cells still in it, will as user to proceed or exit
*/
#include <iostream>
#include <chrono>
#include <thread>
#include <string>

// Info for Sleep function:
// https://stackoverflow.com/questions/158585/how-do-you-add-a-timed-delay-to-a-c-program


using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;

const int WIDTH = 50;                       //set the width and height of the display
const int HEIGHT = 20;
const float init_probability_alive = 0.5;   //probability of character being dot or 0

// W => Windows
// M => Mac
// L => Linux
const char SYSTEM = 'W';                    //use windows

struct Cell{                                //set cell parameters,
    int sum = 0;                            //sum default to 0
    bool alive = false;                     //alive and will_switch set default to false
    bool will_switch = false;
};

void clear_console(){                       //set function that will not return a result to other function
    switch(SYSTEM){                         //run code parameters depending on console type
        case 'W':
            system("cls");
            break;
        case 'M':
        case 'L':
            std::cout << "\x1B[2J\x1B[H";
            break;
        default:
            break;
    }
}

bool is_init_alive(){
                                            // Get a random value between 0 and 1
    double r = ((double) rand() / (RAND_MAX));
    if (r < init_probability_alive){        //if value is less than the probability alive, set to true
        return true;
    }
    // There's no else statement here, will this still work? Yes, loop will exit and return false
    return false;
}

void init_game(Cell cell[HEIGHT][WIDTH]){   //start game!
    for(int i = 0; i < HEIGHT; i++){        //repeat for all rows
        for(int j = 0; j < WIDTH; j++){     //evaluate each cell in row
            bool is_alive = false;          //set cell to be dead
            if(i > 0 && i < HEIGHT-1 && j > 0 && j < WIDTH-1){ //is cell is not on perimeter of box, run function is_init_alive
                is_alive = is_init_alive();
            }
            cell[i][j].alive = is_alive;    //all cells on perimeter are dead
        }
    }
}

int main() {
    std::cout << "game start, type a number" << std::endl;  //ask user for value to initiate random seed
    int temp_seed;                                          //initiate variables
    char temp;
    std::cin >> temp_seed;
    srand(temp_seed);
    unsigned int round_count = 1;                           //initiate round count to 1
    Cell cell[HEIGHT][WIDTH];                               //create cell using defined struct and redefine parameters using width and height
    init_game(cell);                                        //call function "init_game" with parameters set by cell
    bool game_running = true;                               //game is running
    // Allow user to exit after x rounds
    int round_pause = 200;                                  //break the loop after 200 iterations
    while(game_running){
        // Draw the grid
        bool all_dead = true;                               //set all cells to dead initially
        for(int i = 0; i < HEIGHT; i++){                    //for all the rows, execute code starting at 0
            for(int j = 0; j < WIDTH; j++){                 //for all the cells in a row, execute code starting at 0
                if(cell[i][j].alive){                       //if this cell is alive
                    all_dead = false;                       //then all_dead is false
                }
                if(i > 0 && i < HEIGHT-1 && j > 0 && j < WIDTH-1){  //if the cell is within the window set by the width and height, continue
                    int sum = 0;                            //initiate sum set to 0, aka number of alive cells
                    //for each cell, check if the currounding 8 cells are alive or dead
                    for(int k = -1; k <= 1; k++) {          //initiate k set to -1 and while k is less than or equal to 1, execute code and add 1 to k
                        for (int m = -1; m <= 1; m++) {     //initiate m at -1, while m is greater than or equal to 1, execute code

                            if(k == 0 && m ==0){            // Check to make sure [i][j] isn't self
                                continue;
                            }
                            if(cell[i+k][j+m].alive){       //if cell at this postition is alive, add 1 to sum
                                sum += 1;
                            }
                        }
                    }
                    cell[i][j].sum = sum;                   //change parameter of sum within this cell
                    if(cell[i][j].alive){                   //if this cell is alive and the sum is greater than 3 or less to 2, cell will die
                        if(sum > 3 || sum < 2){
                            cell[i][j].will_switch = true;
                        }
                    }else{                                  //if cell is dead but sum is equal to 3, cell will be alive
                        if(sum == 3){
                            cell[i][j].will_switch = true;
                        }
                    }
                }
            }
        }

        std::string curr_line;
        for(int i = 0; i < HEIGHT; i++){            //for all values in frame, execute code
            for(int j = 0; j < WIDTH; j++){
                if(cell[i][j].alive){               //if cell is alive, display 0
                    curr_line += "0 ";
//                    std::cout << cell[i][j].sum << " "; for debuggging, ignore
                }else{                              //if dead, display .
                    curr_line += ". ";
                }
                if (cell[i][j].will_switch){        //if cell will switch, and cell is alive (0), become .
                    cell[i][j].alive = !cell[i][j].alive;
                    cell[i][j].will_switch = false; //if cell will switch but is already dead, remain dead
                }
            }
            curr_line += '\n';                      //after going through all cells in a row, go to next line
        }

        clear_console();                            //execute clear_console
//        sleep_until(system_clock::now() + 50ms);
        std::cout << curr_line << std::endl;        //print line

        sleep_until(system_clock::now() + 50ms);
        if(all_dead){                               // if all cells are dead, print the following
            std::cout << "All life has been exterminated. Good job, hero." << std::endl;
            printf("It survived for %d rounds. Continue? Y/N",round_count);
            std::cin >> temp;                       //if user enters N, end. else, restart
            if(temp == 'N' || temp == 'n'){
                printf("Good call. See ya.");
                game_running = false;
            }else{
                init_game(cell);
            }
        }
        if(round_count % round_pause == 0){         //if the counter reaches 200 before all are dead, tell user
            printf("Paused at %d rounds. Enter n to escape, or anything else to continue.",round_count);
            std::cin >> temp;
            if(temp == 'N' || temp == 'n'){
                printf("Good call. See ya.\n");
                game_running = false;               //if user wishes to end, end code
            }
            if(temp == 'R' || temp == 'r'){         //if user enters r, start over with same seed
                init_game(cell);
                //anything else, continue
            }
        }

        round_count++;                              //increment round count
//        std::cout << rand() << std::endl; debugging purposes
    }
    return 0;
}
