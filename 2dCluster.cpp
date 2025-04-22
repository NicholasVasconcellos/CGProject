#include "2dCluster.h"


FaceInfo::FaceInfo() : clusterIdx(-1), seen(false) {}

Cluster::Cluster(int index) : index(index) {}

void Clusters::buildTable()
{
    // For each Cluster
    for (auto &&cluster : clusterList)
    {
        // If there is already a cluster of that size
        if (table.count(cluster->faces.size()) > 0)
        {
            // Increment the count
            table[cluster->faces.size()]++;
        }
        else
        {
            table[cluster->faces.size()] = 1;
        }
    }
}

void Clusters::getHistogram()
{
    // Open a CSV file for writing
    std::ofstream csvFile("../cluster_histogram.csv");

    // Check if file opened successfully
    if (!csvFile.is_open())
    {
        std::cerr << "Error: Could not open file for writing." << std::endl;
        return;
    }

    // Write header
    csvFile << "ClusterSize,Count" << std::endl;

    // Write each entry in the table to the CSV file
    for (const auto &entry : table)
    {
        csvFile << entry.first << "," << entry.second << std::endl;
    }

    // Close the file
    csvFile.close();

    std::cout << "Histogram data written to cluster_histogram.csv" << std::endl;
}