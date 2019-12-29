function Picture_Process()
img = imread('logo.jpg');
img_x = img(:,end:-1:1,:);
img_y = img(end:-1:1,:,:);

figure,
subplot(1,3,1),subimage(img),title('ԭͼ��')
subplot(1,3,2),subimage(img_x),title('ˮƽ����')
subplot(1,3,3),subimage(img_y),title('��ֱ����')



img_r = imrotate(img,30);

figure,
subplot(1,2,1),subimage(img),title('ԭͼ��')
subplot(1,2,2),subimage(img_r),title('��ת��ͼ��')


img_s = imresize(img,0.5);
img_b = imresize(img,1.2);

figure,imshow(img),title('ԭͼ��')
figure,imshow(img_s),title('��С��ͼ��')
figure,imshow(img_b),title('�Ŵ��ͼ��')