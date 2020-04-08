Time to write a shell.  Your job is to write <b>jsh</b> -- your own
shell, which is going to be very primitive.  <b>Jsh</b> is like <b>csh</b>:
It is a command line interpreter that lets you execute commands and 
redirects their input/output.  You will use the same syntax as the bourne/c 
shell, and implement the operations &lt; , &gt; &gt;&gt;, &amp; and |.
You are not expected to deal with things like quotes, environment
variable expansions, pattern matching, job control or command histories.
<p>
I recommend doing this in three parts:

<HR><H2>Part 1</H2>

Part 1 is to implement a really bare-bones shell with no frills.
This shell should
take one optional command line argument, which is its prompt.
If not specified, the prompt should be <b>"jsh: "</b>.  
If the prompt is "-", then do not print any prompt.
<p>
<b>Jsh</b> reads lines from standard input, and if they are
non-blank, it attempts to execute them.  Like the shell, <b>jsh</b>
should search the <b>PATH</b> variable to find executable files specified
as relative path names (in other words, <b>jsh</b> just needs to call
<b>execvp</b> instead of <b>execve</b>).
If a command line ends with an
ampersand (&amp), then <b>jsh</b> should not wait for the command
to finish before returning the prompt.  
Otherwise, it should wait for the command to finish.

<p><b>Jsh</b> should exit when the user types <b>CNTL-D</b> or <b>exit</b>.

<p>Don't worry about <b>CNTL-C</b> or <b>CNTL-Z</b>.   However, when you 
quit, look around and kill stray processes that might have been left around
by <b>jsh</b> while you are debugging it.

<p>Also, don't worry about file indirection, which is covered in the next 
part.  Just get your <b>fork()</b>'s, <b>wait()</b>'s 
and <b>execvp()</b>'s correct.


<p>You may want to look at 
<a href=forkcat1.c><b>forkcat1.c</a></b> and
<a href=forkcat2.c><b>forkcat2.c</a></b> for some examples of using
<b>fork()</b>, <b>wait()</b> and <b>execvp()</b>.

<p><b>Remember Dr. Plank's Cardinal Sin of Exec!</b>

<p>Also, read below about zombie processes and make sure you have 
that taken care of before continuing onto part 2.

<HR><H2>Part 2</H2>

Here you add input and output
redirection to files -- i.e. &lt;, &gt;, and &gt;&gt;.  
This should also be straightforward from the Dup lecture.
You may assume that the tokens &lt;, &gt;, and &gt;&gt; (and &amp; and | for
that matter) are separated from other arguments by white space (i.e.
you may use the fields library).

<HR><H2>Part 3</H2>

Now, implement pipes.  In other words, any combination 
of |, &lt;, &gt;, and &gt;&gt; and &amp; should work.  Look at 
<a href=headsort.c><b>headsort.c</b></a> for some pointers on 
connecting two processes together with a pipe.

<p>Be careful that you wait for all processes in a pipe to complete before
going on (that is, unless you have specified with &amp; that you 
don't want to wait).

<p>Just to reiterate, you can have <i>any</i> number of processes piped
together.  For example, the following should work (it will reverse
the file <b>f1</b> and put the result in <b>f2</b>):
<pre>
jsh: <font color=darkred><b>cat -n f1 | sort -nr | sed s/.......// | cat > f2</b></font>
</pre>


<HR><H2>General Stuff</H2>

<h3>Zombies</h3>

You should try to minimize the number of zombie processes that will
exist (this is in all parts).  This is not to say that they can't
exist for a little while, but not forever.
When you call <b>wait()</b> for a shell
command, it might return the pid of a zombie process, and not the 
process you thought would return.  This is fine --- you just have
to be able to deal with it.  (i.e. consider the following sequence):

<pre>
jsh: <font color=darkred><b><b>cat f1 > /dev/null &</b></b></font>
jsh: <font color=darkred><b><b>vi lab3.c</b></b></font>
</pre>

You are going to call <b>wait()</b> to wait for the <b>vi</b> command to
terminate, but 
it will return with the status of the zombie process from the <b>cat</b>.
This is all fine -- you just need to be aware that these things may
happen, and that you may have to call <b>wait()</b> again to wait for
<b>vi</b> to complete.  You may use <b>wait3()</b> as well if you want
to use non-blocking waits to clear up zombie processes.  You are not allowed
to use <b>waitpid()</b> though.

<h3>Open files</h3>

You must make sure that when you call <b>execvp</b>, that there are only
three files open -- 0, 1, and 2.  If there are others open, then you have
a bug in your shell.  
<p>
Also, when a command is done, and the shell prints out its prompt, then
it should only have three files open -- 0, 1, and 2.  Otherwise, you have
forgotten to close a file descriptor or two and have a bug in your
code.  Check for this.   My <b>jsh</b> never uses a file descriptor
higher than 5.

<h3>Waiting in <b>jsh</b></h3>

In <b>jsh</b>, if you do not specify the ampersand, then your shell
should not continue until <i>all</i> the processes in the pipe have 
completed.  You'll need a red-black tree for this.

<h3>Errors</h3>

Your code should work in the face of errors. For example, if you
specify a bad output file at the end of a multi-stage pipe, then
the error should be noted, and your shell should continue working.
Make sure you check for all the error conditions that you can 
think of.

<HR><H2>The Grading Script -- Helper Programs</H2>

The first 20 gradescripts test part 1.  The next 40 gradescripts test part 2, and the
remaining ones test part 3.
<p>
The following programs are in the lab directory:

<UL>
<LI> <b>cattostde.c</b>: This works like <b>cat</b>, but it puts standard input to standard error.
<LI> <b>strays.c</b>: This checks for open file descriptors and will flag an error if any file descriptor
higher than three is open.  Then it works just like <b>cat</b>
<LI> <b>strays-files.c</b>: This works like <b>strays</b> except it copies the first argument to the second.
<LI> <b>strays-fsleep.c</b>: This works like <b>strays-files</b> except it sleeps for a 5th of a second before starting.
<LI> <b>strays-sleep.c</b>: This works like <b>strays</b> except it sleeps for a 5th of a second before starting.
</UL>

The grading script uses all of these to test various features of your
shells.  Beside the first few, each gradescript call will take a few
seconds.  The gradescripts are time sensitive, too, so the output of
your program may change as time passes -- for that reason, the
gradescripts can be a little hard to parse.   I can go over them in 
class if you ask me.  I've written up an example of going through one of the
difficult ones (gradescript 61) in 
<a href=gs-61.html>http://web.eecs.utk.edu/~jplank/jplank/classes/cs360/360/labs/lab8/gs-61.html</a>.
