#include <iostream>
#include <fstream> 
#include <sstream>

using namespace std;

template <typename T>
struct NodeAVL
{
    T data;
	int32_t height;
	int8_t balance;
    NodeAVL* left;
    NodeAVL* right;

	NodeAVL(T data) : data(data), height(0), balance(0) , left(nullptr), right(nullptr) {}
};

struct DictionaryWord
{
	string word;
	u_int32_t synonymousAmount;
	string* synonymous;

	DictionaryWord(string line) : DictionaryWord("",0,nullptr)
	{
		stringstream reader(line);
		string aux;
		reader >> word;
		reader >> synonymousAmount;

		synonymous = new string[synonymousAmount];
		
		for(u_int8_t i = 0; i < synonymousAmount; i++)
		{
			reader >> aux;
			synonymous[i] = aux;
		}
	}
	DictionaryWord(string word, u_int32_t synonymousAmount, string* synonymous)
		: word(word), synonymousAmount(synonymousAmount), synonymous(synonymous) {}
	void Print()
	{
		u_int8_t i = 0;
		for( ; i < synonymousAmount-1; i++)
		{
			cout << synonymous[i] << ",";
		}
		cout << synonymous[i] << endl;
	}
	void Write(ofstream* output)
	{
		u_int8_t i = 0;
		for( ; i < synonymousAmount-1; i++)
		{
			*output << synonymous[i] << ",";
		}
		*output << synonymous[i] << endl;
	}
	~DictionaryWord()
	{
        delete[] synonymous;  
    }
};

struct TreeAVLWord
{   
    NodeAVL<DictionaryWord*>* root;

    TreeAVLWord() : root(nullptr) {}

