template <typename T>

class SortedList
{
	struct Node
	{
		T data_;
		Node* next_;
		Node* prev_;

		Node(const T& data = {}, Node* nx = nullptr, Node* pr = nullptr)
		{
			data_ = data;
			next_ = nx;
			prev_ = pr;
		}
	};

	Node* front_;
	Node* back_;
	int size_;

public:
	class const_iterator
	{
		friend class SortedList; // frienship gives access to prive data members
		Node* curr_;// node that is currently we need
		const SortedList* myList_;

		//This is overload constructor for const iterator class
		//It accepts the Node to point to and the list to go through 
		const_iterator(Node* curr, const SortedList* theList)
		{
			curr_ = curr;
			myList_ = theList;
		}

	public:
		//default constructor
		const_iterator()
		{
			curr_ = nullptr;
			myList_ = nullptr;
		}
		//prefix ++ operator overload
		//makes iterator point to the next node in the list.
		//return const iterator point to the next node
		const_iterator operator++()
		{
			curr_ = curr_->next_;
			return *this;
		}
		//postfix ++ operator overload
		//makes iterator point to the next node in the list.
		//return const iterator point to current node
		const_iterator operator++(int)
		{
			const_iterator old = *this;
			curr_ = curr_->next_;
			return old;
		}
		//prefix -- operator overload
		//increments iterator so that it points to previous node in list
		//return const iterator point to the previous node in list
		const_iterator operator--()
		{
			curr_ = curr_->prev_;

			return *this;
		}
		//postfix -- operator overload
		//increments iterator so that it points to previous node in list
		//return const iterator point to the current node
		const_iterator operator--(int)
		{
			const_iterator old = *this;

			curr_ = curr_->prev_;

			return old;
		}
		//operator == overload
		//returns true if two iterators point at the same node, false otherwise
		bool operator==(const_iterator rhs) const
		{
			bool rc = false;

			if (curr_ == rhs.curr_)
			{
				rc = true;
			}

			return rc;
		}
		//operator != overload
		//returns true if two iterators point at different nodes, false otherwise
		bool operator!=(const_iterator rhs) const
		{
			return !(*this == rhs);
		}
		//dereferencing operator
		//returns the data in the node pointed to by iterator
		const T& operator*()const { return curr_->data_; }
	};
	class iterator :public const_iterator
	{
		friend class SortedList;

		//This is overload constructor for iterator class
		//It accepts the Node to point to and the list to go through 
		iterator(Node* curr, SortedList* theList) :const_iterator(curr, theList) {}
	public:
		//default constructor
		iterator() :const_iterator() {}

		//prefix ++ operator overload
		//makes iterator point to the next node in the list.
		//return iterator point to the next node
		iterator operator++()
		{
			this->curr_ = this->curr_->next_;
			return *this;
		}
		//postfix ++ operator overload
		//makes iterator point to the next node in the list.
		//return iterator point to current node
		iterator operator++(int)
		{
			iterator old = *this;
			this->curr_ = this->curr_->next_;
			return old;
		}
		//prefix -- operator overload
		//increments iterator so that it points to previous node in list
		//return iterator point to the previous node in list
		iterator operator--()
		{
			this->curr_ = this->curr_->prev_;
			return *this;
		}
		//postfix -- operator overload
		//increments iterator so that it points to previous node in list
		//return iterator point to the current node
		iterator operator--(int)
		{
			iterator old = *this;
			this->curr_ = this->curr_->prev_;
			return old;
		}
		//dereferencing operator
		//returns the data in the node pointed to by iterator
		T& operator*() { return this->curr_->data_; }

		//dereferencing operator
		//returns the data in the node pointed to by iterator
		const T& operator*()const { return this->curr_->data_; }
	};
	SortedList();
	~SortedList();
	SortedList(const SortedList& rhs);
	SortedList& operator=(const SortedList& rhs);
	SortedList(SortedList&& rhs);
	SortedList& operator=(SortedList&& rhs);
	iterator begin() { return iterator(front_->next_, this); } //returns iterator to Node containing the first piece of data in the linked list
	iterator end() { return iterator(back_, this); }//returns iterator to the Node after the node containing the last piece of data of the linked list
	const_iterator cbegin() const { return const_iterator(front_->next_, this); } //returns const iterator to Node containing the first piece of data in the linked list
	const_iterator cend() const { return const_iterator(back_, this); }//returns const iterator to the Node after the node containing the last piece of data of the linked list
	iterator insert(const T& data);
	iterator search(const T& data);
	const_iterator search(const T& data) const;
	iterator erase(iterator it);
	iterator erase(iterator first, iterator last);
	bool empty() const;
	int size() const;
};

