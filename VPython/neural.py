from vpython import *
import random as ran

#--------------------#

memoryLength = 3000
maxDistance = 1
mutationRate = 5 # Inverse relationship, 1/mutationRate is the % of instructions that will me mutated

#--------------------#


class brain:
    def __init__(self):
        # Creates an empty memory
        self.memory = []
        for i in range(0, memoryLength):
            self.memory.append(0)
        # The current move slot the brain should be reading
        self.move = 0

    # Randomizes brain instructions (intructions in this case being random vectors)
    def randomize(self):
        for i in range(0, memoryLength):
            self.memory[i] = vector(ran.uniform(-maxDistance,maxDistance),ran.uniform(-maxDistance,maxDistance),0)

    def mutate(self):
        # Chooses a random amount of instructions to mutate
        for i in range(0, ran.randint(1,memoryLength/mutationRate)):
            # Finds a random instruction to randomize
            self.memory[ran.randint(0,memoryLength-1)] = vector(ran.uniform(-maxDistance,maxDistance),ran.uniform(-maxDistance,maxDistance),0)            
                        
    
