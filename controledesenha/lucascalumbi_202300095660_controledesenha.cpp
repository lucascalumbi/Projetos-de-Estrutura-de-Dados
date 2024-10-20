#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

struct Person
{
    string name;
    int priority;
    int insertion_order;

    Person(string name, int age) : name(name), priority(age < 60 ? 1 : 2), insertion_order(-1) {}

    bool operator > (const Person& other) const
    {
        return (this->priority > other.priority) || (this->priority == other.priority && this->insertion_order < other.insertion_order);
    } 

    bool operator == (const Person& other) const
    {
        return (this->priority == other.priority && this->insertion_order == other.insertion_order);
    }

    bool operator < (const Person& other) const
    {
        return (this->priority < other.priority) || (this->priority == other.priority && this->insertion_order > other.insertion_order);
    } 
};

struct HeapMaxTree
{   
    string institution_name;
    int workers_amount;
    int capacity;
    Person** array;
    int length;
    int insertion_number;

    HeapMaxTree(string institution_name, int workers_amount)
     : institution_name(institution_name), workers_amount(workers_amount), capacity(100), array(new Person*[100]), length(0) {}

    void Add(Person* person)
    {   
        array[length] = person;
        person->insertion_order = insertion_number;
        length++;
        insertion_number++;
        if(length == capacity)
        {   
            int new_capacity = 2*capacity;
            Person** aux = new Person*[new_capacity];
            for(int i = 0; i < capacity; i++)
                aux[i] = array[i];
            array = aux;
            capacity = new_capacity;
        }
    }

    void AddWithHeapify(Person* person)
    {
        array[length] = person;
        length++;
        
        Heapify(length-1);
    }

    Person* RemoveWithHeapify()
    {   
        if(!length)
        {
            //cout << "estrutura vazia" << endl;
            return nullptr;
        }

        Person* aux = array[0];
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
        Person* aux = array[i];
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

    void WriteResult(ofstream* output)
    {       
        if(!length)
            return;

        Person* aux = RemoveWithHeapify();
        *output << institution_name << ":" << aux->name; 
        
        for(int i = 1; i < workers_amount; i++)
        {   
            aux = RemoveWithHeapify();
            if(!aux)
                break;
            *output << "," << aux->name;
        }
        *output << endl;


    }
    
};

struct HeapTreeQueue
{   
    HeapMaxTree** array;
    int capacity;
    int length;
    HeapTreeQueue() : array(new HeapMaxTree*[2]), capacity(2), length(0) {}

    void Add(HeapMaxTree* heap_Tree)
    {
        array[length] = heap_Tree;
        length++;
        if(length == capacity)
        {   
            int new_capacity = 2*capacity;
            HeapMaxTree** aux = new HeapMaxTree*[new_capacity];
            for(int i = 0; i < capacity; i++)
                aux[i] = array[i];
            array = aux;
            capacity = new_capacity;
        }
    }

    HeapMaxTree* Search(string str)
    {
        for(int i = 0; i < length; i++)
            if(array[i]->institution_name == str)
                return array[i];
        return nullptr;
    }
};

struct HashTable
{   
    HeapTreeQueue** queues;
    HashTable() : queues(new HeapTreeQueue*[1000]) {}

    void Add(HeapMaxTree* heap_tree)
    {
        int index = Hash(heap_tree->institution_name);
        queues[index]->Add(heap_tree);
    }

    HeapMaxTree* Search(string str)
    {
        int index = Hash(str);
        return queues[index]->Search(str);
    }

    int Hash(string str) 
    {
        int sum = 0;
        for (char c : str) 
            sum += (int) c;  // testar
        return (sum % 1000);
    }
};


int main(int argc, char* argv[])
{
    ifstream input(argv[1]);
    ofstream output(argv[2]);              

	//ifstream input("controledesenha.input");
	//ofstream output("saida.txt");

    string line;
    getline(input,line);
    int institution_amount = stoi(line);

    HeapMaxTree** institutions_trees = new HeapMaxTree*[institution_amount];

    stringstream reader;
    string institution;
    int workers_amount;

    for(int i = 0; i < institution_amount; i++)
    {
        getline(input,line);
        reader.str(line);
        reader >> institution >> workers_amount;
        reader.clear();
        institutions_trees[i] = new HeapMaxTree(institution,workers_amount);
    }

    getline(input,line);
    int person_amount = stoi(line);
    string name, age_str;
    for(int i = 0; i < person_amount; i++)
    {
        getline(input,line);
        reader.str(line);
        getline(reader, institution, '|');
        getline(reader, name, '|');
        getline(reader, age_str);
        for(int j = 0; j < institution_amount; j++)
        {   
            if(institutions_trees[j]->institution_name == institution)
            {
                institutions_trees[j]->Add(new Person(name,stoi(age_str)));
                break;
            }
        }
        reader.clear();
    }
    
    for(int i = 0; i < institution_amount; i++)
    {   
        institutions_trees[i]->BuildHeapTree();
        
    }

    for(int j = 0; j < person_amount; j++)
        for(int i = 0; i < institution_amount; i++)
            institutions_trees[i]->WriteResult(&output);

    return EXIT_SUCCESS;
}












