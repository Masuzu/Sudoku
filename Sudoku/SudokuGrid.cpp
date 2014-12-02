#include "SudokuGrid.h"

#include <set>
#include <iostream>

#include "Node.h"

SudokuGrid::SudokuGrid()
{
	for (int row = 0; row < 9; ++row)
		for (int column = 0; column < 9; ++column)
			cells_[row][column] = 0;
}

void SudokuGrid::Set(unsigned int row, unsigned int column, int value)
{
	cells_[row][column] = value;
}

void SudokuGrid::SetRow(unsigned int row, const std::initializer_list<unsigned int> &values)
{
	auto it = values.begin();
	for (int column = 0; column < 9; ++column, ++it)
		cells_[row][column] = *it;
}

int SudokuGrid::Get(unsigned int row, unsigned int column)
{
	return cells_[row][column];
}

bool SudokuGrid::CheckRow(unsigned int row)
{
	std::set<int> values;
	for (int column = 0; column < 9; ++column)
	{
		int cell_value = cells_[row][column];
		if (cell_value)
		{
			if (!values.insert(cell_value).second)
				return false;
		}
	}
	return true;
}
bool SudokuGrid::CheckColumn(unsigned int column)
{
	std::set<int> values;
	for (int row = 0; row < 9; ++row)
	{
		int cell_value = cells_[row][column];
		if (cell_value)
		{
			if (!values.insert(cell_value).second)
				return false;
		}
	}
	return true;
}

bool SudokuGrid::CheckSquare(unsigned int i, unsigned int j)
{
	std::set<int> values;
	for (int k = 0; k < 3; ++k)
	{
		for (int l = 0; l < 3; ++l)
		{
			int cell_value = cells_[3 * i + k][3 * j + l];
			if (cell_value)
			{
				if (!values.insert(cell_value).second)
					return false;
			}
		}
	}
	return true;
}

bool SudokuGrid::CheckValue(unsigned int row, unsigned int column)
{
	if (!CheckRow(row) || !CheckColumn(column) || !CheckSquare(row / 3, column / 3))
		return false;
	return true;
}

bool SudokuGrid::Backtrack(unsigned int current_row, unsigned int current_column)
{
	++num_iterations_;
	for (int value = 1; value < 10; ++value)
	{
		cells_[current_row][current_column] = value;
		if (!CheckValue(current_row, current_column))
			continue;
		else
		{
			// Find the next cell which contains a '0'
			unsigned int next_index = current_row * 9 + current_column + 1;
			if (next_index >= 81)
				return true;	// Solution found
			unsigned int next_row = next_index / 9;
			unsigned int next_column = next_index % 9;
			while (cells_[next_row][next_column])
			{
				next_index = next_row * 9 + next_column + 1;
				if (next_index >= 81)
					return true;	// Solution found
				next_row = next_index / 9;
				next_column = next_index % 9;
				
			}
			if (Backtrack(next_row, next_column))
				return true;
		}
	}
	cells_[current_row][current_column] = 0;
	return false;
}

bool SudokuGrid::CheckGrid()
{
	for (int i = 0; i < 9; ++i)
	{
		if (!CheckRow(i))
			return false;
		if (!CheckColumn(i))
			return false;
	}
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
			if (!CheckSquare(i, j))
				return false;
	}
	return true;
}

void SudokuGrid::Solve()
{
	if (!CheckGrid())
	{
		std::cout << "Invalid grid" << std::endl;
		return;
	}
	num_iterations_ = 0;
	unsigned int next_row = 0;
	unsigned int next_column = 0;
	while (cells_[next_row][next_column])
	{
		unsigned int next_index = (next_row * 9 + next_column + 1);
		if (next_index == 81)
			return;	// Solution found
		next_row = next_index / 9;
		next_column = next_index % 9;
	}
	Backtrack(next_row, next_column);
}

struct Cell
{
	int row;
	int column;
	int number;
};

