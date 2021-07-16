#include <iostream>
#include <vector>

#include "NCluster.h"
#include "SquareLattice.h"
#include "CubicLattice.h"
#include "TriangularLattice.h"

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

int main()
{
    SquareLattice MySquareLattice(100);
    ClusterManager MyClusterManager(6, &MySquareLattice);
    auto result = MyClusterManager.Enumerate();
    for (auto i=0; i<result.size(); ++i)
        std::cout << "SQUARE_TOTAL_" << i+1 << "_CLUSTERS: " << result[i] << std::endl;
    TriangularLattice MyTriangularLattice(100);
    ClusterManager MyClusterManager2(6, &MyTriangularLattice);
    result = MyClusterManager.Enumerate();
    for (auto i=0; i<result.size(); ++i)
        std::cout << "TRIANGULAR_TOTAL_" << i+1 << "_CLUSTERS: " << result[i] << std::endl;
}
