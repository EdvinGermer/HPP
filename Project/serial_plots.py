from matplotlib import pyplot as plt
import numpy as np
from scipy.optimize import curve_fit


############## Raw data ##############

x = ["Org", "V2", "V3", "V4", "native", "-O1",  "-O2","-O3"] 

time_500 = [0.418747, 0.360858, 0.352111, 0.328274 , 0.328439, 0.055601, 0.048345, 0.042892]
time_1000 = [3.352806, 2.838183 , 2.836800 , 2.624026 , 2.623019, 0.402483, 0.403120, 0.308635] 
time_2000 = [27.264053, 23.055847, 22.864937, 21.308515, 21.283782, 3.925879, 3.916838, 3.412250] 


# Create a new figure and axis object
fig, ax = plt.subplots()

# Plot the data on the axis
ax.plot(x, time_500, label='N=500')
ax.plot(x, time_1000, label='N=1000')
ax.plot(x, time_2000, label='N=2000')


# Settings
ax.set_ylabel('Time (s)',fontsize=14)
plt.xticks(rotation=20)
plt.yscale("log")
plt.rcParams.update({'font.size': 14})
plt.xticks(fontsize=14)
plt.yticks(fontsize=14)




ax.legend()

ax.grid(True)

ax.set_title('Serial Optimization')

# Show the plot
plt.savefig('serial_measurements.png')