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

#ifndef READ_GOZ_AND_TRIANGULATE_H
#define READ_GOZ_AND_TRIANGULATE_H

#include <filesystem>
namespace fs = std::filesystem;

#include "Eigen/Core"

void readGoZAndTriangulate(
	const fs::path &GoZFilePath,
	std::string &meshName,
	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> &V,
	Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> &F,
	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> &UV_u,
	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> &UV_v,
	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> &VC,
	Eigen::Matrix<double, Eigen::Dynamic, 1> &M,
	Eigen::Matrix<int, Eigen::Dynamic, 1> &G);

#endif
