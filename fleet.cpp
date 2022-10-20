// Red Black Tree
#include "fleet.h"
Fleet::Fleet()
        :m_root(nullptr)
{
    //done
}
Fleet::~Fleet() {
    //done
    clear();
}
void Fleet::insert(const Ship& ship){
    // done
    r_insert(ship, m_root);
}

void Fleet::clear(){
    //done
    r_clear(m_root);
    m_root = nullptr; //setting pointer itself to null
}

void Fleet::remove(int id){
    //done
    r_remove(id, m_root);
}

void Fleet::dumpTree() const {
    dump(m_root);
}

void Fleet::dump(Ship* aShip) const{
    if (aShip != nullptr){
        cout << "(";
        dump(aShip->m_left);//first visit the left child
        //cout << aShip->m_id << ":" << aShip->m_height;//second visit the node itself
        cout << aShip->m_id << ":" << aShip->m_color;//second visit the node itself
        dump(aShip->m_right);//third visit the right child
        cout << ")";
    }
}

void Fleet::listShips() const {
    //done
    r_list(m_root);
}

bool Fleet::setState(int id, STATE state){
    //done
    return r_setState(id, state, m_root);
}

void Fleet::removeLost(){
    //done
    vector<int> lost_ids;
    r_findLost(m_root, lost_ids);
    for (int id : lost_ids) {
        remove(id);
    }
}

bool Fleet::findShip(int id) const {
    //done
    return r_findShip(id, m_root);
}

Fleet::INSERT_RESULT Fleet::r_insert(const Ship& ship, Ship*& root)
{
    if (root == nullptr) {  // base case
        root = new Ship(ship.getID(), ship.getType(), ship.getState());
        return INSERT_RESULT::B_TO_R_SAME_HT;
    }

    INSERT_RESULT insert_result;

    if (ship.getID() < root->getID()) {
        insert_result = r_insert(ship, root->m_left);
        if (insert_result == INSERT_RESULT::NO_INSERT ||
            insert_result == INSERT_RESULT::SAME_COLOR_SAME_HT) {
            return insert_result;
        }
        if (insert_result == INSERT_RESULT::B_TO_R_SAME_HT) {
            if (root->getColor() == RED) {  // Red violation
                root->setColor(BLACK);
                return INSERT_RESULT::R_TO_B_HT_INC_LRED;
            }
            return INSERT_RESULT::SAME_COLOR_SAME_HT;
        }
        if (insert_result == INSERT_RESULT::R_TO_B_HT_INC_LRED) {  // Black violation
            root->getLeft()->setColor(RED);
            root->getLeft()->getLeft()->setColor(BLACK);
        }
        if (insert_result == INSERT_RESULT::R_TO_B_HT_INC_RRED) {  // Black violation
            root->setLeft(left_rotate(root->getLeft()));
        }
        root = right_rotate(root);
        return INSERT_RESULT::B_TO_R_SAME_HT;
    }
    else if (root->getID() < ship.getID()) {
        insert_result = r_insert(ship, root->m_right);
        if (insert_result == INSERT_RESULT::NO_INSERT ||
            insert_result == INSERT_RESULT::SAME_COLOR_SAME_HT) {
            return insert_result;
        }
        if (insert_result == INSERT_RESULT::B_TO_R_SAME_HT) {
            if (root->getColor() == RED) {  // Red violation
                root->setColor(BLACK);
                return INSERT_RESULT::R_TO_B_HT_INC_RRED;
            }
            return INSERT_RESULT::SAME_COLOR_SAME_HT;
        }
        if (insert_result == INSERT_RESULT::R_TO_B_HT_INC_RRED) {  // Black violation
            root->getRight()->setColor(RED);
            root->getRight()->getRight()->setColor(BLACK);
        }
        if (insert_result == INSERT_RESULT::R_TO_B_HT_INC_LRED) {  // Black violation
            root->setRight(right_rotate(root->getRight()));
        }
        root = left_rotate(root);
        return INSERT_RESULT::B_TO_R_SAME_HT;
    }

    return INSERT_RESULT::NO_INSERT;   // duplicate id
}

