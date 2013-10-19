function df = drawLine(df, x1,y1,x2,y2, color)

    if x1 > 16
        x1 = 16
    end
    
    if x1 < 1
        x1 = 1;
    end
    
    
    
    if x2 > 16
        x2 = 16
    end
    
    if x2 < 1
        x2 = 1;
    end
    
    
    
    if y1 > 16
        y1 = 16
    end
    
    if y1 < 1
        y1 = 1;
    end
    
    
    if y2 > 16
        y2 = 16
    end
    
    if y2 < 1
        y2 = 1;
    end

    dx = abs(x2-x1)+1;
    dy = abs(y2-y1)+1;
    d = [dx,dy];

    xpoints = round(linspace(x1,x2,max(d)));
    ypoints = round(linspace(y1,y2,max(d)));
    
    for i = 1:length(xpoints)
         df = setPixel(df, xpoints(i),ypoints(i),color);
    end
    
%     dx = x2 - x1+1;
%     dy = y2 - y1+1;
%     
%     for x = x1:dx/16:x2
%        for y = y1:dy/16:y2
%           df = setPixel(df, round(x),round(y),color);
%        end
%     end

end