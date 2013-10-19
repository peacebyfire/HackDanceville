
u = udp('192.168.1.9',21337);
u2 = udp('192.168.1.8',21337);
fopen(u);
fopen(u2);

delay = 1/15

i = 1;
while(1)
    
    
    %Set frame to all off
    fwrite(u,['DANCEFLOOR',1,zeros(1,192)]);
    fwrite(u2,['DANCEFLOOR',1,zeros(1,192)]);
    %Latch frame
    fwrite(u,['DANCEFLOOR',2]);
    fwrite(u2,['DANCEFLOOR',2]);
    pause(delay);
    
    %Set frame to all on
    fwrite(u,['DANCEFLOOR',1,ones(1,192)*255]);
    fwrite(u2,['DANCEFLOOR',1,ones(1,192)*255]);
    %Latch frame
    fwrite(u,['DANCEFLOOR',2]);
    fwrite(u2,['DANCEFLOOR',2]);
    pause(delay);

end