    void Push(DictionaryWord* data)
    {	
        PushAux(root,data);
    }
	NodeAVL<DictionaryWord*>* Search(string data)
	{
		return SearchAux(root, data);
	}
	void PushAllWords(ifstream* input, int32_t wordsAmount)
	{	
		string line;
		for(int32_t i = 0; i < wordsAmount; i++)
		{	
			getline(*input,line);
			DictionaryWord* word = new DictionaryWord(line);
			Push(word);
		}
	}
	void WriteAllWords(ifstream* input, ofstream* output,int32_t searchAmount)
	{	
		string line;
		for(int32_t i = 0; i < searchAmount; i++)
		{	
			getline(*input,line);
			*output << "[";
			NodeAVL<DictionaryWord*>* searchResult = SearchAndWrite(line,output);
			if(searchResult)
				searchResult->data->Write(output);
			else
				*output << "-" << endl;
		}
	}
	void PrintAllWords(ifstream* input, int32_t searchAmount)
	{	
		string line;
		for(int32_t i = 0; i < searchAmount; i++)
		{	
			getline(*input,line);
			cout << "[";
			NodeAVL<DictionaryWord*>* searchResult = SearchAndPrint(line);
			if(searchResult)
				searchResult->data->Print();
			else
				cout << "-" << endl;
		}
	}
	NodeAVL<DictionaryWord*>* SearchAndWrite(string data, ofstream* output)
	{
		return SearchAndWriteAux(root, data, output);
	}
	NodeAVL<DictionaryWord*>* SearchAndPrint(string data)
	{
		return SearchAndPrintAux(root, data);
	}
private:
	void PushAux(NodeAVL<DictionaryWord*>*& subTree,DictionaryWord* data)
    {	
        if(!subTree)
		{
            subTree = new NodeAVL<DictionaryWord*>(data);
			return;
		}
        else if(data->word == subTree->data->word)
		{	
			delete subTree->data;
			subTree->data = data;
		}
        else if(data->word > subTree->data->word)
			PushAux(subTree->right,data);
		else
			PushAux(subTree->left,data);

		UpdateHeight(subTree);
		UpdateBalance(subTree);
		Balance(subTree);
    }
	void Balance(NodeAVL<DictionaryWord*>*& subTree)
	{	
		if (subTree->balance > 1) 
		{ 
            if (subTree->right->balance < 0) 
                RotationRightLeft(subTree);
            else 
                RotationToLeft(subTree);
        } 
        else if (subTree->balance < -1) 
		{ 
            if (subTree->left->balance > 0) 
                RotationLeftRight(subTree);
            else 
                RotationToRight(subTree);
        }
	}
	int32_t max(int32_t a, int32_t b)
	{
		return (a > b) ? a : b;
	}
	NodeAVL<DictionaryWord*>* SearchAux(NodeAVL<DictionaryWord*>* subTree, string data)
	{
		if(!subTree)
			return nullptr;
		if(data == subTree->data->word)
		{
			return subTree;
		}
		else if(data > subTree->data->word)
		{
			return SearchAux(subTree->right, data);
		}
		else 
		{
			return SearchAux(subTree->left, data);
		}
	}
	void UpdateHeight(NodeAVL<DictionaryWord*>*& subTree)
	{
		int32_t leftHeight = GetHeight(subTree->left);
		int32_t rightHeight = GetHeight(subTree->right);

		subTree->height = 1 + max(leftHeight,rightHeight);
	}
	void UpdateBalance(NodeAVL<DictionaryWord*>*& subTree)
	{	
		if(!subTree)
			return;

		int32_t leftHeight = GetHeight(subTree->left);
		int32_t rightHeight = GetHeight(subTree->right);

		subTree->balance = rightHeight - leftHeight;
	}
	int32_t GetHeight(NodeAVL<DictionaryWord*>*& subTree)
	{
		return subTree ? subTree->height : -1;
	}
	void RotationToLeft(NodeAVL<DictionaryWord*>*& subTree)
	{
		NodeAVL<DictionaryWord*>* axis = subTree->right;
		subTree->right = axis->left;

		axis->left = subTree;
		subTree = axis;

    	UpdateHeight(subTree->left);
		UpdateHeight(subTree);
	}
	void RotationToRight(NodeAVL<DictionaryWord*>*& subTree)
	{
		NodeAVL<DictionaryWord*>* axis = subTree->left;
		subTree->left = axis->right;
		axis->right = subTree;
		subTree = axis;		

		UpdateHeight(subTree->right); 
		UpdateHeight(subTree);
	}
	void RotationLeftRight(NodeAVL<DictionaryWord*>*& subTree)
	{
		RotationToLeft(subTree->left);
		RotationToRight(subTree);
	}
	void RotationRightLeft(NodeAVL<DictionaryWord*>*& subTree)
	{
		RotationToRight(subTree->right);
		RotationToLeft(subTree);
	}
	NodeAVL<DictionaryWord*>* SearchAndWriteAux(NodeAVL<DictionaryWord*>* subTree,string data, ofstream* output)
	{
		if(!subTree)
		{
			*output << "?]" << endl; 
			return nullptr;
		}
		if(data == subTree->data->word)
		{	
			*output << data << "]" << endl;
			return subTree;
		}
		else if(data > subTree->data->word)
		{	
			*output << subTree->data->word << "->";
			return SearchAndWriteAux(subTree->right, data,output);
		}
		else 
		{	*output << subTree->data->word << "->";
			return SearchAndWriteAux(subTree->left, data,output);
		}
	}
	NodeAVL<DictionaryWord*>* SearchAndPrintAux(NodeAVL<DictionaryWord*>* subTree, string data)
	{
		if(!subTree)
		{
			cout << "?]" << endl; 
			return nullptr;
		}
		if(data == subTree->data->word)
		{	
			cout << data << "]" << endl;
			return subTree;
		}
		else if(data > subTree->data->word)
		{	
			cout << subTree->data->word << "->";
			return SearchAndPrintAux(subTree->right, data);
		}
		else 
		{	cout << subTree->data->word << "->";
			return SearchAndPrintAux(subTree->left, data);
		}
	}
};


int main(int argc, char* argv[])
{
    ifstream input(argv[1]);
    ofstream output(argv[2]);

	// ifstream input("entrada.txt");
	// ofstream output("saida.txt");

    string line;
    getline(input,line);
    int32_t wordsAmount = stoi(line);
    
	TreeAVLWord* dicTree = new TreeAVLWord();
	dicTree->PushAllWords(&input,wordsAmount);
    getline(input,line);
    int32_t searchAmount = stoi(line);
	//dicTree->PrintAllWords(input,searchAmount);
	dicTree->WriteAllWords(&input,&output,searchAmount);

    return EXIT_SUCCESS;
}
