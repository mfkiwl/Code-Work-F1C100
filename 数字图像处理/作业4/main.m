function main
img = zeros(64);
for i=0:7
    for j=0:7        
        img(8*i+1:8*(i+1),8*j+1:8*(j+1))=mod(i+j,2);     
    end
end
subplot(231);
imshow(img);
title('原图');
LEN=5;
Theta=45;
PSF=fspecial('motion',LEN,Theta);
MF=imfilter(img,PSF,'circular','conv');
noise=imnoise(zeros(64),'gaussian',0,0.001);

MFN=MF+noise;
subplot(232);
imshow(MFN);
title('运动模糊和高斯噪声');


%逆滤波
subplot(233);
imshow(deconvwnr(MFN,PSF),[]);
title('逆滤波');

%维纳滤波
NSR=sum(noise(:).^2/sum(MFN(:).^2));
subplot(234);
imshow(deconvwnr(MFN,PSF,NSR),[]);
title('维纳滤波');

%最小二乘方滤波
subplot(235);
imshow(deconvreg(MFN,PSF,0.4,[1e-7 1e7]),[]);
title('最小二乘方滤波');
