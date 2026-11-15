// function declarations

#pragma once

#include <string>
#include <vector>

#include "imu.h"

// Compute knee flexion at each sample from the difference between
// the thigh and shin angles:

std::vector<double> compute_knee_flexion (const IMUSet& imu);

// Detect heel strikes using the vertical acceleration threshold and
// minimum interval specified. Returns sample indices in order:

std::vector<int> locate_heel_strikes (const IMUSet& imu, double vaccel_threshold, int min_interval);

// Compute peak knee flexion between each pair of consecutive heel strikes:

std::vector<double> get_peak_knee_flexion_per_cycle (const std::vector<double>& knee_flexion, const std::vector<int>& heel_strikes);

// Write out a report to the file specified, based on the computed parameters:

void write_report (const std::string& report_filename, const std::string& input_filename,
    const std::vector<int>& heel_strikes, const std::vector<double>& peak_knee_flexion,
    double mean_peak_knee_flexion);
