# Hoku (Lost-in-Space Star Identification for Spacecraft)

## Overview
Ancient mariners could look up the night sky, point out what stars they were looking at, and navigate across the globe 
with precision. _Star identification algorithm_ refers to a computational approach to pointing out which stars are in 
the sky. Given an image of the sky, star identification is matching the bright spots in an image, to stars in an 
astronomical catalog. The device that performs these computations is the star tracker, much like the navigators on the 
ship. _Lost-in-space_ refers to an additional constraint on the problem: the absence of knowing where we took the 
picture and how we pointed the camera.

This repository holds research toward the analysis of various lost-in-space _star identification_ procedures for 
spacecraft. This includes a study of feature uniqueness, permutation order, candidate reduction, and identification 
under the introduction of various noise. The process of identifying blobs in an image, constructing the image 
coordinate system, and efficiently querying static databases is __not__ addressed here. 

There has been an increasing number of approaches toward stellar attitude determination, but little comparison between
each of these methods in a more controlled manner. Interchangeable factors are abstracted away (camera hardware, blob
detection, etc...) to focus more on how each method matches stars in an image to a catalog.

## Getting Started
This repository requires the following:
1. `python3` (with `numpy` and `matplotlib`). Used for the data analysis and visualization.
    1. Install Python 3: [https://www.python.org/downloads/](https://www.python.org/downloads/)
    2. Install Anaconda (`numpy` and `matplotlib`): [https://conda.io/docs/user-guide/install/index.html](https://conda.io/docs/user-guide/install/index.html)
2. `CMake 3.7` or above. Used to manage and build the C++ code here.
    1. Install CMake: [https://cmake.org/install/](https://cmake.org/install/)
    2. CMake tutorial with CLion IDE: [https://www.jetbrains.com/help/clion/quick-cmake-tutorial.html](https://www.jetbrains.com/help/clion/quick-cmake-tutorial.html)
3. `git` or some Git client. Used to clone this repository, and to grab GoogleTest for testing.
    1. Install Git: [https://git-scm.com/book/en/v2/Getting-Started-Installing-Git](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git)
    2. Install GitKraken (optional): [https://www.gitkraken.com/](https://www.gitkraken.com/)
4. `make` and a C++ compiler.
    1. Use `sudo apt-get install build-essential` on Linux.
    2. Download CLion IDE (optional): [https://www.jetbrains.com/clion/download](https://www.jetbrains.com/clion/download)

To get started, clone this repository:
```cmd
git clone https://github.com/glennga/hoku.git
```

Build the necessary directories and create the `HOKU_PROJECT_PATH` variable by running the first build script. Windows
users have to execute `first-build.bat`, and Linux users execute `first-build.sh`.
```cmd
cd hoku/script 
chmod +x first-build.sh
first-build.sh
```

Generate the executables. The output will reside in the `bin` folder. 
```cmd
cd hoku/build
cmake -G"Unix Makefiles" ..
cd src
make -j8 install
```

Download the Nibble database, `nibble.db` [here](https://drive.google.com/open?id=1R7kmOB5QHgCqTh3Uc_w48UDaTEolhrTx),
and store this in `hoku/data`. This holds all the data each identification method will reference (the catalog). 

## Running Experiments
There exist three experiments in this research:
1. Feature Uniqueness (`query`)
2. Candidate Reduction (`reduction`)
3. Identification (`identification`)

There exist six different identification methods implemented here:
1. Gottlieb's Angle Method (`angle`)
2. Liebe's Interior Angle Method (`interior`)
3. Cole and Crassidus's Spherical Triangle Method (`sphere`)
4. Cole and Crassidus's Planar Triangle Method (`plane`)
5. Mortari's Pyramid Method (`pyramid`)
6. Toloei's Composite Pyramid Method (`composite`)

The program `PerformE` will execute all combinations of the experiments and identification methods above. The first 
argument is the identification method, and the second is the type of experiment. To execute the query experiment for
Gottlieb's Angle method, enter the following:
```cmd
cd hoku/bin
./PerformE angle query
```

The results will be logged in the Lumberjack database (`lumberjack.db`), stored in tables according to the experiments 
and grouped by the experiment timestamp. To view the results of these experiments, use the visualize script.

TODO: Finish the `visualize_image.py` portion.

## Star Identification Procedure Usage

To use a star identification procedure in another application (such as an actual star tracker), start by copying the 
entire Hoku directory into your project. 

Include the desired star identification procedure you want to use:
```cpp
#include "hoku/include/identification/angle.h"
```

Example usage of Gottlieb's Angle identification procedure is depicted below. 
```cpp
// Input: A std::vector (Star::list) of Star objects, representing stars as 3D vectors in 
//        the image coordinate system.
Star::list image = {Star(0, 0, 1), ...};  

// Input: A Star object representing the center of the image in the image coordinate system.
Star focus = ...;

// Wrap this in a Benchmark object. 20 here represents the field-of-view.
Benchmark input (image, focus, 20);

// Define any hyperparameters a Parameter struct.
Angle::Parameters p;
unsigned int nu = 0;

/* 
sigma_query <- Query must be within 3 * sigma_query.
sql_limit <- While performing a SQL query, limit results by this number.
sigma_overlay <- Resultant of inertial->body rotation must within 3 * sigma_overlay of *a* body.
nu_max <- Maximum number of query star comparisons before returning an empty list.
nu <- Pointer to the location to hold the count of query star comparisons.
table_name <- Name of the Nibble database table created with 'generate_sep_table'.
*/
p.nu_max = 20000, p.sigma_overlay = Identification::DEFAULT_SIGMA_OVERLAY;
p.sigma_query = IDENTIFICATION::DEFAULT_SIGMA_QUERY; p.table_name = ANGLE_20;
p.nu = std::make_shared<unsigned int>(nu);

// Output: A std::vector (Star::list) of Star objects, holding all stars in the image that were 
//         identified, and with labels attached to them.
Star::list output = Angle(input, p).identify()

// To view the labels, use the 'get_label()' method for each Star.
int ell_0 = output[0].get_label();
int ell_1 = output[1].get_label();
```

Only CMake builds are supported at the moment. The final step is to link the `HOKU_LIBS` to your project in your 
`CMakeLists.txt` file. 
```cmake
add_executable(MyStarTracker my-star-tracker.cpp)
target_link_libraries(MyStarTracker ${SOME_OTHER_LIBS} ${HOKU_LIBS})
```

## Google Test Generation
Generate the test executables. The output will reside in the `test/bin` folder.
```cmd
cd hoku/build
cmake -G"Unix Makefiles" -DBUILD_TEST=ON ..
cd test
make -j8 install
```

TODO: Finish the Google Test generation.

## LaTeX Generation

TODO: Finish the LaTeX generation.