
booleano1 = 1;
booleano2 = 0;
respuesta='N';

t = tcpip('0.0.0.0', 60001, 'NetworkRole', 'server');
t.InputBufferSize = 4000;
t.Timeout = 90;
fopen(t);

while (booleano1)
      buffsize =1;    
      comando = fread(t,buffsize,'char'); 
      comando
      drawnow
            
      
       % if (comando == 'E')     
        %    display('Vitalsgrafr Interfaz no recibio aun el mensaje')
        
        %end
        
        if (comando == 'S')    %
          display('Vitalsgrafr Interfaz recibio S')
          booleano1 = 0;
          display ('enviando configuraciones');
          valueECGmin = evalin('base', 'valueECGmin');
          valueECGmin
          valueECGmax = evalin('base', 'valueECGmax');
          valueECGmax
          valueIBPmin = evalin('base', 'valueIBPmin');
          valueIBPmin
          valueIBPmax = evalin('base', 'valueIBPmax');
          valueIBPmax
          valueSPO2max = evalin('base', 'valueSPO2max');
          valueSPO2max
          valueSPO2max = evalin('base', 'valueSPO2max');
          valueSPO2max
          valueHour = evalin('base', 'valueHour');
          valueHour
          valueMinute = evalin('base', 'valueMinute');
          valueMinute
         
          config = zeros(1,8);
          config(1) = valueECGmin
          config(2) = valueECGmax
          config(3) = valueIBPmin
          config(4) = valueIBPmax
          config(5) = valueSPO2min
          config(6) = valueSPO2max
          config(7) = valueHour
          config(8) = valueMinute
         
          fwrite(t,config);
          
          
          comando = fread(t,buffsize,'char'); 
              if (comando == 'F')
                 booleano1 = 0;
                 display('Vitalsgrafr recibio configuraciones de alarmas y tiempo');
              end
          
        end             
end

fclose(t)