Fleet::REMOVE_RESULT Fleet::r_remove(int id, Ship*& root)
{
    //done
    if (root == nullptr) {
        return REMOVE_RESULT::NO_REMOVAL;   // base case: id not found
    }
    if (id < root->getID()) {
        return rebalance_after_remove(root, r_remove(id, root->m_left), true);
    }
    if (root->getID() < id) {
        return rebalance_after_remove(root, r_remove(id, root->m_right), false);
    }
    return remove_root(root);   // base case: id found.
}

Fleet::REMOVE_RESULT Fleet::remove_root(Ship*& root)
{
    Ship* min_ship;
    REMOVE_RESULT remove_result;
    bool taken_from_left;

    if (root->getRight() == nullptr) {
        // Right subtree empty, replace root w/ left child.
        min_ship = root;
        root = min_ship->getLeft();
        min_ship->setLeft(nullptr);
        if (is_red(min_ship)) {
            remove_result = REMOVE_RESULT::R_BEF_SAME_HT_AFT;
        }
        else if (is_red(root)) {
            root->setColor(BLACK);
            remove_result = REMOVE_RESULT::B_TO_B_SAME_HT;
        }
        else {
            remove_result = REMOVE_RESULT::B_TO_B_HT_DEC;
        }
        delete min_ship;
        min_ship = nullptr;
        return remove_result;
    }

    // Right subtree non-empty. Remove min Ship from right subtree.
    remove_result = r_remove_min(root->m_right, min_ship);
    taken_from_left = false;

    // The min Ship is the inorder successor of the root. Replace
    // root with its successor.
    min_ship->setColor(root->getColor());
    min_ship->setLeft(root->getLeft());
    min_ship->setRight(root->getRight());
    root->setLeft(nullptr);
    root->setRight(nullptr);
    delete root;
    root = min_ship;
    min_ship = nullptr;

    return rebalance_after_remove(root, remove_result, taken_from_left);
}

Fleet::REMOVE_RESULT Fleet::r_remove_min(Ship*& root, Ship*& min_ship)
{
    if (root->getLeft() == nullptr) {   // Left subtree empty, root is min Ship.
        min_ship = root;
        root = min_ship->getRight();
        min_ship->setRight(nullptr);
        if (is_red(root)) {
            root->setColor(BLACK);
            return REMOVE_RESULT::B_TO_B_SAME_HT;
        }
        if (is_red(min_ship)) {
            return REMOVE_RESULT::R_BEF_SAME_HT_AFT;
        }
        return REMOVE_RESULT::B_TO_B_HT_DEC;
    }

    // Left subtree non-empty. min Ship is in left subtree.
    return rebalance_after_remove(root, r_remove_min(root->m_left, min_ship), true);
}

