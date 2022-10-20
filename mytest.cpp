#include "fleet.h"
#include <random>
#include <chrono>
#include <vector>
using namespace std::chrono;
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else{ //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two decimal points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }

private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};
class Tester{
public:
    /******************************************
    * Test function declarations go here! *
    ******************************************/
    bool insert_normal();
    bool insert_edge();
    bool insert_error();
    bool test_remove();
    bool test_remove_edge();
    bool test_balance();
    bool test_removeLost();
    void test_clock(int start, int stop, int key);
};

int main(){
    Tester tester;
    cout << "Testing Normal Insert: " << endl;
    tester.insert_normal();
    cout << endl << "Testing Edge Insert: " << endl;
    tester.insert_edge();
    cout << endl << "Testing Error Insert: " << endl;
    tester.insert_error();
    cout << endl << "Testing Remove Function: " << endl;
    tester.test_remove();
    cout << endl << "Testing Edge Remove Function: " << endl;
    tester.test_remove_edge();
    cout << endl << "Testing Balance Function: " << endl;
    tester.test_balance();
    cout << endl << "Testing Remove Lost Function" << endl;
    tester.test_removeLost();

    //Checking Clock Insert
    long long arr1[4];
    auto start = high_resolution_clock::now();
    tester.test_clock(40000, 1000, 1);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "1000 Insertions: " << duration.count() << endl;
    arr1[0] = duration.count();
    start = high_resolution_clock::now();
    tester.test_clock(40000, 2000, 1);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "2000 Insertions: " << duration.count() << endl;
    arr1[1] = duration.count();
    start = high_resolution_clock::now();
    tester.test_clock(40000, 4000, 1);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "4000 Insertions: " << duration.count() << endl;
    arr1[2] = duration.count();
    start = high_resolution_clock::now();
    tester.test_clock(40000, 8000, 1);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "8000 Insertions: " << duration.count() << endl;
    arr1[3] = duration.count();
    cout << "Upon graphing these values, we see the curve is roughly linearithmic meaning that "
            "a single insert is O(log n)." << endl;
    //Checking Clock Removal
    start = high_resolution_clock::now();
    tester.test_clock(40000, 1000, 2);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "1000 Removals: " << duration.count() << endl;
    start = high_resolution_clock::now();
    tester.test_clock(40000, 2000, 2);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "2000 Removals: " << duration.count() << endl;
    start = high_resolution_clock::now();
    tester.test_clock(40000, 4000, 2);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "4000 Removals: " << duration.count() << endl;
    start = high_resolution_clock::now();
    tester.test_clock(40000, 8000, 2);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "8000 Removals: " << duration.count() << endl;
    cout << "Upon graphing these values, we see the curve is roughly linearithmic meaning that a "
            "single removal is O(log n)." << endl;
    return 0;
}

bool Tester::insert_normal(){
    // Insert ships in random ID order
    Fleet fleet;
    int fleetSize = 300;
    Random randomIDmaker(MINID, MAXID);
    while (fleet.getSize() < fleetSize) {
        Ship ship(randomIDmaker.getRandNum(), CARGO, ALIVE);
        fleet.insert(ship);
        if (!fleet.verifyRB()) {
            return false;
        }
    }
    //fleet.dumpTree();
//    fleet.listShips();
    cout << "Successful" << endl;
    return fleet.verifyRB();
}

