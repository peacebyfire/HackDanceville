function df = drawSquare(df, center, size, color)
    %draw right line
    df = drawLine(df, center(1)-size/2+1, center(2)+size/2, center(1)+size/2, center(2)+size/2, color);
    %draw bottom line
    df = drawLine(df, center(1)+size/2, center(2)+size/2-1, center(1)+size/2, center(2)-size/2+1, color);
    %draw left line
    df = drawLine(df, center(1)-size/2+1, center(2)-size/2+1, center(1)+size/2, center(2)-size/2+1, color);
    %draw top line
    df = drawLine(df, center(1)-size/2+1, center(2)+size/2, center(1)-size/2+1, center(2)-size/2+1, color);
end

%Center is the top right corner of the specified coordinate