<h3>Before we begin, learn about <b>getchar()</b> and <b>fread()</b></h3>

<b>Getchar()</b> and <b>fread()</b> are two library calls from the C standard I/O library.
You will be using them in this lab, so let me explain them to you.
<p>
The simplest is <b>getchar()</b>.  It reads one byte from standard input and returns it
as a signed integer.  If there are no bytes in standard input, it returns the value
<b>EOF</b>, which is typically defined in <b>stdio.h</b> to equal -1.  Thus, when it reads
a character, it will return a value between 0 and 255, and when it fails, it returns <b>EOF</b>.
<p>
The program
<b><a href=read10-getchar.c>read10-getchar.c</a></b> reads up to ten bytes from standard input
using getchar, and prints them in up to four ways:

<UL>
<LI> As an <b>unsigned char</b> -- that will be a number between 0 and 255.
<LI> As a <b>char</b> -- that will be a number between -128 and 127.
<LI> In hexadecimal.  That will be a two-digit number.
<LI> If the C library procedure <b>isprint()</b> returns that the byte is a printable character,
     then it is printed as a character.
</UL>

The program is straightforward.  You need <b>&lt;ctype.h&gt;</b>, because it defines
<b>isprint()</b>:

<p><center><table border=3 cellpadding=3><td><pre>
#include &lt;stdio.h&gt;
#include &lt;stdlib.h&gt;
#include &lt;ctype.h&gt;

void main(int argc, char **argv)
{
  FILE *f;
  int i, nread;
  int c;

  if (argc != 1) {
    fprintf(stderr, "usage: read10 (file on standard input)\n");
  }

  for (i = 0; i &lt; 10; i++) {
    c = getchar();
    if (c == EOF) {
      if (i == 0) printf("The input is empty.\n");
      exit(0);
    }
    printf("Byte %d: Unsigned Decimal: %3d. Signed: %4d.  Hex: %02x.", i, 
           (unsigned char) c, (char) c, (unsigned int) c);
    if (isprint(c)) printf("  Character: %c", c);
    printf("\n");
  }
  exit(0);
}
</pre></td></table></center><p>
  
Let's see it in action:

<pre>
UNIX> <font color=darkred><b>echo abcd | read10-getchar</b></font>
Byte 0: Unsigned Decimal:  97. Signed:   97.  Hex: 61.  Character: a
Byte 1: Unsigned Decimal:  98. Signed:   98.  Hex: 62.  Character: b
Byte 2: Unsigned Decimal:  99. Signed:   99.  Hex: 63.  Character: c
Byte 3: Unsigned Decimal: 100. Signed:  100.  Hex: 64.  Character: d
Byte 4: Unsigned Decimal:  10. Signed:   10.  Hex: 0a.
UNIX>
</pre>

It detected that input ended after the newline character (10) when <b>getchar()</b>
returned <b>EOF</b>.

<pre>
UNIX> <font color=darkred><b>echo "a 1 # )" | read10-getchar</b></font>
Byte 0: Unsigned Decimal:  97. Signed:   97.  Hex: 61.  Character: a
Byte 1: Unsigned Decimal:  32. Signed:   32.  Hex: 20.  Character:  
Byte 2: Unsigned Decimal:  49. Signed:   49.  Hex: 31.  Character: 1
Byte 3: Unsigned Decimal:  32. Signed:   32.  Hex: 20.  Character:  
Byte 4: Unsigned Decimal:  35. Signed:   35.  Hex: 23.  Character: #
Byte 5: Unsigned Decimal:  32. Signed:   32.  Hex: 20.  Character:  
Byte 6: Unsigned Decimal:  41. Signed:   41.  Hex: 29.  Character: )
Byte 7: Unsigned Decimal:  10. Signed:   10.  Hex: 0a.
UNIX> 
</pre>

You'll see that <b>isprint()</b> says that spaces are "printable," but newlines are "not."

