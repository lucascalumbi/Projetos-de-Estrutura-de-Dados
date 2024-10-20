#include <iostream>
#include <fstream>
#include <type_traits>
#include <sstream>

using namespace std;

struct IPrintableAndWritable
{
    virtual void Print() = 0;
    virtual void Write(ofstream* output) = 0;
	virtual ~IPrintableAndWritable() {};
};

template <typename T>
struct Node
{
    T data;
    Node* left;
    Node* right;

    Node(T data) : data(data), left(nullptr), right(nullptr) {}
    Node(T data, Node* left, Node* right) : data(data), left(left), right(right) {}
};

template <typename T>
struct BinaryTree
{   
    Node<T*>* root;

    BinaryTree() : root(nullptr) {}

    void Push(T* data)
    {	
        PushAux(root,data);
    }
	Node<T*>* Search(T* data)
	{
		return SearchAux(root, data);
	}
	Node<T*>* GetHigherElem()
	{	
		return GetHigherElemAux(root);
	}
	Node<T*>* GetLowerElem()
	{
		return GetLowerElemAux(root);
	}
	void RemoveElem(T* data)
	{
		RemoveElemAux(root, nullptr, data);
	}
    void PrintAllOrders()
    {   
        PrintInOrder();
        PrintPreOrder();
        PrintPostOrder();
    }
    void PrintInOrder()
	{	
		cout << "EPD:" << endl;
		PrintInOrderAux(root);
	}
	void PrintPreOrder()
	{	
		cout << "PED:" << endl;
		PrintPreOrderAux(root);
	}
	void PrintPostOrder()
	{	
		cout << "EDP:" << endl;
		PrintPostOrderAux(root);
	}
    void WriteAllOrders(ofstream* output)
    {   
        WriteInOrder(output);
        WritePreOrder(output);
        WritePostOrder(output);
    }
    void WriteInOrder(ofstream* output)
	{	
		*output << "EPD:" << endl;
		WriteInOrderAux(root, output);
	}
	void WritePreOrder(ofstream* output)
	{	
		*output << "PED:" << endl;
		WritePreOrderAux(root, output);
	}
	void WritePostOrder(ofstream* output)
	{	
		*output << "EDP:" << endl;
		WritePostOrderAux(root, output);
	}	
private:            
    virtual void PushAux(Node<T*>*& subTree,T* data)
    {	
        if(!subTree)
        {
            subTree = new Node<T*>(data);
            return;
        }
        if(*data > *(subTree->data))
			PushAux(subTree->right,data);
        else if(*data == *(subTree->data))
		{	
			T* aux = subTree->data;
			subTree->data = data;
			delete aux;
			// *(subTree->data) = *data;
			// delete data;
		}
		else
			PushAux(subTree->left,data);
    }
	Node<T*>* SearchAux(Node<T*>* subTree, T* data)
	{
		if(!subTree)
			return nullptr;
		if(*data == *(subTree->data))
		{
			return subTree;
		}
		else if(*data > *(subTree->data))
		{
			return SearchAux(subTree->right, data);
		}
		else 
		{
			return SearchAux(subTree->left, data);
		}
	}
	Node<T*>* GetHigherElemAux(Node<T*>* subTree)
	{
		if(!subTree)
			return nullptr;
		if(!subTree->right)
			return subTree;
		else 
			return GetHigherElemAux(subTree->right);
	}
	Node<T*>* GetLowerElemAux(Node<T*>* subTree)
	{
		if(!subTree)
			return nullptr;
		if(!subTree->left)
			return subTree;
		else 
			return GetLowerElemAux(subTree->left);
	}
	void RemoveElemAux(Node<T*>* subTree, Node<T*>* parent, T* data)
	{	
		if(!subTree)
			return;
		if(data == subTree->data)
		{	
			if(!subTree->left && !subTree->right)
			{	
				if(parent)
					subTree == parent->left ? parent->left = nullptr : parent->right = nullptr;
				delete subTree;
			}
			else if(!subTree->right)
			{	
				if(parent)
				{
					subTree == parent->left ? parent->left = subTree->left : parent->right = subTree->left;
					delete subTree;
				}
				else
				{
					Node<T*>* aux = subTree->left;
					delete subTree;
					subTree = aux;
				}
				
			}
			else if(!subTree->left)
			{	
				if(parent)
				{
					subTree == parent->left ? parent->left = subTree->right : parent->right = subTree->right;
					delete subTree;
				}
				else 
				{
					Node<T*>* aux = subTree->right;
					delete subTree;
					subTree = aux;
				}
			}
			else
			{	 
				Node<T*>* parentHigherLefterElem = subTree;
				Node<T*>* higherLefterElem = subTree->left;
				
				while (higherLefterElem->right)
				{
					parentHigherLefterElem = higherLefterElem;
					higherLefterElem = higherLefterElem->right; 
				}
		
				if (parentHigherLefterElem != subTree) 
				{
					parentHigherLefterElem->right = higherLefterElem->left; // Conectar o filho esquerdo do substituto ao pai
				}
				else
				{
					subTree->left = higherLefterElem->left;
				}

				higherLefterElem->left = subTree->left; 
				higherLefterElem->right = subTree->right; 

				if (parent) 
				{
					if (parent->left == subTree) 
						parent->left = higherLefterElem;
					else 
						parent->right = higherLefterElem;
				} 
				else 
				{
					root = higherLefterElem; 
				}

				delete subTree;
			}			
		}
		else if(data > subTree->data)
			return RemoveElemAux(subTree->right, subTree, data);
		else 
			return RemoveElemAux(subTree->left, subTree, data);
	}
	void PrintInOrderAux(Node<T*>* subTree)
	{   
        if (!subTree) 
            return;
		if(subTree->left)
		{
			PrintInOrderAux(subTree->left);
		}
		subTree->data->Print();
		if(subTree->right)
		{
			PrintInOrderAux(subTree->right);
		}
	}
    void WriteInOrderAux(Node<T*>* subTree, ofstream* output)
	{   
        if (!subTree) 
            return;
		if(subTree->left)
		{
			WriteInOrderAux(subTree->left, output);
		}
		subTree->data->Write(output); 
		if(subTree->right)
		{
			WriteInOrderAux(subTree->right, output);
		}
	}
	void PrintPreOrderAux(Node<T*>* subTree)
	{   
        if (!subTree) 
            return;
		subTree->data->Print();
		if(subTree->left)
		{
			PrintPreOrderAux(subTree->left);
		}
		if(subTree->right)
		{
			PrintPreOrderAux(subTree->right);
		}
	}
    void WritePreOrderAux(Node<T*>* subTree, ofstream* output)
	{   
        if (!subTree) 
            return;
		subTree->data->Write(output);
		if(subTree->left)
		{
			WritePreOrderAux(subTree->left, output);
		}
		if(subTree->right)
		{
			WritePreOrderAux(subTree->right, output);
		}
	}
	void PrintPostOrderAux(Node<T*>* subTree)
	{   
        if (!subTree) 
            return;
		if(subTree->left)
		{
			PrintPostOrderAux(subTree->left);
		}
		if(subTree->right)
		{
			PrintPostOrderAux(subTree->right);
		}
		subTree->data->Print();
	}
    void WritePostOrderAux(Node<T*>* subTree, ofstream* output)
	{   
        if (!subTree) 
            return;
		if(subTree->left)
		{
			WritePostOrderAux(subTree->left, output);
		}
		if(subTree->right)
		{
			WritePostOrderAux(subTree->right, output);
		}
		subTree->data->Write(output);
	}
};

