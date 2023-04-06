clc;
close all;
clear all;

fs=12000;

T=5;

f0l=697;
f0h=1336;

t=0:1/fs:T;
x=0;
f_t=[697 770 852 941 1209 1336 1477 1633];
for i=1:8
    x=sin(2*pi*f_t(i)*t)+x;
end

yk = goert(x);
stem(f_t,yk);
%soundsc(x,fs);