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
 **********************************************************************/

#include <geos/operation/overlayng/LineBuilder.h>

#include <geos/operation/overlayng/OverlayEdge.h>
#include <geos/operation/overlayng/OverlayLabel.h>
#include <geos/operation/overlayng/OverlayNG.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/CoordinateArraySequence.h>
#include <geos/geom/CoordinateSequence.h>



namespace geos {      // geos
namespace operation { // geos.operation
namespace overlayng { // geos.operation.overlayng

using namespace geos::geom;

/*public*/
std::vector<std::unique_ptr<LineString>>
LineBuilder::getLines()
{
    markResultLines();
    addResultLines();

    // Transfer ownership of all the lines to the
    // caller
    return std::move(lines);
}

/*private*/
void
LineBuilder::markResultLines()
{
    std::vector<OverlayEdge*>& edges = graph->getEdges();
    for (OverlayEdge* edge : edges) {
        if (isInResult(edge))
            continue;
        if (isResultLine(edge->getLabel())) {
            edge->markInResultLine();
        }
    }
}

/*private*/
bool
LineBuilder::isInResult(OverlayEdge* edge) const
{
    return edge->isInResult() || edge->symOE()->isInResult();
}

/*private*/
bool
LineBuilder::isResultLine(const OverlayLabel* lbl) const
{
    /**
    * Edges which are just collapses along boundaries
    * are not output.
    * In other words, an edge must be from a source line
    * or two (coincident) area boundaries.
    */
    if (lbl->isBoundaryCollapse())
        return false;

    /**
    * Skip edges that are inside result area, if there is one.
    * It is sufficient to check against an input area rather
    * than the result area, since
    * if lines are being included then the result area
    * must be the same as the input area.
    * This logic relies on the semantic that if both inputs
    * are areas, lines are only output if there is no
    * result area.
    */
    if (hasResultArea && lbl->isLineInArea(inputAreaIndex))
        return false;

    Location aLoc = effectiveLocation(0, lbl);
    Location bLoc = effectiveLocation(1, lbl);

    bool inResult = OverlayNG::isResultOfOp(opCode, aLoc, bLoc);
    return inResult;
}

/*private*/
Location
LineBuilder::effectiveLocation(int geomIndex, const OverlayLabel* lbl) const
{
    if (lbl->isCollapse(geomIndex)) {
        return Location::INTERIOR;
    }
    if (lbl->isLine(geomIndex)) {
        return Location::INTERIOR;
    }
    return lbl->getLineLocation(geomIndex);
}

/*private*/
void
LineBuilder::addResultLines()
{
    addResultLinesForNodes();
    addResultLinesRings();
}

/**
* FUTURE: To implement a strategy preserving input lines,
* the label must carry an id for each input LineString.
* The ids are zeroed out whenever two input edges are merged.
* Additional result nodes are created where there are changes in id
* at degree-2 nodes.
* (degree>=3 nodes must be kept as nodes to ensure
* output linework is fully noded.
*/

/*private*/
void
LineBuilder::addResultLinesForNodes()
{
    std::vector<OverlayEdge*>& edges = graph->getEdges();
    for (OverlayEdge* edge : edges) {
        if (! edge->isInResultLine())
            continue;
        if (edge->isVisited())
            continue;

        /**
        * Choose line start point as a node.
        * Nodes in the line graph are degree-1 or degree >= 3 edges.
        * This will find all lines originating at nodes
        */
        if (degreeOfLines(edge) != 2) {
            std::unique_ptr<LineString> line = buildLine(edge);
            lines.push_back(std::move(line));
        }
    }
}

/*private*/
void
LineBuilder::addResultLinesRings()
{
    // TODO: an ordering could be imposed on the endpoints to make this more repeatable
    // TODO: preserve input LinearRings if possible?  Would require marking them as such
    std::vector<OverlayEdge*>& edges = graph->getEdges();
    for (OverlayEdge* edge : edges) {
    if (! edge->isInResultLine())
        continue;
    if (edge->isVisited())
        continue;

    lines.push_back(buildLine(edge));
    }
}

/*private*/
std::unique_ptr<LineString>
LineBuilder::buildLine(OverlayEdge* node)
{
    // assert: edgeStart degree = 1
    // assert: edgeStart direction = forward
    std::unique_ptr<CoordinateArraySequence> pts(new CoordinateArraySequence());
    pts->add(node->orig(), false);

    bool isNodeForward = node->isForward();

    OverlayEdge* e = node;
    do {
        e->markVisitedBoth();
        e->addCoordinates(pts.get());

        // end line if next vertex is a node
        if (degreeOfLines(e->symOE()) != 2) {
            break;
        }
        e = nextLineEdgeUnvisited(e->symOE());
        // e will be nullptr if next edge has been visited, which indicates a ring
    }
    while (e != nullptr);
    // reverse coordinates before constructing
    if(!isNodeForward) {
        CoordinateSequence::reverse(pts.get());
    }

    return geometryFactory->createLineString(std::move(pts));
}

/*private*/
OverlayEdge*
LineBuilder::nextLineEdgeUnvisited(OverlayEdge* node) const
{
    OverlayEdge* e = node;
    do {
        e = e->oNextOE();
        if (e->isVisited()) {
            continue;
        }
        if (e->isInResultLine()) {
            return e;
        }
    }
    while (e != node);
    return nullptr;
}

/*private*/
int
LineBuilder::degreeOfLines(OverlayEdge* node) const
{
    int degree = 0;
    OverlayEdge* e = node;
    do {
        if (e->isInResultLine()) {
            degree++;
        }
        e = e->oNextOE();
    }
    while (e != node);
    return degree;
}






} // namespace geos.operation.overlayng
} // namespace geos.operation
} // namespace geos
