/// @file test-pyramid.cpp
/// @author Glenn Galvizo
///
/// Source file for all Pyramid class unit tests and the test runner.

#define ENABLE_TESTING_ACCESS

#include "identification/pyramid.h"
#include "gmock/gmock.h"

// Import several matchers from Google Mock.
using testing::UnorderedElementsAre;
using testing::Each;
using testing::Contains;

/// Check that reference_find method returns the correct star.
TEST(PyramidReference, ReferenceFind) {
    std::random_device seed;
    Chomp ch;

    Pyramid::label_list_pair ei = {Pyramid::label_pair {3, 100}, Pyramid::label_pair{3, 413},
        Pyramid::label_pair {7, 87}};
    Pyramid::label_list_pair ej = {Pyramid::label_pair {3, 2}, Pyramid::label_pair{3, 5}, Pyramid::label_pair {13, 87}};
    Pyramid::label_list_pair ek = {Pyramid::label_pair {90, 12345}, Pyramid::label_pair{3, 7352},
        Pyramid::label_pair {9874, 512}};
    Pyramid a(Benchmark(ch, seed, 20), Pyramid::DEFAULT_PARAMETERS);
    Star b = a.find_reference(ei, ej, ek);
    
    EXPECT_EQ(b, ch.query_hip(3));
}

/// Check that find_candidate_quad method returns the correct quad.
TEST(PyramidQuad, CandidateQuadFind) {
    std::random_device seed;
    Chomp ch;
    Benchmark input(ch, seed, 20);
    Identification::Parameters par = Pyramid::DEFAULT_PARAMETERS;
    par.sigma_query = 10e-7;
    Pyramid a(input, par);
    Pyramid::star_quad b = a.find_candidate_quad({a.input[0], a.input[1], a.input[2], a.input[3]});
    
    EXPECT_EQ(input.stars[0].get_label(), b[0].get_label());
    EXPECT_EQ(input.stars[1].get_label(), b[1].get_label());
    EXPECT_EQ(input.stars[2].get_label(), b[2].get_label());
    EXPECT_EQ(input.stars[3].get_label(), b[3].get_label());
}

/// Check that correct result is returned with a clean input.
TEST(PyramidIdentify, CleanInput) {
    std::random_device seed;
    Chomp ch;
    Benchmark input(ch, seed, 20, 6.5);
    unsigned int nu = 0;
    Identification::Parameters par = Pyramid::DEFAULT_PARAMETERS;
    par.sigma_query = 10e-7;
    par.sigma_overlay = 0.000001;
    par.gamma = 0.66;
    par.nu = std::make_shared<unsigned int>(nu);
    Star::list c = Pyramid(input, par).experiment_crown();
    ASSERT_GT(c.size(), input.stars.size() * par.gamma);
    
    for (unsigned int q = 0; q < c.size() - 1; q++) {
        auto match = [&c, q] (const Star &b) -> bool {
            return b.get_label() == c[q].get_label();
        };
        auto is_found = std::find_if(input.stars.begin(), input.stars.end(), match);
        EXPECT_NE(is_found, input.stars.end());
    }
}

/// Check that correct result is returned with an error input.
TEST(PyramidIdentify, ErrorInput) {
    std::random_device seed;
    Chomp ch;
    Benchmark input(ch, seed, 20);
    input.add_extra_light(1);
    unsigned int nu = 0;
    Identification::Parameters par = Pyramid::DEFAULT_PARAMETERS;
    par.sigma_query = 10e-7;
    par.sigma_overlay = 0.000001;
    par.gamma = 0.66;
    par.nu = std::make_shared<unsigned int>(nu);
    Star::list c = Pyramid(input, par).experiment_crown();
    ASSERT_GT(c.size(), input.stars.size() * par.gamma);
    
    for (unsigned int q = 0; q < c.size() - 1; q++) {
        auto match = [&c, q] (const Star &b) -> bool {
            return b.get_label() == c[q].get_label();
        };
        auto is_found = std::find_if(input.stars.begin(), input.stars.end(), match);
        EXPECT_NE(is_found, input.stars.end());
    }
}

