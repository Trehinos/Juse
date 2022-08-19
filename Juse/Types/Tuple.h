#pragma once

#include "JTypes.h"

namespace Juse::Types {
    struct TupleModel : public virtual Data
    {
        ModelMap fields;
        TupleModel(S8 n, ModelMap f = {}) : Data(n), fields(f) {}
        ByteSet toSet()
        {
            ByteSet output{};
            for (auto& [fieldName, field] : fields) {
                for (U8 byte : field.get()->toSet()) {
                    output.push_back(byte);
                }
            }
            return output;
        }   
        Wrap<Model> field(S8 name)
        {
            if (fields.contains(name)) {
                return wrap(*fields.at(name));
            }
            return std::nullopt;
        }
    };
}