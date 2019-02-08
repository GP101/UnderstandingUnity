x = -1:0.1:1;
y = -1:0.1:1;

[X,Y] = meshgrid(x,y);
Z = X .^ 2 + Y .^ 2;
surf(X,Y,Z)
