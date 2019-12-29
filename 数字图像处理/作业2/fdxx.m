function fdxx
b=imread( 'lena.jpg' );

f0=0;g0=0;
f1=50;g1=10;
f2=100;g2=200;
f3=255;g3=255;

figure,plot([f0,f1,f2,f3],[g0,g1,g2,g3]);

r1=(g1-g0)/(f1-f0);
b1=-r1*f0+g0;

r2=(g2-g1)/(f2-f1);
b2=-r2*f1+g1;

r3=(g3-g2)/(f3-f2);
b3=-r3*f2+g2;

axis([0 255 0 255]);
[m,n]=size(b);
h=double(b);
g = zeros(m,n);
figure,imshow(mat2gray(h));

for i=1:m
  for j=1:n
    t=h(i,j);
    if ((t>=f0)&&(t<=f1))
        g(i,j)=r1*t+b1;
    else
        if ((t>=f1)&&(t<=f2))
            g(i,j)=r2*t+b2;
        else
        if ((t>=f2)&&(t<=f3))
            g(i,j)=r3*t+b3;
        end
        end
    end
  end
end
figure,imshow(mat2gray(g));
