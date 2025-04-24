#pragma once
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>
#include <CGAL/Triangulation_data_structure_2.h>
#include <CGAL/draw_triangulation_2.h>
#include <QApplication>
#include <CGAL/Qt/Basic_viewer_qt.h>
#include <CGAL/IO/Color.h>
#include <fstream>
#include <cmath>
#include <utility>
#include <vector>
#include <cassert>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <random>

struct FaceInfo
{
    int clusterIdx;
    bool seen;
    CGAL::Color color;

    FaceInfo();
};

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<FaceInfo, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Triangulation_2<K, Tds> Triangulation;
typedef CGAL::Delaunay_triangulation_2<K, Tds> DelaunayTriangulation;
typedef Tds::Face_handle Face;
typedef Triangulation::Point Point;

struct Cluster
{
    // Index or ID of this cluster
    int index;

    // List of Faces in this Cluster
    std::vector<Face> faces;

    CGAL::Color color;

    Cluster(int index);

    void setColor();
};

struct Clusters
{
    std::unordered_map<int, int> table;
    std::vector<Cluster *> clusterList;
    std::string pointType;
    std::string trinagulationType;
    std::string parameters;

    Clusters(std::string &pointType, std::string &triangulationType);

    void buildTable();
    void getHistogram();
};

// Point generation function prototypes
std::vector<Point> generateUniformRandomPoints(int numPoints, double xMin, double xMax, double yMin, double yMax);
std::vector<Point> generateClusteredPoints(int numPoints, int numClusters, double clusterDensity,
                                           double xMin, double xMax, double yMin, double yMax);
bool savePointsToCSV(const std::vector<Point> &points, const std::string &filename);
std::vector<Point> loadPointsFromCSV(const std::string &filename);

// Custom color functor for the CGAL triangulation viewer
struct ClusterColorFunctor
{
    template <typename T2>
    static CGAL::Color run(const T2 &,
                           const typename T2::Finite_faces_iterator fh)
    {
        // Return the color stored in the face's info
        return fh->info().color;
    }
};

inline void drawTriangulationWithColors(const Triangulation &t,
                                        const char *title = "Triangulation with Clusters")
{
#ifdef CGAL_USE_BASIC_VIEWER
    int argc = 1;
    const char *argv[2] = {"t2_viewer", "\0"};
    QApplication app(argc, const_cast<char **>(argv));

    // Create viewer with specified parameters
    CGAL::SimpleTriangulation2ViewerQt<Triangulation, ClusterColorFunctor> viewer(app.activeWindow(), t, title, false, ClusterColorFunctor());

    // The viewer should display edges and vertices by default
    viewer.show();
    app.exec();
#else
    // Fallback if the viewer is not available
    std::cerr << "CGAL Basic Viewer was not compiled." << std::endl;
#endif
}