#ifndef FILECHECKER_H_
#define FILECHECKER_H_

struct NemesisInfo;

bool FileCheck(bool isUpdate = false);				// Check the existence of required files
bool PCEACheck(const NemesisInfo* nemesisInfo);		// Check if PCEA is installed
void behaviorActivateMod(VecStr behaviorPriority);	// Check which behavior file will be edited by mod

#endif