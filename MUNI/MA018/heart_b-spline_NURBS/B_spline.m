function B=B_spline(knots,deg)
% function B=B_spline(knots,deg)
% b-spline of given degree for given knots

l=length(knots);
B=[];
for k=1:l-deg-1
    t=knots(k:k+deg+1);
    b=bspline(t);
    B=[B,b];
end
