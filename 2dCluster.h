#include <list>
#include <unordered_map>

struct FaceInfo
{
    int id;
    FaceInfo() : id(-1) {} // Required: DefaultConstructible
    FaceInfo(int i) : id(i) {}
};

struct Cluster
{
    int id;
    int size;

    Cluster(int id, int size): id(this->id), size(this->size){}
};

struct Clusters
{

    std::unordered_map<int, int> Table;

    std::list<Cluster> clusterList;

    void getHistogram()
    {
        // Produce a CSV file with the Table
    }
};
