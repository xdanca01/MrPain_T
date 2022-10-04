function [ y] = B_splval(B,f,t)
% function [ y] = B_splval(B,f,t)
% Values of the approximation B-spline B for function values f in points t
k=length(B);
y=zeros(size(t));
for ii=1:k
    b=B(ii);
    y1=f(ii)*bsplval(b,t);
    y=y+y1;
end

