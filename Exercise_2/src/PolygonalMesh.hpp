#pragma once

#include <iostream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;


namespace PolygonalLibrary {


struct PolygonalMesh
{
    //Cell 0D

    unsigned int NumberCell0D = 0;
    vector<unsigned int> Cell0DId = {};
    vector<Vector2d> Cell0DCoordinates = {};
    map<unsigned int, list<unsigned int>> Cell0DMarkers = {};


    //Cell 1D
    unsigned int NumberCell1D = 0;
    vector<unsigned int> Cell1DId = {};
    vector<Vector2i> Cell1DVertices = {};
    map<unsigned int, list<unsigned int>> Cell1DMarkers = {};


    //Cell 2D
    unsigned int NumberCell2D = 0;
    vector<unsigned int> Cell2DId = {};
    vector<VectorXi> Cell2DVertices = {};
    vector<VectorXi> Cell2DEdges = {};
};

}
