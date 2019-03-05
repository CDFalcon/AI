import numpy as np
from sklearn.decomposition import PCA
from sklearn.preprocessing import StandardScaler
import matplotlib.pyplot as plt
import matplotlib.colors as colors
import matplotlib.cm as cm


def main():

    pca=PCA(n_components=2)
        
    rawData=[]

    for line in open("data.dat"):
       rawData.append([float(i) for i in line.split()])

    length = int((rawData[0])[0])
    MCSSteps = int((rawData[0])[1])
    startTemp = (rawData[0])[2]
    endTemp = (rawData[0])[3]
    tempInc = (rawData[0])[4]
    configs = int((rawData[0])[5])

    snapshotMatrix=[]
    
    for i in range(1,len(rawData)):
        snapshotMatrix.append(rawData[i])

    print("Snapshot Matrix Length: " + str(len(snapshotMatrix)))
    

    # Center data
    X_std = StandardScaler().fit_transform(snapshotMatrix)

    # runPCA
    Y_pca = pca.fit_transform(X_std)

    x=Y_pca[:,0]
    y=Y_pca[:,1]

    temps=[]
    for i in range(0,int((endTemp-startTemp)/tempInc)+1):
        temps.append(startTemp+i*tempInc)

    Map = plt.get_cmap('spring')
    cNorm=colors.Normalize(vmin=startTemp,vmax=endTemp)
    sm=cm.ScalarMappable(cmap=Map,norm=cNorm)

    print("########################"+
          "\nAverage values of P1 per temp:\n")
    
    # For each temperature
    for i in range(0,len(temps)):
        baseRow = i*length*configs
        
        average = 0
        for j in range(baseRow,baseRow+length*configs):
            average += abs(x[j])
        
        print("Temp: " + str(temps[i]) + ", P1: "
              + str(average/(len(x[baseRow:baseRow+length*configs]))))
        
        plt.scatter(x[baseRow:baseRow+length*configs], y[baseRow:baseRow+length*configs], s=40, alpha=.01, c=sm.to_rgba(i), label=temps[i])
    
    sm._A = []
    plt.colorbar(sm)
    plt.xlabel('P1')
    plt.ylabel('P2')
    plt.show()


if __name__ == '__main__':
	main()
