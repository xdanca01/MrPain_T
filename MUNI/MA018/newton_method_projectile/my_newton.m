function [x, iter] = my_newton(f, x, error)
%Newton method
x_prev=Inf;
df=derivate(f);
num=0;
iter = 0;
while abs(x-x_prev)>error
    x_prev=x;
    x=x-f(x)/df(x);
    num=num+1;
    iter = iter + 1;
end
end