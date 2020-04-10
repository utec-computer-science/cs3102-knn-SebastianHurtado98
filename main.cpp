#include <iostream>
#include <vector>
#include <time.h>
#include <chrono>
#include <stdio.h>
#include <unistd.h>
#include <algorithm>
#include <math.h>
#include <thread>
using namespace std;
#define THREADS 4
#define size 16000

template <typename T>
class CartesianCoord {
    public: 
    typedef T value_t;
    value_t x;
    value_t y;
    value_t d;

    public:
    CartesianCoord(value_t _x=0, value_t _y=0) : x(_x),y(_y){}
    ~CartesianCoord(void){}

    template<typename _T>
    friend ostream& operator<<(ostream &out, const CartesianCoord<_T> c){
        out << "(" << c.x << "," << c.y <<")";
        return out;
    }
};

typedef CartesianCoord<int> coord_t;
bool operator <(coord_t x, coord_t y) {
    return x.d < y.d ? true : false;
}



void knn (int k, vector<coord_t> &points, const coord_t &q, int id, vector<coord_t> &result) {


    int beg = (size/THREADS)*id;
    int end = ((size/THREADS)*(id+1)) -1;
    for (int i = beg; i < end; i++) {
        points[i].d = sqrt((points[i].x - q.x) * (points[i].x - q.x) + (points[i].y - q.y) * (points[i].y - q.y));
    }

    sort(points.begin(), points.begin() + k);
    for (int i = 0; i < k; i++) {
        result[i] = points[beg+i];
    }
}





int main() {
    
    srand(time(NULL));
    vector<coord_t> points;
    vector<coord_t> result;
    for (int i = 0; i < THREADS*3; i++) result.push_back(0);

    for (int i=0; i <size; i++) {
        points.push_back(coord_t(rand()%1000, rand()%1000));
    }

    vector<coord_t>::iterator it = points.begin();

    for(; it != points.end(); it++) {
        fflush(stdout);
        cout << "\r" << *it;
        usleep(2000);
    }
    cout << endl;

    vector<coord_t> knns;

    auto my_point = coord_t(100,200);

    auto start = chrono::high_resolution_clock::now();
    thread myThreads[THREADS];

    for (int i = 0; i < THREADS; i++) {
        myThreads[i] = thread(knn, 3, ref(points), my_point, i, ref(result));          
    }
    
    for(int i = 0; i < THREADS; i++){
        myThreads[i].join();
    }

    sort(result.begin(), result.end());

    auto stop = chrono::high_resolution_clock::now();

    for (int i = 0; i < 3; i++) knns.push_back(result[i]);

    auto duration = chrono::duration_cast<chrono::microseconds>(stop-start);
    cout << "Time: " << endl;
    cout << duration.count() << endl;

    cout << "knns" << endl;
    vector<coord_t>::iterator kit = knns.begin();
    for (; kit != knns.end(); kit++) {
        cout << *kit << endl;
    }
}