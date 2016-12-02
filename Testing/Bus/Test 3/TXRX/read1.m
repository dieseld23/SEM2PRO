clear all; clc
M = csvread('C1Trace00001.txt',5,0);
t = M(:,1)*1e6;
TX1 = M(:,2);
TX2 = csvread('C2Trace00001.txt',5,1);
Can_H = csvread('C3Trace00001.txt',5,1);
Can_L = csvread('C4Trace00001.txt',5,1);
clear M;
plot(t,TX1);hold on;plot(t,TX2);hold off

l=length(t);
start = 24000;
stop = 42000;
%plot(t(start:stop),TX1(start:stop));hold on;plot(t(start:stop),TX2(start:stop));hold off
title('TX and RX voltage of a CAN controller being dominated');
xlabel('Time [\mus]');
ylabel('Voltage [V]');