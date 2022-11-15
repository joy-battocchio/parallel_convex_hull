import matplotlib.pyplot as plt
from scipy.spatial import ConvexHull
import numpy as np
import matplotlib.animation as animation
import time

pause = 0.02

class Point():
    def __init__(self, x, y):
        self.x = x
        self.y = y

with open('cloud.txt', 'r') as f:
    lines_cloud = f.readlines()
with open('convex_hull.txt', 'r') as f:
    lines_hull = f.readlines()
with open('steps.txt', 'r') as f:
    lines_steps = f.readlines()

cloud = []
for line in lines_cloud:
    x = (int)(line.split(';')[0])
    y = (int)(line.split(';')[1][:-1])
    cloud.append(Point(x,y))

convex_hull = []
for line in lines_hull:
    x = (int)(line.split(';')[0])
    y = (int)(line.split(';')[1][:-1])
    convex_hull.append(Point(x,y))

X_cloud = [point.x for point in cloud]
Y_cloud = [point.y for point in cloud]
X_hull = [point.x for point in convex_hull]
Y_hull = [point.y for point in convex_hull]
plt.ion()
fig = plt.figure(figsize = (8,8))
plt.scatter(X_cloud, Y_cloud,zorder = 2)
plt.pause(1)

# time.sleep(10)
f.close()

def plot_hull(hull, to_remove,n):
    segments = [] 
    #print(hull[0].x, ";", hull[0].y)
    X_hull = [point.x for point in hull]
    Y_hull = [point.y for point in hull]
    for i in range(n):
        to_remove.pop().remove()

    for i in range(len(hull)):
        segments.extend(
            plt.plot([X_hull[i], X_hull[(i+1)%len(X_hull)]],[Y_hull[i], Y_hull[(i+1)%len(Y_hull)]],'r', linestyle = "-")
        )
        plt.pause(pause)

    return to_remove+segments
        

step = 0
segments = []
while step < len(lines_steps):
    if 'START_BH' in lines_steps[step]:
        step+=1
        while 'END_BH' not in lines_steps[step]:
            x1 = (int)(lines_steps[step].split()[0].split(';')[0])
            x2 = (int)(lines_steps[step].split()[1].split(';')[0])
            y1 = (int)(lines_steps[step].split()[0].split(';')[1])
            y2 = (int)(lines_steps[step].split()[1].split(';')[1])
            segments.extend(
                plt.plot([x1,x2], [y1,y2],'r', linestyle = "-")
            )
            plt.pause(pause)
            step += 1
        step += 1
        #print("main: ",len(segments))
    elif 'START_MERGER' in lines_steps[step]:
        hull = []
        step += 1
        n = (int)(lines_steps[step][:-1]);
        step += 1
        while 'END_MERGER' not in lines_steps[step]:
            x = (int)(lines_steps[step].split(';')[0])
            y = (int)(lines_steps[step].split(';')[1][:-1])
            hull.append(Point(x,y))
            #print(x,";", y)
            step += 1
        segments = plot_hull(hull,segments,n)
        step += 1
    
for i in range(len(X_hull)):
    plt.plot([X_hull[i], X_hull[(i+1)%len(X_hull)]],[Y_hull[i], Y_hull[(i+1)%len(Y_hull)]],'r', linestyle = "-")

plt.waitforbuttonpress(0)
plt.close()
    
        

