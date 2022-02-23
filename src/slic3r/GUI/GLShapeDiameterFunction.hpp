#ifndef slic3r_GLShapeDiameterFunction_hpp_
#define slic3r_GLShapeDiameterFunction_hpp_

#include <memory>

#include <libslic3r/Model.hpp>
#include <libslic3r/Point.hpp>
#include <libslic3r/AABBTreeIndirect.hpp>
#include <libslic3r/ShapeDiameterFunction.hpp>
#include <libslic3r/NormalUtils.hpp>

namespace Slic3r::GUI {
// Shapira, Lior, Ariel Shamir, and Daniel Cohen-Or. 
// "Consistent mesh partitioning and skeletonisation using the shape diameter function." 
// The Visual Computer 24.4 (2008): 249-259.
class GLShapeDiameterFunction
{
    bool         is_calculated = false;
    bool         enabled       = false;
    bool         initialized   = false;
    unsigned int m_vbo_id      = 0;
    unsigned int m_vbo_indices_id = 0;
public:
    ShapeDiameterFunction::Config cfg;

    // create ray directions
    float angle = 120; // [in deg] in range from 1 to 179
    size_t count_samples = 60; // count samples on half sphere

    // visualization of normal
    bool allow_render_normals = false;
    float normal_width = 0.1f;
    float normal_length = .5f;

    bool allow_divide_triangle = true;
    bool allow_remesh = true;
    bool allow_render_points = false;
public:
    GLShapeDiameterFunction() = default; // set default values

    void set_enabled(bool enable) { enabled = enable; };
    bool is_enabled() const { return enabled; };
    const indexed_triangle_set &get_triangle_set() const { return tree.vertices_indices; }
    
    void draw() const;

    // create vertex shader data for draw
    bool initialize_model(const ModelObject *mo);
    // call after change max_triangle_size
    bool divide(); // subdivide triangles
    // call when change source for normals
    bool initialize_normals();
    // calculate width(tree and normals are already calculated)
    bool initialize_width();

    void sample_surface();
private:
    bool initialize_indices();
    void render_normals() const;
    void render_points() const;
    void render_rays() const;

    // structure uploaded to GPU
    struct Vertex
    {
        Vec3f position;
        Vec3f normal;
        float width;
        Vertex(Vec3f position, Vec3f normal, float width)
            : position(position), normal(normal), width(width)
        {}

        static unsigned int size(){ return sizeof(Vertex);}
        static size_t position_offset() {return size_t{0}; }
        static size_t normal_offset() { return (size_t)(sizeof(Vec3f)); }
        static size_t width_offset() { return (size_t)(2 * sizeof(Vec3f)); }
    };

public: 
    // draw information
    size_t indices_count = 0;

    // normals for each vertex of mesh
    ShapeDiameterFunction::IndexTriangleNormals triangles;

    // tree for ray cast
    ShapeDiameterFunction::AABBTree tree;

    // results of SDF
    std::vector<float> widths;

    // sampled points
    ShapeDiameterFunction::PointRadiuses points;

    // for show information about count of generated samples
    size_t count_generated_points = 0; 
};

} // namespace Slic3r::GUI
#endif // slic3r_GLShapeDiameterFunction_hpp_