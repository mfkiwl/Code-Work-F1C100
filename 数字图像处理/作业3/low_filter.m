function low_filter
img = imread('lena.jpg');
subplot(121);
imshow(img);
img = double(img);
f = fft2(img);
g = fftshift(f);

[M, N] = size(f);
n1 = floor(M/2);
n2 = floor(N/2);
d0 = 10;
for i=1:M
    for j=1:N
        d=sqrt((i-n1)^2+ (j-n2)^2);
        if d<=d0
            h=0;
        else
            h=1;
        end
        g(i,j)=h*g(i,j);
    end
end
g = ifftshift(g);
g = uint8(real(ifft2(g)));
subplot(122);
imshow(g);

end