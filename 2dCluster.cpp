#include "2dCluster.h"

FaceInfo::FaceInfo() : clusterIdx(-1), seen(false) {}

Cluster::Cluster(int index) : index(index) {}

Clusters::Clusters(std::string &pointType, std::string &trinagulationType) : pointType(pointType), trinagulationType(trinagulationType) {}

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
    std::cout << "Creating CSV File" << std::endl;
    // Open a CSV file for writing
    std::ofstream csvFile("../results/cluster_histogram.csv");

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

/* Point Generation*/
/**
 * @brief Generate uniform random points in a given rectangular region
 *
 * @param numPoints Number of points to generate
 * @param xMin Minimum x coordinate
 * @param xMax Maximum x coordinate
 * @param yMin Minimum y coordinate
 * @param yMax Maximum y coordinate
 * @return std::vector<Point> Vector of generated points
 */
std::vector<Point> generateUniformRandomPoints(int numPoints, double xMin, double xMax, double yMin, double yMax)
{
    std::vector<Point> points;

    // Initialize random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> xDist(xMin, xMax);
    std::uniform_real_distribution<> yDist(yMin, yMax);

    // Generate points
    for (int i = 0; i < numPoints; ++i)
    {
        double x = xDist(gen);
        double y = yDist(gen);
        points.push_back(Point(x, y));
    }

    return points;
}

/**
 * @brief Generate clustered points around given centers
 *
 * @param numPoints Total number of points to generate
 * @param numClusters Number of cluster centers
 * @param clusterDensity Controls how tightly points cluster around centers
 *                       (smaller values = tighter clusters)
 * @param xMin Minimum x coordinate for cluster centers
 * @param xMax Maximum x coordinate for cluster centers
 * @param yMin Minimum y coordinate for cluster centers
 * @param yMax Maximum y coordinate for cluster centers
 * @return std::vector<Point> Vector of generated points
 */
std::vector<Point> generateClusteredPoints(int numPoints, int numClusters, double clusterDensity,
                                           double xMin, double xMax, double yMin, double yMax)
{
    std::vector<Point> points;

    // Initialize random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> xDist(xMin, xMax);
    std::uniform_real_distribution<> yDist(yMin, yMax);

    // Generate cluster centers
    std::vector<Point> clusterCenters;
    for (int i = 0; i < numClusters; ++i)
    {
        double x = xDist(gen);
        double y = yDist(gen);
        clusterCenters.push_back(Point(x, y));
    }

    // Distribution for selecting which cluster a point belongs to
    std::uniform_int_distribution<> clusterDist(0, numClusters - 1);

    // Generate points around cluster centers
    for (int i = 0; i < numPoints; ++i)
    {
        // Select a random cluster center
        int clusterIdx = clusterDist(gen);
        Point center = clusterCenters[clusterIdx];

        // Generate point with normal distribution around center
        // clusterDensity controls the standard deviation
        std::normal_distribution<> xNormal(center.x(), clusterDensity);
        std::normal_distribution<> yNormal(center.y(), clusterDensity);

        double x = xNormal(gen);
        double y = yNormal(gen);

        points.push_back(Point(x, y));
    }

    return points;
}

/**
 * @brief Save generated points to a CSV file
 *
 * @param points Vector of points to save
 * @param filename Name of the CSV file
 * @return true if successful, false otherwise
 */
bool savePointsToCSV(const std::vector<Point> &points, const std::string &filename)
{
    std::ofstream csvFile(filename);

    if (!csvFile.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
        return false;
    }

    // Write header
    csvFile << "X,Y" << std::endl;

    // Write points
    for (const auto &point : points)
    {
        csvFile << point.x() << "," << point.y() << std::endl;
    }

    csvFile.close();
    std::cout << "Points saved to " << filename << std::endl;
    return true;
}

/**
 * @brief Load points from a CSV file
 *
 * @param filename Name of the CSV file
 * @return std::vector<Point> Vector of loaded points
 */
std::vector<Point> loadPointsFromCSV(const std::string &filename)
{
    std::vector<Point> points;
    std::ifstream csvFile(filename);

    if (!csvFile.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << " for reading." << std::endl;
        return points;
    }

    // Skip header
    std::string line;
    std::getline(csvFile, line);

    // Read points
    double x, y;
    char comma;
    while (csvFile >> x >> comma >> y)
    {
        points.push_back(Point(x, y));
    }

    csvFile.close();
    std::cout << "Loaded " << points.size() << " points from " << filename << std::endl;
    return points;
}
