function myfilter
img = imread('lena.jpg');
img = imnoise(img,'salt & pepper',0.02);
figure;
subplot(1,2,1)
imshow(img);
[h, w] = size(img);

for i = 1 : h
    for j = 1 : w
        up = max(i - 1, 1);
        down = min(i + 1, h);
        left = max(j - 1, 1);
        right = min(j + 1, w);
        img(i, j) = mean(mean(img(up : down, left : right)));
    end
end
subplot(1,2,2)   
imshow(img);

end