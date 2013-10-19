function df = setPixel(df, x, y, color)
    df(x,y,1) = color(1);
    df(x,y,2) = color(2);
    df(x,y,3) = color(3);
end