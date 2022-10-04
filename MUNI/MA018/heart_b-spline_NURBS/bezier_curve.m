function  bezier_curve(X,Y)
% function  bezier_curve(X,Y)
% Bezier curve for data [X,Y]

n=length(X)-1;
t=linspace(0,1,101);
xx=zeros(size(t));
yy=xx;
for k=0:n
    % bk - base Bernstein polynomlas
    bk=nchoosek(n,k)*poly( [zeros(1,k),ones(1,n-k)]);
    bk=bk*(-1)^(n-k);
    xx=xx+X(k+1)*polyval(bk,t);
    yy=yy+Y(k+1)*polyval(bk,t);
end

plot(X,Y,'--*',xx,yy)
end

