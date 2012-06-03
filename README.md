Dijkstra
========
About
-----
Dijkstra's algorithm ANSI C implementation.
Sample
------
**input.txt**
 
* First line: number of nodes.
* Second line: node to search paths from.
* Third+: matrix.
 
&nbsp;

	7
	4
	0	10	3	0	7	0	0
	10	0	0	13	0	0	0
	3	0	0	0	0	0	30
	0	13	0	0	7	0	0
	7	0	0	7	0	1	10
	0	0	0	0	1	0	2
	0	0	30	0	10	2	0

**output.txt**

	L:
	1) ∞	∞	∞	0	∞	∞	∞	
	2) ∞	13	∞	0	7	∞	∞	
	3) 14	13	∞	0	7	8	17	
	4) 14	13	∞	0	7	8	10	
	5) 14	13	40	0	7	8	10	
	6) 14	13	40	0	7	8	10	
	7) 14	13	17	0	7	8	10	
	8) 14	13	17	0	7	8	10	
	
	Paths:
	1) 4 -> 5 -> 1 [14]
	2) 4 -> 2 [13]
	3) 4 -> 5 -> 1 -> 3 [17]
	4) 4 [0]
	5) 4 -> 5 [7]
	6) 4 -> 5 -> 6 [8]
	7) 4 -> 5 -> 6 -> 7 [10]
