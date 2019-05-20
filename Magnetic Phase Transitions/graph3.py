import numpy as np
from sklearn.decomposition import PCA
from sklearn.preprocessing import StandardScaler
import matplotlib.pyplot as plt
import matplotlib.colors as colors
import matplotlib.cm as cm


def main():
        
    rawData=[]

    # Loads data
    for line in open("data.dat"):
       rawData.append([float(i) for i in line.split()])

    # Grabs data info from first row of data file
    length = int((rawData[0])[0])
    MCSSteps = int((rawData[0])[1])
    startTemp = (rawData[0])[2]
    endTemp = (rawData[0])[3]
    tempInc = (rawData[0])[4]
    configs = int((rawData[0])[5])
    pca=PCA(n_components=length)

    snapshotMatrix=[]

    # Stripes top row and creates a new matrix with just the data
    for i in range(1,len(rawData)):
        snapshotMatrix.append(rawData[i])

    print("Snapshot Matrix Length: " + str(len(snapshotMatrix)))
    
    # Centers data
    X_std = StandardScaler().fit_transform(snapshotMatrix)

    # runs pca AND projects onto data
    Y_pca = pca.fit_transform(X_std)

    #p1
    x = Y_pca[:,0]
    #p2
    y = Y_pca[:,1]

    # Temperature range
    temps=[]
    for i in range(0,int((endTemp-startTemp)/tempInc)+1):
        temps.append(startTemp+i*tempInc)
    

    print("########################"+
          "\nAverage values of P1 per temp:\n")

    averageP2 = []
    
    # For each temperature, graph points according to the color map
    for i in range(0,len(temps)):
        
        baseRow = i*configs
        
        average = 0
        
        for j in range(baseRow,baseRow+configs):
            average += abs(y[j])
        averageP2.append((average/(len(y[baseRow:baseRow+configs]))))
        
        print("Temp: " + str(temps[i]) + ", P1: "
              + str(average/(len(y[baseRow:baseRow+configs]))))

    plt.scatter(temps, averageP2)
    
    # Formats plot
    plt.xlabel('Temp')
    plt.ylabel('|P2|')
    plt.show()


if __name__ == '__main__':
	main()
