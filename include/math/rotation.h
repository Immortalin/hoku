/// @file rotation.h
/// @author Glenn Galvizo
///
/// Header file for Rotation class, which represents rotations on 3D star vectors using quaternions.

#ifndef HOKU_ROTATION_H
#define HOKU_ROTATION_H

#include "math/star.h"

/// The rotation class uses the vector functions in the star class to form quaternions. Rotations allow us to
/// simulate a true lost-in-space condition.
///
/// @example
/// @code{.cpp}
/// // Rotate {1, 1, 1} by a random rotation.
/// std::random_device seed;
/// printf("%s", Star::rotate(Star(1, 1, 1), Rotation::chance(seed)).str());
///
/// Star a = Star::chance(seed), b = Star::chance(seed), c, d;
/// Rotation e = Rotation::chance(seed), f;
///
/// // Star C is Star A rotated by Quaternion E. Star D is Star B rotated by Quaternion E.
/// c = Rotation::rotate(a, e);
/// d = Rotation::rotate(b, e);
///
/// // F is the rotation to take AB frame to CD. F == E.
/// f = Rotation::rotation_across_frames({a, b}, {c, d});
///
/// // Result should show the same star.
/// printf("%s : %s", Rotation::rotate(a, e).str(), Rotation::rotate(a, f).str());
/// @endcode
class Rotation {
  public:
    /// Force default constructor. Default is {1, 0, 0, 0} (identity).
    Rotation () = default;
  
  public:
    bool operator== (const Rotation &) const;
    
    static Star rotate (const Star &, const Rotation &);
    static double angle_between (const Rotation &, const Rotation &);
    static Star rotation_difference (const Rotation &, const Rotation &, const Star &);
    
    static Rotation identity ();
    static Rotation chance (std::random_device &);
    
    static Rotation rotation_across_frames (const Star::pair &, const Star::pair &);

#if !defined ENABLE_TESTING_ACCESS
  private:
#endif
    /// Precision default for '==' method.
    constexpr static double ROTATION_EQUALITY_PRECISION_DEFAULT = 0.000000000001;
    
    /// Matrix alias, by using a 3-element array of 3D vectors.
    using matrix = std::array<Star, 3>;

#if !defined ENABLE_TESTING_ACCESS
  private:
#endif
    Rotation (double, const Star &, bool = false);
    
    static Rotation multiply (const Rotation &, const Rotation &);
    static Rotation matrix_to_quaternion (const matrix &);
    static matrix matrix_multiply_transpose (const matrix &, const matrix &);

#if !defined ENABLE_TESTING_ACCESS
  private:
#endif
    /// W component, or the sole real component of a quaternion. Defaults to one (identity quaternion).
    double w = 1;
    
    /// I component (element 0) of quaternion. Defaults to zero (identity quaternion).
    double i = 0;
    
    /// J component (element 1) of quaternion. Defaults to zero (identity quaternion).
    double j = 0;
    
    /// K component (element 2) of quaternion. Defaults to zero (identity quaternion).
    double k = 0;
};

#endif /* HOKU_ROTATION_H */
