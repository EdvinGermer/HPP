from matplotlib import pyplot as plt
import numpy as np
from scipy.optimize import curve_fit


############## Raw data ##############

N = [100,200,300,400,500,600,700,800,900,1000,\
    1100,1200,1300,1400,1500,1600,1700,1800,1900,2000,\
    2100,2200,2300,2400,2500,2600,2700,2800,2900,3000,\
    3100,3200,3300,3400,3500,3600,3700,3800,3900,4000]

time = [0.000378, 0.003238, 0.011221, 0.024425, 0.040349, 0.064511, 0.099685, 0.149928, 0.216533, 0.306421, 0.432770, 0.605873, 0.808688, 1.057698, 1.336190, 1.663895, 2.028079, 2.450140, 2.912621, 3.423609, 3.983538, 4.610089, 5.287699, 6.032286, 6.847222, 7.735735, 8.679799, 9.700110, 10.797920, 11.992367, 13.247146, 14.576543, 15.988183, 17.558883, 19.188162, 20.893394, 22.710231, 24.594217, 26.561070, 28.653511]
############## Generate best fit line ##############


# Define the cubic function
def cubic_func(x, c):
    return c * x**3

# Fit the data to the cubic function
popt, pcov = curve_fit(cubic_func, N, time)

# Print the best-fit value for c
print("Best-fit value for c:", popt[0])



C = popt[0]
N3 = [C*el**3 for el in N]

# Create a new figure and axis object
fig, ax = plt.subplots()

# Plot the data on the axis
ax.plot(N, time,'r*', label='Measurement')
ax.plot(N, N3, 'b', label='4.46e-10*N^3')

# Settings
ax.set_xlabel('Matrix size N')
ax.set_ylabel('Time (s)')

ax.legend()

ax.grid(True)

ax.set_title('Time complexity for LU factorization')

# Show the plot
plt.savefig('N_time_plot.png')