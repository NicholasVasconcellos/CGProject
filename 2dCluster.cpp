#include "2dCluster.h"
#include <iostream>

FaceInfo::FaceInfo() : clusterIdx(-1), seen(false) {}

Cluster::Cluster(int index, int size) : index(index), size(size) {}

// void Clusters::getHistogram()
// {
//     // Produce a CSV file with the Table
// }