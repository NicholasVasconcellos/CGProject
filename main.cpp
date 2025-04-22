#include "2dCluster.h"

void printVertices(Triangulation::Finite_faces_iterator &faceIt)
{
    std::cout << "Face id: " << faceIt->info().clusterIdx << std::endl;
    for (int i = 0; i < 3; i++)
    {
        std::cout << "\tVertex " << i << ": " << faceIt->vertex(i)->point() << std::endl;
    }
}

double getAngle(Point &p1, Point &p2)
{
    // Return Angle in Degrees of an Edge
    return std::atan2(p2.y() - p1.y(), p2.x() - p1.x()) * 180.0 / M_PI;
}

// Check if an edge is vertical based on angle tolerance
bool isVertical(double angle, double tolerance)
{
    return std::abs(90 - std::abs(angle)) <= tolerance;
}

void printEdges(Triangulation::Finite_faces_iterator &faceIt)
{
    // std::cout << "Face id: " << faceIt->info().id << std::endl;
    for (int i = 0; i < 3; i++)
    {
        std::cout << "\tEdge " << i << ": (" << faceIt->vertex(i)->point() << ") --> "
                  << "(" << faceIt->vertex((i + 1) % 3)->point() << ")" << std::endl;
        std::cout << "\tAngle = " << getAngle(faceIt->vertex(i)->point(), faceIt->vertex((i + 1) % 3)->point()) << std::endl;
    }
}

Clusters getClusters(Triangulation &t, double tolerance)
{
    // Create a queue for Curr and next Clusters
    std::queue<Face> currCluster;
    std::queue<Face> nextCluster;

    // Put first face on next Cluster Queue
    nextCluster.push(Face(t.finite_faces_begin()));

    // Create Clusters obj to store result

    Clusters result;

    return result;
}

// Clustering Alg

/*
// Set the tolerance in degrees
int tolerance = 30

// Add an attribute clusterIndex to the face base class set to -1 by default
    // clusterIndex = -1; ( -1 means unassigned flag)

// Create a queue for Curr and next Clusters
// Curr Cluster Queue
Queue currCluster

// Next Cluster Queue
Queue nextCluster

// Put first face on next Cluster Queue
nextCluster.push(faces.begin())

// Initialize Cluster Variables
currClusterIdx = 0;
currClusterSize = 0;


while nextCluster ! empty or currCluster ! empty
    if(currCluster.empty())
// Create a new Cluster, and add element to currCluster queue

// Increment Cluster Index
clusterIdx++;

// Create new cluster object
Cluster currCluster(id= ClusterIndex, size=0, face = newCluster.top().face)

// Add it to list of clusters
clusters.add(currCluster)

// Add it to the currentCluster
currCluster.push( nextCluster.top())
    continue;


    // Process Existing Cluster
// get the queue top and pop
f1 = currCluster.front
currCluster.popFront()
// Increment current cluster size
    currCluster.size++;

    // BFS the Neighbours
// for every neighbouring face
for i in 3
    f2 = f1.neighbour(i)
    if( f2 is Finite && f2 not Seen)
        if(isVertical(Edge(f1, f2))
            // Put the two faces in same cluster
            // if f1 clustered
                // if f2 clustered
                    // This shouldn't happen but ok
                    // Merge clusters (f1.cluster, f2.cluster)
                // else
                    // Add f2 to f1 cluster
                    // f2.cluster = f1.cluster
            // else
                // if f2 clustered
                    // add f1 to f2 cluster
                //else
                    // Create new cluster with f1 and f2
        // Add f2 to currCluster Queue
    else // (Not Vertical)
        // Put each face in a different cluster
            // if f1 clustered
                // if f2 clustered
                    // continue;
                // else
                    // Add f2 to new cluster
                    // newCluster(f2)
            // else
                // if f2 clustered
                    // Add f1 to new cluster
                    // newCluster(f1)
                //else
                    // Create a new cluster for each
                    // newCluster(f1)
                    // newCluster(f2)
        // Add f2 to Next Cluster Queue
        nextCluster.push(f2)
*/

int main(int argc, char *argv[])
{
    // Sample Points (Testing)
    std::vector<Point> points = {
        Point(0, 0), Point(1, 0), Point(0, 1),
        Point(1, 1), Point(0.5, 0.5)};

    Triangulation t;
    t.insert(points.begin(), points.end());

    // Iterate all faces and set ID for each
    int currId = 0;
    for (auto faceIt = t.finite_faces_begin(); faceIt != t.finite_faces_end(); faceIt++)
    {
        // Print Vertex Coordinates
        printVertices(faceIt);

        // Print Edges
        printEdges(faceIt);
    }

    // Cluster Faces together
    double tolerance = 10;

    Clusters faceClusters = getClusters(t, tolerance);

    CGAL::draw(t);

    return EXIT_SUCCESS;
}