void SudokuGrid::SolveWithDLX()
{
	if (!CheckGrid())
	{
		std::cout << "Invalid grid" << std::endl;
		return;
	}

	// For a 9x9 grid, each cell can be filled with either '1' '2' and so on until '9', hence a total of 9*9*9 possible cell values.
	// Each cell must also satisfy a set of 4 constraints, which amounts to 4*81 constraints.
	ConstraintMatrix *constraint_matrix = new ConstraintMatrix(729, 324);
	RowHeaderNode **row_header_nodes = constraint_matrix->row_header_nodes();
	ColumnHeaderNode **column_header_nodes = constraint_matrix->column_header_nodes();
	// Bookkeeping for the downmost node of each column of the constraint matrix
	// Allows linking nodes together in constant time
	Node **downmost_nodes = new Node*[324];
	for (int column = 0; column < 324; ++column)
		downmost_nodes[column] = column_header_nodes[column];

	// Bijection between the rows in the constraint matrix and the cell values in the 9x9 grid
	Cell cells[729];
	// Add new nodes row by row
	for (int row = 0; row < 9; ++row)
	{
		for (int column = 0; column < 9; ++column)
		{
			for (int number = 0; number < 9; ++number)
			{
				int row_index = row * 81 + column * 9 + number;	// Index of the row being initialized in the constraint matrix
				cells[row_index].row = row;
				cells[row_index].column = column;
				cells[row_index].number = number+1;

				// See http://garethrees.org/2007/06/10/zendoku-generation/#section-4.1 Figure 2
				// Constraint1: one number in 'column' and 'row'
				Node *node_constraint_1 = new Node(row_header_nodes[row_index], column_header_nodes[column * 9 + row]);
				row_header_nodes[row_index]->AddNodeToRight(node_constraint_1);
				downmost_nodes[column * 9 + row]->AddNodeDown(node_constraint_1);
				downmost_nodes[column * 9 + row] = node_constraint_1;

				// Constraint2: 'number' must appear in 'row'
				Node *node_constraint_2 = new Node(row_header_nodes[row_index], column_header_nodes[number * 9 + row + 81]);
				downmost_nodes[number * 9 + row + 81]->AddNodeDown(node_constraint_2);
				downmost_nodes[number * 9 + row + 81] = node_constraint_2;
				node_constraint_1->AddNodeToRight(node_constraint_2);

				// Constraint3: 'number' must appear in 'column'
				Node *node_constraint_3 = new Node(row_header_nodes[row_index], column_header_nodes[number * 9 + column + 162]);
				downmost_nodes[number * 9 + column + 162]->AddNodeDown(node_constraint_3);
				downmost_nodes[number * 9 + column + 162] = node_constraint_3;
				node_constraint_2->AddNodeToRight(node_constraint_3);

				// Constraint4: 'number' must appear in the 3x3 square at (row/3;column/3)
				Node *node_constraint_4 = new Node(row_header_nodes[row_index], column_header_nodes[((row / 3) * 3 + (column / 3)) * 9 + number + 243]);
				downmost_nodes[((row / 3) * 3 + (column / 3)) * 9 + number + 243]->AddNodeDown(node_constraint_4);
				downmost_nodes[((row / 3) * 3 + (column / 3)) * 9 + number + 243] = node_constraint_4;
				node_constraint_3->AddNodeToRight(node_constraint_4);
			}
		}
	}

	for (int row = 0; row < 9; ++row)
	{
		for (int column = 0; column < 9; ++column)
		{
			if (cells_[row][column])
				// cells_ values are taken in [[1;9]] while the constraint matrix is built taking values in [[0; 9]]
				constraint_matrix->RemoveRow(row * 81 + column * 9 + cells_[row][column]-1);
		}
	}

	std::vector<int> solution;
	constraint_matrix->ExactCover(solution);

	for (int row : solution)
		cells_[cells[row].row][cells[row].column] = cells[row].number;
	num_iterations_ = constraint_matrix->num_iterations();

	delete constraint_matrix;
}

void SudokuGrid::Print()
{
	for (int row = 0; row < 9; ++row)
	{
		for (int column = 0; column < 9; ++column)
			std::cout << cells_[row][column];
		std::cout << std::endl;
	}
}
