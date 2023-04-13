function [dsdT] = ChangeinSales(la,data)
ds = zeros(1,sum(la));
dT = zeros(1,sum(la));
count = 1;
for i=1:size(la,1)
    if la(i,1) == 1
        count = count+1;
        tmp = data(i-1,3);
        tmp = table2array(tmp);
        ds(1,count) = table2array(data(i,3));
        ds(1,count) = ds(1,count)-tmp; 
        tmp = table2array(data(i-1,2));
        dT(1,count) = table2array(data(i,2));
        dT(1,count) = dT(1,count) - tmp;
    end
end       
dsdT = ds./dT;



end

% ds = zeros(1,sum(la));
% dT = zeros(1,sum(la));
% count = 1;
% for i=1:size(la,1)
%     if la(i,1) == 1
%         count = count+1;
%         tmp = data(i-1,3);
%         tmp = table2array(tmp);
%         ds(1,count) = table2array(data(i,3));
%         ds(1,count) = ds(1,count)-tmp; 
%         tmp = table2array(data(i-1,2));
%         dT(1,count) = table2array(data(i,2));
%         dT(1,count) = dT(1,count) - tmp;
%     end
% end       
% dsdT = ds./dT;