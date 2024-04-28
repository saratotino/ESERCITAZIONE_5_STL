#pragma once

#include <iostream>
#include "PolygonalMesh.hpp"

using namespace std;

namespace PolygonalLibrary {

bool ImportMesh(const string &filepath, PolygonalMesh& mesh, double& resizing);

bool ImportCell0Ds(const string &filename, PolygonalMesh& mesh);

bool ImportCell1Ds(const string &filename, PolygonalMesh& mesh);

bool ImportCell2Ds(const string &filename, PolygonalMesh& mesh);

bool CheckLength(PolygonalMesh& mesh, double& resizing);

bool CheckVertices(PolygonalMesh& mesh);

bool CheckArea(PolygonalMesh& mesh, double& resizing);

}
