#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

/** FUNCTION PROTOTYPES */
int removeByIndex(int n);

//`Node` is used for every version of the file
class Node
{
public:
    Node(const std::string c, unsigned int h, int v, Node* n): content(c), hash_value(h), version(v), next(n)
    {
        contentBuffer = new char[c.size() + 1];

        if (contentBuffer != nullptr)
            strcpy(contentBuffer, c.c_str());    // Copy content to the internal buffer
    }
    
    Node(const std::string c, unsigned int h, int v): content(c), hash_value(h), version(v), next(nullptr)
    {
        contentBuffer = new char[c.size() + 1];

        if (contentBuffer != nullptr)
            strcpy(contentBuffer, c.c_str());    // Copy content to the internal buffer
    }

    ~Node(){}

    Node* getNext() {return this->next;}
    void setNext(Node* n) {this->next = n;}
    unsigned int getHash() {return this->hash_value;}
    int getVersion() {return this->version;}

    char* getContent()
    {
        if(contentBuffer == nullptr) return nullptr;
        return contentBuffer;
    }
    Node* makeCopy()
    {
        return new Node(content, hash_value, version);
    }
    void printNode()
    {
        /* HOW TO GET THIS RIGHT??? */
        
        // std::string info = "Version number: " + this->getVersion()
        // + "\nHash value: " + this->getHash()
        // + "\nContent: " + this->content + '\n';

        std::cout << "Version number: " << this->getVersion()
        << "\nHash value: " << this->getHash()
        << "\nContent: " << this->getContent() << "\n\n";
    }

protected:
    const std::string content;
    char* contentBuffer;
    const unsigned int hash_value;
    const int version;
    Node* next;
};

Node* history;  //List holding git history

namespace LIST
{
    /** HELPER FUNCTIONS */
    
    //Returns hash value of `someString`
    unsigned int hash_it (std::string someString){
        return std::hash<std::string>{}(someString);
    }
    
    void appendNode(const std::string content, unsigned int hash, int version)
    {
        Node* next = nullptr;
        Node* hold = new Node(content, hash, version);

        if(history == nullptr){ // add node if list empty
            history = hold;
        }
        else{  // append if nonempty
            next = history;
            while(next->getNext() != nullptr) next = next->getNext();
            next->setNext(hold);
        }
        return;
    }

    Node* getHead()
    {
        return history;
    }
    
    Node* getNodeByVersion(int version)
    {
        Node* temp = history;

        while(temp != nullptr){
            if(temp->getVersion() == version)
                return temp->makeCopy();
            temp = temp->getNext();
        }

        return nullptr; // unsuccessful!
    }

    int getNumberOfVersions()
    {
        Node* temp = history;
        int count = 0;

        while(temp != nullptr){
            count++;
            temp = temp->getNext();
        }

        return count; // return number of versions
    }

    void printList()
    {
        Node* temp = history;
        while(temp != nullptr){
            temp->printNode();
            temp = temp->getNext();
        }
    }

    //Checks if input content is already in `history`
    bool checkHashInHistory(unsigned int in_hash)
    {
        Node* temp = history;
        
        while(temp != nullptr){
            unsigned int temp_hash = temp->getHash();
            if(in_hash == temp_hash) return true;

            temp = temp->getNext();
        }

        return false;
    }

    bool removeNodeByVersion(int in_version)
    {
        Node* previous, *head = history;

        if(in_version == 1 || getNumberOfVersions() == 1){
            history = head->getNext();
            head->setNext(nullptr);
            return true;
        }

        while(head->getNext() != nullptr){
            previous = head;
            head = head->getNext();
            
            if(head->getVersion() == in_version){
                previous->setNext(head->getNext());
                head->setNext(nullptr);
                
                delete head;    // remove Node
                return true;
            }
        }
        return false;
    }

    //Delete entire list
    void deleteHistory()
    {
        Node* temp, *next = history;
        
        while(next != nullptr){
            temp = next->getNext();
            delete next;
            next = temp;
        }
    }

void DELETE(){
    // // Removes node at input index and returns stored value.
    // int removeByIndex(int index){
    //     int i = 0;
    //     int retval = -1;
    //
    //     Node* current = history;
    //     Node* temp_node = nullptr;
    //     if (index == 0)
    //         return pop(current);
    //
    //     for (i = 0; i < index-1; it+) {
    //         if (current->next == nullptr)
    //             return -1;
    //
    //         current = current->next;
    //     }
    //
    //     temp_node = current->next;
    //
    //     retval = temp_node->value;
    //     current->next = temp_node->next;
    //
    //     delete temp_node;   // clean up mem
    //     return retval;
    }
}

#define MAIN
#ifdef MAIN
int main() {
    history = new Node("Hello World", 123, 1);
    LIST::appendNode("This is another line", 456, 2);
    LIST::appendNode("Goodbye Earth", 224, 3);
    LIST::appendNode("To the moon!!!", 104, 4);

    std::cout << "-> STARTING STATE... <-\n\n";
    LIST::printList();

    std::cout << "Version to remove: ";
    int vers;
    std::cin >> vers;

    std::cout << "\n-> RESULTING STATE... <-\n\n";
    LIST::removeNodeByVersion(vers);
    LIST::printList();

    // LIST::deleteHistory();
    // std::cout << "Deleted List!\n";

    return 0;
}
#endif