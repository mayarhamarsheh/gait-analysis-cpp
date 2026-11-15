// function declarations

#pragma once

#include <string>
#include <vector>

// A simple structure to hold a recording label and its values:

struct IMURecording {

  std::string label;
  std::vector<double> values;

};

// A collection of recordings loaded from the IMU data file:

using IMUSet = std::vector<IMURecording>;

// Load the IMU data contained in the file specified:

IMUSet load_IMU (const std::string& filename);

// Return the recorded values associated with the label specified:

const std::vector<double>& get_recording_of (const IMUSet& imu, const std::string& label);
