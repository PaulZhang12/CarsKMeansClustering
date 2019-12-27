//Author: Letian Zhang
//Uniqname: paulzhan
//Date: 12/20/2019
//Lab Section: 101
//Project: #6




#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include<cmath>

using namespace std;


template <class T> void printVector(vector<T> &list);
double calculateDistance(vector<double> &list, vector<double> &centroid);
const int K = 7;

int main(int argc, char ** argv){

    ifstream csv ("clusters.csv");
    string line;
    
    vector<vector<double>> centroidLocation;
    vector<double> car;

    for (int i=1; i < 8; i++){ 
        car.push_back(atof(argv[i])); 
    }

    getline(csv, line);
    int pos = 0;
    string delimiter = ",";

    while(getline(csv, line)){
        vector<double> entry;
        for(int i = 0; i < K; i++){
            pos = line.find(delimiter);
            entry.push_back(stod(line.substr(0, pos)));
            line.erase(0, pos+1);
        }
        centroidLocation.push_back(entry);
    }

    double distance = calculateDistance(car, centroidLocation.at(0));
    int indexOfMax = 0;
    for(int i = 1; i < 3; i++){
        if(calculateDistance(car, centroidLocation.at(i)) < distance){
            distance = calculateDistance(car, centroidLocation.at(i));
            indexOfMax = i;
        }
    }

    cout << "Mystery Car is type " << indexOfMax+1 << ".";
}


template <class T> void printVector(vector<T> &list){
    for(int i = 0; i < list.size(); i++){
        cout << list.at(i) << ",";
    }
    cout << endl;
}

double calculateDistance(vector<double> &list, vector<double> &centroid){
    double d = 0;

    for(int i = 0; i < 7; i++){
        d += pow((list.at(i)-centroid.at(i)), 2);
    }
    return sqrt(d);
}