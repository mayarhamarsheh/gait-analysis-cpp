#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <numeric>
#include <string>

#include "debug.h"
#include "gait.h"

int main (int argc, char* argv[])
{
  if (argc == 4 && std::string (argv[3]) == "--verbose") {
    debug::verbose_mode = true;
    argc--;
  }

  if (argc != 3) {
    std::cerr <<
      "ERROR: invalid arguments.\n\n"
      "Command usage:\n\n"
      "   gait_analysis input.txt report.txt [--verbose]\n\n";
    return 1;
  }

  try {
    // main processing block:

    const std::string input_filename = argv[1];
    const std::string report_filename = argv[2];

    if (input_filename == report_filename)
      throw std::runtime_error (
          "input and report filenames must be different");

    // Check the input file before loading the IMU data:

    std::ifstream infile (input_filename);
    if (!infile)
      throw std::runtime_error (
          "failed to open input file \"" + input_filename + "\"");

    std::string line;
    std::getline (infile, line);
    std::istringstream header (line);

    std::string label;
    int column_count = 0;
    while (header >> label)
      column_count++;

    if (column_count == 0)
      throw std::runtime_error ("input file has no column labels");

    int row_count = 0;
    while (std::getline (infile, line)) {
      std::istringstream row (line);
      row >> std::ws;

      if (row.eof())
        continue;

      double value;
      int value_count = 0;

      while (row >> value)
        value_count++;

      if (!row.eof() || value_count != column_count)
        throw std::runtime_error (
            "invalid data row: expected one number per column");

      row_count++;
    }

    if (row_count == 0)
      throw std::runtime_error ("input file contains no samples");

    infile.close();

    // Load the recordings and compute knee flexion:

    const auto imu = load_IMU (input_filename);

    const auto knee_flexion = compute_knee_flexion (imu);

    // Detect heel strikes using a threshold of 1.5
    // and a minimum interval of 30 samples:

    const auto heel_strikes = locate_heel_strikes (imu, 1.5, 30);

    if (heel_strikes.size() < 2)
      throw std::runtime_error (
          "at least two heel strikes are required");

    // Compute peak knee flexion for each complete gait cycle:

    const auto peak_knee_flexion =
        get_peak_knee_flexion_per_cycle (knee_flexion, heel_strikes);

    // Compute the mean peak knee flexion:

    const double mean_peak_knee_flexion =
        std::accumulate (
            peak_knee_flexion.begin(), peak_knee_flexion.end(), 0.0)
        / peak_knee_flexion.size();

    // Write the results to the report file:

    write_report (report_filename, input_filename,
        heel_strikes, peak_knee_flexion, mean_peak_knee_flexion);

    std::cout << "Report written to " << report_filename << "\n";

    // end of main processing block
  }

  catch (std::exception& err) {
    // if any exceptions were thrown, we can handle them here:

    std::cerr << "ERROR: " << err.what() << " - aborting\n";
    return 1;
  }

  catch (...) { // the catch-all handler

    std::cerr <<
      "ERROR: exception of unknown type was thrown - aborting\n";
    return 1;
  }

  return 0;
}
