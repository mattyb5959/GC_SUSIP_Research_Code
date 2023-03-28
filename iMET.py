import csv

#Sorry future USIPs not the best code but it works
#Matthieu Guerrero ~ 

concatlist = []

#obviously use your filepath to read and write
with open('C:\\Users\\MatyuGuerrero\\Desktop\\Python\\USIP\\IMetXdata.txt', 'r') as fileref, open('C:\\Users\\MatyuGuerrero\\Desktop\\Python\\USIP\\IMetConversion.csv', 'w', newline = '') as csvref:
    writer = csv.writer(csvref)
    
    #header_rows = ["Hex2String: ", "Decimal sens1: ", "Decimal sens2: ", "Decimal sens3: ", "Decimal Unix", "Decimal loop counter: ", "Decimal crc check: "]
    #writer.writerow(header_rows)
    
    for line in fileref:
        data = line.split()
        third_array = data[2]

        third_array = third_array.replace(' ', '')
        if third_array.startswith("0103"):
            decimalsens1 = int(third_array[10:18], 16)
            decimalsens2 = int(third_array[18:26], 16)
            decimalsens3 = int(third_array[26:34], 16)
            decimalunix = int(third_array[34:42], 16)
            decimalloopc = int(third_array[42:46], 16)
            decimalcrccheck = int(third_array[46:], 16)

            stringlist = ["Hex2String: ", "Decimal sens1: ", "Decimal sens2: ", "Decimal sens3: ", "Decimal Unix", "Decimal loop counter: ", "Decimal crc check: "]
            intlist = [third_array, decimalsens1, decimalsens2, decimalsens3, decimalunix,decimalloopc,decimalcrccheck]
            
            concatlist = [None] * (len(stringlist)+len(intlist))
            concatlist[::2] = stringlist
            concatlist[1::2] = intlist
                
            concatlist = stringlist+intlist

            print(concatlist)
            writer.writerow(concatlist)
        else:

            stringlist = ["Hexstring does not start with '0103'"]
            intlist = [third_array]
            
            concatlist = stringlist + intlist
            
            print(concatlist)
            writer.writerow(concatlist)

#if ((decimalsens1 & 0x8000) or (decimalsens2 & 0x8000) or (decimalsens3 & 0x8000) or (decimalunix & 0x8000) or (decimalloopc & 0x8000) or (decimalcrccheck & 0x8000)):
            #decimalsens1 = -(decimalsens1 & 0x7FFF)
            #decimalsens2 = -(decimalsens2 & 0x7FFF)
            #decimalsens3 = -(decimalsens3 & 0x7FFF)
            #decimalunix = -(decimalunix & 0x7FFF)
            #decimalloopc = -(decimalloopc & 0x7FFF)
            #decimalcrccheck = -(decimalcrccheck & 0x7FFF)