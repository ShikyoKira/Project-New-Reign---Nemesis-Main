Scriptname Nemesis_PCEA_Run extends ReferenceAlias  

GlobalVariable property Nemesis_PCEA_RunOnce auto

event OnPlayerLoadGame()
	Nemesis_PCEA_RunOnce.SetValueInt(0)
endEvent
