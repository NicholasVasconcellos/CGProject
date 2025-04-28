#include "2dCluster.h"

double getAngle(Point &p1, Point &p2)
{
    // Return Angle in Degrees of an Edge
    return std::atan2(p2.y() - p1.y(), p2.x() - p1.x()) * 180.0 / M_PI;
}

void printVertices(Triangulation::Finite_faces_iterator &faceIt)
{
    std::cout << "Face id: " << faceIt->info().clusterIdx << std::endl;
    for (int i = 0; i < 3; i++)
    {
        std::cout << "\tVertex " << i << ": " << faceIt->vertex(i)->point() << std::endl;
    }
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

void printInfo(Triangulation &t)
{
    // Iterate all faces
    int currId = 0;
    for (auto faceIt = t.finite_faces_begin(); faceIt != t.finite_faces_end(); faceIt++)
    {
        // Print Vertex Coordinates
        printVertices(faceIt);

        // Print Edges
        printEdges(faceIt);
    }
}

// Check if an edge is vertical based on angle tolerance
bool isVertical(double angle, double tolerance)
{
    return std::abs(90 - std::abs(angle)) < tolerance;
}

/**
 * @brief Check if i_th neighbour of face f is in same cluster
 *
 * @param f Face handle (reference).
 * @param i Neighbour Index.
 * @param tolerance Tolerance of angle beyond 90 degrees to
 * which still considered vertical
 * @return if they are clusterable.
 */
bool isClusterable(Face f, int i, double tolerance)
{
    // Check if face is in same cluster
    Point p1 = f->vertex((i + 1) % 3)->point();
    Point p2 = f->vertex((i + 2) % 3)->point();

    double angle = getAngle(p1, p2);

    return isVertical(angle, tolerance);
}

Clusters getClusters(Triangulation &t, std::string &pointSetLabel, std::string &triangulationType, double tolerance)
{

    // Create a queue for Curr and next Clusters
    std::queue<Face> currClusterQ;
    std::queue<Face> nextClusterQ;

    // Pick a face to start the traversal
    Face f = t.finite_faces_begin();
    f->info().seen = true; // Mark it as seen

    // Put first face on next Cluster Queue
    nextClusterQ.push(f);

    // Create Clusters obj to store all clusters
    Clusters allClusters(pointSetLabel, triangulationType, tolerance);

    // Reference to current Cluster
    Cluster *clusterPtr = nullptr;

    // Current Cluster Index
    int clusterIdx = 0;

    while (!nextClusterQ.empty() || !currClusterQ.empty())
    {

        // If current Cluster is empty, we start a new one
        if (currClusterQ.empty())
        {
            // Create a new Cluster
            clusterPtr = new Cluster(clusterIdx);

            // Add it to list of clusters
            allClusters.clusterList.push_back(clusterPtr);

            // Add it to CurrCluster Queue
            currClusterQ.push(nextClusterQ.front());

            // Pop from next cluster
            nextClusterQ.pop();

            // Increment Current Cluster Index (For the next cluster)
            clusterIdx++;
        }
        // Process existing cluster
        // Double check queue is not empty
        assert(!currClusterQ.empty());

        // get the top element and pop the queue
        f = currClusterQ.front();
        currClusterQ.pop();

        // Add Information to Face Object
        // Add cluster color to the Face object
        // f->info().color = CGAL::blue();
        f->info().color = clusterPtr->color;
        // Add Current Face to the current cluster
        clusterPtr->faces.push_back(f);

        // BFS Neighbours of f
        for (int i = 0; i < 3; i++)
        {
            // Get ith Neighbour
            Face neighbour = f->neighbor(i);

            // Ignore visited or infinite faces
            if (t.is_infinite(neighbour) || neighbour->info().seen)
            {
                continue;
            }

            if (isClusterable(f, i, tolerance))
            {
                // Add the same cluster
                currClusterQ.push(neighbour);
            }
            else
            {
                // Add to a new Cluster
                nextClusterQ.push(neighbour);
            }

            // Mark Seen
            neighbour->info().seen = true;
        }
    }

    return allClusters;
}

void simulate(std::vector<Point> &points, std::string &label, double tolerance, std::string &triangulationType)
{
    // Create a triangulation and iterative add each point in the set
    if (triangulationType == "Regular")
    {
        Triangulation t;
        t.insert(points.begin(), points.end());
        // // Print Edges and Vertices (Optional)
        // printInfo(t);

        // Cluster Faces together
        // Create Cluster Object
        Clusters faceClusters = getClusters(t, label, triangulationType, tolerance);

        // Build a table
        faceClusters.buildTable();
        // Create a Histogram CSV File
        faceClusters.getHistogram();

        // CGAL::draw(t);

        // drawTriangulationWithColors(t, (label + " " + triangulationType).c_str());
    }
    else if (triangulationType == "Delaunay")
    {
        DelaunayTriangulation t;
        t.insert(points.begin(), points.end());
        // // Print Edges and Vertices (Optional)
        // printInfo(t);

        // Cluster Faces together
        // Create Cluster Object
        Clusters faceClusters = getClusters(t, label, triangulationType, tolerance);

        // Build a table
        faceClusters.buildTable();
        // Create a Histogram CSV File
        faceClusters.getHistogram();

        // CGAL::draw(t);

        // drawTriangulationWithColors(t, (label + " " + triangulationType).c_str());
    }
    else
    {
        throw(std::runtime_error("Invalid Triangulation Type selected: \"" + triangulationType + "\""));
    }
}

void getPointSets(int numPoints, std::vector<int> numClusters, double clusterDensity,
                  double xMin, double xMax, double yMin, double yMax)
{
    // Generate all Point Sets
    std::unordered_map<std::string, std::vector<Point>> pointSets;
    std::string label;

    // Uniform Points
    std::cout << "Generating " << numPoints << " uniform random points..." << std::endl;
    std::vector<Point> uniformPoints = generateUniformRandomPoints(numPoints, xMin, xMax, yMin, yMax);
    label = "N=" + std::to_string(numPoints) + "_Uniform";
    pointSets[label] = uniformPoints;

    // Clustered Points
    for (int i = 0; i < numClusters.size(); i++)
    {
        std::cout << "Generating " << numPoints << " points in " << numClusters.at(i)
                  << " clusters with density " << clusterDensity << "..." << std::endl;
        std::vector<Point> clusteredPoints = generateClusteredPoints(numPoints, numClusters.at(i), clusterDensity, xMin, xMax, yMin, yMax);
        label = "N=" + std::to_string(numPoints) + "_K=" + std::to_string(numClusters.at(i));
        pointSets[label] = clusteredPoints;
    }

    // Use sample points for testing
    std::cout << "Generating Sample Points" << std::endl;
    std::vector<Point> samplePoints = {
        Point(0, 0), Point(1, 0), Point(0, 1),
        Point(1, 1), Point(0.5, 0.5)};
    pointSets["Sample_Points"] = samplePoints;

    std::cout << " Generated " << pointSets.size() << "Point SEts" << std::endl;

    // Write all Point Sets to CSV
    for (auto it = pointSets.begin(); it != pointSets.end(); ++it)
    {

        std::string filename = "../pointSets/" + it->first + ".csv";
        savePointsToCSV(it->second, filename);
    }
}

int main(int argc, char *argv[])
{

    /*=== CREATE POINT SETS ====*/

    // // Set Parameters for point generation
    // int numPoints = 1000;
    // std::vector<int> numClusters = {5, 10, 20, 50};
    // double clusterDensity = 5.0;
    // double xMin = -50.0, xMax = 50.0;
    // double yMin = -50.0, yMax = 50.0;

    // // Write Point Sets to pointSets Folder in CSV format
    // getPointSets(numPoints, numClusters, clusterDensity, xMin, xMax, yMin, yMax);

    /*== Read Point Sets*/

    /*=== CREATE HISTOGRAMS ====*/
    // Get Histogram for all point sets

    // Read Point Sets and Labels
    std::unordered_map<std::string, std::vector<Point>> pointSets = loadPointSets();

    // Set Parameters for Triangulations
    std::vector<double> angleTolerance = {0,10,20,30,40,50,60,70,80,90};
    std::vector<std::string> triangulationTypes = {"Regular", "Delaunay"};

    // // Produce all CSV Histograms
    for (auto it = pointSets.begin(); it != pointSets.end(); it++)
    {
        // For each Point SEt
        for (auto &&triangulationType : triangulationTypes)
        {
            // For Each Triangulation Type
            for (auto &&tolerance : angleTolerance)
            {
                // For each Angle Tolerance
                std::string label = it->first;
                simulate(it->second, label, tolerance, triangulationType);
            }
        }
    }

    /*=== DRAW A TRIANGULATION ===*/
    // Draw Existing Point SEts with Delaunay / Regular Triangulation

    return EXIT_SUCCESS;
}
