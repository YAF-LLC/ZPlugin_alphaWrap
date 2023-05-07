//
// Copyright (C) 2023 Kazutaka Nakashima (kazutaka.nakashima@n-taka.info)
// 
// GPLv3
//
// This file is part of alphaWrap.
// 
// alphaWrap is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// 
// alphaWrap is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with alphaWrap. If not, see <https://www.gnu.org/licenses/>.
//

#include "wrap.h"

#include "nlohmann/json.hpp"

#include <filesystem>
namespace fs = std::filesystem;

#include "readGoZAndTriangulate.h"
#include "alphaWrap.h"
#include "writeGoZFile.h"

// debug
#include <iostream>

extern "C" DLLEXPORT float wrap(char *someText, double optValue, char *outputBuffer, int optBuffer1Size, char *pOptBuffer2, int optBuffer2Size, char **zData)
{
    ////
    // parse parameter (JSON)
    fs::path jsonPath(someText);
    std::ifstream ifs(jsonPath.string());
    nlohmann::json json = nlohmann::json::parse(ifs);
    ifs.close();

    const std::string rootString = json.at("root").get<std::string>();
    fs::path rootPath(rootString);
    rootPath.make_preferred();

    // load GoZ file
    const std::string meshFileRelPathStr = json.at("meshFile").get<std::string>();
    fs::path meshFileRelPath(meshFileRelPathStr);
    fs::path meshFilePath(rootPath);
    meshFilePath /= meshFileRelPath;
    meshFilePath.make_preferred();

    std::string meshName;
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> V;
    Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> F;
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> UV_u;
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> UV_v;
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> VC;
    Eigen::Matrix<double, Eigen::Dynamic, 1> M;
    Eigen::Matrix<int, Eigen::Dynamic, 1> G;
    readGoZAndTriangulate(meshFilePath, meshName, V, F, UV_u, UV_v, VC, M, G);

    // load parameters
    const double alpha = json.at("ZBrush").at("alpha").get<double>();
    const double offset = json.at("ZBrush").at("offset").get<double>();
    const bool usePolygon = json.at("ZBrush").at("usePolygon").get<bool>();

    // convert dynamesh resolution-style alpha/offset to actual size
    // todo
    const double absAlpha = 2.0 / alpha;
    const double absOffset = 2.0 / offset;

    // apply alpha wrap
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> wrapV;
    Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> wrapF;

    alphaWrap(V, F, absAlpha, absOffset, usePolygon, wrapV, wrapF);

    // export GoZ file
    //   covert matrix to vector style
    std::vector<std::vector<double>>
        wrapVVec(wrapV.rows(), std::vector<double>(wrapV.cols()));
    for (int v = 0; v < wrapV.rows(); ++v)
    {
        for (int xyz = 0; xyz < wrapV.cols(); ++xyz)
        {
            wrapVVec.at(v).at(xyz) = wrapV(v, xyz);
        }
    }

    std::vector<std::vector<int>> wrapFVec(wrapF.rows(), std::vector<int>(wrapF.cols()));
    for (int f = 0; f < wrapF.rows(); ++f)
    {
        for (int fv = 0; fv < wrapF.cols(); ++fv)
        {
            wrapFVec.at(f).at(fv) = wrapF(f, fv);
        }
    }

    // update meshName
    fs::path exportMeshFilePath(meshFilePath.parent_path());
    {
        exportMeshFilePath /= meshFilePath.stem();
        exportMeshFilePath += "_wrap";
        exportMeshFilePath += meshFilePath.extension();
    }

    std::string exportMeshName(meshName);
    exportMeshName += "_wrap";

    FromZ::writeGoZFile(exportMeshFilePath.string(), exportMeshName, wrapVVec, wrapFVec);

    return 1.0f;
}
