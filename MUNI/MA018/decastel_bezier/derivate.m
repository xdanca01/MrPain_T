function [df] = derivate(f)
%[df] = derivative(f)
%   f anonymous function
df=matlabFunction(diff(sym(f)));
end