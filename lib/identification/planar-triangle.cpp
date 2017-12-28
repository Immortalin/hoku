/// @file planar-triangle.cpp
/// @author Glenn Galvizo
///
/// Source file for PlanarTriangle class, which matches a set of body vectors (stars) to their inertial counter-parts in
/// the database.

#include "identification/planar-triangle.h"

/// Default parameters for the planar triangle identification method.
const Identification::Parameters Plane::DEFAULT_PARAMETERS = {DEFAULT_SIGMA_QUERY, DEFAULT_SQL_LIMIT,
    DEFAULT_SIGMA_OVERLAY, DEFAULT_GAMMA, DEFAULT_NU_MAX, DEFAULT_NU, "PLANE_20"};

/// Constructor. Sets the benchmark data and fov. Sets the parameters and working table.
///
/// @param input Working Benchmark instance. We are **only** copying the star set and the fov.
/// @param parameters Parameters to use for identification.
PlanarTriangle::PlanarTriangle (const Benchmark &input, const Parameters &parameters) {
    input.present_image(this->input, this->fov);
    this->parameters = parameters;
    
    ch.select_table(this->parameters.table_name);
}

/// Generate the triangle table given the specified FOV and table name. This find the area and polar moment
/// between each distinct permutation of trios, and only stores them if they fall within the corresponding
/// field-of-view.
///
/// @param fov Field of view limit (degrees) that all pairs must be within.
/// @param table_name Name of the table to generate.
/// @return TABLE_ALREADY_EXISTS if the table already exists. Otherwise, 0 when finished.
int PlanarTriangle::generate_table (const double fov, const std::string &table_name) {
    return generate_triangle_table(fov, table_name, Trio::planar_area, Trio::planar_moment);
}

/// Given a trio of body stars, find matching trios of inertial stars using their respective planar areas and polar
/// moments.
///
/// @param i_b Index trio of stars in body (B) frame.
/// @return NO_CANDIDATE_STARS_FOUND if stars are not within the fov or if no matches currently exist.
/// Otherwise, vector of trios whose areas and moments are close.
std::vector<Trio::stars> Plane::match_stars (const index_trio &i_b) {
    return m_stars(i_b, Trio::planar_area, Trio::planar_moment);
}

/// Find the matching pairs using the appropriate triangle table and by comparing areas and polar moments. Input image
/// is not used. We require the following be defined:
///
/// @code{.cpp}
///     - table_name
///     - sigma_query
///     - sql_limit
/// @endcode
///
/// @param s Stars to query with. This must be of length = 3.
/// @return Vector of likely matches found by the planar triangle method.
std::vector<Identification::labels_list> Plane::experiment_query (const Star::list &s) {
    if (s.size() != 3) {
        throw "Input list does not have exactly three stars.";
    }
    
    std::vector<label_trio> h = e_query(Trio::planar_area(s[0], s[1], s[2]), Trio::planar_moment(s[0], s[1], s[2]));
    std::vector<labels_list> h_bar = {};
    
    // Convert our labels to lists.
    std::for_each(h.begin(), h.end(), [&h_bar] (const label_trio &ell) {
        h_bar.emplace_back(labels_list {ell[0], ell[1], ell[2]});
    });
    return h_bar;
}

/// Check all possible configuration of star trios and return quaternion corresponding to the set with the largest
/// number of reference to body matches. Input image is used. We require the following be defined:
///
/// @code{.cpp}
///     - sigma_overlay
/// @endcode
///
/// @param candidates All stars found near the inertial pair.
/// @param r Inertial (frame R) pair of stars that match the body pair. This must be of length = 3.
/// @param b Body (frame B) pair of stars that match the inertial pair. This must be of length = 3.
/// @return Body stars b with the attached labels of the inertial pair r.
Star::list Plane::experiment_first_alignment (const Star::list &candidates, const Star::list &r, const Star::list &b) {
    if (r.size() != 3 || b.size() != 3) {
        throw "Input lists does not have exactly three stars.";
    }
    return e_single_alignment(candidates, {r[0], r[1], r[2]}, {b[0], b[1], b[2]});
}

/// Find the **best** matching pair to the first three stars in our benchmark using the appropriate triangle table.
/// Assumes noise is normally distributed, searches using epsilon (3 * sigma_a) and a basic bounded query. Input image
/// is used. We require the following be defined:
///
/// @code{.cpp}
///     - table_name
///     - sigma_query
///     - sql_limit
/// @endcode
///
/// @return NO_CANDIDATES_FOUND if we cannot query anything. Otherwise, a single match configuration found by the
/// planar triangle method.
Identification::labels_list Plane::experiment_reduction () {
    return e_reduction();
}

/// Find the rotation from the images in our current benchmark to our inertial frame (i.e. the catalog). Input image is
/// used. We require the following be defined:
///
/// @code{.cpp}
///     - table_name
///     - sigma_overlay
///     - sql_limit
///     - sigma_query
///     - nu
///     - nu_max
/// @endcode
///
/// @param input The set of benchmark data to work with.
/// @param p Adjustments to the identification process.
/// @return NO_CONFIDENT_ALIGNMENT if an alignment cannot be found exhaustively. EXCEEDED_NU_MAX if an alignment
/// cannot be found within a certain number of query picks. Otherwise, body stars b with the attached labels
/// of the inertial pair r.
Star::list Plane::experiment_alignment () {
    return e_alignment();
}

/// Match the stars found in the current benchmark to those in the Nibble database. The child class should wrap this
/// function as 'experiment_crown' to mimic the other methods. All parameters must be defined.
///
/// @return NO_CONFIDENT_ALIGNMENT if an alignment cannot be found exhaustively. EXCEEDED_NU_MAX if an alignment
/// cannot be found within a certain number of query picks. Otherwise, a vector of body stars with their
/// inertial catalog IDs that qualify as matches.
Star::list Plane::experiment_crown () {
    return e_crown();
}