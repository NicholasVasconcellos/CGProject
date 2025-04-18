#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_2.h>
#include <CGAL/draw_triangulation_2.h>
#include <fstream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_2<K> Triangulation;
typedef Triangulation::Point Point;

int main(int argc, char *argv[])
{

    // Sample Points (Testing)
    std::vector<Point> points = {
        Point(0, 0), Point(1, 0), Point(0, 1),
        Point(1, 1), Point(0.5, 0.5)};

    Triangulation t;
    t.insert(points.begin(), points.end());

    CGAL::draw(t);

    return EXIT_SUCCESS;
}
