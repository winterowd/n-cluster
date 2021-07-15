#ifndef CUBICLATTICE_H
#define CUBICLATTICE_H

#include "AbstractLattice.h"

class CubicLattice : public AbstractLattice
{
private:
    unsigned int N; /// dimension of lattice

    unsigned int NSq;

public:
    CubicLattice(unsigned int n);

    ~CubicLattice() {};

    int GetSiteIndex(const std::vector<unsigned int>& indices) const override;

    void GetSiteCoordinates(int index, std::vector<unsigned int>& indices) override;

    int GetNearestNeighbor(int siteIndex, int nnIndex) override;

    unsigned int GetN() const override { return this->N; }

    unsigned int GetNbrNN() const override { return this->NbrNN; }

    unsigned int GetDim() const override { return this->Dim; }

    static const unsigned int Dim = 3;

    static const unsigned int NbrNN = 6; /// vectors labeled by 1,2,...,NbrNN

    int GetOppositeDirection(int nnIndex) { return (nnIndex+2)%this->NbrNN+1; }
};

#endif // CUBICLATTICE_H