Fleet::REMOVE_RESULT Fleet::rebalance_after_remove(
        Ship*& root, REMOVE_RESULT remove_result, bool taken_from_left)
{
    if (remove_result == REMOVE_RESULT::NO_REMOVAL) {
        return REMOVE_RESULT::NO_REMOVAL;
        // If nothing was removed, no rebalance needed.
    }
    if (remove_result == REMOVE_RESULT::R_BEF_SAME_HT_AFT) {
        return REMOVE_RESULT::B_TO_B_SAME_HT;
        // If the child Ship was red before the removal, but there was no
        // change to black height after the removal, then no further rebalance
        // is needed. The parent Ship must be black because it had a red child
        // before the removal. Hence, parent stays black with the same black ht.
    }
    if (remove_result == REMOVE_RESULT::B_TO_B_SAME_HT) {
        // If there was no height or color change, then no further rebalance is
        // needed, but the return code is different depending on whether the parent
        // Ship is red or black.
        if (is_red(root)) {
            return REMOVE_RESULT::R_BEF_SAME_HT_AFT;

        }
        return REMOVE_RESULT::B_TO_B_SAME_HT;
    }

    // Having dealt with the first three cases, the only case left is
    // remove_result == REMOVE_RESULT::B_TO_B_HT_DEC
    if (taken_from_left) {
        if (is_red(root)) {
            if (is_red(root->getRight()->getLeft())) {
                // red parent, black sibling, red inner nephew
                root->setColor(BLACK);
                root->setRight(right_rotate(root->getRight()));
            }
            else {
                // red parent, black sibling, black inner nephew
            }
            root = left_rotate(root);
            return REMOVE_RESULT::R_BEF_SAME_HT_AFT;
        }
        if (is_red(root->getRight())) {
            // black parent, red sibling
            root = left_rotate(root);
            root->setColor(BLACK);
            Ship* nephew = root->getLeft()->getRight();
            if (is_black(nephew->getRight())) {
                if (is_black(nephew->getLeft())) {
                    nephew->setColor(RED);
                }
                else {
                    root->getLeft()->setRight(right_rotate(root->getLeft()->getRight()));
                    root->setLeft(left_rotate(root->getLeft()));
                }
            }
            else if (is_black(nephew->getLeft())) {
                root->getLeft()->setColor(RED);
                root->setLeft(left_rotate(root->getLeft()));
            }
            else {
                root->setLeft(left_rotate(root->getLeft()));
                root->getLeft()->setColor(RED);
                root->getLeft()->getRight()->setColor(BLACK);
            }
            nephew = nullptr;
        }
        else {
            // black parent, black sibling
            if (is_black(root->getRight()->getRight())) {
                if (is_black(root->getRight()->getLeft())) {
                    // black parent, black sibling, black inner nephew, black outer nephew
                    root->getRight()->setColor(RED);
                    return REMOVE_RESULT::B_TO_B_HT_DEC;
                }
                // black parent, black sibling, red inner nephew, black outer nephew
                root->getRight()->getLeft()->setColor(BLACK);
                root->setRight(right_rotate(root->getRight()));
            }
            else {
                // black parent, black sibling, red outer nephew
                root->getRight()->getRight()->setColor(BLACK);
            }
            root = left_rotate(root);
        }
    }
    else {  // Removed Ship was taken from the right subtree
        if (is_red(root)) {
            if (is_red(root->getLeft()->getRight())) {
                // red parent, black sibling, red inner nephew
                root->setColor(BLACK);
                root->setLeft(left_rotate(root->getLeft()));
            }
            else {
                // red parent, black sibling, black inner nephew
            }
            root = right_rotate(root);
            return REMOVE_RESULT::R_BEF_SAME_HT_AFT;
        }
        if (is_red(root->getLeft())) {
            // black parent, red sibling
            root = right_rotate(root);
            root->setColor(BLACK);
            Ship* nephew = root->getRight()->getLeft();
            if (is_black(nephew->getLeft())) {
                if (is_black(nephew->getRight())) {
                    nephew->setColor(RED);
                }
                else {
                    root->getRight()->setLeft(left_rotate(root->getRight()->getLeft()));
                    root->setRight(right_rotate(root->getRight()));
                }
            }
            else if (is_black(nephew->getRight())) {
                root->getRight()->setColor(RED);
                root->setRight(right_rotate(root->getRight()));
            }
            else {
                root->setRight(right_rotate(root->getRight()));
                root->getRight()->setColor(RED);
                root->getRight()->getLeft()->setColor(BLACK);
            }
            nephew = nullptr;
        }
        else {
            // black parent, black sibling
            if (is_black(root->getLeft()->getLeft())) {
                if (is_black(root->getLeft()->getRight())) {
                    // black parent, black sibling, black inner nephew, black outer nephew
                    root->getLeft()->setColor(RED);
                    return REMOVE_RESULT::B_TO_B_HT_DEC;
                }
                // black parent, black sibling, red inner nephew, black outer nephew
                root->getLeft()->getRight()->setColor(BLACK);
                root->setLeft(left_rotate(root->getLeft()));
            }
            else {
                // black parent, black sibling, red outer nephew
                root->getLeft()->getLeft()->setColor(BLACK);
            }
            root = right_rotate(root);
        }
    }

    return REMOVE_RESULT::B_TO_B_SAME_HT;
}

void Fleet::r_findLost(Ship* root, vector<int>& lost_ids) {
    // done
    if (root == nullptr) {
        return;
    }
    r_findLost(root->getLeft(), lost_ids);
    if (root->getState() == LOST) {
        lost_ids.push_back(root->getID());
    }
    r_findLost(root->getRight(), lost_ids);
}

