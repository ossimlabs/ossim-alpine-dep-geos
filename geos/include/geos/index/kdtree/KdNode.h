/**********************************************************************
 *
 * GEOS - Geometry Engine Open Source
 * http://geos.osgeo.org
 *
 * Copyright (C) 2020 Paul Ramsey <pramsey@cleverelephant.ca>
 *
 * This is free software; you can redistribute and/or modify it under
 * the terms of the GNU Lesser General Public Licence as published
 * by the Free Software Foundation.
 * See the COPYING file for more information.
 *
 **********************************************************************
 *
 * Last port: index/kdtree/Node.java rev 1.8 (JTS-1.10)
 *
 **********************************************************************/

#pragma once

#include <geos/geom/Coordinate.h> // for composition

namespace geos {
namespace index { // geos::index
namespace kdtree { // geos::index::kdtree

class GEOS_DLL KdNode {

private:

    geom::Coordinate p;
    const void* data;
    KdNode* left;
    KdNode* right;
    std::size_t count;

public:

    KdNode(double p_x, double p_y, const void* p_data);
    KdNode(const geom::Coordinate& p_p, const void* p_data);

    double getX() { return p.x; }
    double getY() { return p.y; }
    const geom::Coordinate& getCoordinate() { return p; }
    const void* getData() { return data; }
    KdNode* getLeft() { return left; }
    KdNode* getRight() { return right; }
    void increment() { count++; }
    std::size_t getCount() { return count; }
    bool isRepeated() { return count > 1; }
    void setLeft(KdNode* p_left) { left = p_left; }
    void setRight(KdNode* p_right) { right = p_right; }

};

} // namespace geos::index::kdtree
} // namespace geos::index
} // namespace geos



