#ifndef qwt3d_autoptr_h__2004_05_14_18_57_begin_guarded_code
#define qwt3d_autoptr_h__2004_05_14_18_57_begin_guarded_code

namespace Qwt3D
{

//! Simple Auto pointer providing deep copies for raw pointer  
/*!
  Requirements: \n
  T* T::clone() const;\n
  T::destroy() const; instead public destructor;\n\n
  clone() is necessary for the pointer to preserve polymorphic behaviour.
  The pointer requires also heap based objects with regard to the template 
  argument in order to be able to get ownership and control over destruction.
  */
template <typename T>
class  qwt3d_ptr
{
public:
  //! Standard ctor
  qwt3d_ptr(T* ptr = 0)
  :rawptr_(ptr)
  {
  }
  //! Dtor (calls T::destroy)
  ~qwt3d_ptr()
  {
    rawptr_->destroy();
  }

  //! Copy ctor (calls (virtual) clone())
  qwt3d_ptr(qwt3d_ptr const& val)
  {
    rawptr_ = val.rawptr_->clone();
  }
  
  //! Assignment in the same spirit as copy ctor
  qwt3d_ptr<T>& operator=(qwt3d_ptr const& val)
  {
    if (this == &val)
      return *this;

    rawptr_->destroy();
    rawptr_ = val.rawptr_->clone();

    return *this;
  }

  //! It's a pointerlike object, isn't it ?
  T* operator->() const
  {
    return rawptr_;
  }

  //! Dereferencing
  T& operator*() const
  {
    return *rawptr_;
  }


private:
  T* rawptr_;
};  

} // ns

#endif /* include guarded */
