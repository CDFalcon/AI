import numpy as np
from sklearn.decomposition import PCA
import matplotlib.pyplot as plt

def main():

    pca=PCA(n_components=16)
        
    rawData=[]

    for line in open("data.dat"):
       rawData.append([float(i) for i in line.split()])

    length = int((rawData[0])[0])
    MCSSteps = int((rawData[0])[1])
    startTemp = (rawData[0])[2]
    endTemp = (rawData[0])[3]
    tempInc = (rawData[0])[4]
    configs = int((rawData[0])[5])

    data=[]
    
    for i in range(1,len(rawData)-1):
        data.append(rawData[i])
        
    snapshotMatrix=[]

    row=0
    
    # For each temperature
    for t in range(0,int((endTemp-startTemp)/tempInc)+1):
        
        baseRow = t*length*configs
        
        # For each configuration
        for c in range(0,configs):

            snapshot = []
            row = baseRow + c*length

            for sRow in range(0,length-1):
                snapshot.append(data[sRow+row])

            snapshotMatrix.append(snapshot)

    print("Snapshot Matrix Length: " + str(len(snapshotMatrix)))

    print(pca.fit_transform(snapshotMatrix))
    
    

    #print(data)
            
    #plot(temperature,graph,'r.',ms=i)
        
    #xlabel('Temperature')
    #ylabel('|p1|/L')
    #show()

if __name__ == '__main__':
	main()
