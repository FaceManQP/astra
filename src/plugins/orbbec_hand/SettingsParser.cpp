#include "../../Astra/vendor/cpptoml.h"
#include "HandSettings.h"

namespace astra { namespace plugins { namespace hand {

    template<typename T>
    T get_from_table(cpptoml::table& t, std::string key, T defaultValue)
    {
        if (t.contains_qualified(key))
        {
            return t.get_qualified(key)->as<T>()->get();
        }
        return defaultValue;
    }

    float get_float_from_table(cpptoml::table& t, std::string key, float defaultValue)
    {
        double value = get_from_table<double>(t, key, defaultValue);

        return static_cast<float>(value);
    }

    int get_int_from_table(cpptoml::table& t, std::string key, int defaultValue)
    {
        int64_t value = get_from_table<int64_t>(t, key, defaultValue);

        return static_cast<int>(value);
    }

    DepthUtilitySettings parse_depth_utility_settings(cpptoml::table t)
    {
        DepthUtilitySettings settings;

        settings.depthSmoothingFactor = get_float_from_table(t, "depthutility.depthSmoothingFactor", settings.depthSmoothingFactor);
        settings.velocityThresholdFactor = get_float_from_table(t, "depthutility.velocityThresholdFactor", settings.velocityThresholdFactor);
        settings.maxDepthJumpPercent = get_float_from_table(t, "depthutility.maxDepthJumpPercent", settings.maxDepthJumpPercent);
        settings.erodeSize = get_int_from_table(t, "depthutility.erodeSize", settings.erodeSize);
        settings.depthAdjustmentFactor = get_float_from_table(t, "depthutility.depthAdjustmentFactor", settings.depthAdjustmentFactor);
        settings.minDepth = get_float_from_table(t, "depthutility.minDepth", settings.minDepth);
        settings.maxDepth = get_float_from_table(t, "depthutility.maxDepth", settings.maxDepth);

        return settings;
    }

    PointProcessorSettings parse_point_processor_settings(cpptoml::table t)
    {
        PointProcessorSettings settings;

        settings.maxMatchDistLostActive = get_float_from_table(t, "pointprocessor.maxMatchDistLostActive", settings.maxMatchDistLostActive);
        settings.maxMatchDistDefault = get_float_from_table(t, "pointprocessor.maxMatchDistDefault", settings.maxMatchDistDefault);
        settings.steadyDeadBandRadius = get_float_from_table(t, "pointprocessor.steadyDeadBandRadius", settings.steadyDeadBandRadius);
        settings.maxInactiveFramesForCandidatePoints = get_int_from_table(t, "pointprocessor.maxInactiveFramesForCandidatePoints", settings.maxInactiveFramesForCandidatePoints);
        settings.maxInactiveFramesForLostPoints = get_int_from_table(t, "pointprocessor.maxInactiveFramesForLostPoints", settings.maxInactiveFramesForLostPoints);
        settings.maxInactiveFramesForActivePoints = get_int_from_table(t, "pointprocessor.maxInactiveFramesForActivePoints", settings.maxInactiveFramesForActivePoints);
        settings.pointSmoothingFactor = get_float_from_table(t, "pointprocessor.pointSmoothingFactor", settings.pointSmoothingFactor);
        settings.pointDeadBandSmoothingFactor = get_float_from_table(t, "pointprocessor.pointDeadBandSmoothingFactor", settings.pointDeadBandSmoothingFactor);
        settings.pointSmoothingDeadZone = get_float_from_table(t, "pointprocessor.pointSmoothingDeadZone", settings.pointSmoothingDeadZone);
        settings.maxFailedTestsInProbation = get_int_from_table(t, "pointprocessor.maxFailedTestsInProbation", settings.maxFailedTestsInProbation);
        settings.probationFrameCount = get_int_from_table(t, "pointprocessor.probationFrameCount", settings.probationFrameCount);
        settings.maxFailedTestsInProbationActivePoints = get_int_from_table(t, "pointprocessor.maxFailedTestsInProbationActivePoints", settings.maxFailedTestsInProbationActivePoints);
        settings.secondChanceMinDistance = get_float_from_table(t, "pointprocessor.secondChanceMinDistance", settings.secondChanceMinDistance);
        settings.mergePointDistance = get_float_from_table(t, "pointprocessor.mergePointDistance", settings.mergePointDistance);
        settings.maxHandPointUpdatesPerFrame = get_int_from_table(t, "pointprocessor.maxHandPointUpdatesPerFrame", settings.maxHandPointUpdatesPerFrame);

        return settings;
    }

    TrajectoryAnalyzerSettings parse_trajectory_analyzer_settings(cpptoml::table t)
    {
        TrajectoryAnalyzerSettings settings;

        settings.maxSteadyDelta = get_float_from_table(t, "trajectoryanalyzer.maxSteadyDelta", settings.maxSteadyDelta);
        settings.minSteadyFrames = get_int_from_table(t, "trajectoryanalyzer.minSteadyFrames", settings.minSteadyFrames);
        settings.minHeadingDist = get_int_from_table(t, "trajectoryanalyzer.minHeadingDist", settings.minHeadingDist);
        settings.deltaHeadingFactor = get_float_from_table(t, "trajectoryanalyzer.deltaHeadingFactor", settings.deltaHeadingFactor);
        settings.minHeadingDiffForInflection = get_float_from_table(t, "trajectoryanalyzer.minHeadingDiffForInflection", settings.minHeadingDiffForInflection);
        settings.maxHeadingDiffForContinuation = get_float_from_table(t, "trajectoryanalyzer.maxHeadingDiffForContinuation", settings.maxHeadingDiffForContinuation);
        settings.minWaveInflectionsForGesture = get_int_from_table(t, "trajectoryanalyzer.minWaveInflectionsForGesture", settings.minWaveInflectionsForGesture);
        settings.maxFramesBetweenInflections = get_int_from_table(t, "trajectoryanalyzer.maxFramesBetweenInflections", settings.maxFramesBetweenInflections);

        return settings;
    }

