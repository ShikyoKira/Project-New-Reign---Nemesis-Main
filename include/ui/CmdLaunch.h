#ifndef CMDLAUNCH_H_
#define CMDLAUNCH_H_

struct NemesisInfo;

void CmdGenerateInitialize(VecStr modlist, const NemesisInfo* nemesisInfo);
void CmdUpdateInitialize(const NemesisInfo* nemesisInfo);

#endif
