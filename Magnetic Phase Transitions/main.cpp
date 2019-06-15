#include <cstdlib>
#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <fstream>

//##############################\\

using namespace std;

//##############################\\
// VARS

const int length = 10;               // Rectangular Lattice length
const int mcs_max = 10000;          // Maximum reps for the Monte Carlo sim
const double startTemp = 2.0;        // Starting temperature for the sim
const double endTemp = 2.8;          // Final temperature for the sim
const auto tempInc = 0.05;           // Temperature increment
const int configurations = 10;    // Number of configurations per temperature
const string filename = "data.dat";  // Name of data file

// NNN vars
const double h0 = 1.0;
const double frequency = 0.001;
const double p = 0.1;


//mt19937 gen(static_cast<unsigned int>(time(nullptr)));                // mersenne twister seeded to time = system
mt19937 mt_rand(time(0));

int lattice[length][length];         // Lattice Array
int startLattice[length][length];    // Starting lattice for each new temperature

//##############################\\
// GENERAL FUNCTIONS

int randomInt(int start, int end) {
    uniform_int_distribution<> dis(start, end);
    return dis(mt_rand);
}

double randomDouble(double start, double end) {
    uniform_real_distribution<> dis(start, end);
    return dis(mt_rand);
}

//##############################\\
// FUNCTIONS

// Fills the lattice with random spins: either 1 or -1

void FirstLattice() {
    for (auto &x : lattice) {
        for (int &y : x) {
            // int initNum = randomInt(0, 1);
            //if(initNum==0)
            y = 1;
            //if(initNum==1)
            //    lattice[x][y]=-1;
        }
    }

    for (auto &x : startLattice) {
        for (int &y : x) {
            // int initNum = randomInt(0, 1);
            //if(initNum==0)
            y = 1;
            //if(initNum==1)
            //    lattice[x][y]=-1;
        }
    }
}

void NewLattice() {
    for (int x = 0; x < length; x++) {
        for (int y = 0; y < length; y++) {
            lattice[x][y] = startLattice[x][y];
        }
    }
}

// Performs a flip
void Flip(int x, int y, double temp,int MCS) {
    int *top, *bottom, *left, *right, *topRight, *bottomRight, *topLeft, *bottomLeft;
    double dEnergy; // Change in energy

    // Checks to make sure the random point is not out of the bounds of the array for NN and NNN

    if (x == 0) {
        if (y == length - 1) {
            topRight = &lattice[length - 1][0];
            topLeft = &lattice[length - 1][y - 1];
            bottomRight = &lattice[1][0];
            bottomLeft = &lattice[1][y - 1];

            right = &lattice[x][0];
            left = &lattice[x][y - 1];
        } else if (y == 0) {
            topRight = &lattice[length - 1][y + 1];
            topLeft = &lattice[length - 1][length - 1];
            bottomRight = &lattice[1][length - 1];
            bottomLeft = &lattice[1][1];

            right = &lattice[x][y + 1];
            left = &lattice[x][length - 1];
        } else {
            topRight = &lattice[length - 1][y + 1];
            topLeft = &lattice[length - 1][y - 1];
            bottomRight = &lattice[1][y + 1];
            bottomLeft = &lattice[1][y - 1];

            right = &lattice[x][y + 1];
            left = &lattice[x][y - 1];
        }

        top = &lattice[length - 1][y];
        bottom = &lattice[x + 1][y];
    } else if (x == length - 1) {
        if (y == length - 1) {
            topRight = &lattice[x - 1][0];
            topLeft = &lattice[x - 1][y - 1];
            bottomRight = &lattice[0][0];
            bottomLeft = &lattice[0][y - 1];

            right = &lattice[x][0];
            left = &lattice[x][y - 1];
        } else if (y == 0) {
            topRight = &lattice[x - 1][1];
            topLeft = &lattice[x - 1][length - 1];
            bottomRight = &lattice[0][y + 1];
            bottomLeft = &lattice[0][length - 1];

            right = &lattice[x][y + 1];
            left = &lattice[x][length - 1];
        } else {
            topRight = &lattice[x - 1][y + 1];
            topLeft = &lattice[x - 1][y - 1];
            bottomRight = &lattice[0][y + 1];
            bottomLeft = &lattice[0][y - 1];

            right = &lattice[x][y + 1];
            left = &lattice[x][y - 1];
        }

        top = &lattice[x - 1][y];
        bottom = &lattice[0][y];
    } else {
        if (y == length - 1) {
            topRight = &lattice[x - 1][0];
            topLeft = &lattice[x - 1][y - 1];
            bottomRight = &lattice[x + 1][0];
            bottomLeft = &lattice[x + 1][y - 1];

            right = &lattice[x][0];
            left = &lattice[x][y - 1];
        } else if (y == 0) {
            topRight = &lattice[x - 1][y + 1];
            topLeft = &lattice[x - 1][length - 1];
            bottomRight = &lattice[x + 1][y + 1];
            bottomLeft = &lattice[x + 1][length - 1];

            right = &lattice[x][y + 1];
            left = &lattice[x][length - 1];
        } else {
            topRight = &lattice[x - 1][y + 1];
            topLeft = &lattice[x - 1][y - 1];
            bottomRight = &lattice[x + 1][y + 1];
            bottomLeft = &lattice[x + 1][y - 1];

            top = &lattice[x - 1][y];
            bottom = &lattice[x + 1][y];
            right = &lattice[x][y + 1];
            left = &lattice[x][y - 1];
        }
    }

    // Calculates the change in energy for NN
    // dEnergy = 2 * lattice[x][y] * (*top + *bottom + *left + *right);

    // Calculates the change in energy for NN and NNN
    dEnergy = 2 * lattice[x][y] *
              ((*top + *bottom + *left + *right) + p * (*topRight + *topLeft + *bottomRight + *bottomLeft) +
               h0 * sin(2 * M_PI * frequency * MCS));

    // If the change in energy is negative (good), flip the spin
    if (dEnergy <= 0) {
        lattice[x][y] = -1 * lattice[x][y];
    }
        // Else if a random number between 0.0 and 1.0 is less than
        // e^(-1*change in energy/temperature), also flip
    else if (randomDouble(0.0, 1.0) < exp((-dEnergy / temp))) {
        lattice[x][y] = -1 * lattice[x][y];
    }
}