<pre>
UNIX> <font color=darkred><b>read10-getchar < read10-getchar.c</b></font>
Byte 0: Unsigned Decimal:  35. Signed:   35.  Hex: 23.  Character: #
Byte 1: Unsigned Decimal: 105. Signed:  105.  Hex: 69.  Character: i
Byte 2: Unsigned Decimal: 110. Signed:  110.  Hex: 6e.  Character: n
Byte 3: Unsigned Decimal:  99. Signed:   99.  Hex: 63.  Character: c
Byte 4: Unsigned Decimal: 108. Signed:  108.  Hex: 6c.  Character: l
Byte 5: Unsigned Decimal: 117. Signed:  117.  Hex: 75.  Character: u
Byte 6: Unsigned Decimal: 100. Signed:  100.  Hex: 64.  Character: d
Byte 7: Unsigned Decimal: 101. Signed:  101.  Hex: 65.  Character: e
Byte 8: Unsigned Decimal:  32. Signed:   32.  Hex: 20.  Character:  
Byte 9: Unsigned Decimal:  60. Signed:   60.  Hex: 3c.  Character: <
UNIX>
</pre>

In the run above, we have redirected <b>read10-getchar.c</b> to standard input, and it has
printed the first 10 bytes in all four formats.

<pre>
UNIX> <font color=darkred><b>read10-getchar < Dog.jpg</b></font>
Byte 0: Unsigned Decimal: 255. Signed:   -1.  Hex: ff.
Byte 1: Unsigned Decimal: 216. Signed:  -40.  Hex: d8.
Byte 2: Unsigned Decimal: 255. Signed:   -1.  Hex: ff.
Byte 3: Unsigned Decimal: 224. Signed:  -32.  Hex: e0.
Byte 4: Unsigned Decimal:   0. Signed:    0.  Hex: 00.
Byte 5: Unsigned Decimal:  16. Signed:   16.  Hex: 10.
Byte 6: Unsigned Decimal:  74. Signed:   74.  Hex: 4a.  Character: J
Byte 7: Unsigned Decimal:  70. Signed:   70.  Hex: 46.  Character: F
Byte 8: Unsigned Decimal:  73. Signed:   73.  Hex: 49.  Character: I
Byte 9: Unsigned Decimal:  70. Signed:   70.  Hex: 46.  Character: F
UNIX> <font color=darkred><b></b></font>
</pre>

The final call above shows what happens when you get a byte between 128 and 255.  Those
bytes aren't in standard printable text files, but they are in files like binary program
files and JPG files.  
  
<p>
<b>Fread()</b> is a way to read multiple bytes at a time from a file.  You call it as:

<p><center><table border=3 cellpadding=3><td><pre>
long fread(void *ptr, long size, long nmemb, FILE *stream);
</pre></td></table></center><p>

This says to read <b>size*nmemb</b> bytes from the "stream" called <b>stream</b>, and put
those bytes into the memory denoted by <b>ptr</b>.  The two parameters are specified so that
you can ask for <b>nmemb</b> items each of whose size is <b>size</b>.
It returns the number of items actually
read.  If you call it with <b>size</b> equal to one, then it returns the number of bytes
that it read.  If, for example, the input stream is a file and there are not <b>nmemb</b> items
in the file, it simply reads the items that are in the file and returns that number.  It
returns 0 if there are no items to read.
<p>
As usual with C, you have to already have the bytes allocated.   You can use the global
variable <b>stdin</b> (which is defined in <b>stdio.h</b>) to read from standard input.
If you want to read from a file, you can create a "stream" using <b>fopen()</b>.  
<p>
The program below, in 
<b><a href=read10-fread.c>read10-fread.c</a></b>,
performs identically to <b>read10-getchar.c</b>, except it uses <b>fread()</b> to read all
of the characters at one time, and it allows you to specify a file to read from on the command
line:

<p><center><table border=3 cellpadding=3><td><pre>
#include &lt;stdio.h&gt;
#include &lt;stdlib.h&gt;
#include &lt;fcntl.h&gt;
#include &lt;ctype.h&gt;

