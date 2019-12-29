  
  %Sobel算子
  I=imread( 'lena_bw.jpg' ); 
  [H,W]=size(I); 
  M=double(I);
  J=M;
  for  i=2:H-1 
      for  j=2:W-1 
          J(i,j)=abs(M(i-1,j+1)-M(i-1,j-1)+2*M(i,j+1)-2*M(i,j-1)+M(i+1,j+1)-M(i+1,j-1))+abs(M(i-1,j-1)-M(i+1,j-1)+2*M(i-1,j)-2*M(i+1,j)+M(i-1,j+1)-M(i+1,j+1)); 
      end 
  end 
  subplot(1,2,1);imshow(I);title( ' 原图 ' ); 
  subplot(1,2,2);imshow(uint8(J));title( 'Sobel  处理后 ' ); 