    AreaTestSettings parse_area_test_settings(cpptoml::table t)
    {
        AreaTestSettings settings;

        settings.areaBandwidth = get_float_from_table(t, "areatest.areaBandwidth", settings.areaBandwidth);
        settings.areaBandwidthDepth = get_float_from_table(t, "areatest.areaBandwidthDepth", settings.areaBandwidthDepth);
        settings.minArea = get_float_from_table(t, "areatest.minArea", settings.minArea);
        settings.maxArea = get_float_from_table(t, "areatest.maxArea", settings.maxArea);

        return settings;
    }

    CircumferenceTestSettings parse_circumference_test_settings(cpptoml::table t)
    {
        CircumferenceTestSettings settings;

        settings.foregroundRadius1 = get_float_from_table(t, "circumferencetest.foregroundRadius1", settings.foregroundRadius1);
        settings.foregroundRadius2 = get_float_from_table(t, "circumferencetest.foregroundRadius2", settings.foregroundRadius2);
        settings.foregroundRadiusMinPercent1 = get_float_from_table(t, "circumferencetest.foregroundRadiusMinPercent1", settings.foregroundRadiusMinPercent1);
        settings.foregroundRadiusMinPercent2 = get_float_from_table(t, "circumferencetest.foregroundRadiusMinPercent2", settings.foregroundRadiusMinPercent2);
        settings.foregroundRadiusMaxPercent1 = get_float_from_table(t, "circumferencetest.foregroundRadiusMaxPercent1", settings.foregroundRadiusMaxPercent1);
        settings.foregroundRadiusMaxPercent2 = get_float_from_table(t, "circumferencetest.foregroundRadiusMaxPercent2", settings.foregroundRadiusMaxPercent2);

        return settings;
    }

    NaturalEdgeTestSettings parse_natural_edge_test_settings(cpptoml::table t)
    {
        NaturalEdgeTestSettings settings;

        settings.naturalEdgeBandwidth = get_float_from_table(t, "naturaledgetest.naturalEdgeBandwidth", settings.naturalEdgeBandwidth);
        settings.minPercentNaturalEdges = get_float_from_table(t, "naturaledgetest.minPercentNaturalEdges", settings.minPercentNaturalEdges);

        return settings;
    }

    SegmentationSettings parse_segmentation_settings(cpptoml::table t)
    {
        SegmentationSettings settings;

        settings.segmentationBandwidthDepthNear = get_float_from_table(t, "segmentation.segmentationBandwidthDepthNear", settings.segmentationBandwidthDepthNear);
        settings.segmentationBandwidthDepthFar = get_float_from_table(t, "segmentation.segmentationBandwidthDepthFar", settings.segmentationBandwidthDepthFar);
        settings.maxSegmentationDist = get_float_from_table(t, "segmentation.maxSegmentationDist", settings.maxSegmentationDist);
        settings.heightScoreFactor = get_float_from_table(t, "segmentation.heightScoreFactor", settings.heightScoreFactor);
        settings.depthScoreFactor = get_float_from_table(t, "segmentation.depthScoreFactor", settings.depthScoreFactor);
        settings.targetEdgeDistance = get_float_from_table(t, "segmentation.targetEdgeDistance", settings.targetEdgeDistance);
        settings.edgeDistanceScoreFactor = get_float_from_table(t, "segmentation.edgeDistanceScoreFactor", settings.edgeDistanceScoreFactor);
        settings.pointInertiaFactor = get_float_from_table(t, "segmentation.pointInertiaFactor", settings.pointInertiaFactor);
        settings.pointInertiaRadius = get_float_from_table(t, "segmentation.pointInertiaRadius", settings.pointInertiaRadius);
        settings.maxDepthToDownscaleTestPass = get_float_from_table(t, "segmentation.maxDepthToDownscaleTestPass", settings.maxDepthToDownscaleTestPass);

        settings.areaTestSettings = parse_area_test_settings(t);
        settings.circumferenceTestSettings = parse_circumference_test_settings(t);
        settings.naturalEdgeTestSettings = parse_natural_edge_test_settings(t);

        return settings;
    }

    HandSettings parse_settings(std::string path)
    {
        HandSettings settings;

        cpptoml::table t;

        try
        {
            t = cpptoml::parse_file(path);
        }
        catch (const cpptoml::parse_exception& e)
        {
            return settings;
        }

        settings.processingSizeWidth = get_int_from_table(t, "handtracker.processingSizeWidth", settings.processingSizeWidth);
        settings.processingSizeHeight = get_int_from_table(t, "handtracker.processingSizeHeight", settings.processingSizeHeight);

        settings.depthUtilitySettings = parse_depth_utility_settings(t);
        settings.pointProcessorSettings = parse_point_processor_settings(t);
        settings.pointProcessorSettings.trajectoryAnalyzerSettings = parse_trajectory_analyzer_settings(t);
        settings.pointProcessorSettings.segmentationSettings = parse_segmentation_settings(t);

        return settings;
    }
}}}
