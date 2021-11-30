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

void costoUniforme(std::vector<std::vector<int>> maze, std::pair<int,int> start, std::pair<int,int> goal){
    std::map<std::pair<int,int>, int> costo = llenarCostos();
    std::map<std::pair<int,int>, std::pair<int,int>> parents;
    
    std::priority_queue <std::pair<int,std::pair<int,int>>, std::vector<std::pair<int,std::pair<int,int>>>, std::greater<std::pair<int,std::pair<int,int>>>> cola;

    cola.push({costo[start],start});
    costo[start] = 0;

    while(!cola.empty()){
        std::pair<int,int> actual = cola.top().second;
        if(actual == goal){
            break;
        }
        cola.pop();

        std::pair<int,int> arriba = {actual.first-1, actual.second};
        std::pair<int,int> abajo = {actual.first+1, actual.second};
        std::pair<int,int> derecha = {actual.first, actual.second+1};
        std::pair<int,int> izquierda = {actual.first, actual.second-1};
        
        if(arriba.first >-1 && costo[arriba] > costo[actual] + maze[arriba.first][arriba.second] && parents[actual] != arriba){
            costo[arriba] = costo[actual] + maze[arriba.first][arriba.second];
            cola.push({costo[arriba],arriba});   
            parents[arriba] = actual;
        }
        if(abajo.first < 9 && costo[abajo] > costo[actual] + maze[abajo.first][abajo.second] && parents[actual] != abajo){
            costo[abajo] = costo[actual] + maze[abajo.first][abajo.second];
            cola.push({costo[abajo], abajo}); 
            parents[abajo] = actual;
        }

        if(derecha.second < 15 && costo[derecha] > costo[actual] + maze[derecha.first][derecha.second] && parents[actual] != derecha){
            costo[derecha] = costo[actual] + maze[derecha.first][derecha.second];
            cola.push({costo[derecha], derecha});
            parents[derecha] = actual;
        }
        if(izquierda.second > -1 && costo[izquierda] > costo[actual] + maze[izquierda.first][izquierda.second] && parents[actual] != izquierda){
            costo[izquierda] = costo[actual] + maze[izquierda.first][izquierda.second];
            cola.push({costo[izquierda], izquierda}); 
            parents[izquierda] = actual;
        }
    }

    std::cout<<"Costo TOTAl Uniforme: " << costo[goal]<<"\n";
    while(goal != start){
        std::cout<<"Costo: "<<costo[goal]<<" ("<<goal.first<< ", "<< goal.second<< ") -> \n"; 
        goal = parents[goal]; 
    }
    std::cout<<"("<< start.first<<" "<< start.second<<")\n"; 

}

void aStar(std::vector<std::vector<int>> maze, std::pair<int,int> start, std::pair<int,int> goal){
    std::map<std::pair<int,int>, int> costo = llenarCostos();
    std::map<std::pair<int,int>, std::pair<int,int>> parents;
    
    std::priority_queue <std::pair<int,std::pair<int,int>>, std::vector<std::pair<int,std::pair<int,int>>>, std::greater<std::pair<int,std::pair<int,int>>>> cola;

    cola.push({costo[start],start});
    costo[start] = 0;

    while(!cola.empty()){
        std::pair<int,int> actual = cola.top().second;
        if(actual == goal){
            break;
        }
        cola.pop();

        std::pair<int,int> arriba = {actual.first-1, actual.second};
        std::pair<int,int> abajo = {actual.first+1, actual.second};
        std::pair<int,int> derecha = {actual.first, actual.second+1};
        std::pair<int,int> izquierda = {actual.first, actual.second-1};
        
        if(arriba.first >-1){
            int fN = maze[arriba.first][arriba.second] + abs(arriba.first - goal.first) + abs(arriba.second - goal.second);
            if(costo[arriba] > costo[actual] + fN && parents[actual] != arriba){
                costo[arriba] = costo[actual] + fN;
                cola.push({costo[arriba],arriba});   
                parents[arriba] = actual;
            }
        }
        

        if(abajo.first < 9){
            int fN = maze[abajo.first][abajo.second] + abs(abajo.first - goal.first) + abs(abajo.second - goal.second);
            if(costo[abajo] > costo[actual] + fN && parents[actual] != abajo){
                costo[abajo] = costo[actual] + fN;
                cola.push({costo[abajo], abajo}); 
                parents[abajo] = actual;
                }
        }
        
        if(derecha.second < 15){
            int fN = maze[derecha.first][derecha.second] + abs(derecha.first - goal.first) + abs(derecha.second - goal.second);
            if(costo[derecha] > costo[actual] + fN && parents[actual] != derecha){
                costo[derecha] = costo[actual] + fN;
                cola.push({costo[derecha], derecha});
                parents[derecha] = actual;
            }
        }
        
        if(izquierda.second > -1){
            int fN = maze[izquierda.first][izquierda.second] + abs(izquierda.first - goal.first) + abs(izquierda.second - goal.second);
            if(costo[izquierda] > costo[actual] + fN && parents[actual] != izquierda){
                costo[izquierda] = costo[actual] + fN;
                cola.push({costo[izquierda], izquierda}); 
                parents[izquierda] = actual;
            }
        }
    }

    std::cout<<"Costo TOTAl A*: " << costo[goal]<<"\n";
    while(goal != start){
        std::cout<<"Costo: "<<costo[goal]<<" ("<<goal.first<< ", "<< goal.second<< ") -> \n"; 
        goal = parents[goal]; 
    }
    std::cout<<"("<< start.first<<" "<< start.second<<")\n";
}


int main(){

    struct timeval startTime, stop;
    double secs = 0;
    std::pair<int,int> goal = {8,0}; //8,0
    std::pair<int,int> start = {7,12}; //7,12
    std::vector<std::vector<int>> maze = llenarLaberinto();


    gettimeofday(&startTime, NULL);
    costoUniforme(maze, start, goal);

    gettimeofday(&stop, NULL);
    secs = (double)(stop.tv_usec - startTime.tv_usec) / 1000000 + (double)(stop.tv_sec - startTime.tv_sec);
    printf("\ntiempo de cálculo %f\n", secs);
    

    gettimeofday(&startTime, NULL);
    aStar(maze, start, goal);

    gettimeofday(&stop, NULL);
    secs = (double)(stop.tv_usec - startTime.tv_usec) / 1000000 + (double)(stop.tv_sec - startTime.tv_sec);
    printf("\ntiempo de cálculo %f\n", secs);

}