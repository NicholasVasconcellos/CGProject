#include "2dCluster.h"


void printVertices(Triangulation::Finite_faces_iterator &faceIt)
{
    std::cout << "Face id: " << faceIt->info().id << std::endl;
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
        // Set ID
        faceIt->info().id = currId++;

        // Print Vertex Coordinates
        printVertices(faceIt);

        // Print Edges
        printEdges(faceIt);
    }

    CGAL::draw(t);

    return EXIT_SUCCESS;
}
