#ifndef __COMPILTER_TENSORRT_SHPAE_TYPE_SPEC_H__
#define __COMPILTER_TENSORRT_SHPAE_TYPE_SPEC_H__
#include <torch/script.h>
#include <functional>
#include <string>
#include <vector>

namespace torch {
namespace blade {
const char* ScalarTypeToString(ScalarType t);
c10::optional<torch::ScalarType> ScalarTypeFromString(const std::string& dtype);

struct ShapeType {
  std::vector<int64_t> shape;
  torch::ScalarType type;
  bool operator==(const ShapeType& rhs) const {
    return shape == rhs.shape && type == rhs.type;
  }
  std::string Serialize() const;
  static ShapeType Deserialize(const std::string&);
};

class ShapeTypeSpec {
 public:
  ShapeTypeSpec(std::vector<ShapeType> shape_types);
  bool operator==(const ShapeTypeSpec& rhs) const {
    if (hash_code_ != rhs.hash_code_) {
      return false;
    }
    return shape_types_ == rhs.shape_types_;
  }
  size_t hashCode() const {
    return hash_code_;
  }

  std::string Serialize() const;
  static ShapeTypeSpec Deserialize(const std::string&);
  static ShapeTypeSpec GetShapeTypeSpec(const torch::List<torch::Tensor>&);
  static ShapeTypeSpec GetShapeTypeSpec(
      const std::vector<const torch::jit::Value*>& values);

  const std::vector<ShapeType>& shape_types() const {
    return shape_types_;
  }

 private:
  void AddShapeType(const ShapeType& shape_type);
  size_t hash_code_;
  std::vector<ShapeType> shape_types_;
};

} // namespace blade
} // namespace torch
namespace std {
template <>
struct hash<torch::blade::ShapeTypeSpec> {
  std::size_t operator()(torch::blade::ShapeTypeSpec const& s) const noexcept {
    return s.hashCode();
  }
};
} // namespace std
#endif //__COMPILTER_TENSORRT_SHPAE_TYPE_SPEC_H__