void Fleet::r_clear(Ship* temp) {
    //done
    if (temp == nullptr) {
        return;
    }
    r_clear(temp->getLeft()); //recursive call
    temp->setLeft(nullptr); //set ptr to null
    r_clear(temp->getRight()); //recursive call
    temp->setRight(nullptr); //set ptr to null
    delete temp;
    temp = nullptr; //keeping valgrind happy
}

void Fleet::r_list(const Ship* temp) const {
    //done
    if (temp == nullptr) { //base case
        return;
    }
    r_list(temp->getLeft());
    cout << endl << temp->getID() << ":" << temp->getStateStr() << ":" << temp->getTypeStr();
    r_list(temp->getRight());
}

bool Fleet::r_setState(int id, STATE state, Ship* root) {
    //done
    if (root == nullptr) {
        return false;
    }
    if (id < root->getID()) {
        return r_setState(id, state, root->getLeft());
    }
    if (id > root->getID()) {
        return r_setState(id, state, root->getRight());
    }
    root->setState(state);
    return true;
}

bool Fleet::r_findShip(int id, Ship* temp) const {
    //done
    if (temp == nullptr) {
        return false;
    }
    if (id < temp->getID()) {
        return r_findShip(id, temp->getLeft());
    }
    if (temp->getID() < id) {
        return r_findShip(id, temp->getRight());
    }
    return true;
}

Ship* Fleet::right_rotate(Ship* temp) {
    //done
    Ship* ptr = temp;
    temp = ptr->getLeft();
    ptr->setLeft(temp->getRight());
    temp->setRight(ptr);
    ptr = nullptr;
    return temp;
}

Ship* Fleet::left_rotate(Ship* temp) {
    //done
    Ship* ptr = temp;
    temp = ptr->getRight();
    ptr->setRight(temp->getLeft());
    temp->setLeft(ptr);
    ptr = nullptr;
    return temp;
}

bool Fleet::verifyRB() const
{
    //done
    int min_id, max_id;
    return r_verifyRB(m_root) && r_verifyBST(m_root, min_id, max_id);
}

int Fleet::r_verifyRB(const Ship* root) const
{
    if (root == nullptr) {
        return 1;
    }

    const Ship* pl = root->getLeft();
    const Ship* pr = root->getRight();
    int lb_ht = r_verifyRB(pl);
    int rb_ht = r_verifyRB(pr);

    if (lb_ht < 0 || rb_ht < 0 || lb_ht != rb_ht) {
        return -1;  // black violation
    }

    if (is_red(root) && (is_red(pl) || is_red(pr))) {
        return -1;  // red violation
    }

    if (is_red(root)) {
        return lb_ht;
    }
    return lb_ht + 1;
}

bool Fleet::r_verifyBST(Ship* ship, int& minid, int& maxid) const
{
    //done
    // base cases: empty tree, left node, internal node with one child
    if (ship == nullptr) {
        return true;
    }
    Ship* pLeft = ship->getLeft();
    Ship* pRight = ship->getRight();
    int id = ship->getID();
    if (pLeft == nullptr && pRight == nullptr) {
        minid = id;
        maxid = id;
        return true;
    }
    if (pLeft == nullptr && pRight != nullptr) {
        minid = id;
        maxid = pRight->getID();
        return minid < maxid;
    }
    if (pLeft != nullptr && pRight == nullptr) {
        minid = pLeft->getID();
        maxid = id;
        return minid < maxid;
    }

    int maxidL, minidR;
    if (!r_verifyBST(pLeft, minid, maxidL)
        || !r_verifyBST(pRight, minidR, maxid)) {
        return false;
    }
    if (id <= maxidL) {
        return false;
    }
    if (minidR <= id) {
        return false;
    }
    return true;
}

bool Fleet::is_red(const Ship* pship) const {
    //done
    return (pship != nullptr && pship->getColor() == RED);
}

bool Fleet::is_black(const Ship* pship) const {
    //done
    return (pship == nullptr || pship->getColor() == BLACK);
}

int Fleet::getSize() const {
    //done
    return r_getSize(m_root);
}

int Fleet::r_getSize(Ship* ship) const {
    //done
    if (ship == nullptr) {
        return 0;
    }
    return 1 + r_getSize(ship->getLeft()) + r_getSize(ship->getRight());
}
