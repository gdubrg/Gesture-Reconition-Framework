//-----------------------------------------------------------------------------
// <copyright file="OpenCVHelper.cpp" company="Microsoft">
//     Copyright (c) Microsoft Corporation. All rights reserved.
// </copyright>
//-----------------------------------------------------------------------------

#include "OpenCVHelper.h"

using namespace cv;

const Scalar OpenCVHelper::SKELETON_COLORS[NUI_SKELETON_COUNT] =
{
    Scalar(255, 0, 0),      // Blue
    Scalar(0, 255, 0),      // Green
    Scalar(64, 255, 255),   // Yellow
    Scalar(255, 255, 64),   // Light blue
    Scalar(255, 64, 255),   // Purple
    Scalar(128, 128, 255)   // Pink
};

/// <summary>
/// Constructor
/// </summary>
OpenCVHelper::OpenCVHelper() :
    m_depthFilterID(-1),
    m_colorFilterID(-1)
{
}

/// <summary>
/// Sets the color image filter to the one corresponding to the given resource ID
/// </summary>
/// <param name="filterID">resource ID of filter to use</param>
void OpenCVHelper::SetColorFilter(int filterID)
{
    m_colorFilterID = filterID;
}

/// <summary>
/// Sets the depth image filter to the one corresponding to the given resource ID
/// </summary>
/// <param name="filterID">resource ID of filter to use</param>
void OpenCVHelper::SetDepthFilter(int filterID)
{
    m_depthFilterID = filterID;
}

/// <summary>
/// Applies the color image filter to the given Mat
/// </summary>
/// <param name="pImg">pointer to Mat to filter</param>
/// <returns>S_OK if successful, an error code otherwise
HRESULT OpenCVHelper::ApplyColorFilter(Mat* pImg)
{
    // Fail if pointer is invalid
    if (!pImg) 
    {
        return E_POINTER;
    }

    // Fail if Mat contains no data
    if (pImg->empty()) 
    {
        return E_INVALIDARG;
    }

    // Apply an effect based on the active filter
    switch(m_colorFilterID)
    {
    case IDM_COLOR_FILTER_GAUSSIANBLUR:
        {
            GaussianBlur(*pImg, *pImg, Size(7,7), 0);
        }
        break;
    case IDM_COLOR_FILTER_DILATE:
        {
            dilate(*pImg, *pImg, Mat());
        }
        break;
    case IDM_COLOR_FILTER_ERODE:
        {
            erode(*pImg, *pImg, Mat());
        }
        break;
    case IDM_COLOR_FILTER_CANNYEDGE:
        {
            const double minThreshold = 30.0;
            const double maxThreshold = 50.0;

            // Convert image to grayscale for edge detection
            cvtColor(*pImg, *pImg, CV_RGBA2GRAY);
            // Remove noise
            blur(*pImg, *pImg, Size(3,3));
            // Find edges in image
            Canny(*pImg, *pImg, minThreshold, maxThreshold);
            // Convert back to color for output
            cvtColor(*pImg, *pImg, CV_GRAY2RGBA);
        }
        break;
    }

    return S_OK;
}

/// <summary>
/// Applies the depth image filter to the given Mat
/// </summary>
/// <param name="pImg">pointer to Mat to filter</param>
/// <returns>S_OK if successful, an error code otherwise</returns>
HRESULT OpenCVHelper::ApplyDepthFilter(Mat* pImg)
{
    // Fail if pointer is invalid
    if (!pImg) 
    {
        return E_POINTER;
    }

    // Fail if Mat contains no data
    if (pImg->empty()) 
    {
        return E_INVALIDARG;
    }

    // Apply an effect based on the active filter
    switch(m_depthFilterID)
    {
    case IDM_DEPTH_FILTER_GAUSSIANBLUR:
        {
            GaussianBlur(*pImg, *pImg, Size(5,5), 0);
        }
        break;
    case IDM_DEPTH_FILTER_DILATE:
        {
            dilate(*pImg, *pImg, Mat());
        }
        break;
    case IDM_DEPTH_FILTER_ERODE:
        {
            erode(*pImg, *pImg, Mat());
        }
        break;
    case IDM_DEPTH_FILTER_CANNYEDGE:
        {
            const double minThreshold = 5.0;
            const double maxThreshold = 20.0;

            // Convert image to grayscale for edge detection
            cvtColor(*pImg, *pImg, CV_RGBA2GRAY);
            // Remove noise
            blur(*pImg, *pImg, Size(3,3));
            // Find edges in image
            Canny(*pImg, *pImg, minThreshold, maxThreshold);
            // Convert back to color for output
            cvtColor(*pImg, *pImg, CV_GRAY2RGBA);
        }
        break;
    }

    return S_OK;
}

