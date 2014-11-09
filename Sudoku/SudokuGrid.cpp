#include "SudokuGrid.h"

#include <set>
#include <iostream>

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
	for (int value = 1; value < 10; ++value, ++num_iterations_)
	{
		cells_[current_row][current_column] = value;
		if (!CheckValue(current_row, current_column))
			continue;
		else
		{
			// Find the next cell which contains a '0'
			unsigned int next_row = (current_row * 9 + current_column + 1) / 9;
			unsigned int next_column = (current_row * 9 + current_column + 1) % 9;
			while (cells_[next_row][next_column])
			{
				unsigned int next_index = (next_row * 9 + next_column + 1);
				next_row = next_index / 9;
				next_column = next_index % 9;
				if (next_row == 9)
					return true;	// Solution found
			}
			if (Backtrack(next_row, next_column))
				return true;
		}
	}
	cells_[current_row][current_column] = 0;
	return false;
}

void SudokuGrid::Solve()
{
	num_iterations_ = 0;
	unsigned int next_row = 0;
	unsigned int next_column = 0;
	while (cells_[next_row][next_column])
	{
		unsigned int next_index = (next_row * 9 + next_column + 1);
		next_row = next_index / 9;
		next_column = next_index % 9;
		if (next_row == 9)
			return;	// Solution found
	}
	Backtrack(next_row, next_column);
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
