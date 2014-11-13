Sudoku solvers
==============

I know that a lot of Sudoku solvers already exist. I just wanted to play with backtracking and familiarize myself with the Dancing Links technique. 

You will find in this repository two solvers I implemented for Sudoku grids:
- naive backtracking
- backtracking again: exact cover problem solving with Dancing Links (the most exciting part)

Exact cover problem
===================

Given a set S of elements subject to a finite set of constraints, the exact cover problem consists in finding a collection in S such that each constraint is satisfied once and only once. Knuth proposed a backtracking approach to solve this problem (look for Knuth's algorithm X), and suggested a technique (Dancing Links aka DLX) to efficiently implement his algorithm.
Now take a Sudoku grid: the set S of elements is the different values for each cell (9 possible values for each cell). For a 9x9 grid, this amounts to 9x9x9=729 elements.
Besides, each element is bound to the following constraints:

1. Each cell must contain a value: as there are 81 cells in a 9x9 grid, there are 81 such constraints
2. Each row must contain the integers from 1 to 9: as there are 9 rows and 9 possible values, there are 81 such constraints
3. Same as 2. but for columns
4. Each 3x3 square must contain the integers from 1 to 9: as there are 9 3x3 squares in a 9x9 grid and 9 possible values, there are 81 such constraints

To sum up, a total of 81x4=324 constraints which must be satisfied. By definition of the rules of a 9x9 Sudoku grid, each element among the 729 possibilities satisfy the 4 constraints above. Solving a 9x9 Sudoku grid boils down to finding the 81 elements among the 729 possibilities such that each of the 324 constraints is satisfied once and only once. At each step in the backtracking, the constraint which has the least degree of freedom is dealt with (each time a constraint is satisfied, the degrees of freedom of the other constraints decrease). This allows accelerating substantially the convergence towards a solution on the tested grids.

The classes in Node.h allow building the constraint matrix representation of an exact cover problem. You can refer to the code in Main.cpp for an example of matrix construction for a 2x2 Sudoku grid, and to the implementation of SudokuGrid::SolveWithDLX() for a 9x9 classic Sudoku grid. Note that in order to link together the different elements/nodes of the sparse constraint matrix in constant time, the matrix is built row by row. The number of recursive calls to the backtracking exploration function is given as a comparison between my naive backtracking and my DLX implementation.

Additional reading
==================
http://garethrees.org/2007/06/10/zendoku-generation/

