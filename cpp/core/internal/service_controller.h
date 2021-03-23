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

#ifndef _CORE_INTERNAL_SERVICE_CONTROLLER_H_
#define _CORE_INTERNAL_SERVICE_CONTROLLER_H_

#include <cstdint>
#include <string>
#include <vector>

#include "core/internal/client_proxy.h"
#include "core/listeners.h"
#include "core/options.h"
#include "core/params.h"
#include "core/payload.h"
#include "core/status.h"

namespace location {
namespace nearby {
namespace connections {

// Interface defines the core functionality of Nearby Connections Service.
//
// In every method, ClientProxy* represents the client app which receives
// notifications from Nearby Connections service and forwards them to the app.
// ResultCallback arguments are not provided for this class, because all methods
// are called synchronously.
// The rest of arguments have the same meaning as the corresponding
// methods in the definition of location::nearby::Core API.
//
// See details here:
// cpp/core/core.h
class ServiceController {
 public:
  virtual ~ServiceController() = default;
  ServiceController() = default;
  ServiceController(const ServiceController&) = delete;
  ServiceController& operator=(const ServiceController&) = delete;

  // Stops and disables service controller.
  //
  // When service controller is stopped all API call fail early.
  // Note that all Core, ClientProxy objects referencing this service
  // controller are affected.
  virtual void Stop() = 0;

  // Starts advertising an endpoint for a local app.
  virtual Status StartAdvertising(ClientProxy* client,
                                  const std::string& service_id,
                                  const ConnectionOptions& options,
                                  const ConnectionRequestInfo& info) = 0;
  virtual void StopAdvertising(ClientProxy* client) = 0;

  virtual Status StartDiscovery(ClientProxy* client,
                                const std::string& service_id,
                                const ConnectionOptions& options,
                                const DiscoveryListener& listener) = 0;
  virtual void StopDiscovery(ClientProxy* client) = 0;

  virtual void InjectEndpoint(ClientProxy* client,
                              const std::string& service_id,
                              const OutOfBandConnectionMetadata& metadata) = 0;

  virtual Status RequestConnection(ClientProxy* client,
                                   const std::string& endpoint_id,
                                   const ConnectionRequestInfo& info,
                                   const ConnectionOptions& options) = 0;
  virtual Status AcceptConnection(ClientProxy* client,
                                  const std::string& endpoint_id,
                                  const PayloadListener& listener) = 0;
  virtual Status RejectConnection(ClientProxy* client,
                                  const std::string& endpoint_id) = 0;

  virtual void InitiateBandwidthUpgrade(ClientProxy* client,
                                        const std::string& endpoint_id) = 0;

  virtual void SendPayload(ClientProxy* client,
                           const std::vector<std::string>& endpoint_ids,
                           Payload payload) = 0;

  virtual Status CancelPayload(ClientProxy* client, Payload::Id payload_id) = 0;

  virtual void DisconnectFromEndpoint(ClientProxy* client,
                                      const std::string& endpoint_id) = 0;
};

}  // namespace connections
}  // namespace nearby
}  // namespace location

#endif  // _CORE_INTERNAL_SERVICE_CONTROLLER_H_
