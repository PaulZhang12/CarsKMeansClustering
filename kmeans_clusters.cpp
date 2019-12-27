//Author: Letian Zhang
//Uniqname: paulzhan
//Date: 12/20/2019
//Lab Section: 101
//Project: #6



#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <math.h> //libraries

using namespace std; //namespace
double getMax(vector<vector<double>> list, int index);
double getMin(vector<vector<double>> list, int index);
void normalize(vector<vector<double>> &list, double max, double min, int index);
template <class T> void printVector(vector<T> &list);
double calculateDistance(vector<double> &list, vector<double> &centroid);
double calculateAverage(vector<vector<double>> &centroids, vector<vector<double>> &centroidLocation, int centroidIndex);
void outputFile(vector<vector<double>> &list, ofstream &o);
int getCentroidClosest(vector<double> &list, vector<vector<double>> &centroidLocation);
void unNormalize(vector<vector<double>> &list, vector<double> &maxValues, vector<double> &minValues, int index);
void outputParameters(vector<string> &list, ofstream &o);


const int K = 7; //number of features

int main(){
    srand(1); 
    ifstream csv ("cars.csv"); //input cars csv
    ofstream o1("cars_clustered.csv"); //output for putting each car in a cluster
    ofstream c1("clusters.csv"); //output file for the centroids
    
    string line, delimiter = ","; //delimiter for the csv file
    double epsilon = 1e-6; //error
    
    int pos = 0; 
    vector<string> parameters; //header
    getline(csv, line); 
    for(int i = 0; i < K; i++){ 
        pos = line.find(delimiter);
        parameters.push_back(line.substr(0, pos)); //get each word in the header
        line.erase(0, pos+1);
    }
    
    vector<vector<double>> cars; //main car vector, containing lists of cars with their features
    vector<vector<double>> centroidLocation; //the 3 centroids
    vector<double> maxValues; //max and min to normalize and denormalize
    vector<double> minValues;

    while(getline(csv, line)){ //for each line in the csv file
        vector<double> entry;
        for(int i = 0; i < K; i++){
            pos = line.find(delimiter);
            entry.push_back(stod(line.substr(0, pos)));
            line.erase(0, pos+1);
        }
        cars.push_back(entry); //put the vector of features into the car vector
    }

    
    double max, min;
    for(int i = 0; i < K; i++){
        max = getMax(cars, i); //max and min for normalization
        min = getMin(cars, i);
        normalize(cars, max, min, i);

        maxValues.push_back(max); //keep track for unnormalizing
        minValues.push_back(min);
    }

    int randomIndex; //random car in cars
    for(int i = 0; i < 3; i++){
        double ran = static_cast<double>(rand())/RAND_MAX; //calculating the random
        randomIndex = static_cast<int>(ran*cars.size()); //get a distinct index for the cars vector
        centroidLocation.push_back(cars.at(randomIndex));
    }


    double error; //used to measure how much the centroids change
    vector<vector<double>> centroid1; //cars belonging to cluster 1
    vector<vector<double>> centroid2; //cars belonging to cluster 2
    vector<vector<double>> centroid3; //cars belonging to cluster 3
    vector<int> clusters; //The different clusters that each car ends up being
    //this ends up being how we know what gets appended to each car line

    do{
        centroid1.clear(); //make these vectors empty for each iteration
        centroid2.clear(); //as the centroid locations are updated
        centroid3.clear();
        clusters.clear();

        error = 0; //reset error

        int cluster;
        for(int i = 0; i < cars.size(); i++){
            cluster = getCentroidClosest(cars.at(i), centroidLocation); //determine which centroid the current car is closest to
            switch (cluster){ //based on that cluster, which centroid vector do I add the car to
                case 0:
                centroid1.push_back(cars.at(i));
                break;
                case 1:
                centroid2.push_back(cars.at(i));
                break;
                case 2:
                centroid3.push_back(cars.at(i));
                break;
                default:
                cout << "No cluster" << endl;
            }
            clusters.push_back(cluster); //keep track of the number as well, for the final iteration in order to add it into
            //the cars_clustered.csv

        }
        error += calculateAverage(centroid1, centroidLocation, 0); //getting the change in centroid location for each one
        error += calculateAverage(centroid2, centroidLocation, 1); //changing the centroid location by adding up the averages
        error += calculateAverage(centroid3, centroidLocation, 2);


    } while(error > 1e-6);

    for(int i = 0; i < K; i++){ //unnormalize the centroids and the cars
        unNormalize(cars, maxValues, minValues, i);
        unNormalize(centroidLocation, maxValues, minValues, i);
    }
    for(int i = 0; i < cars.size(); i++){ //appending which cluster it is to the vector cars
        cars.at(i).push_back(clusters.at(i)+1);
    }

    centroidLocation.at(0).push_back(1); //adding cluster names to the centroids
    centroidLocation.at(1).push_back(2);
    centroidLocation.at(2).push_back(3);

    parameters.push_back(" cluster"); //adding cluster to the header

    outputParameters(parameters, o1); //output headers
    outputFile(cars, o1); //output cars, cars_clustered.csv
    outputParameters(parameters, c1); //output header and centroids for clusters.csv
    outputFile(centroidLocation, c1);


    return 0;
}

