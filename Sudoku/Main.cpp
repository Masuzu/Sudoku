#include "SudokuGrid.h"

#include <iostream>

int main()
{
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