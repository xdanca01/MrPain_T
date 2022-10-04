function [ X,Y ] = nurbs(K,P,W,deg,t)
%function [ X,Y ] = nurbs(K,P,W,deg,t)
%  values of the nurbs curve for given knots K, points P, weights W and degree deg
%   P - matrix of 2 rows, each column is one point
%   t - points for counting, default value: linspace(K(1),K(end),101)


if nargin==4
    t=linspace(K(1),K(end),101);
end

nK=length(K);
nP=length(P);
nW=length(W);
if nP~=nW
    error('Wrong number of points or weights!');
end
if nP+deg+1~=nK
    error('Wrong number of points or knots!');
end

B=B_spline(K,deg);
Px=P(1,:);
Py=P(2,:);
X1=B_splval(B,W.*Px,t);
X2=B_splval(B,W,t);
X=X1./X2;

Y1=B_splval(B,W.*Py,t);
Y2=X2;
Y=Y1./Y2;


end

