%��������,��˹����,���������Լ���ֵ�˲�
img=imread( 'lena.jpg' ); 
img1=imnoise(img, 'salt & pepper' ,0.02);
figure
subplot(321);
imshow(img1);
title('��������');

[h, w] = size(img1);

    for i = 1 : h
        for j = 1 : w
            up = max(i - 1, 1);
            down = min(i + 1, h);
            left = max(j - 1, 1);
            right = min(j + 1, w);
            sub = img1(up : down, left : right);
            sub = sub(:);
            img1(i, j) = median(sub);
        end
    end
  subplot(322);
  imshow(img1);
  title('��ֵ�˲���');
  
  img2=imnoise(img, 'gaussian' ,0.02);
  subplot(323);
  imshow(img2);
  title('��˹����');
  
  
  [h, w] = size(img2);

    for i = 1 : h
        for j = 1 : w
            up = max(i - 1, 1);
            down = min(i + 1, h);
            left = max(j - 1, 1);
            right = min(j + 1, w);
            sub = img2(up : down, left : right);
            sub = sub(:);
            img2(i, j) = median(sub);
        end
    end
  subplot(324);
  imshow(img2);
  title('��ֵ�˲���');
  
 
  img3=imnoise(img,'poisson');
  subplot(325);
  imshow(img3);
  title('��������');

  [h, w] = size(img3);

    for i = 1 : h
        for j = 1 : w
            up = max(i - 1, 1);
            down = min(i + 1, h);
            left = max(j - 1, 1);
            right = min(j + 1, w);
            sub = img3(up : down, left : right);
            sub = sub(:);
            img3(i, j) = median(sub);
        end
    end
  subplot(326);
  imshow(img3);
  title('��ֵ�˲���');
