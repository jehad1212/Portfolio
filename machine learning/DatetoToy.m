function [dates] = DatetoToy(dates)
m = month(dates);
d = day(dates);
doy = (m-1)*30 + d;
dates = doy;
end

