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

    # TODO Follow convergence procedure to find final locations for each center
    #Step 1, randomly pick k clusters
    ran_clust = np.random.randint(1,10)
    #print ("Random number for clusters", ran_clust)
    #print (self._data)
    
    #Step 2, Classify every data point as belonging to a cluster by
    #Euclidean distance measurement (closest cluster wins)
    center = [[0,0]]
    clusters = np.zeros(len(self._data))
    center_old = np.zeros(len(center))
    #print (center)
    #print (clusters)
    #print (center_old)
    euc_dist=1000
    error = 1

    #while euc_dist > 3:
    while error !=0:

      for datapoint in self._data:
        #print ('hello')
        diff = np.subtract (center, datapoint)
        euc_dist = np.linalg.norm(diff, 1)
        #print (euc_dist)

        diff2 = np.subtract (center, center_old)
        error= np.linalg.norm(diff2, None)
        np.copyto(center_old, center_old)


        #for i in range(len(self._data)):
        #  distances = euc_dist
        #  cluster=np.argmin(distances)
        #  clusters[i]=cluster


      #Step 3, Calculate centroid of each cluster. Relocate cluster to centroid position.
      #print (max(self._data))
      max_x = max(self._data)[0]
      #print (max_x)
      max_y = max(self._data)[1]
      #print (max_y)
      cent_x = np.random.randint(0, max_x, ran_clust)
      #print (cent_x)
      cent_y = np.random.randint(0, max_y, ran_clust)
      #print (cent_y)
      center = np.array(list(zip(cent_x, cent_y)), dtype=np.float32)
      #print (center)

      for i in range (ran_clust):
        points = [self._data[j] for j in range(len(self._data)) if clusters[j]==i]
        center[i] = np.mean(points, 0)

      
    #Step 4, Repeat 2-3 until centroids converge.
    #above in while loop
    

    # TODO Add each of the 'k' final cluster_centers to the model (self._cluster_centers)
    self._cluster_centers.append(center)

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

    #TODO: Perform k_nearest_neighbor classification, set best_label to majority label for k-nearest points
    distances = []
    difference = np.diff(self._data)
    for group in self._data:
      for difference in self._data:
        euclidean_distance = np.linalg.norm(np.array(difference), 1)
        distances.append([euclidean_distance, group])

    results = [i[1] for i in sorted (distances)[:k]]
    best_label = Counter(results).most_common(1)[0][0]

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
