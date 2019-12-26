#include "mylist.h"
#include <string>
#include <utility>
#include <functional>

using namespace std;

template <class TYPE>
class Table{
public:
	Table(){}
	virtual bool update(const string& key, const TYPE& value)=0;
	virtual bool remove(const string& key)=0;
	virtual bool find(const string& key, TYPE& value)=0;
	virtual ~Table(){}
};

template <class TYPE>
class SimpleTable:public Table<TYPE>{

	struct Record{
		TYPE data_;
		string key_;
		Record(const string& key, const TYPE& data){
			key_=key;
			data_=data;
		}
		
	};

	Record** records_;   //the table
	int max_;           //capacity of the array
	int size_;          //current number of records held
	int search(const string& key);
	void sort();

public:
	SimpleTable(int maxExpected);
	SimpleTable(const SimpleTable& other);
	SimpleTable(SimpleTable&& other);
	virtual bool update(const string& key, const TYPE& value);
	virtual bool remove(const string& key);
	virtual bool find(const string& key, TYPE& value);
	virtual const SimpleTable& operator=(const SimpleTable& other);
	virtual const SimpleTable& operator=(SimpleTable&& other);
	virtual ~SimpleTable();
};


//returns index of where key is found.
template <class TYPE>
int SimpleTable<TYPE>::search(const string& key){
	int rc=-1;
	for(int i=0;i<size_;i++){
		if(records_[i]->key_==key){
			rc=i;			
		}
	}
	return rc;
}
//sort the according to key in table
template <class TYPE>
void SimpleTable<TYPE>::sort(){
   int minIdx=0;
   for(int i=0;i<size_;i++){
   		minIdx=i;
     	for(int j=i+1;j<size_;j++){
     		if(records_[j]->key_ < records_[minIdx]->key_){
     			minIdx=j;
     		}
    	}
    	Record* tmp=records_[i];
    	records_[i]=records_[minIdx];
    	records_[minIdx]=tmp;
   }
}

/* none of the code in the function definitions below are implemented with
   the most efficient algorithm in mind.  Do not assume anything is written
   a certain way.
*/
template <class TYPE>
SimpleTable<TYPE>::SimpleTable(int maxExpected): Table<TYPE>(){
	records_=new Record*[maxExpected];
	max_=maxExpected;
	size_=0;
}

//copy constructor
template <class TYPE>
SimpleTable<TYPE>::SimpleTable(const SimpleTable<TYPE>& other){
	records_=new Record*[other.max_];
	max_=other.max_;
	size_=0;
	for(int i=0;i<other.size_;i++){
		update(other.records_[i]->key_,other.records_[i]->data_);
	}
}
//move constructor
template <class TYPE>
SimpleTable<TYPE>::SimpleTable(SimpleTable<TYPE>&& other){
	size_=other.size_;
	max_=other.max_;
	records_=other.records_;
	other.records_=nullptr;
	other.size_=0;
	other.max_=0;
}

template <class TYPE>
bool SimpleTable<TYPE>::update(const string& key, const TYPE& value){
	int idx=search(key);
	bool rc=false;
	if(idx==-1){
		if(size_ < max_){
			records_[size_++]=new Record(key,value);
			sort();
			rc=true;
		}
	}
	else{
		rc=true;
		records_[idx]->data_=value;
	}
	return rc;
}

template <class TYPE>
bool SimpleTable<TYPE>::remove(const string& key){
	int idx=search(key);
	if(idx!=-1){
		delete records_[idx];
		for(int i=idx;i<size_-1;i++){
			records_[i]=records_[i+1];
		}
		size_--;
		return true;
	}
	else{
		return false;
	}
}

template <class TYPE>
bool SimpleTable<TYPE>::find(const string& key, TYPE& value){
	int idx=search(key);
	if(idx==-1)
		return false;
	else{
		value=records_[idx]->data_;
		return true;
	}
}
//assignment operator
template <class TYPE>
const SimpleTable<TYPE>& SimpleTable<TYPE>::operator=(const SimpleTable<TYPE>& other){
	if(this!=&other){
		if(records_){
			int sz=size_;
			for(int i=0;i<size_;i++){
				delete records_[0];
				for(int j=0;j<sz-1;j++){
					records_[j]=records_[j+1];
				}
				sz--;
			}
			delete [] records_;
		}
		records_=new Record*[other.max_];
		max_=other.max_;
		size_=0;
		for(int i=0;i<other.size_;i++){
			update(other.records_[i]->key_,other.records_[i]->data_);
		}

	}
	return *this;
}
//move assignment
template <class TYPE>
const SimpleTable<TYPE>& SimpleTable<TYPE>::operator=(SimpleTable<TYPE>&& other){
	swap(records_,other.records_);
	swap(size_,other.size_);
	swap(max_,other.max_);
	return *this;
}
template <class TYPE>
SimpleTable<TYPE>::~SimpleTable(){
	if(records_){
		int sz=size_;
		for(int i=0;i<size_;i++){
			delete records_[0];
			for(int j=0;j<sz-1;j++){
				records_[j]=records_[j+1];
			}
			sz--;
		}
		delete [] records_;
	}
}

