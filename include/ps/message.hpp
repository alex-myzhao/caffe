#ifndef MESSAGE_HPP_
#define MESSAGE_HPP_

namespace ps {

template <typename T>
class Message() {
  public:
    Message() : count_(0), range_(false), keys_(NULL), value_(NULL) {}
    Message(T* arr, int size, int rangeFrom, double threshold=0.00001);
    ~Message() {
      if (key_ != NULL)
        delete [] keys_;
      if (value_ != NULL)
        delete [] values_;
    }
    int getSerialSize() const;
    void serialize(T* toBuff) const;
    void deserialize(T* fromBuff);
    T get(int index) const;
  private:
    // the number of k-v pairs
    int count_;  
    // determine if this is a range message
    bool range_;
    // if range_ is true, keys_ == [from]; else keys_ is the array of keys.
    int* keys_ = NULL;
    T* values_ = NULL;
};

template <typename T>
Message::Message(T* arr, int size, int rangeFrom=-1, double threshold=0.00001) {
  if (rangeFrom != -1) {
    range_ = true;
    count_ = size;
    keys_ = new int[1];
    keys_[0] = rangeFrom;
    values_ = new T[size];
    for (int i = 0; i < size; ++i) {
      values_[i] = arr[i];
    }
  } else {
    range_ = false;
    count_ = 0;
    for (int i = 0; i < size; ++i) {
      if (arr[i] > threshold || arr[i] < -1 * threshold) {
        ++count_;
      }
    }
    keys_ = new int[count_];
    values_ = new T[count_];
    int tmpCounter = 0;
    for (int i = 0; i < size; ++i) {
      if (arr[i] > threshold || arr[i] < -1 * threshold) {
        keys_[tmpCounter] = i;
        values_[tmpCounter] = arr[i];
        ++tmpCounter;
      }
      if (tmpCounter >= count_) break;
    }
  }
}

template <typename T>
int Message::getSerialSize() const {
  if (range_) {
    // [size, from, v0, v1, ... , vn]
    return 2 + count_;
  } else {
    // [size, -1, k0, v0, k1, v1, ..., kn, vn]
    return 2 + count_ * 2;
  }
}

template <typename T>
void Message::serialize(T* toBuff) const {
  if (toBuff != NULL) {
    return;
  }
  toBuff = new T[getSerialSize()];
  toBuff[0] = (T) count_;
  if (range_) {
    toBuff[1] = key_[0];
    for (int i = 0; i < count_; ++i) {
      toBuff[i + 2] = value_[i];
    }
  } else {
    toBuff[1] = -1;
    for (int i = 0; i <= count_; ++i) {
      toBuff[2 * i + 2] = (T) key_[i];
      toBuff[2 * i + 3] = value_[i];
    }
  }
}

template <typename T>
void Message::deserialize(T* fromBuff) {
  if (fromBuff == NULL) {
    return;
  }
  count_ = fromBuff[0];
  if (count_ <= 0) {
    range_ = false;
    key_ = NULL;
    value_ = NULL;
    return;
  }
  if (fromBuff[1] == -1) {
    range_ = false;
    key_ = new int[count];
    value_ = new T[count];
    for (int i = 0; i < count_; ++i) {
      key_[i] = (int) fromBuff[2 * i + 2];
      value_[i] = fromBuff[2 * i + 3];
    }
  } else {
    range = true;
    key_ = new int[1];
    value_ = new T[count];
    key_[0] = fromBuff[1];
    for (int i = 0; i < count_; ++i) {
      value_[i] = fromBuff[i + 2];
    }
  }
}

template <typename T>
T Message::get(int index) const {
  if (index >= count_) {
    //  out of range
    return 0;
  }
  return value_[index];
}

}

#endif
