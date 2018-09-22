Scriptname FNIS_aa Hidden

bool Function SetAnimGroup(actor akTarget, string animGroup, int animBaseNumber, int number, string modname, bool debugOutput = false) global
	return SetAnimGroupEX(akTarget, animGroup, animBaseNumber, number, modname, debugOutput, false)
endFunction

bool Function SetAnimGroupEX(actor akTarget, string animGroup, int animBaseNumber, int number, string modname, bool debugOutput = false, bool skipForce3D = false) global
	return SetAnimGroupLoad(akTarget, animGroup, animBaseNumber, number, modname, debugOutput, skipForce3D, 0, 0)
endFunction

bool Function SetAnimGroupLoad(actor akTarget, string animGroup, int animBaseNumber, int number, string modname, bool debugOutput = false, bool skipForce3D = false, int priority = 0, int newPriority = 0) global
	akTarget.SetAnimationVariableInt("Nemesis_IsLocked", akTarget.GetAnimationVariableInt("Nemesis_IsLocked") + 1)
	bool result = SetAnimGroupCore(akTarget, animGroup, animBaseNumber, number, modname, debugOutput, skipForce3D, priority, newPriority)
	akTarget.SetAnimationVariableInt("Nemesis_IsLocked", akTarget.GetAnimationVariableInt("Nemesis_IsLocked") - 1)
	return result
endFunction

; CAUTION!! DO NOT ACCESS THIS FUNCTION DIRECTLY, USE SetAnimGroupLoad INSTEAD
bool Function SetAnimGroupCore(actor akTarget, string animGroup, int animBaseNumber, int number, string modname, bool debugOutput = false, bool skipForce3D = false, int priority = 0, int newPriority = 0) global
	if (animBaseNumber >= 0 && number >= 0 && number <= 9 && akTarget != none)
		string skipText
		if akTarget == Game.GetPlayer()
			if skipForce3D
				skipText = ", skipforce3d: true"
			else
				Game.ForceThirdPerson()
			endif
		elseif !akTarget.is3dloaded()
			if debugOutput
				Debug.Trace("Target not loaded for Nemesis SetAnimGroup (Mod: " + modname + ", Actor: " + (akTarget.GetBaseObject() as ActorBase) + skipText + ")")
			endif
 			return false
		endif

		int value = animBaseNumber

		if value > 0
			value += number
		endif

		return Nemesis_AA_Reset.CompareAnimVar(akTarget, animGroup, modname, value, priority, newPriority, akTarget.GetAnimationVariableInt("Nemesis_AA_Priority" + animGroup))
	endif

	Debug.Trace("Invalid parameter for Nemesis SetAnimGroup (Mod: " + modname + ", Actor: " + (akTarget.GetBaseObject() as ActorBase) + ", Group: " + animGroup + ", AnimBase: " + animBaseNumber + ", Number: " + number + ", Priority: " + priority + ")")

	return false
endFunction

int Function GetAAmodID(string curAAPrefix, string modname, bool debugOutput = false) global
	int i = Nemesis_AA_Core.GetModID(curAAPrefix)

	if(debugOutput)
		if(i == -1)
			Debug.Trace("Mod ID not registered for Nemesis GetAAmodID. Try re-run Nemesis behavior patcher (Mod: " + modname + ", AAPrefix: " + curAAPrefix + ")")
		else
			Debug.Trace("Nemesis GetAAmodID successful (Mod: " + modname + ", AAPrefix: " + curAAprefix + ", ModID:" + i + ")")
		endif
	endif

	return i
endFunction

int Function GetGroupBaseValue(int AAmodID, int AAgroupID, string modname, bool debugOutput = false) global
	if ( AAmodID < 0 ) ||  ( AAmodID > 29 ) || ( AAgroupID < 0 ) || ( AAgroupID > 53 )
		Debug.Trace("Invalid parameter for Nemesis GetGroupBaseValue (Mod: " + modname + ", ModID: " + AAmodID + ", GroupID:" + AAgroupID + ")")
		return -1
	endif
	
	int i = Nemesis_AA_Core.GetBaseValue(AAmodID, AAgroupID)

	if(i != -1)
		return i
	endif
	
	Debug.Trace("Mod/Group ID not registered for Nemesis GetGroupBaseValue. Try re-run Nemesis behavior patcher (Mod: " + modname + ", ModID:" + AAmodID + ", GroupID:" + AAgroupID + ")")  
	return i
endFunction

