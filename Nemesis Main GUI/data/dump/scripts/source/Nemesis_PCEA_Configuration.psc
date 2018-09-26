ScriptName Nemesis_PCEA_Configuration extends activemagiceffect

FormList property Nemesis_PCEA auto

Message Property aaaNemesisPCEAMain auto

actor property PlayerRef auto

bool[] PCEA
string[] PCEAModList

event OnEffectStart(actor akTarget, actor akCaster)
	PCEAModList = Nemesis_PCEA_Core.GetPCEAList()
	PCEA = new bool[10]
	PCEA[0] = (Nemesis_PCEA.GetAt(0) as globalvariable).GetValueInt() as bool
	PCEA[1] = (Nemesis_PCEA.GetAt(1) as globalvariable).GetValueInt() as bool
	PCEA[2] = (Nemesis_PCEA.GetAt(2) as globalvariable).GetValueInt() as bool
	PCEA[3] = (Nemesis_PCEA.GetAt(3) as globalvariable).GetValueInt() as bool
	PCEA[4] = (Nemesis_PCEA.GetAt(4) as globalvariable).GetValueInt() as bool
	PCEA[5] = (Nemesis_PCEA.GetAt(5) as globalvariable).GetValueInt() as bool
	PCEA[6] = (Nemesis_PCEA.GetAt(6) as globalvariable).GetValueInt() as bool
	PCEA[7] = (Nemesis_PCEA.GetAt(7) as globalvariable).GetValueInt() as bool
	PCEA[8] = (Nemesis_PCEA.GetAt(8) as globalvariable).GetValueInt() as bool
	PCEA[9] = (Nemesis_PCEA.GetAt(9) as globalvariable).GetValueInt() as bool
	MainConfiguration()
endEvent

function MainConfiguration(int button = 0)
	button = aaaNemesisPCEAMain.show()

	if(button < PCEAModList.Length)
		int toggle
		
		if(PCEA[button] == true)
			toggle = 0
			Debug.notification(PCEAModList[button] + ": Deactivated")
		else
			toggle = 1
			Debug.notification(PCEAModList[button] + ": Activated")
		endif

		PCEA[button] = !PCEA[button]
		PlayerRef.SetAnimationVariableInt(PCEAModList[button], toggle)
		(Nemesis_PCEA.GetAt(button) as globalvariable).SetValueInt(toggle)
	else
		Debug.MessageBox("Current slot is empty")
		MainConfiguration()
	endIf
endFunction
