
%Create udp object list for communicating with the df tiles
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