function [x] = regula_falsi(a, b, f)
    precision = 0.001;
    while abs(a - b) > precision
       x = b - (f(b)*((b - a)/(f(b) - f(a))));
       if x == a || x == b
           break
       end
       if f(x) * f(a) >= 0
           a = x;
       else
           b = x;
       end
    end
end