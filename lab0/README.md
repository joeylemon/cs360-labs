<h3>Introduction</h3>

For this lab, you are the mighty healing shaman Urgosa in the popular online
role-playing game, Planet of Peacecraft (PoP). You and your companions work
together to pacify ferocious monsters so that eventually you all co-exist on one
happy and harmonious planet.  Currently, however, the planet is far from peaceful.
It is a battlefield of terror.
<p>
Your job as Urgosa is to keep your companions alive with your healing
spell called <i>Chain Heal</i>.
Chain Heal allows Urgosa to target a player on the battlefield and heal
him/her. Afterward, the spell then jumps from that player to another player that
is within a certain range and heals them as well. It continues to jump to subsequent
players in this fashion. What does it mean to heal them?  Well,
every player has a both a current amount of pacification points (PP) and a maximum amount
of PP. Chain Heal can restore PP up to the maximum amount for that player.
<p>
For example, suppose Chain Heal can restore 300PP and it hits a player with
100PP and a max of 500PP. That player will now have 400PP. Pretty simple, but if
that player were to be healed again they would end up with 500PP, not 700PP.
<p>
Chain Heal has the following limitations:
<ul> <li> Urgosa can only cast the spell on targets within some
  <b>initial_range</b> (Urgosa may target himself as well).
</li><li> The range at which Chain Heal can jump from that initial target to any
  following targets is limited by some <b>jump_range</b>.
</li><li> Chain Heal can only heal a number of players up to  some
  <b>num_jumps</b>.
</li><li> The potential amount of healing done to the initial target is equal to
  some <b>initial_power</b>.
</li><li> The potential amount of healing to subsequent targets is reduced by
  a factor of <b>power_reduction</b> every time it jumps. 
</li><li> Chain Heal can only heal a player once.
</li></ul> 
Let's look at another example. Suppose Urgosa casts a Chain Heal with an
<b>initial_power</b> of 500 and a <b>power_reduction</b> of 0.25 and it will go
through the following people: (in this order)
<ul> <li> Adam_the_Warrior (100/500PP)
</li><li> Catherine_the_Great (400/450PP)
</li><li> Chad_the_Priest (45/400PP)
</li><li> James_the_Lightning_Lord (300/600PP)
</li></ul> 

Adam will be healed for 400 even though the spell has a power of 500 because he
cannot be over his max PP. Now when the spell moves on to Catherine, the
power will be reduced by 25% making the power 500 * (1 - 0.25) = 375. Again PP
cannot exceed maximum so Catherine will be healed for 50. Now moving on to Chad,
the power is now 375 * (1 - 0.25) = 281.25 so Chad will be healed for 281 (pacification
points are integers so we round the power appropriately - use <b>rint()</b> from math.h,
and remember to use "-lm" when you compile). Finally,
the power becomes 281.25 * (1 - 0.25) = 210.9375 so James will gain 211PP.

<hr>
<h3>Your Job</h3>

You are to write <b>chain_heal.c</b> which finds the optimal path for
Chain Heal (the most pacification points restored). It will be called as follows:

<p>
<tt>chain_heal <b>initial_range jump_range num_jumps initial_power 
  power_reduction</b> &lt <b>input_file</b></tt>
<p>
Each command line argument is an integer with the exception of <b>power_reduction</b>,
which is a double.

<b>Chain_heal</b> reads information about the players on the battlefield from stdin. 
Each line contains information about a single player and is composed of exactly 
5 words:

<p>
<OL>
<LI> <b>X</b> (Integer): The x-coordinate of the player on a a 2D grid.  
<LI> <b>Y</b> (Integer): The y-coordinate of the player.  
<LI> <b>Current_PP</b> (Integer): The player's current pacification points.
<LI> <b>Max_PP</b> (Integer): The player's maximum pacification points.
<LI> <b>Name</b> (String, max of 100 characters): The player's name.
</OL>

One of the players will always be Urgosa_the_Healing_Shaman because he is the
one casting the Chain Heal.
<p>
Here is the file for the earlier example,
<b><a href="./small.txt">small.txt</a></b>:

<p>
</p><p></p><center><table border="3" cellpadding="3"><tbody><tr><td><pre>
0 0 100 100 Urgosa_the_Healing_Shaman
2 0 100 500 Adam_the_Warrior
3 0 400 450 Catherine_the_Great
4 0 45 400 Chad_the_Priest
4 1 300 600 James_the_Lightning_Lord
</pre></td></tr></tbody></table></center><p>
</p>

