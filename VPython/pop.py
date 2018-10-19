from member import *

#--------------------#

class pop:
    def __init__(self, size):
        # Creates a population of members
        self.members = []
        for i in range(0, size):
            self.members.append(member())
        self.generation = 1
        self.bestMember = 0
        self.minMoves = 0
        self.size = size
    
    #Updates and displays all of the members
    def update(self):
        for i in range(0, len(self.members)-1):
            # If already greater than max steps when a ball has finished
            if self.members[i].neural.move > self.minMoves and self.members[self.bestMember].finished == True:
                self.members[i].done = True
            else:
                self.members[i].update()
    
    #Checks to see if all of the members have finished
    def allDone(self):
        for i in range(0, len(self.members)-1):
            if (not self.members[i].done) and (not self.members[i].finished):
                return False
        return True

    # Finds the best step of members
    def setBest(self):
        highScore = 0.0
        for i in range(0, len(self.members)-1):
            self.members[i].setScore()
            if self.members[i].score > highScore:
                # Remembers the new best member
                self.bestMember = i
                highScore = self.members[i].score
        if self.members[self.bestMember].finished == True:
            print("Current minimum steps: ", self.members[self.bestMember].neural.move,
                  " (minimum possible: %s)" % str((startPoint-endPoint).mag/maxDistance))
            self.minMoves = self.members[self.bestMember].score
        print("Current highest score: ", self.members[self.bestMember].score,
              " (highest possible: %s)" % str(1/((startPoint-endPoint).mag/maxDistance)))
                
    def mutateMembers(self):
        # The best member is at members[0], so it is not mutated
        for i in range(1, len(self.members)-1):
            self.members[i].neural.mutate()
            
    def nextGeneration(self):
        nextGen = pop(self.size)

        self.setBest()

        nextGen.members[0] = self.members[self.bestMember].makeClone()
        nextGen.members[0].best()
        # The move cannot be 0, as it is with a new clone
        nextGen.members[0].neural.move = self.members[self.bestMember].neural.move
        
        for i in range(1, len(self.members)-1):
            # Important algorithm: if the score of a member is less than a random number between 0 and the max score,
            # it clones itself. Otherwise, it is replaced with a clone of the best dot, which is then mutated
            r = ran.uniform(0, self.members[self.bestMember].score)
            if self.members[i].score > r:
                nextGen.members[i] = self.members[i].makeClone()
            # Surviving members change color
            else:
                nextGen.members[i] = self.members[self.bestMember].makeClone()
                nextGen.members[i].ball.color = color.cyan
                
        nextGen.members[0].neural.move = 0

        # Removes the old ball objects
        for i in range(0, len(self.members)-1):
            self.members[i].ball.visible = False
            del self.members[i].ball

        # Increases the generation
        nextGen.generation += self.generation
        print("\nCurrent generation: %s" % nextGen.generation)
        nextGen.minMoves = self.minMoves

        del self

        return nextGen