//This is constructor, it creates empty SortedList
template <typename T>
SortedList<T>::SortedList()
{
	front_ = new Node();
	back_ = new Node();
	front_->next_ = back_;
	back_->prev_ = front_;
	size_ = 0;

}
//This is destructor it removes all the nodes that contain data from SortedList
template <typename T>
SortedList<T>::~SortedList()
{
	Node* curr = front_;
	while (curr)
	{
		Node* rm = curr;
		curr = curr->next_;
		delete rm;
	}

}
//This is copy constructor, it copies all the nodes from the rhs SortedList 
template <typename T>
SortedList<T>::SortedList(const SortedList& rhs)
{
	*this = rhs;

}
//This is assignment operator, it removes all the nodes from lhs SortedList and copies all the nodes from the rhs SortedList 
template <typename T>
SortedList<T>& SortedList<T>::operator=(const SortedList& rhs)
{
	if (this != &rhs && rhs.size_ != 0)
	{
		const_iterator it;

		if (size_ == 0)
		{
			front_ = new Node();
			back_ = new Node();
		}

		else
		{
			it = cbegin();

			//removes all the nodes from current list
			while (it != cend())
			{
				Node* rm = it.curr_;
				it++;
				delete rm;

			}
			size_ = 0;
		}
		//sets the front empty node to point to the back empty node and vise versa		
		front_->next_ = back_;
		back_->prev_ = front_;

		//inserts nodes from rhs SortedList to the current list
		for (it = rhs.cbegin(); it != rhs.cend(); it++)
		{
			insert(*it);
		}

	}

	return *this;

}
//This is move constructor, it "steals" all the nodes from the rhs SortedList into the current list 
template <typename T>
SortedList<T>::SortedList(SortedList&& rhs)
{
	if (rhs.size_ != 0)
	{
		size_ = rhs.size_;
		rhs.size_ = 0;
		front_ = rhs.front_;
		back_ = rhs.back_;
		rhs.front_ = nullptr;
		rhs.back_ = nullptr;
	}
}
//This is move assignment operator, it "steals" all the nodes from the rhs SortedList into the current list  
template <typename T>
SortedList<T>& SortedList<T>::operator=(SortedList&& rhs)
{

	if (this != &rhs && rhs.size_ != 0)
	{

		if (size_ == 0)
		{
			front_ = new Node();
			back_ = new Node();
		}

		size_ = rhs.size_;
		rhs.size_ = 0;
		front_ = rhs.front_;
		back_ = rhs.back_;
		rhs.front_ = nullptr;
		rhs.back_ = nullptr;
	}

	return *this;

}

//this function inserts a new node with data into the list.
//function returns iterator to the newly inserted node.  
template <typename T>
typename SortedList<T>::iterator SortedList<T>::insert(const T& data)
{
	iterator it;
	bool found_ = false; // indicates if there is data value in the list which is greater than new data

						 //if the list is empty it inserts the first node
	if (size_ == 0)
	{
		Node* nn = new Node(data, front_->next_, front_);
		front_->next_->prev_ = nn;
		front_->next_ = nn;
		size_++;
		it.curr_ = nn;
	}
	else
	{
		//searches for the node with data that is greater than the new data
		//insertes the new data node before the node with greater data
		for (it = begin(); it != end(); it++)
		{
			if (data < *it)
			{
				Node* nn = new Node(data, it.curr_, it.curr_->prev_);
				it.curr_->prev_->next_ = nn;
				it.curr_->prev_ = nn;
				size_++;
				it.curr_ = nn;
				found_ = true;
				break;
			}

		}
		//if the list doesnt have value greater than new data 
		//inserts the new data node to the back of the list
		if (found_ == false)
		{
			Node* nn = new Node(data, back_, back_->prev_);
			back_->prev_->next_ = nn;
			back_->prev_ = nn;
			size_++;
			it.curr_ = nn;
		}

	}

	return it;
}

//this function search for node with specified data in the list.
//function returns iterator to the node containing data.
//If data is not found, returns end() 
template <typename T>
typename SortedList<T>::iterator SortedList<T>::search(const T& data)
{
	for (iterator it = begin(); it != end(); it++)
	{
		if (data == *it)
		{
			return it;
		}
	}

	return end();
}

// this function search for node with specified data in the list.
//function returns const_iterator to the node containing data.
//If data is not found, returns cend() 
template <typename T>
typename SortedList<T>::const_iterator SortedList<T>::search(const T& data) const
{
	for (const_iterator it = cbegin(); it != cend(); it++)
	{
		if (data == *it)
		{
			return it;
		}
	}

	return cend();
}

//this function removes the node referred to by it
//function returns iterator to node after the node removed
template <typename T>
typename SortedList<T>::iterator SortedList<T>::erase(iterator it)
{

	if (it.curr_ != front_ && it.curr_ != back_)
	{
		Node* rm = it.curr_;
		rm->prev_->next_ = rm->next_;
		rm->next_->prev_ = rm->prev_;
		delete rm;
		it++;
		size_--;

	}

	return it;
}

//this function removes the nodes between the nodes referred to by iterator first to last.
//this includes the node referred to by first but NOT the node referred to by last.
//function returns iterator to last
template <typename T>
typename SortedList<T>::iterator SortedList<T>::erase(iterator first, iterator last)
{
	if (first.curr_ != front_)
	{
		first.curr_->prev_->next_ = last.curr_;
		last.curr_->prev_ = first.curr_->prev_;

		for (iterator it_ = first; it_ != last;)
		{
			Node* rm_ = it_.curr_;
			++it_;
			delete rm_;
			size_--;
		}
	}

	return last;

}

//function returns true if the list is empty, false otherwise
template <typename T>
bool SortedList<T>::empty() const
{
	return front_->next_ == back_;
}

//function returns number of pieces of data stored in the list
template <typename T>
int SortedList<T>::size() const
{
	return size_;

}
