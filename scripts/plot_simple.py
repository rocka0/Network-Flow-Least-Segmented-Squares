import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

data = pd.read_csv('data.csv', sep=' ')

centers = round((data['lower_limit'] + data['upper_limit']) / 2, 4)
xs = data['lower_limit']
Xs = data['upper_limit']
widths = round(data['upper_limit'] - data['lower_limit'], 5)
ax = sns.barplot(x=centers, y='number_of_segment', data=data, color='blue', errorbar=None, orient='v', alpha=0.5, linewidth=2)
# for i, p in enumerate(ax.patches):
#     p.set_width(widths[i])

for bar,newwidth,new_x, new_X in zip(ax.patches,widths, xs, Xs):
    x = bar.get_x()
    width = bar.get_width()
    print(new_x, new_X)
    bar.set_x(new_x)
    bar.set_width(newwidth)

ax.set_xlabel('Range')
ax.set_ylabel('Number of Segments')
ax.set_title('Histogram of Segments by Range')

plt.savefig('histogram.png', dpi=300)