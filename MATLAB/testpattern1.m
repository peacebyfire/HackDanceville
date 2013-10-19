
u1 = udp('192.168.1.102',21337);
u2 = udp('192.168.1.100',21337);
u3 = udp('192.168.1.149',21337);
u4 = udp('192.168.1.101',21337);
fopen(u);
fopen(u2);

delay = 1/10




df = ones(8,16,3)*5;

df(:,1,1) = 255;
df(:,4,2) = 255;
df(:,7,3) = 255;

data0 = zeros(1,192);
data1 = zeros(1,192);


i = 1;
deg = 0

while(1)
    deg = deg+10;
    df = circshift(df, [0, 1]);
    
   % df2 = imrotate(df,deg, 'bilinear');
    
    i=1;
    for row=1:8
        for col=1:8
            for c=1:3
                data0(i) = df(row,col,c);
                i = i+1;
            end
        end
    end
    
    i = 1;
    for row=1:8
        for col=9:16
            for c=1:3
                data1(i) = df(row,col,c);
                i = i+1;
            end
        end
    end



    
    
    %Set frame to all off
    fwrite(u,['DANCEFLOOR',1,data0]);
    fwrite(u2,['DANCEFLOOR',1,data1]);
    %Latch frame
    fwrite(u,['DANCEFLOOR',2]);
    fwrite(u2,['DANCEFLOOR',2]);
    pause(delay);
    


end

