#include "Scope.h"
#include "Function.h"

Scope::~Scope() {
    for (auto& iter: functions_) {
        delete iter.second;
    }
}

Function* Scope::get_function(const std::string& function_name) {
    return get_or_create_function_(function_name, false);
}

Function* Scope::add_function(const std::vector<std::string>& function_names,
                              const std::vector<int>& signature) {
    Function* function = nullptr;

    if (function_names.size() == 1) {
        function = get_or_create_function_(function_names.front(), true);
        function->set_signature(signature);
        return function;
    } else {
        function = get_or_create_function_(function_names[0], true);
        std::vector<std::string> rest_names(function_names.begin() + 1,
                                            function_names.end());
        return function->add_function(rest_names, signature);
    }
}

Function* Scope::get_or_create_function_(const std::string& function_name,
                                         bool create) {
    auto iter = functions_.find(function_name);
    if (iter != functions_.end()) {
        return iter->second;
    } else if (create) {
        Function* function = new Function(function_name);
        auto result = functions_.insert({function_name, function});
        return result.first->second;
    } else {
        Rf_error("cannot find signature of function '%s' in '%s'",
                 function_name.c_str(),
                 name_.c_str());
    }
    return iter->second;
}
