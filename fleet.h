#ifndef FLEET_H
#define FLEET_H
#include <iostream>
#include <vector>
using namespace std;
class Grader;
class Tester;
enum STATE {ALIVE, LOST};
enum SHIPTYPE {CARGO, TELESCOPE, COMMUNICATOR, FUELCARRIER, ROBOCARRIER};
const int MINID = 10000;
const int MAXID = 99999;
#define DEFAULT_HEIGHT 0
#define DEFAULT_ID 0
#define DEFAULT_TYPE CARGO
#define DEFAULT_STATE ALIVE
#define BLACK true
#define RED false
class Ship{
public:
    friend class Grader;
    friend class Tester;
    friend class Fleet;
    Ship(int id, SHIPTYPE type = DEFAULT_TYPE, STATE state = DEFAULT_STATE)
            :m_id(id),m_type(type), m_state(state) {
        m_left = nullptr;
        m_right = nullptr;
        //m_height = DEFAULT_HEIGHT;
        m_color = RED;
    }
    Ship(){
        m_id = DEFAULT_ID;
        m_type = DEFAULT_TYPE;
        m_state = DEFAULT_STATE;
        m_left = nullptr;
        m_right = nullptr;
        //m_height = DEFAULT_HEIGHT;
        m_color = RED;
    }
    int getID() const {return m_id;}
    STATE getState() const {return m_state;}
    string getStateStr() const {
        string text = "";
        switch (m_state)
        {
            case LOST:text = "LOST";break;
            case ALIVE:text = "ALIVE";break;
            default:text = "UNKNOWN";break;
        }
        return text;
    }
    SHIPTYPE getType() const {return m_type;}
    string getTypeStr() const {
        string text = "";
        switch (m_type)
        {
            case CARGO:text = "CARGO";break;
            case TELESCOPE:text = "TELESCOPE";break;
            case COMMUNICATOR:text = "COMMUNICATOR";break;
            case FUELCARRIER:text = "FUELCARRIER";break;
            case ROBOCARRIER:text = "ROBOCARRIER";break;
            default:text = "UNKNOWN";break;
        }
        return text
                ;
    }
    //int getHeight() const {return m_height;}
    bool getColor() const { return m_color; }
    Ship* getLeft() const {return m_left;}
    Ship* getRight() const {return m_right;}
    void setID(const int id){m_id=id;}
    void setState(STATE state){m_state=state;}
    void setType(SHIPTYPE type){m_type=type;}
    //void setHeight(int height){m_height=height;}
    void setColor(bool color) { m_color = color; }
    void setLeft(Ship* left){m_left=left;}
    void setRight(Ship* right){m_right=right;}
private:
    int m_id;
    SHIPTYPE m_type;
    STATE m_state;
    Ship* m_left;//the pointer to the left child in the BST
    Ship* m_right;//the pointer to the right child in the BST
    //int m_height;//the height of node in the BST
    bool m_color;   // Red-Black tree nodes don't store height or balance factors,
    // instead they store their color (RED or BLACK).
};
class Fleet{
public:
    friend class Grader;
    friend class Tester;
    Fleet();
    ~Fleet();
    void insert(const Ship& ship);
    void clear();
    void remove(int id);
    void dumpTree() const;
    void listShips() const;
    bool setState(int id, STATE state);
    void removeLost();//remove all lost ships from the tree
    bool findShip(int id) const;//returns true if the ship is in tree
private:
    Ship* m_root;//the root of the BST

    void dump(Ship* aShip) const;//helper for recursive traversal

    // A code that indicates what occured as the result of an insertion into a subtree.
    enum class INSERT_RESULT {
        NO_INSERT,          // No insertion was perform because of duplicate id.
        SAME_COLOR_SAME_HT, // Insertion performed, the root color did not change, the black
        // height did not change.
        B_TO_R_SAME_HT,     // Insertion performed, the root color changed from black to red,
        // the black height did not change.
        R_TO_B_HT_INC_LRED, // Insertion performed, the root color changed from red to black,
        // the black height increased by 1, and the left child is red.
        R_TO_B_HT_INC_RRED  // Insertion performed, the root color changed from red to black,
        // the black height increased by 1, and the right child is red.
    };

    INSERT_RESULT r_insert(const Ship& ship, Ship*& root);

    // A code that indicates what occured as the result of a removal from a subtree.
    enum class REMOVE_RESULT {
        NO_REMOVAL,         // No removal was perform because id wasn't found.
        R_BEF_SAME_HT_AFT,  // Removal performed, the root color was red before the removal,
        // the black height did not change.
        B_TO_B_SAME_HT,     // Removal performed, the root color was black and stayed black,
        // the black height did not change.
        B_TO_B_HT_DEC,      // Removal performed, the root color was black and stayed black,
        // the black height decreased by 1.
    };

    REMOVE_RESULT r_remove(int id, Ship*& root);
    REMOVE_RESULT remove_root(Ship*& root);
    REMOVE_RESULT r_remove_min(Ship*& root, Ship*& min_ship);
    REMOVE_RESULT rebalance_after_remove(Ship*& root, REMOVE_RESULT remove_result, bool taken_from_left); // todo

    void r_findLost(Ship* root, vector<int>& lost_ids);

    void r_clear(Ship*);
    void r_list(const Ship*) const;
    bool r_setState(int id, STATE state, Ship* root);
    bool r_findShip(int id, Ship*) const;
    Ship* right_rotate(Ship* temp);
    Ship* left_rotate(Ship* temp);

    bool verifyRB() const;                  // true iff entire tree is a valid RB tree.
    int r_verifyRB(const Ship* root) const;   // returns -1 if invalid RB tree, otherwise returns
    // the black height of the valid RB tree.
    bool r_verifyBST(Ship* ship, int& minid, int& maxid) const;
    bool is_red(const Ship* pship) const;   // true iff pship is red.
    bool is_black(const Ship* pship) const;   // true iff pship is black.

    int getSize() const;
    int r_getSize(Ship*) const;

    friend class Tester;
};

#endif
