#ifndef __Node_H__
#define __Node_H__

class ColumnHeaderNode;
class Node
{
private:
	// Disable copy constructor
	explicit Node(const Node &header_node)	{}
protected:
	Node *left_ = nullptr;
	Node *right_ = nullptr;
	Node *down_ = nullptr;
	Node *up_ = nullptr;
	ColumnHeaderNode *header_node_ = nullptr;
	Node *row_header_node_ = nullptr;

public:
	inline Node()	{}
	Node(Node *row_header_node, ColumnHeaderNode *column_header_node);
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
	inline Node *row_header_node()	{ return row_header_node_; }
};

// Column headers label the columns of the constraint matrix reprensenting an exact cover problem.
class ColumnHeaderNode : public Node
{
private:
	int column_index_;
	bool is_covered_ = false;

public:
	inline ColumnHeaderNode(int column_index) : column_index_(column_index)	{}
	inline int column_index()	{ return column_index_; }
	inline bool is_covered()	{ return is_covered_; }

	// Implementation of the "cover column" operation in Knuth's dancing links algorithm
	void Cover();
	// Implementation of the "uncover column" operation in Knuth's dancing links algorithm
	void Uncover();
};

class ConstraintMatrix
{
private:
	// Column header nodes are added right of root_column_header_node
	Node *root_column_header_node;
	ColumnHeaderNode **column_header_nodes_;
	// Row header nodes are added down of root_row_header_node
	Node *root_row_header_node;
	// row_header_nodes_[i] points to the first node in the row i of the constraint matrix representing the exact cover problem
	Node **row_header_nodes_;
public:
	ConstraintMatrix(unsigned int num_rows, unsigned int num_columns);
	void Print();
	inline ColumnHeaderNode **column_header_nodes()	{ return column_header_nodes_; }
	inline Node **row_header_nodes()	{ return row_header_nodes_; }
};

#endif