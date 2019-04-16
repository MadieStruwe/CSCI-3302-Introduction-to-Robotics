# CSCI 3302: Homework 3 -- Clustering and Classification
# Implementations of K-Means clustering and K-Nearest Neighbor classification

# YOUR NAME HERE
#Madelaine Struwe

import pickle
import numpy as np
import matplotlib.pyplot as plt
import math
import random
from numpy.random import permutation
import operator

class KMeansClassifier(object):

  def __init__(self):
    self._cluster_centers = [] # List of points representing cluster centers
    self._data = [] # List of datapoints (lists of real values)

  def add_datapoint(self, datapoint):
    self._data.append(datapoint)

  def fit(self, k):
    # Fit k clusters to the data, by starting with k randomly selected cluster centers.
    # HINT: To choose reasonable initial cluster centers, you can set them to be in the same spot as random (different) points from the dataset
    self._cluster_centers = []

    list_length=len(self._data)
    list_length_times_k = list_length * k
    #print (list_length)
    euc_dist_array = ([])

    # TODO Follow convergence procedure to find final locations for each center
    #Step 1, randomly pick k clusters
    #print ('K is', k)
    rand_clust = random.sample(self._data, k)
    #print ('The random clusters are', rand_clust)
    cluster_centers = rand_clust
    C=np.array(cluster_centers)[0]

    #Step 2, Classify every data point as belonging to a cluster by
    #Euclidean distance measurement (closest cluster wins)
  #for iteration_k in cluster_centers:
    for iteration_point in self._data:
      #print ('point', iteration_point)
      #for every point in the data, do stuff
      for iteration_k in cluster_centers:
        #for every point in the data, look at the k clusters
        #print ('k cluster location', iteration_k) #k cluster locations
        #find the difference between the each k and point
        diff = np.subtract (iteration_k, iteration_point)
        #print ('difference',diff)
        euc_dist = np.linalg.norm(diff, 1)
        #print ('distance between each k and the point',euc_dist)
        euc_dist_array.append(euc_dist)
    #print (euc_dist_array)
    #euc_dist_array = np.array(euc_dist_array)
    #print (euc_dist_array)
    stuff=[euc_dist_array[i:i+k] for i in range(0, len(euc_dist_array), k)]
    #print (stuff)
    for count in stuff:
      #print count
      min_euc = min(count)
      #print min_euc
    #  if count % 4 == 0:
    #    split_euc=np.split(euc_dist_array, 4)
        #print (split_euc)


    #Step 3, Calculate centroid of each cluster. Relocate cluster to centroid position.
    center_old = np.zeros(C).shape 
    clusters = np.zeros(len(self._data))
    error = min_euc
    while error != 0:
      for i in range(len(self._data)):
        distances = dist(self._data[i], C)
        cluster = np.argmin(distances)
        clusters[i] =  cluster
      center_old = deepcopu(C)
      for i in range(k):
        points = [self._data[j] for j in range (len(self._data)) if clusters[j] == i]
        C[i] = np.mean(points, axis=0)
      error
    #Step 4, Repeat 2-3 until centroids converge

    # TODO Add each of the 'k' final cluster_centers to the model (self._cluster_centers)
    self._cluster_centers.append(cluster_centers)

  def classify(self,p):
    # Given a data point p, figure out which cluster it belongs to and return that cluster's ID (its index in self._cluster_centers)
    closest_cluster_index = None

    # TODO Find nearest cluster center, then return its index in self._cluster_centers

    return closest_cluster_index

class KNNClassifier(object):

  def __init__(self):
    self._data = [] # list of (data, label) tuples
  
  def clear_data(self):
    self._data = []

  def add_labeled_datapoint(self, data_point, label):
    self._data.append((data_point, label))
  
  def classify_datapoint(self, data_point, k):
    label_counts = {} # Dictionary mapping "label" => count
    best_label = None

    #TODO: Perform k_nearest_neighbor classification, set best_label to majority l0bel for k-nearest points
    #need training set to make predictions, and test set to evaluate

    self._data=np.random.rand(100,5)
    np.random.shuffle(self._data)
    training, test = self._data[:80,:], self._data[80:,:]
    distances = []
    distance=0
    length = len(test)-1
    for x in range(len(training)):
      for x in range (length):
        diff = np.subtract(test, training[x])
        distance = np.linalg.norm(diff, 1)
        #distance +=pow((test, training[x]), 2)
        #distance= math.sqrt(distance)
        #distance = 7
      dist = distance
      distances.append((training[x], dist))
    distances.sort(key=operator.itemgetter(1))
    best_label = []
    for x in range(k):
      best_label.append(distances[x][0])

    return best_label



def print_and_save_cluster_centers(classifier, filename):
  for idx, center in enumerate(classifier._cluster_centers):
    print "  Cluster %d, center at: %s" % (idx, str(center))


  f = open(filename,'w')
  pickle.dump(classifier._cluster_centers, f)
  f.close()

def read_data_file(filename):
  f = open(filename)
  data_dict = pickle.load(f)
  f.close()

  return data_dict['data'], data_dict['labels']


def main():
  # read data file
  data, labels = read_data_file('hw3_data.pkl')

  # data is an 'N' x 'M' matrix, where N=number of examples and M=number of dimensions per example
  # data[0] retrieves the 0th example, a list with 'M' elements
  # labels is an 'N'-element list, where labels[0] is the label for the datapoint at data[0]


  ########## PART 1 ############
  # perform K-means clustering
  kMeans_classifier = KMeansClassifier()
  for datapoint in data:
    kMeans_classifier.add_datapoint(datapoint) # add data to the model

  kMeans_classifier.fit(4) # Fit 4 clusters to the data


  # plot results
  print '\n'*2
  print "K-means Classifier Test"
  print '-'*40
  print "Cluster center locations:"
  print_and_save_cluster_centers(kMeans_classifier, "hw3_kmeans_FIRSTNAME.pkl")


  print '\n'*2


  ########## PART 2 ############
  print "K-Nearest Neighbor Classifier Test"
  print '-'*40

  # Create and test K-nearest neighbor classifier
  kNN_classifier = KNNClassifier()
  k = 2

  correct_classifications = 0
  # Perform leave-one-out cross validation (LOOCV) to evaluate KNN performance
  for holdout_idx in range(len(data)):
    # Reset classifier
    kNN_classifier.clear_data()

    for idx in range(len(data)):
      if idx == holdout_idx: continue # Skip held-out data point being classified

      # Add (data point, label) tuples to KNNClassifier
      kNN_classifier.add_labeled_datapoint(data[idx], labels[idx])

    guess = kNN_classifier.classify_datapoint(data[holdout_idx], k) # Perform kNN classification
    if guess == labels[holdout_idx]: 
      correct_classifications += 1.0
  
  print "kNN classifier for k=%d" % k
  print "Accuracy: %g" % (correct_classifications / len(data))
  print '\n'*2



if __name__ == '__main__':
  main()
