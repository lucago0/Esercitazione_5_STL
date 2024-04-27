#include "Utils.hpp"
#include "PolygonalMesh.hpp"
#include <iostream>

using namespace PolMeshLibrary;
using namespace std;
using namespace Eigen;

int main()
{
    PolygonalMesh mesh;
    string path = "PolygonalMesh";

    if (!ImportMesh(path, mesh)){
        cerr << "Error in memorizing the mesh." << endl;
        return 1;
    }

    if (!TestMesh(mesh)){
        cerr << "Error in the mesh structure." << endl;
        return 1;
    }
    return 0;

}