template <class TYPE>
class ChainingTable:public Table<TYPE>{

	struct Record
	{
		TYPE data_;
		string key_;

		//empty constructor
		Record()
		{
			data_ = TYPE {};
			key_ = "";
		}
		//constructor that receives key and value
		Record(const string& key, const TYPE& data)
		{
			key_ = key;
			data_ = data;
		}
		//copy constructor
		Record(const Record& other)
		{
			key_ = other.key_;
			data_ = other.data_;
		}
		//copy assignment operator
		const Record& operator=(const Record& other)
		{
			if (this != &other)
			{
				key_ = other.key_;
				data_ = other.data_;
			}
			return *this;
		}
		//operator < overload , compare two keys
		//returns true if *this key is shorter than rhs key or,first character that doesn't match is smaller than rhs key.
		bool operator <(const Record& b) const {
			bool rc = false;
			if ((this->key_).compare(b.key_) < 0) {
				rc = true;
			}
			return rc;
		}
		//operator == overload , compare two keys
		//returns true both keys are equal
		bool operator==(const Record& b) const {
			bool rc = false;
			if ((this->key_).compare(b.key_) == 0) {
				rc = true;
			}
			return rc;
		}

				
	};
			  
	SortedList<Record>** records_; //the table	
	int max_;           //capacity of the array
	int size_;          //current number of records held
	std::hash<std::string> hashFunction; //declare an hash function object. hashFunction is a variable but it is also a function
		
	
public:
	
	ChainingTable(int maxExpected);
	ChainingTable(const ChainingTable& other);
	ChainingTable(ChainingTable&& other);
	virtual bool update(const string& key, const TYPE& value);
	virtual bool remove(const string& key);
	virtual bool find(const string& key, TYPE& value);
	virtual const ChainingTable& operator=(const ChainingTable& other);
	virtual const ChainingTable& operator=(ChainingTable&& other);
	virtual ~ChainingTable();	
		
};
//This is constructor, it creates empty ChainingTable
template <class TYPE>
ChainingTable<TYPE>::ChainingTable(int maxExpected): Table<TYPE>()
{
	records_ = new SortedList<Record>*[maxExpected];
	max_ = maxExpected;
	size_ = 0;

	for (int i = 0; i < maxExpected; i++)
	{
		records_[i] = nullptr;
	}
		
}

//This is copy constructor, it copies all the elements of SortedList from the rhs ChainingTable
template <class TYPE>
ChainingTable<TYPE>::ChainingTable(const ChainingTable<TYPE>& other)
{
	if (other.size_ != 0)
	{
		records_ = new SortedList<Record>*[other.max_];
		max_ = other.max_;
		size_ = other.size_;

		for (int i = 0; i < other.max_; i++)
		{
			if (other.records_[i] == nullptr)
			{
				records_[i] = nullptr;
			}
			else
			{
				records_[i] = new SortedList<Record>();

				for (typename SortedList<ChainingTable<TYPE>::Record>::iterator it = other.records_[i]->begin(); it != other.records_[i]->end(); it++)
				{
					records_[i]->insert(*it); 
				}
			}
		}
	}
}

//This is move constructor, it "steals" all the table elements from rhs ChainingTable
template <class TYPE>
ChainingTable<TYPE>::ChainingTable(ChainingTable<TYPE>&& other)
{
	if (other.size_ != 0)
	{
		max_ = other.max_;
		size_ = other.size_;
		other.max_ = 0;
		other.size_ = 0;
		records_ = other.records_;

		delete[] other.records_;
		other.records_ = nullptr;		
	}

}
 
