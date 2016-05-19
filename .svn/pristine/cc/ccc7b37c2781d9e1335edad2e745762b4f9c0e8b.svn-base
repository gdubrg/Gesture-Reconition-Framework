//-----------------------------------------------------------------------------
// <copyright file="OpenCVHelper.h" company="Microsoft">
//     Copyright (c) Microsoft Corporation. All rights reserved.
// </copyright>
//-----------------------------------------------------------------------------

#pragma once

#include "resource.h"
#include <Windows.h>
#include <NuiApi.h>

// OpenCV includes
// Suppress warnings that come from compiling OpenCV code since we have no control over it
#pragma warning(push)
#pragma warning(disable : 6294 6031)
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#pragma warning(pop)

#include "OpenCVFrameHelper.h"



class OpenCVHelper
{
    // Constants:
    // Skeleton colors for each player index
    static const Scalar SKELETON_COLORS[NUI_SKELETON_COUNT];

public:
    /// <summary>
    /// Constructor
    /// </summary>
    OpenCVHelper();

    /// <summary>
    /// Sets the color image filter to the one corresponding to the given resource ID
    /// </summary>
    /// <param name="filterID">resource ID of filter to use</param>
    void SetColorFilter(int filterID);

    /// <summary>
    /// Sets the depth image filter to the one corresponding to the given resource ID
    /// </summary>
    /// <param name="filterID">resource ID of filter to use</param>
    void SetDepthFilter(int filterID);

    /// <summary>
    /// Applies the color image filter to the given Mat
    /// </summary>
    /// <param name="pImg">pointer to Mat to filter</param>
    /// <returns>S_OK if successful, an error code otherwise
    HRESULT ApplyColorFilter(Mat* pImg);

    /// <summary>
    /// Applies the depth image filter to the given Mat
    /// </summary>
    /// <param name="pImg">pointer to Mat to filter</param>
    /// <returns>S_OK if successful, an error code otherwise</returns>
    HRESULT ApplyDepthFilter(Mat* pImg);

    /// <summary>
    /// Draws the skeletons from the skeleton frame in the given color image Mat
    /// </summary>
    /// <param name="pImg">pointer to color image Mat in which to draw the skeletons</param>
    /// <param name="pSkeletons">pointer to skeleton frame to draw</param>
    /// <param name="colorRes">resolution of color image stream</param>
    /// <param name="depthRes">resolution of depth image stream</param>
    /// <returns>S_OK if successful, an error code otherwise</returns>
    HRESULT DrawSkeletonsInColorImage(Mat* pImg, NUI_SKELETON_FRAME* pSkeletons, 
        NUI_IMAGE_RESOLUTION colorResolution, NUI_IMAGE_RESOLUTION depthResolution);

    /// <summary>
    /// Draws the skeletons from the skeleton frame in the given depth image Mat
    /// </summary>
    /// <param name="pImg">pointer to depth image Mat in which to draw the skeletons</param>
    /// <param name="pSkeletons">pointer to skeleton frame to draw</param>
    /// <param name="depthRes">resolution of depth image stream</param>
    /// <returns>S_OK if successful, an error code otherwise</returns>
    HRESULT DrawSkeletonsInDepthImage(Mat* pImg, NUI_SKELETON_FRAME* pSkeletons, 
        NUI_IMAGE_RESOLUTION depthResolution);

	/// <summary>
	/// Converts a point in skeleton space to coordinates in color or depth space
	/// </summary>
	/// <param name="point">point to convert</param>
	/// <param name="pX">pointer to LONG in which to return x-coordinate</param>
	/// <param name="pY">pointer to LONG in which to return y-coorindate</param>
	/// <param name="colorRes">resolution of color image stream, or NUI_IMAGE_RESOLUTION_INVALID for conversions to depth space</param>
	/// <param name="depthRes">resolution of depth image stream</param>
	/// <returns>S_OK if successful, an error code otherwise</param>
	HRESULT GetCoordinatesForSkeletonPoint(Vector4 point, LONG* pX, LONG* pY, NUI_IMAGE_RESOLUTION colorResolution,
		NUI_IMAGE_RESOLUTION depthResolution);

private:
    // Functions:
    /// <summary>
    /// Draws the skeletons from the skeleton frame in the given Mat
    /// </summary>
    /// <param name="pImg">pointer to Mat in which to draw the skeletons</param>
    /// <param name="pSkeletons">pointer to skeleton frame to draw</param>
    /// <param name="colorRes">resolution of color image stream, or NUI_IMAGE_RESOLUTION_INVALID for a depth image</param>
    /// <param name="depthRes">resolution of depth image stream</param>
    /// <returns>S_OK if successful, an error code otherwise</returns>
    HRESULT DrawSkeletons(Mat* pImg, NUI_SKELETON_FRAME* skeletons, NUI_IMAGE_RESOLUTION colorResolution, 
        NUI_IMAGE_RESOLUTION depthResolution);

    /// <summary>
    /// Draws the specified skeleton in the given Mat
    /// </summary>
    /// <param name="pImg">pointer to Mat in which to draw the skeleton</param>
    /// <param name="pSkel">pointer to skeleton to draw</param>
    /// <param name="color">color to draw skeleton</param>
    /// <param name="colorRes">resolution of color image stream, or NUI_IMAGE_RESOLUTION_INVALID for a depth image</param>
    /// <param name="depthRes">resolution of depth image stream</param>
    /// <returns>S_OK if successful, an error code otherwise</returns>
    void DrawSkeleton(Mat* pImg, NUI_SKELETON_DATA* pSkel, Scalar color, NUI_IMAGE_RESOLUTION colorResolution, 
        NUI_IMAGE_RESOLUTION depthResolution);

    /// <summary>
    /// Draws the bone between the two joints of the skeleton in the given Mat
    /// </summary>
    /// <param name="pImg">pointer to Mat in which to draw the skeletons</param>
    /// <param name="pSkel">pointer to skeleton containing bone to draw</param>
    /// <param name="joint0">first joint of bone to draw</param>
    /// <param name="joint1">second joint of bone to draw</param>
    /// <param name="jointPositions">pixel coordinate of the skeleton's joints</param>
    /// <param name="color">color to use</param>
    void DrawBone(Mat* pImg, NUI_SKELETON_DATA* pSkel, NUI_SKELETON_POSITION_INDEX joint0, 
        NUI_SKELETON_POSITION_INDEX joint1, Point jointPositions[NUI_SKELETON_POSITION_COUNT], Scalar color);



    // Variables:
    // Resource IDs of the active filters
    int m_colorFilterID;
    int m_depthFilterID;
};