You do not have to error-check the input. You may assume that it is in the
correct format and that there are no two players with the exact same name.
<p>

Once you have determined the optimal path, you print the path, one player per
line. Each line will have the name of the player healed followed by a space and
the amount of healing done to them. In addition, you print a final line
with the word "Total_Healing" followed by a space and the total amount of healing done by
the Chain Heal.
<p>

Here's is an example of my output on
<b><a href="./small.txt">small.txt</a></b>:
<p>

<pre>
UNIX> <font color=darkred><b>./chain_heal 2 1 4 500 0.25 &lt small.txt</b></font>
Adam_the_Warrior 400
Catherine_the_Great 50
Chad_the_Priest 281
James_the_Lightning_Lord 211
Total_Healing 942
</pre>
<p>

<hr>
<h3>Constraints and Requirements</h3>

<b>X</b> and <b>Y</b> will be between -10000 and 10000.  <b>Max_PP</b> will be between 
1 and 10,000, and <b>Current_PP</b> will be between 0 and <b>Max_PP</b>.
<p>
<b><i>You have to write this program in C</i></b>, with no additional libraries or helper
programs.  That means no standard template library, no fields library and no libfdr library.
I'll give some help below.
<p>
To make this explicit, you may have the following includes in your program:
<p><center><table border=3 cellpadding=3><td><pre>
#include &lt;stdio.h&gt;
#include &lt;string.h&gt;
#include &lt;stdlib.h&gt;
#include &lt;unistd.h&gt;
#include &lt;math.h&gt;           /* If you include this, you need to compile with -lm */
</pre></td></table></center><p>
<p>
When you read standard input, go ahead and use <b>scanf()</b> 
to read words rather than
lines.  You can do this 
because we are guaranteeing that input is in the proper format.  
<p>
You are not allowed to make any assumptions on the size of the input file (i.e. on the number
of nodes in the graph).
<p>

<hr>
<h3>Algorithmic Strategy</h3>

Clearly, this is going to be a graph problem.  You read in all of the players,
and create a node in the graph for each player.  The graph is undirected, and there
is an edge between two players if the distance between them is less than or equal
to <b>jump_range</b>.
<p>
Once you create the graph, you determine each of the nodes that is within <b>initial_range</b>
of Urgosa.  For each of these nodes, you perform a depth-first search to find the optimal
healing path starting from the node.  The depth-first search enumerates all paths starting
from the node.  You maintain the best path, and at the end of the program, you print it.
<p>
For example, suppose I call:

<pre>
UNIX> <font color=darkred><b>./chain_heal 2 1 4 500 0.25 < small.txt</b></font>
</pre>

Here's a picture of the graph, with the potential starting nodes colored green:
<p><center><table border=3><td><img src=http://web.eecs.utk.edu/~mbeck/classes/Spring20-cs360/360/labs/lab0/G1.jpg width=400></td></table></center><p>

There are only two potential paths for the Chain Heal:
<OL>
<LI> [Urgosa,0]
<LI> [Adam,400][Catherine,50][Chad,281][James,211]
</OL>
Obviously, the second one has more healing.
<p>
Instead, suppose I call
<pre>
UNIX> <font color=darkred><b>./chain_heal 1 2 4 500 0.25 < small.txt</b></font>
</pre>

Now, the graph looks as follows:
<p><center><table border=3><td><img src=http://web.eecs.utk.edu/~mbeck/classes/Spring20-cs360/360/labs/lab0/G2.jpg width=400></td></table></center><p>

There are now four possible 4-node paths:


<OL>
<LI> [Urgosa,0][Adam,375][Catherine,50][Chad,211]
<LI> [Urgosa,0][Adam,375][Catherine,50][James,211]
<LI> [Urgosa,0][Adam,375][Chad,281][Catherine,50]
<LI> [Urgosa,0][Adam,375][Chad,281][James,211]
</OL>

The last of these has the most healing: 867.  Accordingly:

<pre>
UNIX> <font color=darkred><b>./chain_heal 1 2 4 500 0.25 < small.txt</b></font>
Urgosa_the_Healing_Shaman 0
Adam_the_Warrior 375
Chad_the_Priest 281
James_the_Lightning_Lord 211
Total_Healing 867
UNIX> <font color=darkred><b></b></font>
</pre>

<hr>
<h3>Implementational Strategy</h3>

