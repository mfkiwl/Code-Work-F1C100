%jpegencode函数用来压缩图像用一种近似的JPEG方法
function res=jpegencode(x,quality)
    %x 为输入图�?
    %quality决定了截去的系数和压缩比
    error(nargchk(1,2,nargin));           %�?查输入参�?
    if nargin<2
        quality=1;                        %缺省的quality=1
    end
    x=double(x)-128;                      %像素层次移动-128
    [xm,xn]=size(x);                      %得到图像的尺�?
    t=dctmtx(8);                          %得到8*8DCT变换矩阵
    %将图像分割成8*8子图�?,进行DCT变换,然后进行量化
    y=blkproc(x,[8 8],'dct2(x)') %            'P1*x*P2',t,t');
    m=[16 11 10 16 24 40 51 61            %JEPG标准化矩�?
        12 12 14 19 26 58 60 55
        14 13 16 24 40 57 69 56
        14 17 22 29 51 87 80 62
        18 22 37 56 68 109 103 77
        24 35 55 64 81 104 113 92
        49 64 78 87 103 121 120 101
        72 92 95 98 112 100 103 99]*quality;
     %用m标准化阵列对变换阵列进行量化,即根据式(    )标准�?
    yy=blkproc(y,[8 8],'round(x./P1)',m);  
    y=im2col(yy,[8 8],'distinct');               %将图像块排列成向�?
    xb=size(y,2);                                %得到列数,也就子图像个�?
    order=[1 9 2 3 10 17 25 18 11 4 5 12 19 26 33 ... %变换系数排列次序
        41 34 27 20 13 6 7 14 21 28 35 42 49 57 50 ...
        43 36 29 22 15 8 16 23 30 37 44 51 58 59 52 ... 
       45 38 31 24 32 39 46 53 60 61 54 47 40 48 55 62 63 56 64];
    %用Z形模板对变换系数重新排列
    y=y(order,:);
    eob=max(x(:))+1;                             %创建�?个块结束符号    
    num=numel(y)+size(y,2);
    r=zeros(num,1);
    count=0;
    %将非零元素重新排列放入r�?.-26 -3   eob  -25    1  eob
    for j=1:xb                               %每次对一�?(即一�?)进行操作
        i=max(find(y(:,j)));                 %找最后一个非零元�?
        if isempty(i)                        %没有非零元素
            i=0;
        end
        p=count+1;
        q=p+i;
        r(p:q)=[y(1:i,j);eob];               %截去0 并加上结束符�?
        count=count+i+1;              
    end
    r((count+1):end)=[];                      %删除没有用的部分
    
    r=r+128;               
    %保存编码信息
    res.size=uint16([xm,xn]);
    res.numblocks=uint16(xb);
    res.quality=uint16(quality*100);
    %对r进行huffman编码
    [res.huffman res.info]=huffencode(uint8(r));
    