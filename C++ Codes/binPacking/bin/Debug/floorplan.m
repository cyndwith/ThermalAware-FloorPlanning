clear;clc;
fileID = fopen('output.txt','r');
%fileID = fopen('TAFloorplanner/114245960.txt','r');
errmsg = '';
while fileID < 0 
   disp(errmsg);
   filename = input('Open file: ', 's');
   [fileID,errmsg] = fopen(filename);
end
fileID2 = fopen('output.pwr','r');
errmsg = '';
while fileID2 < 0 
   disp(errmsg);
   filename = input('Open file: ', 's');
   [fileID2,errmsg] = fopen(filename);
end
noBlocks = 30; %textscan(fileID,'%f',1)
blocks =  textscan(fileID,'%s %f %f %f %f');
power =  textscan(fileID2,'%s %f ');
pwr = power{2};
blockPara = [blocks{2}, blocks{3}, blocks{4}, blocks{5}].*1e4;    
%/*scale the width and height for visualization */
normW = blockPara(:,1);%./1e6;%./minW;
normH = blockPara(:,2);%./1e6;%./minH;
pwrA = pwr(1:30);
%normP = (pwr(:,1)-min(pwr(:,1)))./(max(pwr(:,1)) - min(pwr(:,1))); %map(normP);

% FOR PATCH
X = [blockPara(:,3),blockPara(:,3),blockPara(:,3)+normW,blockPara(:,3)+normW]';
Y = [blockPara(:,4),blockPara(:,4)+normH,blockPara(:,4)+normH,blockPara(:,4)]';
C = [1:1:30];
figure;grid on;
%patch(X,Y,normP');
patch(X,Y,pwrA);
colorbar;
axis([0 320 0 320]*1e6);

fclose(fileID);
fclose(fileID2);