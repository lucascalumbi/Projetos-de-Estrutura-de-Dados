#include <iostream>
#include <fstream>
#include <cstdint>
#include <sstream>
#include <string>

using namespace std;

template <typename T>
struct Stack
{
    int32_t top;
    u_int32_t capacity;
    T* array;

    Stack(u_int32_t capacity) : top(-1), capacity(capacity)
    {   
        array = new T[capacity];
    }
    void Push (T data)
    {   
        if((u_int32_t) top+1 < capacity)
        {
            array[++top] = data;
        }
        else 
            cout << "Stack cheia!!!" << endl;
    }
    T Pop()
    {
        if(top > -1)
            return array[top--];
        else
        {
            cout << "Stack vazia!!!" << endl;
            return T(0);
        }
    }
    // T GetElemIndex(int index)
    // {       
    //     if(index > top)
    //     {
    //         cout << "indice fora dos limites!!!" << endl;
    //         return NULL;
    //     }
    //     return array[index];
    // }
    bool isEmpty()
    {
        return top < 0;
    }
    u_int32_t Size()
    {
        return (u_int32_t) top+1;
    }
    ~Stack()
    {
        delete[] array;
    }
};

struct Server
{   
    u_int32_t serverNumber;
    Stack<string*>* requests;
    static ofstream* output;
    Server(u_int32_t serverNumber, u_int32_t requestsLimit) : serverNumber(serverNumber)
    {
        requests = new Stack<string*>(requestsLimit);
    }
    u_int32_t Push(u_int32_t hash1Result, u_int32_t index, string* str)
    {
        if(requests->Size() == requests->capacity)
            return 0;

        if(index)
        {
            *output << "S" << hash1Result << "->" << "S" << serverNumber << endl;
        }
        requests->Push(str);
        WriteHistoric();        
        return 1;
    }
private:
    void WriteHistoric()
    {
        *output << "[S" << serverNumber << "] ";
        u_int32_t size = requests->Size();
        for(u_int32_t i = 0; i < size ; i++)
        {   
            *output << *requests->array[i] << (i < size - 1 ? ", " : "");
            //*output << requests->array[i] << ", " << endl;
        }
        *output << endl;
        //*output << requests->array[0] << endl;
    }
};

struct ServerQueue
{   
    u_int32_t capacity;
    Server** hashTable;

    ServerQueue(u_int32_t capacity, u_int32_t requestsLimit) : capacity(capacity)
    {
        hashTable = new Server*[capacity];

        for(u_int32_t i = 0; i < capacity; i++)
        {
            hashTable[i] = new Server(i,requestsLimit);
        }
    }
    void Allocate(string* str)
    {
        u_int32_t hash1Result = Hash1(*str);
        u_int32_t hash2Result = Hash2(*str);
        u_int32_t DHResult;
        u_int32_t firstTry = hash1Result % capacity;
        for(u_int32_t i = 0; ; i++)
        {
            DHResult = (hash1Result + i*hash2Result) % capacity;
            if(hashTable[DHResult]->Push(firstTry, i, str))
                return;
        }
        cout << "[ERROR] Quantidade de tentativas excedidas" << endl;
    }

private:
    u_int32_t Hash1(string str)
    {
        return (7919*CheckSum(str));
    }
    u_int32_t Hash2(string str)
    {
        return (104729*CheckSum(str)+123);
    }
    u_int32_t CheckSum(string str)
    {
        if (str.empty()) 
            return 0;

        u_int32_t result = str[0];
        u_int32_t length = str.length();

        for (u_int32_t i = 1; i < length; ++i)
        {   
            if(str[i] == ' ')
                continue;
            result ^= str[i];
            //cout << result << endl;
        } 

        return result; 
    }
};

ofstream* Server::output;

int main(int argc, char *argv[])
{   
    // ifstream input("entrada.txt"); 
    // ofstream output("saida.txt"); 

    ifstream input(argv[1]); 
    if (!input)
    {
        cerr << "Erro ao abrir o arquivo: " << argv[1] << endl;  
        return EXIT_FAILURE;
    }
    ofstream output(argv[2]); 
    if (!output)
    {
        cerr << "Erro ao abrir o arquivo: " << argv[2] << endl;
        return EXIT_FAILURE;
    }

    string line;
    getline(input, line);
    u_int32_t serversAmount;
    u_int32_t requestsLimit;
    stringstream reader;
    reader.clear();
    reader.str(line);
    reader >> serversAmount >> requestsLimit;
    cout << serversAmount << endl;
    cout << requestsLimit << endl;
    ServerQueue* Servers = new ServerQueue(serversAmount,requestsLimit);
    Server::output = &output;

    getline(input, line);
    u_int32_t requestsAmount = stoi(line);
    cout << requestsAmount << endl;

    string requestString;
    for(u_int32_t i = 0; i < requestsAmount; i++)
    {
        getline(input, line);
        requestString = line.substr(2);
        Servers->Allocate(new string(requestString));
    }

    input.close();
    output.close();
    return EXIT_SUCCESS;
}