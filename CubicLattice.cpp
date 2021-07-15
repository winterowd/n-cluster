#include "CubicLattice.h"

CubicLattice::CubicLattice(unsigned int n) : AbstractLattice(), N(n), NSq(n*n)
{

}

int CubicLattice::GetSiteIndex(const std::vector<unsigned int>& indices) const
{
    return this->NSq*indices[2]+this->N*indices[1]+indices[0];
}

void CubicLattice::GetSiteCoordinates(int index, std::vector<unsigned int>& indices)
{
    indices[2] = index/this->NSq;
    auto temp = index%this->NSq;
    indices[1] = temp/this->N;
    indices[0] = temp%this->N;
}

int CubicLattice::GetNearestNeighbor(int siteIndex, int nnIndex)
{
    std::vector<unsigned int> temp(3);
    this->GetSiteCoordinates(siteIndex, temp);
    switch (nnIndex) {
    case 1:
        temp[0] = (temp[0]+1)%this->N;
        break;
    case 2:
        temp[1] = (temp[1]+1)%this->N;
        break;
    case 3:
        temp[2] = (temp[2]+1)%this->N;
        break;
    case 4:
        temp[0] = (temp[0]+this->N-1)%this->N;
        break;
    case 5:
        temp[1] = (temp[1]+this->N-1)%this->N;
        break;
    case 6:
        temp[2] = (temp[2]+this->N-1)%this->N;
        break;
     default:
        throw std::invalid_argument("Invalid nearest neighbor index for CubicLattice!");
    }
    return this->GetSiteIndex(temp);
}
