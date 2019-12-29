
  %理想低通滤波
  G=imread( 'lena.jpg' );
  J=imnoise(G, 'salt & pepper' ,0.02);
  subplot(121),imshow(J);title( ' 添加椒盐噪声图像 ' );
  J=double(J);

    % 采用傅立叶变换
  f=fft2(J);

    % 采用矩阵平衡
    g=fftshift(f);
    [M,N]=size(f);
    n=3;
    d0=45;
    n1=floor(M/2); % 向下取整
    n2=floor(N/2);
    for  i=1:M
    for  j=1:N
    d=sqrt((i-n1)^2+(j-n2)^2);
    if  d<=d0
    h=1;
    else
    h=0;
    end
    g(i,j)=h*g(i,j);
    end
    end
    g=ifftshift(g);
    g=uint8(real(ifft2(g)));
    subplot(122);
    imshow(g);title( ' 理想低通滤波后图像 ' );
