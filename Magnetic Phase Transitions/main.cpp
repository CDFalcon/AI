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

// Globals because I am lazy
const int length = 4;                // Rectangular Lattice length
const int mcs_max = 1000000;            // Maximum reps for the Monte Carlo sim
const double startTemp = 0.1;        // Starting temperature for the sim
const double endTemp = 5.0;          // Final temperature for the sim
const double tempInc = 0.1;          // Temperature increment
const string fileName = "data.dat";  // File name for data storage

int lattice[length][length];         // Lattice Array
mt19937 gen(time(0));                // mersenne twister seeded to time = system

//##############################\\

int randomInt(int start, int end){
    uniform_int_distribution<> dis(start, end);
    return dis(gen);
}

double randomDouble(double start, double end){
    uniform_real_distribution<> dis(start, end);
    return dis(gen);
}

void writeData(string file, int arrayLength, double array[arrayLength]){
    ofstream f(file);

    if(f.is_open()){
        cout<<"#------------------------------#\nWriting data to " << file << endl;
        // Stores basic data at the top of the .dat file
        f << length << endl << mcs_max << endl
            << startTemp << endl << endTemp << endl
            << tempInc << endl;

        for (int i = 0; i < arrayLength - 1; i++) {
            f << array[i] << endl;
        }
    }
    f.close();
}

//##############################\\

// Fills the lattice with random spins: either 1 or -1
void NewLattice()
{
    for (int x=0; x<length; x++){
        for(int y=0; y<length; y++){
            int initNum = randomInt(0,1);
            if(initNum==0)
                lattice[x][y]=1;
            if(initNum==1)
                lattice[x][y]=-1;
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

double LatticeSpin(){
    double net_lattice = 0;

    for (int x=0; x<length; x++){
        for(int y=0; y<length; y++){
            net_lattice += double(lattice[x][y]);
        }
    }
    return net_lattice;
}

int main(int argc, char *argv[])
{
    double timeElapsed=0;     // Timer

    cout << "Simulation starting...\n";

    double dataArray[int((endTemp-startTemp)/tempInc)];

    for(double temp = startTemp; temp <= endTemp; temp += tempInc) {

        NewLattice();

        // Start timer
        clock_t start = clock();
        double avgSpin = 0.0;

        // Loops for the total number of Monte Carlo's specified
        for (int i = 0; i <= mcs_max; i++) {
            // Loop once for each lattice position, or length^2
            for (int j = 0; j < (length * length); j++) {
                Flip(randomInt(0, (length - 1)), randomInt(0, (length - 1)), temp);
            }

            // Checks to see if the values fall in the last 10% of mcs loop,
            // if so then it computes lattice spin
            if (i >= (mcs_max * .90) && (i <= mcs_max)) {
                double net_lattice = LatticeSpin();
                // Finds the average spin for each point in the lattice
                // Note: for a smooth curve, use
                // avgSpin += fabs(net_lattice / (length * length));
                avgSpin += fabs(net_lattice / (length * length));
            }
        }

        double duration = (clock() - start) / (double) CLOCKS_PER_SEC;
        timeElapsed += duration;

        // Prints data for each temperature increase
        cout << "#------------------------------#\nAverage Spin: "
             << avgSpin/(mcs_max*.1) << "\nTemperature: "
             << temp << "\nDuration: " << duration << "(s)\nEst. Time Remaining: "
             // Calculates the est. time remaining in minutes by using the total time
             << timeElapsed/(temp)*(endTemp-temp)/60 << "(min)\n";

        // Stores the data. Note: the average spin is only for the last 10% of the MCS
        dataArray[int(temp/tempInc)-1] = avgSpin/(mcs_max*.1);
    }

    writeData(fileName, int((endTemp-startTemp)/tempInc), dataArray);

    cout << "#------------------------------#\nSimulation finished, time elapsed: " << timeElapsed/60 << "(min)";
}
