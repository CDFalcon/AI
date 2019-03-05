load data.dat

% basic info
length = data(1,1)
steps = data(1,2)
startTemp = data(1,3)
endTemp = data(1,4)
tempInc = data(1,5)
configurations = data(1,6)

% format for the data file: '%f %f %f etc %f\n'
format = string('%f');
for i = 2:1:length
    format = format + ' %f';
end
format = format + '\n';

% format for the combined data file
format2 = string('%f\n');

file = fopen('pcaData.dat','w');

file2 = fopen('combined.dat','w');

% writes basic info to data file
fprintf(file,format,data(1,1:length));
% important: there will be a few blank lines at the start of the file
fprintf(file2,format2,data(1,1:length));

firstRow = 2;
rowNumber = double(0.0);

% for every temperature
for i = 0:1:((endTemp-startTemp)/tempInc)
    baseRow = firstRow + i*length*configurations;
    
    % for every configuration
    for j = 0:1:configurations-1
        rowNumber = baseRow + j*length;
        
        % ColumnStart:ColumnEnd,RowStart:RowEnd
        array = data(rowNumber:(rowNumber+length)-1,1:length);
        
        newArray{i*(configurations)+j+1,1} = array;
        
        %pcaArray = pca(array);
        %fprintf(file,format,pcaArray);
        %fprintf(file2,format2,array(1,1:8)*pcaArray(1:8,1));
    end
end

finalArray=cell2mat(newArray);

%mapcaplot(finalArray)

fclose(file);