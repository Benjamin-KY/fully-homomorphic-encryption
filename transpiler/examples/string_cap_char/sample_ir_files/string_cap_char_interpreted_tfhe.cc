#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "tfhe/tfhe.h"
#include "tfhe/tfhe_io.h"
#include "transpiler/tfhe_runner.h"
#include "xls/common/status/status_macros.h"

namespace {

static constexpr char kXLSPackage[] = R"ir(
package my_package

fn my_package(st: (bits[1]), c: bits[8]) -> (bits[8], (bits[1])) {
  bit_slice.971: bits[1] = bit_slice(c, start=7, width=1, id=971)
  bit_slice.968: bits[1] = bit_slice(c, start=4, width=1, id=968)
  not.972: bits[1] = not(bit_slice.971, id=972)
  not.973: bits[1] = not(bit_slice.968, id=973)
  bit_slice.967: bits[1] = bit_slice(c, start=3, width=1, id=967)
  and.982: bits[1] = and(not.972, not.973, id=982)
  not.974: bits[1] = not(bit_slice.967, id=974)
  bit_slice.966: bits[1] = bit_slice(c, start=2, width=1, id=966)
  bit_slice.970: bits[1] = bit_slice(c, start=6, width=1, id=970)
  bit_slice.969: bits[1] = bit_slice(c, start=5, width=1, id=969)
  and.983: bits[1] = and(and.982, not.974, id=983)
  not.975: bits[1] = not(bit_slice.966, id=975)
  bit_slice.965: bits[1] = bit_slice(c, start=1, width=1, id=965)
  and.987: bits[1] = and(bit_slice.970, bit_slice.969, id=987)
  and.991: bits[1] = and(bit_slice.970, bit_slice.969, id=991)
  bit_slice.964: bits[1] = bit_slice(c, start=0, width=1, id=964)
  or.978: bits[1] = or(bit_slice.971, bit_slice.970, id=978)
  or.980: bits[1] = or(bit_slice.971, bit_slice.969, id=980)
  and.984: bits[1] = and(and.983, not.975, id=984)
  not.976: bits[1] = not(bit_slice.965, id=976)
  and.988: bits[1] = and(and.987, bit_slice.968, id=988)
  and.992: bits[1] = and(and.991, bit_slice.968, id=992)
  not.977: bits[1] = not(bit_slice.964, id=977)
  not.979: bits[1] = not(or.978, id=979)
  not.981: bits[1] = not(or.980, id=981)
  and.985: bits[1] = and(and.984, not.976, id=985)
  and.989: bits[1] = and(and.988, bit_slice.967, id=989)
  and.993: bits[1] = and(and.992, bit_slice.967, id=993)
  and.1009: bits[1] = and(not.977, not.976, id=1009)
  or.998: bits[1] = or(not.979, not.981, id=998)
  and.986: bits[1] = and(and.985, not.977, id=986)
  and.990: bits[1] = and(and.989, bit_slice.966, id=990)
  and.994: bits[1] = and(and.993, bit_slice.965, id=994)
  and.1010: bits[1] = and(and.1009, not.975, id=1010)
  tuple_index.996: bits[1] = tuple_index(st, index=0, id=996)
  or.999: bits[1] = or(or.998, and.986, id=999)
  or.1000: bits[1] = or(bit_slice.971, and.990, id=1000)
  and.995: bits[1] = and(and.994, bit_slice.964, id=995)
  and.1011: bits[1] = and(and.1010, not.974, id=1011)
  bit_slice.997: bits[1] = bit_slice(tuple_index.996, start=0, width=1, id=997)
  not.1002: bits[1] = not(or.999, id=1002)
  or.1001: bits[1] = or(or.1000, and.995, id=1001)
  and.1012: bits[1] = and(and.1011, not.973, id=1012)
  and.1004: bits[1] = and(bit_slice.997, not.1002, id=1004)
  not.1003: bits[1] = not(or.1001, id=1003)
  and.1013: bits[1] = and(and.1012, bit_slice.969, id=1013)
  not.1007: bits[1] = not(bit_slice.970, id=1007)
  and.1005: bits[1] = and(and.1004, not.1003, id=1005)
  and.1014: bits[1] = and(and.1013, not.1007, id=1014)
  not.1006: bits[1] = not(and.1005, id=1006)
  and.1015: bits[1] = and(and.1014, not.972, id=1015)
  and.1008: bits[1] = and(bit_slice.969, not.1006, id=1008)
  concat.1017: bits[1] = concat(and.1015, id=1017)
  concat.1016: bits[8] = concat(bit_slice.971, bit_slice.970, and.1008, bit_slice.968, bit_slice.967, bit_slice.966, bit_slice.965, bit_slice.964, id=1016)
  tuple.1018: (bits[1]) = tuple(concat.1017, id=1018)
  literal.960: bits[1] = literal(value=1, id=960)
  literal.961: bits[1] = literal(value=0, id=961)
  ret tuple.1019: (bits[8], (bits[1])) = tuple(concat.1016, tuple.1018, id=1019)
}

)ir";

static constexpr char kFunctionMetadata[] = R"pb(
  structs {
    as_struct {
      name {
        as_inst {
          name { name: "State" fully_qualified_name: "State" id: 5554463895888 }
        }
      }
      fields {
        name: "last_was_space_"
        type { as_bool {} }
      }
      no_tuple: false
    }
  }
  top_func_proto {
    name {
      name: "my_package"
      fully_qualified_name: "my_package"
      id: 5554464093400
    }
    return_type { as_int { width: 8 is_signed: true } }
    params {
      name: "st"
      type {
        as_inst {
          name { name: "State" fully_qualified_name: "State" id: 5554463895888 }
        }
      }
      is_reference: true
      is_const: false
    }
    params {
      name: "c"
      type { as_int { width: 8 is_signed: true } }
      is_reference: false
      is_const: false
    }
  }
)pb";

using fully_homomorphic_encryption::transpiler::TfheRunner;

}  // namespace

absl::Status my_package(LweSample* result, LweSample* st, LweSample* c,
                        const TFheGateBootstrappingCloudKeySet* bk) {
  XLS_ASSIGN_OR_RETURN(auto runner, TfheRunner::CreateFromStrings(
                                        kXLSPackage, kFunctionMetadata));
  return runner->Run(result, {{"st", st}, {"c", c}}, bk);
}
