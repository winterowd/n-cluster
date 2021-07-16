#ifndef TRIANGULARLATTICE_H
#define TRIANGULARLATTICE_H

#include "AbstractLattice.h"

class TriangularLattice : public AbstractLattice
{
private:
    unsigned int N; /// dimension of lattice

public:
    TriangularLattice(unsigned int n);

    ~TriangularLattice() {};

    int GetSiteIndex(const std::vector<unsigned int>& indices) const override;

    void GetSiteCoordinates(int index, std::vector<unsigned int>& indices) override;

    int GetNearestNeighbor(int siteIndex, int nnIndex) override;

    unsigned int GetN() const override { return this->N; }

    unsigned int GetNbrNN() const override { return this->NbrNN; }

    unsigned int GetDim() const override { return this->Dim; }

    static const unsigned int Dim = 2;

    static const unsigned int NbrNN = 6;  /// vectors labeled by 1,2,...,NbrNN

    int GetOppositeDirection(int nnIndex) { return (nnIndex+1)%this->NbrNN+1; }
};

#endif // TRIANGULARLATTICE_H
