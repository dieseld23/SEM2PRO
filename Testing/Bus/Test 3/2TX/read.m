clear all; clc
M = csvread('C1Trace00000.txt',5,0);
t = M(:,1)*1e6;
TX1 = M(:,2);
TX2 = csvread('C2Trace00000.txt',5,1);
Can_H = csvread('C3Trace00000.txt',5,1);
Can_L = csvread('C4Trace00000.txt',5,1);
clear M;
%plot(t(930:17500),TX1(930:17500));hold on;plot(t(930:17500),TX2(930:17500));hold off

l=length(t);
start = 24000;
stop = 42000;
plot(t(start:stop),TX1(start:stop));hold on;plot(t(start:stop),TX2(start:stop));hold off
title('TX voltage outputs of two CAN controllers');
xlabel('Time [\mus]');
ylabel('Voltage [V]');