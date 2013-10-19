function df = drawFilledSquare(df, center, size, color)
    
%draw top line
    for i = 0:size-1
     df = drawLine(df, center(1)-size/2+1+i, center(2)+size/2, center(1)-size/2+1+i, center(2)-size/2+1, color);
    end
end

%Center is the top right corner of the specified coordinate