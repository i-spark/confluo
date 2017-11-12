#ifndef DIALOG_TABLE_METADATA_H_
#define DIALOG_TABLE_METADATA_H_

#include <cstdint>
#include <string>
#include <fstream>

#include "schema.h"
#include "storage.h"
#include "aggregate_types.h"
#include "io_utils.h"
#include "types/numeric.h"
#include "types/type_manager.h"

using namespace utils;

namespace dialog {

enum metadata_type
  : uint32_t {
    D_SCHEMA_METADATA = 0,
  D_INDEX_METADATA = 1,
  D_FILTER_METADATA = 2,
  D_TRIGGER_METADATA = 3
};

struct index_info {
 public:
  index_info(const std::string& field_name, double bucket_size)
      : field_name_(field_name),
        bucket_size_(bucket_size) {
  }

  std::string field_name() const {
    return field_name_;
  }

  double bucket_size() const {
    return bucket_size_;
  }

 private:
  std::string field_name_;
  double bucket_size_;
};

struct filter_info {
 public:
  filter_info(const std::string& filter_name, const std::string& expr)
      : filter_name_(filter_name),
        expr_(expr) {
  }

  const std::string& filter_name() const {
    return filter_name_;
  }

  const std::string& expr() const {
    return expr_;
  }

 private:
  std::string filter_name_;
  std::string expr_;
};

struct trigger_info {
 public:
  trigger_info(const std::string& trigger_name, const std::string& filter_name,
               aggregate_id agg_id, const std::string& field_name, relop_id op,
               const numeric& threshold, uint64_t periodicity_ms)
      : trigger_id_(trigger_name),
        filter_id_(filter_name),
        agg_id_(agg_id),
        field_name_(field_name),
        op_(op),
        threshold_(threshold),
        periodicity_ms_(periodicity_ms) {
  }

  const std::string& trigger_name() const {
    return trigger_id_;
  }

  const std::string& filter_name() const {
    return filter_id_;
  }

  aggregate_id agg_id() const {
    return agg_id_;
  }

  relop_id op() const {
    return op_;
  }

  const std::string& field_name() const {
    return field_name_;
  }

  const numeric& threshold() const {
    return threshold_;
  }

  uint64_t periodicity_ms() const {
    return periodicity_ms_;
  }

 private:
  std::string trigger_id_;
  std::string filter_id_;
  aggregate_id agg_id_;
  std::string field_name_;
  relop_id op_;
  numeric threshold_;
  uint64_t periodicity_ms_;
};

class metadata_writer {
 public:
  /**
   * Constructor that initializes metadata writer
   * @param path The path of where the metadata is
   * @param id The id of the storage type 
   */
  metadata_writer(const std::string& path, storage::storage_id id)
      : filename_(path + "/metadata"),
        id_(id) {
    if (id_ != storage::storage_id::D_IN_MEMORY) {
      out_.open(filename_);
    }
  }

  void write_schema(const schema_t& schema) {
    if (id_ != storage::storage_id::D_IN_MEMORY) {
      metadata_type type = metadata_type::D_SCHEMA_METADATA;
      io_utils::write(out_, type);
      io_utils::write(out_, schema.columns().size());
      for (auto& col : schema.columns()) {
        io_utils::write(out_, col.name());
        io_utils::write(out_, col.type().id);
        io_utils::write(out_, col.type().size);
        //std::cout << col.name() << std::endl;
        //std::cout << col.type().id << std::endl;
        //std::cout << col.type().size << std::endl;

        /*if (!type_manager::is_primitive(col.type().id) &&
                type_manager::is_valid_id(col.type().id)) {
            io_utils::write(out_, col.min().as<T>());
            io_utils::write(out_, col.max().as<T>());
        }*/
        if (col.type().id != type_id::D_STRING) {
            //std::cout << "Column min: " << col.min().to_string() << std::endl;
            //std::cout << "Column max: " << col.max().to_string() << std::endl;
            data min_data = col.min().to_data();
            data max_data = col.max().to_data();
            const void* min_void = ::operator new(col.type().size);
            //void* min_void;
            //memcpy(min_void, min_data.ptr, col.type().size);
            //char* min_void = new char[col.type().size];
            min_void = const_cast<const void*>(col.min().ptr());

            const void* max_void = ::operator new(col.type().size);
            //void* max_void; 
            //memcpy(max_void, max_data.ptr, col.type().size);
            //max_void = const_cast<const void*>(col.max().ptr());
            max_void = max_data.ptr;

            data m1 = data(min_void, col.type().size);
            data m2 = data(max_void, col.type().size);

            mutable_value mut1 = mutable_value(col.type(), min_void);
            mutable_value mut2 = mutable_value(col.type(), max_void);

            //std::cout << "M1: " << mut1.to_string() << std::endl;
            //std::cout << "M2: " << mut2.to_string() << std::endl;
            //std::cout << "Column name: " << col.name() << std::endl;
            SERIALIZERS[col.type().id](out_, m1);
            SERIALIZERS[col.type().id](out_, m2);
        } 
        /*switch (col.type().id) {
          case type_id::D_BOOL: {
            io_utils::write(out_, col.min().as<bool>());
            io_utils::write(out_, col.max().as<bool>());
            break;
          }
          case type_id::D_CHAR: {
            io_utils::write(out_, col.min().as<int8_t>());
            io_utils::write(out_, col.max().as<int8_t>());
            break;
          }
          case type_id::D_SHORT: {
            io_utils::write(out_, col.min().as<int16_t>());
            io_utils::write(out_, col.max().as<int16_t>());
            break;
          }
          case type_id::D_INT: {
            io_utils::write(out_, col.min().as<int32_t>());
            io_utils::write(out_, col.max().as<int32_t>());
            break;
          }
          case type_id::D_LONG: {
            io_utils::write(out_, col.min().as<int64_t>());
            io_utils::write(out_, col.max().as<int64_t>());
            break;
          }
          case type_id::D_FLOAT: {
            io_utils::write(out_, col.min().as<float>());
            io_utils::write(out_, col.max().as<float>());
            break;
          }
          case type_id::D_DOUBLE: {
            io_utils::write(out_, col.min().as<double>());
            io_utils::write(out_, col.max().as<double>());
            break;
          }
          case type_id::D_STRING: {
            break;
          }
          default:
            THROW(invalid_operation_exception, "Invalid data type");
        }*/
      }
      io_utils::flush(out_);
    }
  }

