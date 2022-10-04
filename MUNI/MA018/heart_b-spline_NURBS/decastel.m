function [XX,YY] = decastel(X,Y,n)
% function [] = decastel(X,Y,n)
% Bezier curve using deCastejau algortihm
% n - number of subintervals in parametric space [0,1]
if nargin==2
    n=100;
end
t=linspace(0,1,n+1);
k=length(X);
XX=[];
YY=[];
for tt=t
    X1=X;
    Y1=Y;
    while length(X1)>1
        X1=(1-tt)*X1(1:end-1)+tt*X1(2:end);
        Y1=(1-tt)*Y1(1:end-1)+tt*Y1(2:end);
    end
    XX=[XX,X1];
    YY=[YY,Y1];
end
end