/// Check that a clean input returns the expected query result.
TEST(PyramidTrial, CleanQuery) {
    std::random_device seed;
    Chomp ch;
    Pyramid::Parameters p = Pyramid::DEFAULT_PARAMETERS;
    p.sigma_query = 10e-9;
    Benchmark input(ch, seed, 15);
    Pyramid a(Benchmark::black(), p);
    
    // We only use the first two stars for querying here.
    std::vector<Identification::labels_list> d = a.experiment_query(
        {input.stars[0], input.stars[1], input.stars[2], input.stars[3]});
    Identification::labels_list ell = {input.stars[0].get_label(), input.stars[1].get_label()};
    
    std::sort(ell.begin(), ell.end());
    EXPECT_THAT(d, Contains(ell));
}

/// Check that a clean input returns the expected alignment of stars.
TEST(PyramidTrial, CleanFirstAlignment) {
    std::random_device seed;
    Chomp ch;
    Rotation q = Rotation::chance(seed);
    Star focus = Star::chance(seed);
    Pyramid::Parameters p = Pyramid::DEFAULT_PARAMETERS;
    p.sigma_query = 10e-9;
    Benchmark input(ch, seed, focus, q, 15, 6.0);
    Pyramid a(input, p);
    
    Star::list b = {a.input[0], a.input[1], a.input[2], a.input[3]}, d = {a.input[0], a.input[1]};
    Star::list c = {ch.query_hip(input.stars[0].get_label()), ch.query_hip(input.stars[1].get_label()),
        ch.query_hip(input.stars[2].get_label()), ch.query_hip(input.stars[3].get_label())};
    
    EXPECT_ANY_THROW(a.experiment_first_alignment(ch.nearby_bright_stars(focus, 20, 100), c, d));
    
    Star::list f = a.experiment_first_alignment(ch.nearby_bright_stars(focus, 20, 100), c, b);
    EXPECT_THAT(f, Contains(Star::define_label(b[0], c[0].get_label())));
    EXPECT_THAT(f, Contains(Star::define_label(b[1], c[1].get_label())));
    EXPECT_THAT(f, Contains(Star::define_label(b[2], c[2].get_label())));
    EXPECT_THAT(f, Contains(Star::define_label(b[3], c[3].get_label())));
}

/// Check that a clean input returns the correct stars from a set of candidates.
TEST(PyramidTrial, CleanReduction) {
    std::random_device seed;
    Chomp ch;
    Pyramid::Parameters p = Pyramid::DEFAULT_PARAMETERS;
    p.sigma_query = 10e-10;
    Benchmark input(ch, seed, 15);
    Pyramid a(input, p);
    Identification::labels_list ell = {input.stars[0].get_label(), input.stars[1].get_label(),
        input.stars[2].get_label(), input.stars[3].get_label()};
    
    std::sort(ell.begin(), ell.end());
    EXPECT_THAT(a.experiment_reduction(), UnorderedElementsAre(ell[0], ell[1], ell[2], ell[3]));
}

/// Check that a clean input returns the expected alignment of stars.
TEST(PyramidTrial, CleanAlignment) {
    std::random_device seed;
    Chomp ch;
    Rotation q = Rotation::chance(seed);
    Star focus = Star::chance(seed);
    unsigned int nu = 0;
    Pyramid::Parameters p = Pyramid::DEFAULT_PARAMETERS;
    p.sigma_query = 10e-9;
    p.nu = std::make_shared<unsigned int>(nu);
    Benchmark input(ch, seed, focus, q, 15, 6.0);
    
    Star::list b = {Rotation::rotate(input.stars[0], q), Rotation::rotate(input.stars[1], q),
        Rotation::rotate(input.stars[2], q), Rotation::rotate(input.stars[3], q)};
    Star::list c = {ch.query_hip(input.stars[0].get_label()), ch.query_hip(input.stars[1].get_label()),
        ch.query_hip(input.stars[2].get_label()), ch.query_hip(input.stars[3].get_label())};
    
    Pyramid a(Benchmark(seed, b, Rotation::rotate(focus, q), 20), p);
    Star::list f = a.experiment_alignment();
    EXPECT_THAT(f, Contains(Star::define_label(b[0], c[0].get_label())));
    EXPECT_THAT(f, Contains(Star::define_label(b[1], c[1].get_label())));
    EXPECT_THAT(f, Contains(Star::define_label(b[2], c[2].get_label())));
    EXPECT_THAT(f, Contains(Star::define_label(b[3], c[3].get_label())));
}

/// Runs all tests defined in this file.
///
/// @param argc Argument count. Used in Google Test initialization.
/// @param argv Argument vector. Used in Google Test initialization.
/// @return The result of running all tests.
int main (int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}