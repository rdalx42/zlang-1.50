# zLang 1.50
An interpreted WORK IN PROGRESS programming language built with c++.

<img width="180" height="180" alt="logo" src="https://github.com/user-attachments/assets/b5fb8bff-e3c8-4d8e-a852-a224cf935272" />

zLang is made in c++ but in the way a C developer would do it - this means no classes, I thought it would be a fun challange.

runs file with .zl extension

Here are some of its functionalities:

**Declare Variables**

Current types: nr [Number], bool [Boolean]

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

**Use basic if statements**

```zLang

nr x = 10
nr y = 15

if y is 15 {

    print $y = 15
}

bool check_boolean = x is y

if check_boolean is false {

    print !nis false
}

if check_boolean is true {

    print !nis true
}

```

**make functions**

```zLang

fn foo : boolean {
 if boolean is true {
  return true
 }
}

fire foo : true

bool fooAns = :foo
if fooAns is true {
 print function is true !
}

```


**wait specific duration**

```zLang

wait 5
-- will only run after 5 seconds

print wait completed.
```

**Change console color**

```zLang
-- Set foreground color to light green (2)
color fg 2

-- Wait for 2 seconds to let user see the effect
wait 2

-- Set background color to blue (1) and keep foreground white (7)
color bg 1

wait 2

-- Set both foreground and background (red on yellow)
color both 4 6

wait 2

-- Reset to default (white on black)
color both 7 0

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