//this function looks for the passed key inside the table.
//if the key is found in the table the record's value is replaced by the value passed to this function.
//if the table doesnt have the key it adds new record with passed key and value to the table as long as it is possible to do so.
//function returns true If the record was successfully added or updated otherwise returns false.
template <class TYPE>
bool ChainingTable<TYPE>::update(const string& key, const TYPE& value)
{
	size_t hash = hashFunction(key);
	size_t idx = hash % max_; // indicates the index of the array in the table where the key is stored
	bool rc = false; // indicate if the record was successfully added or updated

	
	Record* tmp = new Record(key, value); // temporary Record pointer to new Record

	if (records_[idx] != nullptr)
	{
		
		bool found = false; // indicate if key is found in the Sorted list

		//loop through the SortedList , if the key is found it updates the value
		for (typename SortedList<ChainingTable<TYPE>::Record>::iterator it = records_[idx]->begin(); it != records_[idx]->end(); it++)
		{
			if (key == (*it).key_)
			{
				(*it).data_ = value;
				found = true;
				rc = true;
				break;
			}
		}
		//if the key is not found in the SortedList and the size is less than max expected
		//than it insert a new Record to the Sorted List
		if (!found && size_ < max_)
		{
			records_[idx]->insert(*tmp);
			size_++;
			rc = true;
		}
	}
	else
	{	
		//if SortedList is not exist in the given index
		//check the size is less than max expected,create new SortedList and insert a new Record to the SortedList
		if (size_ < max_)
		{
			records_[idx] = new SortedList<Record>();
			records_[idx]->insert(*tmp);
			size_++;
			rc = true;
		}
	}	

	//dealocate the temporary pointer
	delete tmp;

	return rc;
}

//this function looks for the passed key inside the table.
//If table contains a record with a matching key, the record (both the key and the value) is removed from the table
//function returns true If the record was successfully removed otherwise returns false.
template <class TYPE>
bool ChainingTable<TYPE>::remove(const string& key)
{
	size_t hash = hashFunction(key);
	size_t idx = hash % max_;
	
	if (records_[idx] != nullptr)
	{
		//looks for the key inside the SortedList
		for (typename SortedList<ChainingTable<TYPE>::Record>::iterator it = records_[idx]->begin(); it != records_[idx]->end(); it++)
		{
			if (key == (*it).key_)
			{
				records_[idx]->erase(it);
				size_--;
				return true;
			}
		}
	}
	
	return false;
}

//This function accepts a key and a reference for passing back a found value.
//the function returns true if table contains a record with a matching key,and passes back the value from the record.
//function returns false if it does not find a record with a matching key.
template <class TYPE>
bool ChainingTable<TYPE>::find(const string& key, TYPE& value)
{
	size_t hash = hashFunction(key);
	size_t idx = hash % max_;
		
	if (records_[idx] != nullptr)
	{
		for (typename SortedList<ChainingTable<TYPE>::Record>::iterator it = records_[idx]->begin(); it != records_[idx]->end(); it++)
		{
			if (key == (*it).key_)
			{
				value = (*it).data_;
				return true;
			}
		}
	}	
		
	return false;	
}

//This is assignment operator, it deallocates all the elements of the lhs ChainingTable and copies all the elements from the rhs ChainingTable 
template <class TYPE>
const ChainingTable<TYPE>& ChainingTable<TYPE>::operator=(const ChainingTable<TYPE>& other)
{
	if (this != &other && other.size_ != 0)
	{
		if (size_ == 0)
		{
			records_ = new SortedList<Record>*[other.max_];
			
		}
		else
		{
			
			for (int i = 0; i < max_; i++)
			{
				delete records_[i];
			}
		}

		max_ = other.max_;
		size_ = other.size_;

		for (int i = 0; i < other.max_; i++)
		{
			if (other.records_[i] != nullptr)
			{			
				records_[i] = new SortedList<Record>();
				for (typename SortedList<ChainingTable<TYPE>::Record>::iterator it = other.records_[i]->begin(); it != other.records_[i]->end(); it++)
				{
					records_[i]->insert(*it);
				}
			}
			else
			{
				records_[i] = nullptr;
			}
		}
	}

	return *this;
}

//This is move assignment operator, it "steals" all the table elements from rhs ChainingTable  
template <class TYPE>
const ChainingTable<TYPE>& ChainingTable<TYPE>::operator=(ChainingTable<TYPE>&& other)
{
	if (this != &other && other.size_ != 0)
	{

		swap(records_, other.records_);
		swap(size_, other.size_);
		swap(max_, other.max_);		
	}

	return *this;
}

//This is destructor it deallocated all the elements that contain Records from the ChainingTable
template <class TYPE>
ChainingTable<TYPE>::~ChainingTable()
{
	for (int i = 0; i < max_; i++)
	{
		if (records_[i] != nullptr)
		{
			delete records_[i];
			size_--;
		}		
	}

	delete [] records_;
}

