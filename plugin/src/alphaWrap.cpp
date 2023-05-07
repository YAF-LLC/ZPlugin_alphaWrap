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

#include "alphaWrap.h"

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/alpha_wrap_3.h>
#include <CGAL/IO/read_points.h>
#include <CGAL/Real_timer.h>
// #include <iostream>
#include <string>
#include <vector>
namespace AW3 = CGAL::Alpha_wraps_3;
using K = CGAL::Exact_predicates_inexact_constructions_kernel;
using Point_3 = K::Point_3;
using Point_container = std::vector<Point_3>;
using Triangle_container = std::vector<std::vector<int>>;
using Mesh = CGAL::Surface_mesh<Point_3>;

template <
    typename DerivedV,
    typename DerivedF,
    typename DerivedWV,
    typename DerivedWF>
inline void alphaWrap(
    const Eigen::MatrixBase<DerivedV> &V,
    const Eigen::MatrixBase<DerivedF> &F,
    const typename DerivedV::Scalar &alpha,
    const typename DerivedV::Scalar &offset,
    const bool &usePolygon,
    Eigen::PlainObjectBase<DerivedWV> &wrapV,
    Eigen::PlainObjectBase<DerivedWF> &wrapF)
{
    Point_container points;
    points.resize(V.rows());
    for (int vIdx = 0; vIdx < V.rows(); ++vIdx)
    {
        points.at(vIdx) = Point_3(V(vIdx, 0), V(vIdx, 1), V(vIdx, 2));
    }

    Triangle_container faces;
    faces.resize(F.rows());
    for (int fIdx = 0; fIdx < F.rows(); ++fIdx)
    {
        faces.at(fIdx) = std::vector<int>({F(fIdx, 0), F(fIdx, 1), F(fIdx, 2)});
    }

    // Construct the wrap
    Mesh wrap;
    if (usePolygon)
    {
        CGAL::alpha_wrap_3(points, faces, alpha, offset, wrap);
    }
    else
    {
        CGAL::alpha_wrap_3(points, alpha, offset, wrap);
    }

    wrapV.resize(num_vertices(wrap), 3);
    wrapF.resize(num_faces(wrap), 3);

    Mesh::Vertex_range r = wrap.vertices();
    int vIdx = 0;
    for (auto &vIter = r.begin(); vIter != r.end(); ++vIter)
    {
        const Point_3 &point = wrap.point(*vIter);
        wrapV(vIdx, 0) = point.x();
        wrapV(vIdx, 1) = point.y();
        wrapV(vIdx, 2) = point.z();
        ++vIdx;
    }

    Mesh::Face_range f = wrap.faces();
    int fIdx = 0;
    for (auto &fIter = f.begin(); fIter != f.end(); ++fIter)
    {
        CGAL::Iterator_range<CGAL::Vertex_around_face_iterator<Mesh>> fv = CGAL::vertices_around_face(wrap.halfedge(*fIter), wrap);
        int fvIdx = 0;
        for (auto &vIter = fv.begin(); vIter != fv.end(); ++vIter)
        {
            // alpha wrap in CGAL guarantees that the output is triangulated
            // https://doc.cgal.org/latest/Alpha_wrap_3/index.html#title3
            wrapF(fIdx, fvIdx) = *vIter;
            ++fvIdx;
        }
        ++fIdx;
    }
}
