
global udpArr

for i = 1:length(udpArr)
   fclose(udpArr(i)); 
end

udpArr = udp('192.168.1.151',21337);
udpArr(2) = udp('192.168.1.152',21337);
udpArr(3) = udp('192.168.1.153',21337);
udpArr(4) = udp('192.168.1.154',21337);

for i = 1:length(udpArr)
   fopen(udpArr(i)); 
end

%Create dancefloor matrix
df = zeros(16,16,3);
df = uint8(df);


%Frames per second
fps = 10;

% df(:,1,1) = 255;
% df(:,4,2) = 255;
% df(:,7,3) = 255;

df = drawFilledSquare(df, [4,4], 2, [255,0,0]);

while(1)
    for i=1:8
        df = circshift(df, [0, 1]);
        df_write();
        pause(1/fps);
    end

    for i=1:8
        df = circshift(df, [1, 0]);
        df_write();
        pause(1/fps);
    end

    for i=1:8
        df = circshift(df, [0, -1]);
        df_write();
        pause(1/fps);
    end

    for i=1:8
        df = circshift(df, [-1, 0]);
        df_write();
        pause(1/fps);
    end
end