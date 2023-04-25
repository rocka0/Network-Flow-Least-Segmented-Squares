import pandas as pd
import matplotlib.pyplot as plt

# Read CSV file into a Pandas dataframe
data = pd.read_csv('plot.csv')

# Plot the data as a line chart
plt.plot(data['Number_Of_Points'], data['Running_Time_In_Microseconds'])

# Add labels and title to the plot
plt.xlabel('Number of Points')
plt.ylabel('Running Time (microseconds)')
plt.title('Timing Plot')

# Save the plot to a PNG file
plt.savefig('plot.png')
