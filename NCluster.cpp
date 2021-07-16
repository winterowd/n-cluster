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

/// check if lattice sites are at most singularly occupied
bool NCluster::IsValidCluster()
{
    for(auto i=0; i<this->Nodes.size(); ++i)
        for(auto j=i+1; j<this->Nodes.size(); ++j)
            if (this->Nodes[i]==this->Nodes[j])
            {
                std::cerr << "Double-counted lattice site with index: " << this->Nodes[i].SiteIndex << " in cluster of size: " << this->N << std::endl;
                return false;
            }
    return true;
}

/// check that all clusters are valid (lattice site not doubly occupied)
bool ClusterManager::AllValidClusters()
{
    bool result = true;
    for(auto i=0; i<this->NMax; ++i)
        for(auto j=0; j<this->Clusters[i].size(); ++j)
            if (!this->Clusters[i][j].IsValidCluster())
                result = false;
    return result;
}

/// all clusters are unique
bool ClusterManager::NoDoubleCounting()
{
    for(auto i=0; i<this->NMax; ++i)
        for(auto j=0; j<this->Clusters[i].size(); ++j)
            for(auto k=j+1; k<this->Clusters[i].size(); ++k)
                if (Clusters[i][j]==Clusters[i][k])
                    return false;
    return true;
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

std::vector<unsigned int> ClusterManager::Enumerate(bool debug)
{
    /// starting 1-cluster (fix by hand)
    std::vector<unsigned int> startingSiteIndices(this->Lattice->GetDim());
    for (auto i=0; i<startingSiteIndices.size(); ++i)
        startingSiteIndices[i] = this->Lattice->GetN()/2; /// is this a good choice?
    this->AddCluster(NCluster(Node{this->Lattice->GetSiteIndex(startingSiteIndices), -1, -1, -1}, 1, this->Lattice));

    for (auto j=0; j<this->NMax-1; ++j)
    {
        for (auto i=0; i<this->Clusters[j].size(); ++i)
            this->IncrementCluster(this->Clusters[j][i]); /// increment starting from each enumerated (j-1)-cluster

        if (debug)
        {
            for (auto i=0; i<Clusters[j+1].size(); ++i)
                std::cout << this->Clusters[j+1][i];
        }
    }

    if (debug)
    {
        if (this->AllValidClusters())
            std::cout << "SUCCESS! All clusters are valid!" << std::endl;

        if (this->NoDoubleCounting())
            std::cout << "SUCCESS! No double counting!" << std::endl;
    }

    std::vector<unsigned int> result(this->NMax);
    for (auto j=0; j<this->NMax; ++j)
        result[j] = Clusters[j].size();
    return result;
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

/// following the algorithm described in Martin (Domb and Green vol. 3), create all allowed j-clusters from a given (j-1)-cluster
void ClusterManager::IncrementCluster(const NCluster& jMinusOneCluster)
{
    if (jMinusOneCluster.GetSize()==1)
    {
        for (auto nn=1; nn<=Lattice->GetNbrNN(); ++nn)
        {
            Node tempNode{Lattice->GetNearestNeighbor(jMinusOneCluster.GetLastNode().SiteIndex,nn), jMinusOneCluster.GetLastNode().SiteIndex, nn, 0};
            if (tempNode>jMinusOneCluster.GetLastNode())
                this->AddCluster(NCluster(jMinusOneCluster, tempNode));
        }
    }
    else /// 2-cluster or larger
    {
        for (auto nn=jMinusOneCluster.GetLastNode().WhichNNToSite+1; nn<=Lattice->GetNbrNN(); ++nn)
        {
            Node tempNode{Lattice->GetNearestNeighbor(jMinusOneCluster.GetLastNode().ParentIndex,nn), jMinusOneCluster.GetLastNode().ParentIndex, nn, jMinusOneCluster.GetLastNode().ParentNode};
            if (tempNode>jMinusOneCluster.GetParentOfLastNode())
                this->AddCluster(NCluster(jMinusOneCluster, tempNode));
        }

        for (auto nn=1; nn<=Lattice->GetNbrNN(); ++nn)
        {
            Node tempNode{Lattice->GetNearestNeighbor(jMinusOneCluster.GetLastNode().SiteIndex,nn), jMinusOneCluster.GetLastNode().SiteIndex, nn, ((int) jMinusOneCluster.GetSize())-1};
            if (tempNode>jMinusOneCluster.GetParentOfLastNode())
                this->AddCluster(NCluster(jMinusOneCluster, tempNode));
        }
    }

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
