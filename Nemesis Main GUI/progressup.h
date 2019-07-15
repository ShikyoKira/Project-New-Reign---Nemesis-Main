#ifndef PROGRESSUP_H_
#define PROGRESSUP_H_

class NemesisMainGUI;
class BehaviorSub;

struct ProgressUp
{
private:
	NemesisMainGUI* widget;
	BehaviorSub* process;
	
public:
	void newWidget(NemesisMainGUI* _widget);
	void newProcess(BehaviorSub* _process);
	void connect();
};

#endif
