# zLang 1.50+
An interpreted WORK IN PROGRESS programming language built with c++.

<img width="180" height="180" alt="Zlang Version 1.50+" src="https://github.com/user-attachments/assets/bcd7b942-b35b-46fd-90a9-0ef1d72c3a09" />
zLang is made in c++ but in the way a C developer would do it - this means no classes, I thought it would be a fun challange.

runs file with .zl extension

Here are some of its functionalities:

**Declare Variables**

Current types: nr [Number], bool [Boolean]

```zLang
nr x = 69
-- This is a comment
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
print Hello !n World \n NewLine
```
--> Will output

```zLang
Hello
 World
  NewLine
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

**while loops**

```zLang

nr x = 0

while x < 50 {

 x = x + 1
 print $x !n
}
```


**wait specific duration**

```zLang

wait 5
-- will only run after 5 seconds
print wait completed in seconds
waitms 5

-- will run after 5 miliseconds

print wait completed in miliseconds
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


**What's next (zLang 1.60+)?**

*The import keyword will be added*

*zLang will feature support for global variables and strings alongside with support for multiple files*
