%jpegdecode函数jpegedcode的解码程序
function x=jpegdecode(y)
    error(nargchk(1,1,nargin));           %检查输入参数
    m=[16 11 10 16 24 40 51 61            %JEPG标准化矩阵
        12 12 14 19 26 58 60 55
        14 13 16 24 40 57 69 56
        14 17 22 29 51 87 80 62
        18 22 37 56 68 109 103 77
        24 35 55 64 81 104 113 92
        49 64 78 87 103 121 120 101
        72 92 95 98 112 100 103 99];
    order=[1 9 2 3 10 17 25 18 11 4 5 12 19 26 33 ... %变换系数排列次序
        41 34 27 20 13 6 7 14 21 28 35 42 49 57 50 ...
        43 36 29 22 15 8 16 23 30 37 44 51 58 59 52 ... 
       45 38 31 24 32 39 46 53 60 61 54 47 40 48 55 62 63 56 64];
       rev=order;                                    %计算逆序
    for k=1:length(order)
        rev(k)=find(order==k);
    end
    %ff=max(rev(:))+1;
    m=double(y.quality)/100*m;
    xb=double(y.numblocks);             %得到图像块数
    sz=double(y.size);
    xn=sz(1);                          %得到行数
    xm=sz(2);                          %得到列数
    x=huffdecode(y.huffman,y.info);    %huffman解码
    x=double(x)-128;
    eob=max(x(:));                    %得到块结束符
    z=zeros(64,xb);k=1;
    for j=1:xb
        for i=1:64
            if x(k)==eob
                k=k+1;break;
            else
                z(i,j)=x(k);
                k=k+1;
            end
        end
    end
    z=z(rev,:);                      %恢复次序
    x=col2im(z,[8 8],[xm xn],'distinct');%重新排列成图像块
    x=blkproc(x,[8 8],'x.*P1',m);       %反标准化 
    t=dctmtx(8);                        
    x=blkproc(x,[8 8], 'P1*x*P2',t',t);  %DCT反变换
    x=uint8(x+128);                      %进行位移
    