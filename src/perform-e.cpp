/// @file run-trial.cpp
/// @author Glenn Galvizo
///
/// Source file for the trial runner. Based on the arguments, run the specific trial for the given identification
/// method and log the data to a database. There exists four trial types, and six identification methods.
///
/// @code{.cpp}
/// To perform and log experiments...
/// - angle a -> Run trial A with the Angle method.
/// - dot a -> Run trial A with the DotAngle method.
/// - sphere a -> Run trial A with the SphericalTriangle method.
/// - plane a -> Run trial A with the PlanarTriangle method.
/// - pyramid a -> Run trial A with the Pyramid method.
/// - composite a -> Run trial A with the CompositePyramid method.
///
/// - b query -> Run the query trials with the B method.
/// - b reduction -> Run the reduction trials with the B method.
/// - b identification -> Run the identification trials with the B method.
/// - b overlay -> Run the overlay trials. The passed B method does not matter.
/// @endcode{.cpp}
/// @example
/// @code{.cpp}
/// # Run the identification trials using the Angle method.
/// RunTrial angle identification
/// @endcode

#include <chrono>
#include <algorithm>
#include <iostream>
#include <sstream>

#include "identification/angle.h"
#include "identification/dot-angle.h"
#include "identification/spherical-triangle.h"
#include "identification/planar-triangle.h"
#include "identification/pyramid.h"
#include "identification/composite-pyramid.h"
#include "experiment/experiment.h"

/// INIReader to hold configuration associated with experiments.
INIReader cf(std::getenv("HOKU_PROJECT_PATH") + std::string("/CONFIG.ini"));

/// @brief Namespace containing all table 'hashes' for experimentation.
///
/// Hash functions used in experimentation (finding Lumberjack tables, identifier methods).
namespace EHA {
    /// Convert the given user argument specifying the experiment name, to its appropriate hash.
    ///
    /// @param experiment_in Input given by the user, to identify the type of experiment table.
    /// @return Index of the name space below. 3 is given if the given input is not in the name space.
    int experiment_to_hash (const std::string &experiment_in) {
        std::array<std::string, 4> space = {"query", "reduction", "identification", "overlay"};
        return static_cast<int> (std::distance(space.begin(), std::find(space.begin(), space.end(), experiment_in)));
    }
    
    /// Convert the given user argument specifying the experiment name, to its appropriate table name in Lumberjack.
    ///
    /// @param experiment_in Input given by the user, to identify the type of experiment table.
    /// @return Name of the table to log to in the Lumberjack database.
    std::string experiment_to_experiment_table (const std::string &experiment_in) {
        switch (experiment_to_hash(experiment_in)) {
            case 0: return cf.Get("query-experiment", "lu", "QUERY");
            case 1: return cf.Get("reduction-experiment", "lu", "REDUCTION");
            case 2: return cf.Get("identification-experiment", "lu", "IDENTIFICATION");
            case 3: return cf.Get("overlay-experiment", "lu", "OVERLAY");
            default: throw std::runtime_error(std::string("Experiment name is not in the space of trial names."));
        }
    }
    
    /// Convert the given user argument specifying the identifier name, to its appropriate hash.
    ///
    /// @param identifier_in Input given by the user, to identify the type of experiment table.
    /// @return Index of the name space below. 6 is given if the given input is not in the name space.
    int identifier_to_hash (const std::string &identifier_in) {
        std::array<std::string, 6> space = {"angle", "dot", "sphere", "plane", "pyramid", "composite"};
        return static_cast<int> (std::distance(space.begin(), std::find(space.begin(), space.end(), identifier_in)));
    }
}

/// Create the appropriate lumberjack table.
///
/// @param experiment_in Input given by the user, to identify the type of experiment table to create.
/// @return TABLE_NOT_CREATED if the table already exists. Otherwise, 0 if successful.
int create_lumberjack_table (const std::string &experiment_in) {
    std::string table = EHA::experiment_to_experiment_table(experiment_in);
    Chomp ch;
    
    switch (EHA::experiment_to_hash(experiment_in)) {
        case 0: return Lumberjack::create_table(table, Experiment::Query::SCHEMA);
        case 1: return Lumberjack::create_table(table, Experiment::Reduction::SCHEMA);
        case 2: return Lumberjack::create_table(table, Experiment::Map::SCHEMA);
        case 3: return Lumberjack::create_table(table, Experiment::Overlay::SCHEMA);
        default: throw std::runtime_error(std::string("Experiment name is not in the space of trial names."));
    }
}