As always, break this up into parts.  
<p>
<b><u>Reading the input</u></b>: The first part should be simply reading the 
command line and reading the input file.  Do this without storing anything, just so
you can be sure your use of <b>scanf()</b> is correct.
<p>
<b><u>Creating the Nodes and putting them into an array</u></b>:
As a second part, define a <b>Node</b> struct for each person.  We're not going to 
worry about hooking the nodes together yet.  We just want to read them in.
As such, each node should have the following fields:
<UL>
<LI> <b>name</b>.
<LI> <b>X</b> and <b>Y</b> coordinates.
<LI> <b>Current_PP</b> and <b>Max_PP</b> values.
<LI> A pointer to the previous node that you read.
</UL>
Try something like the following:
<p><center><table border="3" cellpadding="3"><tbody><tr><td><pre>
typedef struct node
{
  char *name;
  int x, y;
  int cur_PP, max_PP;
  struct node *prev;
} Node;
</pre></td></tr></tbody></table></center><p>

To read in the nodes, go ahead and read the five words.  Then allocate a new node with 
<b>malloc()</b>.  Initialize the first five fields, and set <b>prev</b> to be the previous
node that you read (obviously, with the first node, have its <b>prev</b> field be <b>NULL</b>).
When you're done reading, you can traverse the nodes from most recent to least recent by 
chasing the <b>prev</b> pointers.  What I did at this point was create an array
of node pointers (Node **), and assigned nodes to it by traversing my linked nodes.
I knew the size of this array, because its the number of nodes that I allocated (obviously,
I kept track). 
<p>
At this point, you no longer need the <b>prev</b> field in the nodes.  You can traverse all 
of the nodes by traversing the array.  This is some pretty old-school C programming, and you'll
note how inconvenient it is compared to vectors in the STL.  With vectors, you get that lovely
<b>push_back()</b> method, which allows you to incrementally create vectors of arbitrary size.
With C, you need to use a linked data structure to read an arbitraty number of elements, and 
when you're done, you can allocate and create an array.
<p>
You should go ahead and test this program to make sure you're not making any mistakes.
Then move onto:
<p>
<b><u>Creating the Graph</u></b>:  I added two more fields to my nodes at this point:
<p><center><table border=3 cellpadding=3><td><pre>
int adj_size;
struct node **adj;
</pre></td></table></center><p>
These define the adjacency lists for the nodes.  I created the actual lists in three
steps.  
<UL>
<LI> In the first step, I calculated the size of the adjacency list for each node.
<LI> In the second step, I allocated the adjacency list for each node.  This is an array
of node pointers, and I made it the <i>exact</i> size of the adjacency lists.
<LI> In the third step, I put the nodes onto their adjacency lists.
</UL>
Once again, this is where C is a pain compared to the STL.  With the STL, I can simply use
<b>push_back()</b> to put nodes onto adjacency lists.  With C, I need to allocate the lists
in their entirety before I put nodes onto them.  I could have simply made the lists linked 
data structures (much like using <b>prev</b> when I read them in), too.  You can do either --
both give you practice with C.
<p>
At this point, print out each node with its adjacency list and double-check yourself.
<p>
<b><u>Doing the DFS</u></b>: 
Now you should add a <b>visited</b> field to each node for your DFS.  
You should write a <b>DFS()</b> procedure, which has three arguments:
<OL>
<LI> The node.
<LI> The hop number.
<LI> A pointer to a struct that contains global information (such as <b>num_jumps</b>,
and <b>power_reduction</b>).
</OL>
Now write a DFS that traverses all of the paths from each starting node.  Have it 
print out [node,hop] for each time that it visits a node.  Here are three examples,
where you may want to make sure that your code matches mine (perhaps not the same order,
but it should have the same visits):

