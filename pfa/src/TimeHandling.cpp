#include "TimeHandling.hpp"

TimeHandling::TimeHandling(const std::chrono::milliseconds& targetFrameDuration)
  : _targetFrameDuration(std::chrono::duration_cast<std::chrono::microseconds>(targetFrameDuration)),
    _totalElapsed(0), _lag(0), _nbFrame(0)
{
}

TimeHandling::~TimeHandling()
{
}

void TimeHandling::start()
{
  _startTime = std::chrono::system_clock::now();
}

void TimeHandling::endFrame()
{
  std::chrono::microseconds elapsed;

  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - _startTime);
  _startTime = end;
  _totalElapsed += elapsed;
  _nbFrame += 1;
  _lag += elapsed;
  if (_lag < _targetFrameDuration)
    std::this_thread::sleep_for(_targetFrameDuration);
  _lag -= _targetFrameDuration;
}
