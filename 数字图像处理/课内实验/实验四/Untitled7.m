
	%巴特沃斯高通滤波	
    J=imread( 'lena.jpg' );
    subplot(221),imshow(uint8(J));
    title( ' 模糊图像 ' );
    J=double(J);
    f=fft2(J);
    g=fftshift(f);
    [M,N]=size(f);
    n1=floor(M/2);
    n2=floor(N/2);
    n=2;
    d0=20;
    for  i=1:M
        for  j=1:N
            d=sqrt((i-n1)^2+(j-n2)^2);
            if  d==0
                h1=0;
                h2=0.5;
            else
                h1=1/(1+(d0/d)^(2*n));
                h2=1/(1+(d0/d)^(2*n))+0.5;
            end
            gg1(i,j)=h1*g(i,j);
            gg2(i,j)=h2*g(i,j);
        end
    end
    gg1=ifftshift(gg1);
    gg1=uint8(real(ifft2(gg1)));
    subplot(223);
    imshow(gg1);
    title( ' 巴特沃斯高通滤波结果 ' );
    gg2=ifftshift(gg2);
    gg2=uint8(real(ifft2(gg2)));
    subplot(224);
    imshow(gg2);
    title( ' 巴特沃斯高通加强滤波结果 ' );