clear all; clc
M = csvread('C1Trace00000.txt',5,0);
t = M(:,1)*1e6;
V_stop = M(:,2);
V_start = csvread('C2Trace00000.txt',5,1);
Can_H = csvread('C3Trace00000.txt',5,1);
Can_L = csvread('C4Trace00000.txt',5,1);

clear M

hold off;
plot(t,V_stop);
hold on;
plot(t,V_start);
plot(t,Can_H);
plot(t,Can_L);

title('CAN bus voltages and start and stop pulses');
xlabel('Time [\mus]');
ylabel('Voltage [V]');
axis([t(1) t(length(t)) -2 5])

V_stop=round(V_stop/3.3)*3.3;
V_start=round(V_start/3.3)*3.3;

CAN = round(1-(Can_H-Can_L)/2);

searching = 1;
n=2;
while searching
    if V_start(n)<1
        if V_start(n-1)>1
            searching = 0;
        end
    end
    n=n+1;
end

t_start = t(n);

searching = 1;
n=2;
while searching
    if V_stop(n)>1
        if V_stop(n-1)<1
            searching = 0;
        end
    end
    n=n+1;
end

t_stop = t(n);

frame_time = t_stop-t_start

searching = 1;
n=2;
while searching
    if V_start(n)>1
        if V_start(n-1)<1
            searching = 0;
        end
    end
    n=n+1;
end

build_frame_time = t_start - t(n)



