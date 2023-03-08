#include "ProcessWorker.h"
#include "Common.h"
#include "ApplicationModel.h"
#include "ImageCompressor.h"

ProcessWorker::ProcessWorker()
{
}

void ProcessWorker::onStartProcessImages()
{
    INFO << "Use another thread";
    MODEL.setSuccessCount(0);
    MODEL.setFailureCount(0);
    MODEL.setIsProcessing(true);
    if (MODEL.totalFound() != 0)
    {
        INFO << "Start process" << MODEL.totalFound() << "images...";
        for (QString file : MODEL.listFiles())
        {
            ImageCompressor process(file);
            bool result = process.startProcess();
            if (result)
                MODEL.setSuccessCount(MODEL.successCount() + 1);
            else
                MODEL.setFailureCount(MODEL.failureCount() + 1);
            if (process.isTerminated())
            {
                WARN << "Process is terminated";
                return;
            }
        }
    }
    INFO << "Done.";
    MODEL.setIsProcessing(false);
}
