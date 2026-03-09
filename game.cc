#include <iostream>
using namespace std;

class Arena{
    private:
        array <double, 2> arena_size;
    public:
        array <double, 2> getSize(){return arena_size;}
};