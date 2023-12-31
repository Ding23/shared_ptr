#include "iostream"
#include "atomic"

class control_block {
private:
  std::atomic<uint32_t> count = 0;

public:
  uint32_t increase_count() {
    return ++count;
  }

  uint32_t decrease_count() {
    return --count;
  }

  uint32_t get_count() {
    return count.load();
  }

};

template <typename T>
class my_shared_ptr {
private:
  T * data = nullptr;
  control_block * control_ptr = nullptr;

public:
  explicit my_shared_ptr(T* p) {
    std::cout << "my_shared_ptr(T* p) " << std::endl;
    assert(p != nullptr);
    control_ptr = new control_block();
    control_ptr->increase_count();
    data = p;

  }

  my_shared_ptr(const my_shared_ptr& rhs) {
    data = rhs.data;
    assert(data != nullptr);
    control_ptr = rhs.control_ptr;
    control_ptr->increase_count();
    std::cout << "my_shared_ptr(const my_shared_ptr& rhs) " << control_ptr->get_count() << std::endl;
  }

  my_shared_ptr& operator= (const my_shared_ptr& rhs) {
    data = rhs.data;
    assert(data != nullptr);
    control_ptr = rhs.control_ptr;
    control_ptr->increase_count();
    std::cout << "my_shared_ptr& operator= (const my_shared_ptr& rhs) " << control_ptr->get_count() << std::endl;
  }

  my_shared_ptr(my_shared_ptr&& rhs) {
    data = rhs.data;
    assert(data != nullptr);
    control_ptr = rhs.control_ptr;
    rhs.data = nullptr;
    rhs.control_ptr = nullptr;
    std::cout << "my_shared_ptr(my_shared_ptr&& rhs)" << control_ptr->get_count() << std::endl;
  }

  my_shared_ptr& operator= (my_shared_ptr&& rhs) {
    data = rhs.data;
    assert(data != nullptr);
    control_ptr = rhs.control_ptr;
    control_ptr->increase_count();
    std::cout << " my_shared_ptr& operator= (my_shared_ptr&& rhs) " << control_ptr->get_count() << std::endl;
  }


  ~my_shared_ptr() {
   // std::cout << "~my_shared_ptr()" << control_ptr->get_count() << std::endl;
    if(data && control_ptr && control_ptr->decrease_count() == 0) {
      std::cout << "Last count, will free memory" <<std::endl;
      delete data;
      delete control_ptr;
    }
    control_ptr = nullptr;
    data = nullptr;
  }

public:
  T* get() {
    return data;
  }

  T* operator ->() const {
    assert(data != nullptr);
    return data;
  }

  T& operator*() {
    assert(data != nullptr);
    return *data;
  }

};
