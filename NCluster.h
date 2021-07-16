#ifndef NCLUSTER_H
#define NCLUSTER_H

#include <array>
#include <vector>
#include <iostream>
#include <fstream>

#include "SquareLattice.h"

#define MAX_CLUSTER_SIZE 100

struct Node {
    int SiteIndex, ParentIndex, WhichNNToSite, ParentNode;
};

inline std::ostream& operator<< (std::ostream& stream, const Node& node)
{
    stream << node.SiteIndex << " " << node.ParentIndex << " " << node.WhichNNToSite;
    return stream;
}

inline bool operator==(const Node& lhs, const Node& rhs)
{
    return (lhs.SiteIndex==rhs.SiteIndex && lhs.ParentIndex==rhs.ParentIndex); /// only need to compare parent and site itself
}

inline bool operator!=(const Node& lhs, const Node& rhs)
{
    return ! (lhs==rhs);
}

inline bool operator<(const Node& lhs, const Node& rhs)
{
    return lhs.SiteIndex<rhs.SiteIndex;
}

inline bool operator>(const Node& lhs, const Node& rhs)
{
    return lhs.SiteIndex>rhs.SiteIndex;
}

inline bool operator<=(const Node& lhs, const Node& rhs)
{
    return lhs.SiteIndex<=rhs.SiteIndex;
}

inline bool operator>=(const Node& lhs, const Node& rhs)
{
    return lhs.SiteIndex>=rhs.SiteIndex;
}

class NCluster
{

private:

    unsigned int N;

    std::vector<Node> Nodes;

    AbstractLattice *Lattice;

public:

    NCluster(unsigned int N, AbstractLattice *lattice);
    NCluster(const Node& firstNode, unsigned int n, AbstractLattice *lattice); /// create cluster from first node
    NCluster(const NCluster& nMinusOneCluster, const Node& nodeN); /// constructor from (N-1) cluster and new site to be added

    Node& operator[](unsigned int index) { if (index>=this->N) throw std::invalid_argument("NCluster: invalid index!"); return this->Nodes[index]; }
    const Node& operator[](unsigned int index) const { if (index>=this->N) throw std::invalid_argument("NCluster: invalid index!"); return this->Nodes[index]; }

    unsigned int GetSize() const { return this->N; }

    Node GetLastNode() const { return Nodes[this->N-1]; }
    Node GetParentOfLastNode() const { return Nodes[this->Nodes[this->N-1].ParentNode]; }

    /// debugging routine
    bool IsValidCluster();

    friend std::ostream& operator<< (std::ostream& stream, const NCluster& nCluster);

};

bool operator==(const NCluster& lhs, const NCluster& rhs);
bool operator!=(const NCluster& lhs, const NCluster& rhs);


class ClusterManager
{
private:

    unsigned int NMax; /// max size of clusters

    AbstractLattice *Lattice;

    std::vector<std::vector<NCluster>> Clusters;

public: /// for debugging only
    void AddCluster(const NCluster& cluster);

private:
    void IncrementCluster(const NCluster& jMinusOneCluster); /// from a single (j-1) cluster push resulting j clusters into the vector

public:
    ClusterManager(unsigned int nMax, AbstractLattice *lattice);

    std::vector<unsigned int> Enumerate(bool debug=false);

    unsigned int GetNumberofJClusters(unsigned int j) { if (j>=this->NMax) throw std::invalid_argument("ClusterManager: asked for size of clusters greater than NMax!"); return this->Clusters[j].size(); }

    /// debugging routines
    bool AllValidClusters();
    bool NoDoubleCounting();

};

#endif // NCLUSTER_H
