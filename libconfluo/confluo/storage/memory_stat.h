#ifndef CONFLUO_STORAGE_MEMPOOL_STAT_H_
#define CONFLUO_STORAGE_MEMPOOL_STAT_H_

#include "atomic.h"
#include "conf/configuration_params.h"

namespace confluo {
namespace storage {

class memory_stat {

 public:
  memory_stat() :
    memory_used_(0) {
  }

  void increment(size_t size) {
    atomic::faa(&memory_used_, size);
  }

  void decrement(size_t size) {
    atomic::fas(&memory_used_, size);
  }

  size_t get() {
    return atomic::load(&memory_used_);
  }

 private:
  atomic::type<size_t> memory_used_;

};

}
}

#endif /* CONFLUO_STORAGE_MEMPOOL_STAT_H_ */