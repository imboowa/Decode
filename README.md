## Decode: Decode ##
This is a project that I ctook up after failing to understand ARM Assembly from a course at Low Level Academy. I took this course a few months ago and gained the true knowledge of how computers work under the hood. I also had Python and C knowledge so I did a project in Python and then took C to make a simple semi-programming language that looked like Assembly.

## Dive Into ##
**About**

Decode is a semi-programming language that I have created and tested on simple mathematical algorithms like fibonacci, exponentiation, sum of even number between X and Y, and many more. Basically, I took my C knowledge from Low Level Academy course and my simple understanding of ARM Assembly and combined those skills and decided to make a simple programming language that looks somehow like ARM Assembly.

`Note: This project was 35% AI and 65% me`

**Features**
1. Verifies if file is a .ggh file
2. Executes code line by line
3. Reads code and arranges it in memeory to attain 0(1) in execution, random access, and jump to index
4. printr: prints a variable value or sentence on screen
5. set: initializes variables to a value
6. //: for comments
7. add: to add two numbers and stores in a variable
8. sub: to subtract two numbers and stores in a variable
9. mul: to multiply two numbers and stores in a variable
10. div: to divide two numbers and stores in a variable
11. when: to do while loop stuff - end
12. if: to do if statement stuff - done

**Future Prospects**
1. To integrate it into my DNAGenre project to do math operations on massive data.
2. To introduce more features like average, mode, median, string operations, standard deviation.

## Bugs Found ##
**comp.c**
``` C
int checkFile(char *fileName, char *realExtension)
{
    strtok(fileName, ".");
    char *extension = strtok(NULL, ".");
    
    if (strlen(realExtension) != strlen(extension))
    {
        return ERROR;
    }
    if (strcmp(extension, realExtension) == 0)
    {
        return GOOD;
    }
    return ERROR;
}
```
**1. strtok**
When passing the `fileName` to `strtok`, `strtok` overwrites the `char *` and inserts `NULL` at locations of the `.` in this case. To avoid this, I stored the file name in a temporary buffer and used the original but later I can use the file name in the temporary buffer.

**2. strlen**
Comparing buffers without visible length is bad practice. In order to prevent this in my code, I decided to check if both `realExtension` and `extension` have the same length.

**3. strcmp**
One may say that these buffers do not have `\0` but remember that when `strtok` is called it returned a value into `extension` that is NULL terminated. This NULL terminator was placed during `strtok` finding occurrences of `.` in filename and replacing them with `\0`.


``` C
for (int i = 0; i < (*varsCount); i++)
{
    // Check If var1 Is A Variable
    if (strcmp(vars[i].name, var1) == 0)
    {
        var1 = vars[i].value;
    }
    // Check If var2 Is A Variable
    if (strcmp(vars[i].name, var2) == 0)
    {
        var2 = vars[i].value;
    }
}
```
**1. Looking For tempVar1 And tempVar2 In Our Array**
When looping in the predefined array that only allows 10 variables. What if the if statements were as follows; an if statement and then an else if statement.
```
set x 0
add x x x
printr x
```
The loop will find `x` in its `vars` and store `x` value in tempVar1. Since we have another if statement to check once more, we get the right `x` and store its value in tempVar2.
**Note: If we had if-else if statements, the loop would check and if block runs then the else if block never runs; right there we miss our only shot.**

``` C
// Check If It Is Safe To Add A Variable
if ((*varsCount) >= 0 && (*varsCount) < VARIABLE_ARRAY_LENGTH)
{
    // Add The Variable
    strncpy(vars[(*varsCount)].name, tempResult, sizeof(vars[(*varsCount)].name) - 1);
    strncpy(vars[(*varsCount)].value, buffer, sizeof(vars[(*varsCount)].value) - 1);
    vars[(*varsCount)].name[sizeof(vars[(*varsCount)].name) - 1] = '\0';
    vars[(*varsCount)].value[sizeof(vars[(*varsCount)].value) - 1] = '\0';
    (*varsCount)++;
}
else
{
    printf("Varaible Error: Variable Array Full\n");
}
```
**1. Index Must Match Array Bounds**
`*varsCount` cannot be controlled but can be managed by always checking if it is within the bounds of the `vars` so that no out of bounds happen, I copy to the array in valid indexes.

