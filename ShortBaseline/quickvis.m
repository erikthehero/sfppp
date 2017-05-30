clc; close all; clear all;

c=299792458;
f0=10.23e6;     
freq1   = 154*f0;
lam1    = c/freq1;
ndeg = 2;

for i = 0:47
    fn = strcat('C:\Users\Erik\Documents\sfppp\cpp\main\shortbaseline\output\out', int2str(i), '.txt');
    plotResult(fn, lam1, i)
end

function plotResult(fn, lam1, i)

data = dlmread(fn);

% remove small segments
if (length(data)<1200)
    fprintf('len < 1200\n');
    return;
end

obs=data(:,4)-data(:,5).*lam1;
t = GpsToDate(data(:,1), data(:,2));
ok=find(data(:,5)~=0.0);
x=4*(t(1:end)-t(round((1+length(t))/2)))./(t(length(t))-t(1));
ndeg=round(log(length(data)/10));ndeg=max(1,ndeg);ndeg=min(8,ndeg);
[p,s]=polyfit(x(ok),obs(ok),ndeg);
yfit=polyval(p,x);
dobs = obs-yfit;

figure;
subplot(3,1,1)
plot(t, obs); hold on;
plot(t, yfit, 'LineWidth', 2); hold off;
datetick('x');
axis tight
title( strcat('Single difference PRN', int2str(data(1,3))) );
ylabel([{'C-L'};{'[m]'}])

subplot(3,1,2)
plot(t, dobs, 'k');
datetick('x');
axis tight
ylabel([{'(C-L)-f_{orb}'};{'[m]'}])

subplot(3,1,3)
plot(t, data(:,7));
datetick('x');
axis tight
ylim([20 50])
ylabel([{'C/N_0'};{'[dB-Hz]'}])
xlabel(['Time'])
box on;

print( '-dpng', strcat('figures/','uxsd', int2str(i), 'prn', int2str(data(1,3)), '.png') );

end



