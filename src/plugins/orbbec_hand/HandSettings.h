#ifndef HANDSETTINGS_H
#define HANDSETTINGS_H

#include <string>

namespace astra { namespace plugins { namespace hand {

    struct DepthUtilitySettings
    {
        float depthSmoothingFactor{ 0.05f };
        float velocityThresholdFactor{ 0.005f };
        float maxDepthJumpPercent{ 0.1f };
        int erodeSize{ 1 };
        float depthAdjustmentFactor{ 2.5f };
        float minDepth { 500.0f };
        float maxDepth { 4000.0f };
    };

    struct TrajectoryAnalyzerSettings
    {
        float maxSteadyDelta{ 5.0f };
        int minSteadyFrames{ 15 };
        int minHeadingDist{ 125 };
        float deltaHeadingFactor{ 0.5f };
        float minHeadingDiffForInflection{ 135.0f };
        float maxHeadingDiffForContinuation{ 45.0f };
        int minWaveInflectionsForGesture{ 2 };
        int maxFramesBetweenInflections{ 90 };
    };

    struct AreaTestSettings
    {
        float areaBandwidth{ 150.0f }; //mm
        float areaBandwidthDepth{ 100.0f }; //mm
        float minArea{ 3000.0f }; //mm^2
        float maxArea{ 35000.0f }; //mm^2
    };

    struct NaturalEdgeTestSettings
    {
        float naturalEdgeBandwidth{ 150.0f }; //mm
        float minPercentNaturalEdges { 0.8f }; //mm^2
    };

    struct CircumferenceTestSettings
    {
        float foregroundRadius1{ 100.0f };
        float foregroundRadius2{ 150.0f };
        float foregroundRadiusMinPercent1{ 0.08f };
        float foregroundRadiusMinPercent2{ 0.04f };
        float foregroundRadiusMaxPercent1{ 0.35f };
        float foregroundRadiusMaxPercent2{ 0.15f };
    };

    struct SegmentationSettings
    {
        AreaTestSettings areaTestSettings;
        CircumferenceTestSettings circumferenceTestSettings;
        NaturalEdgeTestSettings naturalEdgeTestSettings;

        float segmentationBandwidthDepthNear{ 500.0f };
        float segmentationBandwidthDepthFar{ 100.0f };
        float maxSegmentationDist{ 250.0f }; //mm
        float heightScoreFactor{ 0.5f };
        float depthScoreFactor{ 1000.0f };
        float targetEdgeDistance{ 40.0f }; //mm
        float edgeDistanceScoreFactor{ 100.0f };
        float pointInertiaFactor{ 40.0f };
        float pointInertiaRadius{ 40.0f }; //mm
        float maxDepthToDownscaleTestPass { 1300.0f }; //mm
    };

    struct PointProcessorSettings
    {
        TrajectoryAnalyzerSettings trajectoryAnalyzerSettings;
        SegmentationSettings segmentationSettings;

        float maxMatchDistLostActive{ 500.0f }; //mm
        float maxMatchDistDefault{ 500.0f }; //mm
        float steadyDeadBandRadius{ 75.0f }; //mm
        int maxInactiveFramesForCandidatePoints{ 60 };
        int maxInactiveFramesForLostPoints{ 15 };
        int maxInactiveFramesForActivePoints{ 480 };
        float pointSmoothingFactor{ 0.5f };
        float pointDeadBandSmoothingFactor{ 0.05f };
        float pointSmoothingDeadZone{ 50.0f }; //mm
        int maxFailedTestsInProbation{ 5 };
        int probationFrameCount{ 30 };
        int maxFailedTestsInProbationActivePoints{ 3 };
        float secondChanceMinDistance{ 100.0f };
        float mergePointDistance { 100.0f }; //mm
        int maxHandPointUpdatesPerFrame { 10 };
    };

    struct HandSettings
    {
        int processingSizeWidth{ 160 };
        int processingSizeHeight{ 120 };

        DepthUtilitySettings depthUtilitySettings;
        PointProcessorSettings pointProcessorSettings;
    };

    HandSettings parse_settings(std::string path);
}}}

#endif // HANDSETTINGS_H
