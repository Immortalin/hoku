/// @file base-triangle.h
/// @author Glenn Galvizo
///
/// Header file for BaseTriangle class, which holds all common methods between the Planar and Spherical triangle
/// methods (they are very similar).

#ifndef HOKU_BASE_TRIANGLE_H
#define HOKU_BASE_TRIANGLE_H

#include "identification.h"
#include "math/trio.h"
#include <iostream>
#include <functional>

/// The base triangle class is a base class for Crassidis and Cole's Planar and Spherical Pattern Recognition Process.
/// These are two of the five star identification procedures being tested.
class BaseTriangle : public Identification {
  public:
    /// Alias for a list of catalog IDs (STL vector of doubles).
    using label_list = std::vector<double>;
    
    /// Alias for a trio of catalog IDs (3-element STL array of doubles).
    using label_trio = std::array<double, 3>;
  
  protected:
    /// Alias for a trio of index numbers for the input star list (3-element STL array of doubles).
    using index_trio = std::array<double, 3>;
    
    /// Alias for an area function in the Trio class.
    using area_function = std::function<double (const Star &, const Star &, const Star &)>;
    
    /// Alias for a moment function in the Trio class.
    using moment_function = std::function<double (const Star &, const Star &, const Star &)>;
  
  protected:
    std::vector<label_trio> e_query (double a, double i);
    Rotation e_alignment (const Star::list &candidates, const Trio::stars &r, const Trio::stars &b);
    label_trio e_reduction ();
    Rotation e_attitude ();
    Star::list e_crown ();
    
    static int generate_triangle_table (double fov, const std::string &table_name, area_function compute_area,
                                        moment_function compute_moment);
    std::vector<Trio::stars> m_stars (const index_trio &i_b, area_function compute_area,
                                      moment_function compute_moment);
  
  private:
    std::vector<label_trio> query_for_trio (double a, double i);
    virtual std::vector<Trio::stars> match_stars (const index_trio &) = 0;
    index_trio permutate_index (const index_trio &);
    Trio::stars pivot (const index_trio &, const std::vector<Trio::stars> & = {});
    Star::list check_assumptions (const Star::list &, const Trio::stars &, const index_trio &);
    
};

#endif /* HOKU_BASE_TRIANGLE_H */