#ifndef UI_CONTROLLER_H
#define UI_CONTROLLER_H

	int curentMarkedButton; 
	int curentSelectedMenu = -1; 

	void SetMarked(int _number){curentMarkedButton = _number;}
	void SetSelected(int _number){curentSelectedMenu = _number;}
#endif