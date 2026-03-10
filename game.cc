#include <iostream>
using namespace std;

class Arena{
    private:
        double arena_size_x;
        double arena_size_y;
    public:
        double getSize_x(){return arena_size_x;}
        double getSize_y(){return arena_size_y;}
};