int main() {
    double timeElapsed = 0;     // Timer

    cout << "\nSimulation starting...\n";

    //double new dataArray[int((endTemp-startTemp)/tempInc)+1][configurations][length][length];

    double ****dataArray;
    dataArray = new double ***[int((endTemp - startTemp) / tempInc) + 1];

    for (int i = 0; i < int((endTemp - startTemp) / tempInc) + 1; i++) {
        dataArray[i] = new double **[configurations];
        for (int j = 0; j < configurations; j++) {
            dataArray[i][j] = new double *[length];
            for (int k = 0; k < length; k++)
                dataArray[i][j][k] = new double[length];
        }
    }


    double totalSpin = 0;

    FirstLattice();

    for (double temp = startTemp; temp <= endTemp; temp += tempInc) {

        // Start timer
        clock_t start = clock();
        double avgSpin = 0.0;
        // double avgSpin2 = 0.0;

        cout << "\n#-------------------------------#\nTemperature: " << (temp - startTemp) / tempInc + 1
             << " out of " << (endTemp - startTemp) / tempInc + 1
             << "\n#-------------------------------#\n\n";

        for (int i = 0; i < configurations; i++) { // One lattice per config

            /*
            cout << "\n#-------------------------------#\nLattice #" << i+1
                 << " for Temp. #" << (temp-startTemp)/tempInc+1 << " out of "
                 << (endTemp-startTemp)/tempInc << "\n#-------------------------------#\n\n";
            */

            NewLattice();

            // Loops for the total number of Monte Carlo's specified
            for (int j = 0; j <= mcs_max; j++) {
                // Loop once for each lattice position, or length^2
                for (int k = 0; k < (length * length); k++) {
                    Flip(randomInt(0, (length - 1)), randomInt(0, (length - 1)), temp, j);
                }

                // If this is the last snapshot, store it
                if (j == mcs_max) {
                    for (int y = 0; y < length; y++) {
                        for (int x = 0; x < length; x++) {
                            dataArray[int((temp - startTemp) / tempInc)][i][x][y] = lattice[x][y];

                            // cout << dataArray[int((temp-startTemp)/tempInc)][i][x][y] << ",";
                        }
                        // cout << endl;
                    }
                }
            }
        }

        double duration = (clock() - start) / (double) CLOCKS_PER_SEC;
        timeElapsed += duration;

        // Prints data for each temperature increase
        cout << "\n#-------------------------------#\nEst. Time Remaining: "
             // Time per Spin * remaining ratio of spin
             << timeElapsed / 60 / (temp - startTemp + tempInc) * (endTemp - temp) << "(min)\n"
             << "#-------------------------------#\n\n";

        for (int x = 0; x < length; x++) {
            for (int y = 0; y < length; y++) {
                startLattice[x][y] = lattice[x][y];
            }
        }
    }

    ofstream f(filename);

    if (f.is_open()) {

        cout << "\n#-------------------------------#\nWriting data to " << "data.dat" << endl;

        // Stores basic data at the top of the .dat file
        // Example Format:
        // length mcs_max startTemp endTemp tempInc configurations 0 0 0 etc

        f << length;
        f << " " << mcs_max;
        f << " " << startTemp;
        f << " " << endTemp;
        f << " " << tempInc;
        f << " " << configurations;

        // Fills the rest of the row with 0's for matlab
        for (int i = 5; i < length * length - 1; i++)
            f << " 0";
        f << endl;

        // For each temperature
        for (double temp = startTemp; temp <= endTemp; temp += tempInc) {
            // For each configuration
            for (int i = 0; i < configurations; i++) {
                // For each row
                for (int y = 0; y < length; y++) {
                    // For each value
                    for (int x = 0; x < length; x++) {
                        f << dataArray[int((temp - startTemp) / tempInc)][i][x][y];
                        f << " ";
                    }
                    // Multiple row lattice
                    // f << endl;
                }
                // One row lattice
                f << endl;
            }
        }
    }

    f.close();

    for (int i = 0; i < int((endTemp - startTemp) / tempInc) + 1; i++) {
        for (int j = 0; j < configurations; j++) {
            for (int k = 0; k < length; k++)
                delete[] dataArray[i][j][k];
            delete[] dataArray[i][j];
        }
        delete[] dataArray[i];
    }

    delete[] dataArray;

    cout << "#-------------------------------#\n\nSimulation finished, time elapsed: " << timeElapsed / 60 << "(min)\n";
}