**main.c**
``` C
while (execCounter >= 0 && execCounter < sizeof(codeLines)/sizeof(codeLines[0]) && codeLines[execCounter][0] != 0)
{
    // Buffer To Store A Temporary Copy Of Current Line Of Code, Can Be Mutated
    char lineCopy[BUFFER_LENGTH];
    memset(lineCopy, 0, sizeof(lineCopy));
    strncpy(lineCopy, codeLines[execCounter], sizeof(lineCopy) - 1);
    lineCopy[sizeof(lineCopy) - 1] = '\0';
    executeLine(lineCopy, vars, &varsCount, &execCounter, &tempIndex, tempValue, &IF_FLAG);
    execCounter++;
}
```

**1. Short Circuit**
First in the `while` loop condition, we check if we are greater than or equal to 0, then check if we are less than 100 lines of code (our limit), and lastly we check if the current index is out full stop. Imagine we had not respected the order and checked if the current index we are on is our full stop and `execCounter` is 100, right there we are out of bounds hence order matters.

## Usage ##
filedemo.ggh
1. `Sum of first N numbers`
```
set x 5
set y 0
when x
add y y x
sub x x 1
end
printr y
```
**Walkthrough**
First, set x equal to 5
Then, set y equal to 0 - this is our sum variable
Second, create a while loop that loops as long as x is not 0
Third, add whatever is in x at the iteration to y
Fourth, decrement x by 1
Lastly, signal end of loop and show answer
**Note: ** end checks if x is 0 but the when does not check x

2. `Multiplication using only addition`
```
set x 4
set y 3
set z 0
when y
add z z x
sub y y 1
end
printr z
```
**Walkthrough**
First, set x equal to 4
Then, set y equal to 3
Then, set z equal to 0 - this is our sum variable
Second, create a while loop that loops as long as y is not 0
Third, on every iteration add the multiplicant to z in order to accumulate
Fourth, decrement the multiplier by 1
Lastly, signal end of loop and show answer
**Note: ** end checks if y is 0 but the when does not check y

3. `Factorial of a number`
```
set x 7
set e 1
when x
mul e e x
sub x x 1
end
printr e
```
**Walkthrough**
First, set x equal to 7
Then, set e equal to 1
Second, create a while loop to loop as long as x is not 0
Third, multiply the value in e by the value in x and store that into e
Fourth, decrement x by 1
Lastly, signal end of loop and show answer
**Note: ** end checks if x is 0 but the when does not check x

4. `Count even and odd numbers between 1 and n`
```
set e 0
set o 0
set x 5
set c 0
set a 0
set b 0
when x
div c x 2
mul a c 2
sub b x a
if b 1
add o o 1
printr x
printr Is Odd
done
if b 0
add e e 1
printr x
printr Is Even
done
sub x x 1
end
printr e
printr o
```
**Walkthrough**
First, set e equal to 0
Then, set o equal to 0
Then, set x equal to 5
Then, set c equal to 0 - carries the first division result in the modulus process
Then, set a equal to 0 - carries quotient multiplied by 2
Then, set b equal to 0 - stores remainder
Second, create a while loop to loop as long as x is not 0
Then, divide x by 2 and store the result into c
Then, multiply the value in c by 2 and store result in a
Then, subtract x from a to get remainder
Third, if remainder is 1 that is an odd number hence increment the odd counter `o` by 1
Fourth, if remainder is 0 that is an even number hence increment the even counter `e` by 1
Fifth, decrement x by 1
Lastly, signal end of loop and show results
**Note: ** end checks if x is 0 but the when does not check x

5. `Sum of all even numbers between X and Y`
```
set x 1
set y 10
set z 0
set a 0
set b 0
set c 0
set d 0
when z
div b y 2
mul c b 2
sub d y c
if d 0
add a a y
done
sub z y x
sub y y 1
end
printr a
```
**Walkthrough**
First, set x equal to 1
Then, set y equal to 10
Then, set z equal to 0
Then, set a equal to 0
Then, set b equal to 0 - stores quotient of current y divided by 2
Then, set c equal to 0 - stores result of b multiplied by 2
Then, set d equal to 0 - stores the remainder
Second, create a while loop that runs as long as z is not 0
Then, divide current y by 2
Then, multipy b by 2
Then, subtract y from b to get the remainder
Third, if remainder is 0 that is an even number hence increment the even counter `a` by y (that very even number)
Fourth, decrement y by 1
Fifth, decrement y by 1
Lastly, signal end of loop and show results
**Note: ** end checks if z is 0 but the when does not check z

