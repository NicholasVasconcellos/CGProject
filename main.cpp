#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>
#include <CGAL/Triangulation_data_structure_2.h>
#include <CGAL/draw_triangulation_2.h>
#include "FaceInfo.h"
#include <fstream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<FaceInfo, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Triangulation_2<K, Tds> Triangulation;
typedef Triangulation::Point Point;

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
        faceIt->info().id = currId++;
    }

    CGAL::draw(t);

    return EXIT_SUCCESS;
}
