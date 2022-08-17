#pragma once

#include "Method.h"
#include "Tuple.h"

namespace Juse::Types {

    struct Object;

    struct ObjectModel : public virtual TupleModel
    {
        MethodMap methods;
        ObjectModel(S8 n, ModelMap f = {}, MethodMap m = {}) : TupleModel(n, f), methods(m) {}
        void callStatic(S8 methodName)
        {

        }
        virtual SPtr<Object> instantiate(ParameterList parameters);
    };

}