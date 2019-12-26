#include "a3.h"
#include <string>

//This is constructor, it receives an array of strings representing the valid words and an integer representing the size of the array.
//It creates a SpellCheck object with the words that are passed.
SpellCheck::SpellCheck(string data[],int sz)
{
	root_ = nullptr;

	if (sz > 0)
	{
		root_ = new TrieNode();
	}	

	//loop through all the words in the array to add them to SpellCheck 
	for (int i = 0; i < sz; i++)
	{
		TrieNode* curr = root_;

		//loop through each letter in the word and create TrieNode
		for (int j = 0; j < data[i].length(); j++)
		{
			
			int letter = (int)data[i][j] - (int)'a'; // provides index of the letter
			if (curr->children_[letter] == nullptr)
			{
				curr->children_[letter] = new TrieNode();
			}
			curr = curr->children_[letter];
		}

		curr->isTerminal_ = true; // end of the word
		
	}

}

//this function adds new word to the SpellCheck.
//It receives a newword that is purely alphabetic and lower case 
void SpellCheck::addWord(const string& newWord)
{
	TrieNode* curr = root_;
	
	//loop through each letter in the word and create TrieNode if its not exist already
	for (int i = 0; i < newWord.length(); i++)
	{

		int letter = (int)newWord[i] - (int)'a'; // provides index of the letter

		if (curr->children_[letter] == nullptr)
		{
			curr->children_[letter] = new TrieNode();
		}
		curr = curr->children_[letter];
	}

	curr->isTerminal_ = true; // end of the word

}
//this function looks up the word in the SpellCheck object.
//It returns true if the word is in the SpellCheck object, otherwise returns false
bool SpellCheck::lookup(const string& word) const
{
	if (root_ == nullptr)
	{
		return false;
	}

	TrieNode* curr = root_;

	//loop through each letter in the word and check if the letter is exist in the SpellCheck Object
	for (int i = 0; i < word.length(); i++)
	{
		int letter = (int)word[i] - (int)'a'; // provides index of the letter

		curr = curr->children_[letter];

		if (curr == nullptr)
		{
			return false;
		}
		
	}

	return curr->isTerminal_;
}

//This function receives a string and finds all words that begin with the string partialWord within the SpellCheck object and store them into suggestions.
//Function returns number of words stored within suggestions.
int SpellCheck::suggest(const string& partialWord, string suggestions[]) const
{
	TrieNode* curr = root_; // pointer to the root_ of the tree
	int numWords = 0; // represent the total number of words found
	string initialWord;

	//find the last letter TrieNode of the partial word
	for (int i = 0; i < partialWord.length(); i++)
	{
		int letter = (int)partialWord[i] - (int)'a'; // provides index of the letter
		
		curr = curr->children_[letter];						
	}
	//check if the partialWord is an actual word
	if (curr->isTerminal_)
	{
		suggestions[0] = partialWord;
		numWords++;
	}

	initialWord = partialWord;
	
	//look for suggestion words
	suggestWord(curr,suggestions,numWords,initialWord);
		
	return numWords;
}

//This function receives a reference to TrieNode, 
//and finds all words recursively that begin with the string partialWord within the SpellCheck object and store them into suggestions.
void SpellCheck::suggestWord(TrieNode* start, string suggestions[], int& numWord,string& suggestedWord) const
{
	//loop through all pointers inside TrieNode 
	//check if letter exist and looks for terminator
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		TrieNode* curr = start;

		if (curr->children_[i] != nullptr)
		{
			curr = curr->children_[i];
			suggestedWord += i + 'a';
			
			if (curr->isTerminal_)
			{	
				suggestions[numWord] = suggestedWord;
				numWord++;				
			}

			suggestWord(curr,suggestions,numWord, suggestedWord);
		}
	}
	
	suggestedWord.pop_back(); //removes last letter from suggested word if next letter is not found.
}

//This is destructor it removes all the TrieNodes that contains data 
SpellCheck::~SpellCheck() 
{
	if (root_)
	{
		delete root_;
	}

}