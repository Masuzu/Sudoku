#ifndef __Node_H__
#define __Node_H__

class ColumnHeaderNode;
class Node
{
private:
	// Disable copy constructor
	explicit Node(const ColumnHeaderNode &header_node)	{}
protected:
	Node *left_ = nullptr;
	Node *right_ = nullptr;
	Node *down_ = nullptr;
	Node *up_ = nullptr;
	ColumnHeaderNode *header_node_ = nullptr;
	Node()	{}

public:
	explicit Node(ColumnHeaderNode *header_node);
	virtual ~Node();

	inline Node *left()	{ return left_; }
	inline Node *right()	{ return right_; }
	inline Node *down()	{ return down_; }
	inline Node *up()	{ return up_; }
	void AddNodeToRight(Node *node);
	void AddNodeDown(Node *node);
	// Updates the nodes up and down and decrements header_node_->num_cells_in_column_
	void RemoveFromColumn();
	// Updates the nodes up and down and increments header_node_->num_cells_in_column_
	void InsertBackInColumn();
	inline ColumnHeaderNode *header_node()	{ return header_node_; }
	virtual void PrintRow();
};

class ColumnHeaderNode : public Node
{
	friend class Node;
private:
	unsigned int num_cells_in_column_ = 0;
	int column_index_;
public:
	ColumnHeaderNode(int column_index)	{ column_index_ = column_index; }
	inline unsigned int num_cells_in_column()	{ return num_cells_in_column_; }
	inline int column_index()	{ return column_index_; }

	// Implementation of the "cover column" operation in Knuth's dancing links algorithm
	void Cover();
	// Print the binary constraint matrix
	void PrintRow();
};

#endif