#include "SurfaceMesh.hpp"


namespace Slic3r {


Halfedge_index SurfaceMesh::opposite(Halfedge_index h) const
{
    if (h.is_invalid())
        return h;

    int face_idx = m_face_neighbors[h.m_face][h.m_side];
    Halfedge_index h_candidate = halfedge(Face_index(face_idx));

    if (h_candidate.is_invalid())
        return Halfedge_index(); // invalid

    for (int i=0; i<3; ++i) {
        if (is_same_vertex(source(h_candidate), target(h))) {
            // Meshes in PrusaSlicer should be fixed enough for the following not to happen.
            assert(is_same_vertex(target(h_candidate), source(h)));
            return h_candidate;
        }
        h_candidate = next(h_candidate);
    }
    return Halfedge_index(); // invalid
}



Halfedge_index SurfaceMesh::halfedge(Vertex_index source, Vertex_index target) const
{
    assert(false); // TODO: implement
    return Halfedge_index();
}



} // namespace Slic3r
