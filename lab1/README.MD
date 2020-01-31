This is a lab that makes sure that you have red-black trees, dlists and 
fields down.  
<p>
Your job in this lab is to write the program <b>famtree</b>.  <b>Famtree</b>
takes a description of people and their relationships to one another
on standard input.  These descriptions must be in a special format, 
which will be described below.  An example is in the file
<a href=fam1><b>fam1</b></a>, which represents a family composed of
Bob and Nancy, and their three chidren Frank, Lester and 
Diana.
<p>
<b>Famtree</b> takes such a description, and prints out complete
information on all of the people in the file.  This consist of, for
each person, their sex (if known), their father, mother and children.
Therefore 
<a href=fam1output><b>fam1output</b></a> contains valid output of
<b>famtree</b> on <a href=fam1><b>fam1</b></a>.
<p>
The format of the input file is as follows.  Lines must either 
be blank, or have one of following first words:
<UL>
<LI> <b>PERSON</b>: This specifies a person.  Following <b>PERSON</b>
     is the person's name, which may be any number of words.
     <b>Famtree</b> should assume that names consist of words with
     single spaces between them.
<LI> <b>SEX</b>: This specifies the person's sex.  It can be
     either <b>M</b> or <b>F</b>.
<LI> <b>FATHER</b>: This specifies the person's father.  
     Following <b>FATHER</b>
     is the father's name, which may be any number of words.
     It implies that the father is male.
     A person may only have one father.
<LI> <b>MOTHER</b>: This specifies the person's mother.  
     Following <b>MOTHER</b>
     is the mother's name, which may be any number of words.
     It implies that the mother is female.
     A person may only have one mother.
<LI> <b>FATHER_OF</b>: This specifies that the person is male,
     and that the specified person is one of the person's children.
     A person may have any number of children.
<LI> <b>MOTHER_OF</b>: This specifies that the person is female,
     and that the specified person is one of the person's children.
</UL>
People may be specified in any order in the input file.
<p>

<b>Famtree</b> has two other features.  First, it prints out the 
people in a structured order.  That is, no person can be printed out
until both of their parents have been printed out.  If this is impossible
(as, for example, in 
<a href=cyclefam><b>cyclefam</b></a>), then <b>famtree</b> identifies 
this fact before it prints out people.
Beyond this one rule, people may be printed in any order.  
<p>
The second feature of <b>famtree</b> is that it allows for redundancy, 
but it infers as much as it can.  For example, 
<a href=redundant><b>redundant</b></a> has a few lines that are 
redundant.  Line 3 is redundant because Fred must be
male by virtue of being Joe's father.  Moreover, line 7 is redundant
because line 2 already specified Fred as Joe's father.  The file
<a href=nonredundant><b>nonredundant</b></a> is the minimal file specifying
the same family.
<h3>Specifically -- the output should look as follows:</h3>
Each person should have the following information printed, each on a different line.
<UL>
<LI> Their name.
<LI> The word <b>Sex:</b> and the person's sex: <b>Male</b>, <b>Female</b> or <b>Unknown</b>
<LI> The word <b>Father:</b> and name of the person's father.  If that is unknown, print <b>Unknown</b> for the father's name.
<LI> The word <b>Mother:</b> and name of the person's mother.  If that is unknown, print <b>Unknown</b> for the mother's name.
<LI> The word <b>Children:</b> and if the person has no children, then <b>None</b>.
<LI> If the person does have children then print the name of each child on its own line.
<LI> End the person with a blank line.
<LI> My output does some indentation.  I don't care if yours does.
</ul>
You should check for the following errors and print error messages on standard error that match mine:
<UL>
<LI> Specifying two fathers or two mothers.
<LI> Specifying a father when the person is female.
<LI> Specifying a mother when the person is male.
<LI> Specifying that a person is female when the person is already known to be male.
<LI> Specifying that a person is male when the person is already known to be female.
<LI> Having a cycle in the specification.
</UL>

<p>
The cycle detection should be performed after the others.  The others should be 
flagged as they are read in.

<hr>
<h2>Working example</h2>
A working example of <b>famtree</b> is in 
<b>~jplank/cs360/labs/lab1/famtree</b>.  Try it out on the input files in 
this directory.  Other input files are:
<UL>
<LI><a href=fam2><b>fam2</b></a> a more complex family.
<LI><a href=fam3><b>fam3</b></a> A family with 10 generations of single
    parents.
