#include "NCluster.h"

NCluster::NCluster(unsigned int n, AbstractLattice *lattice) :
    N(n),
    Nodes(n, Node{-1,-1,-1}),
    Lattice(lattice)
{

}

NCluster::NCluster(const NCluster& nMinusOneCluster, const Node& nodeN) :
    N(nMinusOneCluster.GetSize()+1),
    Nodes(nMinusOneCluster.GetSize()+1)
{
    this->Lattice = nMinusOneCluster.Lattice;
    for (auto i=0; i<nMinusOneCluster.GetSize(); ++i)
        this->Nodes[i] = nMinusOneCluster[i];
    this->Nodes[this->N-1] = nodeN;
}

NCluster::NCluster(const Node& firstNode, unsigned int n, AbstractLattice *lattice) :
    N(n),
    Nodes(n),
    Lattice(lattice)
{
    this->Nodes[0] = firstNode;
}

std::ostream& operator<< (std::ostream& stream, const NCluster& nCluster)
{
    for (auto i=0; i<nCluster.GetSize(); ++i)
    {
        std::vector<unsigned int> coordinates(nCluster.Lattice->GetDim());
        nCluster.Lattice->GetSiteCoordinates(nCluster[i].SiteIndex,coordinates);
        stream << "Node " << i+1 << ": " << nCluster[i] << " coordinates: ";
        for (auto j=0; j<coordinates.size(); ++j)
            std::cout << " " << coordinates[j];
        std::cout << "\n";
    }
    return stream;
}

ClusterManager::ClusterManager(unsigned int nMax, AbstractLattice *lattice) :
    NMax(nMax),
    Lattice(lattice),
    Clusters(nMax, std::vector<NCluster>(0,NCluster(0,lattice)))
{
}


unsigned int ClusterManager::Enumerate()
{
    NCluster oneCluster(1,this->Lattice);
    oneCluster[0] = Node{this->Lattice->GetSiteIndex(std::vector<unsigned int>{this->Lattice->GetN(), this->Lattice->GetN()}), -1, -1, -1};
    this->Clusters.push_back(std::vector<NCluster>(1,oneCluster));
    return this->Clusters.size();
}

/// add a j-cluster making sure it is not already included
void ClusterManager::AddCluster(const NCluster& cluster)
{
    if (cluster.GetSize()>this->NMax)
        throw std::invalid_argument("ClusterManager: asked to add cluster with size greater than NMax!");
    auto index = cluster.GetSize()-1;
    if (std::find(this->Clusters[index].begin(), this->Clusters[index].end(), cluster) == this->Clusters[cluster.GetSize()-1].end())
           this->Clusters[cluster.GetSize()-1].push_back(cluster);
}

bool operator==(const NCluster& lhs, const NCluster& rhs)
{
    if (lhs.GetSize()!=rhs.GetSize())
        return false;
    for (auto i=0; i<lhs.GetSize(); ++i)
        if (lhs[i]!=rhs[i])
            return false;
    return true;
}

bool operator!=(const NCluster& lhs, const NCluster& rhs)
{
    return ! (lhs==rhs);
}
