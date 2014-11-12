#include "Node.h"

#include <iostream>

Node::Node(Node *row_header_node, ColumnHeaderNode *column_header_node)
	: row_header_node_(row_header_node), header_node_(column_header_node)
{

}

Node::~Node()
{
	if (left_)
		delete left_;
	if (right_)
		delete left_;
	if (down_)
		delete down_;
	if (up_)
		delete up_;
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
	if (up_)
		up_->down_ = down_;
	if (down_)
		down_->up_ = up_;
}

void Node::InsertBackInColumn()
{
	if (up_)
		up_->down_ = this;
	if (down_)
		down_->up_ = this;
}

void ColumnHeaderNode::Cover()
{
	is_covered_ = true;
	// Go through the nodes in the column and remove them
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
	reinterpret_cast<ColumnHeaderNode*>(right_)->left_ = left_;
}

void ColumnHeaderNode::Uncover()
{
	is_covered_ = false;
	// Go through the nodes in the column and remove them
	Node *node = down_;
	while (node)
	{
		// Go through the nodes in the same row as 'node' and remove them from their column
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
	reinterpret_cast<ColumnHeaderNode*>(right_)->left_ = this;
}

ConstraintMatrix::ConstraintMatrix(unsigned int num_rows, unsigned int num_columns)
{
	if (!num_rows || !num_columns)
		exit(-1);
	// Create rows
	row_header_nodes_ = new Node*[num_rows];
	root_row_header_node = new Node();
	row_header_nodes_[0] = new Node();
	root_row_header_node->AddNodeDown(row_header_nodes_[0]);
	for (int row = 1; row < num_rows; ++row)
	{
		row_header_nodes_[row] = new Node();
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
				while (node->header_node()->column_index() < column_header_node->column_index())
					node = node->right();
				if (node->header_node()->column_index() == column_header_node->column_index())
				{
					node = node->right();
					if (!column_header_node->is_covered())
						std::cout << "*";
				}
				else if(!column_header_node->is_covered())
					std::cout << "=";
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