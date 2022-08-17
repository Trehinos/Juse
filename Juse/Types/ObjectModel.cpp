#include "ObjectModel.h"
#include "Object.h"

Juse::SPtr<Juse::Types::Object> Juse::Types::ObjectModel::instantiate(ParameterList parameters)
{
    SPtr<Object> o{ new Object{ this, {}, fields, parameters } };
    return o;
}
