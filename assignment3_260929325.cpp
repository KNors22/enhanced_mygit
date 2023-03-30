/*
=========================
Name        : assignment3_260929325.cpp
Author      : Andrey Gonzalez
Description : Enhanced (simple) git tool in C++
Comment     : File has been developed, compiled and tested on McGill Mimi Server.

Legal Operations include:
    1. Adding a version of the file when the user finds suitable
    2. Removing a version (any version that was being tracked)
    3. Loading a specific version and making it the current active one
    4. Printing to the screen the list of all versions of the file
    5. Comparing any 2 versions of the file
    6. Searching and displaying all the versions having a specific keyword
    7. Exiting the program

    Versions of the file are kept in a linked list using dynamic mem, but are
    still available between program runs.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <filesystem>

#define PROMPT ">>"
#define FILE_NAME "./file.txt"  // only 1 file needed

class Node
{
    public:
    //TODO: fix constructors
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

    ~Node(){} //destructor

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



class LinkedList
{
private: 
    Node* head; // no need for public access
    int size;

public:
    LinkedList(){
        head = nullptr;
        size = 0;
    }

    ~LinkedList(){
        Node* temp, *next = head;
        
        // delete linked list
        // while(next != nullptr){
        //     temp = next->getNext();
        //     delete next;
        //     next = temp;
        // }

        // delete head;
    }

    /** HELPER FUNCTIONS */
    void appendNode(const std::string content, unsigned int hash, int version)
    {
        Node* next = nullptr;
        Node* hold = new Node(content, hash, version);

        if(head == nullptr){ // add node if list empty
            head = hold;
        }
        else{  // append if nonempty
            next = head;
            while(next->getNext() != nullptr) next = next->getNext();
            next->setNext(hold);
        }
        
        this->size++;
        return;
    }

    Node* getHead()
    {
        return head;
    }
    
    Node* getNodeByVersion(int version)
    {
        Node* temp = head;

        while(temp != nullptr){
            if(temp->getVersion() == version)
                return temp->makeCopy();
            temp = temp->getNext();
        }

        return nullptr; // unsuccessful!
    }

    int getNumberOfVersions()
    {
        return this->size;
    }

    void printList()
    {
        Node* temp = head;
        while(temp != nullptr){
            temp->printNode();
            temp = temp->getNext();
        }
    }

    //Checks if input content is already in `head`
    bool checkHashInHistory(unsigned int in_hash)
    {
        Node* temp = this->head;
        
        while(temp != nullptr){
            unsigned int temp_hash = temp->getHash();
            if(in_hash == temp_hash) return true;

            temp = temp->getNext();
        }

        return false;
    }

    bool removeNodeByVersion(int in_version)
    {
        Node* previous, *headtmp = head;

        if(in_version == 1 || getNumberOfVersions() == 1){
            head = headtmp->getNext();
            headtmp->setNext(nullptr);
            this->size--;
            return true;
        }

        while(headtmp->getNext() != nullptr){
            previous = headtmp;
            headtmp = headtmp->getNext();
            
            if(headtmp->getVersion() == in_version){
                previous->setNext(headtmp->getNext());
                headtmp->setNext(nullptr);
                
                // delete headtmp;    // remove Node
                this->size--;
                return true;
            }
        }

        return false;
    }
};


class Git322
{
public:
    Git322(){
        print_welcome_message();
    };

    ~Git322(){
        if(rfile.is_open()) rfile.close();
    };

    void add()
    {
        resetRead(); // prevent fstream failure

        std::string aline, content = "";
        while(getline(rfile, aline)){
            content += aline;
            content += '\n';
        }

        content.erase(content.end()-1); //remove last '\n'

        unsigned int hash = hash_it(content);    // hash input `content`
        bool isExist = mylist.checkHashInHistory(hash);  // checks if `content` already added w/ hash

        if(content.empty() || isExist)
        {
            std::cout << "git322 did not detect any change to your file and will not create a new version.\n";
            return;
        }
        
        // display success message
        std::cout << "Your content had been added successfully." << std::endl;

        // calculate new version number
        int version = mylist.getNumberOfVersions() + 1;

        // add content since new in `head`
        std::string strarg(content);
        mylist.appendNode(strarg, hash, version);

        return;
    }

