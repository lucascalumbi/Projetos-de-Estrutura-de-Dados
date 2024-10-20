#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

struct Bet
{
    string code;
    int* numbers;
    int matchedNumbersAmount;

    Bet(string code, int* numbers) : code(code), numbers(numbers), matchedNumbersAmount(-1) {}

    void CalculateMatchedNumber(int* drawNumbers)
    {   
        matchedNumbersAmount = 0;
        for(int i = 0; i < 10; i++)
            for(int j = 0; j < 15; j++)
            {
                if(drawNumbers[i] == numbers[j])
                    matchedNumbersAmount++;
            }
    }

    bool operator > (const Bet& other) const
    {
        return this->matchedNumbersAmount > other.matchedNumbersAmount;
    } 

    bool operator == (const Bet& other) const
    {
        return this->matchedNumbersAmount == other.matchedNumbersAmount;
    }

    bool operator < (const Bet& other) const
    {
        return this->matchedNumbersAmount < other.matchedNumbersAmount;
    } 
};

struct HeapMinTree
{
    
    int capacity;
    Bet** array;
    int length;

    HeapMinTree(int capacity) : capacity(capacity), array(new Bet*[capacity]), length(0) {}
    HeapMinTree(Bet** array, int capacity, int length)
    {   
        this->capacity = capacity;
        this->array = new Bet*[capacity];
        this->length = length;
        
        for(int i = 0; i < length; i++)
            this->array[i] = array[i]; 
    }

    void Add(Bet* bet)
    {
        array[length] = bet;
        length++;
    }

    void AddWithHeapify(Bet* bet)
    {
        array[length] = bet;
        length++;
        
        Heapify(length-1);
    }

    Bet* RemoveWithHeapify()
    {   
        if(!length)
        {
            cout << "estrutura vazia" << endl;
            return nullptr;
        }

        Bet* aux = array[0];
        array[0] = array[length-1];
        Heapify(0);
        length--;
        return aux;
    }

    int GetParentIndex(int index)
    {
        return (index-1)/2;
    }

    int GetLeftChildIndex(int index)
    {
        return 2*index+1;
    }

    int GetRightChildIndex(int index)
    {
        return 2*index+2;
    }

    void Swap(int i, int j)
    {
        Bet* aux = array[i];
        array[i] = array[j];
        array[j] = aux;
    }

    void BuildHeapTree()
    {   
        int lastFatherNodeIndex = GetParentIndex(length-1);
        for(int i = lastFatherNodeIndex; i >= 0; i--)
        {
            Heapify(i);
            //PrintAll();
        }
    }

    void Heapify(int index)
    {
        int parent = index;
        int rightChild = GetRightChildIndex(index);
        int leftChild = GetLeftChildIndex(index);               

        
        if(leftChild < length && *(array[leftChild]) < *(array[parent]))
            parent = leftChild;
        if(rightChild < length && *(array[rightChild]) < *(array[parent]))
            parent = rightChild;

        if(parent != index)
        {
            Swap(parent, index);
            Heapify(parent);
        }
    }

    void CalculateMatchedNumber(int* drawNumbers)
    {
        for(int i = 0; i < length; i++)
            array[i]->CalculateMatchedNumber(drawNumbers);
    }

    void PrintAll()
    {
        for(int i = 0; i < length; i++)
        {
            cout << array[i]->code << ", " << array[i]->matchedNumbersAmount << endl;
            for(int j = 0; j < 10; j++)
            {
                cout << array[i]->numbers[j] << ", ";
            }
            cout << endl;
        }
        cout << endl;

    }

    void WriteResult(ofstream* output, int moneyAmount)
    {   
        int minMatchedNumbersAmount = array[0]->matchedNumbersAmount;
        Bet* aux = nullptr;
        int lowerMatchersAmount = 0;
        Bet** lowerMatchers = new Bet*[capacity];
        
        int i = 0; 
        for(; i < capacity; i++)
        {   
            aux = RemoveWithHeapify();
            if(aux->matchedNumbersAmount == minMatchedNumbersAmount)
            {
                lowerMatchers[lowerMatchersAmount] = aux;
                lowerMatchersAmount++;
            }
            else
                break;
        }

        *output << "[" << lowerMatchersAmount << ":" << minMatchedNumbersAmount << ":" << ((moneyAmount/2)/lowerMatchersAmount) << "]" << endl;
        for(int i = 0; i < lowerMatchersAmount; i++)
        {
            *output << lowerMatchers[i]->code << endl;
        }
    }
};


struct HeapMaxTree
{
    int capacity;
    Bet** array;
    int length;

