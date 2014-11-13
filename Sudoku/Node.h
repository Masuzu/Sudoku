#ifndef __Node_H__
#define __Node_H__

#include <vector>

class RowHeaderNode;
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
	RowHeaderNode *row_header_node_ = nullptr;

public:
	inline Node()	{}
	Node(RowHeaderNode *row_header_node, ColumnHeaderNode *column_header_node);
	// Pointers to other nodes are not deleted
	virtual ~Node()	{}

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
	inline RowHeaderNode *row_header_node()	{ return row_header_node_; }
};

// Row headers label the rows of the constraint matrix reprensenting an exact cover problem
class RowHeaderNode : public Node
{
private:
	int row_index_;
public:
	inline explicit RowHeaderNode(int row_index) : row_index_(row_index)	{}
	inline int row_index()	{ return row_index_; }
};

// Column headers label the columns of the constraint matrix reprensenting an exact cover problem
class ColumnHeaderNode : public Node
{
	friend class Node;
private:
	int column_index_;
	unsigned int num_of_rows_ = 0;
	bool is_covered_ = false;

public:
	inline explicit ColumnHeaderNode(int column_index) : column_index_(column_index)	{}
	inline int column_index()	{ return column_index_; }
	inline bool is_covered()	{ return is_covered_; }
	inline unsigned int num_of_rows()	{ return num_of_rows_; }

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
	RowHeaderNode **row_header_nodes_;
	int num_iterations_ = 0;

	bool Backtrack(std::vector<int> &solution);
	void Backtrack(std::vector<int> &solution, std::vector<std::vector<int>> &solutions);

public:
	ConstraintMatrix(unsigned int num_rows, unsigned int num_columns);
	void Print();
	void RemoveRow(unsigned int row);
	void InsertBackRow(unsigned int row);
	// Outputs a vector of row indices which are a solution of the exact cover problem
	void ExactCover(std::vector<int> &solution);
	void ExactCover(std::vector<std::vector<int>> &solutions);
	bool ExactCoverFound();
	bool IsCandidateSolution();

	inline ColumnHeaderNode **column_header_nodes()	{ return column_header_nodes_; }
	inline RowHeaderNode **row_header_nodes()	{ return row_header_nodes_; }
	inline int num_iterations()	{ return num_iterations_; }
};

#endif