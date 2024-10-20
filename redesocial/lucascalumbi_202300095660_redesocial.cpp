#include <fstream>
#include <iostream>

template <typename T>
struct Node
{
    T data;
    Node<T>* previous;
    Node<T>* next;

    Node(T data) : data(data), previous(nullptr), next(nullptr) {}
    Node(T data, Node<T> *previous, Node<T> *next) : data(data), previous(previous), next(next) {}
};

struct DoublyLinkedList_string
{
    Node<std::string>* head;

    DoublyLinkedList_string() : head(nullptr) {}

    std::string Add(std::string data)
    {
        if (Search(data))
            return "[ERROR] ADD " + data;

        if (head == nullptr)
        {
            Node<std::string>* node = new Node<std::string>(data);
            node->previous = node;
            node->next = node;
            head = node;
        }
        else
        {
            Node<std::string>* node = new Node<std::string>(data, head->previous, head);
            head->previous->next = node;
            head->previous = node;
        }
        return "[ OK  ] ADD " + data;
    }
    std::string Show(std::string data)
    {
        Node<std::string>* ptr = Search(data);
        if (!ptr)
            return "[ERROR] ?<-" + data + "->?";
        else
            return "[ OK  ] " + ptr->previous->data + "<-" + ptr->data + "->" + ptr->next->data;
    }
    std::string Remove(std::string data)
    {
        Node<std::string>* ptr = Search(data);
        if (!ptr)
            return "[ERROR] REMOVE " + data;

        if (ptr == head)
        {
            Node<std::string>* nextAux = head->next;
            Node<std::string>* previousAux = head->previous;

            nextAux->previous = previousAux;
            previousAux->next = nextAux;
            head = nextAux;
        }
        else
        {
            Node<std::string>* aux = ptr->previous;

            aux->next = ptr->next;
            ptr->next->previous = aux;
        }

        delete ptr;
        return "[ OK  ] REMOVE " + data;
    }
    std::string ReadOperation(std::string line)
    {
        if (line[0] == 'A')
            return Add(line.substr(4));
        else if (line[0] == 'S')
            return Show(line.substr(5));
        else
            return Remove(line.substr(7));
    }

    ~DoublyLinkedList_string()
    {
        if (head)
        {
            Node<std::string>* ptr = head;
            do
            {
                ptr = ptr->next;
                delete ptr->previous;
            } while (ptr != head);
        }
    }

private:
    Node<std::string>* Search(std::string data)
    {
        if (!head)
            return nullptr;
        Node<std::string> *current = head;
        do
        {
            if (current->data == data)
                return current;
            else
                current = current->next;
        } while (current != head);
        return nullptr;
    }
};

int main(int argc, char *argv[])
{
    std::ifstream input(argv[1]);

    if (!input)
    {
        std::cerr << "Erro ao abrir o arquivo: " << argv[1] << std::endl;
        return EXIT_FAILURE;
    }

    std::ofstream output(argv[2]);

    if (!output)
    {
        std::cerr << "Erro ao abrir o arquivo: " << argv[2] << std::endl;
        return EXIT_FAILURE;
    }

    DoublyLinkedList_string *redesocial = new DoublyLinkedList_string;
    std::string line;
    while (std::getline(input, line))
        output << redesocial->ReadOperation(line) << std::endl;

    input.close();
    output.close();
    delete redesocial;

    return EXIT_SUCCESS;
}