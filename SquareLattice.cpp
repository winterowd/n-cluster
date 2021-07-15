#include <iostream>

#include "SquareLattice.h"

SquareLattice::SquareLattice(unsigned int n) : AbstractLattice(), N(n)
{

}

int SquareLattice::GetSiteIndex(const std::vector<unsigned int>& indices) const
{
    return this->N*indices[1]+indices[0];
}

void SquareLattice::GetSiteCoordinates(int index, std::vector<unsigned int>& indices)
{
    indices[0] = index%this->N;
    indices[1] = index/this->N;
}

int SquareLattice::GetNearestNeighbor(int siteIndex, int nnIndex)
{
    std::vector<unsigned int> temp(2);
    this->GetSiteCoordinates(siteIndex, temp);
    switch (nnIndex) {
    case 1:
        temp[0] = (temp[0]+1)%this->N;
        break;
    case 2:
        temp[1] = (temp[1]+1)%this->N;
        break;
    case 3:
        temp[0] = (temp[0]+this->N-1)%this->N;
        break;
    case 4:
        temp[1] = (temp[1]+this->N-1)%this->N;
        break;
     default:
        throw std::invalid_argument("Invalid nearest neighbor index for CubicLattice!");
    }
    return this->GetSiteIndex(temp);
}
