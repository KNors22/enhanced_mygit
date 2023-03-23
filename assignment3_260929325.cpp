/*
=========================
Name        : assignment2_260929325.cpp
Author      : Andrey Gonzalez
Description : Creating simple git tool in C++
Comment     : File has been developed, compiled and tested on Mimi Server.

Legal Operations include:
    1. Adding a version of the file when the user finds suitable
    2. Removing a version (any version that was being tracked)
    3. Loading a specific version and making it the current active one
    4. Printing to the screen the list of all versions of the file
    5. Comparing any 2 versions of the file
    6. Searching and displaying all the versions having a specific keyword
    7. Exiting the program

    Versions of the file are kept in a linked list using dynamic mem.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include "linked_list.cpp"

#define PROMPT ">>"
#define FILE_NAME "./file.txt"  // only 1 file needed

std::fstream rfile, wfile;

// HELPER FUNCTIONS
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

// Ensures in_version is valid
int is_valid_version(int in_version)
{
    Node* retNode = LIST::getNodeByVersion(in_version);
    return (retNode != nullptr);
}

/** `ADD` IMPLEMENTATION */
void add(std::string content)
{
    unsigned int hash = LIST::hash_it(content);    // hash input `content`
    bool isExist = LIST::checkHashInHistory(hash);  // checks if `content` already added w/ hash

    if(content.empty() || isExist)
    {
        std::cout << "git322 did not detect any change to your file and will not create a new version.\n";
        return;
    }

    // calculate new version number
    int version = LIST::getNumberOfVersions() + 1;

    // add content since new in `History`
    std::string strarg(content);
    LIST::appendNode(strarg, hash, version);

    // display success message
    std::cout << "Your content had been added successfully." << std::endl;  
    return;
}

/** `PRINT` IMPLEMENTATION */
void print()
{
    int count = 0;
    count += LIST::getNumberOfVersions();

    std::cout << "Number of versions: " << count << '\n';
    LIST::printList();   //prints entire history
}

/** `LOAD` IMPLEMENTATION */
void load(int version)
{
    Node* result = LIST::getNodeByVersion(version);
    if(result == nullptr) std::cout << "Error while loading... Try Again!\n";
    std::string content_to_load = result->getContent();

    wfile.open(FILE_NAME, std::ios::out | std::ios::trunc);
    wfile << content_to_load;
    wfile.close();

    std::cout << "Version " << version << " loaded successfully. " << 
    "Please refresh your text editor to see the changes.\n";
}

/** `REMOVE` IMPLEMENTATION */
void remove(int version)
{
    bool flag = LIST::removeNodeByVersion(version);
    std::cout << "Version " << version << " deleted successfully. " << '\n';
    return;
}


/** `COMPARE` IMPLEMENTATION */
// Helper function that compares lines and displays if non-identical
void compare_and_print_lines(std::istringstream* text1, std::istringstream* text2, int linenum=1)
{
    if(text1->eof() && text2->eof()) return;  //base case
    
    // get next lines
    std::string line1, line2;
    if( !std::getline(*text1, line1, '\n')) line1 = "<Empty Line>"; // overwrite `line1` if getline unsuccessful
    if( !std::getline(*text2, line2, '\n')) line2 = "<Empty Line>";

    unsigned int hash1 = LIST::hash_it(line1);
    unsigned int hash2 = LIST::hash_it(line2);

    std::cout << "Line " << linenum << ": ";  // display line number

    if(hash1 == hash2) std::cout << "<Identical>\n";
    else std::cout << line1 << " <<>> " << line2 << '\n';
    
    compare_and_print_lines(text1, text2, ++linenum);   // pre-increment linenum
}

void compare(int version1, int version2)
{
    // get content using version number and store as istringstream
    std::istringstream content1( LIST::getNodeByVersion(version1)->getContent() );
    std::istringstream content2( LIST::getNodeByVersion(version2)->getContent() );
    
    // call recursive helper function
    compare_and_print_lines(&content1, &content2);
    
    return;
}


/** `SEARCH` IMPLEMENTATION */
// Helper function that checks following punctuation:
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
    unsigned int keyword_hash = LIST::hash_it(keyword);

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
        if(LIST::hash_it(word) == keyword_hash){
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

// Entry point for `help_search`
void search(std::string keyword)
{
    int count = 0;
    Node* head = LIST::getHead();

    help_search(keyword, head, count);

    if(count == 0 ) std::cout << "Your keyword '" << keyword << "' was not found in any version.\n";
    return;
}


//Interprets the instruction given by user.
int interpreter(const char& input){
    std::string aline, content = "";
    int version1, version2;

    switch(input)
    {
        case 'e':   /* EXIT */
        {
            LIST::deleteHistory;
            break;
        }
        
        case 'a':   /* ADD */
        {
            while(getline(rfile, aline)){
                content += aline;
                content += '\n';
            }

            content.erase(content.end()-1); //remove last '\n'
            
            add(content);
            break;
        }

        case 'r':   /* REMOVE */
        {
            std::cout << "Enter the number of the version that you want to delete: ";
            std::cin >> version1;

            if(is_valid_version(version1)){
                remove(version1);
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

            if(is_valid_version
        (version1)){
                load(version1);
            } 
            else {
                std::cout << "Please enter a valid version number. "
                << "If you are not sure please press 'p' to list all valid version numbers.\n";
            }

            break;
        }

        case 'p':   /* PRINT */
        {
            print();
            break;
        }

        case 'c':   /* COMPARE */
        {
            std::cout << "Please enter the number of the first version to compare: ";
            std::cin >> version1;
            std::cout << "Please enter the number of the second version to compare: ";
            std::cin >> version2;

            //checking if both valid
            if(is_valid_version
        (version1) && is_valid_version
        (version2)){
                compare(version1, version2);
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
            
            search(aline);
            break;
        }
        
        default:    /* INVALID INPUT */
        {
            std::cout << "Invalid input. Please try one of the following:\n\n";
            print_help_message();
        }
    }

    return 0;
}


#define MAIN
//#define TESTING  // VERSION WITH LOADED TESTS

#ifdef MAIN
int main(){
    
    // Starting...
    print_welcome_message();
    
    char user_input = ' ';	//user's input stored here
    int error_code = 0;     //stores error code from interpreter

    //interactive loop
	do{
        //opening file in read/write mode
        if(!rfile.is_open()) rfile.open(FILE_NAME, std::ios::in);
        if(rfile.fail()){
            std::cout << "ERROR 322: `file.txt` does not exist. Goodbye!\n";
            break;
        }

        std::cout << PROMPT << " ";
        std::cin >> user_input;
        std::cin.ignore();      // remove '\n' from stream


        rfile.clear();
        rfile.seekg(0);     // reposition to beginning
        error_code = interpreter(user_input);
        rfile.close();

    } while(user_input != 'e');

    LIST::deleteHistory(); // delete linked list
    rfile.close();  // closing file stream
    return 0;
}
#endif
#ifdef TESTING
int main(){
    
    // Starting...
    print_welcome_message();
    
    char user_input = ' ';	//user's input stored here
    int error_code = 0;     //stores error code from interpreter

    history = new Node("Dear Comp 322 students.", 123, 1);
    LIST::appendNode("Dear Comp 322 students. C++ is a complicated language.", 456, 2);
    LIST::appendNode("Dear Comp 322 students. C++ is a complicated language.\nCoding in C++ is like going to the gym: No pain no gain!", 224, 3);
    LIST::appendNode("To the moon!!!", 104, 4);

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