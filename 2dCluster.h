#pragma once
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>
#include <CGAL/Triangulation_data_structure_2.h>
#include <CGAL/draw_triangulation_2.h>
#include <fstream>
#include <cmath>
#include <utility>
#include <list>
#include <unordered_map>

struct FaceInfo
{
    int clusterIdx;
    bool seen;
    FaceInfo();
};

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<FaceInfo, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Triangulation_2<K, Tds> Triangulation;
typedef Tds::Face_handle Face;
typedef Triangulation::Point Point;


struct Cluster
{
    int index;
    int size = 0;

    Cluster(int index, int size);
};

struct Clusters
{
    std::unordered_map<int, int> Table;
    std::list<Cluster> clusterList;

    void getHistogram();
};
