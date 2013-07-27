/**
 * \file
 * \author Norihiro Watanabe
 * \date   2013-04-16
 * \brief
 *
 * \copyright
 * Copyright (c) 2013, OpenGeoSys Community (http://www.opengeosys.org)
 *            Distributed under a Modified BSD License.
 *              See accompanying file LICENSE.txt or
 *              http://www.opengeosys.org/project/license
 *
 */


#ifndef MESHITEM_H_
#define MESHITEM_H_

namespace VecMatOnMeshLib
{

enum class MeshItemType { Node, Edge, Face, Cell };

inline std::ostream& operator<<(std::ostream& os, MeshItemType const& t)
{
	switch (t)
	{
	case MeshItemType::Node: return os << "N";
	case MeshItemType::Edge: return os << "E";
	case MeshItemType::Face: return os << "F";
	case MeshItemType::Cell: return os << "C";
	};
	return os;
}

/** Spatial location description.
 *
 * The spatial location is given by a mesh by its \c mesh_id, item's type (face,
 * cell, etc. see MeshItemType), and item's number by its \c item_id.
 */
struct Location
{
    std::size_t          mesh_id;
    MeshItemType         item_type;
    std::size_t          item_id;

    Location(std::size_t meshid, MeshItemType itemtype, std::size_t itemid)
    : mesh_id(meshid), item_type(itemtype), item_id(itemid){};
};

/** Lexicographic order of Location.
 */
inline bool operator<(const Location& left, const Location& right)
{
    if (left.mesh_id != right.mesh_id) return left.mesh_id < right.mesh_id;
    if (left.item_type != right.item_type) return left.item_type < right.item_type;
    return left.item_id < right.item_id;
}


}


#endif /* MESHITEM_H_ */
