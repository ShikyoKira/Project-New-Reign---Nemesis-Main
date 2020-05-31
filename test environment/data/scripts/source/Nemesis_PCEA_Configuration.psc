ScriptName Nemesis_PCEA_Configuration extends activemagiceffect

FormList property Nemesis_PCEA auto

Message Property aaaNemesisPCEAMain auto

GlobalVariable property Nemesis_PCEA_ID auto
GlobalVariable property Nemesis_PCEA_RunOnce auto

Actor property PlayerRef auto

bool[] PCEA
string[] PCEAModList

event OnEffectStart(actor akTarget, actor akCaster)
	PCEA = new bool[20]
	PCEAModList = Nemesis_PCEA_Core.GetPCEAList()
	int counter = 0
	bool newState = false

	if(Nemesis_PCEA_RunOnce.GetValueInt() == 0)
		int newPCEA = Nemesis_PCEA_Core.GetPCEAID()
		int pceaID = Nemesis_PCEA_ID.GetValueInt()
		Nemesis_PCEA_RunOnce.SetValueInt(1)
		Debug.Trace("Nemesis Version: " + Nemesis_PCEA_Version.GetVersion())
		Debug.Trace("Nemesis - Old PCEA ID: " + pceaID)
		Debug.Trace("Nemesis - New PCEA ID: " + newPCEA)

		while(counter < PCEAModList.Length)
			Debug.Trace("Nemesis - PCEA Mod: " + PCEAModList[counter])
			counter += 1
		endWhile

		if(pceaID != newPCEA)
			while(counter < 20)
				PCEA[counter] = false
				(Nemesis_PCEA.GetAt(counter) as globalvariable).SetValueInt(0)
				counter += 1
			endWhile
			Nemesis_PCEA_ID.SetValueInt(newPCEA)
			newState = true
		endif
	endif

	if(!newState)
		while(counter < 20)
			PCEA[counter] = (Nemesis_PCEA.GetAt(counter) as globalvariable).GetValueInt() as bool
			counter += 1
		endWhile	
	endif

	MainConfiguration()
endEvent

function MainConfiguration(int button = 0)
	button = aaaNemesisPCEAMain.show()

	if(button < PCEAModList.Length)
		int toggle
		PCEA[button] = !PCEA[button]
		
		if(PCEA[button] == true)
			toggle = 1
			Debug.notification(PCEAModList[button] + ": Deactivated")
		else
			toggle = 0
			Debug.notification(PCEAModList[button] + ": Activated")
		endif

		Debug.Trace("Nemesis PCEA - " + PCEAModList[button] + ": " + PCEA[button] as string)
		PlayerRef.SetAnimationVariableInt(PCEAModList[button], toggle)
		(Nemesis_PCEA.GetAt(button) as globalvariable).SetValueInt(toggle)
	else
		Debug.MessageBox("Current slot is empty")
		MainConfiguration()
	endIf
endFunction
