#ifndef FILECHECKER_H_
#define FILECHECKER_H_

bool FileCheck(bool isUpdate = false);             // Check the existence of required files
bool PCEACheck();                                  // Check if PCEA is installed
void behaviorActivateMod(vecstr behaviorPriority); // Check which behavior file will be edited by mod

#endif