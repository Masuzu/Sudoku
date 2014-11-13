#include "Node.h"

#include <iostream>
#include <set>

Node::Node(RowHeaderNode *row_header_node, ColumnHeaderNode *column_header_node)
	: row_header_node_(row_header_node), header_node_(column_header_node)
{
	++header_node_->num_of_rows_;
}

void Node::AddNodeToRight(Node *node)
{
	right_ = node;
	node->left_ = this;
}

void Node::AddNodeDown(Node *node)
{
	down_ = node;
	node->up_ = this;
}

void Node::RemoveFromColumn()
{
	if (header_node_)
		--header_node_->num_of_rows_;
	if (up_)
		up_->down_ = down_;
	if (down_)
		down_->up_ = up_;
}

void Node::InsertBackInColumn()
{
	if (header_node_)
		++header_node_->num_of_rows_;
	if (up_)
		up_->down_ = this;
	if (down_)
		down_->up_ = this;
}

void ColumnHeaderNode::Cover()
{
	is_covered_ = true;
	// Go through the nodes in the column
	Node *node = down_;
	while (node)
	{
		// Go through the nodes in the same row as 'node' and remove them from their column
		Node *row_node = node->row_header_node()->right();
		while (row_node)
		{
			if (row_node != node)
				row_node->RemoveFromColumn();
			row_node = row_node->right();
		}
		node = node->down();
	}
	// Remove this column header node from the header node row
	reinterpret_cast<ColumnHeaderNode*>(left_)->right_ = right_;
	if (right_)
		reinterpret_cast<ColumnHeaderNode*>(right_)->left_ = left_;
}

void ColumnHeaderNode::Uncover()
{
	is_covered_ = false;
	// Go through the nodes in the column
	Node *node = down_;
	while (node)
	{
		// Go through the nodes in the same row as 'node' and insert them back in their column
		Node *row_node = node->row_header_node()->right();
		while (row_node)
		{
			if (row_node != node)
				row_node->InsertBackInColumn();
			row_node = row_node->right();
		}
		node = node->down();
	}
	// Remove this column header node from the header node row
	reinterpret_cast<ColumnHeaderNode*>(left_)->right_ = this;
	if (right_)
		reinterpret_cast<ColumnHeaderNode*>(right_)->left_ = this;
}

ConstraintMatrix::ConstraintMatrix(unsigned int num_rows, unsigned int num_columns)
{
	if (!num_rows || !num_columns)
		exit(-1);
	// Create rows
	row_header_nodes_ = new RowHeaderNode*[num_rows];
	root_row_header_node = new Node();
	row_header_nodes_[0] = new RowHeaderNode(0);
	root_row_header_node->AddNodeDown(row_header_nodes_[0]);
	for (int row = 1; row < num_rows; ++row)
	{
		row_header_nodes_[row] = new RowHeaderNode(row);
		row_header_nodes_[row - 1]->AddNodeDown(row_header_nodes_[row]);
	}

	// Create columns
	column_header_nodes_ = new ColumnHeaderNode*[num_columns];
	root_column_header_node = new Node();
	column_header_nodes_[0] = new ColumnHeaderNode(0);
	root_column_header_node->AddNodeToRight(column_header_nodes_[0]);
	for (int column = 1; column < num_columns; ++column)
	{
		column_header_nodes_[column] = new ColumnHeaderNode(column);
		column_header_nodes_[column - 1]->AddNodeToRight(column_header_nodes_[column]);
	}
}

void ConstraintMatrix::Print()
{
	if (!root_column_header_node->right())	// There are no columns
		return;
	// Go through the rows of the constraint matrix
	Node *row_header_node = root_row_header_node->down();
	while (row_header_node)
	{
		// Go through the nodes in the row labeled by 'row_node'
		Node *node = row_header_node->right();
		ColumnHeaderNode *column_header_node = reinterpret_cast<ColumnHeaderNode*>(root_column_header_node->right());
		while (column_header_node)
		{
			if (node)
			{
				// Get the first node in 'row_header_node' which is in a column at the right of 'column_header_node'
				while (node->header_node()->column_index() < column_header_node->column_index())
				{
					node = node->right();
					if (!node)
						break;
				}
				if (!node)
				{
					if (!column_header_node->is_covered())
						std::cout << "=";
				}
				else
				{
					if (node->header_node()->column_index() == column_header_node->column_index())
					{
						node = node->right();
						if (!column_header_node->is_covered())
							std::cout << "*";
					}
					else if (!column_header_node->is_covered())
						std::cout << "=";
				}
			}
			else if (!column_header_node->is_covered())
				std::cout << "=";
			
			column_header_node = reinterpret_cast<ColumnHeaderNode*>(column_header_node->right());
		}
		while (column_header_node)
		{
			if (!column_header_node->is_covered())
				std::cout << "=";	// Add '=' symbols until the last non-covered column is reached
			column_header_node = reinterpret_cast<ColumnHeaderNode*>(column_header_node->right());
		}

		std::cout << std::endl;
		row_header_node = row_header_node->down();
	}
}

