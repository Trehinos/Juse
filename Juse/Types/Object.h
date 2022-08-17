#pragma once

#include "ObjectModel.h"

namespace Juse::Types {
    struct Object : public virtual TupleModel {
        SPtr<ObjectModel> model;
        SPtr<ModelMethod> method(S8 methodName)
        {
            if (methods().get().contains(methodName)) {
                return methods().get().at(methodName);
            }
            return nullptr;
        }
        void call(S8 methodName, Machine& m, Cpu& c)
        {
            (*method(methodName))(m, c, this);
        }
        Object(ObjectModel* m, S8 name, ModelMap& fields, ParameterList& p) : TupleModel(name), model(m)
        {

        }
        Object(ObjectModel* m, S8 name, ModelMap& fields) : TupleModel(name), model(m) {}
    private:
        inline Ref<MethodMap> methods()
        {
            return model.get()->methods;
        }
    };
}