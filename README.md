# zLang
An interpreted WORK IN PROGRESS programming language built with c++.

<img width="180" height="180" alt="zlanglogo" src="https://github.com/user-attachments/assets/8d7c95fd-9e27-43d2-8eda-edb16794cb3e" />

zLang is made in c++ but in the way a C developer would do it - this means no classes, I thought it would be a fun challange.

zLang follows a very STRICT way of typing, spaces are requiered between every keyword and new keywords must be on different lines.
The code input is read via fstream and read line by line

Here are some of its functionalities:

**Declare Variables**

Current types: nr [Number]

```zLang
nr x = 69
-- This is a comment
```

**Increment/decrement Variables**

```zLang
nr x = 10
nr y = x / 10

y += x
y += 1
x -= y
x -= 5
```

**IO**

*Output*

```zLang
nr x = 10
nr y = 15


print this is x: $x and this is y: $y
```

--> Will output

```zLang
this is x: 10 and this is y: 15
```

*Input*

--> Input is: 
```
6 9 
```

```zLang
nr x = 0
nr y = 0

toinput x
toinput y

print $x !n
print $y !n
```

--> Will output

```zLang
6
9
```

**Print new lines**

```zlang 
print Hello !n World !n NewLine
```
--> Will output

```zLang
Hello
 World
  NewLine
```

**Use basic true/false expressions**

```zLang
-- here x will be false
bool x = 5 is 6

-- here x will be true
x = 5 isnt 6

-- this works for: >=,<=,>,< operators.

print $x
```

**Supports operations:**
```zLang
-=;
+=;
/=;
*=;
%=;
&=  
|=  
^=  
<<= 
>>= 
```
