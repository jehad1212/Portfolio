%% Statistical Analysis

% ELEC2103 Assignment Semester 2 2020

%% Students Details
clear;
NAME = "Jehad Ibrahim";
SID = "500700884";
Contribution_Percentage_1= 100 ; % Indicate your contribution percentage

NAME2 = "";
SID2 = "";
Contribution_Percentage_2= 0 ; % Indicate your contribution percentage

DATE = "Oct.11";



%% Part 1

%  1.                
% Having empty shelves during times of high demand, and having empty shelves
% during times of low demand can be a huge loss to grocery
% stores like Walmart. It is often hard to predict whether
% demand will rise or drop during a holiday season, So I
% developed a machine learning algorithm that can accurately predict whether the demand for goods in Walmart
% will increase or decrease during different holiday seasons throughout the year by taking into account
% outdoor temperature, feul price, CPI and Unemployment rates.


%   2. 
% The source of the data: https://www.kaggle.com/rutuspatel/retail-analysis-with-walmart-sales-data
% The data provides Weekly Sales, Outdoor Temperature, Fuel Price, CPI, and Unemployment reates
% all as type Double. Store number as an Integer (1 through 45), and
% Holiday Flag as an integer (0 and 1), for this analysis I will only be
% examining holiday seasons (Flag = 1). 
% Lastly, the Date is loaded onto Matlab as a date-time value. 



%% Part 2

%%% Section 1



fileName = "Walmart_sales.xlsx";
opts = detectImportOptions(fileName);
opts.VariableTypes{:,2} = 'datetime';
data = readtable(fileName,opts);


%%% Section Two


%pre processing data
data.Date = DatetoToy(data.Date);   % custom function call
la = data.Holiday_Flag == 1;
dsdT = ChangeinSales(la,data);  % custom function call
incSales = dsdT > 0;
for i=1:length(incSales)-1
    incSales(1,i) = incSales(1,i+1);
end
feat = data(la,:);
feat.incSales = categorical(incSales(1:end-1)');
storeavgsales = zeros(1,45);
for i = 1:size(storeavgsales,2)
    storeavgsales(1,i) = StoreAverageSales(data,i);  % custom function call
end
feat.highavg = zeros(450,1);
for i = 1:45
    la = feat.Store == i;
    feat(la,10) = array2table(table2array(feat(la,3))-storeavgsales(1,i));
end
la = feat.Store < 23;
traindata = feat(la,:);
testdata = feat(~la,:);

% In the processing stage the datetime values were converted into a value
% 1-365 (day of Year). Two new categories were introduced: 

%incSales, which is what will be predicted. True means the sales will
%increase, False means the sales will decrease.

% highavg: the difference between the weekly sales in a given week, 
% and the average weekly for that store. 



% Knn classification model is used because classes can be seperated into
% neighboring regions when predictors are plotted against each other as
% shown in the figures below. 
figure
gscatter(feat.highavg,feat.Weekly_Sales, feat.incSales)
xlabel("difference in Sales from the Average");
ylabel("Weekly Sales");
title('Neighboring Regions between Weekly Sales and Difference in Sales from the Average');
figure
gscatter(feat.Date, feat.Weekly_Sales, feat.incSales)
xlabel("Date");
ylabel("Weekly Sales");
title('Neighboring Regions Between Sales and Time of Year');
figure
gscatter(feat.Temperature,feat.highavg, feat.incSales)
xlabel("Temperature");
ylabel("difference in Sales from the Average");
title('Neighboring Regions Between Sales and Temperature');








% Training data
mdl = fitcknn(traindata, "incSales", "NumNeighbors",3);







%Percentage Accuracy of the model on the data.

ypred = predict(mdl, traindata);
yactual = traindata.incSales;

percentage = ypred == yactual;
percentage = 100*(sum(percentage(:,1))/size(percentage(:,1),1))


%Confusion matrix demonstrating which areas are commonly unaccurately
%predicted by the model

figure
cm = confusionchart(yactual, ypred);
cm.RowSummary = "row-normalized";
cm.ColumnSummary = "column-normalized";
title("Accuracy: " + string(percentage));










%% Part 3 

% A pattern Recognition neural network was used because a pattern starts to
% unfold as the weekly sales are compared to the time of year for each store.
% the figure below shows a sample of 50 data points in different stores, and 
% a general pattern appears for when the sales are likely to increase. 

x = table2array(testdata(1:50,2));
y = table2array(testdata(1:50,3));
figure
p = plot(x,y);
xlabel("Time of Year");
ylabel("Weekly Sales");
title('Pattern between Time of Year and Weekly Sales');
testdata = testdata(1:220,:);


% MATLAB’s neural network tools
net = patternnet;
net.divideParam.trainRatio = 70/100;
net.divideParam.valRatio = 15/100;
net.divideParam.testRatio = 15/100;

%preparing data
xTrain = traindata{:,vartype("numeric")}';
xTest = testdata{:, vartype("numeric")}';
yTrain = dummyvar(traindata.incSales)';
yTest = dummyvar(testdata.incSales)';
labels = categories(traindata.incSales);

% Training data
net = train(net,xTrain,yTrain);

%goodness of fit
scores = net(xTest);
[~,yNum] = max(scores);
yPred = categorical(yNum,1:2,labels)';
yAcc = testdata.incSales;
Accuracy = 100*(sum(yAcc == yPred)./ length(yPred))


%Confusion matrix demonstrating which areas are commonly unaccurately
%predicted by the model
figure
c = confusionchart(yAcc,yPred);
c.RowSummary = "row-normalized";
c.ColumnSummary = "column-normalized";
title("Accuracy: " +string(Accuracy))









%% University of Sydney Statement Concerning Plagiarism

% I certify that:

% (1) I have read and understood the University of Sydney Academic 
% Dishonesty and Plagiarism Policy  (found by following the  Academic 
% Honesty link on the Unit web page).';

% (2) I understand that failure to comply with the Student Plagiarism: 
% Coursework Policy and Procedure can lead to the University commencing 
% proceedings against me for potential student misconduct under Chapter 8 
% of the University of Sydney By-Law 1999 (as amended).';

% (3) This Work is substantially my own, and to the extent that any part 
% of this Work is not my own I have indicated that it is not my own by 
% Acknowledging the Source of that part or those parts of the Work as 
% described in the assignment instructions.';

% NAME: Jehad A Ibrahim
% SID: 500700884

% NAME2: 
% SID2: 

% DATE: 