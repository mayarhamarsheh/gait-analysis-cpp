// debugging helpers

#pragma once

#include <iostream>
#include <string>

namespace debug {

  // Enable or disable debug output:

  inline bool verbose_mode = false;

  // Print the message when verbose mode is enabled:

  inline void log (const std::string& message)
  {
    if (verbose_mode)
      std::cerr << message << '\n';
  }

}
