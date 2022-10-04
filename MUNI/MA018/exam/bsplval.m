function [ y] = bsplval(b,x)
% function [ y] = bsplval(b,x)
% Values of b-spline b in points x
y=ppval(b,x);
y(x<b.breaks(1))=0;
y(x>b.breaks(end))=0;

end

