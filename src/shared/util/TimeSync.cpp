#include "TimeSync.h"

const int BUFFER_SIZE = 30;
const long MAX_AVG_DIFF = 3e8L; // 300ms
const long SYNC_ACCURACY = 1e6L; // 1ms

TimeSync::TimeSync() { currentOffset = 0; }

void TimeSync::update(long timestamp) {
  timeval tv;
  gettimeofday(&tv, nullptr);
  long tRef = tv.tv_sec * 1e9L + tv.tv_usec * 1e3L;
  long tSynced = timestamp - currentOffset;

  long diff = tRef - tSynced;
  diffBuffer.push_back(diff);
  if (diffBuffer.size() > BUFFER_SIZE)
    diffBuffer.pop_front();
  long avgDiff = labs(average(diffBuffer));

  if ((avgDiff > MAX_AVG_DIFF) || !offsetBuffer.empty()) {
    // Start syncing
    offsetBuffer.push_back(timestamp - tRef);
    if(offsetBuffer.size() > BUFFER_SIZE)
      offsetBuffer.pop_front();
    currentOffset = average(offsetBuffer);
    if (avgDiff < SYNC_ACCURACY) {
      // Converged, stop syncing with reference clock
      std::cout << "Synced with System clock. offset=" << currentOffset
                << "ns diff=" << avgDiff << "ns" << std::endl;
      offsetBuffer.clear();
    }
  }
}

long TimeSync::sync(long timestamp) const { return timestamp - currentOffset; }

long TimeSync::reverseSync(long timestamp) const {
  return timestamp + currentOffset;
}

long TimeSync::calcOffset(long tRef, long tOther) { return tOther - tRef; }

long TimeSync::average(const std::deque<long> &deque) {
  int size = deque.size();
  long avg = 0;
  for (long l : deque) {
    avg += l;
  }
  return avg / size;
}