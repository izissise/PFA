#ifndef TIMEHANDLING_H
# define TIMEHANDLING_H

# include <chrono>
# include <thread>

class TimeHandling
{
public:
  /** Default constructor */
  TimeHandling(const std::chrono::milliseconds& targetFrameDuration = std::chrono::milliseconds(16));

  /** Default destructor */
  virtual ~TimeHandling();

  /** Access _totalElapsed
   * \return The total time since the first call of startFrame
   */
  const std::chrono::microseconds& GetTotalElapsed() const {return _totalElapsed;};

  /** Access _targetFrameDuration
  * \return The current value of _targetFrameDuration
  */
  std::chrono::milliseconds GetTargetFrameDuration() const {return std::chrono::duration_cast<std::chrono::milliseconds>(_targetFrameDuration);};

  /** Get number of frame since the beginning
  * \return Value of _nbFrame
  */
  unsigned long int GetNumberOfFrame() const {return _nbFrame;};

  /** Change _targetFrameDuration
  *
  */
  void SetTargetFrameDuration(const std::chrono::milliseconds& tfd) {_targetFrameDuration = std::chrono::duration_cast<std::chrono::microseconds>(tfd);};

  /** Start time count for the program
  * \brief  call it only once
  */
  void start();

  /** End current frame and wait/report time elapsed
  *
  */
  void endFrame();

protected:
private:
  std::chrono::time_point<std::chrono::system_clock> _startTime;
  std::chrono::microseconds _targetFrameDuration;
  std::chrono::microseconds _totalElapsed; //!< Member variable "_totalElapsed"
  std::chrono::microseconds _lag;
  unsigned long int _nbFrame;
};

#endif // TIMEHANDLING_H
