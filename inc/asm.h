#pragma once

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif
  int64_t rand_between(int64_t lower_bound, int64_t upper_bound);

  // staircase helpers
  bool _checkValidStaircasePosition(int x, int y, int width, int height);
  bool _isDoorwayProximity(int x, int y, int doorwayX, int doorwayY,
                           int proximityThreshold);
#ifdef __cplusplus
}
#endif
