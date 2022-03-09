Declare R Integer  
Declare ihfusd Integer  
Declare b Integer  
Declare Temp&1 Integer  
ADD ihfusd b Temp&1 
Declare r Integer  
Declare k Integer  
Declare d Integer  
Declare Temp&2 Integer  
Cmp r 0  
Jnz then1   
Jmp else1   
then1: k Temp&2  
Jmp continue1   
else1: d Temp&2  
continue1:    
Declare e Integer  
Declare Temp&3 Integer  
SUB d e Temp&3 
Declare Temp&4 Integer  
ADD Temp&3 b Temp&4 
Declare Temp&5 Integer  
Cmp Temp&1 0  
Jnz then2   
Jmp else2   
then2: Temp&2 Temp&5  
Jmp continue2   
else2: Temp&4 Temp&5  
continue2:    
Store Temp&5 R  
Halt    
