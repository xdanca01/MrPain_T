function [y,it] = bisection(f,a,b,eps,maxit)
% [y,z] = bisection(f,a,b,eps,maxit)
% find root using bisection method
% y.... estimated root
% z.... total number of used iterations
% f.... anonymous function
% a.... left border of interval
% b.... right border of interval
% eps.. error
% maxit... max number of interations

if nargin==3
    eps=0.000001;
    maxit=100;
elseif nargin==4
    maxit=100;
elseif nargin<3
    error('Not enought input argument')
end

fa = f(a);
fb = f(b); 

it = 0; 

if fa*fb >= 0
    error('Wrong interval');
end

%ADD YOUR CODE
while (abs(fa-fb) > eps) && (it < maxit)
    it = 1 + it;
    y = (b-a)/2+a;
    fy = f(y);
    if fy*fa > 0
        fa = fy;
        a = y;
    else
        fb = fy;
        b = y;
    end
end
