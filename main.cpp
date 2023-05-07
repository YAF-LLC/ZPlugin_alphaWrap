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

#include <fstream>
#include <string>
#include "nlohmann/json.hpp"

#include "wrap.h"

int main()
{
    nlohmann::json json;
    json["root"] = ".";
    json["meshFile"] = "example/Dog.GoZ";
    json["ZBrush"] = nlohmann::json::object();
    json["ZBrush"]["alpha"] = 200.0;
    json["ZBrush"]["offset"] = 200.0;
    json["ZBrush"]["usePolygon"] = true;

    std::ofstream ofs("build/Release/parameters.txt");
    ofs << json.dump(4) << std::endl;
    ofs.close();

    char buf[255];
    char **zData = nullptr;
    wrap("build/Release/parameters.txt", 0.0, buf, 0, buf, 0, zData);
    return 0;

    // char buf[255];
    // char **zData = nullptr;
    // resample("/APPLICATIONS/ZBRUSH 2022/ZSTARTUP/ZPlugs64/remeshWithDualContouring_2022/data/parameters.txt", 0.0, buf, 0, buf, 0, zData);
    // return 0;
}
