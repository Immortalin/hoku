/*
 * @file: benchmark.cpp
 *
 * @brief: Source file for Benchmark class, which generates the input data for star
 * identification testing.
 */

#include "benchmark.h"

/*
 * Constructor. Sets the fov, focus, and rotation of the star set. Generate the stars after
 * collecting this information.
 *
 * @param fov Limit a star must be separated from the focus by.
 * @param focus All stars will be near this vector.
 * @param inertial_to_image Rotation to move stars from inertial to image.
 */
Benchmark::Benchmark(const double fov, const Star &focus, const Rotation &inertial_to_image) {
    this->fov = fov;
    this->focus = focus;
    this->inertial_to_image = inertial_to_image;
    generate_stars();
}

/*
 * Shuffle the current star set. Uses C++11 random library.
 */
void Benchmark::shuffle() {
    // need to keep random device static to avoid starting w/ same seed
    static std::random_device seed;
    static std::mt19937_64 mersenne_twister(seed());

    std::shuffle(this->stars.begin(), this->stars.end(), mersenne_twister);
}

/*
 * Obtain a set of stars around the current focus vector. This is the 'clean' star set, meaning
 * that all stars in 'stars' accurately represent what is found in the catalog. Uses C++11 random
 * library to shuffle.
 */
void Benchmark::generate_stars() {
    // expected number of stars = fov * 4, not too concerned with accuracy here
    unsigned int expected = (unsigned int) this->fov * 4;

    // find nearby stars, rotate these stars and the focus
    for (const Star &s : Nibble::nearby_stars(this->focus, this->fov / 2.0, expected)) {
        this->stars.push_back(Rotation::rotate(s, this->inertial_to_image));
    }
    this->focus = Rotation::rotate(this->focus, this->inertial_to_image);

    // shuffle to maintain randomness
    this->shuffle();
}

/*
 * Set all of the HR numbers in the current star set to 0. In practice, the HR number of a star
 * set is never given from the image itself. Return the current star set as this.
 *
 * @return Current star set with HR numbers set to 0.
 */
Benchmark::star_list Benchmark::clean_stars() {
    for (unsigned int i = 0; i < this->stars.size(); i++) {
        this->stars[i] = Star::reset_hr(this->stars[i]);
    }

    return this->stars;
}

/*
 * Modify the given image_s, image_focus, and image_fov to the current stars, focus, and fov
 * fields. These are all points that define an image, and are required for all identification
 * methods.
 *
 * @param image_s Reference to the star list to benchmark star set.
 * @param image_focus Reference to the star to set as the focus star.
 * @param image_fov Reference to the double to set as the fov.
 */
void Benchmark::present_image(star_list &image_s, Star &image_focus, double &image_fov) {
    image_s = clean_stars();
    image_focus = this->focus;
    image_fov = this->fov;
}

/*
 * Write the current data in the star set to two files. This includes the fov, norm, focus, star
 * set, and the error set.
 *
 * @return -1 if any files were unable to obtain. 0 otherwise.
 */
int Benchmark::record_current_plot() {
    std::ofstream current(this->current_plot), error(this->error_plot);
    std::ostringstream record;

    if (current) {
        // record the fov and norm first
        record << std::setprecision(16) << std::fixed;
        record << this->fov << "\n" << this->focus.norm() << "\n";

        // record the focus second
        for (const double &component : {this->focus[0], this->focus[1], this->focus[2]}) {
            record << component << " ";
        }
        record << "\n";

        // record the rest of the stars
        for (const Star &s : this->stars) {
            for (const double &component : {s[0], s[1], s[2]}) {
                record << component << " ";
            }
            record << s.get_hr() << "\n";
        }
        current << record.str();
    } else {
        return -1;
    }

    if (error) {
        // clear the stream
        record.str("");

        // record each error model
        for (const ErrorModel &model : this->error_models) {
            for (const Star &s : model.affected) {
                for (const double &component : {s[0], s[1], s[2]}) {
                    record << component << " ";
                }
                record << s.get_hr() << " " << model.plot_color << "\n";
            }
        }

        error << record.str();
    } else {
        return -1;
    }

    current.close();
    error.close();
    return 0;
}

/*
 * Write the current data in the star set to a file, and let a separate Python script generate
 * the plot. I am most familiar with Python's MatPlotLib, so this seemed like the most straight-
 * forward approach.
 *
 * @param current_plot_file Location to store current_plot.dat.
 * @param error_plot_file Location to store error_plot.dat.
 * @param generate_plot_script Location of Python generation script.
 * @return -1 if the previous files could not be deleted. 0 otherwise.
 */
