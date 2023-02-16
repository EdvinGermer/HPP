from matplotlib import pyplot as plt
import numpy as np
from scipy.optimize import curve_fit

 




############## Raw data ##############

N = [10,20,30,40,50,60,70,80,90,100,\
     150,200,300,400,500,600,700,800,900,1000,\
     1500,2000,3000]
time = [0.005,0.006,0.008,0.011,0.016,0.020,0.026,0.033,0.041,0.049,0.125, 0.190,0.414,0.730,1.130,1.625,2.211,2.898,3.657,4.513,10.136,18.018,40.547]


############## Generate best fit line ##############


# Define the quadratic function
def quadratic_func(x, c):
    return c * x**2

# Fit the data to the quadratic function
popt, pcov = curve_fit(quadratic_func, N, time)

# Print the best-fit value for c
print("Best-fit value for c:", popt[0])



C = popt[0]
N2 = [C*el**2 for el in N]

# Create a new figure and axis object
fig, ax = plt.subplots()

# Plot the data on the axis
ax.plot(N, time,'r*', label='Measurement')
ax.plot(N, N2, 'b', label='4.5e-06*N^2')

# Settings
ax.set_xlabel('Number of planets (N)')
ax.set_ylabel('Time (s)')

ax.legend()

ax.grid(True)

ax.set_title('Time complexity for galsim.c')

# Show the plot
plt.savefig('N_time_plot.png')