#include "SudokuGrid.h"
#include "Node.h"

#include <iostream>

int main()
{
	ColumnHeaderNode **latin_square_column_header = new ColumnHeaderNode*[12];
	for (int i = 0; i < 12; ++i)
		latin_square_column_header[i] = new ColumnHeaderNode(i);
	for (int i = 0; i < 11; ++i)
		latin_square_column_header[i]->AddNodeToRight(latin_square_column_header[i + 1]);

	for (int row = 0; row < 2; ++row)
	{
		for (int column = 0; column < 2; ++column)
		{
			for (int number = 0; number < 2; ++number)
			{
				// See http://garethrees.org/2007/06/10/zendoku-generation/#section-4.1 Figure 2
				// Constraint1: one number in the column 'column' and row 'row'
				Node *node_constraint_1 = new Node(latin_square_column_header[column * 2 + row]);
				latin_square_column_header[column * 2 + row]->AddNodeDown(node_constraint_1);

				// Constraint2: the number 'number' must appear in row 'row'
				Node *node_constraint_2 = new Node(latin_square_column_header[number * 2 + row + 4]);
				latin_square_column_header[number * 2 + row + 4]->AddNodeDown(node_constraint_2);
				node_constraint_1->AddNodeToRight(node_constraint_2);

				// Constraint3: the number 'number' must appear in column 'column'
				Node *node_constraint_3 = new Node(latin_square_column_header[number * 2 + column + 8]);
				latin_square_column_header[number * 2 + column + 8]->AddNodeDown(node_constraint_3);
				node_constraint_2->AddNodeToRight(node_constraint_3);
			}
		}
	}

	latin_square_column_header[0]->PrintRow();
	std::cout << std::endl;
	for (int i = 0; i < 12; ++i)
	{
		latin_square_column_header[i]->down()->PrintRow();
		std::cout << std::endl;
	}

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