<LI><a href=fam4><b>fam4</b></a> A file with an error in it.
<LI><a href=fam5><b>fam5</b></a> A file with another error in it.
</UL>
You should make your output work exactly like <b>famtree</b>'s.

<hr>
<h2>Grading</h2>

There is a grading script in <b>/home/jplank/cs360/labs/lab1/gradescript</b>.  You should have your executable
in the current directory and call <b>gradescript</b> with a number from 1 to 100.  The script will run a 
test case and test its answer to see if it is correct.  If it is incorrect, it will print information about
why.
<p>
There is a separate program called <b>gradeall</b>, which tests all 100 test cases, and gives you a grade.
This will be a significant component of your grade.  The TA's will also test for commenting, etc.
<p>
Some test cases test your error checking.  The text of your errors needs to match mine.

<hr>
<h2>Help</h2>
I'm going to give rather detailed help here.  In fact, I basically
"give away" how to do the lab.  This is so that you understand the
correct structure of the program.  If you think you can do this
without any help, and don't want the answer given away, please
don't read this, and have fun!  I'd recommend that you at least
think about how you'd do it with no help before you read further.
However, if you want some help,
read on.  
<p>
You will have a <b>struct</b> for a person (mine is called a <b>Person</b>).
That struct will have the following fields: 
<UL>
<LI> Name
<LI> Sex
<LI> Father
<LI> Mother
<LI> List of children
<LI> Some stuff for depth and breadth first traversal.
</UL>
<p>
Your program will work in three phases:
<OL>
<LI> <b>Reading the information into the structs.</b>  You should have 
     a red-black tree (mine is called <b>people</b>) that contains all
     the people.  It is keyed on each person's name, and the <b>val</b>
     fields are the persons' <b>Person</b> structs.  You use an
     <b>IS</b> to read standard input.  

     <p>
     Each time you read a line that has a name (i.e. <b>PERSON</b>, 
     <b>FATHER</b>, <b>MOTHER</b>, <b>FATHER_OF</b> and <b>MOTHER_OF</b>)
     you test to see if the person with that name 
     is in the <b>people</b> tree.
     If not, you create the struct and insert it.  

     <p>
     Whenever you process a line that needs to create some links 
     (i.e. <b>FATHER</b>, <b>MOTHER</b>, <b>FATHER_OF</b> and 
     <b>MOTHER_OF</b>), you first check to see if the link exists and
     is correct.  If incorrect, you flag an error.  If correct, you do
     nothing.  If the link doesn't exist, you should create it in both
     the parent and the child.
   
     <p>
     When you're done with this phase, you'll have a red-black tree with
     all people in it, and all the people will have the correct links to
     their parents and children.
     
<LI> <b>Testing the graph for cycles.</b>
     The graph is messed up if there is a cycle in it.  In other words,
     if a person can be his/her own ancestor or descendant, then there
     is a problem.  Testing for cycles is a simple depth-first search, 
     which you should have learned in CS302.  To test if a person is
     his/her own descendant, the following pseudocode will work:
<pre>
     /* assume that there is an integer field called "visited" 
        in the Person struct, and that this field is initialized 
        to zero for all people */

     is_descendant(Person *p)
     {
       if (p->visited == 1) return 0;  /* I.e. we've processed this 
                                               person before and he/she's ok */
       if (p->visited == 2) return 1;  /* I.e. the graph is messed up */
       p->visited = 2;
       for all children of p do {
         if is_descendant(child) return 1;
       }
       p->visited = 1;
       return 0;
     }
</pre>

<LI> <b>Printing out the graph.</b>
     This is
     a kind of breadth-first search.  What you do is create a queue (which
     will be a <b>Dllist</b>
     called <b>toprint</b>).  You can initially put all people into
     <b>toprint</b>, or you can just put all people who have no parents.
     <p>
     Then you execute a loop that does the following:

<pre>
/* assume that there is an integer field called "printed" 
   in the Person struct, and that this field is initialized 
   to zero for all people */
 
while toprint is not empty
  take p off the head of toprint
  if p has not been printed, then 
    if p doesn't have parents, or if p's parents have been printed then
      print p
      for all of p's children, put the child at the end of toprint
    end if
  end if
end while
</pre>
</OL>

Enjoy!

<hr>
There is a makefile for this lab  
<a href=makefile>here</a>.