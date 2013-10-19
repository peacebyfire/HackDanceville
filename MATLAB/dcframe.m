
%u = udp('192.168.1.12',21337);
u = udp('192.168.1.17',21337);
fopen(u);

%Valid values: 0-63
data = ones(1,192)*63;



%data(pixel+3) = val;

%Set dc frame to all on
fwrite(u,['DANCEFLOOR',17,data]);

