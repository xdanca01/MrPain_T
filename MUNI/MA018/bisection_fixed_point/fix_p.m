function [y,it] = fix_p(f,a,b,eps,maxit)
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
    
syms x a;
expand(f(x,a));

equation=x==f(x,a);
S=solve(equation)
S(1)
S(2)

syms derivative(x,a)

end