int[] Function GetAllGroupBaseValues(int AAmodID, string mod, bool debugOutput = false) global
	int[] GroupBaseValue
	if ( AAmodID < 0 || AAmodID > 29 )
		Debug.Trace("Nemesis Alternate Animation GetAllGroupBaseValues invalid parameter (Mod:" + mod + ", ID: " + AAmodID + ")")
		return GroupBaseValue
	endif
	
	return Nemesis_AA_Core.GetAllBaseValue(AAmodID)
endFunction

int Function GetInstallationCRC() global
	return 182303
endFunction

Function GetAAsets(int nSets, int[] groupID, int[] modID, int[] animBase, int[] Index, string modname, bool debugOutput = false) global
	if debugOutput
		Debug.Trace("Initializing Nemesis GetAAsets (Mod: " + modname + ")")
	endif

	int[] AASet = Nemesis_AA_Core.GetAASetList(nSets)
	int i = 0
	int last = -1

	while (i < nSets)
		int DataCode = AASet[i]
		modID[i] = DataCode / 100000
		groupID[i] = ( DataCode - modID[i] * 100000 ) / 1000
		animBase[i] = ( DataCode - modID[i] * 100000 - groupID[i] * 1000 )

		if ( groupID[i] != last )
			last = groupID[i]
			Index[groupID[i]] = i + 1
		endif

		i += 1
	endWhile

	if debugOutput
		Debug.Trace("Complete Nemesis GetAAsets (Mod: " + modname + ")")
	endif
endFunction

int Function _1hmatk() global
	return 0
endFunction

int Function _1hmatkpow() global
	return 1
endFunction

int Function _1hmblock() global
	return 2
endFunction

int Function _1hmeqp() global
	return 3
endFunction

int Function _1hmidle() global
	return 4
endFunction

int Function _1hmmt() global
	return 5
endFunction

int Function _1hmstag() global
	return 6
endFunction

int Function _2hmatk() global
	return 7
endFunction

int Function _2hmatkpow() global
	return 8
endFunction

int Function _2hmblock() global
	return 9
endFunction

int Function _2hmeqp() global
	return 10
endFunction

int Function _2hmidle() global
	return 11
endFunction

int Function _2hmmt() global
	return 12
endFunction

int Function _2hmstag() global
	return 13
endFunction

int Function _2hwatk() global
	return 14
endFunction

int Function _2hwatkpow() global
	return 15
endFunction

int Function _2hwblock() global
	return 16
endFunction

int Function _2hweqp() global
	return 17
endFunction

int Function _2hwidle() global
	return 18
endFunction

int Function _2hwstag() global
	return 19
endFunction

int Function _axeeqp() global
	return 20
endFunction

int Function _bowatk() global
	return 21
endFunction

int Function _bowblock() global
	return 22
endFunction

int Function _boweqp() global
	return 23
endFunction

int Function _bowidle() global
	return 24
endFunction

int Function _bowmt() global
	return 25
endFunction

int Function _cboweqp() global
	return 26
endFunction

int Function _cbowidle() global
	return 27
endFunction

int Function _dageqp() global
	return 28
endFunction

int Function _dw() global
	return 29
endFunction

int Function _h2hatk() global
	return 30
endFunction

int Function _h2hatkpow() global
	return 31
endFunction

int Function _h2heqp() global
	return 32
endFunction

int Function _h2hidle() global
	return 33
endFunction

int Function _h2hstag() global
	return 34
endFunction

int Function _jump() global
	return 35
endFunction

int Function _maceqp() global
	return 36
endFunction

int Function _magatk() global
	return 37
endFunction

int Function _magcastmt() global
	return 38
endFunction

int Function _magcon() global
	return 39
endFunction

int Function _mageqp() global
	return 40
endFunction

int Function _magidle() global
	return 41
endFunction

int Function _magmt() global
	return 42
endFunction

int Function _mt() global
	return 43
endFunction

int Function _mtidle() global
	return 44
endFunction

int Function _mtturn() global
	return 45
endFunction

int Function _mtx() global
	return 46
endFunction

int Function _shield() global
	return 47
endFunction

int Function _shout() global
	return 48
endFunction

int Function _sneakidle() global
	return 49
endFunction

int Function _sneakmt() global
	return 50
endFunction

int Function _sprint() global
	return 51
endFunction

int Function _staffidle() global
	return 52
endFunction

int Function _stfeqp() global
	return 53
endFunction

