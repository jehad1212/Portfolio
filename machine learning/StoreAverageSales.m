function [strAvg] = StoreAverageSales(data,storeNumber)
la = storeNumber == data.Store;
cnt = 0;
tot = 0;
for i = 1:size(la,1)
    if la(i,1) == 1
        cnt = cnt + 1;
        tot = tot + table2array(data(i,3));
    end
end
strAvg = tot/cnt;
end

