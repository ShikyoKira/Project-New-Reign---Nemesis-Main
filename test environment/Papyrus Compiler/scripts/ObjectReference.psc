Scriptname ObjectReference extends Form Hidden

; Returns if the 3d for this object is loaded or not
bool Function Is3DLoaded() native

; Returns the base object this reference represents
Form Function GetBaseObject() native

; Get a variable from the reference's animation graph (if applicable). Bool version.
bool Function GetAnimationVariableBool(string arVariableName) native

; Get a variable from the reference's animation graph (if applicable). Int version.
int Function GetAnimationVariableInt(string arVariableName) native

; Get a variable from the reference's animation graph (if applicable). Float version.
float Function GetAnimationVariableFloat(string arVariableName) native

; Set a variable on the reference's animation graph (if applicable). Bool version.
Function SetAnimationVariableBool(string arVariableName, bool abNewValue) native

; Set a variable on the reference's animation graph (if applicable). Int version.
Function SetAnimationVariableInt(string arVariableName, int aiNewValue) native

; Set a variable on the reference's animation graph (if applicable). Float version.
Function SetAnimationVariableFloat(string arVariableName, float afNewValue) native