bool Tester::insert_edge(){
    // Insert ships already in increasing ID order
    Fleet fleet;
    int fleetSize = 300;
    int tempID = 0;
    int ID = 50000;
    for (int i = 0; i < fleetSize; i++) {
        Ship ship(ID, CARGO, ALIVE);
        ID += 1;
        fleet.insert(ship);
        if (!fleet.verifyRB()) {
            return false;
        }
    }
    //    fleet.dumpTree();
    //    fleet.listShips();
    cout << "Successful" << endl;
    return fleet.getSize() == fleetSize && fleet.verifyRB();
}
bool Tester::insert_error(){
    Fleet fleet;
    Ship badid(-1, CARGO, LOST);
    Ship dupe1(MINID, CARGO, LOST);
    Ship dupe2(MINID, CARGO, LOST);
    fleet.insert(badid);
    if (fleet.getSize() != 0 || !fleet.verifyRB()) {
        return false;
    }
    fleet.insert(dupe1);
    if (fleet.getSize() != 1 || !fleet.verifyRB()) {
        return false;
    }
    fleet.insert(dupe2);
    if (fleet.getSize() != 1 || !fleet.verifyRB()) {
        return false;
    }
//    fleet.dumpTree();
//    fleet.listShips();
    return true;
}
bool Tester::test_remove(){
    Fleet fleet;
    vector<int> ids;
    int ID = 40000;
    int fleet_size = 300;
    for (int i = 0; i < fleet_size; i++){
        Ship ship(ID, CARGO, ALIVE);
        fleet.insert(ship);
        ids.push_back(ID);
        ID++;
    }
    if (fleet.getSize() != 300 || !fleet.verifyRB()) {
        return false;
    }
    random_shuffle(ids.begin(), ids.end());
    ID = 300;
    for (int id : ids) {
        fleet.remove(id);
        ID--;
        if (fleet.getSize() != ID || !fleet.verifyRB()) {
            return false;
        }
    }
    cout << "Successful" << endl;
    return true;
}

bool Tester::test_remove_edge()
{
    Fleet fleet;
    int ID = 40000;
    int fleet_size = 10;
    // Insert 10 ships
    for (int i = 0; i < fleet_size; i++) {
        Ship ship(ID, CARGO, ALIVE);
        fleet.insert(ship);
        ID++;
    }
    if (fleet.getSize() != 10 || !fleet.verifyRB()) {
        return false;
    }
    // Remove all of them, each time removing whichever ship is the root
    while (fleet_size > 0) {
        ID = fleet.m_root->getID();
        fleet.remove(ID);
        fleet_size--;
        if (fleet.getSize() != fleet_size || fleet.findShip(ID) || !fleet.verifyRB()) {
            cout << "Removal Failed!" << endl;
            return false;
        }
    }
    cout << "Successful" << endl;
    return true;
}

bool Tester::test_balance(){
    Fleet fleet;
    int fleet_size = 300;
    int ID = 40000;
    for (int i = 0; i < fleet_size; i++){
        Ship ship(ID, CARGO, ALIVE);
        fleet.insert(ship);
        if (!fleet.verifyRB()){
            cout << "Error, Tree not valid Red-Black" << endl;
            return false;
        }
        ID++;
    }
    cout << "Tree is valid Red-Black!" << endl;
    return fleet.verifyRB();
}

bool Tester::test_removeLost(){
    Fleet fleet;
    for (int i = 40000; i < 40020; i++){
        Ship ship(i, CARGO, ALIVE);
        fleet.insert(ship);
    }
    fleet.setState(40000, LOST);
    fleet.setState(40005, LOST);
    fleet.setState(40007, LOST);
    fleet.setState(40008, LOST);
    fleet.setState(40012, LOST);
    fleet.removeLost();

    if (fleet.findShip(40000) ||
        fleet.findShip(40005) ||
        fleet.findShip(40007) ||
        fleet.findShip(40008) ||
        fleet.findShip(40012) ||
        fleet.getSize() != 15 ||
        !fleet.verifyRB()) {
        return false;
    }

    return true;
}

//tree of 1000
//clock time
//repeat with 2000, 4000, 8000
//see how time duration increases and
// check if its a logarithmic curve
void Tester::test_clock(int start, int stop, int key){
    Fleet fleet;
    if (key == 1){
        for (int i = start; i < start + stop; i++){
            Ship ship(i, CARGO, ALIVE);
            fleet.insert(ship);
        }
        return;
    }
    if (key == 2){
        for (int i = start; i < start + stop; i++){
            Ship ship(i, CARGO, ALIVE);
            fleet.insert(ship);
        }
        for (int i = start; i < start + stop; i++){
            fleet.remove(i);
        }
    }
}