void main(int argc, char **argv)
{
  FILE *f;
  int i, nread;
  unsigned char buf[10];

  if (argc != 2) {
    fprintf(stderr, "usage: read10 file(- for standard input)\n");
    exit(1);
  }

  if (strcmp("-", argv[1]) == 0) {
    f = stdin;
  } else {
    f = fopen(argv[1], "r");
    if (f == NULL) { perror(argv[1]); exit(1); }
  }

  nread = fread(buf, 1, 10, f);
  if (nread == 0) {
    printf("The input is empty.\n");
  } else if (nread &lt; 0) {
    perror("fread");
    exit(1);
  } else {
    for (i = 0; i &lt; nread; i++) {
      printf("Byte %d: Unsigned Decimal: %3d. Signed: %4d.  Hex: %02x.", i,
         buf[i], (char) buf[i], (unsigned int) buf[i]);
      if (isprint(buf[i])) printf("  Character: %c", buf[i]);
      printf("\n");
    }
  }
  exit(0);
}
</pre></td></table></center><p>

Here are the same four calls as above, only using <b>read10-fread</b>:

<pre>
UNIX> <font color=darkred><b>echo abcd | read10-fread -</b></font>
Byte 0: Unsigned Decimal:  97. Signed:   97.  Hex: 61.  Character: a
Byte 1: Unsigned Decimal:  98. Signed:   98.  Hex: 62.  Character: b
Byte 2: Unsigned Decimal:  99. Signed:   99.  Hex: 63.  Character: c
Byte 3: Unsigned Decimal: 100. Signed:  100.  Hex: 64.  Character: d
Byte 4: Unsigned Decimal:  10. Signed:   10.  Hex: 0a.
UNIX> <font color=darkred><b>echo "a 1 # )" | read10-fread -</b></font>
Byte 0: Unsigned Decimal:  97. Signed:   97.  Hex: 61.  Character: a
Byte 1: Unsigned Decimal:  32. Signed:   32.  Hex: 20.  Character:  
Byte 2: Unsigned Decimal:  49. Signed:   49.  Hex: 31.  Character: 1
Byte 3: Unsigned Decimal:  32. Signed:   32.  Hex: 20.  Character:  
Byte 4: Unsigned Decimal:  35. Signed:   35.  Hex: 23.  Character: #
Byte 5: Unsigned Decimal:  32. Signed:   32.  Hex: 20.  Character:  
Byte 6: Unsigned Decimal:  41. Signed:   41.  Hex: 29.  Character: )
Byte 7: Unsigned Decimal:  10. Signed:   10.  Hex: 0a.
UNIX> <font color=darkred><b>read10-fread read10-getchar.c</b></font>
Byte 0: Unsigned Decimal:  35. Signed:   35.  Hex: 23.  Character: #
Byte 1: Unsigned Decimal: 105. Signed:  105.  Hex: 69.  Character: i
Byte 2: Unsigned Decimal: 110. Signed:  110.  Hex: 6e.  Character: n
Byte 3: Unsigned Decimal:  99. Signed:   99.  Hex: 63.  Character: c
Byte 4: Unsigned Decimal: 108. Signed:  108.  Hex: 6c.  Character: l
Byte 5: Unsigned Decimal: 117. Signed:  117.  Hex: 75.  Character: u
Byte 6: Unsigned Decimal: 100. Signed:  100.  Hex: 64.  Character: d
Byte 7: Unsigned Decimal: 101. Signed:  101.  Hex: 65.  Character: e
Byte 8: Unsigned Decimal:  32. Signed:   32.  Hex: 20.  Character:  
Byte 9: Unsigned Decimal:  60. Signed:   60.  Hex: 3c.  Character: <
UNIX> <font color=darkred><b>read10-fread Dog.jpg</b></font>
Byte 0: Unsigned Decimal: 255. Signed:   -1.  Hex: ff.
Byte 1: Unsigned Decimal: 216. Signed:  -40.  Hex: d8.
Byte 2: Unsigned Decimal: 255. Signed:   -1.  Hex: ff.
Byte 3: Unsigned Decimal: 224. Signed:  -32.  Hex: e0.
Byte 4: Unsigned Decimal:   0. Signed:    0.  Hex: 00.
Byte 5: Unsigned Decimal:  16. Signed:   16.  Hex: 10.
Byte 6: Unsigned Decimal:  74. Signed:   74.  Hex: 4a.  Character: J
Byte 7: Unsigned Decimal:  70. Signed:   70.  Hex: 46.  Character: F
Byte 8: Unsigned Decimal:  73. Signed:   73.  Hex: 49.  Character: I
Byte 9: Unsigned Decimal:  70. Signed:   70.  Hex: 46.  Character: F
UNIX> <font color=darkred><b></b></font>
</pre>

