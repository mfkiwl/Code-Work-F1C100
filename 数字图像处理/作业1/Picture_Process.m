function Picture_Process()
img = imread('logo.jpg');
img_x = img(:,end:-1:1,:);
img_y = img(end:-1:1,:,:);

figure,
subplot(1,3,1),subimage(img),title('原图像')
subplot(1,3,2),subimage(img_x),title('水平镜像')
subplot(1,3,3),subimage(img_y),title('垂直镜像')



img_r = imrotate(img,30);

figure,
subplot(1,2,1),subimage(img),title('原图像')
subplot(1,2,2),subimage(img_r),title('旋转后图像')


img_s = imresize(img,0.5);
img_b = imresize(img,1.2);

figure,imshow(img),title('原图像')
figure,imshow(img_s),title('缩小后图像')
figure,imshow(img_b),title('放大后图像')