#include <iostream>
#include <vector>

#include "NCluster.h"
#include "SquareLattice.h"
#include "CubicLattice.h"

void BasicTests()
{
    SquareLattice MySquareLattice(100);
    /// DEBUG: test square lattice object
    for (auto i=1; i<=SquareLattice::NbrNN; ++i)
        std::cout << "OPP(" << i << "): " << MySquareLattice.GetOppositeDirection(i) << std::endl;
    std::vector<unsigned int> siteSquare{10,2};
    auto index = MySquareLattice.GetSiteIndex(siteSquare);
    Node startNode{index,MySquareLattice.GetNearestNeighbor(index,1),MySquareLattice.GetOppositeDirection(1)}; /// from the right
    std::cout << "DEBUG_SQUARE: " << index << "\n";
    for (auto i=1; i<=4; ++i)
    {
        std::vector<unsigned int> tempNN(2);
        MySquareLattice.GetSiteCoordinates(MySquareLattice.GetNearestNeighbor(index,i), tempNN);
        Node nnNode{MySquareLattice.GetSiteIndex(tempNN), startNode.SiteIndex,i};
        std::cout << "NN: " << i << " " << tempNN[0] << " " << tempNN[1] << " " << nnNode.SiteIndex << std::endl;
        if (nnNode>startNode && nnNode.SiteIndex!=startNode.ParentIndex)
            std::cout << "NEXT_NEIGHBOR_CAN_BE_ADDED!" << std::endl;
        else
            std::cout << "NEXT_NEIGHBOR_CANNOT_BE_ADDED!" << std::endl;
    }

    CubicLattice MyCubicLattice(100);
    /// DEBUG: test cubic lattice object
    for (auto i=1; i<=CubicLattice::NbrNN; ++i)
        std::cout << "OPP(" << i << "): " << MyCubicLattice.GetOppositeDirection(i) << std::endl;
    std::vector<unsigned int> siteCube{10,2,9};
    index = MyCubicLattice.GetSiteIndex(siteCube);
    Node startNode2{index,MyCubicLattice.GetNearestNeighbor(index,1),MyCubicLattice.GetOppositeDirection(1)}; /// from the right
    std::cout << "DEBUG_CUBIC: " << index << "\n";
    for (auto i=1; i<=6; ++i)
    {
        std::vector<unsigned int> tempNN(3);
        MyCubicLattice.GetSiteCoordinates(MyCubicLattice.GetNearestNeighbor(index,i), tempNN);
        Node nnNode{MyCubicLattice.GetSiteIndex(tempNN), startNode2.SiteIndex,i};
        std::cout << "NN: " << i << " " << tempNN[0] << " " << tempNN[1] << " " << tempNN[2] << " " << nnNode.SiteIndex << std::endl;
        if (nnNode>startNode2 && nnNode.SiteIndex!=startNode2.ParentIndex)
            std::cout << "NEXT_NEIGHBOR_CAN_BE_ADDED!" << std::endl;
        else
            std::cout << "NEXT_NEIGHBOR_CANNOT_BE_ADDED!" << std::endl;
    }

    NCluster testCluster4(1, &MyCubicLattice);
    testCluster4[0] = Node{index, -1, -1};
    NCluster testCluster(testCluster4, Node{MyCubicLattice.GetNearestNeighbor(index, 4), index, 4}); /// test constructor which takes lower cluster and new Node

    NCluster testCluster2 = testCluster; /// test copy constructor

    for (auto i=0; i<testCluster.GetSize(); ++i)
        std::cout << i << " testCluster " << testCluster[i] << " testCluster2 " << testCluster2[i] << "\n";

}

void foo(const NCluster &cluster, std::vector<NCluster>& result, AbstractLattice *lattice)
{
    if (cluster.GetSize()==1)
    {
        for (auto nn=1; nn<=lattice->GetNbrNN(); ++nn)
        {
            Node tempNode{lattice->GetNearestNeighbor(cluster.GetLastNode().SiteIndex,nn), cluster.GetLastNode().SiteIndex, nn, 0};
            if (tempNode>cluster.GetLastNode())
                result.push_back(NCluster(cluster, tempNode));
        }
    }
    else /// 2-cluster or larger
    {
        for (auto nn=cluster.GetLastNode().WhichNNToSite+1; nn<=lattice->GetNbrNN(); ++nn)
        {
            Node tempNode{lattice->GetNearestNeighbor(cluster.GetLastNode().ParentIndex,nn), cluster.GetLastNode().ParentIndex, nn, cluster.GetLastNode().ParentNode};
            if (tempNode>cluster.GetParentOfLastNode())
                result.push_back(NCluster(cluster, tempNode));
        }

        for (auto nn=1; nn<=lattice->GetNbrNN(); ++nn)
        {
            Node tempNode{lattice->GetNearestNeighbor(cluster.GetLastNode().SiteIndex,nn), cluster.GetLastNode().SiteIndex, nn, ((int) cluster.GetSize())-1};
            if (tempNode>cluster.GetParentOfLastNode())
                result.push_back(NCluster(cluster, tempNode));
        }

    }
}

int main()
{
    ///
    BasicTests();
    SquareLattice MySquareLattice(100);
    std::vector<std::vector<NCluster>> Clusters(10, std::vector<NCluster>(0,NCluster(0, &MySquareLattice)));

    Clusters[0].push_back(NCluster(Node{MySquareLattice.GetSiteIndex(std::vector<unsigned int>{10,2,9}), -1, -1, -1}, 1, &MySquareLattice));

    for (auto j=0; j<Clusters.size()-1; ++j)
    {
        for (auto i=0; i<Clusters[j].size(); ++i)
            foo(Clusters[j][i], Clusters[j+1], &MySquareLattice);

        //for (auto i=0; i<Clusters[j+1].size(); ++i)
            //std::cout << Clusters[j+1][i];

    }

    for (auto j=1; j<Clusters.size(); ++j)
        std::cout << "SIZE_" << j << "_CLUSTER: " << Clusters[j].size() << std::endl;
}
