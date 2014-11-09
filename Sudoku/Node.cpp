#include "Node.h"

#include <iostream>

Node::Node(ColumnHeaderNode *header_node) : header_node_(header_node)
{
	++header_node_->num_cells_in_column_;
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
	--header_node_->num_cells_in_column_;
	if (up_)
		up_->down_ = down_;
	if (down_)
		down_->up_ = up_;
}

void Node::InsertBackInColumn()
{
	++header_node_->num_cells_in_column_;
	if (up_)
		up_->down_ = this;
	if (down_)
		down_->up_ = this;
}

void Node::PrintRow()
{
	if (left_)
	{
		int num_blanks = header_node_->column_index() - left_->header_node_->column_index();
		for (int i = 0; i < num_blanks*2-1; ++i)
			std::cout << "=";
	}
	std::cout << "#";
	if (right_)
		right_->PrintRow();
}

void ColumnHeaderNode::PrintRow()
{
	std::cout << num_cells_in_column_;
	if (right_)
	{
		std::cout << "=";
		right_->PrintRow();
	}
}