/// Run the specified trial! Select the appropriate header and trial function given the identification choice.
///
/// @param lu Open Lumberjack connection, used to log the results of each trial.
/// @param identifier_in Input given by the user, to identify the type of experiment table to log to.
/// @param experiment_in Input given by the user, to identify the type of identifier to use for the experiment.
void perform_trial (Lumberjack &lu, const std::string &identifier_in, const std::string &experiment_in) {
//    Chomp ch(cf.Get("table-names", identifier_in, ""), cf.Get("table-focus", identifier_in, ""));
    Chomp ch;
    
    switch ((EHA::identifier_to_hash(identifier_in) * 4) + EHA::experiment_to_hash(experiment_in)) {
        case 0: return Experiment::Query::trial<Angle>(ch, lu, cf, identifier_in);
        case 1: return Experiment::Reduction::trial<Angle>(ch, lu, cf, identifier_in);
        case 2: return Experiment::Map::trial<Angle>(ch, lu, cf, identifier_in);
        case 3: return Experiment::Overlay::trial<Angle>(ch, lu, cf, identifier_in);
        
        case 4: return Experiment::Query::trial<Dot>(ch, lu, cf, identifier_in);
        case 5: return Experiment::Reduction::trial<Dot>(ch, lu, cf, identifier_in);
        case 6: return Experiment::Map::trial<Dot>(ch, lu, cf, identifier_in);
        case 7: return Experiment::Overlay::trial<Dot>(ch, lu, cf, identifier_in);
    
        case 8: return Experiment::Query::trial<Sphere>(ch, lu, cf, identifier_in);
        case 9: return Experiment::Reduction::trial<Sphere>(ch, lu, cf, identifier_in);
        case 10: return Experiment::Map::trial<Sphere>(ch, lu, cf, identifier_in);
        case 11: return Experiment::Overlay::trial<Sphere>(ch, lu, cf, identifier_in);
    
        case 12: return Experiment::Query::trial<Plane>(ch, lu, cf, identifier_in);
        case 13: return Experiment::Reduction::trial<Plane>(ch, lu, cf, identifier_in);
        case 14: return Experiment::Map::trial<Plane>(ch, lu, cf, identifier_in);
        case 15: return Experiment::Overlay::trial<Plane>(ch, lu, cf, identifier_in);
    
        case 16: return Experiment::Query::trial<Pyramid>(ch, lu, cf, identifier_in);
        case 17: return Experiment::Reduction::trial<Pyramid>(ch, lu, cf, identifier_in);
        case 18: return Experiment::Map::trial<Pyramid>(ch, lu, cf, identifier_in);
        case 19: return Experiment::Overlay::trial<Pyramid>(ch, lu, cf, identifier_in);
    
        case 20: return Experiment::Query::trial<Composite>(ch, lu, cf, identifier_in);
        case 21: return Experiment::Reduction::trial<Composite>(ch, lu, cf, identifier_in);
        case 22: return Experiment::Map::trial<Composite>(ch, lu, cf, identifier_in);
        case 23: return Experiment::Overlay::trial<Composite>(ch, lu, cf, identifier_in);
    
        default: throw std::runtime_error(std::string("Choices not in appropriate spaces or test does not exist."));
    }
}

/// Select the desired identification method given the first argument. In the second argument, indicate the type of
/// trial desired.
///
/// @code{.cpp}
/// To perform and log experiments...
/// - angle a -> Run trial A with the Angle method.
/// - dot a -> Run trial A with the DotAngle method.
/// - sphere a -> Run trial A with the SphericalTriangle method.
/// - plane a -> Run trial A with the PlanarTriangle method.
/// - pyramid a -> Run trial A with the Pyramid method.
/// - composite a -> Run trial A with the CompositePyramid method.
///
/// - b query -> Run the query trials with the B method.
/// - b reduction -> Run the reduction trials with the B method.
/// - b identification -> Run the identification trials with the B method.
/// - b overlay -> Run the overlay trials. The passed B method does not matter.
/// @endcode{.cpp}
///
/// @param argc Argument count. This must be equal to 3.
/// @param argv Argument vector. Determines which trial to run, and which identification method to test.
/// @return -1 if the arguments are incorrect. 0 otherwise.
int main (int argc, char *argv[]) {
    std::ios::sync_with_stdio(false);
    std::ostringstream l;
    
    /// Determine the timestamp.
    using clock = std::chrono::high_resolution_clock;
    l << clock::to_time_t(clock::now() - std::chrono::hours(24));
    
    // Verify the arguments.
    if (argc != 3) {
        std::cout << "Usage: RunTrial [IdentificationChoice] [TrialChoice]" << std::endl;
        return -1;
    }
    else {
        // Verify that the arguments are within their appropriate spaces.
        auto is_valid_arg = [] (const char *arg, const std::vector<std::string> &input_space) -> bool {
            std::string a = std::string(arg);
            return std::find(input_space.begin(), input_space.end(), a) != input_space.end();
        };
        
        if (!is_valid_arg(argv[1], {"angle", "dot", "sphere", "plane", "pyramid", "composite"})
            || !is_valid_arg(argv[2], {"query", "reduction", "identification", "overlay"})) {
            std::cout << "Usage: RunTrial ['angle', ... , 'composite'] ['query', ... , 'identification']" << std::endl;
            return -1;
        }
    }
    
    // Attempt to connect to the Lumberjack database.
    std::string identification = std::string(argv[1]), identifier_name = std::string(argv[1]);
    std::string trial_name = std::string(argv[2]);
    try {
        Lumberjack lu(EHA::experiment_to_experiment_table(trial_name), identification, l.str());
    }
    catch (const std::exception &e) {
        create_lumberjack_table(trial_name);
    }
    
    // Capitalize our trial name. Perform our trial.
    identification[0] = static_cast<char> (toupper(identification[0]));
    Lumberjack lu(EHA::experiment_to_experiment_table(trial_name), identification, l.str());
    perform_trial(lu, identifier_name, trial_name);
    return 0;
}
