Scriptname Nemesis_AA_Reset hidden

Function ResetAnimGroupPriority(actor akTarget, string animGroup, string modname) global
	int groupCount = Nemesis_AA_Core.GetGroupOrder(Nemesis_AA_Core.GetGroupID(animGroup))
	int i = 0

	if groupCount == -1
		Debug.Trace("Failed to rest Nemesis AnimGroup priority (Mod: " + modname + ", Actor: " + (akTarget.GetBaseObject() as ActorBase) + ", Group: " + animGroup + ")")
		return
	endif		
	
	while(i < groupCount)
		akTarget.SetAnimationVariableInt("Nemesis_AA_Priority" + animGroup + "_" + i as string, 0)
		i += 1
	endWhile

	Debug.Trace("Reset Nemesis AnimGroup priority (Mod: " + modname + ", Actor: " + (akTarget.GetBaseObject() as ActorBase) + ", Group: " + animGroup + ")")
endFunction

bool Function CompareAnimVar(actor akTarget, string animGroup, string modname, int value, int priority, int newPriority, int oldPriority) global
	string aaPriorityGroup = "Nemesis_AA_Priority" + animGroup
	if priority >= oldPriority
		akTarget.SetAnimationVariableInt("Nemesis_AA" + animGroup, value)
		akTarget.SetAnimationVariableInt(aaPriorityGroup, newPriority)
		int groupCount = Nemesis_AA_Core.GetGroupOrder(Nemesis_AA_Core.GetGroupID(animGroup))
		Debug.Trace("Nemesis CompareAnimVar (AnimGroup: " + animGroup + ", Value: " + value + ", Priority: " + newPriority + ", Mod: " + modname + ", Group: " + groupCount)
		int nextnextPriority = newPriority
		int i = 0
		
		while(i < groupCount)			
			string num = i as string
			int data = akTarget.GetAnimationVariableInt(aaPriorityGroup + "_" + num)
			int nextPriority = data / 1000

			if data > 0				
				int newValue = data - (nextPriority * 1000)

				if nextnextPriority <= nextPriority
					akTarget.SetAnimationVariableInt("Nemesis_AA" + animGroup, newValue)
					akTarget.SetAnimationVariableInt(aaPriorityGroup, nextPriority)
					akTarget.SetAnimationVariableInt(aaPriorityGroup + "_" + num, 0)
				endif
			elseif nextPriority == oldPriority
				;akTarget.SetAnimationVariableInt(aaPriorityGroup + "_" + num, 0)				
			endif
			i += 1
		endWhile

		return true
	else
		int modID = Nemesis_AA_Core.GetModIDByGroupValue(Nemesis_AA_Core.GetGroupID(animGroup), value)
		int newValue = newPriority * 1000
		string ID = modID as string
		newValue += value
		akTarget.SetAnimationVariableInt(aaPriorityGroup + "_" + ID, newValue)
		Debug.Trace("Current running AnimGroup has higher priority than the requested AnimGroup (Mod ID: " + modID + ", Actor: " + (akTarget.GetBaseObject() as ActorBase) + ", Group: " + animGroup + ", Priority: " + priority + ", Old Priority: " + oldPriority + ", New Value: " + akTarget.GetAnimationVariableInt(aaPriorityGroup + "_" + ID) + ")")
		return false
	endif
endFunction
