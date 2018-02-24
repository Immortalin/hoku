/// @file star.h
/// @author Glenn Galvizo
///
/// Header file for Star class, which represents three-dimensional star vectors.

#ifndef HOKU_STAR_H
#define HOKU_STAR_H

#include <array>
#include <vector>
#include <limits>
#include "third-party/gmath/Vector3.hpp"

/// @brief Class for 3D vector representation of stars.
///
/// The star class is really a wrapper for 3D vectors, with additional methods for the attachment of labels and
/// magnitudes.  This is the basis for all of the Hoku research.
///
/// @example
/// @code{.cpp}
/// // Define stars (in order): {0, 0, 0}, {random, random, random}, {0, 0, 0}, {-0.680414, 0.680414, 0.272166}
/// Star s_1, s_2 = Star::chance(), s_3 = Star::zero(), s_4(-10, 10, 4, 0, 0, true);
///
/// // Cross stars {-2, -1, 0} and {3, 2, 1} to produce {-1, 2, -1}.
/// std::cout << Star::cross(Star(-2, -1, 0), Star(3, 2, 1)) << std::endl;
///
/// // Add star {1, 1, 1} to star {5, 5, 5}. Subtract result by {2, 2, 2} to get {4, 4, 4}.
/// std::cout << Star(5, 5, 5) + Star(1, 1, 1) - Star(2, 2, 2) << std::endl;
///
/// // Determine angle between Star {2, 3, 5} and {5, 6, 7} to get 0.9744339542.
/// std::cout << Star::angle_between(Star(2, 3, 5), Star(5, 6, 7)) << std::endl;
/// @endcode
class Star : public Vector3 {
  public:
    /// List type, defined as a vector of Stars.
    using list = std::vector<Star>;
    
    /// Pair type, defined as a 2-element array of Stars.
    using pair = std::array<Star, 2>;
    
    /// Trio type, defined as a 3-element array of Stars.
    using trio = std::array<Star, 3>;
    
    /// Quad type, defined as a 4-element array of Stars.
    using quad = std::array<Star, 4>;
    
    /// Returned when a user attempts to access an item using the [] operator for n > 1.
    static constexpr double INVALID_ELEMENT_ACCESSED = 0;
    
    /// The default label of all stars.
    static constexpr int NO_LABEL = 0;
    
    /// The default magnitude of all stars (brighter than our Sun).
    static constexpr double NO_MAGNITUDE = -30.0;
  
  public:
    Star (double i, double j, double k, int label = NO_LABEL, double m = NO_MAGNITUDE);
    Star ();
    
    static Star VStar (Vector3 v, int label = NO_LABEL, double m = NO_MAGNITUDE);
    
    friend std::ostream &operator<< (std::ostream &, const Star &);
    double operator[] (unsigned int n) const;
    int get_label () const;
    double get_magnitude () const;
    
    static Star chance ();
    static Star chance (int label);
    
    static bool within_angle (const Star &s_1, const Star &s_2, double theta);
    static bool within_angle (const list &s_l, double theta);
    
    static Star define_label (const Star &s, int label);
    static Star reset_label (const Star &s);

#if !defined ENABLE_TESTING_ACCESS
  private:
#endif
    /// Catalog specific ID for the given star.
    int label;
    
    /// Apparent magnitude for the given star.
    double m;
};

#endif /* HOKU_STAR_H */
