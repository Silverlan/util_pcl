/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __UTIL_PCL_DEFINITIONS_H__
#define __UTIL_PCL_DEFINITIONS_H__

#ifdef PCLUTIL_STATIC
	#define DLLPCLUTIL
#elif PCLUTIL_DLL
	#ifdef __linux__
		#define DLLPCLUTIL __attribute__((visibility("default")))
	#else
		#define DLLPCLUTIL __declspec(dllexport)
	#endif
#else
	#ifdef __linux__
		#define DLLPCLUTIL
	#else
		#define DLLPCLUTIL __declspec(dllimport)
	#endif
#endif

#endif
