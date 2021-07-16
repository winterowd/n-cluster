#include <iostream>

#include "TriangularLattice.h"

TriangularLattice::TriangularLattice(unsigned int n) : AbstractLattice(), N(n)
{

}

int TriangularLattice::GetSiteIndex(const std::vector<unsigned int>& indices) const
{
    return this->N*indices[1]+indices[0];
}

void TriangularLattice::GetSiteCoordinates(int index, std::vector<unsigned int>& indices)
{
    indices[0] = index%this->N;
    indices[1] = index/this->N;
}

int TriangularLattice::GetNearestNeighbor(int siteIndex, int nnIndex)
{
    std::vector<unsigned int> temp(2);
    this->GetSiteCoordinates(siteIndex, temp);
    switch (nnIndex) {
    case 1:
        temp[0] = (temp[0]+this->N-1)%this->N;
        temp[1] = (temp[1]+1)%this->N;
        break;
    case 2:
        temp[0] = (temp[0]+this->N-1)%this->N;
        break;
    case 3:
        temp[1] = (temp[1]+this->N-1)%this->N;
        break;
    case 4:
        temp[0] = (temp[0]+1)%this->N;
        temp[1] = (temp[1]+this->N-1)%this->N;
        break;
     case 5:
        temp[0] = (temp[0]+1)%this->N;
        break;
     case 6:
        temp[1] = (temp[1]+1)%this->N;
        break;
     default:
        throw std::invalid_argument("Invalid nearest neighbor index for TriangularLattice!");
    }
    return this->GetSiteIndex(temp);
}