<pre>
UNIX> <font color=darkred><b>./a.out 2 1 4 500 0.25 < small.txt</b></font>
Node:Urgosa_the_Healing_Shaman  Hop 1
Node:Adam_the_Warrior  Hop 1
Node:Catherine_the_Great  Hop 2
Node:Chad_the_Priest  Hop 3
Node:James_the_Lightning_Lord  Hop 4
UNIX> <font color=darkred><b>./a.out 1 2 4 500 0.25 < small.txt</b></font>
Node:Urgosa_the_Healing_Shaman  Hop 1
Node:Adam_the_Warrior  Hop 2
Node:Catherine_the_Great  Hop 3
Node:Chad_the_Priest  Hop 4
Node:James_the_Lightning_Lord  Hop 4
Node:Chad_the_Priest  Hop 3
Node:Catherine_the_Great  Hop 4
Node:James_the_Lightning_Lord  Hop 4
UNIX> <font color=darkred><b>./a.out 1 10 4 500 .25 < small.txt</b></font>
Node:Urgosa_the_Healing_Shaman  Hop 1
Node:Adam_the_Warrior  Hop 2
Node:Catherine_the_Great  Hop 3
Node:Chad_the_Priest  Hop 4
Node:James_the_Lightning_Lord  Hop 4
Node:Chad_the_Priest  Hop 3
Node:Catherine_the_Great  Hop 4
Node:James_the_Lightning_Lord  Hop 4
Node:James_the_Lightning_Lord  Hop 3
Node:Catherine_the_Great  Hop 4
Node:Chad_the_Priest  Hop 4
Node:Catherine_the_Great  Hop 2
Node:Adam_the_Warrior  Hop 3
Node:Chad_the_Priest  Hop 4
Node:James_the_Lightning_Lord  Hop 4
Node:Chad_the_Priest  Hop 3
Node:Adam_the_Warrior  Hop 4
Node:James_the_Lightning_Lord  Hop 4
Node:James_the_Lightning_Lord  Hop 3
Node:Adam_the_Warrior  Hop 4
Node:Chad_the_Priest  Hop 4
Node:Chad_the_Priest  Hop 2
Node:Adam_the_Warrior  Hop 3
Node:Catherine_the_Great  Hop 4
Node:James_the_Lightning_Lord  Hop 4
Node:Catherine_the_Great  Hop 3
Node:Adam_the_Warrior  Hop 4
Node:James_the_Lightning_Lord  Hop 4
Node:James_the_Lightning_Lord  Hop 3
Node:Adam_the_Warrior  Hop 4
Node:Catherine_the_Great  Hop 4
Node:James_the_Lightning_Lord  Hop 2
Node:Adam_the_Warrior  Hop 3
Node:Catherine_the_Great  Hop 4
Node:Chad_the_Priest  Hop 4
Node:Catherine_the_Great  Hop 3
Node:Adam_the_Warrior  Hop 4
Node:Chad_the_Priest  Hop 4
Node:Chad_the_Priest  Hop 3
Node:Adam_the_Warrior  Hop 4
Node:Catherine_the_Great  Hop 4
UNIX> <font color=darkred><b></b></font>
</pre>

<p>
<u><b>Calculating the total healing</b></u>: 
Now that you are convinced that you are enumerating all the paths, add a parameter
<b>total_healing</b> to your <b>DFS()</b>, and a <b>best_healing</b> to your global
information.  Use this to calculate and store the best total healing for each path.
Print it out at the end.  Note that you're not maintaining the path at this point -- 
just the best total healing.  Test it against the program in the lab directory:

<pre>
UNIX> <font color=darkred><b>./chain_heal 2 1 4 500 0.25 < small.txt | tail -n 1</b></font>
Total_Healing 942
UNIX> <font color=darkred><b>./chain_heal 1 2 4 500 0.25 < small.txt | tail -n 1</b></font>
Total_Healing 867
UNIX> <font color=darkred><b>./chain_heal 2 2 4 500 0.25 < small.txt | tail -n 1</b></font>
Total_Healing 1086
UNIX> <font color=darkred><b>./chain_heal 10 10 4 500 0.25 < small.txt | tail -n 1</b></font>
Total_Healing 1086
UNIX> <font color=darkred><b>./chain_heal 10 10 5 500 0.25 < small.txt | tail -n 1</b></font>
Total_Healing 1086
UNIX> <font color=darkred><b>./chain_heal 10 10 5 500 0.1 < small.txt | tail -n 1</b></font>
Total_Healing 1105
UNIX> <font color=darkred><b>./chain_heal 10 10 5 500 0.5 < small.txt | tail -n 1</b></font>
Total_Healing 825
UNIX> <font color=darkred><b></b></font>
</pre>

Finally, maintain the best path.  What I did here was add three more fields to my 
global information:

<p><center><table border=3 cellpadding=3><td><pre>
int best_path_length;
Node **best_path;
int *healing;
</pre></td></table></center><p>

Both <b>best_path</b> and <b>healing</b> are allocated to be <b>num_jumps</b> in size.
<p>
I also added a <b>healing</b> integer to each node.  Finally, I added a <b>from</b> node to my 
DFS call.  Now, during the DFS, each node maintains its current <b>healing</b> value, 
plus a pointer to the previous node in the path by reusing the <b>prev</b> field from above.
<p>
When a new "best" path is found, the nodes are stored in <b>best_path</b> (using the <b>prev</b>
fields), and their healing values are stored in the <b>healing</b> array.  The size of the
path is stored in <b>best_path_length</b>.  At the end of the program, these are used to print
out the final best path.

