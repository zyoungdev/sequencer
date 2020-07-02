#ifndef _APP_MAIN_H_
#define _APP_MAIN_H_

#include "include.h"
#include "EngineModel.h"

template <typename Tag, typename StoredType>
struct StrongType
{
  StoredType val;

  explicit operator StoredType&()
  {
    return val;
  }

  explicit operator const StoredType&() const noexcept
  {
    return val;
  }
};

struct Url : StrongType<Url, char const*> {};

#endif
