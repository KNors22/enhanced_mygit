# <p style="color:#DBA901"> ***my_Enhanced_GIT*** </p>

## **Description**

The application developed in C++ is a ***semi-simple*** version control system based on ***mySimple GIT***. 

Using a temporary directory as the persistence layer, every file versions are accessible in between runs, and are only explicitly removed by the user. Features from previous version are still available and produce the same output.

The features are found in the [Features](#features) section, as well as its completion status.

Example of unit tests are found in [Tests](#tests) section.

App development history is found in [Commits](#commits) section.

Tasks for the developer are situated under [Developer's Tasks](#Developer's-Tasks) section.

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
| **Add v1**    | *No effect* :arrow_right: Saving occurs at program exit -***info loss if crash occurs***-<br> :arrow_right_hook: Prevent vNum duplicates | ✅ |
| **Remove**    | Remove target version from persistence layer | ✅ |
| **Load**      | *No effect on persistence layer* | n/a |
| **Exit v1**   | Save existing versions from progmem using text files + their version number| ✅ |
| **Exit v2**   | Persistence layer uses serialization to save versions -***better***- <br> :arrow_right_hook: Persistance layer also holds  hash value now  | ❌ |

#### **Comment:** When using `add()` duplicates of a version number will not appear.
---

## **Developer's-Tasks**
| Task                   | Description of Task | Completed |
| ---------------------- | ------------------- |   :----:  |
| **Make Exit v2**       | Enhanced Serialization :arrow_right: save hash in file name too | ❌ |
| **Exception Handling** | Prevent crashes | ❌ |


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
| ***1bf0638*** | - Added initial `EnhancedGit` with file version persistence feature using `.tmpVersionHolder` dir -***buggy***- |
| ***29f0783*** | - Preloading works using `addPersistentFile()` instead of standard `add()`  <br> - Fixed `add()` to point to `file.txt` after preload |
| ***7ca319d*** | - **FULL WORKING VERSION** <br> - Fixed ordering when adding persistent versions <br> - Fixed segfault when at exit point <br> - `remove()` now updates persistence layer <br> - Fixed logic errors for `load()` and `next_version_num` <br> - Improved naming convention |
| ***CURRENT*** | - Enhanced persistence: hash value saved when version persisted <br> - Fixed end condition for loop in `updatePersistenceLayer()` |

---