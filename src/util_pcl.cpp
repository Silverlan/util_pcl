/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "util_pcl.h"
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/features/normal_3d.h>
#include <pcl/surface/gp3.h>
#include <pcl/filters/passthrough.h>
#include <pcl/filters/project_inliers.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/surface/convex_hull.h>
#include <pcl/ModelCoefficients.h>
#include <pcl/point_types.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>

void util::pcl::build_convex_mesh(const std::vector<Vector3> &pointCloud,std::vector<Vector3> &verts,std::vector<uint32_t> &indices)
{
	::pcl::PointCloud<::pcl::PointXYZ>::Ptr cloud(new ::pcl::PointCloud<::pcl::PointXYZ>);
	for(auto &v : pointCloud)
		cloud->push_back({v.x,v.y,v.z});

	::pcl::PolygonMesh triangles;
	::pcl::ConvexHull<::pcl::PointXYZ> cHull;
	cHull.setInputCloud(cloud);
	cHull.reconstruct (triangles);

	auto cloudPtr = ::pcl::PointCloud<::pcl::PointXYZ>::Ptr(new ::pcl::PointCloud<::pcl::PointXYZ>);
	fromPCLPointCloud2(triangles.cloud,*cloudPtr);

	verts.reserve(verts.size() +cloudPtr->points.size());
	Vector3 center {};
	for(auto &p : cloudPtr->points)
	{
		verts.push_back({p.x,p.y,p.z});
		center += verts.back();
	}
	if(!verts.empty())
		center /= verts.size();

	indices.reserve(indices.size() +triangles.polygons.size() *3);
	for(auto &poly : triangles.polygons)
	{
		assert(poly.vertices.size() == 3);
		for(auto &idx : poly.vertices)
			indices.push_back(idx);
		auto polyCenter = (verts[poly.vertices[0]] +verts[poly.vertices[1]] +verts[poly.vertices[2]]) /3.f;
		auto w = uvec::dot(polyCenter -center,uvec::cross(verts[poly.vertices[1]] -verts[poly.vertices[0]],verts[poly.vertices[2]] -verts[poly.vertices[0]]));
		if(w < 0.f) // Swap order
		{
			auto n = indices.size();
			auto tmp = indices[n -1];
			indices[n -1] = indices[n -2];
			indices[n -2] = tmp;
		}
	}
}
