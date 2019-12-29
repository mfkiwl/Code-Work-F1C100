function  dialog_rotate
    global img img_r;
    d = dialog('Position',[300 300 250 150],'Name','Ðý×ªÍ¼Ïñ');
    
    txt = uicontrol('Parent',d,...
               'Style','text',...
               'Position',[20 80 210 40],...
               'String','Ðý×ª½Ç¶È');
           
    btn = uicontrol('Parent',d,...
           'Position',[89 20 70 25],...
           'String','Close',...
           'Callback',@btn_callback);
       
    c = uicontrol(d,'Position',[10,10,100,10],'Style','slider','Callback',@slider_callback);
    c.Value = 0.5;
    uiwait(d);
        
    function slider_callback(c,event)
        img_r = imrotate(img,30);
        imshow(img_r)
        disp(c.Value);
    end
    
    function btn_callback(btn,event)
        img = img_r;
        imshow(img)
        delete(gcf);   
    end
end