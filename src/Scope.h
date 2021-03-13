#ifndef STRICTR_SCOPE_H
#define STRICTR_SCOPE_H

#include <string>
#include <vector>
#include <unordered_map>

class Function;

class Scope {
  public:
    Scope(const std::string& name): name_(name) {
    }

    ~Scope();

    const std::string& get_name() const {
        return name_;
    }

    Function* get_function(const std::string& function_name);

    Function* add_function(const std::vector<std::string>& function_names,
                           const std::vector<int>& signature);

  protected:
    std::string name_;
    bool applied_;
    std::unordered_map<std::string, Function*> functions_;

    Function* get_or_create_function_(const std::string& function_name,
                                      bool create);
};

#endif /* STRICTR_SCOPE_H */
