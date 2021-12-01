#include <iostream>
#include <fstream>
#include <cstdlib> 
#include <vector>
#include <map>
#include <queue>
#include <limits.h>
#include <math.h>
#include <sys/time.h>
#include <stack>
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

void generarRutaUniforme(std::map<std::pair<int,int>, std::pair<int,int>>& parents, std::map<std::pair<int,int>, int>& costo ){
    std::stack<std::pair<int,int>> orden;
    while(goal != start){
        std::pair<int,int> temp = goal;
        orden.push(temp);
        goal = parents[goal];
    }
    orden.push(start);
    
    std::cout<<"\n********************RUTA OPTIMA**************************\n";
    int count = 1;
    while(!orden.empty()){
         std::cout<<count++<<". Costo: "<<costo[orden.top()]<<"  Nodo: ("<<orden.top().first<< ", "<< orden.top().second<< ") -> \n"; 
         orden.pop();
    }
    std::cout<<"FIN";
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

        if(actual == goal){
            break;
        }

        std::cout<<"***********************HIJOS*****************************\n";

        std::pair<int,int> arriba = {actual.first-1, actual.second};
        std::pair<int,int> abajo = {actual.first+1, actual.second};
        std::pair<int,int> derecha = {actual.first, actual.second+1};
        std::pair<int,int> izquierda = {actual.first, actual.second-1};

        generarHijoUniforme(arriba, actual, cola, costo, parents);
        generarHijoUniforme(abajo, actual, cola, costo, parents);
        generarHijoUniforme(derecha, actual, cola, costo, parents);
        generarHijoUniforme(izquierda, actual, cola, costo, parents);
    }

    generarRutaUniforme(parents,costo);
}

void generarRutaAStar(std::map<std::pair<int,int>, std::pair<int,int>>& parents, std::map<std::pair<int,int>, int>& costo ){
    std::stack<std::pair<int,int>> orden;
    std::pair<int,int> tempGoal = goal;
    while(tempGoal != start){
        std::pair<int,int> temp = tempGoal;
        orden.push(temp);
        tempGoal = parents[tempGoal];
    }
    orden.push(start);
    
    std::cout<<"\n********************RUTA OPTIMA**************************\n";
    int count = 1;
    while(!orden.empty()){
        int hN = abs(orden.top().first - goal.first) + abs(orden.top().second - goal.second);
        int gN = costo[orden.top()];
        std::cout<<count++<<". Costo: "<<gN<<" + "<<hN<<" = "<< gN+hN<<"  Nodo: ("<<orden.top().first<< ", "<< orden.top().second<< ") -> \n"; 
        orden.pop();
    }
    std::cout<<"FIN";
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

    generarRutaAStar(parents,costo);
}


int main(){
    struct timeval startTime, stop;
    double secs = 0;
    maze = llenarLaberinto();

    //Costo Uniforme
    gettimeofday(&startTime, NULL);
    costoUniforme(maze, start, goal);

    gettimeofday(&stop, NULL);
    secs = (double)(stop.tv_usec - startTime.tv_usec) / 1000000 + (double)(stop.tv_sec - startTime.tv_sec);
    printf("\n\nTiempo de Cálculo Algoritmo Uniforme: %f\n", secs);

    /* //A*
    gettimeofday(&startTime, NULL);
    aStar(maze, start, goal);

    gettimeofday(&stop, NULL);
    secs = (double)(stop.tv_usec - startTime.tv_usec) / 1000000 + (double)(stop.tv_sec - startTime.tv_sec);
    printf("\n\nTiempo de Cálculo Algoritmo A*: %f\n", secs);
    */
}