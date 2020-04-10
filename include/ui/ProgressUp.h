#ifndef PROGRESSUP_H_
#define PROGRESSUP_H_

class NemesisEngine;
class BehaviorSub;

struct ProgressUp
{
private:
    NemesisEngine* widget;
    BehaviorSub* process;

public:
    void newWidget(NemesisEngine* _widget);
    void newProcess(BehaviorSub* _process);
    void connect();
};

#endif
