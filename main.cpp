#include <iostream>
#include <vector>
#include <random>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_2<K> Triangulation;
typedef K::Point_2 Point;

std::vector<Point> generate_random_points(int n, double xmin = 0.0, double xmax = 10.0, double ymin = 0.0, double ymax = 10.0)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis_x(xmin, xmax);
    std::uniform_real_distribution<> dis_y(ymin, ymax);

    std::vector<Point> points;
    for (int i = 0; i < n; ++i)
    {
        points.emplace_back(dis_x(gen), dis_y(gen));
    }
    return points;
}

int main()
{
    // //Random Points
    // std::vector<Point> points = generate_random_points(10);

    // Sample Points (Testing)
    std::vector<Point> points = {
        Point(0, 0), Point(1, 0), Point(0, 1),
        Point(1, 1), Point(0.5, 0.5), Point(0.7, 0.2)};

    // Build Triangulation (Incremental)
    Triangulation t;
    t.insert(points.begin(), points.end());

    // Output Faces (p1, p2, p3)
    std::cout << "Number of Faces: " << t.number_of_faces() << "\n";

    int count = 1;

    for (auto face_it = t.finite_faces_begin(); face_it != t.finite_faces_end(); ++face_it)
    {
        Point p1 = face_it->vertex(0)->point();
        Point p2 = face_it->vertex(1)->point();
        Point p3 = face_it->vertex(2)->point();
        std::cout << "Triangle " << count << ": (" << p1 << "), (" << p2 << "), (" << p3 << ")\n";
        count++;
    }

    return 0;
}