/// <summary>
/// Draws the skeletons from the skeleton frame in the given color image Mat
/// </summary>
/// <param name="pImg">pointer to color image Mat in which to draw the skeletons</param>
/// <param name="pSkeletons">pointer to skeleton frame to draw</param>
/// <param name="colorRes">resolution of color image stream</param>
/// <param name="depthRes">resolution of depth image stream</param>
/// <returns>S_OK if successful, an error code otherwise</returns>
HRESULT OpenCVHelper::DrawSkeletonsInColorImage(Mat* pImg, NUI_SKELETON_FRAME* pSkeletons, 
                                                NUI_IMAGE_RESOLUTION colorResolution, NUI_IMAGE_RESOLUTION depthResolution)
{
    return DrawSkeletons(pImg, pSkeletons, colorResolution, depthResolution);
}

/// <summary>
/// Draws the skeletons from the skeleton frame in the given depth image Mat
/// </summary>
/// <param name="pImg">pointer to depth image Mat in which to draw the skeletons</param>
/// <param name="pSkeletons">pointer to skeleton frame to draw</param>
/// <param name="depthRes">resolution of depth image stream</param>
/// <returns>S_OK if successful, an error code otherwise</returns>
HRESULT OpenCVHelper::DrawSkeletonsInDepthImage(Mat* pImg, NUI_SKELETON_FRAME* pSkeletons, 
                                                NUI_IMAGE_RESOLUTION depthResolution)
{
    return DrawSkeletons(pImg, pSkeletons, NUI_IMAGE_RESOLUTION_INVALID, depthResolution);
}

/// <summary>
/// Draws the skeletons from the skeleton frame in the given Mat
/// </summary>
/// <param name="pImg">pointer to Mat in which to draw the skeletons</param>
/// <param name="pSkeletons">pointer to skeleton frame to draw</param>
/// <param name="colorRes">resolution of color image stream, or NUI_IMAGE_RESOLUTION_INVALID for a depth image</param>
/// <param name="depthRes">resolution of depth image stream</param>
/// <returns>S_OK if successful, an error code otherwise</returns>
HRESULT OpenCVHelper::DrawSkeletons(Mat* pImg, NUI_SKELETON_FRAME* pSkeletons, NUI_IMAGE_RESOLUTION colorResolution, 
                                    NUI_IMAGE_RESOLUTION depthResolution)
{
    // Fail if either pointer is invalid
    if (!pImg || !pSkeletons) 
    {
        return E_POINTER;
    }

    // Fail if Mat contains no data or has insufficient channels or if depth resolution is invalid
    if (pImg->empty() || pImg->channels() < 3 || depthResolution == NUI_IMAGE_RESOLUTION_INVALID)
    {
        return E_INVALIDARG;
    }

    // Draw each tracked skeleton
    for (int i=0; i < NUI_SKELETON_COUNT; ++i)
    {
        NUI_SKELETON_TRACKING_STATE trackingState = pSkeletons->SkeletonData[i].eTrackingState;
        if (trackingState == NUI_SKELETON_TRACKED)
        {
            // Draw entire skeleton
            NUI_SKELETON_DATA *pSkel = &(pSkeletons->SkeletonData[i]);
            DrawSkeleton(pImg, pSkel, SKELETON_COLORS[i], colorResolution, depthResolution);
        } 
        else if (trackingState == NUI_SKELETON_POSITION_INFERRED) 
        {
            // Draw a filled circle at the skeleton's inferred position
            LONG x, y;
            GetCoordinatesForSkeletonPoint(pSkeletons->SkeletonData[i].Position, &x, &y, colorResolution, depthResolution);
            circle(*pImg, Point(x, y), 7, SKELETON_COLORS[i], CV_FILLED);
        }
    }

    return S_OK;
}

