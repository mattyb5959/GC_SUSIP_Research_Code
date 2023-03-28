import csv
import matplotlib.pyplot as plt
import pandas as pd

# Open the CSV file

data = pd.read_csv('C:\\Users\\MatyuGuerrero\\Desktop\\Python\\USIP\\SDCardVoltageConvert.csv')

gasppm = ['NOppm', 'O3ppm', 'NO2ppm', 'SO2ppm', 'H2Sppm', 'COppm']

for col in gasppm:
    
    fig, ax = plt.subplots()

    ax.plot(data[col], data.iloc[:,-1]) #For time to be on y axis use this
    #ax.plot(data.iloc[:,-1], data[col])


    ax.set_title(f"Plot of {col} vs Time")
    ax.set_xlabel(col)
    ax.set_ylabel("time")
    #ax.set_title(f"Plot of Time vs {col}")
    #ax.set_xlabel("time")
    #ax.set_ylabel(col)


    plt.show()


#SO2ppm = Vadc89_SO2mV*256
#H2Sppm = Vadc1011_H2SmV*980
#COppm = Vadc1213_COmV * 176
#NOppm = Vadc01_NOmV * 280
#O3ppm = Vadc23_O3mV * 186.5
#NO2ppm = Vadc45_NO2mV * 127.05  