You now know enough about <b>getchar()</b> and <b>fread()</b> to do this lab.

<hr>
<h3>data_uncompress.c - using the C standard I/O library</h3>

Your job is to write the program <b>data_uncompress</b>, using the C standard I/O library.
In particular, you should read using <b>getchar()</b> and <b>fread()</b>.
<p>
This program assumes that standard input is in a compressed format.  The format is a stream of 
bytes as follows.  You read a character, and that character tells you how to proceed:
<p>
<UL>
<LI>  If the character is not 'n', 's', 'i' or 'd', then the input is not correct.  
<LI>  If the character is 'n', then print a newline.
<LI>  If the character is 'i', 'd' or 's', then the next character needs to be interpreted as an integer
(unsigned char).
Call this integer <b>n</b>.  Then you are going to read
<b>n+1</b> contiguous integers, doubles or strings, depending on whether the initial character 
was 'i', 'd' or 's'.  Integers are 4 bytes and doubles are 8 bytes.  Strings are in a special 
format.  You read a character <b>size</b> which you interpret as an integer (again -- unsigned char).  
The string is in the remaining <b>size+1</b> characters.
Obviously, since <b>size</b> and <b>n</b> are unsigned chars, their values will be limited to 255 or
less.
<p>
You are going to print each integer, double or string on the same line, separated by a single space.
You only print a newline when you see the 'n'.  
Do not start a new line with a space. 
Simply print integers with "%d".  Print doubles
with "%.10lg" (that's percent-dot-one-zero-ell-gee).
Continue reading in this vein until you reach EOF.  To reiterate, you 
only print a newline when you see the 'n' character.
</UL>

Here's an example file in <b>comp-1.txt</b> -- each cell of the table is a character of the file:

<p><center><table border=3 cellpadding=3>
<tr>
<td>115 ('s')</td>
<td>1</td>
<td>2</td>
<td>74 ('J')</td>
<td>105 ('i')</td>
<td>109 ('m')</td>
<td>4 </td>
<td>80 ('P')</td>
</tr><tr>
<td>108 ('l')</td>
<td>97 ('a')</td>
<td>110 ('n')</td>
<td>107 ('k')</td>
<td>110 ('n')</td>
<td>115 ('s')</td>
<td>0 </td>
<td>4 </td>
</tr><tr>
<td>67 ('C')</td>
<td>83 ('S')</td>
<td>51 ('3')</td>
<td>54 ('6')</td>
<td>48 ('0')</td>
<td>110 ('n')</td></tr>
</table></center><p></td>

When you uncompress this, you first see the 's', and then a one.  That means you need to read two strings.
The first consists of three characters, which are 'J', 'i' and 'm', and the second consists of five 
characters  - "Plank".  You print "Jim", a space and "Plank."  Then you read a 'n', and print a newline.
Next you read a 's' and the number 0, which means to read one string.  That string has five characters:
"CS360".  You print "CS360", then read the final 'n' and print a newline.  
<p>
Thus:
<p>
<pre>
UNIX> <b>data_uncompress < comp-1.txt</b>
Jim Plank
CS360
UNIX> <b></b>
</pre>
<p>

I have written a program called <b>data_compress</b>, which turns standard input into the proper 
format to be read by <b>data_uncompress</b>.  It turns most numbers into integers and doubles, and
the rest of its words into strings.
For example, if we do:

<pre>
UNIX> <b>echo "1234 3.14159 Fred" | data_compress > comp-2.txt</b>
</pre>

Then <b>comp-2.txt</b> is composed of the following bytes:

<p><center><table border=3 cellpadding=3>
<tr>
<td>105('i')</td>
<td>0</td>
<td>210</td>
<td>4</td>
<td>0</td>
<td>0</td>
<td>100('d')</td>
<td>0</td>
</tr><tr>
<td>110</td>
<td>134</td>
<td>27</td>
<td>240</td>
<td>249</td>
<td>33</td>
<td>9</td>
<td>64</td>
</tr><tr>
<td>115('s')</td>
<td>0</td>
<td>3</td>
<td>70('F')</td>
<td>114('r')</td>
<td>101('e')</td>
<td>100('d')</td>
<td>110('n')</td>
</tr>
</table></center><p>

To uncompress, you first read the 'i' and the 0, which says to read one integer.  You will read the next
four bytes (210, 4, 0, 0) using <b>fread()</b>.  
You next read the byte 'd' and a 0, which say to read one double.
You  will thus read the next eight bytes (110, 134,27, 40, 249, 33, 9, 64) again using <b>fread()</b>.  
Next you read an 's' and 0, which says to read one string.  You read 3, which says that the string is
four characters (Fred).  After reading those, you read the 'n' which says to print a newline.  
<p>
The output is thus:
<pre>
UNIX> <b>data_uncompress < comp-2.txt</b>
1234 3.14159 Fred
UNIX> <b></b>
</pre>

If you call <b>data_compress</b> on a file and pipe the output to <b>data_uncompress</b> or
<b>data_uncompress</b>, you will get a file that is roughly equivalent to the original.
There may be some formatting that is different.  Your output must match mine exactly, however.
<p>
<h3>A Common Mistake</h3>

When you read <i>n</i> and <i>size</i>, you read them as unsigned chars.  However, when you
use them in your computer program, you should convert them to ints.  Why?  Because if you read
in 255 as an unsigned char, and then add one to it, it becomes zero, and that is not what
you want, is it?

<h3>
Error catching</h3>

Your program must catch the following errors, and then print the given strings on standard error.
If you are in doubt about what your output should be, test it against the program in the lab
directory.  
<p>
When you are reading multiple characters, integers or doubles, read them all at
once.  For example, if you are supposed to read 10 integers, do it with one <b>fread()</b>
call, and then check to make sure that the call actually read 10 integers.  Don't do 
10 <b>fread()</b> calls.  That will help you match the proper output.  
<p>
Just a little more information on this -- in my program, I had three different buffers: One
for chars, one for integers and one for doubles.  Each buffer can hold the maximum number of
chars, integers and doubles (257 chars, since we will null terminate strings, and 256 integers
and doubles).  These were the buffers that I used for the <b>fread()</b> calls.

<UL>
<LI> If the <i>type</i> character is not 'n', 's', 'i' or 'd', then print <b>"Input error: bad type\n"</b>
<LI> If you don't get the size byte <b>n</b>, print <b>"Input error: no size\n"</b>
<LI> If you don't get the string size byte <b>size</b>, print <b>"Input error: no string size\n"</b>
<LI> If you don't get enough ints, print <b>"Input error: not enough ints\n"</b>
<LI> If you don't get enough doubles, print <b>"Input error: not enough doubles\n"</b>
<LI> If you don't get enough characters in a string, print <b>"Input error: not enough chars\n"</b>
</UL>

<h3>Walking you through gradescript 100</h3>

<a href=case-100.pdf>This is a piazza post from 2017</a>. 
Please take a look if gradescripts 90-100 are giving you trouble, or if you want some ideas
on debugging.
