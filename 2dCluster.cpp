#include "2dCluster.h"

FaceInfo::FaceInfo() : clusterIdx(-1), seen(false) {}

Cluster::Cluster(int index) : index(index) { setColor(); }

void Cluster::setColor()
{
    // Create a random color
    // Generate 3 random RGB Values for Color
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> colorDist(0, 255);

    // Create color from 3 random RGB values
    this->color = CGAL::Color(colorDist(gen),
                              colorDist(gen),
                              colorDist(gen));
}

Clusters::Clusters(std::string &label, std::string &triangulationType, double tolerance) : label(label), triangulationType(triangulationType)
{
    minAngle = 90.0 - tolerance;
}

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

    // Create filename based on Parameters
    std::string filename = "../results/" + this->label + "_Tri=" + this->triangulationType + "_Angle=" + std::to_string(static_cast<int>(this->minAngle)) + "°.csv";

    // Open a CSV file for writing
    std::ofstream csvFile(filename);

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

    std::cout << "Histogram data written to " << filename << std::endl;
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
 * @brief Parse a single CSV file and extract points
 *
 * Reads a CSV file containing point coordinates and converts them into Point objects.
 *
 * @param filename Path to the CSV file to read
 * @return Vector of Point objects parsed from the file
 */
std::vector<Point> readPointsFromCSV(const std::string &filename)
{
    std::vector<Point> points;
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return points;
    }

    std::string line;
    // Skip header line if it exists
    std::getline(file, line);

    // Read points
    double x, y;
    while (file >> x)
    {
        // Handle CSV format (comma after x value)
        if (file.peek() == ',')
            file.ignore();

        file >> y;
        points.push_back(Point(x, y));

        // Ignore the rest of the line (including the newline)
        std::getline(file, line);
    }

    file.close();
    return points;
}

/**
 * @brief Load point sets from CSV files in directory
 *
 * Reads all CSV files from the ../pointSets directory, parsing each one into a vector
 * of points. The filename (without extension) is used as the key in the returned map.
 *
 * @return An unordered map with filename as key and vector of points as value
 */
std::unordered_map<std::string, std::vector<Point>> loadPointSets()
{
    std::unordered_map<std::string, std::vector<Point>> pointSets;
    std::string directory = "../pointSets";

    try
    {
        // Iterate through all files in the directory
        for (const auto &entry : std::filesystem::directory_iterator(directory))
        {
            if (entry.path().extension() == ".csv")
            {
                // Extract the label from the filename (remove path and extension)
                std::string filename = entry.path().filename().string();
                std::string label = filename.substr(0, filename.length() - 4); // Remove .csv

                // Read points from file
                std::vector<Point> points = readPointsFromCSV(entry.path().string());

                // Add to map
                if (!points.empty())
                {
                    pointSets[label] = points;
                    std::cout << "Loaded " << points.size() << " points from " << filename << std::endl;
                }
            }
        }

        std::cout << "Loaded " << pointSets.size() << " point sets from CSV files" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error reading point sets: " << e.what() << std::endl;
    }

    return pointSets;
}
