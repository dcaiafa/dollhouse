#ifndef DOLLHOUSE_SCOPED_PTR_H_
#define DOLLHOUSE_SCOPED_PTR_H_

template <class T>
class scoped_ptr {
 public:
  explicit scoped_ptr(T* obj) : obj_(obj) {}
  scoped_ptr() : obj_(NULL) {}

  ~scoped_ptr() {
    reset();
  }

  T* operator ->() {
    return obj_;
  }

  T* get() {
    return obj_;
  }

  void reset(T* obj = NULL) {
    if (obj_) {
      delete obj_;
    }
    obj_ = obj;
  }

  T* release() {
    T* obj = obj_;
    obj_ = NULL;
    return obj;
  }

 private:
  scoped_ptr(const scoped_ptr&);
  scoped_ptr& operator =(const scoped_ptr&);

  T* obj_;
};

#endif  // DOLLHOUSE_SCOPED_PTR_H_

