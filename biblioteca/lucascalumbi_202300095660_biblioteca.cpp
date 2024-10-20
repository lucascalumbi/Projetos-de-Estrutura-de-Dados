#include <iostream>
#include <fstream>
#include <cstdint>
#include <sstream>
#include <string>
#include <cmath>

using namespace std;    

struct Book
{
    u_int64_t ISBnumber;
    string* author;
    string* bookName;

    Book(u_int64_t ISBnumber, string* author, string* bookName) : ISBnumber(ISBnumber), author(author), bookName(bookName) {}

    void PrintBook()
    {
        cout << ":Author:" << *author << ",Title:" << *bookName << endl; 
    }

    void PrintBook(ofstream* output)
    {
        *output << ":Author:" << *author << ",Title:" << *bookName << endl; 
    }
};

struct BookArray
{
    Book** array;
    u_int64_t capacity;
    
    BookArray(u_int64_t capacity) : capacity(capacity)
    {
        array = new Book*[capacity];
    }
    void PushAllBooks(ifstream* input)
    {   
        string line;
        u_int64_t ISBnumber;
        string author;
        u_int32_t pos;
        string bookName;
        for(u_int64_t i = 0; i < capacity; i++)
        {   
            getline(*input, line);
            ISBnumber = stoull(line.substr(0,13));
            pos = line.find('&');
            author = line.substr(14, pos-14);
            bookName = line.substr(pos+1);
            Push(new Book(ISBnumber, new string(author),new string(bookName)), i);
        }
    }
    void CompareSearchs(ifstream *input, ofstream* output,u_int64_t ISBNAmount)
    {   
        u_int64_t ISBnumber;
        string line;

        u_int64_t binaryWins = 0;
        u_int64_t interpolWins = 0;
        u_int64_t binaryCallsAmount = 0;
        u_int64_t interpolCallsAmount = 0;


        u_int64_t binaryCallsAux;
        u_int64_t interpolCallsAux;
        for(u_int64_t i = 0; i < ISBNAmount; i++)
        {   
            binaryCallsAux = 0;
            interpolCallsAux = 0;
            getline(*input,line);
            ISBnumber = stoull(line);
            Compare1Search(ISBnumber,&binaryCallsAux,&interpolCallsAux,output);
            binaryCallsAmount += binaryCallsAux;
            interpolCallsAmount += interpolCallsAux;
            binaryCallsAux < interpolCallsAux ? binaryWins++ : interpolWins++;
        }
        *output << "BINARY=" << binaryWins << ":" << trunc(binaryCallsAmount/ (ISBNAmount*1.0)) << endl;
        *output << "INTERPOLATION=" << interpolWins << ":" << trunc(interpolCallsAmount/ (ISBNAmount*1.0)) << endl;
    }
    void PrintAllBooks()
    {
        for(u_int64_t i = 0; i < capacity; i++)
        {
            array[i]->PrintBook();
        }
    }
    bool BinarySearch(
        u_int64_t ISBnumber, int64_t begin, 
        int64_t end, Book** findedBook,
        u_int64_t* binaryCalls)
    {   
        (*binaryCalls)++;
        if(begin > end)
            return 0;
        u_int64_t pivot = floor (((begin+end) / 2.0)); 
        u_int64_t finded = array[pivot]->ISBnumber;
        if(ISBnumber == finded)
        {   
            *findedBook = array[pivot];
            return 1;
        }
        if(ISBnumber > finded)
            return BinarySearch(ISBnumber,pivot+1,end,findedBook,binaryCalls);
        else
            return BinarySearch(ISBnumber,begin,pivot-1,findedBook,binaryCalls);
    }
    bool InterpolationSearch(
        u_int64_t ISBnumber, int64_t begin, 
        int64_t end, Book** findedBook,
        u_int64_t* interpolCalls)
    {   
        if(begin > end)                 
            return 0;        
        u_int64_t pivot = Hash(begin,end);
        (*interpolCalls)++; // ordem supostamente errada, devria estar antes do if
        // if(pivot < begin || pivot > end)
        // {   
        //     cout << "MEC MEC" << endl;
        //     return 0;
        // }
        u_int64_t finded = array[pivot]->ISBnumber;
        if(ISBnumber == finded)
        {   
            *findedBook = array[pivot];
            return 1;
        }
        if(ISBnumber > finded)
            return InterpolationSearch(ISBnumber,pivot+1,end,findedBook,interpolCalls);
        else
            return InterpolationSearch(ISBnumber,begin,pivot-1,findedBook,interpolCalls);
    }
private:

    u_int64_t Hash(u_int64_t begin, u_int64_t end)
    {
        return (begin+((array[end]->ISBnumber - array[begin]->ISBnumber) % (end-begin+1)));
    }
    void Push(Book* book, u_int64_t index)
    {
        array[index] = book;
    }
    void Compare1Search(u_int64_t ISBnumber, u_int64_t* binaryCalls,
        u_int64_t* interpolCalls,ofstream *output)
    {
        Book* findedBook1{}; // inicializa como nulo
        Book* findedBook2{};

        if(BinarySearch(ISBnumber,0,capacity-1,&findedBook1,binaryCalls) &
        InterpolationSearch(ISBnumber,0,capacity-1,&findedBook2,interpolCalls))
        {   
            if(findedBook1 == findedBook2)
            {   
                *output << "[" << ISBnumber << "]";
                *output << "B=" << *binaryCalls << ",I=" << *interpolCalls;
                findedBook1->PrintBook(output);
            }
            else
            {
                cout << "errouu" << endl;
            }
        }
        else
        {   
            *output << "[" << ISBnumber << "]";
            *output << "B=" << *binaryCalls << ",I=" << *interpolCalls << ":ISBN_NOT_FOUND" << endl; 
        }
    }
};

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

    u_int64_t booksAmount = stoull(line);
    cout << booksAmount << endl;

    BookArray* library = new BookArray(booksAmount);

    library->PushAllBooks(&input);
    
    getline(input, line);
    u_int64_t ISBNAmount = stoull(line);
    cout << ISBNAmount << endl;

    library->CompareSearchs(&input,&output,ISBNAmount);

    delete library;

    input.close();
    output.close();
    return EXIT_SUCCESS;
}