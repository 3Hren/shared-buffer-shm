#pragma once

class LowLevelBufferManager;
class HighLevelBufferManager
{
    LowLevelBufferManager *manager;
public:
    HighLevelBufferManager(LowLevelBufferManager *manager);
};
