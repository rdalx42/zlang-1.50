# zLang 1.60
An interpreted WORK IN PROGRESS programming language built with c++.

<img width="180" height="180" alt="zlang new logo" src="https://github.com/user-attachments/assets/286edec4-df89-4247-b956-8d699d36149b" />

zLang is made in c++ but in the way a C developer would do it - this means no classes, I thought it would be a fun challange.

runs files with .zl extension

Here are some of its functionalities:

**Declare Variables**

Current types: nr [Number], bool [Boolean]

```zLang
num x = 69
-- This is a comment
```

**IO**

*Output*

```zLang
const num x = 10
num y = 15


print "this is x: $x and this is y: $y"
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
num x = 0
num y = 0

toinput x
toinput y

print "$x \n"
print "$y \n"
```

--> Will output

```zLang
6
9
```

**Print new lines**

```zlang 
print "Hello \n World \n NewLine"
```
--> Will output

```zLang
Hello
 World
  NewLine
```

**Use basic if statements**

```zLang

num x = 10
num y = 15

if y is 15 {

    print $y = 15
} 

bool check_boolean = false

if check_boolean is false {

    print "\n is false"
}

if check_boolean is true {

    print "\n is true"
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
 print "function is true !"
}

```

**while loops**

```zLang

num x = 0

while x < 50 {

 x = x + 1
 print "$x \n"
}
```


**wait specific duration**

```zLang

wait 5
-- will only run after 5 seconds
print "wait completed in seconds"
waitms 5

-- will run after 5 miliseconds

print "wait completed in miliseconds"
```

hi.zl:

```zLang

print "Hiiiiii"
```

**Change console color**

```zLang

color fg 2

wait 2

color bg 1

wait 2

color both 4 6

wait 2

color both 7 0

```


**What's next (zLang 1.70)?**

*zLang will feature support for global variables and strings alongside with support for multiple files*
