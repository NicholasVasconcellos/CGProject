#include "2dCluster.h"
#include <iostream>

FaceInfo::FaceInfo() : id(-1) {}
FaceInfo::FaceInfo(int i) : id(i) {}

FaceWrapper::FaceWrapper(Triangulation::Face_handle fh) : face(fh) {}

void FaceWrapper::print_info() const
{
    std::cout << "Wrapped Face ID: " << face->info().id << std::endl;
}

Cluster::Cluster(int id, int size) : id(id), size(size) {}

// void Clusters::getHistogram()
// {
//     // Produce a CSV file with the Table
// }