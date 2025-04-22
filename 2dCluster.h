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
    int id;
    bool seen = false;
    FaceInfo();
    FaceInfo(int i);
};

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<FaceInfo, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Triangulation_2<K, Tds> Triangulation;
typedef Triangulation::Point Point;

struct FaceWrapper
{
    Triangulation::Face_handle face;

    FaceWrapper(Triangulation::Face_handle fh);

    void print_info() const;
};

struct Cluster
{
    int id;
    int size;

    Cluster(int id, int size);
};

struct Clusters
{
    std::unordered_map<int, int> Table;
    std::list<Cluster> clusterList;

    void getHistogram();
};
