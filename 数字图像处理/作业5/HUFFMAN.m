function HUFFMAN()
clear
load woman

%X=imread('girl.bmp','bmp')
data=uint8(X);
imshow(data)
[zipped,info]=huffencode(data); %调用Huffman编码程序进行压缩
unzipped=huffdecode(zipped,info);     %调用Huffman解码程序进行解压
%显示原始图像和经编码解码后的图像,显示压缩比,并计算均方根误差得erms=0
%表示是Huffman是无编码
subplot(121);imshow(data);
subplot(122);imshow(unzipped);
erms=compare(data(:),unzipped(:))
cr=info.ratio
whos data unzipped zipped
