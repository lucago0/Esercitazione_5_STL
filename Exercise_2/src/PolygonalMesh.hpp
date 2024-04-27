#pragma once
#include <iostream>
#include <vector>
#include <Eigen/Eigen>

using namespace std;
using namespace Eigen;

namespace PolMeshLibrary{
struct PolygonalMesh{
    unsigned int NumberCell0Ds = 0;
    vector<unsigned int> IdCell0Ds = {};
    vector<Vector2d> CoordinateCell0Ds = {};
    map<unsigned int, list<unsigned int>> MarkerCell0Ds = {};

    unsigned int NumberCell1Ds = 0;
    vector<unsigned int> IdCell1Ds = {};
    vector<Vector2i> VerticesCell1Ds = {};
    map<unsigned int, list<unsigned int>> MarkerCell1Ds = {};

    unsigned int NumberCell2Ds = 0;
    vector<unsigned int> IdCell2Ds = {};
    vector<VectorXi> VerticesCell2Ds = {};
    vector<VectorXi> EdgesCell2Ds = {};
    map<unsigned int, list<unsigned int>> MarkerCell2Ds = {};

    double tau = numeric_limits<double>::epsilon();
    double tol = tau*tau/2;
};
}
