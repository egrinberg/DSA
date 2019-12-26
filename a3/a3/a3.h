#include <string>
using namespace std;

#define ALPHABET_SIZE 26

class SpellCheck{

	struct TrieNode
	{	
		//TrieNode pointers represent letters in the alphabet
		TrieNode* children_[ALPHABET_SIZE];  
		// isTerminal_ is true if the node
		// represents end of a word
		bool isTerminal_;

		TrieNode()
		{
			isTerminal_ = false;
			for (int i = 0; i < ALPHABET_SIZE; i++)
			{
				children_[i] = nullptr;
			}
		}
		//destructor for TrieNode - deallocate memory of all the pointers that are not null in the TrieNode
		~TrieNode()
		{
			for (int i = 0; i < ALPHABET_SIZE; i++)
			{
				if (children_[i] != nullptr)
				{
					delete children_[i];
				}
					
			}
		}
	};

	TrieNode* root_;
		
public:
	SpellCheck(string data[],int sz);
	void addWord(const string& newWord);
	bool lookup(const string& word) const;
	int suggest(const string& partialWord, string suggestions[]) const;
	void suggestWord(TrieNode* curr, string suggestions[], int& numWord,string& suggestedWord) const;

	~SpellCheck();	
};