struct File : IPrintableAndWritable
{
    string name;
    bool isWritable;
    u_int32_t size;
    u_int32_t insertionOrderNumber;

    File(string info, u_int32_t index)
    : name(""), isWritable(false), size(0), insertionOrderNumber(index)
    {
        stringstream reader;
        string writeStatus;
        reader.str(info);
        reader >> name >> writeStatus >> size;
        isWritable = (writeStatus == "rw");
    }
    File(string name, bool isWritable, u_int32_t size, u_int32_t insertionOrderNumber) 
        : name(name), isWritable(isWritable), size(size), insertionOrderNumber(insertionOrderNumber) {}

    bool operator > (const File& file) const
    {
        return (this->name > file.name);
    }
    bool operator < (const File& file) const
    {
        return (this->name < file.name);
    }
    bool operator == (const File& file) const
    {
        return (this->name == file.name);
    } 
	void operator = (const File& file)
	{	
		if(!isWritable)
			return;
		
		name = file.name;
		isWritable = file.isWritable;
		size = file.size;
		insertionOrderNumber = file.insertionOrderNumber; 
	}
	void Print() override
    {
        string permissions = isWritable ? "rw" : "ro";
        string sizeLabel = (size > 1) ? "bytes" : "byte";
        cout << insertionOrderNumber << " " << name << " " << permissions << " " << size << " " << sizeLabel << endl;
    }
    void Write(ofstream* output) override
    {
        string permissions = isWritable ? "rw" : "ro";
        string sizeLabel = (size > 1) ? "bytes" : "byte";
        *output << insertionOrderNumber << " " << name << " " << permissions << " " << size << " " << sizeLabel << endl;
    }
};

struct FileTree : BinaryTree<File>
{
    FileTree() : BinaryTree<File>() {}

	void PushAllFiles(u_int32_t filesAmount, ifstream* input)
    {   
        string line;
		File* newFile;
        for(u_int32_t i = 0; i < filesAmount; i++)
        {
            getline(*input,line);
			newFile = new File(line, i);
            Push(newFile);
        }
    }
};

int main(int argc, char* argv[])
{
    ifstream* input = new ifstream(argv[1]);
    ofstream* output = new ofstream(argv[2]);

    string line;
    getline(*input,line);
    u_int32_t filesAmount = stoi(line);
    
	FileTree* fileTree = new FileTree();

    fileTree->PushAllFiles(filesAmount, input);
    fileTree->WriteAllOrders(output);

    return EXIT_SUCCESS;
}





