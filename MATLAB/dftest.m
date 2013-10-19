
u = udp('192.168.1.12',21337);

fopen(u);

delay = 1/10

i = 1;
while(1)
    
    data = zeros(1,192);
    
    data(i) = 255;
    
    i = i + 1;
    
    if i > 192
        i = 1;
    end
    
    
    %Set frame to all on
    fwrite(u,['DANCEFLOOR',1,data]);
    %Latch frame
    fwrite(u,['DANCEFLOOR',2]);
    pause(delay);

end

