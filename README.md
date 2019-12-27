# CarsKMeansClustering
K-Means Clustering using 3 centroids regarding 7 distinct properties of cars. 

The cars.csv file is the initial data file, where the 7 properties (excluding region) are processed by the kmeans_cluster.cpp.
After running the kmeans_cluster.cpp (in the same directory as the cars.csv), a cars_clustered.csv and a centroids.csv is generated
to introduce a new field, the specific cluster, to each car, along with the location of the centroids. 

kmeans_identify.cpp allows entry of data regarding a new car using command-line arguments, and it will determine the cluster that the car 
belongs to based on the training data.
