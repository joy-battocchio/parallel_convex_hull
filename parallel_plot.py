import matplotlib.pyplot as plt
from scipy.spatial import ConvexHull
import numpy as np
import matplotlib.animation as animation
import time
import os
import math

class Point():
    def __init__(self, x, y):
        self.x = x
        self.y = y

class Process():
    def __init__(self, id, lines):
        self.id = id
        self.lines = lines
        self.status = 0
        self.segments = []
        self.step = 0

pause = 0.2

path = "output/"
p_list = []
n_ps = 0
dir_list = os.listdir(path)
for file in dir_list:
    with open(path+file, 'r') as f:
        if "cloud" in file:
            lines_cloud = f.readlines()
        else:
            p_list.append(Process(file[7:-4],f.readlines()))
            n_ps += 1
        f.close()

cloud = []

for line in lines_cloud:
    x = (int)(line.split(';')[0])
    y = (int)(line.split(';')[1][:-1])
    cloud.append(Point(x,y))

# convex_hull = []
# for line in lines_hull:
#     x = (int)(line.split(';')[0])
#     y = (int)(line.split(';')[1][:-1])
#     convex_hull.append(Point(x,y))

X_cloud = [point.x for point in cloud]
Y_cloud = [point.y for point in cloud]
# X_hull = [point.x for point in convex_hull]
# Y_hull = [point.y for point in convex_hull]
plt.ion()
fig = plt.figure(figsize = (8,8))
plt.scatter(X_cloud, Y_cloud,zorder = 2)
plt.pause(1)

# time.sleep(10)

#until all the processes are done
while sum([p.status for p in p_list]) != n_ps:
    for p in p_list:
        if p.status != 1:
            line = p.lines[p.step]
            if "BH" in line or "MERGER" in line:
                pass
            elif "###" in line:
                p.status = 1
                p.step -= 1
            elif "#" in line:
                n = (int)(line.split()[1]) 
                for i in range(n):
                    p.segments.pop().remove()
            else:
                x1 = (int)(p.lines[p.step].split()[0].split(';')[0])
                x2 = (int)(p.lines[p.step].split()[1].split(';')[0])
                y1 = (int)(p.lines[p.step].split()[0].split(';')[1])
                y2 = (int)(p.lines[p.step].split()[1].split(';')[1])
                p.segments.extend(
                    plt.plot([x1,x2], [y1,y2],'r', linestyle = "-")
                )
        p.step += 1
    plt.pause(pause)

h = math.log(n_ps, 2)

for i in range(h):
    for p in p_list:
        p.status = 0
        if (p.id + 1) % 2**i == 0:
            p.segments = p_list[p.id - 2**(i-1)] + p.segments
            p.step += 2
            n = line.split()[1] 
            for i in range(n):
                p.segments.pop().remove()
            p.step += 1
        else:
            p.status = 1
    while sum([p.status for p in p_list]) != n_ps: 
        for p in p_list:
            if p.status != 1:
                line = p.lines[p.step]
                if "END_MERGER" not in line:
                    x1 = (int)(p.lines[p.step].split()[0].split(';')[0])
                    x2 = (int)(p.lines[p.step].split()[1].split(';')[0])
                    y1 = (int)(p.lines[p.step].split()[0].split(';')[1])
                    y2 = (int)(p.lines[p.step].split()[1].split(';')[1])
                    p.segments.extend(
                        plt.plot([x1,x2], [y1,y2],'r', linestyle = "-")
                    )
                else:
                    p.status = 1
                p.step += 1

plt.waitforbuttonpress(0)
plt.close()