6. `Print a range`
```
set x 3
when x
printr x
sub x x 1
end
```
**Walkthrough**
First, set x equal to 3
Second, create a while loop that loops as long as x is not 0
Then, show result
Then, decrement x by 1
Lastly, signal end of loop
**Note: ** end checks if z is 0 but the when does not check z


tryguest.ggh
1. `Division using subtraction on only divisible numbers`
```
set x 90
set y 10
set z 0
when x
add z z 1
sub x x y
end
printr z
```
**Walkthrough**
First, set x equal to 90
Then, set y equal to 10
Then, set z equal to 0 - increments to show the divisions
Second, create a while loop that loops as long as x is not 0
Third, add 1 to the value in z
Fourth, decrement x by 1
Lastly, signal end of loop and show results
**Note: ** end checks if x is 0 but the when does not check x

2. `Exponentiation using loops`
```
set x 8
set y 2
set z 1
when y
mul z z x
sub y y 1
end
printr z
```
**Walkthrough**
First, set x equal to 8
Then, set y equal to 2
Then, set z equal to 1 - accumulates the multiplication as many times as the power in this case the value of y
Second, create a while loop that loops as long as y is not 0
Third, multiply the value in z by the value in x
Fourth, decrement y by 1
Lastly, signal end of loop and show results
**Note: ** end checks if y is 0 but the when does not check y

3. `Fibonacci`
```
set x 0
set y 1
set z 0
set a 10
when a
add z x y
set x y
set y z
printr x
sub a a 1
end
```
**Walkthrough**
First, set x equal to 0 - the first number
Then, set y equal to 1 - the second number
Then, set z equal to 0 - contains the sum of x and y
Then, set a equal to 10
Second, create a while loop that loops as long as a is not 0
Third, fill z with the sum of x and y
Fourth, set x to be y
Fifth, set y to be z
Lastly, show results and signal end of loop
**Note: ** end checks if a is 0 but the when does not check a

4. `Decimal to binary LSB to MSB`
```
set x 2
set y 0
set z 0
set a 0
when x
div y x 2
mul z y 2
sub a x z
printr a
set x y
end
```
**Walkthrough**
First, set x equal to 2 - the number
Then, set y equal to 0 - stores quotient
Then, set z equal to 0
Then, set a equal to 0 - stores remainder
Second, create a while loop that loops as long as x is not 0
Then, store quotient into y
Then, multiply that quotient by 2
Then, store the remainder into a
Then, show the remainder
Then, set x equal to y
Lastly, signal end of loop
**Note: ** end checks if x is 0 but the when does not check x

5. `Reverse a number`
```
set x 50313
set y 0
set z 0
set a 0
when x
div y x 10
mul z y 10
sub a x z
printr a
set x y
end
```
**Walkthrough**
First, set x equal to 50313
Then, set y equal to 0 - stores quotient from integer division
Then, set z equal to 0
Then, set a equal to 0
Second, create a while loop that loops as long as x is not 0
Then, store quotient into y
Then, multiply y by 10
Then, get the last number by subtracting original from the value in z
Then, show the last number
Third, set x equal to the value in y
Lastly, signal end of loop
**Note: ** end checks if x is 0 but the when does not check x


## Tech Stack ##
```
Xcode - Code Editor
ChatGPT - AI Assistant
```

## Contributing ##
```
If you are interested in making this Lnaguage, Decode, a real mathematical language for the future, please feel at ease to reach out in order to contribute to the project.
```

## Copyright ##
```
All rights are given to anyone who is going to use this project for their personal work, research, educational purposes but not unethical purposes.
```

## Contact ##
LinkedIn - [My LinkedIn](https://www.linkedin.com/in/imboowa/)

Email - emboowae@gmail.com
