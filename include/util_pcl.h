/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __UTIL_PCL_H__
#define __UTIL_PCL_H__

#include <mathutil/uvec.h>
#include "util_pcl_definitions.h"

namespace util
{
	namespace pcl
	{
		DLLPCLUTIL void build_convex_mesh(const std::vector<Vector3> &pointCloud,std::vector<Vector3> &verts,std::vector<uint32_t> &indices);
	};
};

#endif
