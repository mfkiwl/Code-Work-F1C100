function d=d()
clear all
x=imread('girl.bmp');
figure(1);
subplot(121);
imshow(x);
y=jpegencode(x,5);
X=jpegdecode(y);
subplot(122);
imshow(X);
e=double(x)-double(X);
[m,n]=size(e);
erms=sqrt(sum(e(:).^2)/(m*n))
cr=imageratio(x,y.huffman)

