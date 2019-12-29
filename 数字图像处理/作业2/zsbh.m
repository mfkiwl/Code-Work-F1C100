function zsbh
    I=imread( 'lena.jpg' );
    imshow(I);
    Image= 1.5.^(double(I)*0.050)-1;
    figure(2),imshow(Image,[]);
