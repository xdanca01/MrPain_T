function [x] = newton_m1 (x, f, df)
    x_prev = Inf;
    precision = 0.1;
    while abs(x - x_prev) > precision
        x_prev = x;
        x = x - (f(x)/df(x));
    end
end