    void print()
    {
        resetRead();
        int count = 0;
        count += mylist.getNumberOfVersions();

        std::cout << "Number of versions: " << count << '\n';
        mylist.printList();   //prints entire head
    }

    void load(int version)
    {
        if(!this->mylist.getNodeByVersion(version))
        {
            std::cout << "Please enter a valid version number. "
            << "If you are not sure please press 'p' to list all valid version numbers.\n";
        }

        resetRead();

        Node* result = mylist.getNodeByVersion(version);
        if(result == nullptr) std::cout << "Error while loading... Try Again!\n";
        std::string content_to_load = result->getContent();

        wfile.open(FILE_NAME, std::ios::out | std::ios::trunc);
        wfile << content_to_load;
        wfile.close();

        std::cout << "Version " << version << " loaded successfully. " << 
        "Please refresh your text editor to see the changes.\n";
    }

    void remove(int version)
    {
        if( !this->mylist.getNodeByVersion(version) )
        {
            std::cout << "Please enter a valid version number.\n";
        }
        resetRead();
        
        bool flag = mylist.removeNodeByVersion(version);
        std::cout << "Version " << version << " deleted successfully. " << '\n';
        return;
    }

    void compare(int version1, int version2)
    {
        //checking if both valid
        if( !this->mylist.getNodeByVersion(version1) || !this->mylist.getNodeByVersion(version2) )
        {
            std::cout << "Please enter a valid version number. If you are not sure \
            please press 'p' to list all valid version numbers.\n";
        }

        resetRead();
        // get content using version number and store as istringstream
        std::istringstream content1( mylist.getNodeByVersion(version1)->getContent() );
        std::istringstream content2( mylist.getNodeByVersion(version2)->getContent() );
        
        // call recursive helper function
        compare_and_print_lines(&content1, &content2);
        
        return;
    }

    void search(std::string keyword)
    {
        resetRead();
        int count = 0;
        Node* head = mylist.getHead();

        help_search(keyword, head, count);

        if(count == 0 ) std::cout << "Your keyword '" << keyword << "' was not found in any version.\n";
        return;
    }

    

protected:
    // CLASS ATTRIBUTES
    std::fstream rfile, wfile;
    LinkedList mylist;

    // HELPER FUNCTIONS AS CLASS METHODS
    void readFile()
    {
        if(!rfile.is_open()) rfile.open(FILE_NAME, std::ios::in);
        if(rfile.fail()){
            std::cout << "ERROR 322: `file.txt` does not exist. Goodbye!\n";
        }
        return;

        //return std::move(rfile); // does not get deleted with `move`
    }
    
    void resetRead()
    {
        rfile.clear();
        rfile.seekg(0);     // reposition to beginning
    }

    void print_help_message()
    {
        std::string help = "\
    To add the content of your file to version control press 'a'\n\
    To remove a version press 'r'\n\
    To load a version press 'l'\n\
    To print to the screen the detailed list of all versions press 'p'\n\
    To compare any 2 versions press 'c'\n\
    To search versions for a keyword press 's'\n\
    To exit press 'e'\n";

        std::cout << help << std::endl;
        return;
    }

    void print_welcome_message()
    {
        std::cout << "Welcome to the Comp322 file versioning system!\n\n";
        print_help_message();
        return;
    }

    unsigned int hash_it (std::string someString){
        std::hash<std::string> hash_obj;
        return hash_obj(someString);
    }

