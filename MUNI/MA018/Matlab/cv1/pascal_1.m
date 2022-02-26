function [outputArg1] = pascal_1(n)

    v = [1,zeros(1,n-1)];
    M = v;
    
    for k = 2:n
        v = v + [0,v(1:end-1)];
        M = [M;v];
    end
    outputArg1 = M;
end