#include <iostream>
#include <fstream>
#include <cstdlib> 
#include <vector>
#include <map>
#include <queue>
#include <limits.h>
#include <math.h>
#include <sys/time.h>
using std::ifstream;

std::pair<int,int> goal = {8,0}; //8,0
std::pair<int,int> start = {7,12}; //7,12
std::vector<std::vector<int>> maze;

std::vector<std::vector<int>> llenarLaberinto(){
    ifstream indata; 
    indata.open("Practica 1 Maze.txt");
    std::vector<std::vector<int>> maze;

    for(int i = 0 ; i<9; i++){
        std::vector<int> temp;
        for(int j = 0; j<15; j++){
            int num;
            indata>>num;
            temp.push_back(num);
        }
        maze.push_back(temp);
    }
    indata.close();
    return maze;
}

std::map<std::pair<int,int>, int> llenarCostos(){
    std::map<std::pair<int,int>, int> costo;
    for(int i = 0; i<9; i++){
        for(int j = 0; j<15; j++){
            std::pair<int,int> temp = {i,j};
            costo[temp] = INT_MAX;
        }
    }
    return costo;
}

void generarHijoUniforme(std::pair<int,int>& hijo, std::pair<int,int>& actual, auto& cola, std::map<std::pair<int,int>, int>& costo, std::map<std::pair<int,int>, std::pair<int,int>>& parents ){
    if(hijo.first<0|| hijo.first>8 || hijo.second>14 || hijo.second<0){
        return;
    }

    int gN = costo[actual] + maze[hijo.first][hijo.second];
    std::cout<<"("<<hijo.first<<", "<< hijo.second<<") Costo: g(n) = " << gN <<"\n";

    if(costo[hijo] > gN && parents[actual] != hijo){
        costo[hijo] = gN;
        cola.push({gN,hijo});   
        parents[hijo] = actual;
    }
}

void costoUniforme(std::vector<std::vector<int>> maze, std::pair<int,int> start, std::pair<int,int> goal){
    std::map<std::pair<int,int>, int> costo = llenarCostos();
    std::map<std::pair<int,int>, std::pair<int,int>> parents;
    
    std::priority_queue <std::pair<int,std::pair<int,int>>, std::vector<std::pair<int,std::pair<int,int>>>, std::greater<std::pair<int,std::pair<int,int>>>> cola;

    cola.push({costo[start],start});
    costo[start] = 0;

    while(!cola.empty()){
        std::pair<int,int> actual = cola.top().second;
        cola.pop();

        std::cout<<"\nNODO ACTUAL: ("<<actual.first<<", "<< actual.second<<")"<<" Costo: " <<  costo[actual]<<"\n";
        std::cout<<"***********************HIJOS*****************************\n";
        
        if(actual == goal){
            break;
        }

        std::pair<int,int> arriba = {actual.first-1, actual.second};
        std::pair<int,int> abajo = {actual.first+1, actual.second};
        std::pair<int,int> derecha = {actual.first, actual.second+1};
        std::pair<int,int> izquierda = {actual.first, actual.second-1};

        generarHijoUniforme(arriba, actual, cola, costo, parents);
        generarHijoUniforme(abajo, actual, cola, costo, parents);
        generarHijoUniforme(derecha, actual, cola, costo, parents);
        generarHijoUniforme(izquierda, actual, cola, costo, parents);
    }

    std::cout<<"\nCosto TOTAl Uniforme: " << costo[goal]<<"\n";
    while(goal != start){
        std::cout<<"Costo: "<<costo[goal]<<" ("<<goal.first<< ", "<< goal.second<< ") -> \n"; 
        goal = parents[goal]; 
    }
    std::cout<<"("<< start.first<<" "<< start.second<<")\n"; 
}

void generarHijoAStar(std::pair<int,int>& hijo, std::pair<int,int>& actual, auto& cola, std::map<std::pair<int,int>, int>& costo, std::map<std::pair<int,int>, std::pair<int,int>>& parents ){
    if(hijo.first<0|| hijo.first>8 || hijo.second>14 || hijo.second<0){
        return;
    }

    int hN = abs(hijo.first - goal.first) + abs(hijo.second - goal.second);
    int gN = costo[actual] + maze[hijo.first][hijo.second];
    std::cout<<"("<<hijo.first<<", "<< hijo.second<<") Costo: f(n) = " << gN <<" + "<< hN <<" = "<< gN+ hN<<"\n";

    if(costo[hijo] > gN && parents[actual] != hijo){
        costo[hijo] = gN;
        cola.push({gN+hN,hijo});   
        parents[hijo] = actual;
    }
}

void aStar(std::vector<std::vector<int>> maze, std::pair<int,int> start, std::pair<int,int> goal){
    std::map<std::pair<int,int>, int> costo = llenarCostos();
    std::map<std::pair<int,int>, std::pair<int,int>> parents;
    
    std::priority_queue <std::pair<int,std::pair<int,int>>, std::vector<std::pair<int,std::pair<int,int>>>, std::greater<std::pair<int,std::pair<int,int>>>> cola;
    costo[start] = 0;
    cola.push({costo[start],start});
    

    while(!cola.empty()){
        std::pair<int,int> actual = cola.top().second;
        cola.pop();

        int gN = costo[actual];
        int hN = abs(actual.first - goal.first) + abs(actual.second - goal.second);

        std::cout<<"\nNODO ACTUAL: ("<<actual.first<<", "<< actual.second<<")"<<" Costo: " << gN <<" + "<< hN <<" = "<< gN+hN <<"\n";
        std::cout<<"***********************HIJOS*****************************\n";
        
        if(actual == goal){
            break;
        }

        std::pair<int,int> arriba = {actual.first-1, actual.second};
        std::pair<int,int> abajo = {actual.first+1, actual.second};
        std::pair<int,int> derecha = {actual.first, actual.second+1};
        std::pair<int,int> izquierda = {actual.first, actual.second-1};

        generarHijoAStar(arriba, actual, cola, costo, parents);
        generarHijoAStar(abajo, actual, cola, costo, parents);
        generarHijoAStar(derecha, actual, cola, costo, parents);
        generarHijoAStar(izquierda, actual, cola, costo, parents);
    }

    std::cout<<"\nCosto TOTAl A*: " << costo[goal]<<"\n";
    while(goal != start){
        std::cout<<"Costo: "<<costo[goal]<<" ("<<goal.first<< ", "<< goal.second<< ") -> \n"; 
        goal = parents[goal]; 
    }
    std::cout<<"("<< start.first<<" "<< start.second<<")\n";
}


int main(){

    struct timeval startTime, stop;
    double secs = 0;
    maze = llenarLaberinto();

    
    gettimeofday(&startTime, NULL);
    costoUniforme(maze, start, goal);

    gettimeofday(&stop, NULL);
    secs = (double)(stop.tv_usec - startTime.tv_usec) / 1000000 + (double)(stop.tv_sec - startTime.tv_sec);
    printf("\nTiempo de Cálculo Algoritmo Uniforme: %f\n", secs);
    
    std::cout<<"\n\n";
    gettimeofday(&startTime, NULL);
    aStar(maze, start, goal);

    gettimeofday(&stop, NULL);
    secs = (double)(stop.tv_usec - startTime.tv_usec) / 1000000 + (double)(stop.tv_sec - startTime.tv_sec);
    printf("\nTiempo de Cálculo Algoritmo A*: %f\n", secs);
    
}