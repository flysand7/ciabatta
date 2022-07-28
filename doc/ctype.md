
# ctype.h - Classification of ASCII characters

Function declarations:

- [isalnum](#isalnum)
- [isalpha](#isalpha)
- [isblank](#isblank)
- [iscntrl](#iscntrl)
- [isdigit](#isdigit)
- [isgraph](#isgraph)
- [islower](#islower)
- [isprint](#isprint)
- [ispunct](#ispunct)
- [isspace](#isspace)
- [isupper](#isupper)
- [isxdigit](#isxdigit)
- [tolower](#tolower)
- [toupper](#toupper)

## [isalnum](#isalnum)

<details><summary>Description</summary>

Function checks whether given integer is an ASCII code representing a letter or
a digit. These characters are lowercase letters ('a'..'z'), uppercase letters
('A'..'Z') and digits ('0'..'9').

</details>

<details><summary>Declaration</summary>

```c
int isalnum(int c);
```

</details>

## [isalpha](#isalpha)

<details><summary>Description</summary>

Function checks whether given integer is an ASCII code representing a letter.
These characters are lowercase letters ('a'..'z') and uppercase letters.

</details>

<details><summary>Declaration</summary>

```c
int isalpha(int c);
```

</details>

## [isblank](#isblank)

<details><summary>Description</summary>

Checks whether a character is a space (' ') or a horizontal tab ('\t')

</details>

<details><summary>Declaration</summary>

```c
int isblank(int c);
```

</details>

## [iscntrl](#iscntrl)

<details><summary>Description</summary>

Checks whether a character is a control character
('\x00' through '\x1f' and '\x7f').

</details>

<details><summary>Declaration</summary>

```c
int iscntrl(int c);
```

</details>

## [isdigit](#isdigit)

<details><summary>Description</summary>

Checks whether a character is a digit ('0' through '9')

</details>

<details><summary>Declaration</summary>

```c
int isdigit(int c);
```
</details>

## [isxdigit](#isxdigit)

<details><summary>Description</summary>

Checks whether a character is a hexadecimal digit ('0' through '9', 'a' through 'f' or 'A' through 'F')

</details>

<details><summary>Declaration</summary>

```c
int isxdigit(int c);
```
</details>

## [isgraph](#isgraph)

<details><summary>Description</summary>

Checks whether a character is a

- Number;
- Letter;
- or punctuation

</details>

<details><summary>Declaration</summary>

```c
int isgraph(int c);
```
</details>

## [islower](#islower)

<details><summary>Description</summary>

Checks whether a character is a lowercase letter ('a' through 'z')

</details>

<details><summary>Declaration</summary>

```c
int islower(int c);
```
</details>

## [isupper](#isupper)

<details><summary>Description</summary>

Checks whether a character is a uppercase letter ('A' through 'Z')

</details>

<details><summary>Declaration</summary>

```c
int islower(int c);
```
</details>

## [isprint](#isprint)

<details><summary>Description</summary>

Checks whether a character is a

- Number;
- Letter;
- Space;
- or punctuation

</details>

<details><summary>Declaration</summary>

```c
int isprint(int c);
```
</details>

## [ispunct](#ispunct)

<details><summary>Description</summary>

Checks whether a character is a punctuation character (one of `!"#$%&'()*+,-./:;<=>?@[\]^_{|}~`
or backtick).

</details>

<details><summary>Declaration</summary>

```c
int ispunct(int c);
```
</details>

## [isspace](#isspace)

<details><summary>Description</summary>

Function checks whether given integer is a whitespace character. These
characters include whitespace (' '), form feed ('\f'), line feed ('\n'),
carriage return ('\r'), horizontal and vertical tab ('\t' and '\v').

</details>

<details><summary>Declaration</summary>

```c
int isspace(int c);
```

</details>

## [tolower](#tolower)

<details><summary>Description</summary>

If the character is an uppercase letter returns lowercase version of it. Otherwise
returns the character unmodified.

</details>

<details><summary>Declaration</summary>

```c
int tolower(int c);
```
</details>

## [toupper](#toupper)

<details><summary>Description</summary>

If the character is an lowercase letter returns uppercase version of it. Otherwise
returns the character unmodified.

</details>

<details><summary>Declaration</summary>

```c
int toupper(int c);
```
</details>
