ScriptName Nemesis_PCEA_Configuration extends activemagiceffect

FormList property Nemesis_PCEA auto

Message Property aaaNemesisPCEAMain auto

actor property PlayerRef auto

bool[] PCEA
string[] PCEAModList

event OnEffectStart(actor akTarget, actor akCaster)
	PCEAModList = new string[10]
	PCEAModList[0] = "0yukl_YY2Hander_maleonly"
	PCEAModList[1] = "1cyh0405_OrientalSwordsmanship"
	PCEAModList[2] = "2Female_Anim_Pack"
	PCEAModList[3] = ""
	PCEAModList[4] = ""
	PCEAModList[5] = ""
	PCEAModList[6] = ""
	PCEAModList[7] = ""
	PCEAModList[8] = ""
	PCEAModList[9] = ""
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

	if(button >= 3)
		Debug.MessageBox("Current slot is empty")
		MainConfiguration()
	else
		int toggle
		
		if(PCEA[button])
			toggle = 0
			Debug.notification(PCEAModList[9] + ": Activated")
		else
			toggle = 1
			Debug.notification(PCEAModList[9] + ": Deactivated")
		endif

		PCEA[button] = !PCEA[button]
		PlayerRef.SetAnimationVariableInt(PCEAModList[button], toggle)
		(Nemesis_PCEA.GetAt(button) as globalvariable).SetValueInt(toggle)
	endIf
endFunction

