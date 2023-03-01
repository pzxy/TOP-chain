// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: xrpc.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_xrpc_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_xrpc_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3012000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3012004 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_xrpc_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_xrpc_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[2]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_xrpc_2eproto;
namespace top {
class xrpc_reply;
class xrpc_replyDefaultTypeInternal;
extern xrpc_replyDefaultTypeInternal _xrpc_reply_default_instance_;
class xrpc_request;
class xrpc_requestDefaultTypeInternal;
extern xrpc_requestDefaultTypeInternal _xrpc_request_default_instance_;
}  // namespace top
PROTOBUF_NAMESPACE_OPEN
template<> ::top::xrpc_reply* Arena::CreateMaybeMessage<::top::xrpc_reply>(Arena*);
template<> ::top::xrpc_request* Arena::CreateMaybeMessage<::top::xrpc_request>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace top {

// ===================================================================

class xrpc_request PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:top.xrpc_request) */ {
 public:
  inline xrpc_request() : xrpc_request(nullptr) {};
  virtual ~xrpc_request();

  xrpc_request(const xrpc_request& from);
  xrpc_request(xrpc_request&& from) noexcept
    : xrpc_request() {
    *this = ::std::move(from);
  }

  inline xrpc_request& operator=(const xrpc_request& from) {
    CopyFrom(from);
    return *this;
  }
  inline xrpc_request& operator=(xrpc_request&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const xrpc_request& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const xrpc_request* internal_default_instance() {
    return reinterpret_cast<const xrpc_request*>(
               &_xrpc_request_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(xrpc_request& a, xrpc_request& b) {
    a.Swap(&b);
  }
  inline void Swap(xrpc_request* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(xrpc_request* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline xrpc_request* New() const final {
    return CreateMaybeMessage<xrpc_request>(nullptr);
  }

  xrpc_request* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<xrpc_request>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const xrpc_request& from);
  void MergeFrom(const xrpc_request& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(xrpc_request* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "top.xrpc_request";
  }
  protected:
  explicit xrpc_request(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_xrpc_2eproto);
    return ::descriptor_table_xrpc_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kActionFieldNumber = 1,
    kBodyFieldNumber = 2,
  };
  // string action = 1;
  void clear_action();
  const std::string& action() const;
  void set_action(const std::string& value);
  void set_action(std::string&& value);
  void set_action(const char* value);
  void set_action(const char* value, size_t size);
  std::string* mutable_action();
  std::string* release_action();
  void set_allocated_action(std::string* action);
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  std::string* unsafe_arena_release_action();
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  void unsafe_arena_set_allocated_action(
      std::string* action);
  private:
  const std::string& _internal_action() const;
  void _internal_set_action(const std::string& value);
  std::string* _internal_mutable_action();
  public:

  // string body = 2;
  void clear_body();
  const std::string& body() const;
  void set_body(const std::string& value);
  void set_body(std::string&& value);
  void set_body(const char* value);
  void set_body(const char* value, size_t size);
  std::string* mutable_body();
  std::string* release_body();
  void set_allocated_body(std::string* body);
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  std::string* unsafe_arena_release_body();
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  void unsafe_arena_set_allocated_body(
      std::string* body);
  private:
  const std::string& _internal_body() const;
  void _internal_set_body(const std::string& value);
  std::string* _internal_mutable_body();
  public:

  // @@protoc_insertion_point(class_scope:top.xrpc_request)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr action_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr body_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_xrpc_2eproto;
};
// -------------------------------------------------------------------

class xrpc_reply PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:top.xrpc_reply) */ {
 public:
  inline xrpc_reply() : xrpc_reply(nullptr) {};
  virtual ~xrpc_reply();

  xrpc_reply(const xrpc_reply& from);
  xrpc_reply(xrpc_reply&& from) noexcept
    : xrpc_reply() {
    *this = ::std::move(from);
  }

  inline xrpc_reply& operator=(const xrpc_reply& from) {
    CopyFrom(from);
    return *this;
  }
  inline xrpc_reply& operator=(xrpc_reply&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const xrpc_reply& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const xrpc_reply* internal_default_instance() {
    return reinterpret_cast<const xrpc_reply*>(
               &_xrpc_reply_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(xrpc_reply& a, xrpc_reply& b) {
    a.Swap(&b);
  }
  inline void Swap(xrpc_reply* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(xrpc_reply* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline xrpc_reply* New() const final {
    return CreateMaybeMessage<xrpc_reply>(nullptr);
  }

  xrpc_reply* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<xrpc_reply>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const xrpc_reply& from);
  void MergeFrom(const xrpc_reply& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(xrpc_reply* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "top.xrpc_reply";
  }
  protected:
  explicit xrpc_reply(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_xrpc_2eproto);
    return ::descriptor_table_xrpc_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kResultFieldNumber = 1,
    kBodyFieldNumber = 2,
  };
  // string result = 1;
  void clear_result();
  const std::string& result() const;
  void set_result(const std::string& value);
  void set_result(std::string&& value);
  void set_result(const char* value);
  void set_result(const char* value, size_t size);
  std::string* mutable_result();
  std::string* release_result();
  void set_allocated_result(std::string* result);
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  std::string* unsafe_arena_release_result();
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  void unsafe_arena_set_allocated_result(
      std::string* result);
  private:
  const std::string& _internal_result() const;
  void _internal_set_result(const std::string& value);
  std::string* _internal_mutable_result();
  public:

  // string body = 2;
  void clear_body();
  const std::string& body() const;
  void set_body(const std::string& value);
  void set_body(std::string&& value);
  void set_body(const char* value);
  void set_body(const char* value, size_t size);
  std::string* mutable_body();
  std::string* release_body();
  void set_allocated_body(std::string* body);
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  std::string* unsafe_arena_release_body();
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  void unsafe_arena_set_allocated_body(
      std::string* body);
  private:
  const std::string& _internal_body() const;
  void _internal_set_body(const std::string& value);
  std::string* _internal_mutable_body();
  public:

  // @@protoc_insertion_point(class_scope:top.xrpc_reply)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr result_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr body_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_xrpc_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// xrpc_request

// string action = 1;
inline void xrpc_request::clear_action() {
  action_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline const std::string& xrpc_request::action() const {
  // @@protoc_insertion_point(field_get:top.xrpc_request.action)
  return _internal_action();
}
inline void xrpc_request::set_action(const std::string& value) {
  _internal_set_action(value);
  // @@protoc_insertion_point(field_set:top.xrpc_request.action)
}
inline std::string* xrpc_request::mutable_action() {
  // @@protoc_insertion_point(field_mutable:top.xrpc_request.action)
  return _internal_mutable_action();
}
inline const std::string& xrpc_request::_internal_action() const {
  return action_.Get();
}
inline void xrpc_request::_internal_set_action(const std::string& value) {
  
  action_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void xrpc_request::set_action(std::string&& value) {
  
  action_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:top.xrpc_request.action)
}
inline void xrpc_request::set_action(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  action_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:top.xrpc_request.action)
}
inline void xrpc_request::set_action(const char* value,
    size_t size) {
  
  action_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:top.xrpc_request.action)
}
inline std::string* xrpc_request::_internal_mutable_action() {
  
  return action_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* xrpc_request::release_action() {
  // @@protoc_insertion_point(field_release:top.xrpc_request.action)
  return action_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void xrpc_request::set_allocated_action(std::string* action) {
  if (action != nullptr) {
    
  } else {
    
  }
  action_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), action,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:top.xrpc_request.action)
}
inline std::string* xrpc_request::unsafe_arena_release_action() {
  // @@protoc_insertion_point(field_unsafe_arena_release:top.xrpc_request.action)
  GOOGLE_DCHECK(GetArena() != nullptr);
  
  return action_.UnsafeArenaRelease(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      GetArena());
}
inline void xrpc_request::unsafe_arena_set_allocated_action(
    std::string* action) {
  GOOGLE_DCHECK(GetArena() != nullptr);
  if (action != nullptr) {
    
  } else {
    
  }
  action_.UnsafeArenaSetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      action, GetArena());
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:top.xrpc_request.action)
}

// string body = 2;
inline void xrpc_request::clear_body() {
  body_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline const std::string& xrpc_request::body() const {
  // @@protoc_insertion_point(field_get:top.xrpc_request.body)
  return _internal_body();
}
inline void xrpc_request::set_body(const std::string& value) {
  _internal_set_body(value);
  // @@protoc_insertion_point(field_set:top.xrpc_request.body)
}
inline std::string* xrpc_request::mutable_body() {
  // @@protoc_insertion_point(field_mutable:top.xrpc_request.body)
  return _internal_mutable_body();
}
inline const std::string& xrpc_request::_internal_body() const {
  return body_.Get();
}
inline void xrpc_request::_internal_set_body(const std::string& value) {
  
  body_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void xrpc_request::set_body(std::string&& value) {
  
  body_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:top.xrpc_request.body)
}
inline void xrpc_request::set_body(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  body_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:top.xrpc_request.body)
}
inline void xrpc_request::set_body(const char* value,
    size_t size) {
  
  body_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:top.xrpc_request.body)
}
inline std::string* xrpc_request::_internal_mutable_body() {
  
  return body_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* xrpc_request::release_body() {
  // @@protoc_insertion_point(field_release:top.xrpc_request.body)
  return body_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void xrpc_request::set_allocated_body(std::string* body) {
  if (body != nullptr) {
    
  } else {
    
  }
  body_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), body,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:top.xrpc_request.body)
}
inline std::string* xrpc_request::unsafe_arena_release_body() {
  // @@protoc_insertion_point(field_unsafe_arena_release:top.xrpc_request.body)
  GOOGLE_DCHECK(GetArena() != nullptr);
  
  return body_.UnsafeArenaRelease(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      GetArena());
}
inline void xrpc_request::unsafe_arena_set_allocated_body(
    std::string* body) {
  GOOGLE_DCHECK(GetArena() != nullptr);
  if (body != nullptr) {
    
  } else {
    
  }
  body_.UnsafeArenaSetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      body, GetArena());
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:top.xrpc_request.body)
}

// -------------------------------------------------------------------

// xrpc_reply

// string result = 1;
inline void xrpc_reply::clear_result() {
  result_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline const std::string& xrpc_reply::result() const {
  // @@protoc_insertion_point(field_get:top.xrpc_reply.result)
  return _internal_result();
}
inline void xrpc_reply::set_result(const std::string& value) {
  _internal_set_result(value);
  // @@protoc_insertion_point(field_set:top.xrpc_reply.result)
}
inline std::string* xrpc_reply::mutable_result() {
  // @@protoc_insertion_point(field_mutable:top.xrpc_reply.result)
  return _internal_mutable_result();
}
inline const std::string& xrpc_reply::_internal_result() const {
  return result_.Get();
}
inline void xrpc_reply::_internal_set_result(const std::string& value) {
  
  result_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void xrpc_reply::set_result(std::string&& value) {
  
  result_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:top.xrpc_reply.result)
}
inline void xrpc_reply::set_result(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  result_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:top.xrpc_reply.result)
}
inline void xrpc_reply::set_result(const char* value,
    size_t size) {
  
  result_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:top.xrpc_reply.result)
}
inline std::string* xrpc_reply::_internal_mutable_result() {
  
  return result_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* xrpc_reply::release_result() {
  // @@protoc_insertion_point(field_release:top.xrpc_reply.result)
  return result_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void xrpc_reply::set_allocated_result(std::string* result) {
  if (result != nullptr) {
    
  } else {
    
  }
  result_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), result,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:top.xrpc_reply.result)
}
inline std::string* xrpc_reply::unsafe_arena_release_result() {
  // @@protoc_insertion_point(field_unsafe_arena_release:top.xrpc_reply.result)
  GOOGLE_DCHECK(GetArena() != nullptr);
  
  return result_.UnsafeArenaRelease(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      GetArena());
}
inline void xrpc_reply::unsafe_arena_set_allocated_result(
    std::string* result) {
  GOOGLE_DCHECK(GetArena() != nullptr);
  if (result != nullptr) {
    
  } else {
    
  }
  result_.UnsafeArenaSetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      result, GetArena());
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:top.xrpc_reply.result)
}