    HeapMaxTree(int capacity) : capacity(capacity), array(new Bet*[capacity]), length(0) {}
    HeapMaxTree(Bet** array, int capacity, int length)
    {   
        this->capacity = capacity;
        this->array = new Bet*[capacity];
        this->length = length;
        
        for(int i = 0; i < length; i++)
            this->array[i] = array[i]; 
    } 

    void Add(Bet* bet)
    {
        array[length] = bet;
        length++;
    }

    void AddWithHeapify(Bet* bet)
    {
        array[length] = bet;
        length++;
        
        Heapify(length-1);
    }

    Bet* RemoveWithHeapify()
    {   
        if(!length)
        {
            cout << "estrutura vazia" << endl;
            return nullptr;
        }

        Bet* aux = array[0];
        array[0] = array[length-1];
        Heapify(0);
        length--;
        return aux;
    }

    int GetParentIndex(int index)
    {
        return (index-1)/2;
    }

    int GetLeftChildIndex(int index)
    {
        return 2*index+1;
    }

    int GetRightChildIndex(int index)
    {
        return 2*index+2;
    }

    void Swap(int i, int j)
    {
        Bet* aux = array[i];
        array[i] = array[j];
        array[j] = aux;
    }

    void BuildHeapTree()
    {   
        int lastFatherNodeIndex = GetParentIndex(length-1);
        for(int i = lastFatherNodeIndex; i >= 0; i--)
        {
            Heapify(i);
            //PrintAll();
        }
    }

    void Heapify(int index)
    {
        int parent = index;
        int rightChild = GetRightChildIndex(index);
        int leftChild = GetLeftChildIndex(index);               

        
        if(leftChild < length && *(array[leftChild]) > *(array[parent]))
            parent = leftChild;
        if(rightChild < length && *(array[rightChild]) > *(array[parent]))
            parent = rightChild;

        if(parent != index)
        {
            Swap(parent, index);
            Heapify(parent);
        }
    }

    void CalculateMatchedNumber(int* drawNumbers)
    {
        for(int i = 0; i < length; i++)
            array[i]->CalculateMatchedNumber(drawNumbers);
    }

    void PrintAll()
    {
        for(int i = 0; i < length; i++)
        {
            cout << array[i]->code << ", " << array[i]->matchedNumbersAmount << endl;
            for(int j = 0; j < 10; j++)
            {
                cout << array[i]->numbers[j] << ", ";
            }
            cout << endl;
        }
        cout << endl;

    }

    void WriteResult(ofstream* output, int moneyAmount)
    {   
        int maxMatchedNumbersAmount = array[0]->matchedNumbersAmount;
        Bet* aux = nullptr;
        int higherMatchersAmount = 0;
        Bet** higherMatchers = new Bet*[capacity];
        
        for(int i = 0; i < capacity; i++)
        {   
            aux = RemoveWithHeapify();
            if(aux->matchedNumbersAmount == maxMatchedNumbersAmount)
            {
                higherMatchers[higherMatchersAmount] = aux;
                higherMatchersAmount++;
            }
            else
                break;
        }
        
        *output << "[" << higherMatchersAmount << ":" << maxMatchedNumbersAmount << ":" << ((moneyAmount/2)/higherMatchersAmount) << "]" << endl;
        for(int i = 0; i < higherMatchersAmount; i++)
        {
            *output << higherMatchers[i]->code << endl;
        }
    }
};


int main(int argc, char* argv[])
{
    ifstream input(argv[1]);
    ofstream output(argv[2]);              

	//ifstream input("entrada.txt");
	//ofstream output("saida.txt");

    string line;
    getline(input,line);
    int moneyAmount = stoi(line);

    getline(input,line);
    int capacity = stoi(line);

    istringstream reader;
    int* drawNumbers = new int[10];
    getline(input,line);
    reader.str(line);
    for(int j = 0; j < 15; j++)
        reader >> drawNumbers[j];
    reader.clear();

    HeapMaxTree* maxTree = new HeapMaxTree(capacity);


    for(int i = 0; i < capacity; i++)
    {   
        getline(input,line);
        string code;
        int* numbers = new int[15];
        reader.str(line);
        reader >> code;
        for(int j = 0; j < 15; j++)
            reader >> numbers[j];
        Bet* aux = new Bet(code,numbers);
        maxTree->Add(aux);
        reader.clear();
        reader.str("");
    }

    maxTree->CalculateMatchedNumber(drawNumbers);

    HeapMinTree* minTree = new HeapMinTree(maxTree->array, maxTree->capacity, maxTree->length);

    maxTree->BuildHeapTree(); 
    minTree->BuildHeapTree();  
    maxTree->WriteResult(&output, moneyAmount);
    minTree->WriteResult(&output, moneyAmount);

    return EXIT_SUCCESS;
}












