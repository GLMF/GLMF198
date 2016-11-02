function droite(X,T,W,b,x1min ,x1max ,x2min ,x2max ,i)
//tracer de la droite de séparation
[nx ,mx ]= size(X);
figure(1)
a=get("current_axes");//get the handle of the newly created axes
a.data_bounds=[x1min, x1max, x2min, x2max];
set(gca(),"auto_clear","off") //hold on
//axis([ x1min x1max x2min x2max ])
for k=1:mx
    Tzero =T(k)==0;
    if Tzero
        plot(X(1,k),X(2,k),'o')
    else
        plot(X(1,k),X(2,k),'+')
    end
  title([ 'Vecteurs à classifier et droite de séparation - époque : ' string(i)])
  xlabel('x1 ')
  ylabel('x2 ')
  set(gca(),"grid",[1 1])
  end
//tracé de la droite de separation

xx = a.data_bounds(:,1)'; //xx=get(gca ,'xlim');
y=(- xx*W(1) -b)/W(2);
plot(xx ,y,'r')
set(gca(),"auto_clear","on")//hold off 
endfunction