// string body = 2;
inline void xrpc_reply::clear_body() {
  body_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline const std::string& xrpc_reply::body() const {
  // @@protoc_insertion_point(field_get:top.xrpc_reply.body)
  return _internal_body();
}
inline void xrpc_reply::set_body(const std::string& value) {
  _internal_set_body(value);
  // @@protoc_insertion_point(field_set:top.xrpc_reply.body)
}
inline std::string* xrpc_reply::mutable_body() {
  // @@protoc_insertion_point(field_mutable:top.xrpc_reply.body)
  return _internal_mutable_body();
}
inline const std::string& xrpc_reply::_internal_body() const {
  return body_.Get();
}
inline void xrpc_reply::_internal_set_body(const std::string& value) {
  
  body_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void xrpc_reply::set_body(std::string&& value) {
  
  body_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:top.xrpc_reply.body)
}
inline void xrpc_reply::set_body(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  body_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:top.xrpc_reply.body)
}
inline void xrpc_reply::set_body(const char* value,
    size_t size) {
  
  body_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:top.xrpc_reply.body)
}
inline std::string* xrpc_reply::_internal_mutable_body() {
  
  return body_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* xrpc_reply::release_body() {
  // @@protoc_insertion_point(field_release:top.xrpc_reply.body)
  return body_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void xrpc_reply::set_allocated_body(std::string* body) {
  if (body != nullptr) {
    
  } else {
    
  }
  body_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), body,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:top.xrpc_reply.body)
}
inline std::string* xrpc_reply::unsafe_arena_release_body() {
  // @@protoc_insertion_point(field_unsafe_arena_release:top.xrpc_reply.body)
  GOOGLE_DCHECK(GetArena() != nullptr);
  
  return body_.UnsafeArenaRelease(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      GetArena());
}
inline void xrpc_reply::unsafe_arena_set_allocated_body(
    std::string* body) {
  GOOGLE_DCHECK(GetArena() != nullptr);
  if (body != nullptr) {
    
  } else {
    
  }
  body_.UnsafeArenaSetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      body, GetArena());
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:top.xrpc_reply.body)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace top

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_xrpc_2eproto