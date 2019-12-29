%huffencode函数对输入矩阵vector进行Huffman编码,返回编码后的向量(压缩后数据)及相关信息
function [zipped,info]=huffencode(vector)
    %输入,输出都是uint8格式
    %info返回解回需要的结构信息
    %info.pad是添加的比特数
    %info.huffcodes是Huffman码字
    %info.rows是原始图像行数
    %info.cols是原始图像列数
    %info.length是原始图像数据长度
    %info.maxcodelen是最大码长
    if ~isa(vector,'uint8')
        error('input argument must be a uint8 vector');
    end
    [m,n]=size(vector);    
    vector=vector(:)';
    f=frequency(vector);                %计算各符号出现的概率
    simbols=find(f~=0);
    f=f(simbols);
    [f,sortindex]=sort(f);             %将符号按照出现的概率大小排列
    simbols=simbols(sortindex);
    len=length(simbols);
    simbols_index=num2cell(1:len);
    codeword_tmp=cell(len,1);
    while length(f)>1                  %生成Huffman树 ,得到码字得码表      
        index1=simbols_index{1};
        index2=simbols_index{2};
        codeword_tmp(index1)=addnode(codeword_tmp(index1),uint8(0));
        codeword_tmp(index2)=addnode(codeword_tmp(index2),uint8(1));
        f=[sum(f(1:2)) f(3:end)];
        simbols_index=[{[index1,index2]} simbols_index(3:end)];
        [f,sortindex]=sort(f);
        simbols_index=simbols_index(sortindex);
    end
    codeword=cell(256,1);
    codeword(simbols)=codeword_tmp;
    len=0;
    for index=1:length(vector)                           %得到整个图像所有比特数
        len=len+length(codeword{double(vector(index))+1});
    end
    string=repmat(uint8(0),1,len);
    pointer=1;
    for index=1:length(vector)                            %对输入图像进行编码
        code=codeword{double(vector(index))+1};
        len=length(code);
        string(pointer+(0:len-1))=code;
        pointer=pointer+len;
    end
    len=length(string);
    pad=8-mod(len,8);                                     %非8整数倍 ,最后补pad 个0
    if pad>0
        string=[string uint8(zeros(1,pad))];
    end
    codeword=codeword(simbols);
    codelen=zeros(size(codeword));
    weights=2.^(0:23);
    maxcodelen=0;
    for index=1:length(codeword)
        len=length(codeword{index});
        if len>maxcodelen
            maxcodelen=len;
        end
        if len>0
            code=sum(weights(codeword{index}==1));
            code=bitset(code,len+1);
            codeword{index}=code;
            codelen(index)=len;
        end
    end
    codeword=[codeword{:}];
    %计算压缩后的向量
    cols=length(string)/8;
    string=reshape(string,8,cols);
    weights=2.^(0:7);
    zipped=uint8(weights*double(string));
    %码表存储到一个稀疏矩阵
    huffcodes=sparse(1,1);
    for index=1:nnz(codeword)  %length(codeword)  %nume1(codeword)
        huffcodes(codeword(index),1)=simbols(index);
    end
    %填写解码时所需的结构信息
    info.pad=pad;
    info.huffcodes=huffcodes;
    info.ratio=cols./length(vector);
    info.length=length(vector);
    info.maxcodelen=maxcodelen;
    info.rows=m;
    info.cols=n;
    