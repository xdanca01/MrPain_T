function [x, iter] = golden(f, a, b, error)

g = (1 + sqrt(5))/2;
c = b - (b - a) / g;
d = a + (b - a) / g;
iter = 0;
while abs(b - a) > error
    if f(c) < f(d)
        b = d;
    else
        a = c;
    end
    c = b - (b - a) / g;
    d = a + (b - a) / g;
    iter = iter + 1;
end
    x = (b + a)/2;
end