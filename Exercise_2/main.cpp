#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"

using namespace std;
using namespace Eigen;

using namespace PolygonalLibrary;

int main()
{
    PolygonalMesh mesh;

    string filepath = "PolygonalMesh";

    double epsilon_resizing = 11.7;
    if(!ImportMesh(filepath, mesh, epsilon_resizing))
    {
        cerr << "Invalid mesh" << endl;
        return 1;
    }
    cout << "Valid mesh";
    return 0;
}

