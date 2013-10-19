
clear
close all

global df
global fps


%Define basic colors
red = [255,0,0];
green = [0,255,0];
blue = [0,0,255];
yellow = [255,255,0];
orange = [255,8*16,0];
white = [255,255,255];
purple = [92,53,102];

colorArr = [red;green;blue;yellow;orange;white;purple];

df_init();


%Create dancefloor matrix
df = zeros(16,16,3);
df = uint8(df);


%Frames per second
fps = 10;

%Create floor plot figure
%figure(1)
%set(gcf,'numbertitle','off','name','DanceFloor');


df(:,1,1) = 255;
df(:,4,2) = 255;
df(:,7,3) = 255;

%Floor loop
while(1)
    
    operation = random('unid',5);
    
    switch(operation)
        
        case 1
            size = random('unid',15)+1;
            center = [random('unid',5)+2,random('unid',6)+1];
            
            color = colorArr(random('unid',length(colorArr)),:);
            
            df = drawSquare(df, center, size, color);
            
        
        case 2%concentric squares
           % df = drawSquare(df, center, size, color);
            
        case 3
            size = random('unid',4)+2;
            center = [random('unid',5)+2,random('unid',6)+1];
            
            color = colorArr(random('unid',length(colorArr)),:);
            
            df = drawFilledSquare(df, center, size, color);
            
    end
    
        xshift = random('unid',3)-2;
        yshift = random('unid',3)-2;
        %The number of times to do the operation
    for i=1:random('unid',5)+15
            

        if (xshift == 0) && (yshift == 0)
            xshift = 1;
        end
        df = circshift(df, [xshift, yshift]);
        df_write();
        pause(1/fps);
    end    
%     df = circshift(df, [0, 1]);
%     df_write();
%     
%     %Display the floor pattern
%     %image(imrotate(df,90));
%     %image(df);
%     
%     %Delay between frames
%     pause(1/fps);
end

