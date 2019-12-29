function main
img = zeros(64);
for i=0:7
    for j=0:7        
        img(8*i+1:8*(i+1),8*j+1:8*(j+1))=mod(i+j,2);     
    end
end
subplot(231);
imshow(img);
title('ԭͼ');
LEN=5;
Theta=45;
PSF=fspecial('motion',LEN,Theta);
MF=imfilter(img,PSF,'circular','conv');
noise=imnoise(zeros(64),'gaussian',0,0.001);

MFN=MF+noise;
subplot(232);
imshow(MFN);
title('�˶�ģ���͸�˹����');


%���˲�
subplot(233);
imshow(deconvwnr(MFN,PSF),[]);
title('���˲�');

%ά���˲�
NSR=sum(noise(:).^2/sum(MFN(:).^2));
subplot(234);
imshow(deconvwnr(MFN,PSF,NSR),[]);
title('ά���˲�');

%��С���˷��˲�
subplot(235);
imshow(deconvreg(MFN,PSF,0.4,[1e-7 1e7]),[]);
title('��С���˷��˲�');
