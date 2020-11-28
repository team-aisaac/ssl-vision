#ifndef SSL_VISION_TIMESYNC_H
#define SSL_VISION_TIMESYNC_H

#include <deque>

class TimeSync {

public:
  TimeSync();

  void update(long timestamp);
  long sync(long timestamp) const;
  long reverseSync(long timestamp) const;

private:
  long currentOffset;
  std::deque<long> offsetBuffer;
  std::deque<long> diffBuffer;

  static long calcOffset(long tRef, long tOther);
  static long average(const std::deque<long> &deque);
}

#endif // SSL_VISION_TIMESYNC_H
