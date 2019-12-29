function sobel

img=imread('lena.jpg');  
subplot(131);imshow(img);title('原图'); 
[high,width] = size(img);   
F2 = double(img);        
U = double(img);       
img_x = img;
img_y = img;
for i = 2:high - 1   
    for j = 2:width - 1
        Gx = (U(i+1,j-1) + 2*U(i+1,j) + F2(i+1,j+1)) - (U(i-1,j-1) + 2*U(i-1,j) + F2(i-1,j+1));
        Gy = (U(i-1,j+1) + 2*U(i,j+1) + F2(i+1,j+1)) - (U(i-1,j-1) + 2*U(i,j-1) + F2(i+1,j-1)); 
        img_x(i,j) = Gx;
        img_y(i,j) = Gy;
    end
end 

subplot(132);imshow(im2uint8(img_x));title('水平边缘');  
subplot(133);imshow(im2uint8(img_y));title('垂直边缘');

end