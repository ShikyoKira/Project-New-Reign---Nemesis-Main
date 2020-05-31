#ifndef CMDLAUNCH_H_
#define CMDLAUNCH_H_

class NemesisInfo;

void CmdGenerateInitialize(VecStr modlist, const NemesisInfo* nemesisInfo);
void CmdUpdateInitialize(const NemesisInfo* nemesisInfo);

#endif