<hr>
<h3>Couldn't This Be Faster, Dr. Plank?</h3>

Why yes, it could.  You could make this a dynamic program by memoizing the DFS call.
<b>Don't do this, though, because you'd need something like the jrb library to implement
the cache, and I'm not letting you use external libraries</b>.  I'm just saying that you
could make it faster with dynamic programming!


<hr>
<h3>Random Input Generation, Checking, Grading</h3>

The program <b><a href=random_hero_gen.cpp>random_hero_gen.cpp</a></b> generates
random input files.  All of the gradescript examples were created with this program
and the constraints above.  The program seeds the random number generator on the current
time, so it will generate different output in each second:

<pre>
UNIX> <font color=darkred><b>./random_hero_gen 10 10000 10000 10000</b></font>
-8599   945  5234  8678 Urgosa_the_Healing_Shaman
-7118 -9489   429  3830 Spinny-McCrazyPants_the_Bladesinger
-1015  7421  1712  2300 Danielle_the_Gladiator
-2466  6917  1382  6477 Moon-Moon_the_Monk
 7429 -2081   691  7731 Hector_the_Delver
-2003  7930  3876  4071 Sabastian_the_Artificer
   60 -9294   703   760 Elizabeth_the_Shin-Kicker
  276  6332  1168  2066 Aldaricht_the_Hero,-Born-Under-Justice
-5935 -3947  3553  5955 Allen_the_Dragon-Knight
 7855 -2953   332  1746 Varrus_the_Sorcerer
UNIX> <font color=darkred><b>./random_hero_gen 10 10000 10000 10000</b></font>
-7075  7146   493  2890 Urgosa_the_Healing_Shaman
 3977 -2357  4144  6778 Chad_the_Rapper
 8477 -8470  5818  9776 Rob_the_Witchhunter
 9263  -161  6470  7377 Rock-Party_the_Cavalier
 6996  9754  2083  6215 Fist-RockBone_the_Smelly
-7449 -5631  2636  3179 Chad_the_Tiny
-4738  -273    41  4156 Chunkhead_the_Swashbuckler
 3570 -7102  1256  2998 Sabastian_the_Shaman
 1309  6582  2385  2439 Luigi_the_Adept
-6540  2149  6471  7318 Dylan_the_Grumpy-Pants
UNIX> <font color=darkred><b></b></font>
</pre>

The program <b>chain_heal_check</b> can be used to check your output.  It takes the same
command line arguments as <b>chain_heal</b>, plus the name of the file used as standard input
for <b>chain_heal</b>. On standard input, it takes the standard output of the <b>chain_heal</b>
call.  If standard input specifies a correct path (with the correct healings), then <b>chain_heal_check</b> simply exits.  Otherwise, it specifies the error.
<p>
Note that <b>chain_heal_check</b> does <i>not</i> verify that the path is optimal.  Just that it is
legal.

<pre>
UNIX> <font color=darkred><b>./chain_heal 2 1 4 500 0.25 < small.txt | chain_heal_check 2 1 4 500 0.25 small.txt</b></font>
UNIX> <font color=darkred><b>./chain_heal 2 1 4 500 0.25 < small.txt | sed 's/James/Shmames/'</b></font>
Adam_the_Warrior 400
Catherine_the_Great 50
Chad_the_Priest 281
Shmames_the_Lightning_Lord 211
Total_Healing 942
UNIX> <font color=darkred><b>./chain_heal 2 1 4 500 0.25 < small.txt | sed 's/James/Shmames/' | chain_heal_check 2 1 4 500 0.25 small.txt</b></font>
ERROR: Shmames_the_Lightning_Lord is not in the original input file
UNIX> <font color=darkred><b>./chain_heal 2 1 4 500 0.25 < small.txt | sed 's/211/212/' | chain_heal_check 2 1 4 500 0.25 small.txt</b></font>
ERROR: Incorrect healing amount on line 4
Read 212 but should be 211
UNIX> <font color=darkred><b></b></font>
</pre>

The grading script works as in CS140 / CS302.  If you are not familiar with these kinds of
grading scripts, please talk with your TA's.

<p>
The way that the grading script works is that it calls the program in the lab directory to 
get the total healing value.  Then it calls your program and checks to see if your total
healing value is equal to the correct one.  Then it calls <b>chain_heal_check</b> to make sure
that your path is a correct one.  As you'll note, your path does not have to equal mine -- 
it simply has to be legal and give the optimal total healing.