  void write_index_info(const std::string& name, double bucket_size) {
    if (id_ != storage::storage_id::D_IN_MEMORY) {
      metadata_type type = metadata_type::D_INDEX_METADATA;
      io_utils::write(out_, type);
      io_utils::write(out_, name);
      io_utils::write(out_, bucket_size);
      io_utils::flush(out_);
    }
  }

  void write_filter_info(const std::string& filter_name,
                         const std::string& expr) {
    if (id_ != storage::storage_id::D_IN_MEMORY) {
      metadata_type type = metadata_type::D_FILTER_METADATA;
      io_utils::write(out_, type);
      io_utils::write(out_, filter_name);
      io_utils::write(out_, expr);
      io_utils::flush(out_);
    }
  }

  void write_trigger_info(const std::string& trigger_name,
                          const std::string& filter_name, aggregate_id agg_id,
                          const std::string& field_name, relop_id op,
                          const numeric& threshold,
                          const uint64_t periodicity_ms) {
    if (id_ != storage::storage_id::D_IN_MEMORY) {
      metadata_type type = metadata_type::D_TRIGGER_METADATA;
      io_utils::write(out_, type);
      io_utils::write(out_, trigger_name);
      io_utils::write(out_, filter_name);
      io_utils::write(out_, agg_id);
      io_utils::write(out_, field_name);
      io_utils::write(out_, op);
      uint16_t id = threshold.type().id;
      io_utils::write(out_, id);
      /*if (!type_manager::is_primitive(id) &&
              type_manager::is_valid_id(id)) {
          io_utils::write(out_, threshold);
      }*/
      data d1 = threshold.to_data();
      const void* threshold_ptr = d1.ptr;
      data t1 = data(threshold_ptr, threshold.type().size);
      SERIALIZERS[id](out_, t1);
      /*switch (id) {
        case type_id::D_BOOL: {
          io_utils::write(out_, threshold.as<bool>());
          break;
        }
        case type_id::D_CHAR: {
          io_utils::write(out_, threshold.as<int8_t>());
          break;
        }
        case type_id::D_SHORT: {
          io_utils::write(out_, threshold.as<int16_t>());
          break;
        }
        case type_id::D_INT: {
          io_utils::write(out_, threshold.as<int32_t>());
          break;
        }
        case type_id::D_LONG: {
          io_utils::write(out_, threshold.as<int64_t>());
          break;
        }
        case type_id::D_FLOAT: {
          io_utils::write(out_, threshold.as<float>());
          break;
        }
        case type_id::D_DOUBLE: {
          io_utils::write(out_, threshold.as<double>());
          break;
        }
        default:
          THROW(invalid_operation_exception,
                "Threshold is not of numeric type");
      }*/
      io_utils::write(out_, periodicity_ms);
      io_utils::flush(out_);
    }
  }

 private:
  std::string filename_;
  std::ofstream out_;
  storage::storage_id id_;
};

class metadata_reader {
 public:
  metadata_reader(const std::string& path)
      : filename_(path + "/metadata"),
        in_(filename_) {
  }

  metadata_type next_type() {
    return io_utils::read<metadata_type>(in_);
  }

