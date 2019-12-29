clear all;
clc;
fileName = '6.mp4';  
obj = VideoReader(fileName);
vidFrames = read(obj);
numFrames = obj.numberOfFrames;
backFrame = imread('back.jpg');
[m,n] = size(backFrame);
figure;
gausFilter = fspecial('gaussian',[5 5],1);  
se1 = strel('square',4);
for k=1:numFrames
    currentFrame = vidFrames(:,:,:,k);
    newFrame = uint8(abs(double(currentFrame) - double(backFrame)));
    newFrame = rgb2gray(newFrame);
    newFrame = imfilter(newFrame,gausFilter,'replicate'); 
    thr = graythresh(newFrame);
    newFrame = im2bw(newFrame,thr); 
    newFrame = imerode(newFrame,se1);
    subplot(122)
    imshow(newFrame);

    subplot(121)
    imshow(currentFrame);
    hold on

    pause(0.001);
    hold off
 end
    