/// <summary>
/// Draws the specified skeleton in the given Mat
/// </summary>
/// <param name="pImg">pointer to Mat in which to draw the skeleton</param>
/// <param name="pSkel">pointer to skeleton to draw</param>
/// <param name="color">color to draw skeleton</param>
/// <param name="colorRes">resolution of color image stream, or NUI_IMAGE_RESOLUTION_INVALID for a depth image</param>
/// <param name="depthRes">resolution of depth image stream</param>
/// <returns>S_OK if successful, an error code otherwise</returns>
void OpenCVHelper::DrawSkeleton(Mat* pImg, NUI_SKELETON_DATA* pSkel, Scalar color, NUI_IMAGE_RESOLUTION colorResolution,
                                NUI_IMAGE_RESOLUTION depthResolution)
{
    // Convert joint positions into the coordinates for this resolution and view
    Point jointPositions[NUI_SKELETON_POSITION_COUNT];

    for (int j = 0; j < NUI_SKELETON_POSITION_COUNT; ++j)
    {
        LONG x, y;
        GetCoordinatesForSkeletonPoint(pSkel->SkeletonPositions[j], &x, &y, colorResolution, depthResolution);
        jointPositions[j] = Point(x, y);
    }

    // Draw torso
    DrawBone(pImg, pSkel, NUI_SKELETON_POSITION_HEAD, NUI_SKELETON_POSITION_SHOULDER_CENTER, jointPositions, color);
    DrawBone(pImg, pSkel, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SHOULDER_LEFT, jointPositions, color);
    DrawBone(pImg, pSkel, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SHOULDER_RIGHT, jointPositions, color);
    DrawBone(pImg, pSkel, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SPINE, jointPositions, color);
    DrawBone(pImg, pSkel, NUI_SKELETON_POSITION_SPINE, NUI_SKELETON_POSITION_HIP_CENTER, jointPositions, color);
    DrawBone(pImg, pSkel, NUI_SKELETON_POSITION_HIP_CENTER, NUI_SKELETON_POSITION_HIP_LEFT, jointPositions, color);
    DrawBone(pImg, pSkel, NUI_SKELETON_POSITION_HIP_CENTER, NUI_SKELETON_POSITION_HIP_RIGHT, jointPositions, color);

    // Draw left arm
    DrawBone(pImg, pSkel, NUI_SKELETON_POSITION_SHOULDER_LEFT, NUI_SKELETON_POSITION_ELBOW_LEFT, jointPositions, color);
    DrawBone(pImg, pSkel, NUI_SKELETON_POSITION_ELBOW_LEFT, NUI_SKELETON_POSITION_WRIST_LEFT, jointPositions, color);
    DrawBone(pImg, pSkel, NUI_SKELETON_POSITION_WRIST_LEFT, NUI_SKELETON_POSITION_HAND_LEFT, jointPositions, color);

    // Draw right arm
    DrawBone(pImg, pSkel, NUI_SKELETON_POSITION_SHOULDER_RIGHT, NUI_SKELETON_POSITION_ELBOW_RIGHT, jointPositions, color);
    DrawBone(pImg, pSkel, NUI_SKELETON_POSITION_ELBOW_RIGHT, NUI_SKELETON_POSITION_WRIST_RIGHT, jointPositions, color);
    DrawBone(pImg, pSkel, NUI_SKELETON_POSITION_WRIST_RIGHT, NUI_SKELETON_POSITION_HAND_RIGHT, jointPositions, color);

    // Draw left leg
    DrawBone(pImg, pSkel, NUI_SKELETON_POSITION_HIP_LEFT, NUI_SKELETON_POSITION_KNEE_LEFT, jointPositions, color);
    DrawBone(pImg, pSkel, NUI_SKELETON_POSITION_KNEE_LEFT, NUI_SKELETON_POSITION_ANKLE_LEFT, jointPositions, color);
    DrawBone(pImg, pSkel, NUI_SKELETON_POSITION_ANKLE_LEFT, NUI_SKELETON_POSITION_FOOT_LEFT, jointPositions, color);

    // Draw right leg
    DrawBone(pImg, pSkel, NUI_SKELETON_POSITION_HIP_RIGHT, NUI_SKELETON_POSITION_KNEE_RIGHT, jointPositions, color);
    DrawBone(pImg, pSkel, NUI_SKELETON_POSITION_KNEE_RIGHT, NUI_SKELETON_POSITION_ANKLE_RIGHT, jointPositions, color);
    DrawBone(pImg, pSkel, NUI_SKELETON_POSITION_ANKLE_RIGHT, NUI_SKELETON_POSITION_FOOT_RIGHT, jointPositions, color);

    // Draw joints on top of bones
    for (int j = 0; j < NUI_SKELETON_POSITION_COUNT; ++j)
    {
        // Draw a colored circle with a black border for tracked joints
        if (pSkel->eSkeletonPositionTrackingState[j] == NUI_SKELETON_POSITION_TRACKED) 
        {
            circle(*pImg, jointPositions[j], 5, color, CV_FILLED);
            circle(*pImg, jointPositions[j], 6, Scalar(0, 0, 0), 1);
        } 
        // Draw a white, unfilled circle for inferred joints
        else if (pSkel->eSkeletonPositionTrackingState[j] == NUI_SKELETON_POSITION_INFERRED) 
        {
            circle(*pImg, jointPositions[j], 4, Scalar(255,255,255), 2);
        }
    }
}

