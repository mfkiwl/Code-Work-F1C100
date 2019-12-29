function dsbh
    I=imread( 'lena.jpg' );
    imshow(I);
    Image=log(3+double(I));
    figure(2),imshow(Image,[]);
    
   