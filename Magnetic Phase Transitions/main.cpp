#include <cstdlib>
#include <iostream>
#include <math.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <fstream>

//##############################\\

using namespace std;

//##############################\\
// VARS

const int length = 16;               // Rectangular Lattice length
const int mcs_max = 10000;           // Maximum reps for the Monte Carlo sim
const double startTemp = 2.4;        // Starting temperature for the sim
const double endTemp = 3.0;          // Final temperature for the sim
const double tempInc = 0.05;         // Temperature increment
const int configurations = 100;        // Number of configurations per temperature
const string fileName = "data.dat";    // File name for data storage

int lattice[length][length];         // Lattice Array
mt19937 gen(time(0));                // mersenne twister seeded to time = system

//##############################\\
// GENERAL FUNCTIONS

int randomInt(int start, int end){
    uniform_int_distribution<> dis(start, end);
    return dis(gen);
}

double randomDouble(double start, double end){
    uniform_real_distribution<> dis(start, end);
    return dis(gen);
}

void writeData(double array[int((endTemp-startTemp)/tempInc)][configurations][length][length]){
    ofstream f(fileName);

    if(f.is_open()){

        cout<<"\n#-------------------------------#\nWriting data to " << fileName << endl;

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
        for (int i = 5; i < length-1; i++)
            f << " 0";
        f << endl;

        // For each temperature
        for(double temp = startTemp; temp <= endTemp; temp += tempInc){
            // For each configuration
            for(int i=0; i < configurations; i++) {
                // For each row
                for(int y=0; y<length;y++) {
                    // For each value
                    for (int x = 0; x < length; x++) {
                        f << array[int((temp-startTemp)/tempInc)][i][x][y];
                        f << " ";
                    }
                    f << endl;
                }
            }
        }
    }

    f.close();
}

//##############################\\
// FUNCTIONS

// Fills the lattice with random spins: either 1 or -1
void NewLattice()
{
    for (int x=0; x<length; x++) {
        for (int y = 0; y < length; y++) {
            // int initNum = randomInt(0, 1);
            //if(initNum==0)
            lattice[x][y] = 1;
            //if(initNum==1)
            //    lattice[x][y]=-1;
        }
    }
}

// Performs a flip
void Flip(int x, int y, double temp)
{
    int *top, *bottom, *left, *right;
    double dEnergy; // Change in energy

    // Checks to make sure the random point is not out of the bounds of the array
    if (x==0)
        top=&lattice[length-1][y];
    else
        top=&lattice[x-1][y];
    if (x==length-1)
        bottom=&lattice[0][y];
    else
        bottom=&lattice[x+1][y];
    if (y==0)
        left=&lattice[x][length-y];
    else
        left=&lattice[x][y-1];
    if (y==length-1)
        right=&lattice[x][0];
    else
        right=&lattice[x][y+1];

    // Calculates the change in energy
    dEnergy= 2*lattice[x][y]*(*top + *bottom + *left + *right);

    // If the change in energy is negative (good), flip the spin
    if (dEnergy <= 0){
        lattice[x][y]=-1*lattice[x][y];
    }
    // Else if a random number between 0.0 and 1.0 is less than
    // e^(-1*change in energy/temperature), also flip
    else if (randomDouble(0.0,1.0) < exp((-dEnergy/temp))){
        lattice[x][y]=-1*lattice[x][y];
    }
}

void createPCAArray(double dataArray[int((endTemp-startTemp)/tempInc)][configurations][length][length]){

    /*
    double pcaArray[int((endTemp-startTemp)/tempInc)][configurations][length][length];

    // For each temperature
    for(double temp = startTemp; temp <= endTemp; temp += tempInc){
        // For each column
        for(int x=0; x<length;x++) {

            // Find the average spin
            double averageSpin = 0.0;

            // For all configurations
            for(int i=0; i < configurations; i++) {
                // For all Y values in said column
                for (int y = 0; y < length; y++) {
                    averageSpin += dataArray[int((temp-startTemp)/tempInc)][i][x][y];
                }
            }

            averageSpin = averageSpin/(length*configurations);



            cout << "\nAverage Spin for Temp. # " << (temp-startTemp)/tempInc + 1
                 << " and column # " << x+1 << " is " << averageSpin;




            // Then fill the PCA Array with the new values
            for(int i=0; i < configurations; i++) {
                for (int y = 0; y < length; y++) {
                    pcaArray[int((temp-startTemp)/tempInc)][i][x][y] =
                            (dataArray[int((temp-startTemp)/tempInc)][i][x][y]);// - averageSpin);
                }
            }
        }
    }
    */

    writeData(dataArray);
}

int main(int argc, char *argv[])
{
    double timeElapsed=0;     // Timer

    cout << "\nSimulation starting...\n";

    double dataArray[int((endTemp-startTemp)/tempInc)][configurations][length][length];

    double totalSpin = 0;

    for(double temp = startTemp; temp <= endTemp; temp += tempInc){

        // Start timer
        clock_t start = clock();
        double avgSpin = 0.0;
        // double avgSpin2 = 0.0;

        for(int i=0; i < configurations; i++) { // One lattice per config


            cout << "\n#-------------------------------#\nLattice #" << i+1
                 << " for Temp. #" << (temp-startTemp)/tempInc+1 << " out of "
                 << (endTemp-startTemp)/tempInc << "\n#-------------------------------#\n\n";


            NewLattice();

            // Loops for the total number of Monte Carlo's specified
            for (int j = 0; j <= mcs_max; j++) {
                // Loop once for each lattice position, or length^2
                for (int k = 0; k < (length * length); k++) {
                    Flip(randomInt(0, (length - 1)), randomInt(0, (length - 1)), temp);
                }

                // If this is the last snapshot, store it
                if (j == mcs_max) {
                    for (int y = 0; y < length; y++) {
                        for (int x = 0; x < length; x++) {
                            dataArray[int((temp-startTemp)/tempInc)][i][x][y] = lattice[x][y];

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
                 << "#-------------------------------#\n";
    }

    createPCAArray(dataArray);

    cout << "#-------------------------------#\nSimulation finished, time elapsed: " << timeElapsed/60 << "(min)";
}