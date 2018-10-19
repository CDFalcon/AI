from neural import *

#--------------------#

startPoint = vector(0,0,0) # Starting point of the dot
endPoint = vector(0,75,0) # Ending point of the finish
radiusP = 1 # Radius of the dots

#--------------------#

class member:
    def __init__(self):
        # Gives our member a brain
        self.neural = brain()
        self.neural.randomize()
        # If the member has touched a wall or run out of moves
        self.done = False
        # If the member has touched the finish
        self.finished = False
        # The member's score
        self.score = 0.0
        # The member's ball object, empty to start. really weird, but due to vpython's weird memory allocation, it needs to be a list/array
        self.ball = sphere(color=color.blue,pos=startPoint,radius=radiusP)

    # If the ball is the best, make its color green
    def best(self):
        self.ball.color = color.red
        self.ball.radius = 2*radiusP # Doesn't change the interaction with the finish line
            
    # Updates the member's position and checks to see if it has finished or not
    def update(self):
        # If moveable
        if not self.done and not self.finished:
            self.move()
            #If it is on the border, its done
            if (self.ball.pos.x >= 100 or self.ball.pos.x <= -100 or self.ball.pos.y >= 100 or self.ball.pos.y <= -100):
                self.done = True
            #If it is touching the finish, it is finished
            elif (self.ball.pos-endPoint).mag < 1:
                self.finished = True
                
    
    # Moves the member
    def move(self):
        # Checks to make sure there are still steps left in the memory. Should never reach the end
        if len(self.neural.memory)-1 > self.neural.move:
            self.neural.move += 1
        else:
            self.done = True
            return
        # Increases the position by whatever the next brain instruction is
        self.ball.pos += self.neural.memory[self.neural.move]

    # Sets the score of the member. Smaller score is better
    def setScore(self):
        if self.finished:
            # If the ball finished, the score is based on the number of moves it took
            self.score = 1/self.neural.move
        else:
            # If not touching, the score is based on the ending distance of the ball and the finish
            self.score = 1/(((self.ball.pos-endPoint).mag)*1000)
    
    #Creates an identical clone of itself
    def makeClone(self):
        clone = member()
        for i in range(0, len(self.neural.memory)-1):
            clone.neural.memory[i] = self.neural.memory[i]
        return clone
        
