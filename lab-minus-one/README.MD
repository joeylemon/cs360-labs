This is more of a homework than a lab.  Your job is as follows.
On a lab machine, you are to generate two HTML files.  
Let us suppose that your username is "binky."  You should do the following:

<pre>
UNIX> <font color=darkred><b>/home/jplank/cs360/labs/labm1/lab-minus-one binky-1 T > binky-1.html</b></font>
UNIX> <font color=darkred><b>/home/jplank/cs360/labs/labm1/lab-minus-one binky-2 T > binky-2.html</b></font>
</pre>


As you see, this creates two HTML files, named 
<b><a href=binky-1.html>binky-1.html</a></b> and
<b><a href=binky-2.html>binky-2.html</a></b>.
You can click on these links, and you'll see the files.  
Each of these has a <i>key</i>.  The first one's key is "binky-1" and "binky-2".
The first line of each HTML file tells you what its key is.  For example, the first line
of <b><a href=binky-1.html>binky-1.html</a></b> is "Lab (-1). HTML file for key binky-1:".

Each of these HTML files contains 25
questions.  They will be similar in format, but different in content.  
Each key will generate a different HTML file.
Your HTML files will be different, of course, because you have a different username than "binky,"
and therefore your keys will be different.

<p>
Your job is to create two text files -- one for each HTML file.  
Again, let's go back to supposing that your username is
"binky".  You will create two files, <b>binky-1.txt</b> and <b>binky-2.txt</b>.
These files will contain answers to the respective HTML files.
The first line of an answer file should be the key.  
After that, 
each answer should go on its own line,
and each line should be of the form:

<p><center><table border=3 cellpadding=3><td><pre>
number answer
</pre></td></table></center><p>

The <i>number</i> is the question number, and the <i>answer</i> is your answer.  
Take a look at the file <b><a href=binky-1.txt>binky-1.txt</a></b>:

<p><center><table border=3 cellpadding=3><td><pre>
binky-1
0 0x12345678
1 12345678
12 0xb6
</pre></td></table></center><p>

As you can see, I have put the key "binky-1" as the first line.  After that, 
I have answered questions 0, 1 and 12.  I can tell you that the answer to questions
0 and 1 are incorrect.  Question 1's answer is in the wrong format, too (it doesn't match the
<b>printf()</b> statement in the question).  The answer to question 12 is correct.
<p>
You can test your answers for correctness, by calling <b>/home/jplank/cs360/labs/labm1/lab-minus-one</b> with the same key that created the HTML file, but with an 'A' instead of a 'T'.  Redirect
standard input to come from your answer file.  The program
will report your answers and whether or not they are correct.  
<p>
Here's an example with <b>binky-1</b>:

<pre>
UNIX> <font color=darkred><b>/home/jplank/cs360/labs/labm1/lab-minus-one binky-1 A < binky-1.txt</b></font>
Question  0: Your answer: 0x12345678 -- Incorrect: +0
Question  1: Your answer:   12345678 -- Wrong number of characters.  Check your formatting.
Question  2: Your answer:            -- Incorrect: +0
Question  3: Your answer:            -- Incorrect: +0
Question  4: Your answer:            -- Incorrect: +0
Question  5: Your answer:            -- Incorrect: +0
Question  6: Your answer:            -- Incorrect: +0
Question  7: Your answer:            -- Incorrect: +0
Question  8: Your answer:            -- Incorrect: +0
Question  9: Your answer:            -- Incorrect: +0
Question 10: Your answer:            -- Incorrect: +0
Question 11: Your answer:            -- Incorrect: +0
Question 12: Your answer:       0xb6 -- Correct: +4
Question 13: Your answer:            -- Incorrect: +0
Question 14: Your answer:            -- Incorrect: +0
Question 15: Your answer:            -- Incorrect: +0
Question 16: Your answer:            -- Incorrect: +0
Question 17: Your answer:            -- Incorrect: +0
Question 18: Your answer:            -- Incorrect: +0
Question 19: Your answer:            -- Incorrect: +0
Question 20: Your answer:            -- Incorrect: +0
Question 21: Your answer:            -- Incorrect: +0
Question 22: Your answer:            -- Incorrect: +0
Question 23: Your answer:            -- Incorrect: +0
Question 24: Your answer:            -- Incorrect: +0

Your score: 4 / 100
UNIX> <font color=darkred><b></b></font>
</pre>

You should answer all of the questions in both HTML files.  You should have two answer files --
one for each key.  Then, in your first lab session, your
TA's will tell you how to submit your answers.
<p>
<i>It is very important that you follow these directions carefully.  You are <b>not</b> to 
create <b>binky-1.html</b> and <b>binky-2.html</b>.  Those are simply exmaples for this 
writeup.   You will create HTML files that are specific to your username.  That way, each
student gets the pleasure of getting his or her own unique HTML files.  Isn't that awesome?</i>

<hr>
Remember -- when you use '%x', all of the digits are lower case.
<hr>
A word of advice -- you will find the <b>endian.c</b> program in the first set of 
lecture notes to be very helpful in doing this lab.  You should read over that program
and its accompanying writeup before doing this lab.

<hr>
<h3>Answering questions From Piazza</h3>

<UL>
<LI> <b>Question</b>: When I run:
<pre>
/home/jplank/cs360/labs/labm1/lab-minus-one binky11-1 A < binky11-1.txt
</pre>
I get this error:
<pre>
on standard input.You put binky11-1 on the command line, and binky11-1
</pre>
My text file is this:
<pre>
binky11
12 [answer]
</pre>
<b>Answer:</b>
When you run <b>lab-minus-one</b>, you need to give it a key.  
Above, you gave it the key "binky11-1".
You need to use the same key in three places:

<p>
<OL>
<LI> On the command line when you run it with the argument T to generate the assignment.
<LI> On the command line when you run it with the argument A to check answers.
<LI> As the first line of standard input when you run it with the argument A to check answers.
</OL>
<p>

It has to be the same key in all three cases.  Here, #2 and #3 don't match, because one of them
(#2) is "binky11-1" and one of them (#3) is "binky11".
