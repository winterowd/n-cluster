#ifndef SQUARELATTICE_H
#define SQUARELATTICE_H

#include <vector>

#include "AbstractLattice.h"

class SquareLattice : public AbstractLattice
{
private:
    unsigned int N; /// dimension of lattice

public:
    SquareLattice(unsigned int n);

    ~SquareLattice() {};

    int GetSiteIndex(const std::vector<unsigned int>& indices) const override;

    void GetSiteCoordinates(int index, std::vector<unsigned int>& indices) override;

    int GetNearestNeighbor(int siteIndex, int nnIndex) override;

    unsigned int GetN() const override { return this->N; }

    unsigned int GetNbrNN() const override { return this->NbrNN; }

    unsigned int GetDim() const override { return this->Dim; }

    static const unsigned int Dim = 2;

    static const unsigned int NbrNN = 4;  /// vectors labeled by 1,2,...,NbrNN

    int GetOppositeDirection(int nnIndex) { return (nnIndex+1)%this->NbrNN+1; }
};

#endif // SQUARELATTICE_H
