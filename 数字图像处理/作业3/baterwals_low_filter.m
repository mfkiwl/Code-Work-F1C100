function baterwals_low_filter
img = imread('lena.jpg');
subplot(121);
imshow(img);
title('º¬ÓĞ½·ÑÎÔëÉùµÄÍ¼Ïñ');
img = double(img);
f = fft2(img);
g= fftshift(f);
[M, N]=size(f);
n=3;
d0=10;
n1=floor(M/2);
n2=floor(N/2);
for i=1:M
    for j=1:N
        d = sqrt((i-n1)^2+(j-n2)^2);
        h=1/(1+(d0/d)^(2*n));
        g(i,j)=h*g(i,j);
    end
end
g=ifftshift(g);
g=uint8(real(ifft2(g)));
subplot(122);
imshow(g);
title('ÂË²¨ºóÍ¼Ïñ');

end