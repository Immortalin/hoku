/*
 * @file: star.h
 *
 * @brief: Header file for Star class, which represents three-dimensional star vectors.
 */

#ifndef HOKU_STAR_H
#define HOKU_STAR_H

#define _USE_MATH_DEFINES

#include <array>
#include <random>

/*
 * @class Star
 * @brief Star class, which is represented by 3-dimensional vectors and an identification number.
 *
 * The star class is really a 3D vector class in disguise, with methods focusing toward rotation
 * and angular separation. This class is the basis for all of the Hoku research.
 */
class Star {
    public:
        // for spherical representation of vector
        struct Sphere {
            double r, theta, phi;
        };

        // for Mercator projections
        struct Mercator {
            double x, y;
        };

        // constructor, set components and HR, has unit flag
        Star(const double, const double, const double, const int = 0, const bool = false);
        Star();

        // get methods for i, j, k, and HR methods
        double operator[](const int) const;
        int get_hr() const;

        // add and subtract two vector
        Star operator+(const Star &) const;
        Star operator-(const Star &) const;

        // scale vector with a constant
        Star operator*(const double) const;

        // calculate norm of the current vector
        double norm() const;

        // return the current vector as one with norm = 1.0
        Star as_unit() const;

        // return current vector as spherical coordinates
        Sphere as_spherical() const;

        // return Mercator projected spherical coordinate
        static Mercator as_mercator(const Sphere &, const double);

        // determine if the **components** are within a certain value of each other
        static bool is_equal(const Star &, const Star &, const double = 0.000000000001);
        bool operator==(const Star &) const;

        // generate unit vector with random components, overloaded to add own HR number
        static Star chance();
        static Star chance(const int);

        // determine dot/cross product of two vectors
        static double dot(const Star &, const Star &);
        static Star cross(const Star &, const Star &);

        // find angle between two vectors, determine if two stars are within a certain angle
        static double angle_between(const Star &, const Star &);
        static bool within_angle(const Star &, const Star &, const double);

        // reset the HR number to 0
        static Star reset_hr(const Star &);

#ifndef DEBUGGING_MODE_IS_ON
        private:
#endif
        // individual components of 3D vector
        double i, j, k;

        // Harvard Revised number
        int hr;
};


#endif /* HOKU_STAR_H */
