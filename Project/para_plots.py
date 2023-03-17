from matplotlib import pyplot as plt
import numpy as np
from scipy.optimize import curve_fit


############## Raw data ##############

#nThreads = [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32] 
#nThreads = [1,2,4,8,16]
nThreads = [1,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30]

time_2000 = [4.119811, 2.407705, 1.190151, 0.736434, 0.566619, 0.483713, 0.490196, 0.479882, 0.495783, 0.496993, 0.471686, 0.482028, 0.462021, 0.460418, 0.471775, 0.476243]
time_3000 = [14.176307, 8.349568, 4.999415, 3.982785, 3.435965, 2.875515, 2.504198, 2.342051, 2.506126, 2.313839, 2.299025, 2.170930, 2.060212, 2.100732, 2.087087, 1.947021]
time_4000 = [34.092740, 19.112144, 13.175631, 10.131447, 10.241565, 8.537508, 7.793829, 7.606823, 7.337397, 7.849623, 7.732726, 7.439480, 7.205738, 6.923260, 7.236026, 7.080892]

time_2000_ideal = [time_2000[0]/n for n in nThreads]
time_3000_ideal = [time_3000[0]/n for n in nThreads]
time_4000_ideal = [time_4000[0]/n for n in nThreads]


# Create a new figure and axis object
fig, ax = plt.subplots()

# Plot the data on the axis
ax.plot(nThreads, time_4000, 'r', label='N=4000')
ax.plot(nThreads, time_4000_ideal, 'r--', label='N=4000 (ideal)')

ax.plot(nThreads, time_3000, 'b', label='N=3000')
ax.plot(nThreads, time_3000_ideal, 'b--',  label='N=3000 (ideal)')

ax.plot(nThreads, time_2000, 'g', label='N=2000')
ax.plot(nThreads, time_2000_ideal,'g--',  label='N=2000 (ideal)')

# Settings
ax.set_xlabel('Number of threads')
ax.set_ylabel('Time (s)')

ax.legend()

ax.grid(True)

ax.set_title('Parallelisation')

# Show the plot
plt.savefig('para_measurements.png')