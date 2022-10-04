function [x3] = secant(x1, f, x2)
f1 = f(x1);
f2 = f(x2);
it = 0;
maxit = 30;
precision = 0.001;
    while abs(x2 - x1) > precision && maxit > it
        %plot([x1,x2],[f1,f2],"g--*")
        it = it + 1;
        x3 = x2 - f2*(x2-x1)/(f2-f1);
        x1 = x2;
        f1 = f2;
        x2 = x3;
        f2 = f(x3);
    end
end