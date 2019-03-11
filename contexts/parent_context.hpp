#ifndef LR_SUPERSNAKES_PARENT_CONTEXT_HPP
#define LR_SUPERSNAKES_PARENT_CONTEXT_HPP


#include <deque>
#include "context.hpp"

class Context;

class ParentContext {
public:
    virtual void PushNewContext(Context *context) = 0;

    virtual void EndContext(Context *context) = 0;

    virtual Renderer *GetRenderer() = 0;
};


#endif //LR_SUPERSNAKES_PARENT_CONTEXT_HPP