// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#include "arrow/array/builder_union.h"

#include <cstddef>
#include <utility>

<<<<<<< HEAD
#include "arrow/buffer.h"
<<<<<<< HEAD
=======
>>>>>>> 5588-Better-support-for-building-UnionArrays
=======
#include "arrow/util/checked_cast.h"
#include "arrow/util/logging_internal.h"
=======
>>>>>>> 106ca580414f7d55261394f0155476baa894f98a
#include "arrow/util/checked_cast.h"
#include "arrow/util/logging.h"
>>>>>>> 5588-Better-support-for-building-UnionArrays

namespace arrow {

using internal::checked_cast;
<<<<<<< HEAD
using internal::checked_pointer_cast;

Status BasicUnionBuilder::FinishInternal(std::shared_ptr<ArrayData>* out) {
<<<<<<< HEAD
  std::shared_ptr<Buffer> types, null_bitmap;
  RETURN_NOT_OK(null_bitmap_builder_.Finish(&null_bitmap));
=======
  int64_t length = types_builder_.length();

  std::shared_ptr<Buffer> types;
>>>>>>> 106ca580414f7d55261394f0155476baa894f98a
  RETURN_NOT_OK(types_builder_.Finish(&types));

  std::vector<std::shared_ptr<ArrayData>> child_data(children_.size());
  for (size_t i = 0; i < children_.size(); ++i) {
    RETURN_NOT_OK(children_[i]->FinishInternal(&child_data[i]));
  }

<<<<<<< HEAD
  *out = ArrayData::Make(type(), length(), {null_bitmap, types, nullptr}, null_count_);
=======
  *out = ArrayData::Make(type(), length, {nullptr, types}, /*null_count=*/0);
>>>>>>> 106ca580414f7d55261394f0155476baa894f98a
=======

DenseUnionBuilder::DenseUnionBuilder(MemoryPool* pool)
    : ArrayBuilder(nullptr, pool), types_builder_(pool), offsets_builder_(pool) {}

DenseUnionBuilder::DenseUnionBuilder(MemoryPool* pool,
                                     std::vector<std::shared_ptr<ArrayBuilder>> children,
                                     const std::shared_ptr<DataType>& type)
    : ArrayBuilder(type, pool), types_builder_(pool), offsets_builder_(pool) {
  auto union_type = checked_cast<const UnionType*>(type.get());
  DCHECK_NE(union_type, nullptr);
  type_id_to_child_num_.resize(union_type->max_type_code() + 1, -1);
  DCHECK_EQ(union_type->mode(), UnionMode::DENSE);
  int child_i = 0;
  for (auto type_id : union_type->type_codes()) {
    type_id_to_child_num_[type_id] = child_i++;
  }
  children_ = std::move(children);
}

Status DenseUnionBuilder::FinishInternal(std::shared_ptr<ArrayData>* out) {
  std::shared_ptr<Buffer> types, offsets, null_bitmap;
  RETURN_NOT_OK(null_bitmap_builder_.Finish(&null_bitmap));
  RETURN_NOT_OK(types_builder_.Finish(&types));
  RETURN_NOT_OK(offsets_builder_.Finish(&offsets));

  std::vector<std::shared_ptr<ArrayData>> child_data(children_.size());
  for (size_t i = 0; i < children_.size(); ++i) {
    std::shared_ptr<ArrayData> data;
    RETURN_NOT_OK(children_[i]->FinishInternal(&data));
    child_data[i] = data;
  }

  // If the type has not been specified in the constructor, infer it
  if (!type_) {
    std::vector<std::shared_ptr<Field>> fields;
    std::vector<uint8_t> type_ids;
    for (size_t i = 0; i < children_.size(); ++i) {
      fields.push_back(field(field_names_[i], children_[i]->type()));
      type_ids.push_back(static_cast<uint8_t>(i));
    }
    type_ = union_(fields, type_ids, UnionMode::DENSE);
  }

  *out = ArrayData::Make(type_, length(), {null_bitmap, types, offsets}, null_count_);
>>>>>>> 5588-Better-support-for-building-UnionArrays
  (*out)->child_data = std::move(child_data);
  return Status::OK();
}

<<<<<<< HEAD
<<<<<<< HEAD
BasicUnionBuilder::BasicUnionBuilder(
    MemoryPool* pool, UnionMode::type mode,
    const std::vector<std::shared_ptr<ArrayBuilder>>& children,
    const std::shared_ptr<DataType>& type)
    : ArrayBuilder(pool),
      child_fields_(children.size()),
      mode_(mode),
      types_builder_(pool) {
  DCHECK_EQ(type->id(), Type::UNION);
  const auto& union_type = checked_cast<const UnionType&>(*type);
  DCHECK_EQ(union_type.mode(), mode);
=======
Status DenseUnionBuilder::AppendArraySlice(const ArraySpan& array, const int64_t offset,
                                           const int64_t length) {
  const int8_t* type_codes = array.GetValues<int8_t>(1);
  const int32_t* offsets = array.GetValues<int32_t>(2);
  for (int64_t row = offset; row < offset + length; row++) {
    const int8_t type_code = type_codes[row];
    const int child_id = type_id_to_child_id_[type_code];
    const int32_t union_offset = offsets[row];
    RETURN_NOT_OK(Append(type_code));
    RETURN_NOT_OK(type_id_to_children_[type_code]->AppendArraySlice(
        array.child_data[child_id], union_offset, /*length=*/1));
  }
  return Status::OK();
}

Status DenseUnionBuilder::FinishInternal(std::shared_ptr<ArrayData>* out) {
  ARROW_RETURN_NOT_OK(BasicUnionBuilder::FinishInternal(out));
  (*out)->buffers.resize(3);
  ARROW_RETURN_NOT_OK(offsets_builder_.Finish(&(*out)->buffers[2]));
  return Status::OK();
}

BasicUnionBuilder::BasicUnionBuilder(
    MemoryPool* pool, int64_t alignment,
    const std::vector<std::shared_ptr<ArrayBuilder>>& children,
    const std::shared_ptr<DataType>& type)
    : ArrayBuilder(pool, alignment),
      child_fields_(children.size()),
      types_builder_(pool, alignment) {
  const auto& union_type = checked_cast<const UnionType&>(*type);
  mode_ = union_type.mode();

>>>>>>> 106ca580414f7d55261394f0155476baa894f98a
  DCHECK_EQ(children.size(), union_type.type_codes().size());

  type_codes_ = union_type.type_codes();
  children_ = children;

<<<<<<< HEAD
  type_id_to_children_.resize(union_type.max_type_code() + 1, nullptr);
  DCHECK_LT(
      type_id_to_children_.size(),
=======
  type_id_to_child_id_.resize(union_type.max_type_code() + 1, -1);
  type_id_to_children_.resize(union_type.max_type_code() + 1, nullptr);
  DCHECK_LE(
      type_id_to_children_.size() - 1,
>>>>>>> 106ca580414f7d55261394f0155476baa894f98a
      static_cast<decltype(type_id_to_children_)::size_type>(UnionType::kMaxTypeCode));

  for (size_t i = 0; i < children.size(); ++i) {
    child_fields_[i] = union_type.field(static_cast<int>(i));

    auto type_id = union_type.type_codes()[i];
<<<<<<< HEAD
=======
    type_id_to_child_id_[type_id] = static_cast<int>(i);
>>>>>>> 106ca580414f7d55261394f0155476baa894f98a
    type_id_to_children_[type_id] = children[i].get();
  }
}

<<<<<<< HEAD
BasicUnionBuilder::BasicUnionBuilder(MemoryPool* pool, UnionMode::type mode)
    : BasicUnionBuilder(pool, mode, {}, union_(mode)) {}

int8_t BasicUnionBuilder::AppendChild(const std::shared_ptr<ArrayBuilder>& new_child,
                                      const std::string& field_name) {
  children_.push_back(new_child);

  auto new_type_id = NextTypeId();

  type_id_to_children_[new_type_id] = new_child.get();

  child_fields_.push_back(field(field_name, nullptr));

=======
int8_t BasicUnionBuilder::AppendChild(const std::shared_ptr<ArrayBuilder>& new_child,
                                      const std::string& field_name) {
  children_.push_back(new_child);
  auto new_type_id = NextTypeId();

  type_id_to_child_id_[new_type_id] = static_cast<int>(children_.size() - 1);
  type_id_to_children_[new_type_id] = new_child.get();
  child_fields_.push_back(field(field_name, nullptr));
>>>>>>> 106ca580414f7d55261394f0155476baa894f98a
  type_codes_.push_back(static_cast<int8_t>(new_type_id));

  return new_type_id;
}

std::shared_ptr<DataType> BasicUnionBuilder::type() const {
  std::vector<std::shared_ptr<Field>> child_fields(child_fields_.size());
  for (size_t i = 0; i < child_fields.size(); ++i) {
    child_fields[i] = child_fields_[i]->WithType(children_[i]->type());
  }
<<<<<<< HEAD
  return union_(std::move(child_fields), type_codes_, mode_);
=======
  return mode_ == UnionMode::SPARSE ? sparse_union(std::move(child_fields), type_codes_)
                                    : dense_union(std::move(child_fields), type_codes_);
>>>>>>> 106ca580414f7d55261394f0155476baa894f98a
}

int8_t BasicUnionBuilder::NextTypeId() {
  // Find type_id such that type_id_to_children_[type_id] == nullptr
  // and use that for the new child. Start searching at dense_type_id_
  // since type_id_to_children_ is densely packed up at least up to dense_type_id_
  for (; static_cast<size_t>(dense_type_id_) < type_id_to_children_.size();
       ++dense_type_id_) {
    if (type_id_to_children_[dense_type_id_] == nullptr) {
      return dense_type_id_++;
    }
  }

  DCHECK_LT(
      type_id_to_children_.size(),
      static_cast<decltype(type_id_to_children_)::size_type>(UnionType::kMaxTypeCode));

  // type_id_to_children_ is already densely packed, so just append the new child
<<<<<<< HEAD
  type_id_to_children_.resize(type_id_to_children_.size() + 1);
  return dense_type_id_++;
=======
  type_id_to_child_id_.resize(type_id_to_child_id_.size() + 1);
  type_id_to_children_.resize(type_id_to_children_.size() + 1);
  return dense_type_id_++;
}

Status SparseUnionBuilder::AppendArraySlice(const ArraySpan& array, const int64_t offset,
                                            const int64_t length) {
  for (size_t i = 0; i < type_codes_.size(); i++) {
    RETURN_NOT_OK(type_id_to_children_[type_codes_[i]]->AppendArraySlice(
        array.child_data[i], array.offset + offset, length));
  }
  const int8_t* type_codes = array.GetValues<int8_t>(1);
  RETURN_NOT_OK(types_builder_.Append(type_codes + offset, length));
  return Status::OK();
>>>>>>> 106ca580414f7d55261394f0155476baa894f98a
=======
SparseUnionBuilder::SparseUnionBuilder(MemoryPool* pool)
    : ArrayBuilder(nullptr, pool), types_builder_(pool) {}

SparseUnionBuilder::SparseUnionBuilder(
    MemoryPool* pool, std::vector<std::shared_ptr<ArrayBuilder>> children,
    const std::shared_ptr<DataType>& type)
    : ArrayBuilder(type, pool), types_builder_(pool) {
  auto union_type = checked_cast<const UnionType*>(type.get());
  DCHECK_NE(union_type, nullptr);
  type_id_to_child_num_.resize(union_type->max_type_code() + 1, -1);
  DCHECK_EQ(union_type->mode(), UnionMode::SPARSE);
  int child_i = 0;
  for (auto type_id : union_type->type_codes()) {
    type_id_to_child_num_[type_id] = child_i++;
  }
  children_ = std::move(children);
  for (auto&& child : children_) {
    DCHECK_EQ(child->length(), 0);
  }
}

Status SparseUnionBuilder::FinishInternal(std::shared_ptr<ArrayData>* out) {
  std::shared_ptr<Buffer> types, offsets, null_bitmap;
  RETURN_NOT_OK(null_bitmap_builder_.Finish(&null_bitmap));
  RETURN_NOT_OK(types_builder_.Finish(&types));

  std::vector<std::shared_ptr<ArrayData>> child_data(children_.size());
  for (size_t i = 0; i < children_.size(); ++i) {
    std::shared_ptr<ArrayData> data;
    RETURN_NOT_OK(children_[i]->FinishInternal(&data));
    child_data[i] = data;
  }

  // If the type has not been specified in the constructor, infer it
  if (!type_) {
    std::vector<std::shared_ptr<Field>> fields;
    std::vector<uint8_t> type_ids;
    for (size_t i = 0; i < children_.size(); ++i) {
      fields.push_back(field(field_names_[i], children_[i]->type()));
      type_ids.push_back(static_cast<uint8_t>(i));
    }
    type_ = union_(fields, type_ids, UnionMode::SPARSE);
  }

  *out = ArrayData::Make(type_, length(), {null_bitmap, types, offsets}, null_count_);
  (*out)->child_data = std::move(child_data);
  return Status::OK();
}

int8_t SparseUnionBuilder::AppendChild(const std::shared_ptr<ArrayBuilder>& child,
                                       const std::string& field_name) {
  // force type inferrence in Finish
  type_ = NULLPTR;
  DCHECK_EQ(child->length(), length_);

  children_.push_back(child);
  field_names_.push_back(field_name);
  auto child_num = static_cast<int8_t>(children_.size() - 1);
  // search for an available type_id
  // FIXME(bkietz) far from optimal
  auto max_type = static_cast<int8_t>(type_id_to_child_num_.size());
  for (int8_t type = 0; type < max_type; ++type) {
    if (type_id_to_child_num_[type] == -1) {
      type_id_to_child_num_[type] = child_num;
      return type;
    }
  }
  type_id_to_child_num_.push_back(child_num);
  return max_type;
>>>>>>> 5588-Better-support-for-building-UnionArrays
}

}  // namespace arrow