    // Helper function for `compare()` that compares lines and displays if non-identical
    void compare_and_print_lines(std::istringstream* text1, std::istringstream* text2, int linenum=1)
    {
        if(text1->eof() && text2->eof()) return;  //base case
        
        // get next lines
        std::string line1, line2;
        if( !std::getline(*text1, line1, '\n')) line1 = "<Empty Line>"; // overwrite `line1` if getline unsuccessful
        if( !std::getline(*text2, line2, '\n')) line2 = "<Empty Line>";

        unsigned int hash1 = hash_it(line1);
        unsigned int hash2 = hash_it(line2);

        std::cout << "Line " << linenum << ": ";  // display line number

        if(hash1 == hash2) std::cout << "<Identical>\n";
        else std::cout << line1 << " <<>> " << line2 << '\n';
        
        compare_and_print_lines(text1, text2, ++linenum);   // pre-increment linenum
    }

    // Helper function for `search()` that checks following punctuation:
    // ! " # $ % & ' ( ) * , - . / : ; ? @ [ \ ] ^ _ ` { | } ~ < > 
    bool isPunctuation(char in)
    {
        return (
            in == '.' || 
            in == '!' || 
            in == '?' || 
            in == ':' ||
            in == '\"'||
            in == '@' ||
            in == '#' ||
            in == '^' ||
            in == '&' ||
            in == '*' ||
            in == '\''||
            in == '(' ||
            in == ')' ||
            in == '-' ||
            in == ';' ||
            in == '[' ||
            in == ']' ||
            in == '\\'||
            in == '_' ||
            in == '|' ||
            in == '{' ||
            in == '}' ||
            in == '~' ||
            in == '<' ||
            in == '>' 
            );
    }

    // Recursive helper function for `search`
    void help_search(const std::string keyword, Node* node, int& count, bool isFoundPrinted=false)
    {
        if(node == nullptr) return; // base case

        std::string word, content = node->getContent();
        unsigned int keyword_hash = hash_it(keyword);

        // remove any punctuation in word
        int length = content.size();
        for (int i = 0; i < length; i++)
        {
            if (isPunctuation(content[i]))
            {
                content.erase(i--, 1);
                length = content.size();
            }
        }

        // save content as string stream for easier word access
        std::stringstream contentstream (content);
        
        bool isFound = false;
        while(contentstream >> word)
        {
            if(hash_it(word) == keyword_hash){
                count++;
                isFound = true;
            }
        }

        if(count > 0 && !isFoundPrinted){
            std::cout << "The keyword '" << keyword << "' has been found in the following versions:\n";
            node->printNode();
            isFoundPrinted = true;
        }
        else if (count > 1 && isFound) node->printNode();

        help_search(keyword, node->getNext(), count, isFoundPrinted);
    }

};


class EnhancedGit322 : public Git322 {
    public:
    EnhancedGit322()
    {
        namespace fsys = std::filesystem; // for reading and saving files
        
        if(!fsys::exists(tempdir)){
            fsys::create_directory(tempdir); // create tempdir since no old versions
        } else {
            //reads files in tempdir 1-by-1, and saves to program memory
            for (const auto & persistent_file : fsys::directory_iterator(tempdir)){
                std::string full_path(persistent_file.path());
                full_path.erase(0,0); // trim both `"`
                full_path.erase(full_path.end(), full_path.end());
                
                rfile.close();
                if(!readFile(full_path)) continue; // point rfile to persistent_file

                int version_num = getVersionNum(getRelativePath(full_path)); // get vNum from file name
                addPersistentFile(version_num); // pre-load persistent_file
            }
        }

        rfile.close();
        readFile(FILE_NAME); // point rfile to `file.txt`
    }
    ~EnhancedGit322()
    {
        // close any file streams
        if(rfile.is_open()) rfile.close();

        // save all versions in program mem to persistent layer
        updatePersistentLayer();
    };

    private:
    // CLASS MEMBERS
    std::string tempdir = ".tempVersionHolder/";

    // sets `rfile` read pointer to `in_file`
    bool readFile(std::string in_file)
    {
        this->resetRead();
        rfile.open(in_file, std::ios::in);
        
        if(rfile.fail()){
            std::cout << "ERROR 322e: `" << in_file << "` could not be loaded properly!\n";
            return false;
        }
        return true;
    }

