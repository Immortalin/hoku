/// @file spherical-triangle.h
/// @author Glenn Galvizo
///
/// Header file for SphericalTriangle class, which matches a set of body vectors (stars) to their inertial counter-parts
/// in the database.

#ifndef HOKU_SPHERICAL_TRIANGLE_H
#define HOKU_SPHERICAL_TRIANGLE_H

#include "benchmark/benchmark.h"
#include "base-triangle.h"

/// @brief Star identification class using spherical triangles.
///
/// The spherical triangle class is an implementation of Cole and Crassidus's Spherical Triangle method with Tappe's
/// DMT process for alignment determination. This is one of the six star identification procedures being tested.
///
/// @example
/// @code{.cpp}
/// // Find all stars around a random star within 7.5 degrees of it. Rotate all stars by same random rotation.
/// Benchmark b(15, Star::chance(), Rotation::chance());
///
/// // Append 2 extra stars to the data-set above.
/// b.add_extra_light(2);
///
/// // Determine an identification. 'A' contains the body set with catalog label attached.
/// Star::list a = Sphere(b, Sphere::DEFAULT_PARAMETERS).identify();
/// for (const Star &s : a) { std::cout << s.str(); << std::endl; }
///
/// // Extract an attitude instead of an identification.
/// Rotation q = Sphere(b, Sphere::DEFAULT_PARAMETERS).align();
/// @endcode
class SphericalTriangle : public BaseTriangle {
  public:
    using BaseTriangle::Parameters;
    static const Parameters DEFAULT_PARAMETERS;
    
    SphericalTriangle (const Benchmark &, const Parameters &);
    
    std::vector<labels_list> query (const Star::list &s);
    Star::list reduce ();
    Star::list identify ();
    
    static int generate_table (INIReader &cf);
    
    static const int DEFAULT_TD_H;
    static const unsigned int QUERY_STAR_SET_SIZE;

#if !defined ENABLE_TESTING_ACCESS
    private:
#endif
    std::vector<Star::trio> query_for_trios (const index_trio &);
};

/// Alias for the SphericalTriangle class. 'Sphere' distinguishes the process I am testing here enough from the 5 other
/// methods.
typedef SphericalTriangle Sphere;

#endif /* HOKU_SPHERICAL_TRIANGLE_H */