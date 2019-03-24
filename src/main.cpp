/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <iostream>

#pragma comment(lib,"sharedutils.lib")
#pragma comment(lib,"mathutil.lib")

#ifdef _DEBUG
#pragma comment(lib,"pcl_common_debug.lib")
#pragma comment(lib,"pcl_search_debug.lib")
#pragma comment(lib,"pcl_surface_debug.lib")
#pragma comment(lib,"pcl_features_debug.lib")
#pragma comment(lib,"pcl_io_debug.lib")
#pragma comment(lib,"pcl_segmentation_debug.lib")
#pragma comment(lib,"pcl_filters_debug.lib")
#else
#pragma comment(lib,"pcl_common_release.lib")
#pragma comment(lib,"pcl_search_release.lib")
#pragma comment(lib,"pcl_surface_release.lib")
#pragma comment(lib,"pcl_features_release.lib")
#pragma comment(lib,"pcl_io_release.lib")
#pragma comment(lib,"pcl_segmentation_release.lib")
#pragma comment(lib,"pcl_filters_release.lib")
#endif
/*
int main(int argc,char *argv[])
{

	return 0;
}
*/