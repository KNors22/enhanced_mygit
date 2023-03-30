# <p style="color:#DBA901"> ***my_Enhanced_GIT*** </p>

## **Description**

The application developed in C++ is a ***semi-simple*** version control system based on ***mySimple GIT***. Now, 
the file versions are accessible in between runs and will not get implicited removed. Features from previous version
are still available and produce the same output.

The features are found in the [Features](#features) section, as well as its completion status.

Example of unit tests are found in [Tests](#tests) section.

App development history is found in [Commits](#commits) section.

---
## **Features**

### This semi-simple version control includes the following features:

| Function    | Command | Description | Completed |
| ----------- | :-----: | ----------- | :----: |
| `add()`     | a       | Adding version of file when request iff modified. | ✅ |
| `remove()`  | r       | Remove any tracked version in log. | ✅ |
| `load()`    | l       | Load specific version in active mode. | ✅ |
| `print()`   | p       | Display the log of all versions. | ✅ |
| `compare()` | c       | Compare any 2 versions of the file. | ✅ |
| `search()`  | s       | Search and display all versions with specific keyword. | ✅ |
| `exit()`    | e       | Exit the program. | ✅ |
 
### Persistence Features
| Feature       | Description of Feature | Completed |
| ------------- | ---------------------- |   :----:  |  
| **Pre-Load**  | Load previous versions from persistence layer to program memory | ✅ |
| **Add v1**    | *No effect* -> Saving occurs at program exit ~***info loss if crash occurs***~<br> --> Prevent vNum duplicates | ✅ |
| **Add v2**    | Save versions in progmem at every `load` command ~***better***~ <br> --> Prevent vNum duplicates | ❌ |
| **Remove**    | Remove target version from persistence layer | ❌ |
| **Load**      | *No effect on persistence layer* | n/a |
| **Exit**      | Save existing versions from progmem if needed | ✅ |

#### **Comment:** When using `add()` version numbers cannot be duplicated.
---
## **Tests**

### Pre-Loading Content:
``` C++
- LIST::appendNode("Dear Comp 322 students.", 123, 1);
- LIST::appendNode("Dear Comp 322 students. C++ is a complicated language.", 456, 2);
- LIST::appendNode("Dear Comp 322 students.\nCoding in C++ is like going to the gym: No pain no gain!", 789, 3);
```

### Following Tests for `search`:
```C++
>> s
>> gym // displays version 3

>> s
>> C++ // displays version 2 and 3

>> s
>> Dear // displays all versions
```

---
## **Commits**
|   Commit Id   | Description |
|   ---------   | ----------- |
| ***6841ab9*** | - Initiated using code from `mySimple Git`|
| ***1fb5566*** | - Added `Node` and merged `Linked_List`; all features seem to work |
| ***bcf9392*** | - `getNumberOfVersions()` O(1) optimized <br> - Fixed ~LinkedList() <br> - Global vars are private `Git322` members |
| ***1bf0638*** | - Added initial `EnhancedGit` with file version persistence feature using `.tmpVersionHolder` dir ~***buggy***~ |
| ***CURRENT*** | - Preloading works using `addPersistentFile()` instead of standard `add()`  <br> - Fixed `add()` to point to `file.txt` after preload |

---