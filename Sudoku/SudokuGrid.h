#ifndef __SudokuGrid_H__
#define __SudokuGrid_H__

#include <initializer_list>

class SudokuGrid
{
private:
	int cells_[9][9];
	unsigned int num_iterations_;
	bool CheckRow(unsigned int row);
	bool CheckColumn(unsigned int column);
	bool CheckSquare(unsigned int i, unsigned int j);
	bool Backtrack(unsigned int current_row, unsigned int current_column);

public:
	SudokuGrid();
	void SetRow(unsigned int row, const std::initializer_list<unsigned int> &values);
	void Set(unsigned int row, unsigned int column, int value);
	int Get(unsigned int row, unsigned int column);
	bool CheckValue(unsigned int row, unsigned int column);
	void Solve();
	void Print();

	inline unsigned int num_iterations()	{ return num_iterations_; }
};

#endif