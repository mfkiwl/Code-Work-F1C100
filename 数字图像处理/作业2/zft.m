function zft

img=imread('lena.jpg');
imshow(img);
title('���⻯ǰͼ��');

[m,n]=size(img);
GL=zeros(1,256); 
GLPeq = zeros(1,256);

for k=0:255
    GL(k+1)=length(find(img==k))/(m*n);
end

figure,bar(0:255,GL,'g') 
title('ԭͼ��ֱ��ͼ')
xlabel('�Ҷ�ֵ')
ylabel('���ָ���')

S1=zeros(1,256); 
for i=1:256
    for j=1:i
        S1(i)=GL(j)+S1(i); 
    end
end
S2=round((S1*256)+0.5); 
for i=1:256
    GLPeq(i)=sum(GL(S2==i)); 
end
figure,bar(0:255,GLPeq,'b') 
title('���⻯���ֱ��ͼ')
xlabel('�Ҷ�ֵ')
ylabel('���ָ���')


img1=img;
for i=0:255
    img1(img==i)=S2(i+1);
end
figure,imshow(img1)
title('���⻯��ͼ��')