  schema_t next_schema() {
    size_t ncolumns = io_utils::read<size_t>(in_);
    //std::cout << "Cols: " << ncolumns << std::endl;
    schema_builder builder;
    for (size_t i = 0; i < ncolumns; i++) {
      //std::cout << "Iteration: " << i << std::endl;
      std::string name = io_utils::read<std::string>(in_);
      data_type type;
      type.id = io_utils::read<type_id>(in_);
      type.size = io_utils::read<size_t>(in_);
      //std::cout << "Id: " << type.id << std::endl;
      //std::cout << "Size: " << type.size << std::endl;
      /*if (!type_manager::is_primitive(type.id) &&
              type_manager::is_valid_id(type.id)) {
          mutable_value min(io_utils::read(in_));
          mutable_value max(io_utils::read(in_));
          builder.add_column(type, name, min, max);
      }*/
      if (type.id != type_id::D_STRING) {
        data min(::operator new(type.size), type.size);
        data max(::operator new(type.size), type.size);
        DESERIALIZERS[type.id](in_, min);
        DESERIALIZERS[type.id](in_, max);
        mutable_value min_ = mutable_value(type, min);
        mutable_value max_ = mutable_value(type, max);
        //std::cout << "Min: " << min_.to_string() << std::endl;
        //std::cout << "Max: " << max_.to_string() << std::endl;
        builder.add_column(type, name, min_, max_);
      } else {
          builder.add_column(type, name);
      }

      /*switch (type.id) {
        case type_id::D_BOOL: {
          mutable_value min(io_utils::read<bool>(in_));
          mutable_value max(io_utils::read<bool>(in_));
          builder.add_column(type, name, min, max);
          break;
        }
        case type_id::D_CHAR: {
          mutable_value min(io_utils::read<int8_t>(in_));
          mutable_value max(io_utils::read<int8_t>(in_));
          builder.add_column(type, name, min, max);
          break;
        }
        case type_id::D_SHORT: {
          mutable_value min(io_utils::read<int16_t>(in_));
          mutable_value max(io_utils::read<int16_t>(in_));
          builder.add_column(type, name, min, max);
          break;
        }
        case type_id::D_INT: {
          mutable_value min(io_utils::read<int32_t>(in_));
          mutable_value max(io_utils::read<int32_t>(in_));
          builder.add_column(type, name, min, max);
          break;
        }
        case type_id::D_LONG: {
          mutable_value min(io_utils::read<int64_t>(in_));
          mutable_value max(io_utils::read<int64_t>(in_));
          builder.add_column(type, name, min, max);
          break;
        }
        case type_id::D_FLOAT: {
          mutable_value min(io_utils::read<float>(in_));
          mutable_value max(io_utils::read<float>(in_));
          builder.add_column(type, name, min, max);
          break;
        }
        case type_id::D_DOUBLE: {
          mutable_value min(io_utils::read<double>(in_));
          mutable_value max(io_utils::read<double>(in_));
          builder.add_column(type, name, min, max);
          break;
        }
        case type_id::D_STRING: {
          builder.add_column(type, name);
          break;
        }
        default:
          THROW(invalid_operation_exception, "Invalid data type");
      }*/
    }
    return schema_t(builder.get_columns());
  }

  index_info next_index_info() {
    std::string field_name = io_utils::read<std::string>(in_);
    double bucket_size = io_utils::read<double>(in_);
    return index_info(field_name, bucket_size);
  }

  filter_info next_filter_info() {
    std::string filter_name = io_utils::read<std::string>(in_);
    std::string expr = io_utils::read<std::string>(in_);
    return filter_info(filter_name, expr);
  }

  trigger_info next_trigger_info() {
    std::string trigger_name = io_utils::read<std::string>(in_);
    std::string filter_name = io_utils::read<std::string>(in_);
    aggregate_id agg_id = io_utils::read<aggregate_id>(in_);
    std::string field_name = io_utils::read<std::string>(in_);
    relop_id op = io_utils::read<relop_id>(in_);
    type_id tid = io_utils::read<type_id>(in_);
    numeric threshold;
    const data_type type = data_types[tid];
    data thresh(::operator new(type.size), type.size);
    DESERIALIZERS[tid](in_, thresh);
    threshold = numeric(data_types[tid], const_cast<void*>(thresh.ptr));
    /*switch (tid) {
      case type_id::D_BOOL: {
        threshold = io_utils::read<bool>(in_);
        break;
      }
      case type_id::D_CHAR: {
        threshold = io_utils::read<int8_t>(in_);
        break;
      }
      case type_id::D_SHORT: {
        threshold = io_utils::read<int16_t>(in_);
        break;
      }
      case type_id::D_INT: {
        threshold = io_utils::read<int32_t>(in_);
        break;
      }
      case type_id::D_LONG: {
        threshold = io_utils::read<int64_t>(in_);
        break;
      }
      case type_id::D_FLOAT: {
        threshold = io_utils::read<float>(in_);
        break;
      }
      case type_id::D_DOUBLE: {
        threshold = io_utils::read<double>(in_);
        break;
      }
      default:
        THROW(invalid_operation_exception, "Threshold is not of numeric type");
    }*/
    uint64_t periodicity_ms = io_utils::read<uint64_t>(in_);
    return trigger_info(trigger_name, filter_name, agg_id, field_name, op,
                        threshold, periodicity_ms);
  }

 private:
  std::string filename_;
  std::ifstream in_;
};

}

#endif /* DIALOG_TABLE_METADATA_H_ */
