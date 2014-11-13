#include "SudokuGrid.h"
#include "Node.h"

#include <iostream>

int main()
{
	// For a 2x2 grid, each cell can be filled with either '1' or '2', hence a total of 2*2*2 possible cell values.
	ConstraintMatrix *constraint_matrix = new ConstraintMatrix(8, 12);
	RowHeaderNode **row_header_nodes = constraint_matrix->row_header_nodes();
	ColumnHeaderNode **column_header_nodes = constraint_matrix->column_header_nodes();
	// Bookkeeping for the downmost node of each column of the constraint matrix
	// Allows linking nodes together in constant time
	Node **downmost_nodes = new Node*[12];
	for (int column = 0; column < 12; ++column)
		downmost_nodes[column] = column_header_nodes[column];

	// Add new nodes row by row
	for (int row = 0; row < 2; ++row)
	{
		for (int column = 0; column < 2; ++column)
		{
			for (int number = 0; number < 2; ++number)
			{
				int row_index = row * 4 + column * 2 + number;	// Index of the row being initialized in the constraint matrix
				// See http://garethrees.org/2007/06/10/zendoku-generation/#section-4.1 Figure 2
				// Constraint1: one number in 'column' and 'row'
				Node *node_constraint_1 = new Node(row_header_nodes[row_index], column_header_nodes[column * 2 + row]);
				row_header_nodes[row_index]->AddNodeToRight(node_constraint_1);
				downmost_nodes[column * 2 + row]->AddNodeDown(node_constraint_1);
				downmost_nodes[column * 2 + row] = node_constraint_1;

				// Constraint2: 'number' must appear in 'row'
				Node *node_constraint_2 = new Node(row_header_nodes[row_index], column_header_nodes[number * 2 + row + 4]);
				downmost_nodes[number * 2 + row + 4]->AddNodeDown(node_constraint_2);
				downmost_nodes[number * 2 + row + 4] = node_constraint_2;
				node_constraint_1->AddNodeToRight(node_constraint_2);

				// Constraint3: 'number' must appear in 'column'
				Node *node_constraint_3 = new Node(row_header_nodes[row_index], column_header_nodes[number * 2 + column + 8]);
				downmost_nodes[number * 2 + column + 8]->AddNodeDown(node_constraint_3);
				downmost_nodes[number * 2 + column + 8] = node_constraint_3;
				node_constraint_2->AddNodeToRight(node_constraint_3);
			}
		}
	}

	constraint_matrix->Print();

	std::vector<int> solution;
	std::vector<std::vector<int>> solutions;
	constraint_matrix->ExactCover(solutions);

	std::cout << constraint_matrix->IsCandidateSolution() << " " << constraint_matrix->ExactCoverFound() << std::endl;
	SudokuGrid grid;
#if 0
	grid.SetRow(0, { 4, 9, 8, 0, 0, 0, 6, 2, 0 });
	grid.SetRow(1, { 7, 0, 5, 0, 3, 0, 4, 0, 8 });
	grid.SetRow(2, { 3, 0, 6, 8, 0, 0, 0, 0, 5 });
	grid.SetRow(3, { 0, 0, 0, 0, 7, 0, 8, 0, 0 });
	grid.SetRow(4, { 0, 3, 0, 0, 8, 9, 2, 5, 0 });
	grid.SetRow(5, { 0, 8, 0, 6, 0, 5, 0, 0, 1 });
	grid.SetRow(6, { 0, 6, 3, 5, 1, 0, 0, 0, 0 });
	grid.SetRow(7, { 2, 4, 0, 0, 0, 0, 0, 8, 6 });
	grid.SetRow(8, { 5, 7, 9, 0, 6, 8, 0, 3, 2 });
#endif
	grid.SetRow(0, { 0, 3, 0, 6, 0, 5, 0, 0, 0 });
	grid.SetRow(1, { 6, 0, 0, 0, 9, 0, 0, 0, 2 });
	grid.SetRow(2, { 0, 7, 0, 1, 0, 0, 0, 0, 6 });
	grid.SetRow(3, { 0, 9, 0, 0, 0, 0, 0, 0, 0 });
	grid.SetRow(4, { 8, 1, 0, 0, 5, 0, 0, 6, 9 });
	grid.SetRow(5, { 0, 0, 0, 0, 0, 0, 0, 8, 0 });
	grid.SetRow(6, { 4, 0, 0, 0, 0, 3, 0, 2, 0 });
	grid.SetRow(7, { 9, 0, 0, 0, 2, 0, 0, 0, 5 });
	grid.SetRow(8, { 0, 0, 0, 9, 0, 8, 0, 3, 0 });

	grid.Solve();
	grid.Print();
	std::cout << "Number of iterations: " << grid.num_iterations() << std::endl;
	system("pause");

}