from vpython import *
from pop import *

#--------------------#

populationSize = 150 # Changes the number of members in the population
rateP = 100 # Change to speed up/slow down animation

#--------------------#

def main():

    # Create a new population of size 100
    population = pop(populationSize)
    print("\nCurrent generation: ", population.generation)

    # Creates the walls and endpoints
    finish = sphere(color=color.red,pos=endPoint,radius=1)
    wallR = box(pos=vector(100,0,0),size=vector(0.1,200,1),color=color.green)
    wallL = box(pos=vector(-100,0,0),size=vector(0.1,200,1),color=color.green)
    wallU = box(pos=vector(0,100,0),size=vector(200,0.1,1),color=color.green)
    wallD = box(pos=vector(0,-100,0),size=vector(200,0.1,1),color=color.green)

    scene.autoscale = True

    while(True):
        rate(rateP)
        
        if population.allDone():
            population = population.nextGeneration()
            population.mutateMembers()
        else:
            population.update() 

if __name__ == '__main__':
    main()