    // returns relative file name from full path
    std::string getRelativePath(std::string file)
    {
        return file.substr(file.find_last_of("/") + 1); // using '/' paths
    }

    int getVersionNum(std::string in_str)
    {
        std::string num = "";
        for (int i=0; i < in_str.length(); i++) {
            if(in_str[i] >= '0' && in_str[i] <= '9'){
                num += in_str[i];
            }   
        }

        return std::stoi(num);
    }
    
    void addPersistentFile(int version_num)
    {
        resetRead();

        std::string aline, content = "";
        while(getline(rfile, aline)){
            content += aline;
            content += '\n';
        }

        content.erase(content.end()-1); //remove last '\n'

        unsigned int hash = hash_it(content);    // hash input `content`

        // add content from persisten layer
        std::string strarg(content);
        mylist.appendNode(strarg, hash, version_num);

        return;
    }

    bool updatePersistentLayer(const std::string update_this_version = "")
    {
        // save all versions
        if(update_this_version.empty())
        {
            for(int i=1; i<= mylist.getNumberOfVersions(); i++)
            {
                Node* tempNode = mylist.getNodeByVersion(i);
                int versionNum = tempNode->getVersion();
                std::string fileName = tempdir + "file" + std::to_string(versionNum) + ".txt";

                //fileName = "file[1-9].txt"
                wfile.open(fileName, std::ios::out | std::ios::trunc);
                if(wfile.fail()) return false;
                
                wfile << tempNode->getContent();
                wfile.close();
            }
        }
        return true;
    }

};

#define MAIN_ENHANCED   // uses EnhancedGit322
//#define MAIN          // uses Git322
//#define TESTING       // VERSION WITH LOADED TESTS

#ifdef MAIN_ENHANCED
int main(){
    
    // Starting...
    EnhancedGit322 Log;
    
    char user_input = ' ';	//user's input stored here

    //interactive loop
	do{
        //opening file in read/write mode

        std::cout << PROMPT << " ";
        std::cin >> user_input;
        std::cin.ignore();      // remove '\n' from stream

        // Interprets `user_input`
        std::string aline, content = "";
        int version1, version2;

        switch(user_input)
        {
            case 'e':   /* EXIT */
            {
                break; // Log is deleted after out-of-scope
            }
            
            case 'a':   /* ADD */
            {
                Log.add();
                break;
            }

            case 'r':   /* REMOVE */
            {
                std::cout << "Enter the number of the version that you want to delete: ";
                std::cin >> version1;

                Log.remove(version1);
                break;
            }

            case 'l':   /* LOAD */ 
            {
                std::cout << "Which version would you like to load? ";
                std::cin >> version1;

                Log.load(version1);
                break;
            }

            case 'p':   /* PRINT */
            {
                Log.print();
                break;
            }

            case 'c':   /* COMPARE */
            {
                std::cout << "Please enter the number of the first version to compare: ";
                std::cin >> version1;
                std::cout << "Please enter the number of the second version to compare: ";
                std::cin >> version2;

                Log.compare(version1, version2);
                break;
            }

            case 's':   /* SEARCH */
            {
                aline = "";
                std::cout << "Please enter the keyword that you are looking for: ";
                std::cin >> aline;  // `aline` reused for input keyword
                std::cout << '\n';
                
                Log.search(aline);
                break;
            }
            
            default:    /* INVALID INPUT */
            {
                std::cout << "Invalid input. Please try one of the following:\n\n";
                std::string help = "\
                To add the content of your file to version control press 'a'\n\
                To remove a version press 'r'\n\
                To load a version press 'l'\n\
                To print to the screen the detailed list of all versions press 'p'\n\
                To compare any 2 versions press 'c'\n\
                To search versions for a keyword press 's'\n\
                To exit press 'e'\n";

                std::cout << help << std::endl;
            }
        }

    } while(user_input != 'e');

    //delete LinkedList::head;
    return 0;
}
#endif


