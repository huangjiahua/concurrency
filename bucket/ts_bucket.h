//
// Created by jiahua on 2019/2/28.
//

#ifndef CONCURRENCY_TS_BUCKET_H
#define CONCURRENCY_TS_BUCKET_H
#include <vector>
#include <utility>
#include <atomic>

template <typename K, typename V>
class ts_bucket {
public:
    using Key = K;
    using Value = V;
    enum class Status {
       Empty, Valid, Marked
    };
private:
    struct Entry {
       Key key;
       Value value;
       std::atomic<unsigned> status;
       Entry(const Key &key_, const Value &value_): key(key_), value(value_), status(Status::Empty) {}
       Entry(): status(Status::Empty) {}
    };

    std::vector<Entry> _data_vec;

    std::atomic<unsigned> _range, _size;

public:
    explicit ts_bucket(unsigned capacity): _range(0), _size(0), _data_vec(capacity) {}

    bool insert(const Key &key, const Value &value) {
        unsigned curr_ran = _range.load();
    }

    bool update(const Key &key, const Value &newValue);
    bool remove(const Key &key);
    Value get(const Key &key);

};


#endif //CONCURRENCY_TS_BUCKET_H
