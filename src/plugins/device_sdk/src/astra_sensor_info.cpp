// This file is part of the Orbbec Astra SDK [https://orbbec3d.com]
// Copyright (c) 2015 Orbbec 3D
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Be excellent to each other.
#include "astra_sensor_info.hpp"

namespace astra { namespace devices {

    sensor_info::sensor_info(const sensor_type& type, const std::string& displayName)
        : type_(type), displayName_(displayName)
    {
    }

    const sensor_type& sensor_info::type() const
    {
        return type_;
    }

    const std::string& sensor_info::displayName() const
    {
        return displayName_;
    }
}}
