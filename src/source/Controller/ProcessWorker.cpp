#include "ProcessWorker.h"
#include "Common.h"
#include "ApplicationModel.h"
#include "ImageCompressor.h"

ProcessWorker::ProcessWorker()
{
}

void ProcessWorker::onStartProcessImages()
{
    INFO << "Use another thread to compress";
    MODEL.setSuccessCount(0);
    MODEL.setFailureCount(0);
    MODEL.setIsProcessing(true);
    if (MODEL.totalFound() != 0)
    {
        INFO << "Start process" << MODEL.totalFound() << "images...";
        for (QString file : MODEL.listFiles())
        {
            ImageCompressor process(file);
            bool result = process.compress();
            if (result)
                MODEL.setSuccessCount(MODEL.successCount() + 1);
            else
                MODEL.setFailureCount(MODEL.failureCount() + 1);
            if (process.isTerminated())
            {
                WARN << "Worker has been terminated";
                return;
            }
        }
    }
    MODEL.setIsProcessing(false);
}
