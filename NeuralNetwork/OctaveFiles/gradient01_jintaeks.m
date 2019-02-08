% Gradient test, 20190125_jintaeks
clear all; clc; close all;

x = -1:0.1:1;
y = -1:0.1:1;

[x,y] = meshgrid(x,y);
z = x .^ 2 + y .^ 2;
subplot(1,2,1);
meshc(x,y,z) % surf() for solid mesh

DZx = 2*x
DZy = 2*y;
subplot(1,2,2);
quiver(x,y,DZx, DZy);
