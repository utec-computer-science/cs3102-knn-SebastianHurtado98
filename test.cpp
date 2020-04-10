#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <vector>
#include <time.h>
#include <chrono>
#include <stdio.h>
#include <unistd.h>
#define THREADS 4

using namespace std;

void mess(vector<int> x, int tn)
{
    int delta = x.size()/THREADS;
    int start = tn * delta;
    int end = ((tn+1) * delta) -1;
    for(int i = start; i <end; i++){
        x[i] = x[i] * x[i];
    }
}

int main() 
{
    srand(time(NULL));
    
    //crea un vector largo y partelo. Haz multiplicaciones
    //o algo pesado en el, revisa hasta cuanto aguanta.
    thread ts[THREADS];
    vector<int> x;

    for (int i =0; i < 100; i++) {
        x[i] = rand()%50;
    }
    for(int i=0; i < 10; i++) {
        cout << "N" << i << ": " << x[i] << endl;
    }
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < THREADS; i++){
        ts[i] = thread(mess, x, i);
    }

    for (int i = 0; i < THREADS; i++){
        ts[i].join();
    }

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop-start);

    cout <<"Time with " << THREADS << "threads: " << duration.count() << endl;
    
    for(int i=0; i < 10; i++) {
        cout << "N" << i << ": " << x[i] << endl;
    }
  return 0;
}