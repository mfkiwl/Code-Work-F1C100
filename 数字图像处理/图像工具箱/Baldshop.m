function varargout = Baldshop(varargin)
% BALDSHOP MATLAB code for Baldshop.fig
%      BALDSHOP, by itself, creates a new BALDSHOP or raises the existing
%      singleton*.
%
%      H = BALDSHOP returns the handle to a new BALDSHOP or the handle to
%      the existing singleton*.
%
%      BALDSHOP('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in BALDSHOP.M with the given input arguments.
%
%      BALDSHOP('Property','Value',...) creates a new BALDSHOP or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before Baldshop_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to Baldshop_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help Baldshop

% Last Modified by GUIDE v2.5 17-Dec-2019 13:34:23

% Begin initialization code - DO NOT EDIT
gui_Singleton = 0;
gui_State = struct('gui_Name',       mfilename, ...
    'gui_Singleton',  gui_Singleton, ...
    'gui_OpeningFcn', @Baldshop_OpeningFcn, ...
    'gui_OutputFcn',  @Baldshop_OutputFcn, ...
    'gui_LayoutFcn',  [] , ...
    'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT
end

% --- Executes just before Baldshop is made visible.
function Baldshop_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to Baldshop (see VARARGIN)

% Choose default command line output for Baldshop
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

global img img_buffer sp;
img = ones(500);
img_buffer = cell(50,1);
sp = 1;
% UIWAIT makes Baldshop wait for user response (see UIRESUME)
% uiwait(handles.figure1);
end

% --- Outputs from this function are returned to the command line.
function varargout = Baldshop_OutputFcn(hObject, eventdata, handles)
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;

end


function push_img()

global img img_buffer sp;

if sp<10
    img_buffer(sp) = {img};
    sp = sp+1;
else
    for i=1:9
        img_buffer(i)=img_buffer(i+1);
    end
    img_buffer(10) = {img};
end

end

function pop_img()

global img img_buffer sp;
if sp > 1
    img = img_buffer{sp-1};
    img_buffer(sp) = [];
    sp = sp-1;
end

end

function show_zft(handles)
global img;

[m,n]=size(img);
GL=zeros(1,256);

for k=0:255
    GL(k+1)=length(find(img==k))/(m*n);
end

axes(handles.axes2);
axis([0,255,0,1]);
xlim=[0,255];
ylim=[0,1];
bar(0:255,GL,'b');

end


function refresh_img(handles)
global img;
axes(handles.axes1);

imshow(img);
pos = get(handles.axes1,'pos');
pos(3:4) = size(img);
pos
set(handles.axes1,'pos',pos);
show_zft(handles);
axes(handles.axes1);
end



% --- Executes on button press in pushbutton1.
function pushbutton1_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
end


% --------------------------------------------------------------------
function File_Callback(hObject, eventdata, handles)
% hObject    handle to File (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
end
% --------------------------------------------------------------------
function File_open_Callback(hObject, eventdata, handles)
% hObject    handle to File_open (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global filename pathname img;
[filename, pathname] = uigetfile({'*.bmp; *.jpg;*.jpeg;*.png;*.tif;'});
fpath = [pathname filename];
img = imread(fpath);
refresh_img(handles);
end
% --------------------------------------------------------------------
function File_save_Callback(hObject, eventdata, handles)
% hObject    handle to File_save (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global filename pathname img;
fpath = [pathname filename];
imwrite(img,[pathname filename]);
end

% --------------------------------------------------------------------
function File_resave_Callback(hObject, eventdata, handles)
% hObject    handle to File_resave (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global img;
[file,path] = uiputfile({'*.png';'*.jpg';'*.jpeg';'*.bmp'});
imwrite(img,[path file]);
end

% --- Executes on button press in pushbutton2.
function pushbutton2_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global img;
push_img();
img = img(:,end:-1:1,:);
refresh_img(handles);
end


% --- Executes on button press in pushbutton5.
function pushbutton5_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton5 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global img;
push_img();
img = img(end:-1:1,:,:);
refresh_img(handles);
end

% --- Executes on button press in pushbutton6.
function pushbutton6_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton6 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global img img_r ;
push_img();
d = dialog('Position',[100 100 450 450],'Name','旋转预览');
ax = axes(d);
ax.Units = 'pixels';
ax.Position = [10 70 450 350];
imshow(img);

btn1 = uicontrol('Parent',d,...
    'Position',[100 10 70 25],...
    'String','确认',...
    'Callback',@btn1_callback);

btn2 = uicontrol('Parent',d,...
    'Position',[280 10 70 25],...
    'String','取消',...
    'Callback',@btn2_callback);
txt1 = uicontrol('Parent',d,...
    'Style','text',...
    'Position',[15 30 40 20],...
    'String','-180°');
txt2 = uicontrol('Parent',d,...
    'Style','text',...
    'Position',[400 30 40 20],...
    'String','+180°');
txt3= uicontrol('Parent',d,...
    'Style','text',...
    'Position',[180 50 55 20],...
    'String','当前角度: ');
txt3= uicontrol('Parent',d,...
    'Style','text',...
    'Position',[230 50 55 20],...
    'String','0');
c = uicontrol(d,'Position',[50,40,350,10],'Style','slider','Callback',@slider_callback);
c.Value = 0.5;
uiwait(d);

    function slider_callback(c,event)
        angle = c.Value*360  -180;
        set(txt3,'String',angle);
        img_r = imrotate(img,angle);
        imshow(img_r)
    end

    function btn1_callback(btn,event)
        img = img_r;
        delete(gcf);
    end

    function btn2_callback(btn,event)
        delete(gcf);
    end

refresh_img(handles);

end


% --- Executes on button press in pushbutton7.
function pushbutton7_Callback(hObject, eventdata, handles)

global img img_resize;
push_img();
d = dialog('Position',[100 100 450 450],'Name','缩放预览');
ax = axes(d);
ax.Units = 'pixels';
ax.Position = [10 70 450 350];
imshow(img);

btn1 = uicontrol('Parent',d,...
    'Position',[100 10 70 25],...
    'String','确认',...
    'Callback',@btn1_callback);

btn2 = uicontrol('Parent',d,...
    'Position',[280 10 70 25],...
    'String','取消',...
    'Callback',@btn2_callback);
txt1 = uicontrol('Parent',d,...
    'Style','text',...
    'Position',[15 30 40 20],...
    'String','1/10');
txt2 = uicontrol('Parent',d,...
    'Style','text',...
    'Position',[400 30 40 20],...
    'String','10');
txt3= uicontrol('Parent',d,...
    'Style','text',...
    'Position',[180 50 55 20],...
    'String','当前倍数: ');
txt3= uicontrol('Parent',d,...
    'Style','text',...
    'Position',[230 50 55 20],...
    'String','1');
c = uicontrol(d,'Position',[50,40,350,10],'Style','slider','Callback',@slider_callback);
c.Value = 0.5;
uiwait(d);

    function slider_callback(c,event)
        times = 10^(-1 + c.Value*2);
        set(txt3,'String',times);
        img_resize = imresize(img,times);
        imshow(img_resize)
    end

    function btn1_callback(btn,event)
        img = img_resize;
        delete(gcf);
    end

    function btn2_callback(btn,event)
        delete(gcf);
    end

refresh_img(handles);
end


% --- Executes on button press in pushbutton13.
function pushbutton13_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton13 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global img;
push_img();
d = dialog('Position',[500 500 200 150],'Name','参数设置');

btn1 = uicontrol('Parent',d,...
    'Position',[50 10 40 20],...
    'String','确认',...
    'Callback',@btn1_callback);

btn2 = uicontrol('Parent',d,...
    'Position',[120 10 40 20],...
    'String','取消',...
    'Callback',@btn2_callback);
txt1 = uicontrol('Parent',d,...
    'Style','text',...
    'Position',[10  100 40 20],...
    'String','点 1');
txt2 = uicontrol('Parent',d,...
    'Style','text',...
    'Position',[10 50 40 20],...
    'String','点 2');
edit1 = uicontrol('Parent',d,'Style','edit','Position',[50 100 60 30],'String','30.0');
edit2 = uicontrol('Parent',d,'Style','edit','Position',[120 100 60 30],'String','10.0');
edit3 = uicontrol('Parent',d,'Style','edit','Position',[50 50 60 30],'String','80.0');
edit4 = uicontrol('Parent',d,'Style','edit','Position',[120 50 60 30],'String','200.0');


uiwait(d);


    function btn1_callback(btn,event)
        
        
        b = img;
        
        demensions = ndims(b);
        if demensions == 3
            b = rgb2gray(b);
        end
        
        f0=0;g0=0;
        f1=str2double(edit1.String);g1=str2double(edit2.String);
        f2=str2double(edit3.String);g2=str2double(edit4.String);
        f3=255;g3=255;
        
        r1=(g1-g0)/(f1-f0);
        b1=-r1*f0+g0;
        
        r2=(g2-g1)/(f2-f1);
        b2=-r2*f1+g1;
        
        r3=(g3-g2)/(f3-f2);
        b3=-r3*f2+g2;
        
        [m,n]=size(b);
        h=double(b);
        g = zeros(m,n);
        
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
        img = uint8(round(g));
        
        delete(gcf);
    end

    function btn2_callback(btn,event)
        delete(gcf);
    end

refresh_img(handles);
end

% --- Executes on button press in pushbutton14.
function pushbutton14_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton14 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global img;
push_img();
d = dialog('Position',[500 500 200 100],'Name','参数设置');

btn1 = uicontrol('Parent',d,...
    'Position',[50 10 40 20],...
    'String','确认',...
    'Callback',@btn1_callback);

btn2 = uicontrol('Parent',d,...
    'Position',[120 10 40 20],...
    'String','取消',...
    'Callback',@btn2_callback);
txt1 = uicontrol('Parent',d,...
    'Style','text',...
    'Position',[30 50 40 20],...
    'String','参数 1');

edit1 = uicontrol('Parent',d,'Style','edit','Position',[80 50 60 30],'String','3.0');
%     edit2 = uicontrol('Parent',d,'Style','edit','Position',[120 100 60 30],'String','10.0');
%     edit3 = uicontrol('Parent',d,'Style','edit','Position',[50 50 60 30],'String','80.0');
%     edit4 = uicontrol('Parent',d,'Style','edit','Position',[120 50 60 30],'String','200.0');


uiwait(d);


    function btn1_callback(btn,event)
        I = img;
        demensions = ndims(I);
        if demensions == 3
            I = rgb2gray(I);
        end
        
        c = str2double(edit1.String);
        I=log(c+double(I));
        I = (I.*255)/(max(max(I)));
        I = uint8(I);
        
        img = I;
        
        delete(gcf);
    end

    function btn2_callback(btn,event)
        delete(gcf);
    end

refresh_img(handles);
end

% --- Executes on button press in pushbutton15.
function pushbutton15_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton15 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global img;
push_img();
d = dialog('Position',[500 500 200 100],'Name','参数设置');

btn1 = uicontrol('Parent',d,...
    'Position',[50 10 40 20],...
    'String','确认',...
    'Callback',@btn1_callback);

btn2 = uicontrol('Parent',d,...
    'Position',[120 10 40 20],...
    'String','取消',...
    'Callback',@btn2_callback);
txt1 = uicontrol('Parent',d,...
    'Style','text',...
    'Position',[5 50 40 20],...
    'String','参数 1');
txt2 = uicontrol('Parent',d,...
    'Style','text',...
    'Position',[100 50 40 20],...
    'String','参数 2');

edit1 = uicontrol('Parent',d,'Style','edit','Position',[50 50 50 30],'String','1.5');
edit2 = uicontrol('Parent',d,'Style','edit','Position',[140 50 50 30],'String','0.050');
%     edit3 = uicontrol('Parent',d,'Style','edit','Position',[50 50 60 30],'String','80.0');
%     edit4 = uicontrol('Parent',d,'Style','edit','Position',[120 50 60 30],'String','200.0');


uiwait(d);


    function btn1_callback(btn,event)
        I = img;
        demensions = ndims(I);
        if demensions == 3
            I = rgb2gray(I);
        end
        
        c1=str2double(edit1.String);
        c2=str2double(edit2.String)
        I= c1.^(double(I)*c2)-1;
        I = (I.*255)/(max(max(I)));
        I = uint8(I);
        
        img = I;
        
        delete(gcf);
    end

    function btn2_callback(btn,event)
        delete(gcf);
    end

refresh_img(handles);
end

% --- Executes on button press in pushbutton16.
function pushbutton16_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton16 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global img;
push_img();
[m,n]=size(img);
GL=zeros(1,256);
GLPeq = zeros(1,256);

for k=0:255
    GL(k+1)=length(find(img==k))/(m*n);
end

S1=zeros(1,256);
for i=1:256
    for j=1:i
        S1(i)=GL(j)+S1(i);
    end
end
S2=round((S1*256)+0.5);
for i=1:256
    GLPeq(i)=sum(GL(S2==i));
end

img1=img;
for i=0:255
    img1(img==i)=S2(i+1);
end

img = img1;
refresh_img(handles);
end


% --- Executes during object creation, after setting all properties.
function axes1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to axes1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: place code in OpeningFcn to populate axes1
set(hObject,'xTick',[]);
set(hObject,'ytick',[]);
set(hObject,'box','on');
end


% --- Executes during object creation, after setting all properties.
function axes2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to axes2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: place code in OpeningFcn to populate axes2
set(hObject,'xTick',[0,255]);
set(hObject,'ytick',[0,1]);
set(hObject,'box','on');
set(gca, 'ylim', [0, 1]);
set(gca, 'xlim', [0, 255])

end


% --------------------------------------------------------------------
function Untitled_1_Callback(hObject, eventdata, handles)
% hObject    handle to Untitled_1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
end

% --------------------------------------------------------------------
function redo_Callback(hObject, eventdata, handles)
% hObject    handle to redo (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
pop_img();
refresh_img(handles);
end


% --- Executes on button press in pushbutton17.
function pushbutton17_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton17 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global img;
push_img();
img = imnoise(img,'gaussian',0.02);

refresh_img(handles);
end
% --- Executes on button press in pushbutton18.
function pushbutton18_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton18 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global img;
push_img();
img = imnoise(img,'salt & pepper',0.02);

refresh_img(handles);
end
% --- Executes on button press in pushbutton19.
function pushbutton19_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton19 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global img;
push_img();
img = imnoise(img,'poisson');
refresh_img(handles);
end

% --- Executes on button press in pushbutton20.
function pushbutton20_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton20 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global img;
push_img();

[h, w] = size(img);

for i = 1 : h
    for j = 1 : w
        up = max(i - 1, 1);
        down = min(i + 1, h);
        left = max(j - 1, 1);
        right = min(j + 1, w);
        sub = img(up : down, left : right);
        sub = sub(:);
        img(i, j) = median(sub);
    end
end

refresh_img(handles);
end

% --- Executes on button press in pushbutton21.
function pushbutton21_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton21 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global img;
push_img();
I = img;
[H,W]=size(I);
M=double(I);
J=M;
for  i=2:H-1
    for  j=2:W-1
        J(i,j)=abs(M(i-1,j+1)-M(i-1,j-1)+2*M(i,j+1)-2*M(i,j-1)+M(i+1,j+1)-M(i+1,j-1))+abs(M(i-1,j-1)-M(i+1,j-1)+2*M(i-1,j)-2*M(i+1,j)+M(i-1,j+1)-M(i+1,j+1));
    end
end
img = uint8(J);
refresh_img(handles);

end

% --- Executes on button press in pushbutton22.
function pushbutton22_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton22 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


global img;
push_img();
I=img;
[H,W]=size(I);
M=double(I);
J=M;
for  i=2:H-1
    for  j=2:W-1
        J(i,j)=abs(M(i-1,j+1)-M(i-1,j-1)+M(i,j+1)-M(i,j-1)+M(i+1,j+1) -M(i+1,j-1))+abs(M(i-1,j-1)-M(i+1,j-1)+M(i-1,j)-M(i+1,j)+M(i-1,j+1)-M(i+1,j+1));
    end
end
img = uint8(J);
refresh_img(handles);
end

% --- Executes on button press in pushbutton23.
function pushbutton23_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton23 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global img;
push_img();
J=img;
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
img = g;
refresh_img(handles);
end

% --- Executes on button press in pushbutton24.
function pushbutton24_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton24 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global img;
push_img();

%巴特沃斯高通滤波
J=img;
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
    end
end
gg1=ifftshift(gg1);
gg1=uint8(real(ifft2(gg1)));
img =gg1;

refresh_img(handles);
end

% --- Executes on button press in pushbutton25.
function pushbutton25_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton25 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global img;
push_img();
img = edge(img,'roberts');
refresh_img(handles);
end

% --- Executes on button press in pushbutton26.
function pushbutton26_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton26 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global img;
push_img();
[m,n]=size(img);

imgn=zeros(m,n);  % 边界标记图像
ed=[-1 -1;0 -1;1 -1;1 0;1 1;0 1;-1 1;-1 0];  % 从左上角像素判断
for  i=2:m-1
    for  j=2:n-1
        if  img(i,j)==1  % 如果当前像素为 1
            
            for  k=1:8
                ii=i+ed(k,1);
                jj=j+ed(k,2);
                if  img(ii,jj)==0  % 当前像素周围如果是背景，边界标记图像相应像素标记
                    imgn(ii,jj)=1;
                end
            end
            
        end
    end
end
img=imgn;
refresh_img(handles);
end
% --- Executes on button press in pushbutton27.
function pushbutton27_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton27 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global img;
push_img();
f=double(img);
hv=fspecial( 'prewitt' );  % 建立一个预定义的滤波算子
hh=hv';  % 计算梯度图
gv=abs(imfilter(f,hv, 'replicate' ));
gh=abs(imfilter(f,hh, 'replicate' ));
g=sqrt(gv.^2+gh.^2);  % 计算距离
L=watershed(g);
wr=L==0;
img=wr;
refresh_img(handles);
end


% --- Executes on button press in pushbutton29.
function pushbutton29_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton29 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global img;
push_img();
level=graythresh(img);%使用graythresh计算灰度门槛
bw2=im2bw(img,level);
img = bw2;
refresh_img(handles);
end
