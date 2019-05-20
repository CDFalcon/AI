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
        
    # Creates colormap
    Map = plt.get_cmap('jet')
    cNorm=colors.Normalize(vmin=startTemp,vmax=endTemp)
    sm=cm.ScalarMappable(cmap=Map,norm=cNorm)

    print("########################"+
          "\nAverage values of P1 per temp:\n")

    # For each temperature, graph points according to the color map
    for i in range(0,len(temps)):
        
        baseRow = i*configs
        
        average = 0
        
        for j in range(baseRow,baseRow+configs):
            average += abs(x[j])
        
        print("Temp: " + str(temps[i]) + ", P1: "
              + str(average/(len(x[baseRow:baseRow+configs]))))
        
        #for j in range(baseRow,baseRow+configs):
            #if y[j] <= 10 and y[j] >= -10:
                #plt.scatter(x[j], y[j], s=40, alpha=0.9, c=sm.to_rgba(temps[i]), label=temps[i])

        plt.scatter(x[baseRow:baseRow+configs], y[baseRow:baseRow+configs], s=40, alpha=0.9, c=sm.to_rgba(temps[i]), label=temps[i])

    # Formats plot
    sm._A = []
    plt.colorbar(sm)
    plt.xlabel('P1')
    plt.ylabel('P2')
    plt.show()


if __name__ == '__main__':
	main()
