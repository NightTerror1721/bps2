import os

def forEachFile(dir, filter, action):
    for filename in os.listdir(dir):
        filename = os.path.join(dir, filename)
        if filter(filename):
            action(filename)