void ConstraintMatrix::RemoveRow(unsigned int row)
{
	// Go through the row nodes
	Node *node = row_header_nodes_[row]->right();
	while (node)
	{
		node->header_node()->Cover();
		node = node->right();
	}
	row_header_nodes_[row]->RemoveFromColumn();
}

bool ConstraintMatrix::Backtrack(std::vector<int> &solution)
{
	if (ExactCoverFound())
		return true;
	// Get the column with the least rows
	ColumnHeaderNode *column_header_node = reinterpret_cast<ColumnHeaderNode*>(root_column_header_node->right());
	ColumnHeaderNode *column_to_cover = column_header_node;
	unsigned int min_num_of_rows = UINT_MAX;
	while (column_header_node)
	{
		if (min_num_of_rows < column_header_node->num_of_rows())
		{
			min_num_of_rows = column_header_node->num_of_rows();
			column_to_cover = column_header_node;
		}
		column_header_node = reinterpret_cast<ColumnHeaderNode*>(column_header_node->right());
	}
	// Remove a row from 'column_to_cover' and repeat the process until
	Node *node = column_to_cover->down();
	while (node)
	{
		int row_index = node->row_header_node()->row_index();
		solution.push_back(row_index);
		RemoveRow(row_index);
		if (!IsCandidateSolution())	// Backtrack
		{
			solution.pop_back();
			InsertBackRow(row_index);
		}
		else
		{
			if (Backtrack(solution))
				return true;
		}
		node = node->down();
	}
	
	return false;
}

void ConstraintMatrix::Backtrack(std::vector<int> &solution, std::vector<std::vector<int>> &solutions)
{
	if (ExactCoverFound())
	{
		solutions.push_back(solution);
		return;
	}
	// Get the column with the least rows
	ColumnHeaderNode *column_header_node = reinterpret_cast<ColumnHeaderNode*>(root_column_header_node->right());
	ColumnHeaderNode *column_to_cover = column_header_node;
	unsigned int min_num_of_rows = UINT_MAX;
	while (column_header_node)
	{
		if (min_num_of_rows < column_header_node->num_of_rows())
		{
			min_num_of_rows = column_header_node->num_of_rows();
			column_to_cover = column_header_node;
		}
		column_header_node = reinterpret_cast<ColumnHeaderNode*>(column_header_node->right());
	}
	// Remove a row from 'column_to_cover' and repeat the process until
	Node *node = column_to_cover->down();
	while (node)
	{
		int row_index = node->row_header_node()->row_index();
		solution.push_back(row_index);
		RemoveRow(row_index);
		if (IsCandidateSolution())	// Backtrack
			Backtrack(solution, solutions);
		solution.pop_back();
		InsertBackRow(row_index);
		node = node->down();
	}
}

bool ConstraintMatrix::ExactCoverFound()
{
	return(!root_column_header_node->right());
}

bool ConstraintMatrix::IsCandidateSolution()
{
	ColumnHeaderNode *node = reinterpret_cast<ColumnHeaderNode*>(root_column_header_node->right());
	while (node)
	{
		if (!node->num_of_rows())
			return false;
		node = reinterpret_cast<ColumnHeaderNode*>(node->right());
	}
	return true;
}

void ConstraintMatrix::InsertBackRow(unsigned int row)
{
	// Go through the row nodes
	Node *node = row_header_nodes_[row]->right();
	// Uncover the columns in the reverse order they were covered
	// Get the rightmost node
	while (node->right())
		node = node->right();
	while (node)
	{
		if (node != row_header_nodes_[row])
			node->header_node()->Uncover();
		node = node->left();
	}
	row_header_nodes_[row]->InsertBackInColumn();
}

void ConstraintMatrix::ExactCover(std::vector<int> &solution)
{
	Backtrack(solution);
}

void ConstraintMatrix::ExactCover(std::vector<std::vector<int>> &solutions)
{
	std::vector<int> solution;
	Backtrack(solution, solutions);
}