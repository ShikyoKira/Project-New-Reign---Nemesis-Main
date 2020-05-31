Scriptname Nemesis_PCEA_Alias extends ReferenceAlias

FormList property Nemesis_PCEA auto

Actor Property PlayerRef Auto

spell property aaaNemesisConfigurationSpell auto

event OnInit()
	SpellCondition()
endEvent

event OnPlayerLoadGame()
	SpellCondition()
endEvent

event OnRaceSwitchComplete()
	Debug.Trace("Initializing Nemesis PCEA race update")
	string[] PCEAModList = Nemesis_PCEA_Core.GetPCEAList()
	int counter = 0

	while(counter < PCEAModList.Length)
		PlayerRef.SetAnimationVariableInt(PCEAModList[counter], (Nemesis_PCEA.GetAt(counter) as globalvariable).GetValueInt())
		counter += 1
	endWhile

	Debug.Trace("Nemesis PCEA race update complete")
endEvent

function SpellCondition()
	if(SKSE.getVersionRelease() > 0)
		PlayerRef.removeSpell(aaaNemesisConfigurationSpell)
	else
		PlayerRef.addSpell(aaaNemesisConfigurationSpell, false)
	endIf
endFunction
