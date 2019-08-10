load data.dat

% basic info
length = data(1,1)
steps = data(1,2)
startTemp = data(1,3)
endTemp = data(1,4)
tempInc = data(1,5)
configurations = data(1,6)

% format for the data file: '%f %f %f etc %f\n'
format = string('%f %f %f %f %f %f\n');
%for i = 2:1:length
    %format = format + ' %f';
%end
%format = format + '\n';

% format for the combined data file
format2 = string('%f\n');

% file = fopen('pcaData.dat','w');

file1 = fopen('p1.dat','w');
file2 = fopen('p2.dat','w');
file3 = fopen('settings.dat','w');

% writes basic info to data file
%fprintf(file1,format,data(1,1:length));
% important: there will be a few blank lines at the start of the file
%fprintf(file2,format2,data(1,1:length));

firstRow = 1;
rowNumber = double(0.0);

array=data(2:configurations*length*((endTemp-startTemp)/tempInc)+2,1:length);

pcaArray = pca(array)

% for every temperature
for i = 0:1:((endTemp-startTemp)/tempInc)
    baseRow = i*length*configurations+1;
    
    % for every configuration
    for j = 0:1:configurations-1
        rowNumber = baseRow + j*length;
        
        for k = 0:1:length-1
            snapshot = array(rowNumber+k,1:length);
            p1Array{rowNumber+k,1} = snapshot*pcaArray(1:length,1); 
            p2Array{rowNumber+k,1} = snapshot*pcaArray(1:length,2); 
        end
        
        % ColumnStart:ColumnEnd,RowStart:RowEnd
        % snapshot = array(rowNumber:(rowNumber+length)-1,1:lAength);
        
        % newArray{i*(configurations)+j+1,1} = array*pcaArray;
    end
end

fprintf(file1,format2,cell2mat(p1Array));
fprintf(file2,format2,cell2mat(p2Array));
fprintf(file3,format,data(1,1:6));

fclose(file1);
fclose(file2);
