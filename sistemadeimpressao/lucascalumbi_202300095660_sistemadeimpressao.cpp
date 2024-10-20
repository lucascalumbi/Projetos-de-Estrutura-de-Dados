#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

struct Document
{
    string name;
    int pages;
    Document(string name, int pages) : name(name), pages(pages) {}
};  

template <typename T>
struct Elem
{   

    T data;
    Elem* above;

    Elem(T data) : data(data), above(nullptr) {}
    Elem(T data, Elem* above) : data(data), above(above) {}

};

struct Document_historic 
{   
    Elem<Document*>* top;
    int size;

    Document_historic() : top(nullptr), size(0){}

    void Push (Document* data)
    {   
        if(!top)
            top = new Elem<Document*>(data);
        else 
            top = new Elem<Document*>(data,top);
        size++;
    }
    Document* Pop() 
    {   
        if(size > 0)
        {
            Elem<Document*>* aux = top; 
            top = top->above;
            Document* data = aux->data;
            delete aux;
            size--;
            return data;
        }
        else
        {
            cout << "Stack vazia!!!" << endl;
            return NULL;
        }
    }
    bool isEmpty()
    {
        return !size;
    }
    ~Document_historic()
    {       
        Elem<Document*>* aux;
        while (top)
        {   
            aux = top;
            top = top->above;
            delete aux;
        }
    }
};

struct Document_stack
{
    int top;
    int capacity;
    int printedPagesAmount;
    Document** array;

    Document_stack(int capacity) : top(-1), capacity(capacity), printedPagesAmount(0)
    {   
        array = new Document*[capacity];
    }
    void Push (Document* data)
    {   
        if(top < capacity)
        {
            array[++top] = data;
            printedPagesAmount += data->pages;
        }
        else 
            cout << "Stack cheia!!!" << endl;
    }
    Document* Pop()
    {
        if(top > -1)
            return array[top--];
        else
        {
            cout << "Stack vazia!!!" << endl;
            return NULL;
        }
    }
    Document* GetElemIndex(int index)
    {       
        if(index > top)
        {
            cout << "indice fora dos limites!!!" << endl;
            return NULL;
        }
        return array[index];
    }
    void ShowDocumentStack(ofstream* output)
    {   
        *output << printedPagesAmount << "p" << endl;
        int size = Size();
        Document* aux;
        for(int i = size-1; i > -1; i--)
        {   
            aux = GetElemIndex(i);
            *output << aux->name + "-" + to_string(aux->pages) + "p" << endl;
        }
        return;
    }
    bool isEmpty()
    {
        return top < 0;
    }
    int Size()
    {
        return top+1;
    }
    ~Document_stack()
    {
        for(int i = capacity-1; i > -1; i--)
            delete array[i];    
        delete array;
    }
};

struct Printer
{
    string name;
    int pagesAmount;
    Document* currentPrintingDoc;
    Document_historic* historic;
    static Document_stack* printedDocuments;

    Printer(string name) : name(name), pagesAmount(0), currentPrintingDoc(nullptr)
    {
        historic = new Document_historic();
    }

    void Add(ofstream* output,string line)
    {   
        stringstream reader;
        string documentName;
        int pages;
        reader.clear();
        reader.str(line);
        reader >> documentName >> pages;
        this->pagesAmount += pages;
        if(historic->isEmpty())
        {   
            currentPrintingDoc = new Document(documentName,pages);
            historic->Push(currentPrintingDoc);
            WriteHistoric(output);
        }
        else
        {   
            printedDocuments->Push(currentPrintingDoc);
            currentPrintingDoc = new Document(documentName,pages);
            historic->Push(currentPrintingDoc);
            WriteHistoric(output);
        }
    }
    void Finish()
    {   
        printedDocuments->Push(currentPrintingDoc);
        currentPrintingDoc = nullptr;
    }
    
private:
    void WriteHistoric(ofstream* output) // ineficiente
    {
        *output << "[" + name + "] ";
        int size = historic->size;
        Elem<Document*>* aux = historic->top;

        for(int i = 0; i < size-1; i++)
        {   
            *output << aux->data->name + "-" + to_string(aux->data->pages) + "p, ";
            aux = aux->above;
        }
        *output << aux->data->name + "-" + to_string(aux->data->pages) + "p" << endl;
        return;
    }
    ~Printer()
    {
        delete historic;
    }
};

struct PrinterQueue
{
    int begin;
    int end;
    int length;
    int capacity;
    int printedPages;
    Printer** array;

    PrinterQueue (int capacity) : begin(0), end(0), length(0), capacity(capacity), printedPages(0)
    {   
        array = new Printer*[capacity];
    }
    void AddPrinter(string name)
    {
        Push(new Printer(name));
    }
    void AddDocuments(ofstream* output,string line)
    {   
        GetWorklessPrinter()->Add(output,line);
    }
    void Finish()
    {
        GetWorklessNotIdlePrinter()->Finish();
    }
private:

    Printer* GetWorklessPrinter()
    {   
        Printer* aux = array[0];
        for(int i = 1; i < length; i++)
        {   
            aux = aux->pagesAmount > array[i]->pagesAmount ?  array[i] : aux;
        }
        return aux;
    }
    Printer* GetWorklessNotIdlePrinter()
    {   
        Printer* aux = array[0];
        for(int i = 1; i < length; i++)
        {   
            if(!aux->currentPrintingDoc)
            {
                aux = array[i];
                continue;
            }
            if(!array[i]->currentPrintingDoc)
                continue;

            aux = aux->pagesAmount > array[i]->pagesAmount ?  array[i] : aux;
        }
        return aux;
    }
    void Push(Printer* newPrinter)
    {   
        if(length < capacity)
        {
            array[end] = newPrinter;
            ++end %= capacity;  
            length++;
        }
        else
            cout << "Queue cheia!!!" <<endl;
    }
    Printer* Pop()
    {   
        if(length)
        {   
            length--;
            Printer* aux = array[begin];
            ++begin %= capacity;
            return aux;
        }
        else
        {
            cout << "Queue vazia!!!" << endl;
            return NULL;
        }
    }
    bool isEmpty()
    {
        return length == 0;
    }
    int Size()
    {
        return length;
    }
    ~PrinterQueue()
    {
        delete[] array;
    }
};

Document_stack* Printer::printedDocuments;

int main(int argc, char *argv[])
{   
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
    int printersCount = stoi(line);

    PrinterQueue* printers = new PrinterQueue(printersCount);

    for(int i = 0; i < printersCount; i++)
    {   
        getline(input, line);
        printers->AddPrinter(line);
    }

    getline(input, line);
    int documentsAmout = stoi(line); 

    Printer::printedDocuments = new Document_stack(documentsAmout);

    for(int i = 0; i < documentsAmout; i++)
    {
        getline(input, line);
        printers->AddDocuments(&output, line);
    }

    for(int i = 0; i < printersCount; i++)
    {
        printers->Finish();
    }

    Printer::printedDocuments->ShowDocumentStack(&output);

    input.close();
    output.close();
    return EXIT_SUCCESS;
}