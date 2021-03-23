// Copyright 2020 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _CORE_INTERNAL_MEDIUMS_BLE_V2_BLE_ADVERTISEMENT_H_
#define _CORE_INTERNAL_MEDIUMS_BLE_V2_BLE_ADVERTISEMENT_H_

#include <utility>

#include "platform/base/byte_array.h"

namespace location {
namespace nearby {
namespace connections {
namespace mediums {

// Represents the format of the Mediums BLE Advertisement used in Advertising +
// Discovery.
//
// [VERSION][SOCKET_VERSION][FAST_ADVERTISEMENT_FLAG][1_RESERVED_BIT][SERVICE_ID_HASH][DATA_SIZE][DATA][DEVICE_TOKEN]
//
// For fast advertisement, we remove SERVICE_ID_HASH since we already have one
// copy in Nearby Connections(b/138447288)
// [VERSION][SOCKET_VERSION][FAST_ADVERTISEMENT_FLAG][1_RESERVED_BIT][DATA_SIZE][DATA][DEVICE_TOKEN]
//
// See go/nearby-ble-design for more information.
class BleAdvertisement {
 public:
  // Versions of the BleAdvertisement.
  enum class Version {
    kUndefined = 0,
    kV1 = 1,
    kV2 = 2,
    // Version is only allocated 3 bits in the BleAdvertisement, so this can
    // never go beyond V7.
  };

  // Versions of the BLESocket.
  enum class SocketVersion {
    kUndefined = 0,
    kV1 = 1,
    kV2 = 2,
    // SocketVersion is only allocated 3 bits in the BleAdvertisement, so this
    // can never go beyond V7.
  };

  static constexpr int kServiceIdHashLength = 3;
  static constexpr int kDeviceTokenLength = 2;

  BleAdvertisement() = default;
  BleAdvertisement(Version version, SocketVersion socket_version,
                   const ByteArray &service_id_hash, const ByteArray &data,
                   const ByteArray &device_token);
  explicit BleAdvertisement(const ByteArray &ble_advertisement_bytes);
  BleAdvertisement(const BleAdvertisement &) = default;
  BleAdvertisement &operator=(const BleAdvertisement &) = default;
  BleAdvertisement(BleAdvertisement &&) = default;
  BleAdvertisement &operator=(BleAdvertisement &&) = default;
  ~BleAdvertisement() = default;

  explicit operator ByteArray() const;
  // Operator overloads when comparing BleAdvertisement.
  bool operator==(const BleAdvertisement &rhs) const;
  bool operator<(const BleAdvertisement &rhs) const;

  bool IsValid() const { return IsSupportedVersion(version_); }
  Version GetVersion() const { return version_; }
  SocketVersion GetSocketVersion() const { return socket_version_; }
  bool IsFastAdvertisement() const { return fast_advertisement_; }
  ByteArray GetServiceIdHash() const { return service_id_hash_; }
  ByteArray &GetData() & { return data_; }
  const ByteArray &GetData() const & { return data_; }
  ByteArray &&GetData() && { return std::move(data_); }
  const ByteArray &&GetData() const && { return std::move(data_); }
  ByteArray GetDeviceToken() const { return device_token_; }

 private:
  void DoInitialize(bool fast_advertisement, Version version,
                    SocketVersion socket_version,
                    const ByteArray &service_id_hash, const ByteArray &data,
                    const ByteArray &device_token);
  bool IsSupportedVersion(Version version) const;
  bool IsSupportedSocketVersion(SocketVersion socket_version) const;
  void SerializeDataSize(bool fast_advertisement,
                         char *data_size_bytes_write_ptr,
                         size_t data_size) const;
  int ComputeAdvertisementLength(int data_length, int total_optional_length,
                                 bool fast_advertisement) const {
    // The advertisement length is the minimum length + the length of the data +
    // the length of in-use optional fields.
    return fast_advertisement ? (kMinFastAdvertisementLegth + data_length +
                                 total_optional_length)
                              : (kMinAdvertisementLength + data_length +
                                 total_optional_length);
  }

  static constexpr int kVersionLength = 1;
  static constexpr int kVersionBitmask = 0x0E0;
  static constexpr int kSocketVersionBitmask = 0x01C;
  static constexpr int kFastAdvertisementFlagBitmask = 0x002;
  static constexpr int kDataSizeLength = 4;      // Length of one int.
  static constexpr int kFastDataSizeLength = 1;  // Length of one byte.
  static constexpr int kMinAdvertisementLength =
      kVersionLength + kServiceIdHashLength + kDataSizeLength;
  // The maximum length for a Gatt characteristic value is 512 bytes, so make
  // sure the entire advertisement is less than that. The data can take up
  // whatever space is remaining after the bytes preceding it.
  static constexpr int kMaxAdvertisementLength = 512;
  static constexpr int kMinFastAdvertisementLegth =
      kVersionLength + kFastDataSizeLength;
  // The maximum length for the scan response is 31 bytes. However, with the
  // required header that comes before the service data, this leaves the
  // advertiser with 27 leftover bytes.
  static constexpr int kMaxFastAdvertisementLength = 27;

  Version version_{Version::kUndefined};
  SocketVersion socket_version_{SocketVersion::kUndefined};
  bool fast_advertisement_ = false;
  ByteArray service_id_hash_;
  ByteArray data_;
  ByteArray device_token_;
};

}  // namespace mediums
}  // namespace connections
}  // namespace nearby
}  // namespace location

#endif  // _CORE_INTERNAL_MEDIUMS_BLE_V2_BLE_ADVERTISEMENT_H_
