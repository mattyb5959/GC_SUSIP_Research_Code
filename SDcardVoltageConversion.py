import csv


#double Vadc01 = ((adc01*2.048) / 16777216.0);
#double Vadc23 = ((adc23*2.048) / 16777216.0);
#double Vadc45 = ((adc45*2.048) / 16777216.0);
#double Vadc67 = ((adc67*2.048) / 16777216.0);
#double Vadc89 = ((adc89*2.048) / 16777216.0);
#double Vadc1011 = ((adc1011*2.048) / 16777216.0);
#double Vadc1213 = ((adc1213*2.048) / 16777216.0);
#double Vadc1415 = ((adc1415*2.048) / 16777216.0);

#obviously use your filepath to read and write
fr = 'C:\\Users\\MatyuGuerrero\\Desktop\\Python\\USIP\\SDCard.csv'
fw = 'C:\\Users\\MatyuGuerrero\\Desktop\\Python\\USIP\\SDCardVoltageConvert.csv'



with open(fr, 'r') as csvread, open(fw, 'w', newline = '') as csvwrite:
    
    csvreader = csv.reader(csvread)
    csvwriter = csv.writer(csvwrite)

    #header_rows = ['NOmV', 'O3mV', 'NO2mV', 'pt1mV', 'SO2mV', 'H2SmV', 'COmV', 'pt2mV', 'Time Since Start:']
    header_rows = ['NOppm', 'O3ppm', 'NO2ppm', 'SO2ppm', 'H2Sppm', 'COppm', 'Time Since Start:']
    csvwriter.writerow(header_rows)

    for row in csvreader:
        C1value = float(row[2]) 
        G1value = float(row[6])
        K1Value = float(row[10])
        O1Value = float(row[14])
        S1Value = float(row[18])
        W1Value = float(row[22])
        AA1Value = float(row[26])
        AF1Value = float(row[30])
        AI1Value = float(row[34])

        #Using the old Usip Code you will get a differnt graph
        #C1value = float(row[5]) 
        #G1value = float(row[8])
        #K1Value = float(row[11])
        #O1Value = float(row[14])
        #S1Value = float(row[17])
        #W1Value = float(row[20])
        #AA1Value = float(row[23])
        #AF1Value = float(row[26])
        #AI1Value = float(row[29])

        Vadc01_NOmV = abs(((C1value*2.048)/16777216.0)* 1000.0)
        Vadc23_O3mV = abs(((G1value*2.048)/16777216.0) * 1000.0) 
        Vadc45_NO2mV = abs(((K1Value*2.048)/16777216.0) * 1000.0)
        #Vadc67_pt1mV = abs(((O1Value*2.048)/16777216.0) * 1000.0)
        Vadc89_SO2mV = abs(((S1Value*2.048)/16777216.0) * 1000.0)
        Vadc1011_H2SmV = abs(((W1Value*2.048)/16777216.0) * 1000.0)
        Vadc1213_COmV = abs(((AA1Value*2.048)/16777216.0) * 1000.0)
        #Vadc1415_pt2mV = abs(((AF1Value*2.048)/16777216.0) * 1000.0)

    

        SO2ppm = (Vadc89_SO2mV*256)/100000
        H2Sppm = (Vadc1011_H2SmV*980)/100000
        COppm = (Vadc1213_COmV * 176)/100000
        NOppm = (Vadc01_NOmV * 280)/100000
        O3ppm = (Vadc23_O3mV * 186.5)/100000
        NO2ppm = (Vadc45_NO2mV * 127.05)/100000   

        #stringlist = ["NOmV:", "O3mV:", "NO2mV:", "pt1mV:", "SO2mV:", "H2SmV:", "COmV:", "pt2mV", "Time Since Start:"]
        voltage_converted = [NOppm, O3ppm, NO2ppm, SO2ppm, H2Sppm, COppm, AI1Value]
        
        #concatlist = [None] * (len(stringlist)+len(voltage_converted))
        #concatlist[::2] = stringlist
        #concatlist[1::2] = voltage_converted

        print(voltage_converted)
        csvwriter.writerow(voltage_converted)





            