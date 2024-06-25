#include <a_samp>
#include "rndwstSelection.inc"

main() {}

public OnPlayerConnect(playerid)
{
	ShowTestSelection(playerid);
	return 1;
}

stock ShowTestSelection(playerid) 
{
	new slot;
	slot = AddPlayerSelectionItem(playerid, 522, "NRG-500", "This is NRG-500 lorem ipsum dolor sit amet");
	SetPlayerSelectionItemRot(playerid, slot, 2.0, 2.0, 0.0, 3.0);
	SetPlayerSelectionVehColor(playerid, slot, 2, 2);
	SetPlayerSelectionItemAmount(playerid, slot, 2000);
	
	slot = AddPlayerSelectionItem(playerid, 560, "Sultan", "This is Sultan lorem ipsum dolor sit amet");
	SetPlayerSelectionVehColor(playerid, slot, 4, 5);
	SetPlayerSelectionItemAmount(playerid, slot, 4003);

	slot = AddPlayerSelectionItem(playerid, 562, "Elegy", "This is elegy lorem ipsum dolor sit amet");
	SetPlayerSelectionVehColor(playerid, slot, 6, 9);
	
	slot = AddPlayerSelectionItem(playerid, 970, "Tree", "TREE");
	
	slot = AddPlayerSelectionItem(playerid, 432, "TANK", "This is tank lorem ipsum dolor sit amet");
	SetPlayerSelectionVehColor(playerid, slot, 4, 5);
	SetPlayerSelectionItemAmount(playerid, slot, 4003);

	slot = AddPlayerSelectionItem(playerid, 522, "NRG-500", "This is NRG-500 lorem ipsum dolor sit amet");
	SetPlayerSelectionItemRot(playerid, slot, 2.0, 2.0, 0.0, 3.0);
	SetPlayerSelectionVehColor(playerid, slot, 2, 2);
	SetPlayerSelectionItemAmount(playerid, slot, 2000);
	
	slot = AddPlayerSelectionItem(playerid, 560, "Sultan", "This is Sultan lorem ipsum dolor sit amet");
	SetPlayerSelectionVehColor(playerid, slot, 4, 5);
	SetPlayerSelectionItemAmount(playerid, slot, 4003);

	slot = AddPlayerSelectionItem(playerid, 562, "Elegy", "This is elegy lorem ipsum dolor sit amet");
	SetPlayerSelectionVehColor(playerid, slot, 6, 9);
	
	slot = AddPlayerSelectionItem(playerid, 970, "Tree", "TREE");
	
	slot = AddPlayerSelectionItem(playerid, 432, "TANK", "This is tank lorem ipsum dolor sit amet");
	SetPlayerSelectionVehColor(playerid, slot, 4, 5);
	SetPlayerSelectionItemAmount(playerid, slot, 4003);

	
	slot = AddPlayerSelectionItem(playerid, 522, "NRG-500", "This is NRG-500 lorem ipsum dolor sit amet");
	SetPlayerSelectionItemRot(playerid, slot, 2.0, 2.0, 0.0, 3.0);
	SetPlayerSelectionVehColor(playerid, slot, 2, 2);
	SetPlayerSelectionItemAmount(playerid, slot, 2000);
	
	slot = AddPlayerSelectionItem(playerid, 560, "Sultan", "This is Sultan lorem ipsum dolor sit amet");
	SetPlayerSelectionVehColor(playerid, slot, 4, 5);
	SetPlayerSelectionItemAmount(playerid, slot, 4003);

	slot = AddPlayerSelectionItem(playerid, 562, "Elegy", "This is elegy lorem ipsum dolor sit amet");
	SetPlayerSelectionVehColor(playerid, slot, 6, 9);
	
	slot = AddPlayerSelectionItem(playerid, 970, "Tree", "TREE");
	
	slot = AddPlayerSelectionItem(playerid, 432, "TANK", "This is tank lorem ipsum dolor sit amet");
	SetPlayerSelectionVehColor(playerid, slot, 4, 5);
	SetPlayerSelectionItemAmount(playerid, slot, 4003);

	ShowPlayerSelection(playerid, Lorem, "HEADER");
	return 1;
}

Selection:Lorem(playerid, response, model, vehicle_color1, vehicle_color2)
{
	printf("%d %d %d %d %d", playerid, response, model, vehicle_color1, vehicle_color2);
	return 1;
}