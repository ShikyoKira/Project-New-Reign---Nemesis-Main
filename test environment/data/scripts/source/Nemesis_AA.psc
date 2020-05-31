Scriptname Nemesis_AA  Hidden 

bool Function SetAnimGroupFP(actor akTarget, string animGroup, int animBaseNumber, int number, string modname, bool debugOutput = false) global
	return SetAnimGroupFPEX(akTarget, animGroup, animBaseNumber, number, modname, debugOutput, false)
endFunction

bool Function SetAnimGroupFPEX(actor akTarget, string animGroup, int animBaseNumber, int number, string modname, bool debugOutput = false, bool skipForce3D = false) global
	return SetAnimGroupFPLoad(akTarget, animGroup, animBaseNumber, number, modname, debugOutput, skipForce3D, 0, 0)
endFunction

bool Function SetAnimGroupFPLoad(actor akTarget, string animGroup, int animBaseNumber, int number, string modname, bool debugOutput = false, bool skipForce3D = false, int priority = 0, int newPriority = 0) global
	akTarget.SetAnimationVariableInt("bIsNemesisLocked", akTarget.GetAnimationVariableInt("bIsNemesisLocked") + 1)
	bool result = SetAnimGroupCore(akTarget, animGroup + "_1p*", animBaseNumber, number, modname, debugOutput, skipForce3D, priority, newPriority)
	akTarget.SetAnimationVariableInt("bIsNemesisLocked", akTarget.GetAnimationVariableInt("bIsNemesisLocked") - 1)
	return result
endFunction

bool Function SetAnimGroup(actor akTarget, string animGroup, int animBaseNumber, int number, string modname, bool debugOutput = false) global
	return SetAnimGroupEX(akTarget, animGroup, animBaseNumber, number, modname, debugOutput, false)
endFunction

bool Function SetAnimGroupEX(actor akTarget, string animGroup, int animBaseNumber, int number, string modname, bool debugOutput = false, bool skipForce3D = false) global
	return SetAnimGroupLoad(akTarget, animGroup, animBaseNumber, number, modname, debugOutput, skipForce3D, 0, 0)
endFunction

bool Function SetAnimGroupLoad(actor akTarget, string animGroup, int animBaseNumber, int number, string modname, bool debugOutput = false, bool skipForce3D = false, int priority = 0, int newPriority = 0) global
	akTarget.SetAnimationVariableInt("bIsNemesisLocked", akTarget.GetAnimationVariableInt("bIsNemesisLocked") + 1)
	bool result = SetAnimGroupCore(akTarget, animGroup, animBaseNumber, number, modname, debugOutput, skipForce3D, priority, newPriority)
	akTarget.SetAnimationVariableInt("bIsNemesisLocked", akTarget.GetAnimationVariableInt("bIsNemesisLocked") - 1)
	return result
endFunction

; CAUTION!! DO NOT ACCESS THIS FUNCTION DIRECTLY, USE SetAnimGroupLoad INSTEAD
bool Function SetAnimGroupCore(actor akTarget, string animGroup, int animBaseNumber, int number, string modname, bool debugOutput = false, bool skipForce3D = false, int priority = 0, int newPriority = 0) global
	if (animBaseNumber >= 0 && number >= 0 && number <= 9 && akTarget != none)
		string skipText
		if (akTarget == Game.GetPlayer())
			if (skipForce3D)
				skipText = ", skipforce3d: true"
			else
				Game.ForceThirdPerson()
			endif
		elseif (!akTarget.is3dloaded())
			if (debugOutput)
				Debug.Trace("Target not loaded for Nemesis SetAnimGroup (Mod: " + modname + ", Actor: " + (akTarget.GetBaseObject() as ActorBase) + skipText + ")")
			endif
 			return false
		endif

		int value = animBaseNumber

		if (value > 0)
			value += number
		endif

		return Nemesis_AA_Reset.CompareAnimVar(akTarget, animGroup, modname, value, priority, newPriority, akTarget.GetAnimationVariableInt("Nemesis_AA_Priority" + animGroup))
	endif

	Debug.Trace("Invalid parameter for Nemesis SetAnimGroup (Mod: " + modname + ", Actor: " + (akTarget.GetBaseObject() as ActorBase) + ", Group: " + animGroup + ", AnimBase: " + animBaseNumber + ", Number: " + number + ", Priority: " + priority + ")")

	return false
endFunction
