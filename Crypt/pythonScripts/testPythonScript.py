import torch
import numpy as np
import os

import cpp_module

class Reward:
    def __init__(self):
        reward = 0
        last_reward = 0


def randomAction(state: list):
    action = cpp_module.Action()
    
    shouldFlipGravity = 0.0 if np.random.random() < 0.5 else 1.0
    cursorAngle = np.random.randint(0, 360)
    random_int = np.random.randint(0, 3)
    speed_control = random_int - 1.0
    shouldShoot = 0.0 if np.random.random() < 0.5 else 1.0
    
    action.setBox(shouldFlipGravity, cursorAngle, speed_control, shouldShoot)
    
    return action

def Start():
    print(testString)

# State will act as previous state of the last frame (since we just got the reward to go along with it, as well as the state for the current frame
def Update(state, reward):
    playerPosition = state.getPlayerPosition()
    score = reward.getReward()
    
    # Look at the differences between the frames, not the cumulative. This is easier to track in the python.
    reward_structure.last_reward = reward_structure.reward
    reward_structure.reward = score - reward_structure.last_reward
    
    # Get monster positions, up to 10 monsters probably (This will be limited on the python side)
    
    
    
    
    # testList = state.testPlayerPosition()
    
    # element 0: flipGravity (0.0 for don't, 1.0 for do)
    # element 1: angleOfCursor (0-360)
    # element 2: -1, 0, or 1 for slow, regular, fast speed
    # element 3: shoot (0.0 for don't, 1.0 for do)
    action = randomAction(playerPosition)
    return action

def Reset():
    pass


reward_structure = Reward()
testString = "Hello there!"