#ifdef MAIN
int main(){
    
    // Starting...
    Git322 Log;
    
    char user_input = ' ';	//user's input stored here
    int error_code = 0;     //stores error code from interpreter

    //interactive loop
	do{
        //opening file in read/write mode

        std::cout << PROMPT << " ";
        std::cin >> user_input;
        std::cin.ignore();      // remove '\n' from stream

        // Interprets `user_input`
        std::string aline, content = "";
        int version1, version2;

        switch(user_input)
        {
            case 'e':   /* EXIT */
            {
                break; // Log is deleted after out-of-scope
            }
            
            case 'a':   /* ADD */
            {
                Log.add();
                break;
            }

            case 'r':   /* REMOVE */
            {
                std::cout << "Enter the number of the version that you want to delete: ";
                std::cin >> version1;


                if(Log.mylist.getNodeByVersion(version1)){
                    Log.remove(version1);
                } 
                else {
                    std::cout << "Please enter a valid version number.\n";
                }
                break;
            }

            case 'l':   /* LOAD */ 
            {
                std::cout << "Which version would you like to load? ";
                std::cin >> version1;

                if(Log.mylist.getNodeByVersion(version1)){
                    Log.load(version1);
                } 
                else {
                    std::cout << "Please enter a valid version number. "
                    << "If you are not sure please press 'p' to list all valid version numbers.\n";
                }

                break;
            }

            case 'p':   /* PRINT */
            {
                Log.print();
                break;
            }

            case 'c':   /* COMPARE */
            {
                std::cout << "Please enter the number of the first version to compare: ";
                std::cin >> version1;
                std::cout << "Please enter the number of the second version to compare: ";
                std::cin >> version2;

                //checking if both valid
                if(Log.mylist.getNodeByVersion(version1) && 
                Log.mylist.getNodeByVersion(version2)){
                    Log.compare(version1, version2);
                }
                else {
                    std::cout << "Please enter a valid version number. If you are not sure \
                    please press 'p' to list all valid version numbers.\n";
                }

                break;
            }

            case 's':   /* SEARCH */
            {
                aline = "";
                std::cout << "Please enter the keyword that you are looking for: ";
                std::cin >> aline;  // `aline` reused for input keyword
                std::cout << '\n';
                
                Log.search(aline);
                break;
            }
            
            default:    /* INVALID INPUT */
            {
                std::cout << "Invalid input. Please try one of the following:\n\n";
                std::string help = "\
                To add the content of your file to version control press 'a'\n\
                To remove a version press 'r'\n\
                To load a version press 'l'\n\
                To print to the screen the detailed list of all versions press 'p'\n\
                To compare any 2 versions press 'c'\n\
                To search versions for a keyword press 's'\n\
                To exit press 'e'\n";

                std::cout << help << std::endl;
            }
        }

    } while(user_input != 'e');

    //delete LinkedList::head;
    return 0;
}
#endif


#ifdef TESTING
int main(){
    
    // Starting...
    print_welcome_message();
    
    char user_input = ' ';	//user's input stored here
    int error_code = 0;     //stores error code from interpreter

    mylist.head = new Node("Dear Comp 322 students.", 123, 1);
    mylist.appendNode("Dear Comp 322 students. C++ is a complicated language.", 456, 2);
    mylist.appendNode("Dear Comp 322 students. C++ is a complicated language.\nCoding in C++ is like going to the gym: No pain no gain!", 224, 3);
    mylist.appendNode("To the moon!!!", 104, 4);

    //interactive loop
	do{
        //opening file in read/write mode
        if(!rfile.is_open()) rfile.open(FILE_NAME, std::ios::in);

        std::cout << PROMPT << " ";
        std::cin >> user_input;
        std::cin.ignore();      // remove '\n' from stream


        rfile.clear();
        rfile.seekg(0);     //reposition to beginning
        error_code = interpreter(user_input);
        rfile.close();

    } while(user_input != 'e');

    
    rfile.close();  //closing file stream
    return 0;
}
#endif