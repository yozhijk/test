#include "stdafx.h"

template <typename T> class DefaultTraits
{
public:
	static bool less (T const& t1, T const& t2)
	{
		return t1 < t2;
	}

	static bool equal (T const& t1, T const& t2)
	{
		return t1 == t2;
	}
};

template <typename T, typename T_Traits = DefaultTraits<T>> 
class IteratorBase
{
public:
	virtual ~IteratorBase() = 0 {}
	virtual void Reset() = 0;
	virtual bool HasNext() = 0;
	virtual void Next() = 0;
	virtual T& Value() = 0;
};

template <typename T, typename T_Traits = DefaultTraits<T>> 
class BinaryTreeIterator;

template <typename T, typename T_Traits = DefaultTraits<T>>
class BinaryTree
{
public:
	/// Base iterator class
	typedef std::unique_ptr< IteratorBase<T, T_Traits> > Iterator;

	BinaryTree();

	template <typename U, typename U_Traits> BinaryTree( BinaryTree<U, U_Traits> const& other );

	virtual ~BinaryTree();

	void AddKey(T const& val);
	virtual Iterator CreateIterator() const;

protected:
	struct Node;

	Node* Root() const { return root_; }
	Node* FindNode(T const& val) const;

	/// Provide this to customize memory management 
	virtual Node* NewNode(T const& val = T())
	{
		Node* tmp = new Node(val);
		allocations_.push_back(tmp);
		return tmp;
	}

	struct Node 
	{
		T value_;
		Node* right_;
		Node* left_;
		Node(T const& val = T()) : value_(val), right_(nullptr), left_(nullptr){}
	};

private:
	Node* root_;
	std::vector<Node*> allocations_;

	friend class BinaryTreeIterator<T, T_Traits>;
};



template <typename T, typename T_Traits> 
class BinaryTreeIterator : public IteratorBase<T, T_Traits>
{
public:
	~BinaryTreeIterator(){}
	void Reset()
	{
		ResetImpl();
	}

	bool HasNext()
	{
		return (navigationStack_.size() > 1) || (navigationStack_.size() == 1 && navigationStack_.top()->right_) ;
	}

	void Next()
	{
		assert (!navigationStack_.empty());
		Node* top = navigationStack_.top();
		navigationStack_.pop();

		if (top->right_)
		{
			DrillDownLeft(top->right_);
		}
	}

	T& Value()
	{
		assert (!navigationStack_.empty());
		return navigationStack_.top()->value_;
	}

private:
	typedef typename BinaryTree<T, T_Traits>::Node Node;

	void ResetImpl()
	{
		navigationStack_.swap(std::stack<Node*>());

		if (tree_.root_)
		{
			DrillDownLeft(tree_.root_);
		}
	}

	void DrillDownLeft(Node* node)
	{
		assert(node);
		while(node)
		{
			navigationStack_.push(node);
			node = node->left_;
		}
	}

	BinaryTreeIterator( BinaryTree<T, T_Traits> const& tree ) : tree_(tree) 
	{
		ResetImpl();
	}


	BinaryTree<T, T_Traits> const& tree_;
	std::stack<Node*> navigationStack_;

	friend class BinaryTree<T, T_Traits>;
};


template <typename T, typename T_Traits> 
inline BinaryTree<T,T_Traits>::BinaryTree() : root_(nullptr) 
{
}

template <typename T, typename T_Traits> 
inline BinaryTree<T,T_Traits>::~BinaryTree()
{
	std::for_each( allocations_.begin(), allocations_.end(), [](Node* node){ delete node; } ); 
	root_ = nullptr;
}

template <typename T, typename T_Traits> 
inline std::unique_ptr< IteratorBase<T, T_Traits> > BinaryTree<T,T_Traits>::CreateIterator() const
{
	return std::unique_ptr< IteratorBase<T, T_Traits> >(new BinaryTreeIterator<T, T_Traits>(*this));
}


template <typename T, typename T_Traits> 
inline void  BinaryTree<T,T_Traits>::AddKey(T const& val)
{
	Node* parentNode = FindNode(val);

	if (parentNode == nullptr)
	{
		// There is no root_ element
		assert(root_ == nullptr);
		root_ = NewNode(val);
	}
	else
	{
		if (T_Traits::less(val, parentNode->value_))
		{
			assert (parentNode->left_ == nullptr);
			parentNode->left_ = NewNode(val);
		}
		else
		{
			assert (parentNode->right_ == nullptr);
			parentNode->right_ = NewNode(val);
		}
	}
}

template <typename T, typename T_Traits> 
inline typename BinaryTree<T,T_Traits>::Node*  BinaryTree<T,T_Traits>::FindNode(T const& val) const
{
	if (root_ == nullptr)
	{
		return nullptr;
	}
	else
	{
		/// It's guaranteed that ptr != null here
		Node* ptr = root_;

		for(;;)
		{
			if (T_Traits::less(val, ptr->value_))
			{
				if (ptr->left_)
				{
					ptr = ptr->left_;
					continue;
				}
				else
				{
					break;
				}
			}
			else
			{
				if (ptr->right_)
				{
					ptr = ptr->right_;
					continue;
				}
				else
				{
					break;
				}
			}
		}

		return ptr;
	}
}