/// <summary>
/// Draws the bone between the two joints of the skeleton in the given Mat
/// </summary>
/// <param name="pImg">pointer to Mat in which to draw the skeletons</param>
/// <param name="pSkel">pointer to skeleton containing bone to draw</param>
/// <param name="joint0">first joint of bone to draw</param>
/// <param name="joint1">second joint of bone to draw</param>
/// <param name="jointPositions">pixel coordinate of the skeleton's joints</param>
/// <param name="color">color to use</param>
void OpenCVHelper::DrawBone(Mat* pImg, NUI_SKELETON_DATA* pSkel, NUI_SKELETON_POSITION_INDEX joint0, 
                            NUI_SKELETON_POSITION_INDEX joint1, Point jointPositions[NUI_SKELETON_POSITION_COUNT], Scalar color)
{
    NUI_SKELETON_POSITION_TRACKING_STATE joint0state = pSkel->eSkeletonPositionTrackingState[joint0];
    NUI_SKELETON_POSITION_TRACKING_STATE joint1state = pSkel->eSkeletonPositionTrackingState[joint1];

    // Don't draw unless at least one joint is tracked
    if (joint0state == NUI_SKELETON_POSITION_NOT_TRACKED || joint1state == NUI_SKELETON_POSITION_NOT_TRACKED) 
    {
        return;
    }

    if (joint0state == NUI_SKELETON_POSITION_INFERRED && joint1state == NUI_SKELETON_POSITION_INFERRED) 
    {
        return;
    }

    // If both joints are tracked, draw a colored line
    if (joint0state == NUI_SKELETON_POSITION_TRACKED && joint1state == NUI_SKELETON_POSITION_TRACKED) 
    {
        line(*pImg, jointPositions[joint0], jointPositions[joint1], color, 2);
    } 
    // If only one joint is tracked, draw a thinner white line
    else 
    {
        line(*pImg, jointPositions[joint0], jointPositions[joint1], Scalar(255,255,255), 1);
    }
}

/// <summary>
/// Converts a point in skeleton space to coordinates in color or depth space
/// </summary>
/// <param name="point">point to convert</param>
/// <param name="pX">pointer to LONG in which to return x-coordinate</param>
/// <param name="pY">pointer to LONG in which to return y-coorindate</param>
/// <param name="colorRes">resolution of color image stream, or NUI_IMAGE_RESOLUTION_INVALID for conversions to depth space</param>
/// <param name="depthRes">resolution of depth image stream</param>
/// <returns>S_OK if successful, an error code otherwise</param>
HRESULT OpenCVHelper::GetCoordinatesForSkeletonPoint(Vector4 point, LONG* pX, LONG* pY, 
                                                     NUI_IMAGE_RESOLUTION colorResolution, NUI_IMAGE_RESOLUTION depthResolution)
{
    // Fail if either pointer is invalid
    if (!pX || !pY) 
    {
        return E_POINTER;
    }

    // Convert the point from skeleton space to depth space
    LONG depthX, depthY;
    USHORT depth;
    NuiTransformSkeletonToDepthImage(point, &depthX, &depthY, &depth, depthResolution);

	*pX = depthX;
	*pY = depthY;
    // If the color resolution is invalid, return these coordinates
    if (colorResolution == NUI_IMAGE_RESOLUTION_INVALID) 
    {
        *pX = depthX;
        *pY = depthY;
    } 
    // Otherwise, convert the point from depth space to color space
    else 
    {
        HRESULT hr = NuiImageGetColorPixelCoordinatesFromDepthPixelAtResolution(colorResolution, depthResolution, NULL, depthX, depthY, depth, pX, pY);
        if (FAILED(hr))
        {
            return hr;
        }
    }

    return S_OK;
}