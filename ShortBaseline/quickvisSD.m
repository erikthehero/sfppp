clc; clear all; close all;

c=299792458;
f0=10.23e6;     
freq1   = 154*f0;
lam1    = c/freq1;
ndeg = 2;

% for i = 0:47
%     fn = strcat('C:\Users\Erik\Documents\sfppp\cpp\main\shortbaseline\output\out', int2str(i), '.txt');
%     plotResultSD(fn, i)
% end

%% sd statistics results
fn1 = 'C:\Users\Erik\Documents\sfppp\cpp\main\shortbaseline\output\sdResults_rp3_take1.txt';
fn2 = 'C:\Users\Erik\Documents\sfppp\cpp\main\shortbaseline\output\sdResults_rp2_take1.txt';
fn3 = 'C:\Users\Erik\Documents\sfppp\cpp\main\shortbaseline\output\sdD1C_rpi2_take1.txt';
fn4 = 'C:\Users\Erik\Documents\sfppp\cpp\main\shortbaseline\output\sdD1C_rpi3_take1.txt';
data1 = dlmread(fn1);
data2 = dlmread(fn2);
data3 = dlmread(fn3);
data4 = dlmread(fn4);

h1 = figure;
ind = (data1(:,3) > 50 | data1(:,2) > 100 | data1(:,2) < 10);
data1(ind,:) = [];

plot(data1(data1(:,1)<100,2), data1(data1(:,1)<150,3), '.'); hold on;
plot(data1(data1(:,1)>100,2), data1(data1(:,1)>150,3), '.'); hold off;
title('Code SD Ant2');
xlabel('S/N_{0}');
ylabel([{'(C-L)-p'};{'[m]'}])
ylim([0 5])
legend('GPS', 'GALILEO');
%print( '-dpng', strcat('figures/','uxsd_ant2', '.png') );

h2 = figure;
ind = (data2(:,3) > 50 | data2(:,2) > 100 | data2(:,2) < 10);
data2(ind,:) = [];
plot(data2(data2(:,1)<100,2), data2(data2(:,1)<150,3), '.'); hold on;
plot(data2(data2(:,1)>100,2), data2(data2(:,1)>150,3), '.');
title('Code SD Ant1');
xlabel('S/N_{0}');
ylabel([{'(C-L)-p'};{'[m]'}])
ylim([0 5])
legend('GPS', 'GALILEO');
% print( '-dpng', strcat('figures/','uxsd_ant1', '.png') );

h3 = figure;
indG = data3(:,1)<150 & abs(data3(:,2))<60 & abs(data3(:,3)<2); 
indE = data3(:,1)>150 & abs(data3(:,2))<60 & abs(data3(:,3)<2); 

plot(data3(indG,2), data3(indG,3), '.'); hold on;
plot(data3(indE,2), data3(indE,3), '.');
title('Doppler SD Ant1');
xlabel('S/N_{0}');
ylabel([{'(D-\Delta L)'};{'[m/s]'}])
legend('GPS', 'GALILEO');
ylim([0 0.2])
print( '-dpng', strcat('figures/','uxsd_ant1D1C', '.png') );

h4 = figure;
indG = data4(:,1)<150 & abs(data4(:,2))<60 & abs(data4(:,3)<2); 
indE = data4(:,1)>150 & abs(data4(:,2))<60 & abs(data4(:,3)<2); 

plot(data4(indG,2), data4(indG,3), '.'); hold on;
plot(data4(indE,2), data4(indE,3), '.');
title('Doppler SD Ant2');
xlabel('S/N_{0}');
ylabel([{'(D-\Delta L)'};{'[m/s]'}])
legend('GPS', 'GALILEO');
ylim([0 0.2])
print( '-dpng', strcat('figures/','uxsd_ant2D1C', '.png') );

function plotResultSD(fn, i)
  data = dlmread(fn);
  
  % remove small segments
  if (length(data)<1200)
    return;
  end
  
  t = GpsToDate(data(:,1), data(:,2));
  
  figure;
  subplot(3,1,1)
  plot(t, data(:,3)+data(:,5)); hold on;
  plot(t, data(:,5),'LineWidth', 2); hold off;
  datetick('x');
  axis tight
  %title( strcat('Single difference PRN', int2str(data(1,3))) );
  ylabel([{'C-L'};{'[m]'}])
  
  subplot(3,1,2)
  plot(t, data(:,3), 'k');
  datetick('x');
  axis tight
  ylabel([{'(C-L)-f_{orb}'};{'[m]'}])

  subplot(3,1,3)
  plot(t, data(:,3));
  datetick('x');
  axis tight
  ylim([20 50])
  ylabel([{'C/N_0'};{'[dB-Hz]'}])
  xlabel(['Time'])
  box on;

  print( '-dpng', strcat('figures/','uxsd', int2str(i), 'prn', int2str(data(1,3)), '.png') ); 
 
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



