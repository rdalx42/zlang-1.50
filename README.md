# zLang
An interpreted WORK IN PROGRESS programming language built with c++

zLang follow a very STRICT way of typing, spaces are requiered between every keyword

Here are some of its functionalities:

**Declare Variables**

Current types: nr [Number]

```zLang
nr x = 69
```

**Increment/decrement Variables**

```zLang
nr x = 10
nr y = x * 50

y += x
y += 1
x -= y
x /= 5
```

**Print Text & Variables**

```zLang
nr x = 10
nr y = 15


print this is x: $x and this is y: $y
```

--> Will output

```zLang
this is x: 10 and this is y: 15
```

**Capabilities**

Operations:

// Arithmetic
+   
-   
*   
/   
%   

// Compound assignment (arithmetic)
+=  
-=  
*=  
/=  
%=  

// Bitwise
&   
|   
^   
<<  
>>  

// Compound assignment (bitwise)
&=  
|=  
^=  
<<= 
>>= 
