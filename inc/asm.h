#pragma once

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif
  // rng functions
  int64_t rand_between(int64_t lower_bound, int64_t upper_bound);
  int _seed();
  // staircase helpers
  bool _checkValidStaircasePosition(int x, int y, int width, int height);
  bool _isDoorwayProximity(int x, int y, int doorwayX, int doorwayY,
                           int proximityThreshold);
  // room connection helpers
  bool _hasAnyConnection(void** adjacentRooms, int numDirections);
  int _getOppositeDirection(int direction);
  void _shuffleDirections(int* directions, int count, unsigned int seed);
#ifdef __cplusplus
}
#endif