int Benchmark::display_plot(const std::string &current_plot_file,
                            const std::string &error_plot_file,
                            const std::string &generate_plot_script) {
    std::remove(current_plot_file.c_str());
    std::remove(error_plot_file.c_str());
    this->current_plot = current_plot_file;
    this->error_plot = error_plot_file;

    std::string cmd = "python " + generate_plot_script;
    if (std::ifstream(this->current_plot.c_str()) || std::ifstream(this->error_plot.c_str())) {
        return -1;
    }

    // record plot, and let python work its magic 
    this->record_current_plot();
    std::system(cmd.c_str());
    return 0;
}

/*
 * Append n randomly placed vectors that fall within fov/2 degrees of the focus. This models stray
 * light that may randomly wander into the detector.
 *
 * @param n Number of extra stars to add.
 */
void Benchmark::add_extra_light(const int n) {
    int current_n = 0;
    ErrorModel extra_light = {"Extra Light", "r", {Star()}};

    while (current_n < n) {
        Star generated = Star::chance(-current_n - 1);
        if (Star::within_angle(generated, this->focus, this->fov / 2.0)) {
            this->stars.emplace_back(generated);
            extra_light.affected.emplace_back(generated);
            current_n++;
        }
    }

    // shuffle to maintain randomness
    this->shuffle();

    // remove first element, add to error models
    extra_light.affected.erase(extra_light.affected.begin());
    this->error_models.emplace_back(extra_light);
}

/*
 * Randomly generate n dark spots. All stars in the input that are within psi/2 degrees of the
 * blobs are removed. This model simulates celestial bodies that may block light from the
 * detector.
 *
 * @param n Number of blobs to generate.
 * @param psi Dark spot size (degrees). This is the cone size of the dark spot vectors.
 */
void Benchmark::remove_light(const int n, const double psi) {
    int current_n = 0;
    std::vector<Star> blobs;
    ErrorModel removed_light = {"Removed Light", "0.5", {Star(0, 0, 0)}};

    // first, generate the blobs
    while (current_n < n) {
        Star generated = Star::chance();
        if (Star::within_angle(generated, this->focus, this->fov / 2.0)) {
            blobs.emplace_back(generated);
            current_n++;
        }
    }

    // second, check if any of the stars fall within psi / 2 of a dark spot
    for (unsigned int i = 0; i < blobs.size(); i++) {
        for (const Star &star : this->stars) {
            if (Star::within_angle(blobs[i], star, psi / 2.0)) {
                this->stars.erase(this->stars.begin() + i);
                removed_light.affected.emplace_back(star);
            }
        }
    }

    // shuffle to maintain randomness
    this->shuffle();

    // remove first element, add to error models
    removed_light.affected.erase(removed_light.affected.begin());
    this->error_models.emplace_back(removed_light);
}

/*
 * Randomly move n vectors based off the given sigma. Change is normally distributed, with 0
 * change being the most expected and having a standard deviation of sigma.
 *
 * @param n Number of stars to move.
 * @param sigma Amount to shift stars by, in terms of XYZ coordinates.
 */
void Benchmark::shift_light(const int n, const double sigma) {
    static std::random_device seed;
    static std::mt19937_64 mersenne_twister(seed());
    std::normal_distribution<double> dist(0, sigma);
    ErrorModel shifted_light = {"Shifted Light", "g", {Star()}};
    int current_n = 0;

    // loop through entire list again if n not met
    while (current_n < n) {
        for (unsigned int i = 0; i < this->stars.size(); i++) {
            // check inside if n is met early
            if (current_n < n) {
                Star candidate = Star(this->stars[i][0] + dist(mersenne_twister),
                                      this->stars[i][1] + dist(mersenne_twister),
                                      this->stars[i][2] + dist(mersenne_twister),
                                      this->stars[i].get_hr()).as_unit();

                // if shifted star is near focus, add shifted star and remove the old
                if (Star::within_angle(candidate, this->focus, this->fov / 2.0)) {
                    this->stars.push_back(candidate);
                    this->stars.erase(this->stars.begin() + i);
                    shifted_light.affected.emplace_back(candidate);
                    current_n++;
                }
            } else break;
        }
    }

    // shuffle to maintain randomness
    this->shuffle();

    // remove first element, add to error models
    shifted_light.affected.erase(shifted_light.affected.begin());
    this->error_models.push_back(shifted_light);
}