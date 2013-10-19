
function df_write()

%The udp objects for each tile
global udpArr

%The floor frame
global df

%df(:,:,1)
%Generate data packets and send frames
for t = 0:length(udpArr)-1

    %col seq [1:8],[9:16] x2
    %row seq [1:8] [1:8] [9:16] [9:16]
    
    i=1;

for row=floor(t/2)*8+1:floor(t/2)*8+8
        for col=(rem(t,2) * 8 + 1):(8*(rem(t,2)+1))
            for c=1:3    
                data(i) = df(row,col,c);
                i = i+1;
            end
        end
    end
    
    fwrite(udpArr(t+1),['DANCEFLOOR',1,data]);
end

%Latch frames
for t = 1:length(udpArr)
    fwrite(udpArr(t),['DANCEFLOOR',2]);
end
