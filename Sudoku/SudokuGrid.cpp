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
	if (cells_[current_row][current_column])
	{
		if (current_column == 8)
		{
			if (current_row == 8)	// Solution found
				return true;
			else
				return Backtrack(current_row + 1, 0);
		}
		else
			return Backtrack(current_row, current_column + 1);
	}
	for (int value = 1; value < 10; ++value)
	{
		cells_[current_row][current_column] = value;
		if (!CheckValue(current_row, current_column))
			continue;
		else
		{
			if (current_column == 8)
			{
				if (current_row == 8)	// Solution found
					return true;
				else
				{
					if (!Backtrack(current_row + 1, 0))
						continue;
					else
						return true;
				}
			}
			else
			{
				if (!Backtrack(current_row, current_column + 1))
					continue;
				else
					return true;
			}
		}
	}
	cells_[current_row][current_column] = 0;
	return false;
}

void SudokuGrid::Solve()
{
	Backtrack(0, 0);
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
