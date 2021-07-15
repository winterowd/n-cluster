#ifndef ABSTRACTLATTICE_H
#define ABSTRACTLATTICE_H

#include <vector>

class AbstractLattice {

public:
    virtual ~AbstractLattice(){};

    virtual int GetSiteIndex(const std::vector<unsigned int>& indices) const  = 0;

    virtual void GetSiteCoordinates(int index, std::vector<unsigned int>& indices) = 0;

    virtual int GetNearestNeighbor(int siteIndex, int nnIndex) = 0;

    virtual unsigned int GetN() const = 0;

    virtual unsigned int GetNbrNN() const = 0;

    virtual unsigned int GetDim() const = 0;
};

#endif // ABSTRACTLATTICE_H