void outputParameters(vector<string> &list, ofstream &o){
    for(int i = 0; i < list.size()-1; i++){
        o << list.at(i) << ",";
    }
    o << list.at(list.size()-1) << endl;

}

void unNormalize(vector<vector<double>> &list, vector<double> &maxValues, vector<double> &minValues, int index){
    for(int i = 0; i < list.size(); i++){
        list.at(i).at(index) = (list.at(i).at(index)*maxValues.at(index))+minValues.at(index);
    }
}

double getMax(vector<vector<double>> list, int index){
    double max = list.at(0).at(index);
    for(int i = 1; i < list.size(); i++){
        if(list.at(i).at(index) > max){
            max = list.at(i).at(index);
        }
    }
    return max;
}

double getMin(vector<vector<double>> list, int index){
    double min = list.at(0).at(index);
    for(int i = 1; i < list.size(); i++){
        if(list.at(i).at(index) < min){
            min = list.at(i).at(index);
        }
    }
    return min;
}

void normalize(vector<vector<double>> &list, double max, double min, int index){
    for(int i = 0; i < list.size(); i++){
        list.at(i).at(index) = (list.at(i).at(index) - min)/max;
    }
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

double calculateAverage(vector<vector<double>> &centroids, vector<vector<double>> &centroidLocation, int centroidIndex){
    if(centroids.empty())
        return 0;
    
    double error = 0;
    vector<double> sums(7, 0);

    for(int i = 0; i < centroids.size(); i++){ //Iterate through the data regarding the 3 centroids
        for(int j = 0; j < centroids.at(i).size(); j++){ //going through each vector in the respective centroid
            sums.at(j) += centroids.at(i).at(j); //add the value of that field to the respective entry in sum
        }
    }

    for(int s = 0; s < sums.size(); s++){ //going throughout the sums
            sums.at(s) = sums.at(s)/centroids.size(); //setting each field to the average of the values
            error += fabs(sums.at(s) - centroidLocation.at(centroidIndex).at(s)); //adding the error between the sum and the past centroid location
    }
    centroidLocation.at(centroidIndex) = sums;

    return error;
}

void outputFile(vector<vector<double>> &list, ofstream &o){
    for(int i = 0; i < list.size(); i++){
        for(int j = 0; j < list.at(i).size()-1; j++){
            o << list.at(i).at(j) << ", ";
        }
        o << list.at(i).at(list.at(i).size()-1) << endl;
    }
}

void outputCluster(vector<vector<double>> &list, ofstream &o){
    for(int i = 0; i < list.size(); i++){
        for(int j = 0; j < list.at(i).size()-1; j++){
            o << list.at(i).at(j) << ", ";
        }
        o << list.at(i).at(list.at(i).size()-1) << endl;
    }
}

int getCentroidClosest(vector<double> &list, vector<vector<double>> &centroidLocation){
    double minDistance = 10, distance;
    int cluster;
    for(int c = 0; c < 3; c++){
        distance = calculateDistance(list, centroidLocation.at(c));
        if(distance < minDistance){
            cluster = c;
            minDistance = distance;
        }
    }
    return cluster;
}